/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017-2021 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>

#include "main.h"
#include "Board_LED.h"                  /* ::Board Support:LED */
#include "Board_Buttons.h"              /* ::Board Support:Buttons */
#include "LCD.h"

#include "RTE_Components.h"             /* Component selection */

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (512U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

static volatile uint32_t delay_val = 500U;

static osThreadId_t task_1_ID;       /* Thread id of thread: task_1 */
static osThreadId_t task_2_ID;       /* Thread id of thread: task_2 */
static osThreadId_t task_3_ID;       /* Thread id of thread: task_3 */
static osThreadId_t task_4_ID;       /* Thread id of thread: task_4 */
static osThreadId_t tid_thrBUT;			 /* Thread id of thread: thrBUT */

int counter = 0;          // used in task_1
int global_counter = 499;	// used in task_3 and task_4
// sleep flag
volatile int flag = 0;
/*----------------------------------------------------------------------------
	task_1: counts and displays on LCD digits 0-9 in a round robin 
 *----------------------------------------------------------------------------*/
__NO_RETURN static void task_1(void *argument) { 

  (void)argument;

  for (;;) {
    //osThreadFlagsWait(0x0001U, osFlagsWaitAny ,osWaitForever);
		LCD_clear();                  /* clear LCD */
		LED_On(1U);
		// will block here
		//flag = 1;
		osSemaphoreAcquire(task_2_ID,2000);
		
		osDelay(2000);
    LCD_send_intString(counter);  /* display current counter vale */

		counter++;                    /* update counter value */
    if (counter >= 10) {
      counter = 0;                /* restart counter when it gets to 10 */
    }
		
  }
}

/*----------------------------------------------------------------------------
	task_2: makes the blue LED blink every one second
 *----------------------------------------------------------------------------*/
__NO_RETURN static void task_2(void *argument) {

	(void)argument;

  for (;;) {
		LED_Off(1U);
		osSemaphoreRelease(task_1_ID);
		 if (flag == 1) {
            // Task_1 has blocked, so Task_2 runs
            // Reset flag
         flag = 0;
		 }
    //osDelay(1); // set back to 1000
    //LED_On(1U);      /* turn blue LED on */
    //osDelay(1);
    //LED_Off(1U);     /* turn blue LED off */
	}
}

/*----------------------------------------------------------------------------
	task_3: keeps incrementing a global counter displayed on the LCD 
 *----------------------------------------------------------------------------*/
__NO_RETURN static void task_3(void *argument) {

  (void)argument;

  for (;;) {
		global_counter++;
		osDelay(1000);
		place_lcd_cursor(2);								 /* move cursor to second line */
    LCD_send_intString(global_counter);  /* update global counter value */
		osDelay(200);
		LCD_clear();                         /* clear LCD */
  }
}

/*----------------------------------------------------------------------------
	task_4: keeps decrementing a global counter displayed on the LCD
 *----------------------------------------------------------------------------*/
__NO_RETURN static void task_4(void *argument) {

  (void)argument;

  for (;;) {
		global_counter = global_counter - 10;
		osDelay(3000);
		place_lcd_cursor(2);								 /* move cursor to second line */
    LCD_send_intString(global_counter);  /* update global counter value */
		osDelay(200);
		LCD_clear();                         /* clear LCD */
  }
}

/*----------------------------------------------------------------------------
  thrBUT: check button state
 *----------------------------------------------------------------------------*/
__NO_RETURN static void thrBUT(void *argument) {
  uint32_t button_msk = (1U << Buttons_GetCount()) - 1U;

  (void)argument;

  for (;;) {
    osDelay(delay_val);                        /* Wait */
    if (Buttons_GetState() & (button_msk)) {   /* Wait while holding USER button */
			osThreadFlagsSet(task_1_ID, 0x0001U);
		}
  }

}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *argument) {

  (void)argument;
	
	LED_On(0);   // light green LED
	LCD_setup(); // initialize LCD
	LCD_send_String("Hello World!");
	HAL_Delay(1000); // delay for one second

  LED_Initialize();                                     /* initalize LEDs */
  Buttons_Initialize();                                 /* initalize Buttons */

  tid_thrBUT = osThreadNew (thrBUT, NULL, NULL);        /* create BUT thread */
  if (tid_thrBUT == NULL) { /* add error handling */ }

  task_1_ID = osThreadNew (task_1, NULL, NULL);         /* create task 1 thread */
	osThreadSetPriority(task_1_ID, osPriorityHigh);
  if (task_1_ID == NULL) { /* add error handling */ }
	
	task_2_ID = osThreadNew (task_2, NULL, NULL);         /* create task 2 thread */
	osThreadSetPriority(task_1_ID, osPriorityNormal);
  if (task_2_ID == NULL) { /* add error handling */ }
	
	task_3_ID = osThreadNew (task_3, NULL, NULL);         /* create task 3 thread */
  if (task_3_ID == NULL) { /* add error handling */ }
	
	task_4_ID = osThreadNew (task_4, NULL, NULL);         /* create task 4 thread */
  if (task_4_ID == NULL) { /* add error handling */ }

  osThreadExit();
}
