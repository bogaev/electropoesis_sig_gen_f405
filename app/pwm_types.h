#ifndef _PWM_TYPES_H_
#define _PWM_TYPES_H_

#include <stdint.h>

const int EMITTERS_NUM = 8;

typedef enum {
  RPI_SIGNAL_CARRIER = 0,
  RPI_SIGNAL_AMP_MOD,
  RPI_SIGNAL_FREQ_MOD,
  RPI_SIGNAL_NUM
} enPwmSignals;

typedef enum {
  SIGNAL_TYPE_NONE = 0,
  SIGNAL_TYPE_SINUS,
  SIGNAL_TYPE_SQUARE,
  SIGNAL_TYPE_TRIANGLE,
  SIGNAL_TYPE_SAW,
  SIGNAL_TYPE_NUM
} tdSignalTypes;

//typedef enum {
//  RPI_PARAM_SIGNAL_TYPE = 0,
//  RPI_PARAM_AMP,
//  RPI_PARAM_FREQ,
//  RPI_PARAM_NUM
//} enSignalParams;

typedef enum {
  COM_MSG_PAUSE_ALL = 0,
  COM_MSG_CHECK_MCU_READY,
  COM_MSG_CHANGE_PARAM,
  COM_MSG_COMMIT,
  COM_MSG_RELAY_ON,
  COM_MSG_RELAY_OFF
} tdMessageType;

typedef struct {
  uint32_t type:4;
  uint32_t emitter:4;
  uint32_t signal:4;
  uint32_t param:4;
  uint32_t value:16;
} tdPwmData;

typedef struct {
  uint32_t crc = 0;
  tdPwmData data = {0};
} tdRpiMessage;

#endif // #ifndef _PWM_TYPES_H_