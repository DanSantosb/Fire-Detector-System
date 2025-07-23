#include "../inc/headers.h"
#include "../inc/uart.h"



void uart0_putch(char c){
  while (!(UART0_LSR & UART0_LSR_THRE))
    __asm__ __volatile__("nop");
  UART0_THR = c;
}



void uart0_write(const char* str) {
  while (*str) {
    uart0_putch(*str++);
  }
}



char uart0_getch() {
  while (!(UART0_LSR & 0x01))
    __asm__ __volatile__("nop");
  return UART0_THR;
}



void uart0_getln(char* buffer) {
  unsigned int contador = 0;
  char currentChar = '0';

  while(1){
    currentChar = uart0_getch();

    if (currentChar == '\n' || currentChar == '\r'){
      buffer[contador] = '\0';
      uart0_putch('\n');
      uart0_putch('\r');
      break;
    } else if (currentChar == 127){
      if (contador == 0) continue;
      contador--;
      uart0_putch('\b');
      uart0_putch(' ');
      uart0_putch('\b');
    } else {
      buffer[contador] = currentChar;
      contador++;
      uart0_putch(currentChar);
    }
  }

  return;
}



void uart0_writeln(const char *str) {
  if (str[0] == '\0'){
    uart0_putch('\n');
    uart0_putch('\r');
    return;
  }
    
  while (*str) {
    uart0_putch(*str++);
  }

  uart0_putch('\n');
  uart0_putch('\r');
}