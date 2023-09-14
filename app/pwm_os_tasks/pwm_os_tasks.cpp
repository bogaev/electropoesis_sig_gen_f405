#include "pwm_os_tasks.h"

#include "main.h"
#include "iwdg.h"

#include "app/sig_gen_config.h"
#include "app/sig_gens/sig_gens.h"
#include "app/com_interface/com_interface_i2c.h"

#include "signal_generator/pwm_controller.h"
#include "signal_generator/sig_gen_include.h"

#ifdef SELF_MESSAGE_ON
  #define DELAY_LED 2000
#else
  #define DELAY_LED 200
#endif

void StartDefaultTask(void *argument) {
  /* Infinite loop */
  static UBaseType_t uxHighWaterMark2;
  static size_t minEverFreeHeapSize2;

  MY_SIG_GEN_Init();

//  SIG_GEN_SetSignalType(&sig_gen_1, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
//  SIG_GEN_SetSignalType(&sig_gen_2, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
//  SIG_GEN_SetSignalType(&sig_gen_3, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
//  SIG_GEN_SetSignalType(&sig_gen_4, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
//
//  SIG_GEN_SetFreq(&sig_gen_1, SIG_GEN_CARRIER, 111);
//  SIG_GEN_SetFreq(&sig_gen_2, SIG_GEN_CARRIER, 122);
//  SIG_GEN_SetFreq(&sig_gen_3, SIG_GEN_CARRIER, 133);
//  SIG_GEN_SetFreq(&sig_gen_4, SIG_GEN_CARRIER, 144);
//
//  SIG_GEN_CommitChanges(&sig_gen_1);
//  SIG_GEN_CommitChanges(&sig_gen_2);
//  SIG_GEN_CommitChanges(&sig_gen_3);
//  SIG_GEN_CommitChanges(&sig_gen_4);

  I2cStart(&hi2c1);

//  RELAY_GROUND();
//  LED_ON(LED3_RELAY_GROUND_GPIO_Port, LED3_RELAY_GROUND_Pin);

  for(;;) {
    LED_ON(LED4_WD_UPDATE_GPIO_Port, LED4_WD_UPDATE_Pin);
    osDelay(DELAY_LED);
    HAL_IWDG_Refresh(&hiwdg);
    LED_OFF(LED4_WD_UPDATE_GPIO_Port, LED4_WD_UPDATE_Pin);
    osDelay(DELAY_LED);

    uxHighWaterMark2 = uxTaskGetStackHighWaterMark( NULL );
    minEverFreeHeapSize2 = xPortGetMinimumEverFreeHeapSize();

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
  tdRpiMessage msg;
  osStatus_t status;

  static UBaseType_t uxHighWaterMark3;
  static size_t minEverFreeHeapSize3;

  for(;;) {
//    static uint32_t currentItemsGet = osMessageQueueGetCount(SignalGeneratorQueueHandle);
    status = osMessageQueueGet(SignalGeneratorQueueHandle, &msg, NULL, osWaitForever);
//    currentItemsGet = osMessageQueueGetCount(SignalGeneratorQueueHandle);

    uxHighWaterMark3 = uxTaskGetStackHighWaterMark( NULL );
    minEverFreeHeapSize3 = xPortGetMinimumEverFreeHeapSize();

    if (status == osOK) {
      uint8_t channel = msg.data.emitter;
      if (msg.data.type == COM_MSG_CHANGE_PARAM) {
        SIG_GEN_SetSignal(emitter_to_siggen[channel], msg.data.signal, msg.data.param, msg.data.value);
      } else if (msg.data.type == COM_MSG_COMMIT) {
        SIG_GEN_CommitChanges(emitter_to_siggen[channel]);
//        osDelay(1000);
//        RELAY_GROUND();
//        LED_ON(LED3_RELAY_GROUND_GPIO_Port, LED3_RELAY_GROUND_Pin);
//        HAL_TIM_Base_Start_IT(&htim12);
      }
    } else {
      Error_Handler();
    }
  }
}
