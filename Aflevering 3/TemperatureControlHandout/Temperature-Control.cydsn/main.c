    /* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdio.h>
#include "project.h"
#include "PIDControl.h"
#include "TempReader.h"
#include "UART.h"

static char outputBuffer[256];
static volatile float setPoint = 30; // degrees celcius

#define SAMPLES_PER_SECOND 3
static uint16_t sampleWaitTimeInMilliseconds = 1000 / SAMPLES_PER_SECOND;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    float Kp = 10.0f;
    float Ki = 1.0f/5.0f;
    float Kd = 0.0f;
    float integralMax = 3000;
    float integralMin = -3000;
    float temp = 0;
    float controlSignal = 0;

    float dt = ((float)sampleWaitTimeInMilliseconds) / 1000; // dt is measured in seconds
    PIDControl_init(Kp, Ki, Kd, integralMax, integralMin, dt);
    PIDControl_changeSetPoint(setPoint);
    
    isr_uart_rx_StartEx(isr_UART_rx);
    initComp(); /* Init all used components from topdesign */

    UART_1_PutString("TCA started\r\n");

    for(;;)
    {
        /* Place your application code here. */
        float error = setPoint - temp;
        float proportionalPart = 0;
        float integralPart = 0;
        float derivativePart = 0;
        setPoint = getSetPoint();
        
        temp = readTemp();
        
        controlSignal = PIDControl_doStep(temp, &proportionalPart, &integralPart, &derivativePart);            
        snprintf(outputBuffer, sizeof(outputBuffer), "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f \r\n", 
                                                      setPoint, temp, error, controlSignal, Kp, Ki, Kd, 
                                                      proportionalPart, integralPart, derivativePart);
        
        
        
        PWM_1_WriteCompare(controlSignal);
        
        UART_1_PutString(outputBuffer);

        CyDelay(sampleWaitTimeInMilliseconds);
    }
}

/* [] END OF FILE */
