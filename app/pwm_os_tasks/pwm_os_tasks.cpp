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
  #define DELAY_LED 100
#endif

void StartDefaultTask(void *argument) {
//  static UBaseType_t uxHighWaterMark;
//  static size_t minEverFreeHeapSize;

  MY_SIG_GEN_Init();
  I2cStart(&hi2c1);

  for(;;) {
    LED_ON(LED4_WD_UPDATE_GPIO_Port, LED4_WD_UPDATE_Pin);
    osDelay(DELAY_LED);
    HAL_IWDG_Refresh(&hiwdg);
    LED_OFF(LED4_WD_UPDATE_GPIO_Port, LED4_WD_UPDATE_Pin);
    osDelay(DELAY_LED);

//    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
//    minEverFreeHeapSize = xPortGetMinimumEverFreeHeapSize();

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
  tdPwmData data;
  osStatus_t status;

  for(;;) {
    status = osMessageQueueGet(SignalGeneratorQueueHandle, &data, NULL, osWaitForever);

    if (status == osOK) {
      uint8_t channel = data.emitter;
      if (data.type == COM_MSG_CHANGE_PARAM) {
        SIG_GEN_SetSignal(emitter_to_siggen[channel], data.signal, data.param, data.value);
      } else if (data.type == COM_MSG_COMMIT) {
        SIG_GEN_CommitChanges(emitter_to_siggen[channel]);
      } else if (data.type == COM_MSG_RELAY_ON) {
        SIG_GEN_Pause(&sig_gen_1);
        SIG_GEN_Pause(&sig_gen_2);
        SIG_GEN_Pause(&sig_gen_3);
        SIG_GEN_Pause(&sig_gen_4);
        osDelay(DELAY_LED);
        RELAY_GROUND();
        LED_ON(LED3_RELAY_GROUND_GPIO_Port, LED3_RELAY_GROUND_Pin);
        osDelay(DELAY_LED);
        i2c->SetStatus({COM_STATUS_OK, MCU_STATUS_RELAY_ON});
      } else if (data.type == COM_MSG_RESUME) {
        SIG_GEN_Resume(&sig_gen_1);
        SIG_GEN_Resume(&sig_gen_2);
        SIG_GEN_Resume(&sig_gen_3);
        SIG_GEN_Resume(&sig_gen_4);
        i2c->SetStatus({COM_STATUS_OK, MCU_STATUS_RESUME});
      } else if (data.type == COM_MSG_PAUSE) {
        SIG_GEN_Pause(&sig_gen_1);
        SIG_GEN_Pause(&sig_gen_2);
        SIG_GEN_Pause(&sig_gen_3);
        SIG_GEN_Pause(&sig_gen_4);
        i2c->SetStatus({COM_STATUS_OK, MCU_STATUS_PAUSE});
      }
    } else {
        i2c->SetStatus({COM_STATUS_ERR, MCU_STATUS_ERR});
    }
  }
}
