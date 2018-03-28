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



#define DrivingMotor_FORWARD() 	({DrivingMotor.Direction = FORWARD; DrivingMotor_Direction_ClrVal();})

#define DrivingMotor_BACKWARD() ({DrivingMotor.Direction = BACKWARD; DrivingMotor_Direction_SetVal();})

uint8_t DrivingMotor_Move(int16_t stepsWithoutMicrosteppingRevolution); 

uint8_t DrivingMotor_MoveContinuous(void); 

uint8_t DrivingMotor_Brakes(void); 

void DrivingMotor_Event(void); 



#define LiftingMotor_FORWARD()	({LiftingMotor.Direction = FORWARD; LiftingMotor_Direction_ClrVal();})

#define LiftingMotor_BACKWARD()	({LiftingMotor.Direction = BACKWARD; LiftingMotor_Direction_SetVal();})

uint8_t LiftingMotor_Move(int16_t stepsWithoutMicrosteppingRevolution);

uint8_t LiftingMotor_MoveContinuous(void); 

void LiftingMotor_Event(void);



uint8_t Stepper_Init(void); 

void Stepper_TestApplication(void); 

uint8_t Stepper_ParseCommand(unsigned char* cmd); 




#endif /* STEPPER_H_ */
