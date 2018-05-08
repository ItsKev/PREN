/*
 * Electromagnet.c
 *
 *  Created on: Mar 29, 2018
 *      Author: burak
 */

#include "Electromagnet_Driver.h"
#include "Electromagnet.h"
#include "Shell.h"
#include "PE_Types.h"

bool Electromagnet_Driver_State; 

uint8_t Electromagnet_Driver_Init(void){
	Electromagnet_Driver_OFF();
	return ERR_OK;
}

uint8_t Electromagnet_Driver_ParseCommand(unsigned char* cmd){
	uint8_t result = ERR_OK;
	
	if(strcmp(cmd, (unsigned char*)"ema status")==0){
		CLS1_SendStr((unsigned char*)"\t Electromagnet: ", CLS1_GetStdio()->stdOut); 
		
		// checking state of the Electromagnet-Driver
		if(Electromagnet_Driver_State){
			CLS1_SendStr((unsigned char*)"ON", CLS1_GetStdio()->stdOut); 
		} else {
			CLS1_SendStr((unsigned char*)"OFF", CLS1_GetStdio()->stdOut); 
		}
	} 
	
	else if (strcmp(cmd, "ema on") == 0){
		Electromagnet_Driver_ON();
		STRING_SUCCES(); 
	}
	
	else if (strcmp(cmd, "ema off") == 0){
		Electromagnet_Driver_OFF(); 
		STRING_SUCCES(); 
	}
	
	return result; 
}
