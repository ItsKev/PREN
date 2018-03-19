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
#include "TU1.h"


#define FORWARD TRUE /* Forward-Direction for the Stepper-Motors */
#define BACKWARD FALSE /* Backward-Direction for the Stepper-Motors */
#define RADIUS_DRIVING_WHEEL 2 /* 2cm */
#define RADIUS_Lifting_WHEEL 2 /* 2cm */

typedef enum{
	STOPPED,
	ACCELARATING,
	CRUSING,
	DECELERATING
}StepperState;

typedef struct {
	bool AccelarationFinished; 
	bool Direction; 
	uint16_t Speed; 
	uint16_t TargetFreq; 
	uint16_t AccelartionFreq; // Need for Accelaration
	bool MS1; 
	bool MS2; 
	bool Enable;
	uint32_t Steps;
	StepperState State; 
	LDD_TDeviceData* Stepper_TimerDevice; 
}StepperDevice;

/* Variablen */
extern StepperDevice DrivingMotor; 
extern StepperDevice LiftingMotor; 


uint8_t DrivingMotorSetDirection(bool direction); 

uint8_t DrivingMotorSetSpeed(uint8_t speed, uint8_t radius); 

uint8_t DrivingMotorMove(uint32_t steps); 

uint8_t DrivingMotor_Stop(void); 

uint8_t LifitingMotorSetDirection(bool direction); 

uint8_t LifitingMotorSetSpeed(uint8_t speed, uint8_t radius); 

uint8_t LifitingMotorMove(uint32_t steps); 

uint8_t LiftingMotor_Stop(void); 

uint8_t StepperInit(void); 

void Stepper_TestApplication(void); 

uint8_t Stepper_ParseCommand(unsigned char* cmd); 




#endif /* STEPPER_H_ */
