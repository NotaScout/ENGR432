
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

void LCD_init(){
// STEP 1: Wait for 100ms for power-on-reset to take effect
	int i;
	for(i=0;i<=200;i++){}
		
// STEP 2: Set RS pin LOW to send instructions
	GPIOD -> ODR &= ~(1u<<RS); // fix // odr 7 =  // ands the 7th odr pin with 0
		
// STEP 3a-3d: Set 4-bit mode (takes a total of 4 steps)
		LCD_sendInstr(3); // 0010 0000 // sets 4-bit mode // 
		//BF_check();
		LCD_sendInstr(3); // home return
		//BF_check();
		LCD_sendInstr(3); // entry mode set (increment by 1)
		//BF_check();
		LCD_sendInstr(2); // turn on display
		//BF_check();


// STEP 4: Set 2 line display -- treats 16 char as 2 lines
//			001DL NF** (DL 0: 4bits; N= 1: 2 lines; F=0 : 5x8 display
	LCD_sendInstr(2); // we already did this is step 3a // 0010 0100
	//BF_check();
// STEP 5: Set DISPLAY to OFF
	LCD_sendInstr(8);
	LCD_sendInstr(0);
	LCD_sendInstr(8);
	LCD_sendInstr(0);
	LCD_sendInstr(1);
	LCD_sendInstr(0);
	LCD_sendInstr(6);
	LCD_sendInstr(0);
	LCD_sendInstr(15);
	LCD_sendInstr(0);
	LCD_sendInstr(2);

		
		
		
}







