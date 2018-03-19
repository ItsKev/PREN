/*
 * Shell.c
 *
 *  Created on: Mar 6, 2018
 *      Author: Burak Kizilkaya
 */

#include "Application.h"
#include "FRTOS1.h"
#include "Shell.h"
#include "WAIT1.h"
#include "CLS1.h"
#include "PE_Error.h"
#include "UTIL1.h"
#include "LED.h"
#include "Ultasonic.h"
#include "InfraredTOF.h"

static const char* COMMAND_TABLE[] = { 
		"\t help\n",
		"\t LED_Onboard_R status\n\r",
		"\t LED_Onboard_R on|off|neg\n\r",
		"\t LED_Onboard_Green status\n\r",
		"\t LED_Onboard_Green on|off|neg\n\r",
		"\t US status|start\n\r",
		"\t INF TOF status|start\n\r",
		"\n\r"};

static void TerminalInitText(void) {
	int i;

	CLS1_SendStr((unsigned char*) "Walking Cat, Team33 Terminal ;)\n\r",	CLS1_GetStdio()->stdOut);
	
	for (i = 0; i < (sizeof(COMMAND_TABLE) / sizeof(const char*)); i++) {
		CLS1_SendStr(COMMAND_TABLE[i], CLS1_GetStdio()->stdOut);
	}
}

static uint8_t DoCommand(uint8_t* cmd){
	uint8_t result = ERR_OK; 
	
	if (strcmp(cmd, "help")==0){
		TerminalInitText();
		return result; 
	} else if(strncmp(cmd, "LED", 3)==0){
		result = LEDOnboardParseCommand(cmd); 
	} else if(strncmp(cmd, "US", 2)==0){
		result = US_ParseCommand(cmd); 
	} else if(strncmp(cmd, "INF", 3)==0){
		result = VL_ParseCommand(cmd);
	} else {
		CLS1_SendStr((unsigned char*)"Unknown CMD!\n\r", CLS1_GetStdio()->stdOut);
		return result = ERR_FAILED; 
	}
	
	return result; 
}

static uint8_t ReadAndParseCommand(uint8_t *cmdBuf, size_t cmdBufSize, CLS1_ConstStdIOType *io) {	
	uint8_t res = ERR_OK;
	size_t len;

	if (io == NULL) { /* no I/O handler? */
			return ERR_FAILED;
	}
	
	len = UTIL1_strlen((const char*)cmdBuf);
	if (CLS1_ReadLine(cmdBuf, cmdBuf + len, cmdBufSize - len, io)) {
		len = UTIL1_strlen((const char*)cmdBuf); /* length of buffer string */
		if (len == 0) { /* error case */
			return ERR_FAILED;
		} else if (len == 1 && (cmdBuf[0] == '\n' || cmdBuf[0] == '\r')) { /* eat preceding newline characters */
			cmdBuf[0] = '\0';
		}
		if (len >= cmdBufSize - 1) { /* buffer overflow? Parse what we have, will be likely return an error */
			CLS1_SendStr((unsigned char*)"Overflow\n", CLS1_GetStdio()->stdOut); 
			cmdBuf[0] = '\0'; /* start again */
			res = ERR_OVERFLOW;
		} else if (cmdBuf[len - 1] == '\n' || cmdBuf[len - 1] == '\r') { /* line end: parse command */
			cmdBuf[len - 1] = '\0'; /* remove line end character for parser */
			DoCommand(cmdBuf);
			CLS1_SendStr((unsigned char*)"\n", CLS1_GetStdio()->stdOut); 
			CLS1_PrintPrompt(CLS1_GetStdio());
			cmdBuf[0] = '\0'; /* start again */
		} else {
			/* continue to append to buffer */
		}
	}
	return res;
}

static portTASK_FUNCTION(ShellTask, pvParameters) {
	unsigned char buf[48];
	uint8_t result; 

	(void) pvParameters; /* not used */
	buf[0] = '\0';
	TerminalInitText();
	CLS1_PrintPrompt(CLS1_GetStdio());
	for (;;) {		 
		result = ReadAndParseCommand(buf, sizeof(buf), CLS1_GetStdio()); 
		FRTOS1_vTaskDelay(50/portTICK_RATE_MS);
	}
}

void Shell_Init(void) {	
	if (FRTOS1_xTaskCreate(ShellTask, "Shell", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL)
			!= pdPASS ) {
		for (;;) {
		} /* error */
	}
}
