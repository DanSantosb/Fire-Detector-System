#include <stdbool.h>
#include <stdint.h>
#include "../inc/headers.h"
#include "../inc/IOsetups.h"
#include "../inc/timer.h"

//============================================= DESLIGA O WATCHDOG =========================================  
void disable_watchdog(void) {
  WDT_WSPR = 0xAAAA;
  while (WDT_WWPS & (1 << 4));
  WDT_WSPR = 0x5555;
  while (WDT_WWPS & (1 << 4));
}
//============================================================================================================
void ISR_Handler(void) {
  uint32_t irq = INTC_SIR_IRQ & 0x7F;
  if (irq == 95) {
    overflowCounter++;
    DM_TIMER7_IRQ_EOI = 0x0;
  }
  if (irq == 98){
    gpio_isr_handler();
  }  
  INTC_CONTROL = 0x1;
}

int main(void) {
  disable_watchdog();
  gpio_setup();
  begin_WMTIMER7();

  GPIO1_SETDATAOUT = LED_WHITE;
  uart0_writeln("CALIBRANDO SENSORES, ESPERE UM POUCO...");
  _delay_sec(10);
  GPIO1_CLEARDATAOUT = LED_WHITE;
  uart0_writeln("SENSORS CALIBRADOS :)");
    
    //Loop principal
  while (1){
    char entrada = uart0_getch();
    if (entrada - '0' == 1){
      changeGreen(); 
    } else if (entrada - '0' == 2){
      changeYellow();
    } else if (entrada - '0' == 3){
      changeRed();
    }
  }
  return 0;
} 
