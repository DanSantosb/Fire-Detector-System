#ifndef IOSETUPS_H
#define IOSETUPS_H
#include "../inc/defines.h"
#include "../inc/timer7.h"
#include "../inc/uart.h"

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
    
    //BOTÃO RED
    /*GPIO1_RISINGDETECT |= BUTTON_MODE_PIN;    // Borda de subida
    GPIO1_IRQSTATUS_0 = BUTTON_MODE_PIN;      // Limpa flag pendente
    GPIO1_IRQSTATUS_SET_0 = BUTTON_MODE_PIN;  // Habilita interrupção
    */
    //BOTÃO WHITE
    GPIO1_RISINGDETECT |= BUTTON_RESET_PIN;   // Borda de subida
    GPIO1_IRQSTATUS_0 = BUTTON_RESET_PIN;     // Limpa flag pendente
    GPIO1_IRQSTATUS_SET_0 = BUTTON_RESET_PIN; // Habilita interrupção
    
    // Configura INTC
    INTC_MIR_CLEAR3 = (1 << 2);  // Habilita IRQ 98 (GPIO1)
}

//============================================= ENERGIZA OS LEDS ==============================================

void changeWhite(){
  GPIO1_SETDATAOUT = LED_RED;
  uart0_write_str("CALIBRANDO OS SENSORES, ESPERE UM POUCO...\r\n");
  _delay_sec(5);
  GPIO1_CLEARDATAOUT = LED_RED;
  uart0_write_str("SENSORES CALIBRADOS :)\r\n");
}

/*void changeYellow(){
  if (button_mode_pressed){
    GPIO1_SETDATAOUT = LED_YELLOW;
  } else {
    GPIO1_CLEARDATAOUT = LED_YELLOW;
  }
}

void changeRed(){
  if (button_reset_pressed){
    GPIO1_SETDATAOUT = LED_RED;
  } else {
    GPIO1_CLEARDATAOUT = LED_RED;
  }
}


void changeGreen(){
  if (button_white_pressed){
    GPIO1_SETDATAOUT = LED_GREEN;
  } else {
    GPIO1_CLEARDATAOUT = LED_GREEN;
  }
}*/


//============================================= FUNÇÕES COM INTERRUPÇÃO =====================================
void gpio_isr_handler(void) {
  /*if(GPIO1_IRQSTATUS_0 & BUTTON_MODE_PIN) {
    delay(1000000);
    GPIO1_IRQSTATUS_0 = BUTTON_MODE_PIN; // Limpa flag
    button_mode_pressed = !button_mode_pressed;
    uart0_write_str("<Botão 2>\r\n");
    printTextRed();
  }*/
  if(GPIO1_IRQSTATUS_0 & BUTTON_RESET_PIN) {
    _delay_sec(2);
    GPIO1_IRQSTATUS_0 = BUTTON_RESET_PIN; // Limpa flag
    changeWhite();
  }
  uart0_write_str("\r\n");
}

//volatile bool button_mode_pressed = false;
volatile bool button_reset_pressed = false;


void ISR_Handler(void) {
  uint32_t irq = INTC_SIR_IRQ & 0x7F;
  if (irq == 95) {
    overflow_counter++;
    DM_TIMER7_IRQ_EOI = 0x0;
  }
  if (irq == 98){
    if (GPIO1_IRQSTATUS_0 & BUTTON_RESET_PIN){
      button_reset_pressed=!button_reset_pressed;
      GPIO1_IRQSTATUS_0 &= ~GPIO1_IRQSTATUS_0;
    }
  }
}


#endif