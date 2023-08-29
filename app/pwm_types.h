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
  RPI_MESSAGE_DATA = 0,
  RPI_MESSAGE_START,
  RPI_MESSAGE_END
} tdMessageType;

typedef struct {
  uint8_t type;
  uint8_t emitter;
  uint8_t signal;
  uint8_t param;
  uint16_t value;
} tdPwmData;

typedef struct {
  uint32_t crc = 0;
//  tdPwmData data = {0};
  uint32_t data = 0xDEADBEEF;
} tdRpiMessage;

#endif // #ifndef _PWM_TYPES_H_