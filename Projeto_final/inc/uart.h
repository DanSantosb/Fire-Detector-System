#ifndef UART_H
#define UART_H
#include <stdint.h>


void uart0_write(const char *str);
void uart0_writeln(const char *str);
char uart0_getch();
void uart0_getln(char* buffer);


#endif