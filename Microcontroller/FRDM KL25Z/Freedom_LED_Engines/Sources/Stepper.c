/*
 * Stepper.c
 *
 *  Created on: Mar 13, 2018
 *      Author: burak
 */

#include "Stepper.h"
#include "PE_Types.h"
#include "PE_LDD.h"
#include "LiftingMotor.h"
#include "LiftingMotor_Direction.h"
#include "LiftingMotor_Enable.h"
#include "LiftingMotor_MS1.h"
#include "LiftingMotor_MS2.h"

#include "DrivingMotor.h"
#include "DrivingMotor_Direction.h"
#include "DrivingMotor_Enable.h"
#include "DrivingMotor_MS1.h"
#include "DrivingMotor_MS2.h"

#define PI 3.141592653589793238462643383

StepperDevice DrivingMotor;
StepperDevice LiftingMotor;

uint8_t StepperInit(void) {
	// Driving Motor
	DrivingMotor_Disable();

	DrivingMotor.Direction = FORWARD;
	DrivingMotor_Direction_SetVal();

	DrivingMotor.Enable = FALSE;
	DrivingMotor_Enable_ClrVal();

	DrivingMotor.MS1 = FALSE;
	DrivingMotor_MS1_ClrVal();

	DrivingMotor.MS2 = FALSE;
	DrivingMotor_MS2_ClrVal();

	DrivingMotor.Speed = 0;
	DrivingMotor.TargetFreq = 1;
	DrivingMotor.AccelartionFreq = 1;
	DrivingMotor.Steps = 0;
	DrivingMotor.AccelarationFinished = FALSE;
	DrivingMotor.State = STOPPED;

	// Lifting Motor
	LiftingMotor_Disable(); 
	
	LiftingMotor.Direction = FORWARD;
	LiftingMotor_Direction_SetVal();

	LiftingMotor.Enable = FALSE;
	LiftingMotor_Enable_ClrVal();

	LiftingMotor.MS1 = FALSE;
	LiftingMotor_MS1_ClrVal();

	LiftingMotor.MS2 = FALSE;
	LiftingMotor_MS2_ClrVal();

	LiftingMotor.Speed = 0;
	LiftingMotor.TargetFreq = 1;
	LiftingMotor.AccelartionFreq = 1;
	LiftingMotor.Steps = 0;
	LiftingMotor.AccelarationFinished = FALSE;
	LiftingMotor.Stepper_TimerDevice = TU1_Init(&LiftingMotor.Stepper_TimerDevice); 
	LiftingMotor.State = STOPPED;

	return ERR_OK;
}

void Stepper_TestApplication(void) {
	for(;;){
		DrivingMotorSetSpeed(10, 2); 
		DrivingMotorMove(10e3); 
		
		WAIT1_WaitOSms(10000); 
	}
}

uint8_t DrivingMotorSetDirection(bool direction) {
	DrivingMotor.Direction = direction;

	if (direction == 1) {
		DrivingMotor_Direction_SetVal();
	} else {
		DrivingMotor_Direction_ClrVal();
	}

	return ERR_OK;
}

uint8_t LifitingMotorSetDirection(bool direction) {
	LiftingMotor.Direction = direction;

	if (direction == 1) {
		LiftingMotor_Direction_SetVal();
	} else {
		LiftingMotor_Direction_ClrVal();
	}

	return ERR_OK;
}

uint8_t DrivingMotorSetSpeed(uint8_t speed, uint8_t radius) {
	uint8_t errorCode;

	DrivingMotor.Speed = speed;
	DrivingMotor.TargetFreq = (100 * speed) / (radius * (double) PI);

	errorCode = ERR_OK;

	return errorCode;
}

uint8_t LifitingMotorSetSpeed(uint8_t speed, uint8_t radius) {
	uint8_t errorCode;

	DrivingMotor.Speed = speed;
	LiftingMotor.TargetFreq = (100 * speed) / (radius * (double) PI);

	errorCode = ERR_OK;

	return errorCode;
}

uint8_t DrivingMotorMove(uint32_t steps) {
	uint32_t accelarationSteps = 0.2 * steps;
	uint32_t cruisingSteps = steps - 2 * accelarationSteps;
	bool moveFinished = FALSE;

	DrivingMotor.State = ACCELARATING;
	DrivingMotor_SetFreqHz(DrivingMotor.AccelartionFreq);
	DrivingMotor_Enable();
	
	while (!moveFinished) {
		switch (DrivingMotor.State) {
		case ACCELARATING:
			if (DrivingMotor.Steps >= accelarationSteps) {
				DrivingMotor.State = CRUSING;
			}
			WAIT1_WaitOSms(1);
			break;

		case CRUSING:
			if (DrivingMotor.Steps >= cruisingSteps + accelarationSteps) {
				DrivingMotor.State = DECELERATING;
			}
			break;

		case DECELERATING:
			if (DrivingMotor.Steps >= cruisingSteps + 2*accelarationSteps) {
				DrivingMotor.State = STOPPED;
			}
			WAIT1_WaitOSms(1);
			break;

		case STOPPED:
			moveFinished = TRUE;
			DrivingMotor_Stop();
			break;
		}

		if (steps == DrivingMotor.Steps) {
			DrivingMotor.State = STOPPED;
		}
	}
}

uint8_t DrivingMotor_Stop(void) {
	DrivingMotor_Disable();
	DrivingMotor_SetFreqHz(1);
	DrivingMotor.AccelartionFreq = 1;
	DrivingMotor.State = STOPPED;

	return ERR_OK;
}

uint8_t LiftingMotor_Stop(void) {
	LiftingMotor_Disable();
	LiftingMotor_SetFreqHz(1);
	LiftingMotor.AccelartionFreq = 1;
	DrivingMotor.State = STOPPED;

	return ERR_OK;
}

