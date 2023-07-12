#ifndef _SIG_GEN_CONFIG_H_
#define _SIG_GEN_CONFIG_H_

// Параметры сигналов
#define CARRIER_FREQ_MAX_HZ 1000 // максимальная частота несущего сигнала
#define CARRIER_FREQ_MIN_HZ 1 // максимальная частота несущего сигнала
#define POINTS_PER_PERIOD_NUM 16 // количество точек на минимальный период несущего сигнала
#define SAMPLE_RATE (POINTS_PER_PERIOD_NUM * CARRIER_FREQ_MAX_HZ) // частота семплирования таймера без DMA / частота вывода DMA

// Параметры генератора DMA
#define DMA_GEN_TOTAL_NUM 4 // общее количество генераторов DMA
#define BUF_DATA_TYPE int32_t // тип данных для DMA
#define IT_BUF_DATA_TYPE int8_t // тип данных для IT

#define DMA_BUF_SIZE 1

#define IT_BUF_SIZE ((CARRIER_FREQ_MAX_HZ - CARRIER_FREQ_MIN_HZ) \
                  / CARRIER_FREQ_MIN_HZ \
                  * POINTS_PER_PERIOD_NUM)
                  
//#define DMA_BUF_SIZE 16
//#define IT_BUF_SIZE DMA_BUF_SIZE

#define RTOS_TASK_STACK_SIZE 1024

/*
  Прерывание таймера семплирования для генераторов, которые работают не по DMA,
  должны иметь более высокий приоритет
*/

#endif // #ifndef _SIG_GEN_CONFIG_H_