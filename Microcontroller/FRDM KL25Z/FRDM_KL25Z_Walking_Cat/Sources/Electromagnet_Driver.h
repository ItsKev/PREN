/*
 * Electromagnet.h
 *
 *  Created on: Mar 29, 2018
 *      Author: burak
 */

#ifndef ELECTROMAGNET_H_
#define ELECTROMAGNET_H_

#include "PE_LDD.h"
#include "PE_Types.h"
#include "Electromagnet_Driver.h"

extern bool Electromagnet_Driver_State; 

#define Electromagnet_Driver_ON()		({Electromagnet_SetVal(); Electromagnet_Driver_State = 1;})
#define Electromagnet_Driver_OFF()		({Electromagnet_ClrVal(); Electromagnet_Driver_State = 0;})

uint8_t Electromagnet_Driver_Init(void); 

uint8_t Electromagnet_Driver_ParseCommand(unsigned char* cmd); 

#endif /* ELECTROMAGNET_H_ */
