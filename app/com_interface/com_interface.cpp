#include "app/com_interface/com_interface.h"
#include "boost/crc.hpp"
#include <iostream>

using namespace std;

HAL_UART_StateTypeDef debug_uart;
Uart uart;

void UartStart(UART_HandleTypeDef *huart) {
  uart.Init(huart);
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if(uart.ReceiveMessage() == HAL_OK) {
    uart.ReadMessage();
  } else {
    Error_Handler();
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
  HAL_Delay(1);
  HAL_UART_RxCpltCallback(huart);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
  uart.Init(huart);
}

void Uart::Init(UART_HandleTypeDef *huart) {
  huart_ = huart;
  HAL_UART_Receive_IT(huart_, (uint8_t*)&message, sizeof(tdPwmData));
}

HAL_StatusTypeDef Uart::ReceiveMessage() {
  HAL_UART_Receive_IT(huart_, (uint8_t*)&message, sizeof(tdPwmData));
}

void Uart::ReadMessage() {
//  boost::crc_32_type crc32;
//  crc32.process_bytes( &message.data, sizeof(message.data) );
//  uint32_t crc = crc32.checksum();
//  if(crc == message.crc) {
    osStatus_t status = osMessageQueuePut(SignalGeneratorQueueHandle, &message, 0U, 0U);
    if(status == osOK) {
//      cout << status << endl;
//      cout << data.emitter << endl;
//      cout << data.signal << endl;
//      cout << data.param << endl;
//      cout << data.value << endl;
//      cout << endl;
      ReceiveMessage();
    } else {
      Error_Handler();
//      cout << status << endl;
    }
}
