#include <fsl_device_registers.h>
#include "ADC.h"
#include "fsl_debug_console.h"
#include "board.h"
#include <stdlib.h>

#define ARM_MATH_CM4
#include <arm_math.h>
#include <math.h>

unsigned int VECLEN = 3000; 
unsigned int CORRLEN = 5999;

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
	
	float32_t max; //correlation max
	uint32_t maxIdx; //index of max
	
  //q31_t dummy1[3] = {2, 2, 2};
	//q31_t dummy2[3] = {1, 1, 1};
	//q31_t corr[5] = {0};
	
	int i;
	float32_t timeDelay;
	float32_t Ts = 1.7644E-6f;
	float32_t angle;
	float32_t dist = 0.1f;
	
	while(1){
		//debug_printf("Collecting data...\r\n");
		collectData(adc0Data, adc1Data);
		//debug_printf("Done.\r\nPerforming correlation...\r\n");
		arm_correlate_f32(adc0Data, VECLEN, adc1Data, VECLEN, corrData);
		//debug_printf("Done.\r\nCalculating time delay...\r\n");
		arm_max_f32(corrData, CORRLEN, &max, &maxIdx);
		
	/*int i;
	for (i = 0; i < VECLEN; i++)
	{
		debug_printf("%d %d\r\n", ((unsigned)adc0Data[i]), ((unsigned) adc1Data[i]));
		//debug_printf("%d\r\n", (unsigned)[i]);
	}*/

		debug_printf("%d %d\r\n", (unsigned)max, (unsigned)maxIdx);
		timeDelay = Ts*(maxIdx - VECLEN);
		angle = asin(343*timeDelay/dist)*180/3.14159265;
		debug_printf("angle: %d \r\n", (unsigned)angle);
	}
}

void initLED() {
	PORTC->PCR[5] = PORT_PCR_MUX(001);
	PTC->PDDR = GPIO_PDDR_PDD(1<<5);
	PORTC->PCR[7] = PORT_PCR_MUX(001);
	PTC->PDDR = GPIO_PDDR_PDD(1<<7);
	PORTC->PCR[0] = PORT_PCR_MUX(001);
	PTC->PDDR = GPIO_PDDR_PDD(1<<0);
	PORTC->PCR[9] = PORT_PCR_MUX(001);
	PTC->PDDR = GPIO_PDDR_PDD(1<<9);
	PORTC->PCR[8] = PORT_PCR_MUX(001);
	PTC->PDDR = GPIO_PDDR_PDD(1<<8);
}

void lightLED(float angle) {
	if(angle < 36)
		
	
	
}

void collectData(float32_t* vec0, float32_t * vec1)
{
	int i;
	float32_t vec0Mean;
	float32_t vec1Mean;
	for (i = 0; i < VECLEN; i++)
	{
		ADC0_SC1A = 0 & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
		while(ADC0_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
		while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); //Wait until conversion complete
		vec0[i] = ((float32_t)ADC0_RA); //save to buffer

		ADC1_SC1A = 0 & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
		while(ADC1_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
		while(!(ADC1_SC1A & ADC_SC1_COCO_MASK)); //Wait until conversion complete
		vec1[i] = ((float32_t)ADC1_RA); //save to buffer
	}
	arm_mean_f32(vec0, VECLEN, &vec0Mean);
	arm_mean_f32(vec1, VECLEN, &vec1Mean);
	//arm_offset_f32(vec0, -vec0Mean, vec0, VECLEN);
	//arm_offset_f32(vec1, -vec1Mean, vec1, VECLEN);
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


