#include "pwm_os_tasks.h"
#include "main.h"
#include "tim.h"

#include <memory>

#include "app/com_interface/com_interface.h"

#include "signal_generator/pwm_controller.h"
#include "signal_generator/sig_gen_include.h"

extern osMessageQueueId_t SignalGeneratorQueueHandle;
extern UART_HandleTypeDef huart5;
extern Uart uart;

static SIG_GEN_HandleTypeDef sig_gen_1 = {0};
static SIG_GEN_HandleTypeDef sig_gen_2 = {0};
static SIG_GEN_HandleTypeDef sig_gen_3 = {0};
static SIG_GEN_HandleTypeDef sig_gen_4 = {0};

static SIG_GEN_HandleTypeDef* emitter_to_siggen[4] = { 
  &sig_gen_1,
  &sig_gen_2,
  &sig_gen_3,
  &sig_gen_4
};

//#define ADDITIONAL_MCU

#ifndef ADDITIONAL_MCU

#define START_LED 0
#define END_LED 4

static GPIO_TypeDef* emitter_to_led_port[8] = { 
  LED1_GREEN_GPIO_Port,
  LED2_GREEN_GPIO_Port,
  LED3_RED_GPIO_Port,
  LED4_RED_GPIO_Port,
  LED1_GREEN_GPIO_Port,
  LED2_GREEN_GPIO_Port,
  LED3_RED_GPIO_Port,
  LED4_RED_GPIO_Port
};

static uint16_t emitter_to_led_pin[8] = { 
  LED1_GREEN_Pin,
  LED2_GREEN_Pin,
  LED3_RED_Pin,
  LED4_RED_Pin,
  LED1_GREEN_Pin,
  LED2_GREEN_Pin,
  LED3_RED_Pin,
  LED4_RED_Pin
};

#else

#define START_LED 4
#define END_LED 8

static GPIO_TypeDef* emitter_to_led_port[8] = { 
  LED2_GREEN_GPIO_Port,
  LED1_GREEN_GPIO_Port,
  LED3_RED_GPIO_Port,
  LED4_RED_GPIO_Port,
  LED2_GREEN_GPIO_Port,
  LED1_GREEN_GPIO_Port,
  LED3_RED_GPIO_Port,
  LED4_RED_GPIO_Port
};

static uint16_t emitter_to_led_pin[8] = { 
  LED2_GREEN_Pin,
  LED1_GREEN_Pin,
  LED3_RED_Pin,
  LED4_RED_Pin,
  LED2_GREEN_Pin,
  LED1_GREEN_Pin,
  LED3_RED_Pin,
  LED4_RED_Pin
};

#endif

static void MY_SIG_GEN_Init();

void StartDefaultTask(void *argument) {
  /* Infinite loop */
  
  MY_SIG_GEN_Init();
  
  SIG_GEN_SetSignalType(&sig_gen_1, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
  SIG_GEN_SetSignalType(&sig_gen_2, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
  SIG_GEN_SetSignalType(&sig_gen_3, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);
  SIG_GEN_SetSignalType(&sig_gen_4, SIG_GEN_CARRIER, SIG_GEN_TYPE_SINUS);

  SIG_GEN_SetFreq(&sig_gen_1, SIG_GEN_CARRIER, 100);
  SIG_GEN_SetFreq(&sig_gen_2, SIG_GEN_CARRIER, 100);
  SIG_GEN_SetFreq(&sig_gen_3, SIG_GEN_CARRIER, 100);
  SIG_GEN_SetFreq(&sig_gen_4, SIG_GEN_CARRIER, 100);

  uart.Init(&huart5);
  
  SIG_GEN_Start(&sig_gen_1);
  SIG_GEN_Start(&sig_gen_2);
  SIG_GEN_Start(&sig_gen_3);
  SIG_GEN_Start(&sig_gen_4);
  
  for(;;) {
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
    // wait for message
    status = osMessageQueueGet(SignalGeneratorQueueHandle, &msg, NULL, osWaitForever);
    if (status == osOK) {
      if (msg.emitter >= START_LED && msg.emitter < END_LED) {
        SIG_GEN_SetSignal(emitter_to_siggen[msg.emitter], msg.signal, msg.param, msg.value);
        HAL_GPIO_WritePin(emitter_to_led_port[msg.emitter], emitter_to_led_pin[msg.emitter], GPIO_PIN_SET);
        HAL_Delay(500);
        HAL_GPIO_WritePin(emitter_to_led_port[msg.emitter], emitter_to_led_pin[msg.emitter], GPIO_PIN_RESET);
      }
      HAL_Delay(1);
    } else {
      Error_Handler();
    }
  }
}

void MY_SIG_GEN_Init() {
  sig_gen_1.pwm_timer = &htim1;
	sig_gen_1.sample_timer = &htim12;
  sig_gen_1.pwm_mode = SIG_GEN_IT_MODE;
  sig_gen_1.channels[0] = TIM_CHANNEL_1;
  sig_gen_1.channels[1] = TIM_CHANNEL_2;
  sig_gen_1.min_duty_cycle_percent = 0;
  sig_gen_1.max_duty_cycle_percent = 100;
  sig_gen_1.dead_time_th_percent = 20;
  sig_gen_1.coeffs = 0;
  if (SIG_GEN_Init(&sig_gen_1) != SIG_GEN_OK) {
    Error_Handler();
  }

  sig_gen_2.pwm_timer = &htim2;
	sig_gen_2.sample_timer = &htim12;
  sig_gen_2.pwm_mode = SIG_GEN_IT_MODE;
  sig_gen_2.channels[0] = TIM_CHANNEL_1;
  sig_gen_2.channels[1] = TIM_CHANNEL_2;
  sig_gen_2.min_duty_cycle_percent = 0;
  sig_gen_2.max_duty_cycle_percent = 100;
  sig_gen_2.dead_time_th_percent = 20;
  sig_gen_2.coeffs = 0;
  if (SIG_GEN_Init(&sig_gen_2) != SIG_GEN_OK) {
    Error_Handler();
  }
	
  sig_gen_3.pwm_timer = &htim3;
	sig_gen_3.sample_timer = &htim12;
  sig_gen_3.pwm_mode = SIG_GEN_IT_MODE;
  sig_gen_3.channels[0] = TIM_CHANNEL_3;
  sig_gen_3.channels[1] = TIM_CHANNEL_4;
  sig_gen_3.min_duty_cycle_percent = 0;
  sig_gen_3.max_duty_cycle_percent = 100;
  sig_gen_3.dead_time_th_percent = 20;
  sig_gen_3.coeffs = 0;
  if (SIG_GEN_Init(&sig_gen_3) != SIG_GEN_OK) {
    Error_Handler();
  }
	
  sig_gen_4.pwm_timer = &htim4;
	sig_gen_4.sample_timer = &htim12;
  sig_gen_4.pwm_mode = SIG_GEN_IT_MODE;
  sig_gen_4.channels[0] = TIM_CHANNEL_1;
  sig_gen_4.channels[1] = TIM_CHANNEL_2;
  sig_gen_4.min_duty_cycle_percent = 0;
  sig_gen_4.max_duty_cycle_percent = 100;
  sig_gen_4.dead_time_th_percent = 20;
  sig_gen_4.coeffs = 0;
  if (SIG_GEN_Init(&sig_gen_4) != SIG_GEN_OK) {
    Error_Handler();
  }
}
