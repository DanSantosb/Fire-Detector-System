#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

#define pulsosPorSegundo                24000000
#define pulsosPorMilissegundo           24000
#define pulsosPorMicrossegundo          24

extern volatile uint64_t overflowCounter;

void begin_WMTIMER7(void);
void unsetOverflow(void);
void setOverflow(void);
void _delay_sec(unsigned int time);
void _delay_ms(unsigned int time);
void _delay_us(unsigned int time);

#endif