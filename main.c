#include <fsl_device_registers.h>
#include "ADC.h"
#include "fsl_debug_console.h"
#include "board.h"
#include <stdlib.h>

#define ARM_MATH_CM4
#include <arm_math.h>

unsigned int VECLEN = 1000; 
unsigned int CORRLEN = 1999;

void collectData(float32_t * vec0, float32_t * vec1); //retrieves data from both adc inputs
void printUSB(float32_t* vec); //prints to USB cable

int main(){
	hardware_init();
	initADC();
	//init data arrays
	float32_t *adc0Data = (float32_t*) malloc(VECLEN*sizeof(float32_t));
	if (!adc0Data) {
		debug_printf("%d", -1);
		return -1;
	}
	float32_t *adc1Data = (float32_t*) malloc(VECLEN*sizeof(float32_t));
	if (!adc1Data) {
		debug_printf("%d", -1);
		return -1;
	}
	float32_t *corrData = (float32_t*) malloc(CORRLEN*sizeof(float32_t));
	if (!corrData) {
		debug_printf("%d", -1);
		return -1;
	}
	
	float32_t dummy1[3] = {2.0, 2.0, 2.0};
	float32_t dummy2[3] = {1.0, 1.0, 1.0};
	float32_t corr[5] = {0};
	float32_t max;
	uint32_t maxIdx;
  //q31_t dummy1[3] = {2, 2, 2};
	//q31_t dummy2[3] = {1, 1, 1};
	//q31_t corr[5] = {0};
	
	int i;
	int timeDelay;
	//float Ts = ;
	while(1){
		debug_printf("Collecting data...\r\n");
		collectData(adc0Data, adc1Data);
		debug_printf("Done.\r\nPerforming correlation...\r\n");
		arm_correlate_f32(adc0Data, VECLEN, adc1Data, VECLEN, corrData);
		debug_printf("Done.\r\nCalculating time delay...\r\n");
		arm_max_f32(corrData, CORRLEN, &max, &maxIdx);
		//arm_add_q31(adc0Data, adc1Data, corrData, VECLEN); 
		//arm_mult_q31(dummy1, dummy2, corr, 3);
		//arm_mult_f32((float32_t*)dummy1, (float32_t*)dummy2, (float32_t*)corr, 3);
		//arm_correlate_f32(dummy1, 3, dummy2, 3, corr);
		
		//printUSB(corrData);
		debug_printf("%d %d\r\n", (unsigned)max, (unsigned)maxIdx);
		//timeDelay = Ts*(maxIdx - VECLEN);
		
		
	}
}

void collectData(float32_t* vec0, float32_t * vec1)
{
	int i;
	for (i = 0; i < VECLEN; i++)
		{
			ADC0_SC1A = 0 & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
			while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
			while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Wait until conversion complete
			vec0[i] = (float32_t)(ADC0_RA/VECLEN); //save to buffer

			ADC1_SC1A = 0 & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
			while(ADC1_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
			while(!(ADC1_SC1A & ADC_SC1_COCO_MASK)); //Wait until conversion complete
			vec1[i] = (float32_t)(ADC1_RA/VECLEN); //save to buffer
		}
}

void printUSB(float32_t * vec)
{
	int i;
	for (i = 0; i < CORRLEN; i++)
	{
		//debug_printf("%d %d\r\n", corrData[i] & 0x0000ffff, coo[i] >> 16);
		debug_printf("%d\r\n", (unsigned)vec[i]);
	}
}


