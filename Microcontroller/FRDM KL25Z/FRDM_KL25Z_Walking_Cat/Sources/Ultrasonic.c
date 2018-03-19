/*
 * Ultrasonic.c
 *
 *  Created on: Mar 7, 2018
 *      Author: burak
 */

/* includes */
#include "Ultasonic.h"
#include "US_Trig.h"
#include "Shell.h"

/* defines */
#define TEMPERATURE_AIR 19 // Unit: °C

/* variable */
US_DeviceType usDeviceFront;

/* methods */

static uint16_t calcAirspeed_dms(uint8_t temperatureCelsius) {
  /* Return the airspeed depending on the temperature, in deci-meter per second */
  unsigned int airspeed; /* decimeters per second */
 
  airspeed = 3313 + (6 * temperatureCelsius); /* dry air, 0% humidity, see http://en.wikipedia.org/wiki/Speed_of_sound */
  airspeed -= (airspeed/100)*15; /* factor in ~15% for a relative humidity of ~40% */
  return airspeed;
}

void US_Init(void) {
	usDeviceFront.state = ECHO_IDLE;
	usDeviceFront.capture = 0;
	usDeviceFront.trigDevice = US_Trig_Init(NULL);
	usDeviceFront.echoDevice = TU1_Init(&usDeviceFront);
	usDeviceFront.value = 0;
	usDeviceFront.timeUS = 0; 
}

void US_EventEchoOverflow(LDD_TUserData *UserDataPtr) {
	US_DeviceType* ptr = (US_DeviceType*) UserDataPtr;

	ptr->state = ECHO_OVERFLOW;
}

void US_EventEchoCapture(LDD_TUserData *UserDataPtr) {
	US_DeviceType* ptr = (US_DeviceType*) UserDataPtr;

	if (ptr->state == ECHO_TRIGGERED) { /* 1st edge sending, this is the raising edge, start measurement */
		TU1_ResetCounter(ptr->echoDevice);
		ptr->state = ECHO_MEASURE;
	} else if (ptr->state == ECHO_MEASURE) { /* 2nd edge, this is the faling edge: use measurement  */
		(void) TU1_GetCaptureValue(ptr->echoDevice, 0, &ptr->capture);
		ptr->state = ECHO_FINISHED;
	}
}

uint16_t US_usToCentimeters(uint16_t microseconds, uint8_t temperatureCelsius) {
	return (microseconds*100UL)/calcAirspeed_dms(temperatureCelsius)/2; /* 2 because of two way */
	//return (((float)microseconds/1000)*343.46)/2;
}

uint16_t US_Measure_us(void) {
	/* send 10us pulse on TRIG line */
	US_Trig_SetVal(usDeviceFront.trigDevice);
	WAIT1_Waitus(10);
	usDeviceFront.state = ECHO_TRIGGERED;
	US_Trig_ClrVal(usDeviceFront.trigDevice);

	/* Wait for echo */
	while (usDeviceFront.state != ECHO_FINISHED) { /* measurement took too long! */
		if (usDeviceFront.state == ECHO_OVERFLOW) {
			usDeviceFront.state = ECHO_IDLE;
			return ERR_FAILED; /* no echo */
		}
	}
	usDeviceFront.timeUS = (usDeviceFront.capture * 1000UL) / (TU1_CNT_INP_FREQ_U_0 / 1000);
	return usDeviceFront.timeUS;
}

uint8_t US_ParseCommand(unsigned char* cmd) {
	uint8_t result = ERR_OK;
	unsigned char tmpDistance[20];
	unsigned char tmpTime[20]; 

	/*For the control from US_Device with the Shell*/
	if (strcmp(cmd, "US status") == 0) {
		CLS1_SendStr((unsigned char*)"US Device front\n", CLS1_GetStdio()->stdOut); 
		CLS1_SendStr((unsigned char*)"\t value:\t", CLS1_GetStdio()->stdOut);
		UTIL1_Num16uToStrFormatted(tmpDistance, sizeof(tmpDistance),usDeviceFront.value, ' ', 10);
		CLS1_SendStr(tmpDistance, CLS1_GetStdio()->stdOut); 
		CLS1_SendStr((unsigned char*)" cm\n", CLS1_GetStdio()->stdOut);
		
		CLS1_SendStr((unsigned char*)"\t time:\t", CLS1_GetStdio()->stdOut);
		UTIL1_Num16uToStrFormatted(tmpTime, sizeof(tmpTime),usDeviceFront.timeUS, ' ', 10);
		CLS1_SendStr(tmpTime, CLS1_GetStdio()->stdOut); 
		CLS1_SendStr((unsigned char*)" us\n", CLS1_GetStdio()->stdOut);
		return result; 
	} else if(strcmp(cmd, "US start") == 0){
		usDeviceFront.value = US_usToCentimeters(US_Measure_us(), TEMPERATURE_AIR); 
		return result; 
	}

	return result=ERR_FAILED;
}
