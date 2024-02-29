//ENGR 331 - External Input demo (PA0 - USER button)
//William Schoon

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
	int j;
	Init_LED();
	Init_Switch();
	Init_Timer6(21000, 999); // TIM 6 has 16-bit resolution
	LCD_port_init();
	
	/*
	while(1)
		{
			GPIOB->BSRR = ((1<<(LED_ORANGE+16))|(1<<(LED_GREEN+16))|(1<<(LED_RED+16))|(1<<(LED_BLUE+16)));
			GPIOB->BSRR = ((1<<(LED_ORANGE))|(1<<(LED_GREEN))|(1<<(LED_RED)) |(1<<(LED_BLUE)));
			for(j=0; j<500000;j++){
			int m = 0;
			}
			GPIOB->BSRR = ((1<<(LED_ORANGE+16))|(1<<(LED_GREEN+16))|(1<<(LED_RED+16))|(1<<(LED_BLUE+16)));
			for(j=0; j<500000;j++){}
		}
	*/
	
	while(1)
	{/* // im simply dumber
		switch(Bit0){
			case 0:
			{
				LED_Toggle(LED_BLUE);
				Bit1 ^= 1u;
			}
			break;
			case 1:
			{
				Bit1 = Bit1;
			}
			break;
			default:
				Bit1 = 0;
		}
	// lnbreak1
		switch(Bit1){
			case 0:
			{
				Bit2 ^= 1u;
			}
			break;
			case 1:
			{
				Bit2 = Bit2;
			}
			break;
			default:
				Bit2 = 0;
		}
		//lnbreak2
		switch(Bit2){
			case 0:
			{
				
			}
			break;
			case 1:
			{
				
			}
			break;
			default:
				Bit1 = 0;
		}
		
		
		*/
	}
}





