#ifndef _SIG_GENS_H_
#define _SIG_GENS_H_

#include "main.h"

#include "signal_generator/sig_gen_include.h"

#include <utility>
#include <unordered_map>

//#define LED_ON(PORT, PIN) HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_SET)
//#define LED_OFF(PORT, PIN) HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_RESET)

//#define RELAY_TRI_STATE() HAL_GPIO_WritePin(RELAY_220V_GPIO_Port, RELAY_220V_Pin, GPIO_PIN_SET)
//#define RELAY_GROUND() HAL_GPIO_WritePin(RELAY_220V_GPIO_Port, RELAY_220V_Pin, GPIO_PIN_RESET)

using LedPortPinTypeDef = std::pair<GPIO_TypeDef*, uint16_t>;

extern SIG_GEN_HandleTypeDef sig_gen_1;
extern SIG_GEN_HandleTypeDef sig_gen_2;
extern SIG_GEN_HandleTypeDef sig_gen_3;
extern SIG_GEN_HandleTypeDef sig_gen_4;

extern std::unordered_map<int, LedPortPinTypeDef> emitter_to_led;
extern std::unordered_map<int, SIG_GEN_HandleTypeDef*> emitter_to_siggen;

void MY_SIG_GEN_Init();
void PauseAllChannels();

#endif // #ifndef _SIG_GENS_H_