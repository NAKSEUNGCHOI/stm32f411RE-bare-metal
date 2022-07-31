#include "stm32f4xx.h"

/**
 * main.c
 * @brief initializes LCD
 *	PC0 - PC7 = D0 - D7
 *  PB5 = RS
 *  PB6 = R/W
 *  PB7 = EN
 */

#define RS	0x20
#define RW 	0x40
#define EN 	0x80

/* Function Prototypes */
void LCD_Init(void);
void GPIO_Init(void);
void LCD_command(unsigned char command);
void LCD_data(char data);
void delay_ms(int delay);

int main(void){
	
	LCD_Init();
	
	while(1){
		LCD_data('H');
		LCD_data('e');
		LCD_data('l');
		LCD_data('l');
		LCD_data('o');
	
		LCD_command(1);
		delay_ms(500);
	}
	
	
}
void LCD_Init(void){
	GPIO_Init();
	
	/*2. LCD init sequence. */
	delay_ms(30);
	LCD_command(0x30);
	delay_ms(10);
	LCD_command(0x30);
	delay_ms(1);
	LCD_command(0x30);

	/* set 8-bit data mode, 2-line 5x7 font*/
	LCD_command(0x38);
	/* move cursor right */
	LCD_command(0x06);
	/* Clear screen, move cursor home*/
	LCD_command(0x01);
	/* Turn display, blink cursor */
	LCD_command(0x0F);
	
}
void GPIO_Init(void){
	
	/*1. Set clock access to GPIOB and GPIOC*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
	
	/*2. Set GPIOC mode to output mode. (PC0 - PC7 = D0 - D7)*/
	GPIOC->MODER |= GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0;
	GPIOC->MODER |= GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0;
	
	/*3. Set GPIOB mode to output mode (PB5 = RS, PB6 = R/W, PB7 = EN) */
	GPIOB->MODER |= GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0;
	
	GPIOB->BSRR = 0x00C;
	
}
void LCD_command(unsigned char command){
	GPIOB->BSRR = GPIO_BSRR_BR5 | GPIO_BSRR_BR6;
	GPIOC->ODR = command;
	GPIOB->BSRR = GPIO_BSRR_BS7;
	delay_ms(50);
	GPIOB->BSRR = GPIO_BSRR_BR7;

	
}
void LCD_data(char data){
	GPIOB->BSRR = GPIO_BSRR_BS5;
	GPIOB->BSRR = GPIO_BSRR_BR6;
	GPIOC->ODR = data;
	GPIOB->BSRR = GPIO_BSRR_BS7;
	delay_ms(50);
	GPIOB->BSRR = GPIO_BSRR_BR7;
}
void delay_ms(int delay){
	int i;
	for(; delay>0; delay--){
		for(i = 0; i < 3195; i++){}
	}
	
}





