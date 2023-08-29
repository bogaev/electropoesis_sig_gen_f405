#ifndef _COM_INTERFACE_H_
#define _COM_INTERFACE_H_

#include "main.h"
#include "cmsis_os.h"
#include "app/pwm_types.h"
#include "boost/crc.hpp"

extern osMessageQueueId_t SignalGeneratorQueueHandle;

class ComInterface {
public:
  virtual HAL_StatusTypeDef TransmitMessage(uint16_t target_addr,
                                            uint32_t data) = 0;
  virtual HAL_StatusTypeDef ReceiveMessage() = 0;

protected:
  ComInterface() = default;

  ComInterface(osMessageQueueId_t queue)
    : queue_(queue)
  {}

  virtual void ReadMessage() = 0;

  osMessageQueueId_t queue_ = SignalGeneratorQueueHandle;
  boost::crc_32_type crc32_{};
  tdRpiMessage msg_{};
};

#endif // #ifndef _COM_INTERFACE_H_