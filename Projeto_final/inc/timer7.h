#ifndef TIMER7_H
#define TIMER7_H
#include "../inc/defines.h"


//======================================= DECLARA O DELAY =============================
volatile unsigned int overflow_counter = 0;

void _delay_sec(unsigned int secs) {
  DM_TIMER7_TCLR |= DM_TIMER7_START_STOP_TIMER;
  uint64_t alvo = (uint64_t)secs * pulsosPorSegundo;
  while (((uint64_t)overflow_counter << 32) + DM_TIMER7_TCRR < alvo);
  DM_TIMER7_TCLR &= ~DM_TIMER7_START_STOP_TIMER;
  DM_TIMER7_TCRR = 0x0;
  overflow_counter = 0;
}


//======================================= SETUP DO TIMER =============================

void timer7_setup() { //CONFIGURA TIME
  DM_TIMER7_TCLR = 0x0;
  DM_TIMER7_TLDR = 0x0;
  DM_TIMER7_TCRR = 0x0;

  DM_TIMER7_TCLR |= DM_TIMER7_AUTORELOAD;
  DM_TIMER7_IRQENABLE_SET |= (1 << 1); // Habilita interrupção por overflow
  INTC_MIR_CLEAR2 = (1 << 31); // IRQ 95 → Timer7
}

#endif