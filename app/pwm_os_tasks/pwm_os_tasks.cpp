#include "pwm_os_tasks.h"
#include "main.h"

#include "app/com_interface/com_interface.h"
#include "app/sig_gen_config.h"
#include "app/sig_gens/sig_gens.h"

#include "signal_generator/pwm_controller.h"
#include "signal_generator/sig_gen_include.h"

#ifdef SELF_MESSAGE_ON
  #define DELAY_LED 1000
#else
  #define DELAY_LED 10
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

  uart.Init(&huart5);

  SIG_GEN_Start(&sig_gen_1);
  SIG_GEN_Start(&sig_gen_2);
  SIG_GEN_Start(&sig_gen_3);
  SIG_GEN_Start(&sig_gen_4);

  PauseAllChannels();

  for(;;) {
#ifdef SELF_MESSAGE_ON
    static tdPwmData data = {100, 0, 0, UART_SIGNAL_CARRIER, SIG_GEN_PARAM_FREQ, 0};
    osDelay(DELAY_LED);
    osStatus_t status = osMessageQueuePut(SignalGeneratorQueueHandle, &data, 0U, 0U);
    data.emitter = (data.emitter < 7 ? ++data.emitter : 0);
#endif
  }
}

/**
* @brief Function implementing the ChngSignalParam thread.
* @param argument: Not used
* @retval None
*/
void ChangeSignalParamsTask(void *argument) {
  tdPwmData msg;
  osStatus_t status;

  for(;;) {
    status = osMessageQueueGet(SignalGeneratorQueueHandle, &msg, NULL, osWaitForever);
    if (status == osOK) {
      uint8_t channel = msg.emitter;
      if (emitter_to_siggen.count(channel)) {
        PauseAllChannels();
        osDelay(1);
//        SIG_GEN_SetSignal(emitter_to_siggen[channel], msg.signal, msg.param, msg.value);
        SIG_GEN_Resume(emitter_to_siggen[channel]);
        LedPortPinTypeDef led = emitter_to_led[channel];
        HAL_GPIO_WritePin(led.first, led.second, GPIO_PIN_SET);
        osDelay(DELAY_LED);
        HAL_GPIO_WritePin(led.first, led.second, GPIO_PIN_RESET);
      }
    } else {
//      Error_Handler();
    }
  }
}
