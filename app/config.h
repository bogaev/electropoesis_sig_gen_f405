#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#if defined (WIN32)
  #define USE_FAKE_INTERFACE
  #define USE_FAKE_MEMORY
#elif defined (HARDWARE_TESTS)
  #define USE_HARDWARE_TESTS
#elif defined (SIMULATOR_TESTS)
  #define USE_SIMULATOR_TESTS
#else
  #define USE_FLASH_MEMORY
//  #define USE_EEPROM_MEMORY
//  #define USE_FAKE_MEMORY
  #define USE_FAKE_INTERFACE
//  #define USE_WIRELESS_INTERFACE
#endif

#endif // CONFIG_H
