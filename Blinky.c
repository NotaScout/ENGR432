#include <stdio.h>
#include "main.h"
#include "Board_LED.h"
#include "Board_Buttons.h"
#include "LCD.h"
#include "RTE_Components.h"

// Declare semaphore
osSemaphoreId_t lcdSemaphore;

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (512U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

static volatile uint32_t delay_val = 500U;

static osThreadId_t task_1_ID;
static osThreadId_t task_2_ID;
static osThreadId_t task_3_ID;
static osThreadId_t task_4_ID;
static osThreadId_t tid_thrBUT;

unsigned int counter = 0;          
unsigned int global_counter = 499; 
unsigned int global_counter_decrement = 499;

// Task 1: counts and displays on LCD digits 0-9 in a round robin 
__NO_RETURN static void task_1(void *argument) { 
  (void)argument;

  for (;;) {
    //osThreadFlagsWait(0x0001U, osFlagsWaitAny ,osWaitForever);
		osSemaphoreAcquire(lcdSemaphore, osWaitForever);
    counter++; 
    osDelay(200);
    LCD_place_cursor(0x80);
    LCD_send_String("  ");
    LCD_place_cursor(0x80);
    LCD_send_intString(counter);
		
    if (counter >= 10) {
      counter = 0;
    }
		osSemaphoreRelease(lcdSemaphore);
  } 
	
}

// Task 2: makes the blue LED blink every one second
__NO_RETURN static void task_2(void *argument) {
  (void)argument;

  for (;;) {
    osDelay(1000);
    LED_On(1U);
    osDelay(1000);
    LED_Off(1U);
  }
}

// Task 3: keeps incrementing a global counter displayed on the LCD 
__NO_RETURN static void task_3(void *argument) {
  (void)argument;

  for (;;) {
    osSemaphoreAcquire(lcdSemaphore, osWaitForever);
    global_counter++;
    osDelay(1000);
    LCD_place_cursor(0xC6);
    LCD_send_intString(global_counter);
    osSemaphoreRelease(lcdSemaphore);
    osDelay(200);
  }
}

// Task 4: keeps decrementing a global counter displayed on the LCD
__NO_RETURN static void task_4(void *argument) {
  (void)argument;

  for (;;) {
    osSemaphoreAcquire(lcdSemaphore, osWaitForever);
    global_counter_decrement = global_counter_decrement - 10;
    osDelay(3000);
    LCD_place_cursor(0xC0);
    LCD_send_intString(global_counter_decrement);
    osSemaphoreRelease(lcdSemaphore);
    osDelay(200);
  }
}

// thrBUT: check button state
__NO_RETURN static void thrBUT(void *argument) {
  uint32_t button_msk = (1U << Buttons_GetCount()) - 1U;
  (void)argument;

  for (;;) {
    osDelay(delay_val);
    if (Buttons_GetState() & (button_msk)) {
      osThreadFlagsSet(task_1_ID, 0x0001U);
    }
  }
}

// Application main thread
__NO_RETURN void app_main (void *argument) {
  (void)argument;
	
  // Create semaphore
  lcdSemaphore = osSemaphoreNew(1, 1, NULL);
  if (lcdSemaphore == NULL) { /* add error handling */ }
	
  LED_On(0);
  LCD_setup();
  LCD_send_String("Hello World!");
  HAL_Delay(1000);
  LCD_clear();

  LED_Initialize();
  Buttons_Initialize();

  tid_thrBUT = osThreadNew (thrBUT, NULL, NULL);
  if (tid_thrBUT == NULL) { /* add error handling */ }

  task_1_ID = osThreadNew (task_1, NULL, NULL);
  osThreadSetPriority(task_1_ID, osPriorityHigh);
  if (task_1_ID == NULL) { /* add error handling */ }

  task_2_ID = osThreadNew (task_2, NULL, NULL);
  if (task_2_ID == NULL) { /* add error handling */ }

  task_3_ID = osThreadNew (task_3, NULL, NULL);
  if (task_3_ID == NULL) { /* add error handling */ }

  task_4_ID = osThreadNew (task_4, NULL, NULL);
  if (task_4_ID == NULL) { /* add error handling */ }

  osThreadExit();
}
