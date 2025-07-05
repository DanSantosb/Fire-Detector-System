#ifndef UART_H
#define UART_H
#include "../inc/defines.h"

//============================================= FUNÇÕES DA UART ===========================================

unsigned char uart0_getc(){
  while (!(UART0_LSR & 0x01));
  return UART0_THR;
}

void uart0_putc(char c) {
    while (!(UART0_LSR & UART0_LSR_THRE));
    UART0_THR = c;
}

void uart0_write_str(const char *str) {
    while (*str) {
        uart0_putc(*str++);
    }
}

void uart0_write_int(unsigned char inteiro) {
    int resto = 0;
    int contador = 0;
    char final[100];
    char temp[100];

    while (inteiro){
        resto = inteiro % 10;
        temp[contador] = resto + '0';
        inteiro /= 10;
        contador++;
    }

    for (int i = 0; i < contador; i++){
        final[i] = temp[contador - i - 1];
    }

    final[contador] = '\r';
    final[contador+1] = '\n';

    uart0_write_str(final);
}

/*void printTextInt(){
  if (button_mode_pressed){
    uart0_write_str("LED INTERNO ACESO\r\n");
  } else {
    uart0_write_str("LED INTERNO APAGADO\r\n");
  }
}
void printTextRed(){
  if (button_reset_pressed){
    uart0_write_str("LED VERMELHO ACESO\r\n");
  } else {
    uart0_write_str("LED VERMELHO APAGADO\r\n");
  }
}*/


#endif