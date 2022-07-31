#include "stm32f4xx.h"
#include <stdint.h>

void delayms(int delay);

int main(void){
		
	/*1. Enable clock access to GPIOA*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	/*2. Set GPIOA moder register 5. */
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	
	while(1){
		GPIOA->ODR ^= GPIO_ODR_OD5;
		delayms(100);
	}

	
}
void delayms(int delay){
	int i;
	for(; delay>0; delay--){
		for(i = 0; i < 3195; i++);
	}
}



