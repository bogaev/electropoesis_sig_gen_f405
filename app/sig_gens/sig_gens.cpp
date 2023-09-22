#include "sig_gens.h"

#include "tim.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

const SIG_GEN_RangeCoeff corr_amp[] = {
  { .from = 0, .to = 2, .coeff = 100.0 }
};

const SIG_GEN_RangeCoeff corr_freq[] = {
  { .from = 0, .to = 100, .coeff = 97 },
  { .from = 100, .to = 150, .coeff = 100 },
  { .from = 150, .to = 200, .coeff = 103 },
  { .from = 200, .to = 250, .coeff = 108 },
  { .from = 250, .to = 300, .coeff = 110 },
  { .from = 300, .to = 350, .coeff = 115 },
  { .from = 350, .to = 450, .coeff = 125 },
  { .from = 450, .to = 500, .coeff = 130 },
  { .from = 500, .to = 600, .coeff = 135 },
  { .from = 600, .to = 800, .coeff = 140 },
  { .from = 800, .to = 900, .coeff = 165 },
  { .from = 900, .to = CARRIER_FREQ_MAX_HZ, .coeff = 185 }
};

SIG_GEN_CoeffsInitStruct corr_coeff = {
  corr_amp,
  1,
  corr_freq,
  12
};
//  sig_gen_##N.max_duty_cycle_percent = 68; \

#define SEG_GEN_INIT(N, CH1, CH2) \
  sig_gen_##N.pwm_timer = &htim##N; \
  sig_gen_##N.sample_timer = &htim12; \
  sig_gen_##N.pwm_mode = SIG_GEN_IT_MODE; \
  sig_gen_##N.channels[0] = CH1; \
  sig_gen_##N.channels[1] = CH2; \
  sig_gen_##N.min_duty_cycle_percent = 53; \
  sig_gen_##N.max_duty_cycle_percent = 68; \
  sig_gen_##N.dead_time_th_percent = 20; \
  sig_gen_##N.coeffs = &corr_coeff; \
  if (SIG_GEN_Init(&sig_gen_##N) != SIG_GEN_OK) { \
    Error_Handler(); \
  }

SIG_GEN_HandleTypeDef sig_gen_1 = {0};
SIG_GEN_HandleTypeDef sig_gen_2 = {0};
SIG_GEN_HandleTypeDef sig_gen_3 = {0};
SIG_GEN_HandleTypeDef sig_gen_4 = {0};

#ifdef THIS_IS_MAIN_MCU

std::unordered_map<int, LedPortPinTypeDef> emitter_to_led = {
  {0, {LED1_HARD_FAULT_GPIO_Port, LED1_HARD_FAULT_Pin}},
  {1, {LED2_UART_MSG_GPIO_Port, LED2_UART_MSG_Pin}},
  {3, {LED4_WD_UPDATE_GPIO_Port, LED4_WD_UPDATE_Pin}},
  {5, {LED3_RELAY_GROUND_GPIO_Port, LED3_RELAY_GROUND_Pin}}
};

std::unordered_map<int, SIG_GEN_HandleTypeDef*> emitter_to_siggen = {
  {0, &sig_gen_1},
  {1, &sig_gen_2},
  {3, &sig_gen_4},
  {5, &sig_gen_3}
};

#else

std::unordered_map<int, LedPortPinTypeDef> emitter_to_led = {
  {2, {LED2_UART_MSG_GPIO_Port, LED2_UART_MSG_Pin}},
  {4, {LED3_RELAY_GROUND_GPIO_Port, LED3_RELAY_GROUND_Pin}},
  {6, {LED4_WD_UPDATE_GPIO_Port, LED4_WD_UPDATE_Pin}},
  {7, {LED1_HARD_FAULT_GPIO_Port, LED1_HARD_FAULT_Pin}}
};

std::unordered_map<int, SIG_GEN_HandleTypeDef*> emitter_to_siggen = {
  {2, &sig_gen_1},
  {4, &sig_gen_3},
  {6, &sig_gen_4},
  {7, &sig_gen_2}
};

#endif

void MY_SIG_GEN_Init() {
  SEG_GEN_INIT(1, TIM_CHANNEL_1, TIM_CHANNEL_2);
  SEG_GEN_INIT(2, TIM_CHANNEL_1, TIM_CHANNEL_2);
  SEG_GEN_INIT(3, TIM_CHANNEL_3, TIM_CHANNEL_4);
  SEG_GEN_INIT(4, TIM_CHANNEL_1, TIM_CHANNEL_2);
  HAL_TIM_Base_Start_IT(&htim12);
}
