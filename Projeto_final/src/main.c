#include <stdbool.h>
#include <stdint.h>
#include "../inc/defines.h"
#include "../inc/IOsetups.h"
#include "../inc/timer7.h"

//============================================= DESLISGA O WATCHDOG =========================================  
void disable_watchdog(void) {
  WDT_WSPR = 0xAAAA;
  while (WDT_WWPS & (1 << 4));
  WDT_WSPR = 0x5555;
  while (WDT_WWPS & (1 << 4));
}
//============================================================================================================

int main(void) {
  disable_watchdog();
  gpio_setup();
  timer7_setup();

  GPIO1_SETDATAOUT = LED_WHITE;
  uart0_write_str("CALIBRANDO SENSORES, ESPERE UM POUCO...\r\n");
  _delay_sec(10);
  GPIO1_CLEARDATAOUT = LED_WHITE;
  uart0_write_str("SENSORS CALIBRADOS :)\r\n");
    
    //Loop principal
  while (1){
    char entrada = uart0_getc();
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
