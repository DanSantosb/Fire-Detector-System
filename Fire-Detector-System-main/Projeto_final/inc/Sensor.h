#ifndef SENSOR_H
#include <math.h>

const float Vcc = 5.0;  //Tensão padrão de 5v
const float RL = 20.0;  //Resistor de carga interno

const float R0_MQ2 = 100;   //Calabrindo com 1000ppm
const float R0_MQ6 = 100;   //Calibrando com 1000ppm

//Curvas dos datasheets
const float m_MQ2 = -0.45;
const float b_MQ2 = 1.6;

const float m_MQ6 = -0.38;
const float b_MQ6 = 1.52;

const int LIMITE_MQ2 = 100;  //PPM
const int LIMITE_MQ6 = 100;  //PPM

float calcularRS(float Vout){
    return (Vcc - Vout) * RL/Vout;
}
float calcularPPM(float ratio, float m, float b) {
  return pow(10, (m * log10(ratio) + b));
}

#endif