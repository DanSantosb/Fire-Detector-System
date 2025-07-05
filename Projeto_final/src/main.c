#include <stdbool.h>
#include <stdint.h>
#include "../inc/defines.h"

//======================================= DECLARA O DELAY E INICIA OS BOTÕES =============================
volatile unsigned int overflow_counter = 0;

void _delay_sec(unsigned int secs) {
  DM_TIMER7_TCLR |= DM_TIMER7_START_STOP_TIMER;
  uint64_t alvo = (uint64_t)secs * pulsosPorSegundo;
  while (((uint64_t)overflow_counter << 32) + DM_TIMER7_TCRR < alvo);
  DM_TIMER7_TCLR &= ~DM_TIMER7_START_STOP_TIMER;
  DM_TIMER7_TCRR = 0x0;
  overflow_counter = 0;
}

void timer7_setup() { //CONFIGURA TIME
  DM_TIMER7_TCLR = 0x0;
  DM_TIMER7_TLDR = 0x0;
  DM_TIMER7_TCRR = 0x0;

  DM_TIMER7_TCLR |= DM_TIMER7_AUTORELOAD;
  DM_TIMER7_IRQENABLE_SET |= (1 << 1); // Habilita interrupção por overflow
  INTC_MIR_CLEAR2 = (1 << 31); // IRQ 95 → Timer7
}

//volatile bool button_mode_pressed = false;
volatile bool button_reset_pressed = false;
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

//============================================= ENERGIZA OS LEDS ==============================================

void changeWhite(){
  GPIO1_SETDATAOUT = LED_RED;
  uart0_write_str("CALIBRANDO OS SENSORES, ESPERE UM POUCO...\r\n");
  delay(999999999);
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
    delay(100000);
    GPIO1_IRQSTATUS_0 = BUTTON_RESET_PIN; // Limpa flag
    changeWhite();
  }
  uart0_write_str("\r\n");
}


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
      delay(300);
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
