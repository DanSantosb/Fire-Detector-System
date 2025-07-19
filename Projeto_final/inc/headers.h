#ifndef DEFINES_H
#define DEFINES_H
#define WDT1_BASE   0x44E35000
#define WDT_WSPR    (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS    (*(volatile uint32_t *)(WDT1_BASE + 0x34))

//============================== ATIVAÇÃO DO CLOCKS (GPIO1, ADC E TIME) ================================
#define CM_PER_BASE             0x44E00000  //Clock Module GPIO1 Registers
#define CM_PER_GPIO1_CLKCTRL    (*(volatile uint32_t*)(CM_PER_BASE + 0xAC))
#define CM_PER_TIMER7_CLKCTRL   (*(volatile uint32_t *)(CM_PER_BASE + 0x7C))

#define CM_WKUP_BASE            0x44E00400 //Clock Module Wakeup Registers
#define CM_WKUP_ADC_TSC_CLKCTRL (*(volatile uint32_t *)(CM_WKUP_BASE + 0xBC))

//======================================== CONFIG RESISTRADORES DO GPIO1 ===============================
#define GPIO1_BASE              0x4804C000
#define GPIO1_OE                (*(volatile uint32_t*)(GPIO1_BASE + 0x134))
#define GPIO1_CLEARDATAOUT      (*(volatile uint32_t*)(GPIO1_BASE + 0x190))
#define GPIO1_SETDATAOUT        (*(volatile uint32_t*)(GPIO1_BASE + 0x194))
#define GPIO1_DATAIN            (*(volatile uint32_t*)(GPIO1_BASE + 0x138))
#define GPIO1_IRQSTATUS_SET_0   (*(volatile uint32_t*)(GPIO1_BASE + 0x34))
#define GPIO1_IRQSTATUS_0       (*(volatile uint32_t*)(GPIO1_BASE + 0x2C))
#define GPIO1_RISINGDETECT      (*(volatile uint32_t*)(GPIO1_BASE + 0x148))

//++++++++++++++++++++++++++++++++++++++++ CONFIG RESISTRADORES DO ADC ==================================
#define ADC_TSC_BASE                0x44E0D000
#define ADC_CTRL                    (*(volatile uint32_t *)(ADC_TSC_BASE + 0x40))
#define ADC_CLKDIV                  (*(volatile uint32_t *)(ADC_TSC_BASE + 0x4C))
#define ADC_STEPENABLE              (*(volatile uint32_t *)(ADC_TSC_BASE + 0x54))
#define ADC_STEPCONFIG1             (*(volatile uint32_t *)(ADC_TSC_BASE + 0x64))
#define ADC_STEPDELAY1              (*(volatile uint32_t *)(ADC_TSC_BASE + 0x68))
#define ADC_STEPCONFIG2             (*(volatile uint32_t *)(ADC_TSC_BASE + 0x6C))
#define ADC_STEPDELAY2              (*(volatile uint32_t *)(ADC_TSC_BASE + 0x70))
#define ADC_FIFO0COUNT              (*(volatile uint32_t *)(ADC_TSC_BASE + 0xE4))
#define ADC_FIFO0DATA               (*(volatile uint32_t *)(ADC_TSC_BASE + 0x100))

//====================================== CONFIG RESISTRADORES DO WATCHDOG ===============================
#define WDT1_BASE   0x44E35000
#define WDT_WSPR    (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS    (*(volatile uint32_t *)(WDT1_BASE + 0x34))

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
#define CONF_GPMC_AD12      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x830)) // MUX DO LED WHITE
#define CONF_GPMC_AD13      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x834)) // MUX DO LED RED
#define CONF_GPMC_AD14      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x838)) // MUX DO LED YELLOW
#define CONF_GPMC_AD15      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x83C)) // MUX DO LED GREEN
#define CONF_GPMC_BE1N      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x878)) // MUX DO RESET
#define CONF_GPMC_A0        (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x840)) // MUX DO BUZZER

//LCD 
#define CONF_GPMC_AD6       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x818)) // MUX D0    PIN3
#define CONF_GPMC_AD7       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x81C)) // MUX D1    PIN4
#define CONF_GPMC_AD2       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x808)) // MUX D2    PIN5
#define CONF_GPMC_AD3       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x80C)) // MUX D3    PIN6
#define CONF_GPMC_CSN1      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x880)) // MUX D4    PIN21
#define CONF_GPMC_CSN2      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x884)) // MUX D5    PIN20
#define CONF_GPMC_AD5       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x814)) // MUX D6    PIN22
#define CONF_GPMC_AD4       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x810)) // MUX D7    PIN23
#define CONF_GPMC_AD1       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x804)) // MUX DO E  PIN24
#define CONF_GPMC_AD0       (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x800)) // MUX DO RS PIN25
#define CONF_GPMC_CSN0      (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x87C)) // MUX DO RW PIN26

#define CONF_AIN0           (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x984)) // P9_39 MUX DO MQ2ADC_STEPDELAY2 ANALOGICO
#define CONF_AIN1           (*(volatile uint32_t*)(CONTROL_MODULE_BASE + 0x988)) // P9_40 MUX DO MQ6 ANALOGICO

//=========================================== INTERRUPÇÃO =================================================
#define INTC_BASE       0x48200000
//VERIFICAR SE A INTERRUPÇÃO DO ADC ESTÁ NO 3º VETOR
//OLHAR SE O VETOR 3 TEM INTERRUPÇÃO POR VALOR LOGICO 
#define INTC_MIR_CLEAR3 (*(volatile uint32_t*)(INTC_BASE + 0xE8)) // VETOR DA INTERRUPÇÃO DO GPIO
#define INTC_MIR_CLEAR2 (*(volatile uint32_t*)(INTC_BASE + 0xC8)) // VETOR DA INTERRUPÇÃO DO TIME
#define INTC_SIR_IRQ    (*(volatile uint32_t*)(INTC_BASE + 0x40))
#define INTC_CONTROL    (*(volatile uint32_t*)(INTC_BASE + 0x48))

//========================================== CONFIG UART ==================================================
#define UART0_BASE      0x44E09000
#define UART0_THR       (*(volatile unsigned int *)(UART0_BASE + 0x00))  // Registrador de Transmissão (que é o mesmo que o de Recepção)
#define UART0_LSR       (*(volatile unsigned int *)(UART0_BASE + 0x14))  // Base do LSR
#define UART0_LSR_THRE  (1 << 5)                                         // Avisa que registrador THR transmite dados
#define UART0_LSR_RHRE  (1 << 0)                                         // Avisa que registrador RHR recebe dados

//============================================== DEF PIN ==================================================
#define BUZZER            (1 << 16) // P9_15 (PINO DO BUZZER)
#define BUTTON_RESET_PIN  (1 << 28) // P9_12 (BOTAO DE RESET)
#define LED_WHITE         (1 << 12) // P8_12 (PINO DO LED WHITE)
#define LED_RED           (1 << 13) // P8_11 (PINO DO LED RED)
#define LED_YELLOW        (1 << 14) // P8_16 (PINO DO LED YELLOW)
#define LED_GREEN         (1 << 15) // P8_15 (PINO DO LED GREEN)
#define LED_INT           (1 << 21)


#define LCD_REGISTER_SELECT (1 << 0)        //P8_4 (RS DISPLAY)   // 0 = Comando | 1 = Dado
#define LCD_READ_WRITE (1 << 29)            //P8_4 (RW DISPLAY) 
#define LCD_ENABLE (1 << 1)                 //P8_24 (E DISPLAY)
#define LCD_DATA0 (1 << 6)                  //P8_3  (D0 DISPLAY)  
#define LCD_DATA1 (1 << 7)                  //P8_4 (D1 DISPLAY)
#define LCD_DATA2 (1 << 2)                  //P8_5 (D2 DISPLAY) 
#define LCD_DATA3 (1 << 3)                  //P8_6 (D3 DISPLAY) 
#define LCD_DATA4 (1 << 31)                 //P8_20 (D4 DISPLAY) 
#define LCD_DATA5 (1 << 30)                 //P8_21 (D5 DISPLAY)
#define LCD_DATA6 (1 << 5)                  //P8_22 (D6 DISPLAY)
#define LCD_DATA7 (1 << 4)                  //P8_23 (D7 DISPLAY)

#define CMD_CLEARDISPLAY                0x1
#define CMD_RETURNHOME                  0x2
#define CMD_ENTRYMODESET(id, s)         (0x4 | (s << 0) | (id << 1))
#define CMD_DISPLAYONOFF(d, c, b)       (0x8 | (d << 2) | (c << 1) | (b << 0))
#define CMD_FUNCTIONSET(dl, ft1, ft0)   (0x28 | (ft0 << 0) | (ft1 << 1) | (dl << 4))
#define CMD_SETCURSOR(addr)             (0b10000000 | (addr))
#define INICIO_LINHA1                   0x0000000
#define INICIO_LINHA2                   0b1000000

//======================================== CONFIG RESISTRADORES DO TIME ===============================
#define DM_TIMER7_BASE            0x4804A000
#define DM_TIMER7_TCLR            (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x38)) 
#define DM_TIMER7_IRQENABLE_SET   (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x2C))
#define DM_TIMER7_IRQENABLE_CLEAR (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x30))
#define DM_TIMER7_IRQSTATUS       (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x28))
#define DM_TIMER7_IRQ_EOI         (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x20))
#define DM_TIMER7_TSICR           (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x54))
#define DM_TIMER7_TCRR            (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x3C))
#define DM_TIMER7_TLDR            (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x4C))
#define DM_TIMER7_TSICR           (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x54))
#define DM_TIMER7_TWPS            (*(volatile unsigned int*)(DM_TIMER7_BASE + 0x48))
#define DM_TIMER7_START_STOP_TIMER      (1 << 0)
#define DM_TIMER7_AUTORELOAD            (1 << 1)
#define DM_TIMER7_PRESCALER_ENABLE      (1 << 5)

#define pulsosPorSegundo                24000000
#endif 
