
////////

#include "stm32f4xx.h" 
#include <string.h>
#include <stdio.h>
#include "LCD.h"

/*
TODO BLOCK
*/

void LCD_clear(void){
//LCD_sendInstr(0);
//LCD_sendInstr(1);

}

void LCD_port_init(){
	// GPIOG & GPIOF
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	//RCC->AHB1ENR|= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	
	
// DATA EN		GPIOG -> MODER &= ~(3u<<2*DB4); //2* for two bits
	GPIOG -> MODER |= ~(3u<<2*DB4);
	GPIOG -> MODER |= (1u<<2*DB4);
	GPIOG -> MODER &= ~(3u<<2*DB5);
	GPIOG -> MODER |= (1u<<2*DB5);
	GPIOG -> MODER &= ~(3u<<2*DB6);
	GPIOG -> MODER |= (1u<<2*DB6);
	GPIOG -> MODER &= ~(3u<<2*DB7);
	GPIOG -> MODER |= (1u<<2*DB7);// ZEROS, Just in case
	
// RW EN
	GPIOF -> MODER &= ~(3u<<2*EN);
	GPIOF -> MODER |= (1u<<2*EN);
	GPIOF -> MODER &= ~(3u<<2*RW);
	GPIOF -> MODER |= (1u<<2*RW);
	GPIOF -> MODER &= ~(3u<<2*RS);
	GPIOF -> MODER |= (1u<<2*RS);
// OTYPER
	GPIOF -> MODER &= ~(3u<<2*EN);
	GPIOF -> MODER &= ~(3u<<2*RW);
	GPIOF -> MODER &= ~(3u<<2*RS);
	
	GPIOG -> MODER &= ~(3u<<2*4);
	GPIOG -> MODER &= ~(3u<<2*5);
	GPIOG -> MODER &= ~(3u<<2*6);
	GPIOG -> MODER &= ~(3u<<2*7);
	
	
	// might not // need??
	GPIOF -> OTYPER &= ~(127u);
	GPIOG -> OTYPER &= ~(127u);

}

