#include "sig_gens.h"

#include "tim.h"

#define SEG_GEN_INIT(N, CH1, CH2) \
  sig_gen_##N.pwm_timer = &htim##N; \
  sig_gen_##N.sample_timer = &htim12; \
  sig_gen_##N.pwm_mode = SIG_GEN_IT_MODE; \
  sig_gen_##N.channels[0] = CH1; \
  sig_gen_##N.channels[1] = CH2; \
  sig_gen_##N.min_duty_cycle_percent = 53; \
  sig_gen_##N.max_duty_cycle_percent = 67; \
  sig_gen_##N.dead_time_th_percent = 20; \
  sig_gen_##N.coeffs = 0; \
  if (SIG_GEN_Init(&sig_gen_##N) != SIG_GEN_OK) { \
    Error_Handler(); \
  }

SIG_GEN_HandleTypeDef sig_gen_1 = {0};
SIG_GEN_HandleTypeDef sig_gen_2 = {0};
SIG_GEN_HandleTypeDef sig_gen_3 = {0};
SIG_GEN_HandleTypeDef sig_gen_4 = {0};

#ifdef THIS_IS_MAIN_MCU

std::unordered_map<int, LedPortPinTypeDef> emitter_to_led = {
  {0, {LED1_GREEN_GPIO_Port, LED1_GREEN_Pin}},
  {1, {LED2_GREEN_GPIO_Port, LED2_GREEN_Pin}},
  {2, {LED4_RED_GPIO_Port, LED4_RED_Pin}},
  {5, {LED3_RED_GPIO_Port, LED3_RED_Pin}}
};

std::unordered_map<int, SIG_GEN_HandleTypeDef*> emitter_to_siggen = {
  {0, &sig_gen_1},
  {1, &sig_gen_2},
  {2, &sig_gen_4},
  {5, &sig_gen_3}
};

#else

std::unordered_map<int, LedPortPinTypeDef> emitter_to_led = {
  {3, {LED2_GREEN_GPIO_Port, LED2_GREEN_Pin}},
  {4, {LED3_RED_GPIO_Port, LED3_RED_Pin}},
  {6, {LED4_RED_GPIO_Port, LED4_RED_Pin}},
  {7, {LED1_GREEN_GPIO_Port, LED1_GREEN_Pin}}
};

std::unordered_map<int, SIG_GEN_HandleTypeDef*> emitter_to_siggen = {
  {3, &sig_gen_1},
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

void PauseAllChannels() {
  for (const auto& sig_gen : emitter_to_siggen) {
    SIG_GEN_Pause(sig_gen.second);
  }
}
