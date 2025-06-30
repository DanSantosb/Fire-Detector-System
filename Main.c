#include <stdbool.h>
#include <stdint.h>

//====================================== CONFIG RESISTRADORES DO WATCHDOG ===============================
#define WDT1_BASE   0x44E35000
#define WDT_WSPR    (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS    (*(volatile uint32_t *)(WDT1_BASE + 0x34))

//========================================== ATIVAÇÃO DO CLOCK DO GPIO1 ==================================
#define CM_PER_BASE           0x44E00000
#define CM_PER_GPIO1_CLKCTRL  (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))

//======================================== CONFIG RESISTRADORES DO GPIO1 ===============================
#define GPIO1_BASE              0x4804C000
#define GPIO1_OE                (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_CLEARDATAOUT      (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_SETDATAOUT        (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO1_DATAIN            (*(volatile uint32_t*)(GPIO1_BASE + 0x138))
#define GPIO1_IRQSTATUS_SET_0   (*(volatile uint32_t*)(GPIO1_BASE + 0x34))
#define GPIO1_IRQSTATUS_0       (*(volatile uint32_t*)(GPIO1_BASE + 0x2C))
#define GPIO1_RISINGDETECT      (*(volatile uint32_t*)(GPIO1_BASE + 0x148))

//=========================================== MUX DECLARATION ============================================
#define CONTROL_MODULE_BASE 0x44E10000
#define CONF_GPMC_AD3       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x80C)) //MUX DO BOTÃO DE MODO
#define CONF_GPMC_AD5       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x814)) //MUX DO SOM
#define CONF_GPMC_AD6       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x818)) //MUX DO DISPLAY
#define CONF_GPMC_AD7       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x81C)) //MUX DO BOTÃO RESET
#define CONF_GPMC_AD12      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x830)) //MUX DO LED WHITE
#define CONF_GPMC_AD13      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x834)) //MUX DO LED RED
#define CONF_GPMC_AD14      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x838)) //MUX DO LED WELLOW
#define CONF_GPMC_AD15      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x83C)) //MUX DO LED GREEN
#define CONF_GPMC_CSN1      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x880)) //MUX DO MQ2
#define CONF_GPMC_CSN2      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x884)) //MUX DO MQ6

//=========================================== INTERRUPÇÃO =================================================
#define INTC_BASE       0x48200000
#define INTC_MIR_CLEAR3 (*(volatile uint32_t*)(INTC_BASE + 0xE8))
#define INTC_SIR_IRQ    (*(volatile uint32_t*)(INTC_BASE + 0x40))
#define INTC_CONTROL    (*(volatile uint32_t*)(INTC_BASE + 0x48))

//========================================== CONFIG UART ==================================================
#define UART0_BASE      0x44E09000
#define UART0_THR       (*(volatile unsigned int *)(UART0_BASE + 0x00))// Registrador de Transmissão (que é o mesmo que o de Recepção)
#define UART0_LSR       (*(volatile unsigned int *)(UART0_BASE + 0x14))// Base do LSR
#define UART0_LSR_THRE  (1 << 5)                                       // Avisa que registrador THR transmite dados
#define UART0_LSR_RHRE  (1 << 0)                                       // Avisa que registrador RHR recebe dados

//============================================== DEF PIN ==================================================
#define BUTTON_MODE_PIN   (1 << 3)  // P8_6  (BOTAO DE MODO)
#define BUZZER            (1 << 5)  // P8_22 (PINO DO BUZZER)
#define DISPLAY_PIN       (1 << 6)  // P8_3  (DISPLAY)
#define BUTTON_RESET_PIN  (1 << 7)  // P8_4  (BOTAO DE RESET)
#define LED_WHITE         (1 << 12) // P8_12 (PINO DO LED WHITE)
#define LED_RED           (1 << 13) // P8_11 (PINO DO LED RED)
#define LED_WELLOW        (1 << 14) // P8_16 (PINO DO LED WELLOW)
#define LED_GREEN         (1 << 15) // P8_15 (PINO DO LED GREEN)
#define MQ2               (1 << 30) // P8_21 (PINO DO MQ2)
#define MQ6               (1 << 31) // P8_20 (PINO DO MQ6)

//======================================= DECLARA O DELAY E INICIA OS BOTÕES =============================

void delay(uint32_t cycles) {
    volatile uint32_t i;
    for(i = 0; i < cycles; i++);
}

volatile bool button_mode_pressed = false;
volatile bool button_reset_pressed = false;

//============================================= FUNÇÕES DA UART ===========================================

unsigned char uart0_getc(){
  while (!(UART0_LSR & 0x01));
  return UART0_THR;
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

void printTextInt(){
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
}

//============================================= ENERGIZA OS LEDS ==============================================

void changeWhite(){
  if (button_reset_pressed){
    GPIO1_SETDATAOUT = LED_WHITE;
    uart0_write_str("CALIBRANDO OS SENSORES, ESPERE UM POUCO...\r\n");
    delay(300000);
    GPIO1_CLEARDATAOUT = LED_WHITE;
    uart0_write_str("SENSORES CALIBRADOS :)\r\n");
  }
}

void changeWellow(){
  if (button_mode_pressed){
    GPIO1_SETDATAOUT = LED_WELLOW;
  } else {
    GPIO1_CLEARDATAOUT = LED_WELLOW;
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
}

//================================================= CONFIG GPIO1 ============================================
void gpio_setup(void) {
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
    CONF_GPMC_AD14  |= 7;               //SET LED WELLOW NO MOD7
    CONF_GPMC_AD15  |= 7;               //SET LED GREEN NO MOD7

    // Configura pinos
    GPIO1_OE &= ~LED_WELLOW;          // LED WELLOW como saida
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
    GPIO1_RISINGDETECT |= BUTTON_MODE_PIN;    // Borda de subida
    GPIO1_IRQSTATUS_0 = BUTTON_MODE_PIN;      // Limpa flag pendente
    GPIO1_IRQSTATUS_SET_0 = BUTTON_MODE_PIN;  // Habilita interrupção

    //BOTÃO WHITE
    GPIO1_RISINGDETECT |= BUTTON_RESET_PIN;   // Borda de subida
    GPIO1_IRQSTATUS_0 = BUTTON_RESET_PIN;     // Limpa flag pendente
    GPIO1_IRQSTATUS_SET_0 = BUTTON_RESET_PIN; // Habilita interrupção
    
    // Configura INTC
    INTC_MIR_CLEAR3 = (1 << 2);  // Habilita IRQ 98 (GPIO1)
}
 
//============================================= FUNÇÕES COM INTERRUPÇÃO =====================================
void gpio_isr_handler(void) {
  if(GPIO1_IRQSTATUS_0 & BUTTON_MODE_PIN) {
    delay(1000000);
    GPIO1_IRQSTATUS_0 = BUTTON_MODE_PIN; // Limpa flag
    button_mode_pressed = !button_mode_pressed;
    uart0_write_str("<Botão 2>\r\n");
    printTextRed();
  }
  if(GPIO1_IRQSTATUS_0 & BUTTON_RESET_PIN) {
    delay(1000000);
    GPIO1_IRQSTATUS_0 = BUTTON_RESET_PIN; // Limpa flag
    button_reset_pressed = !button_reset_pressed;
    changeWhite();
  }
  uart0_write_str("\r\n");
}


void ISR_Handler(void) {
  uint32_t irq_number = INTC_SIR_IRQ & 0x7F;
  if(irq_number == 98) {  // IRQ do GPIO1
      gpio_isr_handler();
  }
  INTC_CONTROL = 0x1;     // Reabilita interrupções
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
    uart0_write_str("----------------------------------------\r\n");
    uart0_write_str("             MENU DO PROJETO\r\n");
    uart0_write_str("----------------------------------------\r\n\n");
    uart0_write_str("BOTÃO 1) LED INTERNO\r\n");
    uart0_write_str("BOTÃO 2) LED VERMELHO\r\n");
    uart0_write_str("BOTÃO 3) LED BRANCO\r\n\n");
    uart0_write_str("----------------------------------------\r\n");
    
    // Loop principal
  while (1){
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
      printTextWhite();
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
    
  }
    return 0;
}
