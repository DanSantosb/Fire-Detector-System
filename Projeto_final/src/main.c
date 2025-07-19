#include <stdbool.h>
#include <stdint.h>
#include "../inc/headers.h"
#include "../inc/IOsetups.h"
#include "../inc/timer.h"
#include "../inc/Sensor.h"

//============================================= DESLIGA O WATCHDOG =========================================  
void disable_watchdog(void) {
  WDT_WSPR = 0xAAAA;
  while (WDT_WWPS & (1 << 4));
  WDT_WSPR = 0x5555;
  while (WDT_WWPS & (1 << 4));
}
//=========================================== TRATADOR DE INTERRUPÇÃO =========================================
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
//==============================================================================================================

int main(void) {
  disable_watchdog();
  gpio_setup();
  begin_WMTIMER7();
  ADC_setup();

  GPIO1_SETDATAOUT = LED_WHITE;
  uart0_writeln("CALIBRANDO SENSORES, ESPERE UM POUCO...");
  _delay_sec(10);
  GPIO1_CLEARDATAOUT = LED_WHITE;
  uart0_writeln("SENSORS CALIBRADOS :)");
  
  float ppm_MQ2, ppm_MQ6;
  //Loop principal
  while (1){
    lerSensores(&ppm_MQ2, &ppm_MQ6);

    if (ppm_MQ2 < 500 || ppm_MQ6 < 500){
      changeGreen(); 
    } else if (ppm_MQ2 >= 500 && ppm_MQ2 <= 999 || ppm_MQ6 >= 500 && ppm_MQ6 <= 999){
      changeYellow();
    } else if (ppm_MQ2 >= 1000 || ppm_MQ6 >= 1000){
      changeRed();
    }
  }
  return 0;
} 
