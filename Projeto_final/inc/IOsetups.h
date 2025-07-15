#ifndef IOSETUPS_H
#define IOSETUPS_H
#include "../inc/defines.h"
#include "../inc/timer7.h"
#include "../inc/uart.h"

//================================================= CONFIG ADC ==============================================
volatile unsigned int MQ2_VALUE;
volatile unsigned int MQ6_VALUE;

volatile bool button_reset_pressed = false;
volatile bool button_mode_pressed = false;
//================================================= CONFIG GPIO1 ============================================
void gpio_setup(void){

  // Ativa clock do GPIO1
  CM_PER_GPIO1_CLKCTRL |= 0x2;
  while(!(CM_PER_GPIO1_CLKCTRL & 0x3)); // Espera ativação
  
  CONF_GPMC_AD3   |= (7 | (1 << 5));  //SET O BOTÃO DE MODO NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD7   |= (7 | (1 << 5));  //SET O BOTÃO RESET NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_CSN1  |= (7 | (1 << 5));  //SET O MQ2 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_CSN2  |= (7 | (1 << 5));  //SET O MQ6 NO MOD7 E RECEPTOR ATIVO
  CONF_GPMC_AD6   |= 7;               //SET O DISPLAY NO MOD7 
  CONF_GPMC_AD5   |= 7;               //SET O BUZZER NO MOD7
  CONF_GPMC_AD12  |= 7;               //SET LED WHITE NO MOD7
  CONF_GPMC_AD13  |= 7;               //SET LED RED NO MOD7
  CONF_GPMC_AD14  |= 7;               //SET LED YELLOW NO MOD7
  CONF_GPMC_AD15  |= 7;               //SET LED GREEN NO MOD7

  // Configura pinos
  GPIO1_OE &= ~LED_YELLOW;          // LED YELLOW como saida
  GPIO1_OE &= ~LED_GREEN;           // LED GREEN como saída
  GPIO1_OE &= ~LED_WHITE;           // LED WHITE como saida
  GPIO1_OE &= ~LED_RED;             // LED RED como saida
  GPIO1_OE &= ~BUZZER;              // BUZZER como saida
  GPIO1_OE &= ~DISPLAY_PIN;         // DISPLAY como saida
  GPIO1_OE |= BUTTON_MODE_PIN;      // Botão MODE como entrada
  GPIO1_OE |= BUTTON_RESET_PIN;     // Botão RESET como entrada
  GPIO1_OE |= MQ2;                  // MQ2 como entrada
  GPIO1_OE |= MQ6;                  // MQ6 como entrada
  
//========================================= Configura interrupção ==========================================
  
  GPIO1_RISINGDETECT |= BUTTON_MODE_PIN;    // Borda de subida
  GPIO1_IRQSTATUS_0 = BUTTON_MODE_PIN;      // Limpa flag pendente
  GPIO1_IRQSTATUS_SET_0 = BUTTON_MODE_PIN;  // Habilita interrupção
  
  
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
    uart0_write_str("CALIBRANDO OS SENSORES, ESPERE UM POUCO...\r\n");
    _delay_sec(10);
    GPIO1_CLEARDATAOUT = LED_WHITE;
    uart0_write_str("SENSORES CALIBRADOS :)\r\n");
  }
}

void changeGreen(){
  GPIO1_CLEARDATAOUT = BUZZER;
  GPIO1_SETDATAOUT = LED_GREEN;
  uart0_write_str("TUDO TRANQUILO, METRICAS NORMAIS :)\r\n");
  GPIO1_CLEARDATAOUT = LED_RED;
  GPIO1_CLEARDATAOUT = LED_YELLOW;
}

void changeYellow(){
  GPIO1_CLEARDATAOUT = BUZZER;
  GPIO1_SETDATAOUT = LED_YELLOW;
  uart0_write_str("ATENCAO, NIVEIS DE GAS ESTÃO FICANDO PERIGOSOS\r\n");
  GPIO1_CLEARDATAOUT = LED_RED;
  GPIO1_CLEARDATAOUT = LED_GREEN;
}

void changeRed(){
  GPIO1_SETDATAOUT = LED_RED;
  uart0_write_str("CUIDADO !!! PERIGO EXTREMO\r\n");
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

void ISR_Handler(void) {
  uint32_t irq = INTC_SIR_IRQ & 0x7F;
  if (irq == 95) {
    overflow_counter++;
    DM_TIMER7_IRQ_EOI = 0x0;
  }
  if (irq == 98){
    gpio_isr_handler();
  }  
  INTC_CONTROL = 0x1;
}


#endif
