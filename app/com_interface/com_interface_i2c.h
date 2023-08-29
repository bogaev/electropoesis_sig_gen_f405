#ifndef _COM_INTERFACE_I2C_H_
#define _COM_INTERFACE_I2C_H_

#include "com_interface.h"
#include "i2c.h"

typedef enum {
  I2C_MODE_MASTER = 0,
  I2C_MODE_SLAVE
} enI2cMode;

extern "C" void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
extern "C" void I2cStart(I2C_HandleTypeDef *hi2c);

#ifdef THIS_IS_MAIN_MCU
  constexpr uint16_t _I2C_OWN_ADDR_ = 0x00AA;
  constexpr enI2cMode _I2C_MODE_ = I2C_MODE_MASTER;
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
  virtual HAL_StatusTypeDef ReceiveMessage() override;

protected:
  virtual void ReadMessage() override;

private:
  void ReInit();
  HAL_StatusTypeDef AnswerBackToMaster(const tdRpiMessage& msg);

  I2C_HandleTypeDef *hi2c_ = nullptr;
  uint16_t addr_ = 0;
  enI2cMode mode_ = I2C_MODE_SLAVE;

  static void RPI_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
  static void RPI_ErrorCallback(I2C_HandleTypeDef *hi2c);
};

extern I2cInterface* i2c;

#endif // #ifndef _COM_INTERFACE_H_