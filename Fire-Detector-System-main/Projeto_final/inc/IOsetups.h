#ifndef IOSETUPS_H
#define IOSETUPS_H
#include "../inc/headers.h"
#include "../inc/timer.h"
#include "../inc/uart.h"
#include <stdbool.h>

volatile bool button_reset_pressed = false;

//================================================= CONFIG GPIO1 ============================================
void gpio_setup(void){

  // Ativa clock do GPIO1
  CM_PER_GPIO1_CLKCTRL |= 0x2;
  while(!(CM_PER_GPIO1_CLKCTRL & 0x3)); // Espera ativação
  
  CONF_GPMC_AD3   |= (7 | (1 << 5));  //SET O D3 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD7   |= (7 | (1 << 5));  //SET O D1 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD6   |= (7 | (1 << 5));  //SET O D0 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD4   |= (7 | (1 << 5));  //SET O D7 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD5   |= (7 | (1 << 5));  //SET O D6 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD2   |= (7 | (1 << 5));  //SET O D2 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_CSN1  |= (7 | (1 << 5));  //SET O D4 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_CSN2  |= (7 | (1 << 5));  //SET O D5 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD1   |= (7 | (1 << 5));  //SET O E NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD0   |= (7 | (1 << 5));  //SET O RS NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_CSN0  |= (7 | (1 << 5));  //SET O RW NO MOD7 E RECEPTOR ATIVO

  CONF_GPMC_AD12  |= 7;               //SET LED WHITE NO MOD7
  CONF_GPMC_AD13  |= 7;               //SET LED RED NO MOD7
  CONF_GPMC_AD14  |= 7;               //SET LED YELLOW NO MOD7
  CONF_GPMC_AD15  |= 7;               //SET LED GREEN NO MOD7
  CONF_GPMC_A0    |= 7;               //SET O BUZZER NO MOD7
  CONF_GPMC_BE1N  |= (7 | (1 << 5));  //SET O RESET NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_A2    |= (7 | (1 << 5));  //SET O MQ2 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_A3    |= (7 | (1 << 5));  //SET O M16 NO MOD7 E RECEPTOR ATIVO 

  // Configura pinos
  GPIO1_OE &= ~LED_YELLOW;          // LED YELLOW como saida
  GPIO1_OE &= ~LED_GREEN;           // LED GREEN como saída
  GPIO1_OE &= ~LED_WHITE;           // LED WHITE como saida
  GPIO1_OE &= ~LED_RED;             // LED RED como saida
  GPIO1_OE &= ~BUZZER;              // BUZZER como saida
  GPIO1_OE |= BUTTON_RESET_PIN;     // Botão RESET como entrada
  GPIO1_OE |= MQ2;                  // MQ2 como estrada
  GPIO1_OE |= MQ6;                  // MQ6 como entrada

  GPIO1_OE |= LCD_DATA0;
  GPIO1_OE |= LCD_DATA1;
  GPIO1_OE |= LCD_DATA2;
  GPIO1_OE |= LCD_DATA3;
  GPIO1_OE |= LCD_DATA4;
  GPIO1_OE |= LCD_DATA5;
  GPIO1_OE |= LCD_DATA6;
  GPIO1_OE |= LCD_DATA7;
  GPIO1_OE |= LCD_ENABLE ;
  GPIO1_OE |= LCD_REGISTER_SELECT;
  GPIO1_OE |= LCD_READ_WRITE;

//========================================= Configura interrupção ==========================================
  
  GPIO1_RISINGDETECT |= BUTTON_RESET_PIN;   // Borda de subida
  GPIO1_IRQSTATUS_0 = BUTTON_RESET_PIN;     // Limpa flag pendente
  GPIO1_IRQSTATUS_SET_0 = BUTTON_RESET_PIN; // Habilita interrupção
  
  // Configura INTC
  INTC_MIR_CLEAR3 = (1 << 2);  // Habilita IRQ 98 (GPIO1)
}

//============================================= ON/OFF OS LEDS ==============================================

void changeWhite(){
  if (button_reset_pressed){
    GPIO1_CLEARDATAOUT = BUZZER;
    GPIO1_CLEARDATAOUT = LED_GREEN;
    GPIO1_CLEARDATAOUT = LED_YELLOW;
    GPIO1_CLEARDATAOUT = LED_RED;
    GPIO1_SETDATAOUT = LED_WHITE;
    uart0_writeln("CALIBRANDO OS SENSORES, ESPERE UM POUCO...\r\n");
    _delay_sec(10);
    GPIO1_CLEARDATAOUT = LED_WHITE;
    uart0_writeln("SENSORES CALIBRADOS :)\r\n");
  }
}

void changeGreen(){
  GPIO1_CLEARDATAOUT = BUZZER;
  GPIO1_SETDATAOUT = LED_GREEN;
  uart0_writeln("TUDO TRANQUILO, METRICAS NORMAIS :)\r\n");
  GPIO1_CLEARDATAOUT = LED_RED;
  GPIO1_CLEARDATAOUT = LED_YELLOW;
}

void changeRed(){
  GPIO1_SETDATAOUT = LED_RED;
  uart0_writeln("CUIDADO !!! PERIGO EXTREMO\r\n");
  GPIO1_SETDATAOUT = BUZZER;
  GPIO1_CLEARDATAOUT = LED_YELLOW;
  GPIO1_CLEARDATAOUT = LED_GREEN;
}

//============================================= FUNÇÕES COM INTERRUPÇÃO =====================================
void gpio_isr_handler(void) {
  if(GPIO1_IRQSTATUS_0 & BUTTON_RESET_PIN) {
    _delay_sec(1);
    GPIO1_IRQSTATUS_0 = BUTTON_RESET_PIN; // Limpa flag
    button_reset_pressed = !button_reset_pressed;
    changeWhite();
    button_reset_pressed = false;
  }
}
#endif
