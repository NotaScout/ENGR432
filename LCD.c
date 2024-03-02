#include "stm32f4xx.h" 
#include <string.h>
#include <stdio.h>
#include "LCD.h"

/*
TODO BLOCK
Possibly set busy flag chek to interrupt?
-really only need to look into this if LCD is taking up too much CPU time in while()

*/

/// PIN manipulation

void LCD_PIN_clear(int PINNO){
if((PINNO == RS) || (PINNO == RW) || (PINNO == EN)){
GPIOE -> ODR &= ~(1u<<PINNO);
}
else if((PINNO == DB4) || (PINNO == DB5) || (PINNO == DB6)){
GPIOF -> ODR &= ~(1u<<PINNO);
}
else{
GPIOG -> ODR &= ~(1u<<PINNO);
}
}
void LCD_PIN_set(int PINNO){
if((PINNO == RS) || (PINNO == RW) || (PINNO == EN)){
GPIOE -> ODR |= (1u<<PINNO);
}
else if((PINNO == DB4) || (PINNO == DB5) || (PINNO == DB6)){
GPIOF -> ODR |= (1u<<PINNO);
}
else{
GPIOG -> ODR |= (1u<<PINNO);
}
}
void LCD_PIN_MODE(int PINNO, unsigned int MODE){
MODE &= 3u; // cant have any pesky glitches exceeding MODER bounds
if((PINNO == RS) || (PINNO == RW) || (PINNO == EN)){
GPIOE -> MODER &= ~(3u<<2*PINNO); // clear first, then set
GPIOE -> MODER |= (MODE<<2*PINNO);
}
else if((PINNO == DB4) || (PINNO == DB5) || (PINNO == DB6)){
GPIOF -> MODER &= ~(3u<<2*PINNO);
GPIOF -> MODER |= (MODE<<2*PINNO);
}
else{
GPIOG -> MODER &= ~(3u<<2*PINNO);
GPIOG -> MODER |= (MODE<<2*PINNO);
}
}
void LCD_BF_check(void){
int i;
LCD_PIN_clear(RS);
LCD_PIN_set(RW);
LCD_PIN_MODE(DB7, 0);
LCD_PIN_set(EN);
for(i=0;i<=2000;i++){} // smol delae
LCD_PIN_clear(EN);
LCD_PIN_clear(RW);
LCD_PIN_MODE(DB7, 1);

}



// Setup Routines // Remember to call these if you want to use this lib
void LCD_port_init(){
	// GPIOG & GPIOF
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	//RCC->AHB1ENR|= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	
	
// RW EN		GPIOG -> MODER &= ~(3u<<2*DB4); //2* for two bits
LCD_PIN_MODE(RS, 1);
LCD_PIN_MODE(RW, 1);
LCD_PIN_MODE(EN, 1);
// ZEROS, Just in case
	
// DATA EN
LCD_PIN_MODE(DB4, 1);
LCD_PIN_MODE(DB5, 1);
LCD_PIN_MODE(DB6, 1);
LCD_PIN_MODE(DB7, 1);
	
// OTYPER	
	// might not // need?? // sets to push pull (theoretically)
	GPIOE -> OTYPER &= ~(127u);
	GPIOF -> OTYPER &= ~(127u);
	GPIOG -> OTYPER &= ~(127u);

}

void LCD_init(){
// STEP 1: Wait for 100ms for power-on-reset to take effect
	int i;
	for(i=0;i<=200;i++){} // deley
		
// STEP 2: Set RS pin LOW to send instructions
	LCD_PIN_clear(RS); // fix // odr 7 =  // ands the 7th odr pin with 0
		
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

// Basic Function Abstraction
void LCD_sendInstr(unsigned char Instruction){
/*
	Bit3 = (Instruction & 8u)>>3;
	Bit2 = (Instruction & 4u)>>2;
	Bit1 = (Instruction & 2u)>>1;
	Bit0 = (Instruction & 1u)>>0;
*/
LCD_PIN_clear(RW);
LCD_PIN_clear(RS);
LCD_PIN_set(EN);
LCD_PIN_clear(DB4);
LCD_PIN_clear(DB5);
LCD_PIN_clear(DB6);
LCD_PIN_clear(DB7);
if((Instruction & 1u)>>0){
LCD_PIN_set(DB4);
}
if((Instruction & 2u)>>1){
LCD_PIN_set(DB5);
}
if((Instruction & 4u)>>2){
LCD_PIN_set(DB6);
}
if((Instruction & 8u)>>3){
LCD_PIN_set(DB7);
}
LCD_PIN_clear(EN);
LCD_BF_check();

}
void LCD_sendData(unsigned char data){
LCD_PIN_set(RS);
LCD_PIN_clear(RW); // wriet mode
LCD_PIN_set(EN);
LCD_PIN_clear(DB4);
LCD_PIN_clear(DB5);
LCD_PIN_clear(DB6);
LCD_PIN_clear(DB7);
if((data & 0x10u)>>4){ // top 4 MSB
LCD_PIN_set(DB4);
}
if((data & 0x20u)>>5){
LCD_PIN_set(DB5);
}
if((data & 0x40u)>>6){
LCD_PIN_set(DB6);
}
if((data & 0x80u)>>7){
LCD_PIN_set(DB7);
}
LCD_PIN_clear(EN);
LCD_PIN_set(EN);	
LCD_PIN_clear(DB4);
LCD_PIN_clear(DB5);
LCD_PIN_clear(DB6);
LCD_PIN_clear(DB7);
if((data & 1u)>>0){
LCD_PIN_set(DB4);
}
if((data & 2u)>>1){
LCD_PIN_set(DB5);
}
if((data & 4u)>>2){
LCD_PIN_set(DB6);
}
if((data & 8u)>>3){
LCD_PIN_set(DB7);
}
LCD_PIN_clear(EN);
LCD_BF_check();
}
void LCD_clear(void){
LCD_sendInstr(0);
LCD_sendInstr(1);

}

// String Abstraction

void  LCD_send_String(char string[]){ // use this fcn to output charstrings to lcd
int i = 0;
	for(i=0;string[i]!='\0';i++){
		LCD_sendData(string[i]); // make into function // no u 
	}
}
void  LCD_send_intString(int integer){ // possibly breaks the code // double check ln; sprintf(ansstr,"%d",ANS);
int i = 0;
	char ansstr[8] = ""; // empty char string 8 chars long
	sprintf(ansstr,"%d",integer); // function to send numbers to print
	for(i=0;ansstr[i]!='\0';i++){
		LCD_sendData(ansstr[i]);
	}

}
void  LCD_send_floatString(float floating_point){ // possibly breaks the code // double check ln; sprintf(ansstr,"%d",ANS);
int i = 0;
	char ansstr[17] = ""; // empty char string 8 chars long
	sprintf(ansstr,"%f",(double)floating_point); // function to send numbers to print
	for(i=0;ansstr[i]!='\0';i++){
		LCD_sendData(ansstr[i]);
	}
	
}

// Commands

void LCD_place_cursor(unsigned char lineno){

}

