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

void I2cInterface::RPI_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
//  std::cout << "Master Tx Complete" << std::endl;
}

void I2cInterface::RPI_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
//  std::cout << "Master Rx Complete" << std::endl;
}

void I2cInterface::RPI_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
//  std::cout << "Slave Tx Complete" << std::endl;
  i2c->ReceiveMessage();
}

void I2cInterface::RPI_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
//  std::cout << "Slave Rx Complete" << std::endl;

  RELAY_TRI_STATE();
  LED_OFF(LED3_RELAY_GROUND_GPIO_Port, LED3_RELAY_GROUND_Pin);
  HAL_TIM_Base_Stop_IT(&htim12);
  if(i2c->ReceiveMessage() == HAL_OK) {
    i2c->ReadMessage();
  } else {
//    Error_Handler();
    HAL_I2C_ErrorCallback(hi2c);
  }
}

void I2cInterface::RPI_ErrorCallback(I2C_HandleTypeDef *hi2c) {
//  __HAL_UART_CLEAR_FLAG(hi2c, UART_FLAG_ORE);
  HAL_Delay(1);
//  UART_RxCpltCallback(hi2c);
  i2c->ReInit();
}

void I2cInterface::Init(I2C_HandleTypeDef *hi2c,
                        uint16_t addr,
                        enI2cMode mode) {
  assert(hi2c);
  assert(addr);

  hi2c_ = hi2c;
  addr_ = addr;

  if (HAL_I2C_DeInit(hi2c_) != HAL_OK) {
    // De-initialization Error Handling
  }

//  hi2c_->Init.OwnAddress1 = (uint32_t)addr_ << 1; // Shifting because the HAL often expects the 7-bit address to be shifted
  hi2c_->Init.OwnAddress1 = (uint32_t)addr_; // Shifting because the HAL often expects the 7-bit address to be shifted

  if (HAL_I2C_Init(hi2c_) != HAL_OK) {
    // Initialization Error Handling
  }

  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, RPI_MasterTxCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_MASTER_RX_COMPLETE_CB_ID, RPI_MasterRxCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_SLAVE_TX_COMPLETE_CB_ID, RPI_SlaveTxCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_SLAVE_RX_COMPLETE_CB_ID, RPI_SlaveRxCpltCallback);
  HAL_I2C_RegisterCallback(hi2c_, HAL_I2C_ERROR_CB_ID, RPI_ErrorCallback);
#ifdef THIS_IS_MAIN_MCU
#else
  ReceiveMessage();
#endif
}

void I2cInterface::ReInit() {
  Init(hi2c_, addr_, mode_);
}

HAL_StatusTypeDef I2cInterface::TransmitMessage(uint16_t target_addr, uint32_t data) {
  msg_.data = data;
  crc32_.reset();
  crc32_.process_bytes( &msg_.data, sizeof(uint32_t) );
  msg_.crc = crc32_.checksum();

  return HAL_I2C_Master_Transmit_IT(hi2c_, target_addr, (uint8_t*)&msg_, sizeof(tdRpiMessage));
}

HAL_StatusTypeDef I2cInterface::AnswerBackToMaster(const tdRpiMessage& msg) {
  return HAL_I2C_Slave_Transmit_IT(hi2c_, (uint8_t*)&msg_, sizeof(tdRpiMessage));
}

HAL_StatusTypeDef I2cInterface::ReceiveMessage() {
  return HAL_I2C_Slave_Receive_IT(hi2c_, (uint8_t*)&msg_, sizeof(tdRpiMessage));
}

void I2cInterface::ReadMessage() {
  crc32_.reset();
  crc32_.process_bytes( &msg_.data, sizeof(uint32_t) );

  if (crc32_.checksum() == msg_.crc) {
//    if (!emitter_to_siggen.count(msg_.data.emitter)) {
////      RELAY_TRI_STATE();
////      PauseAllChannels();
//      return;
//    }
    static uint32_t currentItemsPut = osMessageQueueGetCount(queue_);
    osStatus_t status = osMessageQueuePut(queue_, (void*)&msg_, 0U, 0U);
    currentItemsPut = osMessageQueueGetCount(queue_);

    if(status == osOK) {
      AnswerBackToMaster(msg_);
    } else {
      Error_Handler();
//      ReceiveMessage();
//      cout << status << endl;
    }
  }
}
