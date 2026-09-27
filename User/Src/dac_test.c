#include "dac_test.h"

void Dac_Init()
{
	HAL_DAC_Start(&hdac,DAC1_CHANNEL_1);
	HAL_DAC_Start(&hdac,DAC1_CHANNEL_2);
	
	HAL_DAC_SetValue(&hdac,DAC1_CHANNEL_1,DAC_ALIGN_12B_R,2482);		//电压约为2.00015V
	HAL_DAC_SetValue(&hdac,DAC1_CHANNEL_2,DAC_ALIGN_12B_R,1241);		// 电压约为1.00007V
}