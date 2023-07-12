#ifndef _PWM_OS_TASKS_H_
#define _PWM_OS_TASKS_H_

#include "tim.h"
#include "../common.h"

extern "C" void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim);
extern "C" void GeneratePWM_Task(void *argument);
extern "C" void InitPwmControllers();
extern "C" void Run_PWM();

void ChangeSignalParamsTask(void *argument);
void StartDefaultTask(void *argument);

#endif // #ifndef _PWM_OS_TASKS_H_