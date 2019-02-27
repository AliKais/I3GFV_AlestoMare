#include "project.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived); // Handle recieved bytes
void runStep(u_int8_t stepstate); // Cases with all possible modes
void waveDrive(bool direction); // Start waveDrive with direction
void fullStep(bool direction); // Start Full step with direction
void halfStep(bool direction); // Start Half step with direction
void fullRotation(bool direction); // One full rotation with direction

static uint16_t speed = 100; //Delay til at ændre dutycycle
static bool clockWise = true; //true = clockWise. false = counterclockwise
static uint8_t state = 0; // 1 = waveDrive. 2 = fullStep. 3 = halfStep.
static bool stateRunning = true; // Bestemmer om den skal køre eller ej

int main(void)
{
	CyGlobalIntEnable; /* Enable global interrupts. */
  isr_uart_rx_StartEx(ISR_UART_rx_handler);
	UART_1_Start();

    // Liste med kommandoer der bliver skrevet ud på UARTEN
	UART_1_PutString("Step-Motor application started\r\n");
	UART_1_PutString("0: Start\r\n");
	UART_1_PutString("1: Stop\r\n");
	UART_1_PutString("2: Change Direction\r\n");
	UART_1_PutString("q: Decrease speed\r\n");
	UART_1_PutString("w: Increase speed\r\n");
  UART_1_PutString("a: Wave Drive\r\n");
  UART_1_PutString("s: Fullstep\r\n");
  UART_1_PutString("d: HalfStep\r\n");
  UART_1_PutString("f: Full rotation\r\n");

	for(;;)
	{
        if(stateRunning)
        {
            if(state==1)
                waveDrive(clockWise);

            else if(state==2)
                fullStep(clockWise);

            else if(state==3)
                halfStep(clockWise);

            else if(state==4)
            {
                fullrotation(clockWise);
                state = 0;
            }
        }
	}
}

CY_ISR(ISR_UART_rx_handler)
{
	uint8_t bytesToRead = UART_1_GetRxBufferSize();
	while (bytesToRead > 0)
	{
		uint8_t byteReceived = UART_1_ReadRxData();
		UART_1_WriteTxData(byteReceived);

		handleByteReceived(byteReceived);

		bytesToRead--;
	}
}

void handleByteReceived(uint8_t byteReceived)
{
	switch(byteReceived)
	{
        case '0' :
		{
		    stateRunning = true;
            UART_1_PutString("Started\r\n");
		}
        break;
        case '1' :
		{
			stateRunning = false;
            UART_1_PutString("Stopped\r\n");
		}
		break;
		case '2' :
		{
            if (clockWise)
                clockWise = false;
            else if (clockWise == false)
                clockWise = true;

            UART_1_PutString("Direction changed\r\n");
		}
        break;
		case 'q' :
		{
            if (speed < 500)
            {
		        speed += 10;
                UART_1_PutString("Speed decreased\r\n");
            }
		}
		break;
		case 'w' :
		{
            if (speed > 0)
            {
			    speed -= 10;
                UART_1_PutString("Speed increased\r\n");
            }
		}
		break;
		case 'a' :
		{
            state = 1;
            UART_1_PutString("state: Wave Drive\r\n");
		}
		break;
        case 's' :
		{
            state = 2;
            UART_1_PutString("state: fullStep\r\n");
		}
		break;
		case 'd' :
		{
            state = 3;
            UART_1_PutString("state: halfStep\r\n");
		}
		break;
        case 'f' :
		{
            state = 4;
            UART_1_PutString("Full rotation\r\n");
		}
		break;
		default :
		{
			// nothing
		}
		break;
	}
}

void runStep(uint8_t stepstate)
{
	switch(stepstate)
	{
	case 0:
	    {
		    Pin_1a_Write(1);
				Pin_1b_Write(0);
				Pin_2a_Write(0);
				Pin_2b_Write(0);
        CyDelay(speed);
        }
		break;

	case 1:
		{
		Pin_1a_Write(1);
		Pin_1b_Write(0);
		Pin_2a_Write(1);
		Pin_2b_Write(0);
        CyDelay(speed);
        }
		break;

	case 2:
		{
		Pin_1a_Write(0);
		Pin_1b_Write(0);
		Pin_2a_Write(1);
		Pin_2b_Write(0);
        CyDelay(speed);
        }
		break;

	case 3:
		{
		Pin_1a_Write(0);
		Pin_1b_Write(1);
		Pin_2a_Write(1);
		Pin_2b_Write(0);
        CyDelay(speed);
        }
		break;

	case 4:
		{
		Pin_1a_Write(0);
		Pin_1b_Write(1);
		Pin_2a_Write(0);
		Pin_2b_Write(0);
        CyDelay(speed);
	    }
		break;

	case 5:
	    {
		Pin_1a_Write(0);
		Pin_1b_Write(1);
		Pin_2a_Write(0);
		Pin_2b_Write(1);
        CyDelay(speed);
	    }
		break;

	case 6:
	    {
		Pin_1a_Write(0);
		Pin_1b_Write(0);
		Pin_2a_Write(0);
		Pin_2b_Write(1);
        CyDelay(speed);
	    }
		break;

	case 7:
	    {
		Pin_1a_Write(1);
		Pin_1b_Write(0);
		Pin_2a_Write(0);
		Pin_2b_Write(1);
        CyDelay(speed);
	    }
		break;

		default:
		{}
        break;
	}
}

void waveDrive(bool direction)
{
    if (direction)
    {
        for(uint8_t i = 0; i < 8; i+=2) // Unsigned int to use positive values
        {
            runStep(i); // Wavedrive clockwise
        }
    }
    else
    {
        for(int8_t i = 6; i >= 0; i-=2) // Signed int to include negative values
        {
            runStep(i); // Wavedrive counter clockwise
        }
    }

}

void halfStep(bool direction)
{
    if (direction)
    {
        for(uint8_t i = 0; i < 8; i++) // Signed int to include negative values
        {
            runStep(i); //runstep() clockwise
        }
    }
    else
    {
        for(int8_t i = 7; i >= 0; i--) // Signed int to include negative values
        {
            runStep(i); //runstep() counterclockwise
        }
    }
}

void fullStep(bool direction)
{
    if (direction)
    {
        for(uint8_t i = 1; i < 8; i+=2) // To use only positive values
        {
            runStep(i); // fullstep clockwise
        }
    }
    else
    {
        for(int8_t i = 7; i >= 0; i-=2) // Signed int to include negative values
        {
            runStep(i); // fullstep counterclockwise
        }
    }
}

void fullrotation(bool direction)
{
		// 12 waveDrive cycles equals one full rotation.

    for(uint8_t i = 0; i < 12; i++)
		{                             
        waveDrive(direction);
    }
}

/* [] END OF FILE */
