#include <stdbool.h>
#include <stdint.h>
#include "../inc/headers.h"
#include "../inc/IOsetups.h"
#include "../inc/timer.h"


uint16_t barramentoDeDados[] = {LCD_DATA0, LCD_DATA1, LCD_DATA2, LCD_DATA3, LCD_DATA4, LCD_DATA5, LCD_DATA6, LCD_DATA7};

void pinMode(uint32_t bitmask, bool isOutput) {
    if (isOutput)
        GPIO1_OE &= ~bitmask;  
    else
        GPIO1_OE |= bitmask;   
}

void digitalWrite(uint32_t bitmask, bool value) {
    if (value)
        GPIO1_SETDATAOUT = bitmask;
    else
        GPIO1_CLEARDATAOUT = bitmask;
}

bool digitalRead(uint32_t bitmask) {
    if (GPIO1_DATAIN & bitmask) {
        return true;
    } else {
        return false;
    }
}

void setupForRead(){
  for (int i = 0; i < 8; i++){
    pinMode(barramentoDeDados[i], false);
  }
}

void setupForWrite(){
  for (int i = 0; i < 8; i++){
    pinMode(barramentoDeDados[i], true);
  }
}



void writeCMDToLCD(uint16_t data){
  setupForWrite();

  digitalWrite(LCD_REGISTER_SELECT, false);  // RS = 0 → comando
  digitalWrite(LCD_READ_WRITE, false);       // RW = 0 → escrita
  digitalWrite(LCD_ENABLE, false);
  _delay_ms(1);

  for (uint8_t i = 0; i < 8; i++){
    digitalWrite(barramentoDeDados[i], (data >> i) & 0x01);         // Prepara e estabiliza o dado.
  }

  _delay_ms(1);                    // Setup time (tDS68)
  digitalWrite(LCD_ENABLE, true);
  _delay_ms(1);                    // Enable pulse width (tPW68)
  digitalWrite(LCD_ENABLE, false);
  _delay_ms(1);
}



bool readCMDBitFromLCD(uint8_t bit){
  setupForRead();
  uint8_t buffer[11];
  digitalWrite(LCD_REGISTER_SELECT, false);  // RS = 0 → comando
  digitalWrite(LCD_READ_WRITE, true);
  digitalWrite(LCD_ENABLE, false);
  _delay_ms(1);
  digitalWrite(LCD_ENABLE, true);   // Flanco de subida → LCD coloca dado no barramento
  _delay_ms(1);             // Tempo mínimo de setup

  for (uint8_t i = 0; i < 8; i++) {
    buffer[i] = digitalRead(barramentoDeDados[i]);
  }

  digitalWrite(LCD_ENABLE, false);


  return buffer[bit];
}

void writeCharToLCD(char c) {
  setupForWrite();

  digitalWrite(LCD_REGISTER_SELECT, true);  // RS = 1 → dado
  digitalWrite(LCD_READ_WRITE, false);        // RW = 0 → escrita
  digitalWrite(LCD_ENABLE, false);

  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(barramentoDeDados[i], (c >> i) & 0x01);
  }

  _delay_ms(1);
  digitalWrite(LCD_ENABLE, true);
  _delay_ms(1);
  digitalWrite(LCD_ENABLE, false);
  _delay_ms(50); // necessário para dar tempo de processar o caractere
}

void writeStringToLCD(const char* s) {
  while (*s) {
    writeCharToLCD(*s++);
  }
}



int main() {
  // Configuração dos pinos de controle
  pinMode(LCD_ENABLE, true);
  pinMode(LCD_READ_WRITE, true);
  pinMode(LCD_REGISTER_SELECT, true);

  _delay_ms(200);  // Aguarda LCD iniciar (obrigatório após power-on)

  writeCMDToLCD(CMD_FUNCTIONSET(1, 0, 0));                    // Function set: 8-bit, 2 linhas, fonte 5x8
  _delay_ms(1);
  writeCMDToLCD(CMD_DISPLAYONOFF(1, 1, 1));                   // Display ON, cursor ON, blink ON
  _delay_ms(1);
  writeCMDToLCD(CMD_CLEARDISPLAY);                            // Limpa a tela
  _delay_ms(2);                                                   // Esse comando exige >1.52ms
  writeCMDToLCD(CMD_ENTRYMODESET(1, 0));                      // Cursor incrementa, sem shift
  _delay_ms(1);

  writeStringToLCD("CALIBRANDO ");
  _delay_ms(1);
  writeStringToLCD(" SEN-");
  _delay_ms(1);
  writeCMDToLCD(CMD_SETCURSOR(INICIO_LINHA2));
  _delay_ms(1);
  writeStringToLCD("SORES! AGUARDE.");

}
