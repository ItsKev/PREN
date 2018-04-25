/*
 * InfraredTOF.c
 *
 *  Created on: Mar 10, 2018
 *      Author: burak
 */

#include "InfraredTOF.h"
#include "InfraredTOF_I2C.h"
#include "GI2C1.h"
#include "LED_Onboard_Green.h"


// VL53L0X internal registers
#define REG_IDENTIFICATION_MODEL_ID					0xc0
#define REG_IDENTIFICATION_REVISION_ID				0xc2
#define REG_SYSRANGE_START							0x00
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        0x8a

#define REG_RESULT_INTERRUPT_STATUS 				0x13
#define RESULT_RANGE_STATUS      					0x14
#define ALGO_PHASECAL_LIM                       	0x30
#define ALGO_PHASECAL_CONFIG_TIMEOUT            	0x30

#define GLOBAL_CONFIG_VCSEL_WIDTH               	0x32
#define FINAL_RANGE_CONFIG_VALID_PHASE_LOW      	0x47
#define FINAL_RANGE_CONFIG_VALID_PHASE_HIGH     	0x48

#define PRE_RANGE_CONFIG_VCSEL_PERIOD           	0x50
#define PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI      	0x51
#define PRE_RANGE_CONFIG_VALID_PHASE_LOW        	0x56
#define PRE_RANGE_CONFIG_VALID_PHASE_HIGH       	0x57

#define REG_MSRC_CONFIG_CONTROL                 	0x60
#define FINAL_RANGE_CONFIG_VCSEL_PERIOD         	0x70
#define FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI    	0x71
#define MSRC_CONFIG_TIMEOUT_MACROP              	0x46
#define FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT 0x44
#define SYSRANGE_START                          	0x00
#define SYSTEM_SEQUENCE_CONFIG                  	0x01
#define SYSTEM_INTERRUPT_CONFIG_GPIO            	0x0A
#define RESULT_INTERRUPT_STATUS                 	0x13
#define VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV       	0x89
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_0        	0xB0
#define GPIO_HV_MUX_ACTIVE_HIGH                 	0x84
#define SYSTEM_INTERRUPT_CLEAR                  	0x0B


uint8_t VL_WriteReg8(uint8_t i2cDeviceAddress, uint16_t reg, uint8_t val) {
  uint8_t r[2];

  r[0] = reg>>8;
  r[1] = reg&0xff;
  return GI2C1_WriteAddress(i2cDeviceAddress, &r[0], sizeof(r), &val, sizeof(val));
}

uint8_t VL_WriteReg16(uint8_t i2cDeviceAddress, uint16_t reg, uint16_t val) {
  uint8_t r[2], v[2];

  r[0] = reg>>8;
  r[1] = reg&0xff;
  v[0] = val>>8;
  v[1] = val&0xff;
  return GI2C1_WriteAddress(i2cDeviceAddress, &r[0], sizeof(r), &v[0], sizeof(v));
}

uint8_t VL_ReadReg8(uint8_t i2cDeviceAddress, uint16_t reg, uint8_t *valP) {
  uint8_t tmp[2];

  tmp[0] = reg>>8;
  tmp[1] = reg&0xff;
  return GI2C1_ReadAddress(i2cDeviceAddress, &tmp[0], sizeof(tmp), valP, 1);
}

uint8_t VL_ReadReg16(uint8_t i2cDeviceAddress, uint16_t reg, uint16_t *valP) {
  uint8_t tmp[2];

  tmp[0] = reg>>8;
  tmp[1] = reg&0xff;
  return GI2C1_ReadAddress(i2cDeviceAddress, &tmp[0], sizeof(tmp), (uint8_t*)valP, 2);
}

uint8_t VL_SetI2CDeviceAddress(uint8_t i2cDeviceAddress) {
  uint8_t res;
  uint8_t val;
  int i;

  i2cDeviceAddress &= 0x7F; /* make sure it is 7bits only */
  /* test: check if the device is already configured */
//  res = VL_ReadReg8(i2cDeviceAddress, VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, &val);
//  if (res==ERR_OK || val == i2cDeviceAddress) {
//    /* already configured :-) */
//    return ERR_OK;
//  }
  /* device just has been powered on: it shall be accessible using the default address */
  res = VL_WriteReg8(VL5310X_DEFAULT_I2C_ADDRESS, VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, i2cDeviceAddress);
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  /* test: read back with new address */
  res = VL_ReadReg8(i2cDeviceAddress, VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, &val);
  if (res!=ERR_OK || val != i2cDeviceAddress) {
    return ERR_FAILED;
  }
  /* device is accessible with the new address now */
  return ERR_OK;
}

void InfraredTOF_Run(void){	
	LED_Onboard_Green_On();

	VL_SetI2CDeviceAddress(0x10);
	
	for(;;){
		
	}
}
