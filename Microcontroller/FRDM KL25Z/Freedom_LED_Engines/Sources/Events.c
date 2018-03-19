/* ###################################################################
 **     Filename    : Events.c
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
 ** @file Events.c
 ** @version 01.00
 ** @brief
 **         This is user's event module.
 **         Put your event handler code here.
 */
/*!
 **  @addtogroup Events_module Events module documentation
 **  @{
 */
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Stepper.h"

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
void Cpu_OnNMIINT(void) {
	/* Write your code here ... */
}

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
void DrivingMotor_OnEnd(void) {
	/* Write your code here ... */
	switch (DrivingMotor.State) {
	case ACCELARATING:
		DrivingMotor_SetFreqHz(DrivingMotor.AccelartionFreq++);
		break;

	case CRUSING:
		break;

	case DECELERATING:
		DrivingMotor_SetFreqHz(DrivingMotor.AccelartionFreq--);
		break;

	case STOPPED:
		DrivingMotor_SetFreqHz(1);
		break;
	}

	if (DrivingMotor.Direction) {
		DrivingMotor.Steps++;
	} else {
		DrivingMotor.Steps--;
	}

}

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
void LiftingMotor_OnEnd(void) {
	/* Write your code here ... */
}

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
void TU1_OnChannel0(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
} /* extern "C" */
#endif 

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
