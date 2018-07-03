/*
 * Application.c
 *
 *  Created on: Mar 5, 2018
 *      Author: burak
 *      
 *      Hinweise: 
 *      	Target 
 */

#include "Application.h"
#include "LED_Onboard_R.h"
#include "LED_Onboard_Green.h"
#include "FRTOS1.h"
#include "Shell.h"
#include "Stepper.h"
#include "Electromagnet_Driver.h"
#include "PE_Types.h"
#include "Lockup_Table.h"
#include "EndSwitch.h"
#include "MeasurmentHandler.h"

/* Defines */
#define MAX_STEPS_FOR_DRIVINGMOTOR  5500//7000//5470
#define BRAKING_DISTANCE 170

// Geschwindigkeit
#define SAFTY_LIFTINGMOTOR_SPEED 			10 // [cm/s]
#define SAFTY_LIFTINGMOTOR_ACCELARATION		5 // It's variable --> Testing
#define SAFTY_DRIVINGMOTOR_SPEED 			25 // [cm/s]
#define SAFTY_DRIVINGMOTOR_ACCELARATION		5 // It's variable --> Testing

#define LIFTINGMOTOR_SPEED_DOWN1				50
#define LIFTINGMOTOR_SPEED_UP1					70
#define LIFTINGMOTOR_SPEED_DOWN2				70
#define LIFTINGMOTOR_SPEED_UP2					70
#define LIFTINGMOTOR_ACCELARATION_DOWN1			2
#define LIFTINGMOTOR_ACCELARTAION_UP1			10
#define LIFTINGMOTOR_ACCELARATION_DOWN2			7
#define LIFTINGMOTOR_ACCELARTAION_UP2			30

#define DRIVINGMOTOR_SPEED_FIRSTSTAGE			50
#define DRIVINGMOTOR_SPEED_SECONDSTAGE			40
#define DRIVINGMOTOR_SPEED_THIRDSTAGE			62
#define DRIVINGMOTOR_ACCELARATION_FIRSTSTAGE	4
#define DRIVINGMOTOR_ACCELARATION_SECONDSTAGE	3//3//5
#define DRIVINGMOTOR_ACCELARATION_THIRDSTAGE	2

/* Globale Variable */
Parcour_Handler Parcour_FSM_Handler;

/* Intern Variable */
static const uint16_t stepsAcrossLoadDrivingMotor = 684;
static uint16_t stepsForFirstDownLiftingMotor = 3300;
static uint32_t stepsForSecondDownLiftingMotor = 6000;

static void FSM_Parcour_Initalization(void){
	/* Initialization for the FSM handler */
	Parcour_FSM_Handler.ParcourState = Z0_INITIALIZATION;	
	Parcour_FSM_Handler.StartSignal_FromPI = 0;
	Parcour_FSM_Handler.ActualState_Finished = 0;
	Parcour_FSM_Handler.parcourFinished = 0;
	Parcour_FSM_Handler.z0_initalizationFinished = 0;
	Parcour_FSM_Handler.z0_finished = 0;
	Parcour_FSM_Handler.z1_liftingMotorStarted = 0;
	Parcour_FSM_Handler.z1_drivingMotorStarted = 0;
	Parcour_FSM_Handler.z1_finished = 0;
	Parcour_FSM_Handler.z2_sendTargetdetection = 0;
	Parcour_FSM_Handler.z2_liftingMotorStarted = 0;
	Parcour_FSM_Handler.z3_drivingMotorStarted = 0;
	Parcour_FSM_Handler.z3_targetFound = 0;
	Parcour_FSM_Handler.z3_liftingMotorStopped = 0;
	Parcour_FSM_Handler.z4_liftingMotorStarted = 0;
	Parcour_FSM_Handler.z4_liftingMotorOldSteps = 0;
	Parcour_FSM_Handler.z5_drivingMotorStarted = 0;
	Parcour_FSM_Handler.z6_endSwitchIsClicked = 0;
	Parcour_FSM_Handler.z4_liftingIsFinished = 0;
}


static void Initalization(void) {
	FSM_Parcour_Initalization(); 
	MeasurmentInit();
	Stepper_Init();
	Electromagnet_Driver_Init();
}

static void FSM_Parcour(void) {
	/**
	 * Is the End-Switch Clicked
	 */
	if (!EndSwitch_GetVal() && !Parcour_FSM_Handler.z6_endSwitchIsClicked) {
		Parcour_FSM_Handler.ParcourState = Z6_UNTIL_THE_ENDSWITCH_IS_CLICKED;
		CLS1_SendStr((unsigned char*) "parcour finished\n",CLS1_GetStdio()->stdOut);
		Parcour_FSM_Handler.z6_endSwitchIsClicked = 1;
	}
	
	switch (Parcour_FSM_Handler.ParcourState) {
	case Z0_INITIALIZATION:
		// just one time Initialization
		if (!Parcour_FSM_Handler.z0_initalizationFinished && !Parcour_FSM_Handler.z6_endSwitchIsClicked) {
			Initalization();
			Parcour_FSM_Handler.z0_initalizationFinished = 1;
		}
		// The Start button is clicked
		if (Parcour_FSM_Handler.StartSignal_FromPI && !Parcour_FSM_Handler.z0_finished) {
			Parcour_FSM_Handler.ParcourState = Z1_CRUISING_UNTIL_ACROSS_LOAD;
			Parcour_FSM_Handler.z0_finished = 1;
		}
		break;

	case Z1_CRUISING_UNTIL_ACROSS_LOAD:
		// Driving and Lifting Motor started in the same time
		if (!Parcour_FSM_Handler.z1_drivingMotorStarted
				&& !Parcour_FSM_Handler.z2_liftingMotorStarted) {
			DrivingMotor_Move(stepsAcrossLoadDrivingMotor, DRIVINGMOTOR_SPEED_FIRSTSTAGE, DRIVINGMOTOR_ACCELARATION_FIRSTSTAGE);
			LiftingMotor_Move(stepsForFirstDownLiftingMotor, LIFTINGMOTOR_SPEED_DOWN1, LIFTINGMOTOR_ACCELARATION_DOWN1);
			Electromagnet_Driver_ON();
			Parcour_FSM_Handler.z1_drivingMotorStarted = 1;
			Parcour_FSM_Handler.z1_liftingMotorStarted = 1;
		}
		// Switching in the next state is available, when the grab across the load is
		if (((DrivingMotor.Steps / 8) >= stepsAcrossLoadDrivingMotor)
				&& ((LiftingMotor.Steps / 8) >= stepsForFirstDownLiftingMotor)) {
			Parcour_FSM_Handler.ParcourState = Z2_LIFTING_UNTIL_FINISHED;
			Parcour_FSM_Handler.z1_finished = 1;
		}
		break;

	case Z2_LIFTING_UNTIL_FINISHED:
		// Raspi started the target detection
		if (!Parcour_FSM_Handler.z2_sendTargetdetection) {
			CLS1_SendStr((unsigned char*) "start detecting\n",
					CLS1_GetStdio()->stdOut);
			Parcour_FSM_Handler.z2_sendTargetdetection = 1;
		}
		// The Lifting Motor started reverse
		if (!Parcour_FSM_Handler.z2_liftingMotorStarted) {
			LiftingMotor_Move(-(stepsForFirstDownLiftingMotor), LIFTINGMOTOR_SPEED_UP1, LIFTINGMOTOR_ACCELARTAION_UP1);
			Parcour_FSM_Handler.z2_liftingMotorStarted = 1;
		}
		// 
		if ((LiftingMotor.Steps / 8) <= (stepsForFirstDownLiftingMotor - stepsForFirstDownLiftingMotor/3)) {
			Parcour_FSM_Handler.ParcourState = Z3_CRUISING_UNTIL_ACROSS_TARGET;
		}

		break;

	case Z3_CRUISING_UNTIL_ACROSS_TARGET:
		if (!Parcour_FSM_Handler.z3_drivingMotorStarted) {
			DrivingMotor_Move(MAX_STEPS_FOR_DRIVINGMOTOR - (DrivingMotor.Steps/8), DRIVINGMOTOR_SPEED_SECONDSTAGE, DRIVINGMOTOR_ACCELARATION_SECONDSTAGE);
			Parcour_FSM_Handler.z3_drivingMotorStarted = 1;
		}

		if (Parcour_FSM_Handler.z3_targetFound) {
			LiftingMotor.State = STOPPED;
		}
		
		if(!Parcour_FSM_Handler.z3_liftingMotorStopped && LiftingMotor.Speed == 0 && Parcour_FSM_Handler.z3_targetFound){
			Parcour_FSM_Handler.z3_targetFound = 0; 
			Parcour_FSM_Handler.z3_liftingMotorStopped = 1;
			Parcour_FSM_Handler.z4_liftingMotorOldSteps = LiftingMotor.Steps/8;
			stepsForSecondDownLiftingMotor = lockupTableForLiftingMotor_NumberOfStepsForLiftingDown[(DrivingMotor.Steps/8)-1] - LiftingMotor.Steps/8 + 300;
			LiftingMotor_Move(stepsForSecondDownLiftingMotor, LIFTINGMOTOR_SPEED_DOWN2, LIFTINGMOTOR_ACCELARATION_DOWN2);
			DrivingMotor_Brakes(BRAKING_DISTANCE);
			Parcour_FSM_Handler.ParcourState = Z4_LOWER_LOAD_UNTIL_FINISHED;
		}
		break;

	case Z4_LOWER_LOAD_UNTIL_FINISHED:
		if ((LiftingMotor.Steps / 8) >= Parcour_FSM_Handler.z4_liftingMotorOldSteps + stepsForSecondDownLiftingMotor) {
			Electromagnet_Driver_OFF();
			LiftingMotor_Move(-stepsForSecondDownLiftingMotor, LIFTINGMOTOR_SPEED_UP2, LIFTINGMOTOR_ACCELARTAION_UP2); 
			CLS1_SendStr((unsigned char*) "measurement finished\n",
								CLS1_GetStdio()->stdOut);
			
			Parcour_FSM_Handler.ParcourState = Z5_CRUISING_UNTIL_MASTS2_IS_REACHED;
		}
		break;
	
	case Z5_CRUISING_UNTIL_MASTS2_IS_REACHED:
		if ((LiftingMotor.Steps / 8) <= (stepsForSecondDownLiftingMotor - 300) && !Parcour_FSM_Handler.z5_drivingMotorStarted) {
			DrivingMotor_Move(MAX_STEPS_FOR_DRIVINGMOTOR - (DrivingMotor.Steps/8), DRIVINGMOTOR_SPEED_THIRDSTAGE, DRIVINGMOTOR_ACCELARATION_THIRDSTAGE);
			Parcour_FSM_Handler.z5_drivingMotorStarted = 1;
			Parcour_FSM_Handler.ParcourState = Z6_UNTIL_THE_ENDSWITCH_IS_CLICKED;
		}
		break;
		
	case Z6_UNTIL_THE_ENDSWITCH_IS_CLICKED: 
		if ((Parcour_FSM_Handler.z6_endSwitchIsClicked || DrivingMotor.Steps/8 >= MAX_STEPS_FOR_DRIVINGMOTOR)) {
			//DrivingMotor_Step_Disable();
			//LiftingMotor_Step_Disable();
			Parcour_FSM_Handler.z6_endSwitchIsClicked = 0;
			
			DrivingMotor.State = STOPPED; 
			LiftingMotor.State = STOPPED;
			DrivingMotor_Step_Disable();
			//DrivingMotor_Move(-(DrivingMotor.Steps/8 - 600), SAFTY_DRIVINGMOTOR_SPEED, SAFTY_DRIVINGMOTOR_ACCELARATION); 
			LiftingMotor_Move(-(LiftingMotor.Steps/8), SAFTY_LIFTINGMOTOR_SPEED, SAFTY_LIFTINGMOTOR_ACCELARATION);
			
		}
		
		if (LiftingMotor.Steps/8 <= 0 && !Parcour_FSM_Handler.parcourFinished && DrivingMotor.Steps/8 <= 600) {
			Parcour_FSM_Handler.parcourFinished = 1; 
			Parcour_FSM_Handler.z0_initalizationFinished = 0; // Initalization can always start for reset
			Parcour_FSM_Handler.ParcourState = Z0_INITIALIZATION;
		}
		break;

	default: // Nothing
		break;
	}
}

static portTASK_FUNCTION(Task1, pvParameters) {
	(void) pvParameters; /* parameter not used */

	Stepper_Init();
	for (;;) {		
		LED_Onboard_Green_Neg();
		FSM_Parcour();
		//FRTOS1_vTaskDelay(10/portTICK_RATE_MS);
	}
}

void APP_Run(void) {
	Shell_Init();	

	if (FRTOS1_xTaskCreate(
			Task1, /* pointer to the task */
			"Task1", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS ) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	FRTOS1_vTaskStartScheduler();
}
