//ENGR 331 - External Input demo (PA0 - USER SWITCH)

#include "stm32f4xx.h"                  // Device header

int count=5;
#define LED_ORANGE 13  //PortDPin13
#define LED_GREEN 0
#define LED_RED 14
#define LED_BLUE 7


// no idea if we have a usable 4th LED, but the 3 that are usable appear to be R,G,&B ones...
#define SWITCH 0 //PortA Pin0


void Init_LED(void){
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER|=((1<<2*LED_ORANGE)|(1<<2*LED_GREEN)|(1<<2*LED_RED)|(1<<2*LED_BLUE));
}


void Init_Switch(void){
	//user switch is on PA0
	// it has a pull down register so dont enable pull-up pull down internally.
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
		
//	To configure the 23 lines as interrupt sources, use the following procedure:
//• Configure the mask bits of the 23 interrupt lines (EXTI_IMR)
//• Configure the Trigger selection bits of the interrupt lines (EXTI_RTSR and EXTI_FTSR)
//• Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
//	external interrupt controller (EXTI) so that an interrupt coming from one of the 23 lines
//	can be correctly acknowledged.
//	
	//Connect the portA pin0 to external interrupt line0
		SYSCFG->EXTICR[0] &= ~(0x000F);			//EXTICR1_EXTI0_PA; //and because we want to clear all other
		EXTI->IMR |= (1<<SWITCH);//Interrupt Mask
		EXTI->RTSR|= (1<<SWITCH);//Rising trigger selection


	
		__enable_irq();
		NVIC_SetPriority(EXTI0_IRQn,0);
		NVIC_ClearPendingIRQ(EXTI0_IRQn);
		NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void){	
	//Clear the EXTI pending bits
	EXTI->PR|=(1<<SWITCH);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	
	GPIOB->BSRR = ((1<<(LED_ORANGE+16))|(1<<(LED_GREEN))|(1<<(LED_RED))|(1<<(LED_BLUE+16)));
}



int main(void)
{
	int j;
	Init_LED();
	Init_Switch();
	while(1)
		{
			GPIOB->BSRR = ((1<<(LED_ORANGE+16))|(1<<(LED_GREEN+16))|(1<<(LED_RED+16))|(1<<(LED_BLUE+16)));
			GPIOB->BSRR = ((1<<(LED_ORANGE))|(1<<(LED_GREEN))|(1<<(LED_RED)) |(1<<(LED_BLUE)));
			for(j=0; j<500000;j++){};
			GPIOB->BSRR = ((1<<(LED_ORANGE+16))|(1<<(LED_GREEN+16))|(1<<(LED_RED+16))|(1<<(LED_BLUE+16)));
			for(j=0; j<500000;j++){};
		}
}