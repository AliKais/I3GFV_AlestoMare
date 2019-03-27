#include "UART.h"
#include "PIDControl.h"

CY_ISR(isr_UART_rx)
{

    uint8_t input = UART_1_ReadRxData();
    
    if(input == 'f')
    {
        PIDControl_changeSetPoint(50);
        UART_1_PutString("SetPoint updated: 50 celcius\r\n");
    }
    else if (input == 't')
    {
        PIDControl_changeSetPoint(30);
        UART_1_PutString("SetPoint updated: 30 celcius\r\n");
    }
    
}

/* [] END OF FILE */
