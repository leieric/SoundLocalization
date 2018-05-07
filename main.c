#include <fsl_device_registers.h>
#include "ADC.h"
#include "fsl_debug_console.h"
#include "board.h"


int main(){
	hardware_init();
	initADC();
	unsigned short bADCData;
	while(1){
		bADCData = ADC_read16b();
		debug_printf("%d \r\n", bADCData);
		DelayFunction();
	}
}