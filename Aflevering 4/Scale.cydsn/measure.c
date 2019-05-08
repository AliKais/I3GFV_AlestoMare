#include "measure.h"

char uartBuffer[256];
int measureValue = 0;


void uartADCinit(){
           
    UART_1_Start();
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
    
}

void measure(){
    
    uint16_t result = ADC_SAR_1_GetResult16();

    float stigning = 0.715;                         // Stigning
    int offset = -1612;                             // Offset
    float weight = stigning * (result + offset);    // Ligning for ret linje
    

    //Decimal værdi
    snprintf(uartBuffer, sizeof(uartBuffer), "ADC result: %d \r\n", result);
    UART_1_PutString(uartBuffer);

    //Vægten
    snprintf(uartBuffer, sizeof(uartBuffer), "Omregnet vaegt: %f gram \r\n\r\n", weight);
    UART_1_PutString(uartBuffer);
    
}