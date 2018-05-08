/*
 * Stepper.c
 * 
 * Nützliche Infos:
 *
 *  Created on: Mar 13, 2018
 *      Author: Burak Kizilkaya
 */

#include "Stepper.h"
#include "PE_Types.h"
#include "PE_LDD.h"
#include "Shell.h"

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
#define RADIUS_DRIVING_WHEEL 					1.74 // cm
#define RADIUS_Lifting_WHEEL 					0.5 // cm
#define TARGET_VELOCITY_DRIVING_MOTOR 			25 // [cm/s]
#define TARGET_VELOCITY_LIFTING_MOTOR 			10 // [cm/s]
#define MICROSTEPPING_RESOLUTION_LIFTINGMOTOR	8 // Fullstep(1), Halfstep(2), Quarterstep(4), Eighterstep(8), Sixteenthstep(16)
#define MICROSTEPPING_RESOLUTION_DRIVINGMOTOR	8 // Fullstep(1), Halfstep(2), Quarterstep(4), Eighterstep(8), Sixteenthstep(16)
#define INITIAL_FREQ 							400 // Hz 
#define LIFTINGMOTOR_ACCELARATIONINKREMENT		2 // It's variable --> Testing
#define DRIVINGMOTOR_ACCELARATIONINKREMENT		5 // It's variable --> Testing

// Calculates-Methods
// (100*v)/(r*pi)
#define TARGET_FREQ_DRIVING_MOTOR(velocity)		((MICROSTEPPING_RESOLUTION_DRIVINGMOTOR*100*velocity)/(RADIUS_DRIVING_WHEEL*PI)) 
#define TARGET_FREQ_LIFTING_MOTOR(velocity)		((MICROSTEPPING_RESOLUTION_LIFTINGMOTOR*100*velocity)/(RADIUS_Lifting_WHEEL*PI))

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
static uint32_t liftingMotor_oldSteps; 
static uint32_t liftingMotor_currentSteps; 
static bool liftingMotor_speedChanges;
static uint16_t liftingMotor_speedChangesSteps;

static bool drivingMotor_continuous;
static bool drivingMotor_stopped;
static bool drivingMotor_brakes;
static bool drivingMotor_finished; 
static uint32_t drivingMotor_accelarationSteps;
static uint32_t drivingMotor_cruisingSteps;
static uint32_t drivingMotor_oldSteps; 
static uint32_t drivingMotor_currentSteps;
static bool drivingMotor_speedChanges;
static uint16_t drivingMotor_speedChangesSteps;


//--------------------------------------
/* Methods */
//--------------------------------------
/**
 * 
 */
uint8_t Stepper_Init(void) {
	// Driving Motor
	DrivingMotor_Step_Disable(); // No pulse is generated
	DrivingMotor_FORWARD();
	DrivingMotor_Driver_Disable(); // Motor driver is off	
	DrivingMotor_MS2_SetVal();
	DrivingMotor_MS1_SetVal();

	DrivingMotor.Speed = 0;
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
	drivingMotor_oldSteps = 0; 
	drivingMotor_currentSteps = 0; 
	drivingMotor_speedChanges = 0;
	drivingMotor_speedChangesSteps = 0;

	// Lifting Motor
	LiftingMotor_Step_Disable(); // No pulse is generated
	LiftingMotor_FORWARD();
	LiftingMotor_Driver_Enable(); // Motor driver is off
	LiftingMotor_MS1_SetVal();
	LiftingMotor_MS2_SetVal();

	LiftingMotor.Speed = 0;
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
	liftingMotor_oldSteps = 0; 
	liftingMotor_currentSteps = 0; 
	liftingMotor_speedChanges = 0;
	liftingMotor_speedChangesSteps = 0;

	return ERR_OK;
}


void Stepper_TestApplication(void) {
	DrivingMotor_Move(10e3);
	LiftingMotor_Move(10e3);
	
	for (;;) {
	}
}

uint8_t DrivingMotor_Brakes(uint32_t brakingDistanceSteps) {
	drivingMotor_brakes = TRUE;
	uint16_t actualSteps = drivingMotor_currentSteps; 
	uint16_t differenceSteps = brakingDistanceSteps*MICROSTEPPING_RESOLUTION_DRIVINGMOTOR - drivingMotor_accelarationSteps; 
	bool notDifference = 0; 
	
	if (brakingDistanceSteps == 0) {
		notDifference = 1; 
	}
	
	if(differenceSteps < 0 ){
		differenceSteps = (-1) * differenceSteps; 
	}
	
	while ((DrivingMotor.Steps <= actualSteps + differenceSteps) && !notDifference); // wait until braking steps is finished

	if(DrivingMotor.State != DECELERATING && DrivingMotor.State != STOPPED && !drivingMotor_speedChanges){
		DrivingMotor.State = DECELERATING; 
		drivingMotor_speedChanges = 0;
		STRING_SUCCES(); 
	}
	
	
	return ERR_OK;
}

uint8_t DrivingMotor_MoveContinuous(void) {
	DrivingMotor_Move(10e3); 
	STRING_SUCCES();
	return ERR_OK;
}

uint8_t DrivingMotor_SetSpeed(uint8_t targetVelocity){
	uint8_t result = ERR_OK; 
	uint16_t tmpTargetFreq = TARGET_FREQ_DRIVING_MOTOR(targetVelocity);
	uint32_t oldSteps = drivingMotor_cruisingSteps + 2*drivingMotor_accelarationSteps;
	
	if (DrivingMotor.State == CRUSING || DrivingMotor.State == CONTINIUOUS) {
		if (tmpTargetFreq > DrivingMotor.TargetFreq) {
			DrivingMotor.State = ACCELARATING;
		} else if (tmpTargetFreq < DrivingMotor.TargetFreq){
			DrivingMotor.State = DECELERATING;
		}
		DrivingMotor.TargetFreq = tmpTargetFreq;
		DrivingMotor.Speed = targetVelocity;
		drivingMotor_speedChanges = 1; // Signalisation for speed changes
		drivingMotor_accelarationSteps = (DrivingMotor.TargetFreq - INITIAL_FREQ) / DRIVINGMOTOR_ACCELARATIONINKREMENT;
		drivingMotor_cruisingSteps = oldSteps - 2*drivingMotor_accelarationSteps;
		drivingMotor_speedChangesSteps = drivingMotor_currentSteps + MICROSTEPPING_RESOLUTION_DRIVINGMOTOR * 400; // 2 turn for the new speed
		STRING_SUCCES();
		return result;
	}
	STRING_FAILURE();
	return result;
}

uint8_t DrivingMotor_Move(int16_t stepsWithoutMicrosteppingRevolution) {
	uint32_t tmpSteps; 
	DrivingMotor.TargetFreq = TARGET_FREQ_DRIVING_MOTOR(TARGET_VELOCITY_DRIVING_MOTOR);
	DrivingMotor.Speed = TARGET_VELOCITY_DRIVING_MOTOR; 
	drivingMotor_currentSteps = 0; 
	drivingMotor_accelarationSteps = 0;
	drivingMotor_cruisingSteps = 0;
	
	if(stepsWithoutMicrosteppingRevolution < 0){
		tmpSteps = (-1) * MICROSTEPPING_RESOLUTION_DRIVINGMOTOR * stepsWithoutMicrosteppingRevolution;
		DrivingMotor_BACKWARD(); 
	} else {
		tmpSteps = MICROSTEPPING_RESOLUTION_DRIVINGMOTOR * stepsWithoutMicrosteppingRevolution;
		DrivingMotor_FORWARD(); 
	}
	
	if (tmpSteps <= 400*MICROSTEPPING_RESOLUTION_DRIVINGMOTOR) {
		drivingMotor_cruisingSteps = 0;
		drivingMotor_accelarationSteps = (tmpSteps / 2);
	} else {
		drivingMotor_accelarationSteps = (DrivingMotor.TargetFreq - INITIAL_FREQ) / DRIVINGMOTOR_ACCELARATIONINKREMENT;
		drivingMotor_cruisingSteps = tmpSteps - (drivingMotor_accelarationSteps * 2);
	}

	// Invalid steps for move Backward and Steps are less then move Steps
	if (!DrivingMotor.Direction && (tmpSteps/MICROSTEPPING_RESOLUTION_DRIVINGMOTOR) > DrivingMotor.Steps){ 
		STRING_FAILURE(); 
		return ERR_FAILED; 
	}
	
	drivingMotor_stopped = FALSE;
	drivingMotor_finished = FALSE; 
	DrivingMotor.State = ACCELARATING;
	DrivingMotor_Driver_Enable();
	DrivingMotor_Step_SetFreqHz(DrivingMotor.AccelartionFreq);
	DrivingMotor_Step_Enable();	
	STRING_SUCCES(); 
	
	return ERR_OK;
}

void DrivingMotor_Event(void) {
	switch (DrivingMotor.State) {
	case ACCELARATING:
		if ((DrivingMotor.AccelartionFreq < DrivingMotor.TargetFreq)) {
			DrivingMotor_Step_SetFreqHz(DrivingMotor.AccelartionFreq += DRIVINGMOTOR_ACCELARATIONINKREMENT);
		}

		if ((drivingMotor_currentSteps >= drivingMotor_accelarationSteps) && !drivingMotor_speedChanges) {
			DrivingMotor.State = CRUSING;
		} else if(drivingMotor_speedChangesSteps <= drivingMotor_currentSteps && drivingMotor_speedChanges){
			DrivingMotor.State = CRUSING;
			drivingMotor_speedChanges = 0;
		}
		
		break;

	case CRUSING:
		if (drivingMotor_currentSteps >= drivingMotor_cruisingSteps + drivingMotor_accelarationSteps) {
			DrivingMotor.State = DECELERATING;
		}
		break;

	case DECELERATING:
		// Normal-Case: without speed changing
		if (!drivingMotor_speedChanges) { 
			// decrement until the freq is INITIAL_FRQ
			if (DrivingMotor.AccelartionFreq > INITIAL_FREQ) {
				DrivingMotor_Step_SetFreqHz(DrivingMotor.AccelartionFreq -= DRIVINGMOTOR_ACCELARATIONINKREMENT);
			}
			// change the State when the black panther has finished his Steps or the Method ..._Brakes() is actived
			if ((drivingMotor_currentSteps >= ((drivingMotor_accelarationSteps * 2) + drivingMotor_cruisingSteps))
				 || ((DrivingMotor.AccelartionFreq <= INITIAL_FREQ) && drivingMotor_brakes)) {
				DrivingMotor.State = STOPPED;
				drivingMotor_stopped = TRUE;
			}
		} 
		// Speed-Change-Case: slower
		else{
			if(DrivingMotor.AccelartionFreq > DrivingMotor.TargetFreq){
				DrivingMotor_Step_SetFreqHz(DrivingMotor.AccelartionFreq -= DRIVINGMOTOR_ACCELARATIONINKREMENT);
			}
			if((drivingMotor_speedChangesSteps <= drivingMotor_currentSteps)){
				drivingMotor_speedChanges = 0; 
				DrivingMotor.State = CRUSING;
			}
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
			
			DrivingMotor_Step_Disable();
			//DrivingMotor_Driver_Disable(); //--> is not required (Testing)
			DrivingMotor.AccelartionFreq = INITIAL_FREQ;
			DrivingMotor.Speed = 0; 
			
		}
		break;
	}

	if (DrivingMotor.State != STOPPED){
		drivingMotor_currentSteps++;
	}
	
	/* Count the Steps */
	if (DrivingMotor.State != STOPPED && DrivingMotor.Direction) {
		DrivingMotor.Steps++;
	} else if (DrivingMotor.State != STOPPED && !DrivingMotor.Direction){
		DrivingMotor.Steps--; 
	}
}



uint8_t LiftingMotor_Move(int16_t stepsWithoutMicrosteppingRevolution) {
	uint32_t tmpSteps;
	LiftingMotor.TargetFreq = TARGET_FREQ_LIFTING_MOTOR(TARGET_VELOCITY_LIFTING_MOTOR);	
	LiftingMotor.Speed = TARGET_VELOCITY_LIFTING_MOTOR; 
	liftingMotor_currentSteps = 0; 
	liftingMotor_accelarationSteps = 0; 
	liftingMotor_cruisingSteps = 0; 
	
	if(stepsWithoutMicrosteppingRevolution < 0){
		LiftingMotor_BACKWARD(); 
		tmpSteps = (-1)*MICROSTEPPING_RESOLUTION_LIFTINGMOTOR * stepsWithoutMicrosteppingRevolution;
	} else {
		LiftingMotor_FORWARD(); 
		tmpSteps = MICROSTEPPING_RESOLUTION_LIFTINGMOTOR * stepsWithoutMicrosteppingRevolution;
	}

	if(tmpSteps <= 400*MICROSTEPPING_RESOLUTION_LIFTINGMOTOR){
		liftingMotor_cruisingSteps = 0; 
		liftingMotor_accelarationSteps = (tmpSteps/2);
	} else {
		liftingMotor_accelarationSteps = (LiftingMotor.TargetFreq-INITIAL_FREQ)/LIFTINGMOTOR_ACCELARATIONINKREMENT;
		liftingMotor_cruisingSteps = tmpSteps - (liftingMotor_accelarationSteps*2); 
	}
	
	// Invalid steps for move
	if (!LiftingMotor.Direction && (tmpSteps/MICROSTEPPING_RESOLUTION_LIFTINGMOTOR) > LiftingMotor.Steps){ 
			STRING_FAILURE(); 
			return ERR_FAILED; 
	}

	liftingMotor_stopped = FALSE;
	liftingMotor_finished = FALSE;
	LiftingMotor.State = ACCELARATING;
	LiftingMotor_Driver_Enable();
	LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq);
	LiftingMotor_Step_Enable();
	STRING_SUCCES(); 

	return ERR_OK;
}

uint8_t LiftingMotor_Brakes(void) {
	liftingMotor_brakes = TRUE;
	
	if(LiftingMotor.State != DECELERATING && LiftingMotor.State != STOPPED){
		LiftingMotor.State = DECELERATING; 
	} 

	STRING_SUCCES(); 
	
	return ERR_OK;
}

uint8_t LiftingMotor_MoveContinuous(void) {
	LiftingMotor_Move(10e3); 
	STRING_SUCCES(); 
	
	return ERR_OK; 
}

uint8_t LiftingMotor_SetSpeed(uint8_t targetVelocity){
	return ERR_OK; 
}

void LiftingMotor_Event(void) {

	switch (LiftingMotor.State) {
	case ACCELARATING:
		if ((LiftingMotor.AccelartionFreq < LiftingMotor.TargetFreq)) {
			LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq += LIFTINGMOTOR_ACCELARATIONINKREMENT);
		}

		if ((liftingMotor_currentSteps >= liftingMotor_accelarationSteps)) {
			LiftingMotor.State = CRUSING;
		}
		break;

	case CRUSING:
		if (liftingMotor_currentSteps >= liftingMotor_cruisingSteps + liftingMotor_accelarationSteps) {
			LiftingMotor.State = DECELERATING;
		}
		break;

	case DECELERATING:
		if (LiftingMotor.AccelartionFreq > INITIAL_FREQ) {
			LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq -= LIFTINGMOTOR_ACCELARATIONINKREMENT);
		}

		if (liftingMotor_currentSteps >= ((liftingMotor_accelarationSteps * 2) + liftingMotor_cruisingSteps)
			|| ((LiftingMotor.AccelartionFreq <= INITIAL_FREQ) && liftingMotor_brakes)) {
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
			liftingMotor_brakes = FALSE; 
			
			LiftingMotor_Step_Disable();
//			LiftingMotor_Driver_Disable(); //--> is not required
			LiftingMotor.AccelartionFreq = INITIAL_FREQ;
			LiftingMotor.Speed = 0; 
			
			
		}
		break;
	}
	if (LiftingMotor.State != STOPPED){
		liftingMotor_currentSteps++; 
	}
	
	/* Count steps */
	if(LiftingMotor.State != STOPPED && LiftingMotor.Direction){
		LiftingMotor.Steps++;  
	} else if(LiftingMotor.State != STOPPED && !LiftingMotor.Direction){
		LiftingMotor.Steps--; 
	}
}

uint8_t Stepper_ParseCommand(unsigned char* cmd){
	uint8_t result = ERR_OK;
	unsigned char tmpSteps[20];
	unsigned char tmpSpeed[20]; 
	uint16_t tmpValue;  

	/* For the control for Steppers with the Shell */
	if(strcmp(cmd, (unsigned char*)"ant status")==0){
		STRING_S();
		UTIL1_Num32uToStrFormatted(tmpSteps, sizeof(tmpSteps), DrivingMotor.Steps/8, ' ', 0);
		CLS1_SendStr(tmpSteps, CLS1_GetStdio()->stdOut);
		STRING_CONNECTOR(); 
		UTIL1_Num16uToStrFormatted(tmpSpeed, sizeof(tmpSpeed), DrivingMotor.Speed, ' ', 0);
		CLS1_SendStr(tmpSpeed, CLS1_GetStdio()->stdOut);
	} 
	
	else if (strncmp(cmd, (unsigned char*)"ant n", 5)==0){	
		 tmpValue = atoi(cmd+6); 
		 return DrivingMotor_Move(tmpValue);
	}
	
	else if (strcmp(cmd, "ant slow") == 0){
		return DrivingMotor_SetSpeed(TARGET_VELOCITY_DRIVING_MOTOR*0.5); 
	}
	
	else if (strcmp(cmd, "ant fast") == 0){
		return DrivingMotor_SetSpeed(TARGET_VELOCITY_DRIVING_MOTOR*1.5);
	}
	
	else if (strcmp(cmd, "ant stop") == 0) { 
		return DrivingMotor_Brakes(400); 
	}
	
	
	/* Lifting Motor */
	if (strcmp(cmd, (unsigned char*)"lst status") == 0) {
		CLS1_SendStr((unsigned char*)"s", CLS1_GetStdio()->stdOut);
		UTIL1_Num16uToStrFormatted(tmpSteps, sizeof(tmpSteps), LiftingMotor.Steps/8, ' ', 0);
		CLS1_SendStr(tmpSteps, CLS1_GetStdio()->stdOut);
		CLS1_SendStr((unsigned char*)";", CLS1_GetStdio()->stdOut);
		UTIL1_Num16uToStrFormatted(tmpSpeed, sizeof(tmpSpeed), LiftingMotor.Speed, ' ', 0);
		CLS1_SendStr(tmpSpeed, CLS1_GetStdio()->stdOut);
	} 
	
	else if (strncmp(cmd, "lst n", 5) == 0) {
		tmpValue = atoi(cmd+6); 
		return LiftingMotor_Move(tmpValue);
	}
	
	else if (strcmp(cmd, "lst fast") == 0) {
		;
	}
	
	else if (strcmp(cmd, "lst slow") == 0) {
		;
	}
	
	else if (strcmp(cmd, "lst stop") == 0) {
		return LiftingMotor_Brakes(); 
	}
	return result = ERR_FAILED;
}
