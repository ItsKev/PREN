/*
 * Stepper.c
 *
 *  Created on: Mar 13, 2018
 *      Author: burak
 */

#include "Stepper.h"
#include "PE_Types.h"

typedef struct {
	bool direction; 
	uint16_t speed; 
	bool MS1; 
	bool MS2; 
	bool MS3; 
}StepperDevice;









uint8_t StepperInit(void); 
