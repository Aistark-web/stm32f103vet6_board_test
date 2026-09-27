#include "key_led_test.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == KEY1_Pin) {
		HAL_GPIO_TogglePin(RED_LED_GPIO_Port,RED_LED_Pin);
	}
	else if(GPIO_Pin == KEY2_Pin) {
		HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port,GREEN_LED_Pin);
	}
	else if(GPIO_Pin == KEY3_Pin) {
		HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port,BLUE_LED_Pin);
	}
}