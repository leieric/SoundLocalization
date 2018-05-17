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
void initLED(); //initialize LED pins to GPIO
void lightLED(float32_t angle); //lights appropriate LEDs

int main(){
	hardware_init();
	initADC();
	initLED();
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
	
	float32_t max; //correlation max
	uint32_t maxIdx; //index of max
	float32_t timeDelay; //time delay between sound signals
	float32_t Ts = 1.7644E-6f; //calculated ADC sampling period
	float32_t angle; //angle of incoming sound w.r. to perpendicualar
	float32_t dist = 0.1f; //distance between microphones
	
	while(1){
		collectData(adc0Data, adc1Data); //Collect data from ADC
		arm_correlate_f32(adc0Data, VECLEN, adc1Data, VECLEN, corrData); //Compute correlation
		arm_max_f32(corrData, CORRLEN, &max, &maxIdx); //Find max index

		debug_printf("%d %d\r\n", (unsigned)max, (unsigned)maxIdx);
		timeDelay = Ts*((int)maxIdx - (int)VECLEN);
		angle = asinf(343*timeDelay/dist)*180/3.14159265;
		debug_printf("angle: %d \r\n", (int)angle);
		lightLED(angle);
	}
}

void initLED() {
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; //Enable Port C
	PORTC->PCR[5] = PORT_PCR_MUX(001);  //Set pin as GPIO
	PTC->PDDR |= (1 << 5); 							//Set as output	
	PORTC->PCR[7] = PORT_PCR_MUX(001);
	PTC->PDDR |= (1<<7);
	PORTC->PCR[0] = PORT_PCR_MUX(001);
	PTC->PDDR |= (1<<0);
	PORTC->PCR[9] = PORT_PCR_MUX(001);
	PTC->PDDR |= (1<<9);
	PORTC->PCR[8] = PORT_PCR_MUX(001);
	PTC->PDDR |= (1<<8);
	PTC->PTOR |= 0x00003A1; //Flash all LEDs on
	DelayFunction();
	PTC->PTOR |= 0x00003A1;
}

void lightLED(float32_t angle) {
	PTC->PCOR |= 0xFFFFFFFF; //turn off all LEDs
	if(angle < -54)
		PTC->PSOR = (1 << 8);
	else if(angle < -18)
		PTC->PSOR = (1 << 9);
	else if(angle < 18)
		PTC->PSOR = (1 << 0);
	else if(angle < 54)
		PTC->PSOR = (1 << 7);
	else if (angle < 90)
		PTC->PSOR = (1 << 5);	
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
		vec0[i] = (((float32_t)ADC0_RA)/1000); //save to buffer

		ADC1_SC1A = 0 & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
		while(ADC1_SC2 & ADC_SC2_ADACT_MASK); //Conversion in progress
		while(!(ADC1_SC1A & ADC_SC1_COCO_MASK)); //Wait until conversion complete
		vec1[i] = (((float32_t)ADC1_RA)/1000); //save to buffer
	}
	arm_mean_f32(vec0, VECLEN, &vec0Mean); //Remove mean from both signals
	arm_mean_f32(vec1, VECLEN, &vec1Mean);
	arm_offset_f32(vec0, -vec0Mean, vec0, VECLEN);
	arm_offset_f32(vec1, -vec1Mean, vec1, VECLEN);
}

void printUSB(float32_t * vec)
{
	int i;
	for (i = 0; i < CORRLEN; i++)
		debug_printf("%d\r\n", (unsigned)vec[i]);
}