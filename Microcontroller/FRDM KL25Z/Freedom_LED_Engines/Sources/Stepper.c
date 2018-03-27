/*
 * Stepper.c
 * 
 * Nützliche Infos:
 * 		Lasthebemotor: 150 Steps / 1 cm Höhe
 *
 *  Created on: Mar 13, 2018
 *      Author: Burak Kizilkaya
 */

#include "Stepper.h"
#include "PE_Types.h"
#include "PE_LDD.h"
#include "TPM_PDD.h"
#include "string.h"

#include "LiftingMotor_Step.h"
#include "LiftingMotor_Direction.h"
#include "LiftingMotor_Enable.h"
#include "LiftingMotor_MS1.h"
#include "LiftingMotor_MS2.h"

#include "DrivingMotor_Step.h"
#include "DrivingMotor_Direction.h"
#include "DrivingMotor_Enable.h"
#include "DrivingMotor_MS1.h"
#include "DrivingMotor_MS2.h"

//--------------------------------------
/* Macros */
//--------------------------------------
// Methods
#define DrivingMotor_Driver_Enable() 			({DrivingMotor.Driver_Enable = TRUE; DrivingMotor_Enable_ClrVal();})
#define DrivingMotor_Driver_Disable() 			({DrivingMotor.Driver_Enable = FALSE; DrivingMotor_Enable_SetVal();})
#define LiftingMotor_Driver_Disable()			({LiftingMotor.Driver_Enable = FALSE; LiftingMotor_Enable_SetVal();})
#define LiftingMotor_Driver_Enable()			({LiftingMotor.Driver_Enable = TRUE; LiftingMotor_Enable_ClrVal();})

// Constants
#define PI 										3.141592653589793238462643383 // For calculates
#define RADIUS_DRIVING_WHEEL 					2 // cm
#define RADIUS_Lifting_WHEEL 					0.5 // cm
#define TARGET_VELOCITY_DRIVING_MOTOR 			40 // [cm/s]
#define TARGET_VELOCITY_LIFTING_MOTOR 			10 // [cm/s]
#define MICROSTEPPING_RESOLUTION_LIFTINGMOTOR	8 // Fullstep(1), Halfstep(2), Quarterstep(4), Eighterstep(8), Sixteenthstep(16)
#define MICROSTEPPING_RESOLUTION_DRIVINGMOTOR	16 // Fullstep(1), Halfstep(2), Quarterstep(4), Eighterstep(8), Sixteenthstep(16)
#define INITIAL_FREQ 							400 // Hz 
#define LIFTINGMOTOR_ACCELARATIONINKREMENT		5 // It's variable --> Testing
#define DRIVINGMOTOR_ACCELARATIONINKREMENT		5 // It's variable --> Testing

// Calculates-Methods
#define TARGET_FREQ_DRIVING_MOTOR(velocity)		((MICROSTEPPING_RESOLUTION_DRIVINGMOTOR*100*velocity)/(RADIUS_DRIVING_WHEEL*PI)) // (100*v)/(r*pi) 
#define TARGET_FREQ_LIFTING_MOTOR(velocity)		((MICROSTEPPING_RESOLUTION_LIFTINGMOTOR*100*velocity)/(RADIUS_Lifting_WHEEL*PI)) // (100*v)/(r*pi)

//--------------------------------------
/* Variables */
//--------------------------------------
// Global
StepperDevice DrivingMotor;
StepperDevice LiftingMotor;

// Just in this file
static bool liftingMotor_continuous;
static bool liftingMotor_stopped;
static bool liftingMotor_brakes;
static bool liftingMotor_finished;
static uint32_t liftingMotor_accelarationSteps;
static uint32_t liftingMotor_cruisingSteps; 

static bool drivingMotor_continuous;
static bool drivingMotor_stopped;
static bool drivingMotor_brakes;
static bool drivingMotor_finished; 
static uint32_t drivingMotor_accelarationSteps;
static uint32_t drivingMotor_cruisingSteps;


//--------------------------------------
/* Methods */
//--------------------------------------
// Static

// Public (Interface)
/**
 * 
 */
uint8_t Stepper_Init(void) {
	// Driving Motor
	DrivingMotor_Step_Disable(); // No pulse is generated
	DrivingMotor_FORWARD();
	DrivingMotor_Driver_Disable(); // Motor driver is off	

	DrivingMotor.Speed = TARGET_VELOCITY_DRIVING_MOTOR;
	DrivingMotor.TargetFreq = INITIAL_FREQ;
	DrivingMotor.AccelartionFreq = INITIAL_FREQ;
	DrivingMotor.Steps = 0;
	DrivingMotor.State = STOPPED;
	drivingMotor_stopped = FALSE;
	drivingMotor_continuous = FALSE;
	drivingMotor_brakes = FALSE;
	drivingMotor_finished = FALSE; 
	drivingMotor_cruisingSteps = 0;
	drivingMotor_accelarationSteps = 0; 

	// Lifting Motor
	LiftingMotor_Step_Disable(); // No pulse is generated
	LiftingMotor_FORWARD();
	LiftingMotor_Driver_Disable(); // Motor driver is off

	LiftingMotor.Speed = TARGET_VELOCITY_LIFTING_MOTOR;
	LiftingMotor.TargetFreq = INITIAL_FREQ;
	LiftingMotor.AccelartionFreq = INITIAL_FREQ;
	LiftingMotor.Steps = 0;
	LiftingMotor.State = STOPPED;
	liftingMotor_stopped = FALSE;
	liftingMotor_continuous = FALSE;
	liftingMotor_brakes = FALSE; 
	liftingMotor_finished = FALSE; 
	liftingMotor_cruisingSteps = 0; 
	liftingMotor_accelarationSteps = 0; 

	return ERR_OK;
}


void Stepper_TestApplication(void) {
	DrivingMotor_Move(10e3, TARGET_VELOCITY_DRIVING_MOTOR);
	LiftingMotor_Move(10e3, TARGET_VELOCITY_LIFTING_MOTOR);
	
	for (;;) {
	}
}

uint8_t DrivingMotor_Brakes(void) {
	drivingMotor_brakes = TRUE;

	if(DrivingMotor.State != DECELERATING && DrivingMotor.State != STOPPED){
		DrivingMotor.State = DECELERATING; 
	}
	
	return ERR_OK;
}

uint8_t DrivingMotor_MoveContinuous(uint32_t targetVelocity) {
	/* Not implemented yet */
}

uint8_t DrivingMotor_Move(uint32_t stepsWithoutMicrosteppingRevolution, uint32_t targetVelocity) {
	uint32_t tmpSteps = MICROSTEPPING_RESOLUTION_DRIVINGMOTOR * stepsWithoutMicrosteppingRevolution;
	DrivingMotor.TargetFreq = TARGET_FREQ_DRIVING_MOTOR(targetVelocity);

	if (stepsWithoutMicrosteppingRevolution <= 400) {
		drivingMotor_cruisingSteps = 0;
		drivingMotor_accelarationSteps = (tmpSteps / 2);
	} else {
		drivingMotor_accelarationSteps = (DrivingMotor.TargetFreq - INITIAL_FREQ) / DRIVINGMOTOR_ACCELARATIONINKREMENT;
		drivingMotor_cruisingSteps = tmpSteps - (drivingMotor_accelarationSteps * 2);
	}

	drivingMotor_stopped = FALSE;
	drivingMotor_finished = FALSE; 
	DrivingMotor.State = ACCELARATING;
	DrivingMotor_Driver_Enable();
	DrivingMotor_Step_SetFreqHz(DrivingMotor.AccelartionFreq);
	DrivingMotor_Step_Enable();
	
	
	return ERR_OK;
}

void DrivingMotor_Event(void) {
	switch (DrivingMotor.State) {
	case ACCELARATING:
		if ((DrivingMotor.AccelartionFreq < DrivingMotor.TargetFreq)) {
			DrivingMotor_Step_SetFreqHz(DrivingMotor.AccelartionFreq += DRIVINGMOTOR_ACCELARATIONINKREMENT);
		}

		if ((DrivingMotor.Steps == drivingMotor_accelarationSteps)) {
			DrivingMotor.State = CRUSING;
		}
		break;

	case CRUSING:
		if (DrivingMotor.Steps >= drivingMotor_cruisingSteps + drivingMotor_accelarationSteps) {
			DrivingMotor.State = DECELERATING;
		}
		break;

	case DECELERATING:
		if (DrivingMotor.AccelartionFreq > INITIAL_FREQ) {
			DrivingMotor_Step_SetFreqHz(DrivingMotor.AccelartionFreq -= DRIVINGMOTOR_ACCELARATIONINKREMENT);
		}

		if ((DrivingMotor.Steps >= ((drivingMotor_accelarationSteps * 2) + drivingMotor_cruisingSteps))
			 || ((DrivingMotor.AccelartionFreq <= INITIAL_FREQ) && drivingMotor_brakes)) {
			DrivingMotor.State = STOPPED;
			drivingMotor_stopped = TRUE;
		}
		break;

	case CONTINIUOUS:
		if (drivingMotor_brakes) {
			DrivingMotor.State = DECELERATING;
		}

		break;

	case STOPPED:
		if (drivingMotor_stopped) {
			drivingMotor_stopped = FALSE;
			drivingMotor_finished = TRUE;
			drivingMotor_brakes = FALSE;
			drivingMotor_accelarationSteps = 0;
			drivingMotor_cruisingSteps = 0;
			DrivingMotor_Step_Disable();
			DrivingMotor_Driver_Disable(); //--> is not required (Testing)
			DrivingMotor.AccelartionFreq = INITIAL_FREQ;
		}
		break;
	}

	/* Count the Steps */
	if (DrivingMotor.Direction && DrivingMotor.State != STOPPED) {
		DrivingMotor.Steps++;
	} else if (!DrivingMotor.Direction && DrivingMotor.State != STOPPED) {
		DrivingMotor.Steps--;
	}
}



uint8_t LiftingMotor_Move(uint32_t stepsWithoutMicrosteppingRevolution, uint32_t targetVelocity) {
	uint32_t tmpSteps = MICROSTEPPING_RESOLUTION_LIFTINGMOTOR * stepsWithoutMicrosteppingRevolution;
	LiftingMotor.TargetFreq = TARGET_FREQ_LIFTING_MOTOR(targetVelocity);	

	if(stepsWithoutMicrosteppingRevolution <= 400){
		liftingMotor_cruisingSteps = 0; 
		liftingMotor_accelarationSteps = (tmpSteps/2);
	} else {
		liftingMotor_accelarationSteps = (LiftingMotor.TargetFreq-INITIAL_FREQ)/LIFTINGMOTOR_ACCELARATIONINKREMENT;
		liftingMotor_cruisingSteps = tmpSteps - (liftingMotor_accelarationSteps*2); 
	}

	liftingMotor_stopped = FALSE;
	liftingMotor_finished = FALSE; 
	LiftingMotor.State = ACCELARATING;
	LiftingMotor_Driver_Enable();
	LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq);
	LiftingMotor_Step_Enable();

	return ERR_OK;
}

uint8_t LiftingMotor_Brakes(void) {
	liftingMotor_brakes = TRUE;

	return ERR_OK;
}

uint8_t LiftingMotor_MoveContinuous(uint32_t targetFreq) {
	/* Not implemented yet */
}

void LiftingMotor_Event(void) {

	switch (LiftingMotor.State) {
	case ACCELARATING:
		if ((LiftingMotor.AccelartionFreq < LiftingMotor.TargetFreq)) {
			LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq += LIFTINGMOTOR_ACCELARATIONINKREMENT);
		}

		if ((LiftingMotor.Steps == liftingMotor_accelarationSteps)) {
			LiftingMotor.State = CRUSING;
		}
		break;

	case CRUSING:
		if (LiftingMotor.Steps >= liftingMotor_cruisingSteps + liftingMotor_accelarationSteps) {
			LiftingMotor.State = DECELERATING;
		}
		break;

	case DECELERATING:
		if (LiftingMotor.AccelartionFreq > INITIAL_FREQ) {
			LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq -= LIFTINGMOTOR_ACCELARATIONINKREMENT);
		}

		if (LiftingMotor.Steps >= ((liftingMotor_accelarationSteps * 2) + liftingMotor_cruisingSteps)) {
			LiftingMotor.State = STOPPED;
			liftingMotor_stopped = TRUE;
		}
		break;

	case CONTINIUOUS:		
		if(liftingMotor_brakes){
			LiftingMotor.State = DECELERATING; 
		}
		
		break;

	case STOPPED:
		if (liftingMotor_stopped) {
			liftingMotor_stopped = FALSE;
			liftingMotor_finished = TRUE; 
			liftingMotor_accelarationSteps = 0; 
			liftingMotor_cruisingSteps = 0; 
			LiftingMotor_Step_Disable();
			LiftingMotor_Driver_Disable(); //--> is not required
			LiftingMotor.AccelartionFreq = INITIAL_FREQ;
		}
		break;
	}

	if (LiftingMotor.Direction && LiftingMotor.State != STOPPED) {
		LiftingMotor.Steps++;
	} else if (!LiftingMotor.Direction && LiftingMotor.State != STOPPED) {
		LiftingMotor.Steps--;
	}
}

uint8_t Stepper_ParseCommand(unsigned char* cmd){
	uint8_t result = ERR_OK;
	unsigned char tmpDistance[20];
	unsigned char tmpTime[20];

	/* For the control for Steppers with the Shell */
	if(strcmp(cmd, "ant status")==0){
		;
	} else if(strcmp(cmd, "ant n")== 0){
		; 
	} else if (strcmp(cmd, "ant fast") == 0){
		; 
	} else if (strcmp(cmd, "ant slow") == 0){
		; 
	} else if (strcmp(cmd, "ant stop") == 0) {
		; 
	}
	
	if (strcmp(cmd, "lst status") == 0) {
		;
	} else if (strcmp(cmd, "lst n") == 0) {
		;
	} else if (strcmp(cmd, "lst fast") == 0) {
		;
	} else if (strcmp(cmd, "lst slow") == 0) {
		;
	} else if (strcmp(cmd, "lst stop") == 0) {
		;
	}
	
	/*For the control from US_Device with the Shell*/
//	if (strcmp(cmd, "US status") == 0) {
//		CLS1_SendStr((unsigned char*) "US Device front\n", CLS1_GetStdio()->stdOut);
//		CLS1_SendStr((unsigned char*) "\t value:\t", CLS1_GetStdio()->stdOut);
//		UTIL1_Num16uToStrFormatted(tmpDistance, sizeof(tmpDistance), usDeviceFront.value, ' ', 10);
//		CLS1_SendStr(tmpDistance, CLS1_GetStdio()->stdOut);
//		CLS1_SendStr((unsigned char*) " cm\n", CLS1_GetStdio()->stdOut);
//
//		CLS1_SendStr((unsigned char*) "\t time:\t", CLS1_GetStdio()->stdOut);
//		UTIL1_Num16uToStrFormatted(tmpTime, sizeof(tmpTime), usDeviceFront.timeUS, ' ', 10);
//		CLS1_SendStr(tmpTime, CLS1_GetStdio()->stdOut);
//		CLS1_SendStr((unsigned char*) " us\n", CLS1_GetStdio()->stdOut);
//		return result;
//	} else if (strcmp(cmd, "US start") == 0) {
//		usDeviceFront.value = US_usToCentimeters(US_Measure_us(), TEMPERATURE_AIR);
//		return result;
//	}

	return result = ERR_FAILED;
}
