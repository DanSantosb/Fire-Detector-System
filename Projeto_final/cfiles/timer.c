#include "../inc/timer.h"
#include "../inc/headers.h"

volatile uint64_t overflowCounter = 0;

void begin_WMTIMER7(void){
  CM_PER_TIMER7_CLKCTRL = 0x2;
  while ((CM_PER_TIMER7_CLKCTRL & 0x3) != 0x2);

  DM_TIMER7_TCLR = 0x0;           // Para o timer
  DM_TIMER7_TLDR = 0x0;           // Valor recarga
  DM_TIMER7_TCRR = 0x0;           // Zera contador

  DM_TIMER7_IRQSTATUS = (1 << 1);      // Limpa flag de overflow antes de habilitar
  DM_TIMER7_IRQENABLE_SET = (1 << 1);  // Habilita interrupção overflow
  INTC_MIR_CLEAR2 = (1 << 31);         // Desmascara IRQ 95
}


void _delay_sec(unsigned int time){
  uint64_t alvo = (uint64_t)time * pulsosPorSegundo;
  overflowCounter = 0;
  DM_TIMER7_TCRR = 0x0;

  DM_TIMER7_TCLR = DM_TIMER7_AUTORELOAD | DM_TIMER7_START_STOP_TIMER; // Inicia timer

  while (((uint64_t)overflowCounter << 32) + DM_TIMER7_TCRR < alvo) {
    // Espera o tempo passar
  }

  DM_TIMER7_TCLR = 0; // Para o timer
  DM_TIMER7_IRQSTATUS = (1 << 1); // Limpa flag de overflow
  overflowCounter = 0;
}

void _delay_ms(unsigned int time){
  uint64_t alvo = (uint64_t)time * pulsosPorMilissegundo;
  overflowCounter = 0;
  DM_TIMER7_TCRR = 0x0;

  DM_TIMER7_TCLR = DM_TIMER7_AUTORELOAD | DM_TIMER7_START_STOP_TIMER; // Inicia timer

  while (((uint64_t)overflowCounter << 32) + DM_TIMER7_TCRR < alvo) {
    // Espera o tempo passar
  }

  DM_TIMER7_TCLR = 0; // Para o timer
  DM_TIMER7_IRQSTATUS = (1 << 1); // Limpa flag de overflow
  overflowCounter = 0;
}

void _delay_us(unsigned int time){
  uint64_t alvo = (uint64_t)time * pulsosPorMilissegundo;
  overflowCounter = 0;
  DM_TIMER7_TCRR = 0x0;

  DM_TIMER7_TCLR = DM_TIMER7_AUTORELOAD | DM_TIMER7_START_STOP_TIMER; // Inicia timer

  while (((uint64_t)overflowCounter << 32) + DM_TIMER7_TCRR < alvo) {
    // Espera o tempo passar
  }

  DM_TIMER7_TCLR = 0; // Para o timer
  DM_TIMER7_IRQSTATUS = (1 << 1); // Limpa flag de overflow
  overflowCounter = 0;
}