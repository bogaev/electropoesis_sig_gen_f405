#ifndef _COM_INTERFACE_I2C_H_
#define _COM_INTERFACE_I2C_H_

#include "com_interface.h"
#include "i2c.h"

extern "C" void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
extern "C" void I2cStart(I2C_HandleTypeDef *hi2c);

typedef enum {
  I2C_MODE_MASTER = 0,
  I2C_MODE_SLAVE
} enI2cMode;

#ifdef THIS_IS_MAIN_MCU
  constexpr uint16_t _I2C_OWN_ADDR_ = 0x00AA;
//  constexpr enI2cMode _I2C_MODE_ = I2C_MODE_MASTER;
  constexpr enI2cMode _I2C_MODE_ = I2C_MODE_SLAVE;
#else
  constexpr uint16_t _I2C_OWN_ADDR_ = 0x0055;
  constexpr enI2cMode _I2C_MODE_ = I2C_MODE_SLAVE;
#endif

class I2cInterface : public ComInterface {
public:
  void Init(I2C_HandleTypeDef *hi2c,
            uint16_t addr,
            enI2cMode mode);

  virtual HAL_StatusTypeDef TransmitMessage(uint16_t target_addr,
                                            uint32_t data) override;
  virtual HAL_StatusTypeDef WaitNextMessage() override;
  void RPI_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);

private:
  virtual void ReadMessage() override;
  virtual void SetStatus(enComStatus com_status) override;

  void ReInit();
  void GetMessageFromMaster();
  void AnswerBackToMaster();

  I2C_HandleTypeDef *hi2c_ = nullptr;
  uint16_t addr_ = 0;
  enI2cMode mode_ = I2C_MODE_SLAVE;

  static void RPI_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_AbortCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_ErrorCallback(I2C_HandleTypeDef *hi2c);
};

extern I2cInterface* i2c;

#endif // #ifndef _COM_INTERFACE_H_