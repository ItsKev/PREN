/*
 * Application.h
 *
 *  Created on: Mar 5, 2018
 *      Author: Burak Kizilkaya, Team 33
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "PE_Types.h"

typedef enum {
	Z0_INITIALIZATION, 
	Z1_CRUISING_UNTIL_ACROSS_LOAD,
	Z2_LIFTING_IS_FINISHED,
	Z3_CRUISING_UNTIL_ACROSS_TARGET,
	Z4_LOWER_LOAD_IS_FINISHED,
	Z5_CRUISING_UNTIL_MASTS2_IS_REACHED
}Parcour_FSM_State;

typedef struct{
	Parcour_FSM_State ParcourState;
	bool StartSignal_FromPI;
	bool ActualState_Finished;
	bool z0_initalizationFinished;
	bool z0_finished;
	bool z1_liftingMotorStarted;
	bool z1_drivingMotorStarted;
	bool z1_finished;
}Parcour_Handler;

extern Parcour_Handler ParcourState;

void APP_Run(void);

#endif /* APPLICATION_H_ */
