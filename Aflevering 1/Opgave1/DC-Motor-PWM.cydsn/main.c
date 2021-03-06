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
#include "project.h"

CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived);
void decreaseSpeed(void);
void increaseSpeed(void);
void driveForwards(void);
void driveBackwards(void);
void stop(void);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    PWM_1_Start();
    
    UART_1_PutString("DC-Motor-PWM application started\r\n");
    UART_1_PutString("0: Stop\r\n");
    UART_1_PutString("1: Drive forwards\r\n");
    UART_1_PutString("2: Drive backwards\r\n");
    UART_1_PutString("q: Decrease speed\r\n");
    UART_1_PutString("w: Increase speed\r\n");
    
    Pin_Out1_Write(0xFF);
    Pin_Out2_Write(0);       

    for(;;)
    {
        /* Place your application code here. */
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
    //Switch funktionen herunder bruges til at kunne kalde de forskellige fukntioner ud fra tasterne på PC'en ved hjælp af UART
    switch(byteReceived)
    {
        case 'q' :
        {
            decreaseSpeed();
        }
        break;
        case 'w' :
        {
            increaseSpeed();
        }
        break;
        case '1' :
        {
            driveForwards();
        }
        break;
        case '2' :
        {
            driveBackwards();
        }
        break;
        case '0' :
        {
            stop();
            
        }
        break;
        default :
        {
            // nothing
        }
        break;
    }
}

void decreaseSpeed()
{
    UART_1_PutString("Decreasing speed\r\n");
    
    //Her minuser vi PWM signalet med 5%
    PWM_1_WriteCompare(PWM_1_ReadCompare() - 5);
}

void increaseSpeed()
{
    UART_1_PutString("Increasing speed\r\n");
    
    //Her pluser vi PWM signalet med 5%
    PWM_1_WriteCompare(PWM_1_ReadCompare() + 5);
}

void driveForwards()
{
    UART_1_PutString("Set direction: forwards\r\n");   
    
    //For at motoren køre frem
    
    //Pin_Out1 sættes høj
    Pin_Out1_Write(0xFF);
    //Pin_Out2 sættes lavt
    Pin_Out2_Write(0);
}

void driveBackwards()
{
    UART_1_PutString("Set direction: backwards\r\n");
    
    //For at motoren køre tilbage
    
    //Pin_Out1 sættes lav
    Pin_Out1_Write(0);    
    //Pin_Out2 sættes højt
    Pin_Out2_Write(0xFF);
}

void stop()
{
    UART_1_PutString("Stop\r\n");
    
    //Her sætter vi PWM signalet til 0%, sådan motoren stopper
    PWM_1_WriteCompare(0);
}

/* [] END OF FILE */
