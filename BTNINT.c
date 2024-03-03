//ENGR 432 - LCD Library demo
//William Schoon
// Instructions for using this Lib:
/*
Step 0:
hook up LCD. This header is intended for the Hitachi HD44780 16x2 char display.

Pinmap:
    RS: GPIOE6
    RW: GPIOE7
    EN: GPIOE3
    DB4: GPIOF8
    DB5: GPIOF7
    DB6: GPIOF9
    DB7: GPIOG1

Step 1:
Include LCD .c & .h files within project
Step 2;
initialise LCD with LCD_setup command
Step 3:
refer to header for data commands. Most useful commands are send string & send number commands.

An example of how to some Data commands is on ln 246.
*/
#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include "LCD.h"

#define LED_ORANGE 13  //PortDPin13
#define LED_GREEN 0
#define LED_RED 14
#define LED_BLUE 7
#define button 13 // PC 13


int count=5;
int Countup = 1;

int Downcount_Delay = 0;

int Bit0 = 0; // bit 1 2 & 3 for out binary counter 
int Bit1 = 0; // you could say it's a BIT of a hacky solution
int Bit2 = 0; // yea ...

	unsigned char test1 = 13;
	
	int one1 = 0;
	int one2 = 0;
	
	int b0 = 0;
	int b1 = 0;
	int b2 = 0;
	int b3 = 0;


// no idea if we have a usable 4th LED, but the 3 that are usable appear to be R,G,&B ones...

// begin prototyping
void LED_Toggle(int LED);
void LED_On(int LED);
void LED_Off(int LED);
void Init_LED(void);
void Init_Switch(void);
void Init_Timer6(uint32_t  PSC_num, uint32_t  ARR_num);
void LEDSET(int countvalue);

// end prototypes

void Init_LED(void){
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER|=((1<<2*LED_ORANGE)|(1<<2*LED_GREEN)|(1<<2*LED_RED)|(1<<2*LED_BLUE));
}


void Init_Switch(void){
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
// setting up the EXTI/GPIO lines to YELL out interrupt
RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // 1<<14
SYSCFG->EXTICR[3] &=~(0xFu)<<4;
SYSCFG->EXTICR[3] |=(0x2u)<<4;
EXTI->IMR |= 1u<<13; // 
EXTI->RTSR |= 1u<<13; // PC 13
// setting up the NVIC to hear that YELL from EXTI/GPIO line
NVIC_SetPriority(EXTI15_10_IRQn,0);
NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
NVIC_EnableIRQ(EXTI15_10_IRQn);
// ENABLE the main processor interrupt
__enable_irq();
}

void  EXTI15_10_IRQHandler(void){
	//Clear the EXTI pending bits
	EXTI->PR|=(1<<button);
	
//GPIOB->ODR ^= (1u<<LED_GREEN); for testing purposes
Downcount_Delay = 0; // makes the counter take twice as long on downcount // just resets it to make it consistient
Countup ^= 1u; // count down now when toggled
NVIC_ClearPendingIRQ(EXTI15_10_IRQn); // clear pending
	
	
	EXTI->PR |= (1<<0);// dont delete, or it gets stuck here
}



void Init_Timer6(uint32_t  PSC_num, uint32_t  ARR_num)
{
	/* enables clock*/
	RCC -> APB1ENR |= (RCC_APB1ENR_TIM6EN);
	TIM6 ->CNT =0;
	TIM6 ->PSC =PSC_num;
	TIM6 ->ARR =ARR_num;
	TIM6 ->DIER |= 1<<0; //enable UIE for interrupt
	// starts the counter
	TIM6 ->CR1 |= 1<<0;//bit 0 of CR1 is count enable
	NVIC_SetPriority(TIM6_DAC_IRQn,1); //sets priority, if all are set to 0, than all share the same priority
	NVIC_ClearPendingIRQ(TIM6_DAC_IRQn); //clears pending
	NVIC_EnableIRQ(TIM6_DAC_IRQn); //enables
	
	__enable_irq();
	
}

void TIM6_DAC_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
	if(TIM6->SR & TIM_SR_UIF){ // if UIF flag is set -- Just in case
		TIM6->SR &= ~TIM_SR_UIF; // clear UIF flag
	}
	
	
	if((count == 7) && (Countup == 1)){
	count = 0;
	}
	else if((count == 0)&&(Countup == 0)&&(Downcount_Delay == 1)){
	count = 7;
	Downcount_Delay = 0;
	}
	else if((Countup == 0)&&(Downcount_Delay == 1)){
	count--;
	Downcount_Delay = 0;
	}
	else if((Countup == 1)){
	count++;
	}
	else{
		Downcount_Delay = 1;
	}
	
	Bit0 = count;
	Bit1 = count;
	Bit2 = count;
	
	// ez little bit mapping int >> binary, another note for future me, possible use another way ? research more efficient way
	
	Bit0 &= 1u; // NO actually and like: 001 &=> 111 = 001 etc, bit capture//////// XOR for st8 machine
	Bit1 &= 2u; 
	Bit2 &= 4u; 
	
	
	
	if(Bit0){
	LED_On(LED_GREEN);
	}
	else
	{
	LED_Off(LED_GREEN);
	}
	
	if(Bit1 == 2u){
	LED_On(LED_BLUE);
	}
	else
	{
	LED_Off(LED_BLUE);
	}
	
	if(Bit2 == 4u){
	LED_On(LED_RED);
	}
	else
	{
	LED_Off(LED_RED);
	}
	// R B G
	// M   L
	// 0 0 0
	// 0 0 1
	// 0 1 0
	// e t c
	
	/*
	LED_Toggle(LED_BLUE);
	LED_Toggle(LED_GREEN);
	LED_Toggle(LED_RED);
*/
}

// LED basic control abstraction
void LED_Toggle(int LED)
{
GPIOB->ODR ^= (1u<<LED);

}

void LED_On(int LED)
{
GPIOB->ODR |= (1u<<LED);
}

void LED_Off(int LED)
{
GPIOB->ODR &= ~(1u<<LED);
}






int main(void)
{
	// safe delete
one1 = test1;
one2 = (test1 & 8u)>>3;
	// safe delete
	
	// sample bit deconstructor
	b0 = (test1 & 1u)>>0;
	b1 = (test1 & 2u)>>1;
	b2 = (test1 & 4u)>>2;
	b3 = (test1 & 8u)>>3;
	
	
	
	int i;
	Init_LED();
	Init_Switch();
	Init_Timer6(21000, 999); // TIM 6 has 16-bit resolution
	
	 // example of how to use lib
	LCD_setup();
	LCD_clear();
	LCD_send_String("Initialised");
	for (i = 0; i <= 2000000; i++) {} // smol delayt
	LCD_clear();
	LCD_place_cursor(0xC0); // Row2,Col1
	for (i = 0; i <= 2000000; i++) {}
	LCD_place_cursor(0x85); // Row1,Col5
	for (i = 0; i <= 2000000; i++) {}
	LCD_send_intString(2+4);
	LCD_place_cursor(0xC2);
	LCD_send_floatString(0.444+2);
	LCD_place_cursor(0x80);
	
	 //
	
	while(1)
	{
	}
}





