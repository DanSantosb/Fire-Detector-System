#include <math.h>
#include "../inc/headers.h"
#include "../inc/IOsetups.h"
#include "../inc/Sensor.h"

float adc_voltage(unsigned int adc_value) {
    return (adc_value / 4095.0f) * 1.8f;
}
void lerSensores(float* ppm_MQ2, float* ppm_MQ6) {
    unsigned int ain0, ain1;
    adc_read(&ain0, &ain1);

    float Vout_MQ2 = adc_voltage(ain0);
    float Vout_MQ6 = adc_voltage(ain1);

    float Rs_MQ2 = calcularRS(Vout_MQ2);
    float Rs_MQ6 = calcularRS(Vout_MQ6);

    float ratio_MQ2 = Rs_MQ2 / R0_MQ2;
    float ratio_MQ6 = Rs_MQ6 / R0_MQ6;

    *ppm_MQ2 = calcularPPM(ratio_MQ2, m_MQ2, b_MQ2);
    *ppm_MQ6 = calcularPPM(ratio_MQ6, m_MQ6, b_MQ6);
}
