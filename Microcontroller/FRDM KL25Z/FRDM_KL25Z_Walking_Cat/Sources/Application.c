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
#define MAX_STEPS_FOR_DRIVINGMOTOR 5450
#define BRAKING_DISTANCE 400

/* Globale Variable */
Parcour_Handler Parcour_FSM_Handler;

/* Intern Variable */
static const uint16_t stepsAcrossLoadDrivingMotor = 654;
static uint16_t stepsForFirstDownLiftingMotor = 3190;
static uint32_t stepsForSecondDownLiftingMotor = 6000;

static void FSM_Parcour_Initalization(void){
	/* Initialization for the FSM handler */
	Parcour_FSM_Handler.ParcourState = Z0_INITIALIZATION;	
	Parcour_FSM_Handler.StartSignal_FromPI = 0;
	Parcour_FSM_Handler.ActualState_Finished = 0;
	Parcour_FSM_Handler.z0_initalizationFinished = 0;
	Parcour_FSM_Handler.z0_finished = 0;
	Parcour_FSM_Handler.z1_liftingMotorStarted = 0;
	Parcour_FSM_Handler.z1_drivingMotorStarted = 0;
	Parcour_FSM_Handler.z1_finished = 0;
	Parcour_FSM_Handler.z2_sendTargetdetection = 0;
	Parcour_FSM_Handler.z2_liftingMotorStarted = 0;
	Parcour_FSM_Handler.z3_drivingMotorStarted = 0;
	Parcour_FSM_Handler.z3_targetFound = 0;
	Parcour_FSM_Handler.z4_liftingMotorStarted = 0;
	Parcour_FSM_Handler.z5_drivingMotorStarted = 0;
	Parcour_FSM_Handler.z6_endSwitchIsClicked = 0;
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
//	if (EndSwitch_GetVal()) {
//		Parcour_FSM_Handler.ParcourState = Z6_THE_ENDSWITCH_IS_CLICKED;
//		CLS1_SendStr((unsigned char*) "parcour finished\n",
//							CLS1_GetStdio()->stdOut);
//	}
	
	switch (Parcour_FSM_Handler.ParcourState) {
	case Z0_INITIALIZATION:
		// just one time Initialization
		if (!Parcour_FSM_Handler.z0_initalizationFinished && !EndSwitch_GetVal()) {
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
		// 
		if (!Parcour_FSM_Handler.z1_drivingMotorStarted
				&& !Parcour_FSM_Handler.z2_liftingMotorStarted) {
			DrivingMotor_Move(stepsAcrossLoadDrivingMotor);
			LiftingMotor_Move(stepsForFirstDownLiftingMotor);
			Electromagnet_Driver_ON();
			Parcour_FSM_Handler.z1_drivingMotorStarted = 1;
			Parcour_FSM_Handler.z1_liftingMotorStarted = 1;
		}

		if (((DrivingMotor.Steps / 8) >= stepsAcrossLoadDrivingMotor)
				&& ((LiftingMotor.Steps / 8) >= stepsForFirstDownLiftingMotor)) {
			Parcour_FSM_Handler.ParcourState = Z2_LIFTING_UNTIL_FINISHED;
			Parcour_FSM_Handler.z1_finished = 1;
		}
		break;

	case Z2_LIFTING_UNTIL_FINISHED:
		if (!Parcour_FSM_Handler.z2_sendTargetdetection) {
			CLS1_SendStr((unsigned char*) "start detecting\n",
					CLS1_GetStdio()->stdOut);
			Parcour_FSM_Handler.z2_sendTargetdetection = 1;
		}

		if (!Parcour_FSM_Handler.z2_liftingMotorStarted) {
			LiftingMotor_Move(-stepsForFirstDownLiftingMotor);
			Parcour_FSM_Handler.z2_liftingMotorStarted = 1;

		}

		if ((LiftingMotor.Steps / 8) < stepsForFirstDownLiftingMotor - stepsForFirstDownLiftingMotor/3) {
			Parcour_FSM_Handler.ParcourState = Z3_CRUISING_UNTIL_ACROSS_TARGET;
		}

		break;

	case Z3_CRUISING_UNTIL_ACROSS_TARGET:
		if (!Parcour_FSM_Handler.z3_drivingMotorStarted) {
			DrivingMotor_Move(MAX_STEPS_FOR_DRIVINGMOTOR - (DrivingMotor.Steps/8));
			Parcour_FSM_Handler.z3_drivingMotorStarted = 1;
		}

		if (Parcour_FSM_Handler.z3_targetFound && LiftingMotor.Steps == 0) {
			stepsForSecondDownLiftingMotor = lockupTableForLiftingMotor_NumberOfStepsForLiftingDown[(DrivingMotor.Steps/8)-1];
			LiftingMotor_Move(stepsForSecondDownLiftingMotor);
			DrivingMotor_Brakes(BRAKING_DISTANCE);
			//DrivingMotor_Move(200);
			Parcour_FSM_Handler.ParcourState = Z4_LOWER_LOAD_UNTIL_FINISHED;
			Parcour_FSM_Handler.z3_targetFound = 0; 
		}
		break;

	case Z4_LOWER_LOAD_UNTIL_FINISHED:
		if ((LiftingMotor.Steps / 8) >= stepsForSecondDownLiftingMotor) {
			Electromagnet_Driver_OFF();
			LiftingMotor_Move(-stepsForSecondDownLiftingMotor);
			Parcour_FSM_Handler.ParcourState = Z5_CRUISING_UNTIL_MASTS2_IS_REACHED;
		}
		break;
	
	case Z5_CRUISING_UNTIL_MASTS2_IS_REACHED:
		if ((LiftingMotor.Steps / 8) <= (stepsForSecondDownLiftingMotor - stepsForSecondDownLiftingMotor/3) && !Parcour_FSM_Handler.z5_drivingMotorStarted) {
			DrivingMotor_Move(MAX_STEPS_FOR_DRIVINGMOTOR - (DrivingMotor.Steps/8));
			Parcour_FSM_Handler.z5_drivingMotorStarted = 1;
			Parcour_FSM_Handler.ParcourState = Z6_UNTIL_THE_ENDSWITCH_IS_CLICKED;
		}
		break;
		
	case Z6_UNTIL_THE_ENDSWITCH_IS_CLICKED: 
		if (Parcour_FSM_Handler.z6_endSwitchIsClicked || DrivingMotor.Steps >= MAX_STEPS_FOR_DRIVINGMOTOR) {
			DrivingMotor_Step_Disable();
			LiftingMotor_Step_Disable();
			Parcour_FSM_Handler.z6_endSwitchIsClicked = 0; 
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

	for (;;) {
		LED_Onboard_Green_Neg();
		FSM_Parcour();
		FRTOS1_vTaskDelay(500/portTICK_RATE_MS);
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
