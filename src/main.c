/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h" //maybe
#include "stm32f4xx_tim.h" //maybe
#include "stm32f4xx_gpio.h" //yes?
#include "misc.h" //why?
#include <stdbool.h> //why?
#include "GPIO_init.h" //check
#include "ADC.h"

#include "FreeRTOSConfig.h"

#include "FreeRTOS.h"
#include "task.h"

#define TESTING_SAMPLES		(2000)
uint32_t all_freqs[TESTING_SAMPLES];

void blinkyTask(void *dummy){
	while(1){
		GPIOB->ODR ^= GPIO_Pin_12;
		/* maintain LED C9 status for 200ms */
		vTaskDelay(100);
	}
}

void hADC(void *dummy){
	//ADC_init();

	while(1){
		vTaskDelay(800);
	}
}

void vGeneralTaskInit(void){
	xTaskCreate(blinkyTask,
		(const signed char *)"blinkyTask",
		configMINIMAL_STACK_SIZE,
		NULL,                 // pvParameters
		tskIDLE_PRIORITY + 1, // uxPriority
		NULL              ); // pvCreatedTask */
	xTaskCreate(hADC,
		(const signed char *)"hADC",
		configMINIMAL_STACK_SIZE,
		NULL,                 // pvParameters
		tskIDLE_PRIORITY + 1, // uxPriority
		NULL              ); // pvCreatedTask */
}

int main(void)
{
	init_GPIOB;
	//init_pwm_in();

	vGeneralTaskInit();
	vTaskStartScheduler();

	//Should never get here
	for(;;);
}

void vApplicationTickHook(void) {
}

/* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created.  It is also called by various parts of the
   demo application.  If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
void vApplicationMallocFailedHook(void) {
  taskDISABLE_INTERRUPTS();
  for(;;);
}

/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
   task.  It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()).  If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
void vApplicationIdleHook(void) {
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
  (void) pcTaskName;
  (void) pxTask;
  /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     function is called if a stack overflow is detected. */
  taskDISABLE_INTERRUPTS();
  for(;;);
}

void TIM5_IRQHandler(){
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		//GPIOD->ODR ^= GPIO_Pin_15;
	}else if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET){
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
		GPIOD->ODR ^= GPIO_Pin_14;
	}
}

void DMA1_Stream2_IRQHandler(){

}
