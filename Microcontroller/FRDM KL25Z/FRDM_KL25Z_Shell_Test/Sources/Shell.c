/*
 * Shell.c
 *
 *  Created on: Mar 6, 2018
 *      Author: Burak Kizilkaya
 */

#include "Application.h"
#include "Shell.h"
#include "WAIT1.h"
#include "LED_Onboard_R.h"
#include "LED_Onboard_Green.h"
#include "CLS1.h"
#include "PE_Error.h"
#include "UTIL1.h"
#include <string.h>

static const unsigned char* COMMAND_TABLE[] = { 
		"\t help\n",
		"\t LED_Onboard_R status\n",
		"\t LED_Onboard_R on|off|neg\n",
		"\t LED_Onboard_Green status\n",
		"\t LED_Onboard_Green on|off|neg\n" };

static void TerminalInitText(void) {
	int i;

	CLS1_SendStr((unsigned char*) "Walking Cat, Team33 Terminal ;)\n",	CLS1_GetStdio()->stdOut);
	for (i = 0; i < (sizeof(COMMAND_TABLE) / sizeof(unsigned char*)); i++) {
		CLS1_SendStr(COMMAND_TABLE[i], CLS1_GetStdio()->stdOut);
	}
}

static uint8_t DoCommand(uint8_t* cmd){
	uint8_t result = ERR_OK; 
	
	if (strcmp(cmd, "help")==0){
		TerminalInitText();
		return result; 
	} else if(strcmp(cmd, "LED_Onboard_R status")==0){
		CLS1_SendStr((unsigned char*) "LED_Onboard_R status:",CLS1_GetStdio()->stdOut);
		if (LED_Onboard_R_Get()!=0){
			CLS1_SendStr((unsigned char*) "\t on\n",CLS1_GetStdio()->stdOut);
		} else{
			CLS1_SendStr((unsigned char*) "\t off\n",CLS1_GetStdio()->stdOut);
		}
	} else if (strcmp(cmd, "LED_Onboard_R on")==0){
		LED_Onboard_R_On(); 
	}	
	else {
		return result = ERR_FAILED; 
	}
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
			//CLS1_SendStr((unsigned char*)"Command was OK\n", CLS1_GetStdio()->stdOut); 
			DoCommand(cmdBuf);
			cmdBuf[0] = '\0'; /* start again */
		} else {
			/* continue to append to buffer */
		}
	}
	return res;
}

void Shell_Init(void) {
	unsigned char buf[48];
	uint8_t result; 
	buf[0] = '\0';

	TerminalInitText();

	for (;;) {
		//CLS1_PrintPrompt(CLS1_GetStdio()->stdOut);
		result = ReadAndParseCommand(buf, sizeof(buf), CLS1_GetStdio()); 
	}

}


