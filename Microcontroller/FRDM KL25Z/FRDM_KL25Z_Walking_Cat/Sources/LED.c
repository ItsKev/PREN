/*
 * LED_Onboard.c
 *
 *  Created on: Mar 7, 2018
 *      Author: burak
 */

#include "LED_Onboard_R.h"
#include "LED_Onboard_Green.h"
#include "PE_Error.h"
#include "Shell.h"

uint8_t LEDOnboardParseCommand(unsigned char* cmd) {
	uint8_t result = ERR_OK;

	/*For the red LED on Board*/
	if (strcmp(cmd, "LED_Onboard_R status") == 0) {
		CLS1_SendStr((unsigned char*) "LED_Onboard_R status:",
				CLS1_GetStdio()->stdOut);
		if (LED_Onboard_R_Get() != 0) {
			CLS1_SendStr((unsigned char*) "\t on\n", CLS1_GetStdio()->stdOut);
		} else {
			CLS1_SendStr((unsigned char*) "\t off\n", CLS1_GetStdio()->stdOut);
		}
		return result;
	} else if (strcmp(cmd, "LED_Onboard_R on") == 0) {
		LED_Onboard_R_On();
		return result;
	} else if (strcmp(cmd, "LED_Onboard_R off") == 0) {
		LED_Onboard_R_Off();
		return result;
	} else if (strcmp(cmd, "LED_Onboard_R neg") == 0){
		LED_Onboard_R_Neg();
		return result; 
	}

	/*For the green LED on Board*/
	if (strcmp(cmd, "LED_Onboard_Green status") == 0) {
		CLS1_SendStr((unsigned char*) "LED_Onboard_Green status:",
				CLS1_GetStdio()->stdOut);
		if (LED_Onboard_Green_Get() != 0) {
			CLS1_SendStr((unsigned char*) "\t on\n", CLS1_GetStdio()->stdOut);
		} else {
			CLS1_SendStr((unsigned char*) "\t off\n", CLS1_GetStdio()->stdOut);
		}
		return result;
	} else if (strcmp(cmd, "LED_Onboard_Green on") == 0) {
		LED_Onboard_Green_On();
		return result;
	} else if (strcmp(cmd, "LED_Onboard_Green off") == 0) {
		LED_Onboard_Green_Off();
		return result;
	} else if (strcmp(cmd, "LED_Onboard_Green neg") == 0){
		LED_Onboard_Green_Neg();
		return result; 
	}
	
	return result = ERR_FAILED; 

}
