//ENGR 331 - External Input demo (PA0 - USER SWITCH)

#include "stm32f4xx.h"                  // Device header

int count=5;
int Countup = 1;
#define LED_ORANGE 13  //PortDPin13
#define LED_GREEN 0
#define LED_RED 14
#define LED_BLUE 7
#define SWITCH 13 // PC 13





// no idea if we have a usable 4th LED, but the 3 that are usable appear to be R,G,&B ones...


void LED_Toggle(int LED);
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
	EXTI->PR|=(1<<SWITCH);
	
GPIOB->ODR ^= (1u<<LED_GREEN);

NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	
	
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
	else if((count == 0)&&(Countup == 0)){
	count = 7;
	}
	else if((Countup == 0)){
	count--;
	}
	else{
	count++;
	}
	LEDSET(count);
	
	/*
	LED_Toggle(LED_BLUE);
	LED_Toggle(LED_GREEN);
	LED_Toggle(LED_RED);
*/
}

void LED_Toggle(int LED)
{
GPIOB->ODR ^= (1u<<LED);

}


void LEDSET(int countvalue)//
{


}





int main(void)
{
	int j;
	Init_LED();
	Init_Switch();
	Init_Timer6(9999, 999); // TIM 6 has 16-bit resolution
	
	
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
	while(1);
}





