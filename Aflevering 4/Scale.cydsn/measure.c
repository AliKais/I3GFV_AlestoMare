#include "measure.h"

char uartBuffer[256];
int measureBool = 0;
int measureValue = 0;


void set(int x){
    measureBool = x;
}

void uartADCinit(){
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
       
    UART_1_Start();
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
    
}

void measure(){
    
    uint16_t result = ADC_SAR_1_GetResult16();

    float stigning = 4.641142;    // Stigning
    int offset = -288;          // Standard værdi når vægten er belastet med 1000g
    int weight = stigning * (result + offset);    // Ligning for ret linje y=ax+b
    
    if (measureBool == 1)
    {
        //Kode til udførelse at reset
        measureValue = weight;
        measureBool = 0;
        UART_1_PutString("Setting tare value");
    }    

    //vægt minus measureValue
    weight = weight - measureValue;

    //decimal værdi
    snprintf(uartBuffer, sizeof(uartBuffer), "ADC result: %d \r\n", result);
    UART_1_PutString(uartBuffer);

    //vægten
    snprintf(uartBuffer, sizeof(uartBuffer), "weight Result: %d gram \r\n\r\n", weight);
    UART_1_PutString(uartBuffer);
    
}