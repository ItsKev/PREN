/*
 * Application.c
 *
 *  Created on: Mar 5, 2018
 *      Author: burak
 */

#include "Application.h"
#include "LED_Onboard_R.h"
#include "LED_Onboard_Green.h"
#include "FRTOS1.h"
#include "Shell.h"
#include "Sensoric.h"
#include "Stepper.h"

static portTASK_FUNCTION(Task1, pvParameters) {
	(void) pvParameters; /* parameter not used */
	
	for (;;) {
		//LED_Onboard_Green_Neg(); 
		FRTOS1_vTaskDelay(1000/portTICK_RATE_MS);
	}
}

void APP_Run(void) {
	Shell_Init();
	Sensoric_Init();
	Stepper_Init(); 

	if (FRTOS1_xTaskCreate(
			Task1, /* pointer to the task */
			"Task1", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS ) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	FRTOS1_vTaskStartScheduler();
}
