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
#include "DrivingMotor.h"
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
#include "LiftingMotor.h"
#include "PpgLdd1.h"
#include "TU1.h"
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
**     Event       :  DrivingMotor_OnEnd (module Events)
**
**     Component   :  DrivingMotor [PPG]
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
void DrivingMotor_OnEnd(void);

/*
** ===================================================================
**     Event       :  LiftingMotor_OnEnd (module Events)
**
**     Component   :  LiftingMotor [PPG]
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
void LiftingMotor_OnEnd(void);

/*
** ===================================================================
**     Event       :  TU1_OnChannel0 (module Events)
**
**     Component   :  TU1 [TimerUnit_LDD]
*/
/*!
**     @brief
**         Called if compare register match the counter registers or
**         capture register has a new content. OnChannel0 event and
**         Timer unit must be enabled. See [SetEventMask] and
**         [GetEventMask] methods. This event is available only if a
**         [Interrupt] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TU1_OnChannel0(LDD_TUserData *UserDataPtr);

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
