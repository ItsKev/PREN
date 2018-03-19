/*
 * InfraredTOF.c
 *
 *  Created on: Mar 10, 2018
 *      Author: Burak Kizilkaya
 */

#include "InfraredTOF.h"
#include "InfraredTOF_I2C.h"
#include "GI2C1.h"
#include "PE_Error.h"
#include "LED_Onboard_Green.h"
#include "CLS1.h"

InfraredTOF_Device VL53L0X_DeviceFront;

/* Methoden */

/* static methods */
static uint8_t readRangeContinuous(uint8_t i2cDeviceAddress, uint8_t *valP) {
	uint8_t range;
	uint8_t res, val;
	uint16_t timeoutMs = 100;

	*valP = 0; /* init */
	do { /* breaks */
		res = VL_ReadReg8(i2cDeviceAddress, RESULT_INTERRUPT_STATUS, &val);
		if (res != ERR_OK) {
			break;
		}
		if ((val & 0x4) != 0) {
			break; /* 4: New Sample Ready threshold event */
		}
		if (timeoutMs == 0) { /* timeout */
			break;
		}
		WAIT1_WaitOSms(1);
		timeoutMs--;
	} while (1);
	if (timeoutMs == 0) {
		return ERR_NOTAVAIL; /* timeout */
	}

	res = VL_ReadReg8(i2cDeviceAddress, RESULT_RANGE_STATUS, &range); /* read range in millimeters */
	if (res != ERR_OK) {
		return res;
	}
	res = VL_WriteReg8(i2cDeviceAddress, SYSTEM_INTERRUPT_CLEAR, 0x01); /* clear interrupt flag */
	if (res != ERR_OK) {
		return res;
	}
	*valP = range;
	return ERR_OK;
}

uint8_t VL_ReadRangeSingle(uint8_t i2cDeviceAddress, uint16_t *rangeP) {
	uint8_t res;
	uint8_t val;

	res = VL_WriteReg8(i2cDeviceAddress, SYSRANGE_START, 0x01);
	//WAIT1_WaitOSms(8); /* logic analyzer shows it takes around 9 ms until the data is ready */
	res = readRangeContinuous(i2cDeviceAddress, &val);
	if (res != ERR_OK) {
		*rangeP = -1;
		return res; /* error */
	}
	if (val >= 0 && val <= 255) {
		*rangeP = val; /* store value */
		return ERR_OK;
	}
	*rangeP = -2; /* error */
	return ERR_FAILED;
}

uint8_t VL_WriteReg8(uint8_t i2cDeviceAddress, uint16_t reg, uint8_t val) {
	uint8_t r[2];

	r[0] = reg >> 8;
	r[1] = reg & 0xff;
	return GI2C1_WriteAddress(i2cDeviceAddress, &r[0], sizeof(r), &val,
			sizeof(val));
}

uint8_t VL_WriteReg16(uint8_t i2cDeviceAddress, uint16_t reg, uint16_t val) {
	uint8_t r[2], v[2];

	r[0] = reg >> 8;
	r[1] = reg & 0xff;
	v[0] = val >> 8;
	v[1] = val & 0xff;
	return GI2C1_WriteAddress(i2cDeviceAddress, &r[0], sizeof(r), &v[0],
			sizeof(v));
}

uint8_t VL_ReadReg8(uint8_t i2cDeviceAddress, uint16_t reg, uint8_t *valP) {
	uint8_t tmp[2];

	tmp[0] = reg >> 8;
	tmp[1] = reg & 0xff;
	return GI2C1_ReadAddress(i2cDeviceAddress, &tmp[0], sizeof(tmp), valP, 1);
}

uint8_t VL_ReadReg16(uint8_t i2cDeviceAddress, uint16_t reg, uint16_t *valP) {
	uint8_t tmp[2];

	tmp[0] = reg >> 8;
	tmp[1] = reg & 0xff;
	return GI2C1_ReadAddress(i2cDeviceAddress, &tmp[0], sizeof(tmp),
			(uint8_t*) valP, 2);
}



uint8_t VL_ParseCommand(unsigned char* cmd) {
	uint8_t result = ERR_OK;
	unsigned char tmpDistance[20];

	/*For the control from US_Device with the Shell*/
	if (strcmp(cmd, "INF TOF status") == 0) {
		CLS1_SendStr((unsigned char*) "INF Device front\n", CLS1_GetStdio()->stdOut);
		CLS1_SendStr((unsigned char*) "\t value:\t", CLS1_GetStdio()->stdOut);
		UTIL1_Num16uToStrFormatted(tmpDistance, sizeof(tmpDistance), VL53L0X_DeviceFront.distance, ' ', 10);
		CLS1_SendStr(tmpDistance, CLS1_GetStdio()->stdOut);
		CLS1_SendStr((unsigned char*) " mm\n", CLS1_GetStdio()->stdOut);

		return result;
	}

	return result = ERR_FAILED;

}

uint8_t VL_Init(void){
	uint8_t result = ERR_OK; 
	
	VL53L0X_DeviceFront.address = VL53L0X_DEFAULT_I2C_ADDRESS;
	VL53L0X_DeviceFront.distance = 0; 
	VL53L0X_DeviceFront.xshut = 0; 
	
	/* Set I2C standard mode */
	result = VL_WriteReg8(VL53L0X_DeviceFront.address, 0x88, 0x00); 
	
	
}

void InfraredTOF_Run(void) {
	uint8_t res;

	for (;;) {

	}
}


uint8_t VL_Test(uint8_t i2cDeviceAddress) {
	uint8_t result = ERR_OK;
	uint8_t val;

	/* test: check if the device is already configured */
	result = VL_ReadReg8(i2cDeviceAddress, I2C_SLAVE_DEVICE_ADDRESS, &val);
	if (val == i2cDeviceAddress) {
		/* already configured :-) */
		CLS1_SendStr((unsigned char*) "Test was successful :)\n", CLS1_GetStdio()->stdOut);
		
		/* test: check if can the device can send a measure */
		result = VL_ReadRangeSingle(VL53L0X_DeviceFront.address, &VL53L0X_DeviceFront.distance); 
		
		return result;
	}
	return result = ERR_FAILED; 

}
