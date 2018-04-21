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
#include "Sensoric.h"
#include "Stepper.h"
#include "Electromagnet_Driver.h"

/* Defines */
#define STEPS_ACROSS_LOAD_DrivingMotor				1000
#define STEPS_FOR_LIFTING_THE_LOAD_LiftingMotor		1000
#define STEPS_FOR_BEGIN_LiftingMotor				1000

/* Globale Variable */
Parcour_Handler Parcour_FSM_Handler;


static void Initalization(void){
	;
}

static void FSM_Parcour(void){
	
	switch (Parcour_FSM_Handler.ParcourState) {
	case Z0_INITIALIZATION:
		if (!Parcour_FSM_Handler.z0_initalizationFinished) {
			Initalization();
			Parcour_FSM_Handler.z0_initalizationFinished = 1;
		}
		
		if ((Parcour_FSM_Handler.StartSignal_FromPI) && !Parcour_FSM_Handler.z0_finished) {
			Parcour_FSM_Handler.ParcourState = Z1_CRUISING_UNTIL_ACROSS_LOAD;
			Parcour_FSM_Handler.z0_finished = 1;
		}
		break;

	case Z1_CRUISING_UNTIL_ACROSS_LOAD:
		if(!Parcour_FSM_Handler.z1_drivingMotorStarted){
			DrivingMotor_Move(STEPS_ACROSS_LOAD_DrivingMotor);
			Parcour_FSM_Handler.z1_drivingMotorStarted = 1;
		}
		
		if(DrivingMotor.Steps == STEPS_FOR_BEGIN_LiftingMotor && !Parcour_FSM_Handler.z1_liftingMotorStarted){
			LiftingMotor_Move(STEPS_FOR_LIFTING_THE_LOAD_LiftingMotor);
			Parcour_FSM_Handler.z1_liftingMotorStarted = 1;
		}
		
		if ((DrivingMotor.Steps == STEPS_ACROSS_LOAD_DrivingMotor) && (LiftingMotor.Steps == STEPS_FOR_LIFTING_THE_LOAD_LiftingMotor)){
			Parcour_FSM_Handler.ParcourState = Z2_LIFTING_IS_FINISHED;
			Parcour_FSM_Handler.z1_finished = 1;
		}
		break;

	case Z2_LIFTING_IS_FINISHED:
		
		break;

	case Z3_CRUISING_UNTIL_ACROSS_TARGET:
		break;

	case Z4_LOWER_LOAD_IS_FINISHED:
		break;

	case Z5_CRUISING_UNTIL_MASTS2_IS_REACHED:
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
		FRTOS1_vTaskDelay(1000/portTICK_RATE_MS);
	}
}

void APP_Run(void) {
	Shell_Init();
	Sensoric_Init();
	Stepper_Init(); 
	Electromagnet_Driver_Init();
	
	/* Initialization for the FSM handler */
	Parcour_FSM_Handler.ActualState_Finished = 0; 
	Parcour_FSM_Handler.ParcourState = Z0_INITIALIZATION;
	Parcour_FSM_Handler.StartSignal_FromPI = 0;
	Parcour_FSM_Handler.z0_initalizationFinished = 0;
	Parcour_FSM_Handler.z1_liftingMotorStarted = 0;

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
