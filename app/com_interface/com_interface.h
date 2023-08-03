#ifndef _COM_INTERFACE_H_
#define _COM_INTERFACE_H_

#include "usart.h"
#include "cmsis_os.h"
#include "app/pwm_types.h"

extern UART_HandleTypeDef huart5;
extern CRC_HandleTypeDef hcrc;
extern osMessageQueueId_t SignalGeneratorQueueHandle;
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
extern "C" void UartStart(UART_HandleTypeDef *huart);

class Uart {
public:
  void Init(UART_HandleTypeDef *huart);
  HAL_StatusTypeDef ReceiveMessage();
  void ReadMessage();

private:
  typedef struct tdUartMessage_ {
    tdPwmData data = {0};
    uint32_t crc = 0;
  } tdUartMessage;

  UART_HandleTypeDef* huart_;
  tdUartMessage message;
};

extern Uart uart;

#endif // #ifndef _COM_INTERFACE_H_