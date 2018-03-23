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
#define DrivingMotor_FORWARD() 			({DrivingMotor.Direction = FORWARD; DrivingMotor_Direction_SetVal();})
#define DrivingMotor_BACKWARD() 		({DrivingMotor.Direction = BACKWARD; DrivingMotor_Direction_ClrVal();})
#define DrivingMotor_Driver_Enable() 	({DrivingMotor.Driver_Enable = TRUE; DrivingMotor_Enable_ClrVal();})
#define DrivingMotor_Driver_Disable() 	({DrivingMotor.Driver_Enable = FALSE; DrivingMotor_Enable_SetVal();})
#define DrivingMotor_MS1_On()  			({DrivingMotor.MS1 = FALSE; DrivingMotor_MS1_ClrVal();})

#define LiftingMotor_FORWARD()			({LiftingMotor.Direction = FORWARD; LiftingMotor_Direction_ClrVal();})
#define LiftingMotor_BACKWARD()			({LiftingMotor.Direction = BACKWARD; LiftingMotor_Direction_SetVal();})
#define LiftingMotor_Driver_Enable()	({LiftingMotor.Driver_Enable = TRUE; LiftingMotor_Enable_ClrVal();})
#define LiftingMotor_Driver_Disable()	({LiftingMotor.Driver_Enable = FALSE; LiftingMotor_Enable_SetVal();})

// Constants
#define PI 								3.141592653589793238462643383
#define RADIUS_DRIVING_WHEEL 			2 // cm
#define RADIUS_Lifting_WHEEL 			0.5 // cm
#define TARGET_VELOCITY_DRIVING_MOTOR 	10 // [cm/s]
#define TARGET_VELOCITY_LIFTING_MOTOR 	15 // [cm/s]
#define SCOPE_DRIVING_WHEEL 			2 // 2 cm
#define SCOPE_LIFTING_WHEEL 			2 // 2 cm
#define MICROSTEPPING_REVOLUTION		16 // Fullstep(1), Halfstep(2), Quarterstep(4), Eighterstep(8), Sixteenthstep(16)
#define INITIAL_FREQ 					400 // Hz
// Calculates
#define TARGET_FREQ_DRIVING_MOTOR 		((MICROSTEPPING_REVOLUTION*100*TARGET_VELOCITY_DRIVING_MOTOR)/(RADIUS_DRIVING_WHEEL*PI)) // (100*v)/(r*pi) 
#define TARGET_FREQ_LIFTING_MOTOR 		((MICROSTEPPING_REVOLUTION*100*TARGET_VELOCITY_LIFTING_MOTOR)/(RADIUS_Lifting_WHEEL*PI)) // (100*v)/(r*pi)
//--------------------------------------
/* Variables */
//--------------------------------------
// Global
StepperDevice DrivingMotor;
StepperDevice LiftingMotor;

// Just in this file
static uint32_t liftingMotor_accelarationSteps = 0;
static uint32_t liftingMotor_cruisingSteps = 0;
static uint32_t drivingMotor_accelarationSteps = 0;
static uint32_t drivingMotor_cruisungSteps = 0;
static uint8_t liftingMotor_accelarationInkrement = 1;

static bool liftingMotor_continuous;
static bool liftingMotor_stopped;
static bool liftingMotor_brakes;
static bool liftingMotor_finished; 


static bool drivingMotor_continuous;
static bool drivingMotor_stopped;
static bool drivingMotor_brakes;

static const uint8_t LUP_StepperMotor_TargetFreq[] = { 
		400, 500, 600, 700, 800,
		900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000,
		2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200,
		3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400,
		4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600,
		5700, 5800, 5900, 6000, 6100, 6200, 6300, 6400, 6500, 6600, 6700, 6800,
		6900, 7000, 7100, 7200, 7300, 7400, 7500, 7600, 7700, 7800, 7900, 8000,
		8100, 8200, 8300, 8400, 8500, 8600, 8700, 8800, 8900, 9000, 9100, 9200,
		9300, 9400, 9500, 9600, 9700, 9800, 9900, 10000, 10100, 10200, 10300,
		10400, 10500, 10600, 10700, 10800, 10900, 11000, 11100, 11200, 11300,
		11400, 11500, 11600, 11700, 11800, 11900, 12000, 12100, 12200, 12300,
		12400, 12500, 12600, 12700, 12800, 12900, 13000, 13100, 13200, 13300,
		13400, 13500, 13600, 13700, 13800, 13900, 14000, 14100, 14200, 14300,
		14400, 14500, 14600, 14700, 14800, 14900, 15000 
};

//--------------------------------------
/* Methods */
//--------------------------------------
// Static
static uint8_t DrivingMotor_Brakes(void) {
	drivingMotor_brakes = TRUE;

	return ERR_OK;
}

static uint8_t DrivingMotor_MoveContinuous(void) {
	/* Not implemented yet */
}

static uint8_t LiftingMotor_Brakes(void) {
	liftingMotor_brakes = TRUE;

	return ERR_OK;
}

static uint8_t LiftingMotor_MoveContinuous(void) {
	/* Not implemented yet */
}

// Public (Interface)
/**
 * 
 */
uint8_t StepperInit(void) {
	// Driving Motor
	DrivingMotor_Step_Disable(); // No pulse is generated
	DrivingMotor_FORWARD();
	DrivingMotor_Driver_Disable(); // Motor driver is off	

	DrivingMotor.Speed = TARGET_VELOCITY_DRIVING_MOTOR;
	DrivingMotor.TargetFreq = TARGET_FREQ_DRIVING_MOTOR;
	DrivingMotor.AccelartionFreq = INITIAL_FREQ;
	DrivingMotor.Steps = 0;
	DrivingMotor.State = STOPPED;
	drivingMotor_stopped = FALSE;
	drivingMotor_continuous = FALSE;
	drivingMotor_brakes = FALSE;

	// Lifting Motor
	LiftingMotor_Step_Disable(); // No pulse is generated
	LiftingMotor_FORWARD();
	LiftingMotor_Driver_Disable(); // Motor driver is off

	LiftingMotor.Speed = TARGET_VELOCITY_LIFTING_MOTOR;
	LiftingMotor.TargetFreq = TARGET_FREQ_LIFTING_MOTOR;
	LiftingMotor.AccelartionFreq = INITIAL_FREQ;
	LiftingMotor.Steps = 0;
	LiftingMotor.State = STOPPED;
	liftingMotor_stopped = FALSE;
	liftingMotor_continuous = FALSE;
	liftingMotor_accelarationInkrement = 1;
	liftingMotor_finished = FALSE; 

	return ERR_OK;
}

void Stepper_TestApplication(void) {
	//LiftingMotor_BACKWARD(); 
	int i; 
	
	LiftingMotor_Move(3e3); 
//	DrivingMotor_Step_Enable(); 
//	LiftingMotor_Driver_Enable(); 
//	for(i=400; i<10e3; i++){
//		DrivingMotor_Step_SetFreqHz(i); 
//	}

	Electromagnet_SetVal(); 
	
	for (;;) {
	}
	
	for(;;){
		
	}
}

uint8_t DrivingMotorMove(uint32_t stepsWithoutMicrosteppingRevolution) {
	uint32_t tmpSteps = MICROSTEPPING_REVOLUTION * stepsWithoutMicrosteppingRevolution;
	drivingMotor_accelarationSteps = (double) 0.2 * tmpSteps;
	drivingMotor_cruisungSteps = tmpSteps - 2 * drivingMotor_accelarationSteps;

	DrivingMotor.State = ACCELARATING;
	DrivingMotor_Driver_Enable(); // Motor driver is ON.
	DrivingMotor_Step_SetFreqHz(DrivingMotor.AccelartionFreq);
	DrivingMotor_Step_Enable();
	return ERR_OK;
}

uint8_t LiftingMotor_Move(uint32_t stepsWithoutMicrosteppingRevolution) {
	uint32_t tmpSteps = MICROSTEPPING_REVOLUTION * stepsWithoutMicrosteppingRevolution;

	if(stepsWithoutMicrosteppingRevolution <= 200){
		liftingMotor_cruisingSteps = 0; 
		liftingMotor_accelarationSteps = (tmpSteps/2);
	} else if((stepsWithoutMicrosteppingRevolution > 200) && (stepsWithoutMicrosteppingRevolution <= 2000)){
		liftingMotor_accelarationSteps = 200*MICROSTEPPING_REVOLUTION;
		liftingMotor_cruisingSteps = tmpSteps - (liftingMotor_accelarationSteps<<2); 
	} else if(stepsWithoutMicrosteppingRevolution > 2000){
		liftingMotor_accelarationSteps = 400*MICROSTEPPING_REVOLUTION;
		liftingMotor_cruisingSteps = tmpSteps - (liftingMotor_accelarationSteps*2);
	}

	liftingMotor_stopped = FALSE;
	LiftingMotor.State = ACCELARATING;
	LiftingMotor_Driver_Enable();
	LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq);
	LiftingMotor_Step_Enable();

	return ERR_OK;
}

void DrivingMotor_Event(void) {
	/* Not implemented yet*/
}

void LiftingMotor_Event(void) {

	switch (LiftingMotor.State) {
	case ACCELARATING:
		if ((LiftingMotor.AccelartionFreq < LiftingMotor.TargetFreq)) {
			LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq++);
		}

		if ((LiftingMotor.AccelartionFreq >= LiftingMotor.TargetFreq)) {
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
			LiftingMotor_Step_SetFreqHz(LiftingMotor.AccelartionFreq--);
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
			liftingMotor_stopped = TRUE;
			liftingMotor_finished = TRUE; 
			liftingMotor_accelarationSteps = 0; 
			liftingMotor_cruisingSteps = 0; 
			LiftingMotor_Step_Disable();
			//LiftingMotor_Driver_Disable(); //--> is not required
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

