#include "pwm_os_tasks.h"
#include "main.h"

#include "app/com_interface/com_interface.h"
#include "app/sig_gen_config.h"
#include "app/sig_gens/sig_gens.h"

#include "signal_generator/pwm_controller.h"
#include "signal_generator/sig_gen_include.h"

#ifdef SELF_MESSAGE_ON
  #define DELAY_LED 2000
#else
  #define DELAY_LED 50
#endif

void StartDefaultTask(void *argument) {
  /* Infinite loop */

  MY_SIG_GEN_Init();

  SIG_GEN_SetSignalType(&sig_gen_1, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
  SIG_GEN_SetSignalType(&sig_gen_2, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
  SIG_GEN_SetSignalType(&sig_gen_3, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
  SIG_GEN_SetSignalType(&sig_gen_4, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);

  SIG_GEN_SetFreq(&sig_gen_1, SIG_GEN_CARRIER, 111);
  SIG_GEN_SetFreq(&sig_gen_2, SIG_GEN_CARRIER, 122);
  SIG_GEN_SetFreq(&sig_gen_3, SIG_GEN_CARRIER, 133);
  SIG_GEN_SetFreq(&sig_gen_4, SIG_GEN_CARRIER, 144);

  SIG_GEN_CommitChanges(&sig_gen_1);
  SIG_GEN_CommitChanges(&sig_gen_2);
  SIG_GEN_CommitChanges(&sig_gen_3);
  SIG_GEN_CommitChanges(&sig_gen_4);

  uart.Init(&huart5);

//  SIG_GEN_Start(&sig_gen_1);
//  SIG_GEN_Start(&sig_gen_2);
//  SIG_GEN_Start(&sig_gen_3);
//  SIG_GEN_Start(&sig_gen_4);

//  PauseAllChannels();

  for(;;) {
//    osDelay(5000);
//    RELAY_GROUND();
//    osDelay(5000);
//    RELAY_TRI_STATE();
#ifdef SELF_MESSAGE_ON
    static tdUartMessage msg = {{100, 0, 4, UART_SIGNAL_CARRIER, SIG_GEN_PARAM_FREQ, 0}, 0};
    osDelay(DELAY_LED*2);
    osStatus_t status = osMessageQueuePut(SignalGeneratorQueueHandle, &msg, 0U, 0U);
    msg.data.emitter = (msg.data.emitter < 3 ? ++msg.data.emitter : 0);
#endif
  }
}

/**
* @brief Function implementing the ChngSignalParam thread.
* @param argument: Not used
* @retval None
*/
void ChangeSignalParamsTask(void *argument) {
  tdUartMessage msg;
  osStatus_t status;

  for(;;) {
    status = osMessageQueueGet(SignalGeneratorQueueHandle, &msg, NULL, osWaitForever);
    if (status == osOK) {
      uint8_t channel = msg.data.emitter;
      if (msg.data.type == UART_MESSAGE_DATA) {
//        PauseAllChannels();
//        osDelay(1);
        SIG_GEN_SetSignal(emitter_to_siggen[channel], msg.data.signal, msg.data.param, msg.data.value);
//        SIG_GEN_Resume(emitter_to_siggen[channel]);
        LedPortPinTypeDef led = emitter_to_led[channel];
        LED_ON(LED1_GREEN_GPIO_Port, LED1_GREEN_Pin);
//        RELAY_GROUND();
        osDelay(DELAY_LED);
        LED_OFF(LED1_GREEN_GPIO_Port, LED1_GREEN_Pin);
//        RELAY_TRI_STATE();
        osDelay(DELAY_LED);
      } else if (msg.data.type == UART_MESSAGE_END) {
        SIG_GEN_CommitChanges(emitter_to_siggen[channel]);
        osDelay(3000);
        RELAY_GROUND();
        HAL_TIM_Base_Start_IT(&htim12);
      }
    } else {
//      Error_Handler();
    }
  }
}
