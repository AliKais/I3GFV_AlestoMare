#include "project.h"
#include <stdio.h>
#include <stdbool.h>
#include "measure.h"

CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    
    uartADCinit();
    
    UART_1_PutString("Scale application started\r\n");

    for(;;)
    {
        /* Place your application code here. */
        if (ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT))
        {            
            measure();
        }
        CyDelay(100);
    }
}

CY_ISR(ISR_UART_rx_handler)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize();
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();
        UART_1_WriteTxData(byteReceived); // echo back
        handleByteReceived(byteReceived);
        bytesToRead--;
    }
}

void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case 'q' :
        {
            set(1);
        }
        break;
        
        case 'w' :
        {
            set(0);
        }
        break;
        
        default :
        {
            // nothing
        }
        break;
    }
}

/* [] END OF FILE */
