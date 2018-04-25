/*
 * Sensoric.c
 *
 *  Created on: Mar 7, 2018
 *      Author: burak
 */
#include "LED_Onboard_Green.h"
#include "FRTOS1.h"
#include "Ultasonic.h"


static portTASK_FUNCTION(SensoricTask, pvParameters) {
	uint8_t result = ERR_OK; 
	
	(void) pvParameters; /* not used */
	
	
	/* Initialization Sensoric */
	US_Init();
//	result = VL_Init(); 
//	result = VL_Test(VL53L0X_DEFAULT_I2C_ADDRESS); 
	
	for (;;) {		 
		FRTOS1_vTaskDelay(50/portTICK_RATE_MS);
	}
}


void Sensoric_Init(void){
	if (FRTOS1_xTaskCreate(SensoricTask, "Sensoric", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL)
				!= pdPASS ) {
			for (;;) {
			} /* error */
		}
}
