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


int main(void) {
    disable_watchdog();
    gpio_setup();

    while(1){
      __asm__ __volatile__("nop");
      _delay_sec(2);
    }
    // Loop principal
  /*while (1){
    char entrada = uart0_getc();

    if (entrada - '0' == 1){
      uart0_write_int(1);
      delay(1000000);
      button_mode_pressed = !button_mode_pressed;
      printTextInt();
      changeInt();
    } else if (entrada - '0' == 2){
      uart0_write_int(2);
      delay(1000000);
      button_reset_pressed = !button_reset_pressed;
      printTextRed();
      changeRed();
    } else if (entrada - '0' == 3){
      uart0_write_int(3);
      delay(1000000);
      button_white_pressed = !button_white_pressed;
      changeWhite();
    } else if (entrada == 'h'){
      uart0_write_str("\r\n");
      uart0_write_str("----------------------------------------\r\n");
      uart0_write_str("             MENU DO PROJETO\r\n");
      uart0_write_str("----------------------------------------\r\n\n");
      uart0_write_str("BOTÃO 1) LED INTERNO\r\n");
      uart0_write_str("BOTÃO 2) LED VERMELHO\r\n");
      uart0_write_str("BOTÃO 3) LED BRANCO\r\n\n");
      uart0_write_str("----------------------------------------\r\n");
      uart0_write_str("\r\n");
    }

    uart0_write_str("\r\n");
    
  }*/
  return 0;
} 
