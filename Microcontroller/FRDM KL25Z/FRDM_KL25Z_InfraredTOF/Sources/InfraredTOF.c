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


static InfraredTOF_TDataState deviceData; 

uint8_t InfraredTOF_ReadReg(uint8_t addr, uint8_t *data, short dataSize) {
  uint8_t res;
 
  /* Send I2C address plus register address to the I2C bus *without* a stop condition */
  res = I2C2_MasterSendBlock(deviceData.handle, &addr, 1U, LDD_I2C_NO_SEND_STOP);
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataTransmittedFlg) {} /* Wait until data is sent */
  deviceData.dataTransmittedFlg = FALSE;
 
  /* Receive InpData (1 byte) from the I2C bus and generates a stop condition to end transmission */
  res = I2C2_MasterReceiveBlock(deviceData.handle, data, dataSize, LDD_I2C_SEND_STOP);
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataReceivedFlg) {} /* Wait until data is received received */
  deviceData.dataReceivedFlg = FALSE;
  return ERR_OK;
}

uint8_t InfraredTOF_WriteReg(uint8_t addr, uint8_t val) {
  uint8_t buf[2], res;
 
  buf[0] = addr;
  buf[1] = val;
  res = I2C2_MasterSendBlock(deviceData.handle, &buf, 2U, LDD_I2C_SEND_STOP); /* Send OutData (3 bytes with address) on the I2C bus and generates not a stop condition to end transmission */
  if (res!=ERR_OK) {
    return ERR_FAILED;
  }
  while (!deviceData.dataTransmittedFlg) {}  /* Wait until date is sent */
  deviceData.dataTransmittedFlg = FALSE;
  return ERR_OK;
}

void InfraredTOF_Run(void){
	uint8_t res;
	
	deviceData.handle = I2C2_Init(&deviceData);


	
	for(;;){
		
	}
}
