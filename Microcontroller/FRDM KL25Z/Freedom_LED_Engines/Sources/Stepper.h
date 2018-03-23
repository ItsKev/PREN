/*
 * Stepper.h
 *
 *  Created on: Mar 13, 2018
 *      Author: burak
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "PE_LDD.h"
#include "PE_Types.h"


#define FORWARD TRUE /* Forward-Direction for the Stepper-Motors */
#define BACKWARD FALSE /* Backward-Direction for the Stepper-Motors */


typedef enum{
	STOPPED,
	ACCELARATING,
	CRUSING,
	DECELERATING,
	CONTINIUOUS
}StepperState;

typedef struct { 
	bool Direction; 
	uint16_t Speed; 
	uint16_t TargetFreq; 
	uint16_t AccelartionFreq; // Need for Accelaration
	bool MS1; 
	bool MS2; 
	bool Driver_Enable;
	uint32_t Steps;
	StepperState State; 
}StepperDevice;

/* Global Variables */
extern StepperDevice DrivingMotor; 
extern StepperDevice LiftingMotor; 


uint8_t DrivingMotorSetDirection(bool direction);  

uint8_t DrivingMotorMove(uint32_t stepsWithoutMicrosteppingRevolution); 

uint8_t LiftingMotor_SetDirection(bool direction); 

uint8_t LiftingMotor_Move(uint32_t stepsWithoutMicrosteppingRevolution); 

void LiftingMotor_Event(void);

void DrivingMotor_Event(void); 

uint8_t StepperInit(void); 

void Stepper_TestApplication(void); 

uint8_t Stepper_ParseCommand(unsigned char* cmd); 




#endif /* STEPPER_H_ */
