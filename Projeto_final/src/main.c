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
  
  GPIO1_SETDATAOUT = LED_WHITE;
  uart0_write_str("CALIBRANDO SENSORES, ESPERE UM POUCO...\r\n");
  _delay_sec(300);
  GPIO1_CLEARDATAOUT = LED_WHITE;
  uart0_write_str("SENSORS CALIBRADOS :)\r\n");
    
    //Loop principal
  while (1){
    if (MQ2_VALUE | MQ6_VALUE == 1){
      changeGreen(); 
    } else if (MQ2_VALUE| MQ6_VALUE == 2){
      changeYellow();
    } else if (MQ2_VALUE| MQ6_VALUE == 3){
      changeRed();
    }
  }
  return 0;
} 
