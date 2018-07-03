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
#include "Stepper.h"
#include "Electromagnet_Driver.h"
#include "MeasurmentHandler.h"
#include <string.h>
#include <stdio.h>

static int power(int x, unsigned int y) {
	int temp;
	if (y == 0)
		return 1;
	temp = power(x, y / 2);
	if (y % 2 == 0)
		return temp * temp;
	else
		return x * temp * temp;
}

static const char* COMMAND_TABLE[] = { 
		"\t help\r",
		"\t LED_Onboard_R status|on|off|neg\r",
		"\t LED_Onboard_Green status|on|off|neg\r",
		"\t ant status|n|fast|slow|stop\r",
		"\t lst status|n|fast|slow|stop\r",
		"\t ema	status|on|off\r",
		"\r"};

// reverses a string 'str' of length 'len'
static void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}
 
 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
static int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}
 
// Converts a floating point number to string.
static void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * power(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}


static void TerminalInitText(void) {
	int i;

	CLS1_SendStr((unsigned char*) "Walking Cat, Team33 Terminal ;)\r",	CLS1_GetStdio()->stdOut);
	
	for (i = 0; i < (sizeof(COMMAND_TABLE) / sizeof(const char*)); i++) {
		CLS1_SendStr(COMMAND_TABLE[i], CLS1_GetStdio()->stdOut);
	}
}

static uint8_t DoCommand(uint8_t* cmd){
	uint8_t result = ERR_OK; 
	unsigned char tmpDistanceX[10];
	unsigned char tmpDistanceY[10];
	
	if (strcmp(cmd, (unsigned char*)"help") == 0) {
		TerminalInitText();
		return result;
	} else if (strncmp(cmd, "LED", 3) == 0) {
		result = LEDOnboardParseCommand(cmd);
	} else if ((strncmp(cmd, "ant", 3) == 0) || (strncmp(cmd, "lst", 3) == 0)) {
		result = Stepper_ParseCommand(cmd);
	} else if (strncmp(cmd, "ema", 3) == 0) {
		result = Electromagnet_Driver_ParseCommand(cmd);
	} else if (strncmp(cmd, "start", 5) == 0) {
		// Start parcour
		Parcour_FSM_Handler.StartSignal_FromPI = 1;
	} else if (strncmp(cmd, "stop", 4) == 0) {
		// Stop parcour;
		Parcour_FSM_Handler.parcourFinished = 1; 
	} else if (strncmp(cmd, "target found", 12) == 0) {
		Parcour_FSM_Handler.z3_targetFound = 1;
	} else if (strncmp(cmd, "msg last", 8) == 0) {
		// measurment (calculating) x and z koordinates of load;
		ftoa(Measurment_XAxis(DrivingMotor.Steps/8)-1.5f,tmpDistanceX,1);
		ftoa(Measurment_YAxis(DrivingMotor.Steps/8),tmpDistanceY,1);
		STRING_S();
		CLS1_SendStr(tmpDistanceX, CLS1_GetStdio()->stdOut);
		CLS1_SendStr((unsigned char*)";", CLS1_GetStdio()->stdOut);
		CLS1_SendStr(tmpDistanceY, CLS1_GetStdio()->stdOut);
		
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
