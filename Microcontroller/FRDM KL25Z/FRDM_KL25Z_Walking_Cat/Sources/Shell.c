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
#include "Stepper.h"
#include "Electromagnet_Driver.h"

static const char* COMMAND_TABLE[] = { 
		"\t help\r",
		"\t LED_Onboard_R status|on|off|neg\r",
		"\t LED_Onboard_Green status|on|off|neg\r",
		"\t US status|start\r",
		"\t ant status|n|fast|slow|stop\r",
		"\t lst status|n|fast|slow|stop\r",
		"\t ema	status|on|off\r",
		"\r"};

static void TerminalInitText(void) {
	int i;

	CLS1_SendStr((unsigned char*) "Walking Cat, Team33 Terminal ;)\r",	CLS1_GetStdio()->stdOut);
	
	for (i = 0; i < (sizeof(COMMAND_TABLE) / sizeof(const char*)); i++) {
		CLS1_SendStr(COMMAND_TABLE[i], CLS1_GetStdio()->stdOut);
	}
}

static uint8_t DoCommand(uint8_t* cmd){
	uint8_t result = ERR_OK; 
	
	if (strcmp(cmd, (unsigned char*)"help") == 0) {
		TerminalInitText();
		return result;
	} else if (strncmp(cmd, "LED", 3) == 0) {
		result = LEDOnboardParseCommand(cmd);
	} else if (strncmp(cmd, "US", 2) == 0) {
		result = US_ParseCommand(cmd);
	} else if ((strncmp(cmd, "ant", 3) == 0) || (strncmp(cmd, "lst", 3) == 0)) {
		result = Stepper_ParseCommand(cmd);
	} else if (strncmp(cmd, "ema", 3) == 0) {
		result = Electromagnet_Driver_ParseCommand(cmd);
	} else if (strncmp(cmd, "start", 5) == 0) {
		// Start parcour
		Parcour_FSM_Handler.StartSignal_FromPI = 1;
	} else if (strncmp(cmd, "stop", 4) == 0) {
		// Stop parcour;
	} else if (strncmp(cmd, "target found", 12) == 0) {
		Parcour_FSM_Handler.z3_targetFound = 1;
	} else if (strncmp(cmd, "msg last", 8) == 0) {
		// measurment (calculating) x and z koordinates of load;
	} else if (strncmp(cmd, "mst 2 reached", 13) == 0) {
		// Parcour is finished --> limit switched detected;
	} else {
		CLS1_SendStr((unsigned char*) "Unknown CMD!\r", CLS1_GetStdio()->stdOut);
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
			CLS1_SendStr((unsigned char*)"\n\r", CLS1_GetStdio()->stdOut); // new line  
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
	//TerminalInitText();
	//STRING_PROMPT(); 
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
