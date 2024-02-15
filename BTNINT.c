//ENGR 331 - External Input demo (PA0 - USER SWITCH)

#include "stm32f4xx.h"                  // Device header

int count=5;
int Count_Up = 1;
#define LED_ORANGE 13  //PortDPin13
#define LED_GREEN 0
#define LED_RED 14
#define LED_BLUE 7


// no idea if we have a usable 4th LED, but the 3 that are usable appear to be R,G,&B ones...
#define SWITCH 0 //PortA Pin0

// begin prototyping
void LED_Toggle(int LED);
void Init_LED(void);
void Init_Switch(void);
void Init_Timer6(uint32_t  PSC_num, uint32_t  ARR_num);


// end prototyping






void Init_LED(void){
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER|=((1<<2*LED_ORANGE)|(1<<2*LED_GREEN)|(1<<2*LED_RED)|(1<<2*LED_BLUE)); // moder setup
	// we can control LEDs
}


void Init_Switch(void){
	//user switch is on PA0
	// it has a pull down register so dont enable pull-up pull down internally.
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
	
		// setting up the EXTI/GPIO lines to YELL out interrupt
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // 1<<14 //
	SYSCFG->EXTICR[0] &=~(0xFu);
	
		
//	To configure the 23 lines as interrupt sources, use the following procedure:
//• Configure the mask bits of the 23 interrupt lines (EXTI_IMR)
//• Configure the Trigger selection bits of the interrupt lines (EXTI_RTSR and EXTI_FTSR)
//• Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
//	external interrupt controller (EXTI) so that an interrupt coming from one of the 23 lines
//	can be correctly acknowledged.
//	
	//Connect the portA pin0 to external interrupt line0
		SYSCFG->EXTICR[0] &= ~(0x000Fu);			//EXTICR1_EXTI0_PA; //and because we want to clear all other
		EXTI->IMR |= (1<<SWITCH);//Interrupt Mask
		EXTI->RTSR|= (1<<SWITCH);//Rising trigger selection
	
	
	
	// setup interrupt EXTI0
		NVIC_SetPriority(EXTI0_IRQn,6);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);
	// ENABLE the main processor interrupt
	__enable_irq(); 


	
		__enable_irq();
		NVIC_SetPriority(EXTI0_IRQn,0);
		NVIC_ClearPendingIRQ(EXTI0_IRQn);
		NVIC_EnableIRQ(EXTI0_IRQn);
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
	NVIC_SetPriority(TIM6_DAC_IRQn,0); //sets priority, if all are set to 0, than all share the same priority
	NVIC_ClearPendingIRQ(TIM6_DAC_IRQn); //clears pending
	NVIC_EnableIRQ(TIM6_DAC_IRQn); //enables
	
	__enable_irq();
	
}


void EXTI0_IRQHandler(void){	// EXT Interrupt handler
	// make this short and sweet
	EXTI->PR |= (1u<<SWITCH);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	GPIOB->ODR ^= 1u<<LED_RED;
}


void TIM6_DAC_IRQHandler(void) // TIM6
{
	NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
	if(TIM6->SR & TIM_SR_UIF){ // if UIF flag is set -- Just in case
		TIM6->SR &= ~TIM_SR_UIF; // clear UIF flag
	}
	if(Count_Up)
	{
	
	}
	LED_Toggle(LED_BLUE);
	LED_Toggle(LED_GREEN);
	LED_Toggle(LED_RED);

}

void LED_Toggle(int LED)
{
GPIOB->ODR ^= (1u<<LED);

}


/* || SAFE DELETE

void EXTI0_IRQHandler(void){	
	//Clear the EXTI pending bits
	EXTI->PR|=(1<<SWITCH);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	
	GPIOB->BSRR = ((1<<(LED_ORANGE+16))|(1<<(LED_GREEN))|(1<<(LED_RED))|(1<<(LED_BLUE+16)));
}

*/ //|| SAFE DELETE (I THINK)

int main(void)
{
	//int j; remove
	Init_LED();
	Init_Switch();
	Init_Timer6(9999, 999); // TIM 6 has 16-bit resolution // 
	// make this 1 sec (find frequency) (datasheet?)
	
	
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





