#ifndef _COM_INTERFACE_H_
#define _COM_INTERFACE_H_

#include "main.h"
#include "cmsis_os.h"
#include "app/pwm_types.h"
#include "boost/crc.hpp"

extern osMessageQueueId_t SignalGeneratorQueueHandle;

typedef enum {
  COM_STATUS_OK = 0,
  COM_STATUS_CRC_ERR,
  COM_STATUS_OS_ERR,
  COM_STATUS_ERR
} enComStatus;

typedef enum {
  MCU_STATUS_RESTARTED = 0,
  MCU_STATUS_RELAY_ON,
  MCU_STATUS_RESUME,
  MCU_STATUS_PAUSE,
  MCU_STATUS_ERR
} enMcuStatus;

struct ComAnswer {
  uint8_t com_status = COM_STATUS_OK;
  uint8_t mcu_status = MCU_STATUS_RESTARTED;
};

class ComInterface {
public:
  virtual HAL_StatusTypeDef TransmitMessage(uint16_t target_addr,
                                            uint32_t data) = 0;
  virtual HAL_StatusTypeDef WaitNextMessage() = 0;
  virtual void SetStatus(const ComAnswer& answer) = 0;

protected:
  ComInterface() = default;

  ComInterface(osMessageQueueId_t queue)
    : queue_(queue)
  {}

  virtual void ReadMessage() = 0;

  osMessageQueueId_t queue_ = SignalGeneratorQueueHandle;
  tdRpiMessage msg_{};
  ComAnswer answer_{};
};

#endif // #ifndef _COM_INTERFACE_H_