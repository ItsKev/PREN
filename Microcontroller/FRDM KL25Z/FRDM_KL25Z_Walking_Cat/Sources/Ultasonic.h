/*
 * Ultasonic.h
 *
 *  Created on: Mar 7, 2018
 *      Author: burak
 */

#ifndef ULTASONIC_H_
#define ULTASONIC_H_

#include "PE_LDD.h"
#include "PE_Error.h"
#include "TU1.h"

typedef enum {
	ECHO_IDLE, /* device not used */ 
	ECHO_TRIGGERED, /* started trigger pulse */
	ECHO_MEASURE, /* measuring echo pulse */
	ECHO_OVERFLOW, /* measurement took too long */
	ECHO_FINISHED /* measurement finished */
} US_EchoState;

typedef struct {
	LDD_TDeviceData* trigDevice; /* */
	LDD_TDeviceData* echoDevice; /**/
	volatile US_EchoState state; /**/
	TU1_TValueType capture; /**/
	uint16_t value; 
	uint16_t timeUS; 
} US_DeviceType; 

extern US_DeviceType usDeviceFront; 

void US_Init(void); 

void US_EventEchoOverflow(LDD_TUserData *UserDataPtr);

void US_EventEchoCapture(LDD_TUserData *UserDataPtr);

uint16_t US_usToCentimeters(uint16_t microseconds, uint8_t temperatureCelsius);


uint16_t US_Measure_us(void);

uint8_t US_ParseCommand(unsigned char* cmd);

#endif /* ULTASONIC_H_ */
