/*
 * MeasurmentHandler.c
 *
 *  Created on: May 3, 2018
 *      Author: burak
 */

#include "MeasurmentHandler.h"
#include "Stepper.h"
#include "Lockup_Table.h"
#include "PE_Types.h"
#include "PE_Const.h"

static double calculationDistance_YAxis(uint32_t steps){
	// Function for the different Distance
	return 0.0f;
}

float Measurment_XAxis(uint32_t steps){
	return lockupTableForMeasurment_XAxisAir[steps];
}

float Measurment_YAxis(uint32_t steps){
	return lockupTableForMeasurment_YAxis[steps] - calculationDistance_YAxis(LiftingMotor.Steps);
}

void MeasurmentInit(void){
	;
}

