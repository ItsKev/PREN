/* ###################################################################
**     Filename    : Events.h
**     Project     : Freedom_LED_PWM
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-12-18, 22:43, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "LiftingMotor_MS1.h"
#include "BitIoLdd1.h"
#include "DrivingMotor_Step.h"
#include "PpgLdd2.h"
#include "LiftingMotor_MS2.h"
#include "BitIoLdd3.h"
#include "LiftingMotor_Enable.h"
#include "BitIoLdd4.h"
#include "LiftingMotor_Direction.h"
#include "BitIoLdd5.h"
#include "DrivingMotor_MS1.h"
#include "BitIoLdd6.h"
#include "DrivingMotor_MS2.h"
#include "BitIoLdd7.h"
#include "DrivingMotor_Enable.h"
#include "BitIoLdd8.h"
#include "DrivingMotor_Direction.h"
#include "BitIoLdd9.h"
#include "LiftingMotor_Step.h"
#include "PpgLdd1.h"
#include "Electromagnet.h"
#include "BitIoLdd2.h"
#include "WAIT1.h"
#include "MCUC1.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);


/*
** ===================================================================
**     Event       :  DrivingMotor_Step_OnEnd (module Events)
**
**     Component   :  DrivingMotor_Step [PPG]
**     Description :
**         This event is called when the specified number of iterations
**         is generated. (Only when the component is enabled - <Enable>
**         and the events are enabled - <EnableEvent>). The event is
**         available only when the peripheral supports an interrupt,
**         that is generated at the end of the PWM period.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DrivingMotor_Step_OnEnd(void);

/*
** ===================================================================
**     Event       :  LiftingMotor_Step_OnEnd (module Events)
**
**     Component   :  LiftingMotor_Step [PPG]
**     Description :
**         This event is called when the specified number of iterations
**         is generated. (Only when the component is enabled - <Enable>
**         and the events are enabled - <EnableEvent>). The event is
**         available only when the peripheral supports an interrupt,
**         that is generated at the end of the PWM period.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LiftingMotor_Step_OnEnd(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
