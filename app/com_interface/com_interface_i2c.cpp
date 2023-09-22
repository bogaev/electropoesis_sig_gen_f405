#include "com_interface_i2c.h"

#include "tim.h"
#include "app/sig_gens/sig_gens.h"

#include <iostream>

I2cInterface* i2c = nullptr;

void I2cStart(I2C_HandleTypeDef *hi2c) {
  static I2cInterface i2c_;
  i2c = &i2c_;
  i2c->Init(hi2c, _I2C_OWN_ADDR_, _I2C_MODE_);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
  i2c->RPI_AddrCallback(hi2c, TransferDirection, AddrMatchCode);
}

void I2cInterface::RPI_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
}

void I2cInterface::RPI_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
}

void I2cInterface::RPI_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
}

void I2cInterface::RPI_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  i2c->ReadMessage();
  i2c->WaitNextMessage();
}

void I2cInterface::RPI_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
  if (TransferDirection == 1) {
    i2c->GetMessageFromMaster();
  } else {
    i2c->AnswerBackToMaster();
  }
}

void I2cInterface::RPI_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
  i2c->WaitNextMessage();
}

void I2cInterface::RPI_AbortCallback(I2C_HandleTypeDef *hi2c) {
}

void I2cInterface::RPI_ErrorCallback(I2C_HandleTypeDef *hi2c) {
  Error_Handler();
}

void I2cInterface::Init(I2C_HandleTypeDef *hi2c,
                        uint16_t addr,
                        enI2cMode mode) {
  assert(hi2c);
  assert(addr);

  hi2c_ = hi2c;
  addr_ = addr;

  if (HAL_I2C_DeInit(hi2c_) != HAL_OK) {
    Error_Handler();
  }

  // Shifting because the HAL often expects the 7-bit address to be shifted
  hi2c_->Init.OwnAddress1 = (uint32_t)addr_ << 1;

  if (HAL_I2C_Init(hi2c_) != HAL_OK) {
    Error_Handler();
  }

  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, RPI_MasterTxCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_MASTER_RX_COMPLETE_CB_ID, RPI_MasterRxCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_SLAVE_TX_COMPLETE_CB_ID, RPI_SlaveTxCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_SLAVE_RX_COMPLETE_CB_ID, RPI_SlaveRxCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_LISTEN_COMPLETE_CB_ID, RPI_ListenCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_ABORT_CB_ID, RPI_AbortCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_ERROR_CB_ID, RPI_ErrorCallback);

  if (mode == I2C_MODE_SLAVE) {
    WaitNextMessage();
  }
}

void I2cInterface::ReInit() {
  Init(hi2c_, addr_, mode_);
}

HAL_StatusTypeDef I2cInterface::TransmitMessage(uint16_t target_addr, uint32_t data) {
  return HAL_I2C_Master_Transmit_IT(hi2c_, target_addr, (uint8_t*)&msg_, MSG_TOTAL_SIZE_);
}

void I2cInterface::GetMessageFromMaster() {
  HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, (uint8_t*)&msg_, MSG_TOTAL_SIZE_, I2C_FIRST_AND_NEXT_FRAME);
}

void I2cInterface::AnswerBackToMaster() {
  HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, (uint8_t*)&answer_, MSG_ANSWER_SIZE, I2C_LAST_FRAME);
}

HAL_StatusTypeDef I2cInterface::WaitNextMessage() {
  return HAL_I2C_EnableListen_IT(&hi2c1);
}

void I2cInterface::ReadMessage() {
  ComAnswer answer{};
  boost::crc_32_type crc32{};
  crc32.reset();
  crc32.process_bytes( &msg_.data, MSG_DATA_SIZE_ );

  if (crc32.checksum() != msg_.crc) {
    answer = { COM_STATUS_CRC_ERR, MCU_STATUS_ERR };
  } else {
    answer = ReadData();
  }
  SetStatus(answer);
}

void I2cInterface::SetStatus(const ComAnswer& answer) {
  answer_ = answer;
}

ComAnswer I2cInterface::ReadData() {
  if (msg_.data.type != COM_MSG_CHECK_MCU_STATUS) {
      osStatus_t status = osMessageQueuePut(queue_, (void*)&msg_.data, 0U, 0U);
    if(status != osOK) {
      osMessageQueueReset(queue_);
      return { COM_STATUS_OS_ERR, MCU_STATUS_ERR };
    }
    return { COM_STATUS_OK, answer_.mcu_status };
  }

  if (msg_.data.type == COM_MSG_CHECK_MCU_STATUS) {
    return { COM_STATUS_OK, answer_.mcu_status };
  }

  return { COM_STATUS_ERR, MCU_STATUS_ERR };
}
