/*
 * InfraredTOF.h
 *
 *  Created on: Mar 10, 2018
 *      Author: burak
 */

#ifndef INFRAREDTOF_H_
#define INFRAREDTOF_H_

#include "PE_Types.h"
#include "PE_LDD.h"

#define VL5310X_DEFAULT_I2C_ADDRESS 0x29

typedef struct{
	uint8_t address; 
	uint16_t distance;
	bool xshut; /* True or false */
}InfraredTOF_device;

uint8_t VL_WriteReg8(uint8_t i2cDeviceAddress, uint16_t reg, uint8_t val);

uint8_t VL_WriteReg16(uint8_t i2cDeviceAddress, uint16_t reg, uint16_t val);

uint8_t VL_ReadReg8(uint8_t i2cDeviceAddress, uint16_t reg, uint8_t *valP);

uint8_t VL_ReadReg16(uint8_t i2cDeviceAddress, uint16_t reg, uint16_t *valP);

uint8_t VL_ChipEnable(uint8_t deviceNo, bool on);
uint8_t VL_SetI2CDeviceAddress(uint8_t i2cDeviceAddress);

uint8_t VL_InitDevice(uint8_t i2cDeviceAddress);
uint8_t VL_ConfigureDefault(uint8_t i2cDeviceAddress);

uint8_t VL_InitAndConfigureDevice(uint8_t i2cDeviceAddress);
uint8_t VL_Init(void);

void InfraredTOF_Run(void);



#endif /* INFRAREDTOF_H_ */
