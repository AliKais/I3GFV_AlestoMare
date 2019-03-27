#include "TempReader.h"
#include <stdio.h>

uint8 address = 72;

/* 3 variables with 8 bit */
uint8 status;
uint8 b0;
uint8 b1;

/* Variable with 16 bit */
uint16_t temp = 0;

void initComp()
{
    UART_1_Start(); /* Enable UART */
    I2C_1_Start(); /* Enable I2C */
    I2C_1_EnableInt(); /* Enable I2C INT */
    PWM_1_Start();
}

float readTemp()
{
    
    float float_temp;
    
    status = I2C_1_MasterSendStart(address, 1);

        if (status == I2C_1_MSTR_NO_ERROR)
        {
            b0 = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
            b1 = I2C_1_MasterReadByte(I2C_1_NAK_DATA);


            /* Shift b0 left by 8 - cast to 16 bit */
            uint16_t b0_tmp = (uint16_t) b0 << 8;

            /* Mask MSB in b1 */
            b1 = b1 & 0b10000000;

            /* Combine b0 and b1 and shift by 7 - to read 9 MSB*/
            temp = (b0_tmp | b1) >> 7;

            if (temp & 0b100000000)
            {
                /* Temp is negative */

                UART_1_PutString("Negative value \r\n");

                /* Find Two's complement */
                uint16_t temp_inverted = ~temp;
                temp_inverted = temp_inverted & 0b0000000111111111;
                temp_inverted = temp_inverted + 1;

                /* Find real value by dividing by 2 - and find the negative value */
                float_temp = -1 * (float) temp_inverted / 2;
            }
            else
            {
                /* Temp is positive */

                /* Find real value by dividing by 2 */
                float_temp = (float) temp / 2;
            }
        }

        status = I2C_1_MasterSendStop();
        /* Delay - to register new temp */
        CyDelay(1000);
        
        return float_temp;
}

/* [] END OF FILE */
