#include "com_interface.h"

#include "tim.h"
#include "app/sig_gens/sig_gens.h"
#include "boost/crc.hpp"

#include <iostream>

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
  RELAY_TRI_STATE();
  HAL_TIM_Base_Stop_IT(&htim12);
//  HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_ALL);
//  HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_ALL);
//  HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_ALL);
//  HAL_TIM_PWM_Stop_IT(&htim4, TIM_CHANNEL_ALL);
  if(uart.ReceiveMessage() == HAL_OK) {
    uart.ReadMessage();
  } else {
//    Error_Handler();
    HAL_UART_ErrorCallback(huart);
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
  ReceiveMessage();
}

HAL_StatusTypeDef Uart::ReceiveMessage() {
  return HAL_UART_Receive_IT(huart_, (uint8_t*)&message, sizeof(tdUartMessage));
}

void Uart::ReadMessage() {
//  boost::crc_32_type crc32;
//  crc32.process_bytes( &message.data, sizeof(message.data) );
//  uint32_t crc = crc32.checksum();
  //if (crc == message.crc) {
    if (!emitter_to_siggen.count(message.data.emitter)) {
//      RELAY_TRI_STATE();
//      PauseAllChannels();
      return;
    }
    osStatus_t status = osMessageQueuePut(SignalGeneratorQueueHandle, &message.data, 0U, 0U);
    if(status == osOK) {
      ReceiveMessage();
    } else {
      Error_Handler();
//      ReceiveMessage();
//      cout << status << endl;
    }
  //}
}
