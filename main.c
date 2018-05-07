#include <fsl_device_registers.h>
#include "ADC.h"
#include "fsl_debug_console.h"
#include "board.h"
#include <stdlib.h>


int main(){
	hardware_init();
	initADC();
	unsigned short bADCData0;
	unsigned short bADCData1;
	unsigned int *outputData = (unsigned*) malloc(10000*sizeof(int));
	if (!outputData) debug_printf("%d", -1);
	int i;
	while(1){
		for (i = 0; i < 10000; i++)
		{
			ADC0_SC1A = 0 & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
			while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
			while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Wait until conversion complete

			ADC1_SC1A = 0 & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
			while(ADC1_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
			while(!(ADC1_SC1A & ADC_SC1_COCO_MASK)); //Wait until conversion complete
			//outputData[i] = ADC0_RA;
			outputData[i] = (ADC0_RA << 16)| ADC1_RA;
		}
		for (i = 0; i < 10000; i++)
		{
			debug_printf("%d %d\r\n", outputData[i] & 0x0000ffff, outputData[i] >> 16);
		}
	}
}

//DEFAULT_SYSTEM_CLOCK