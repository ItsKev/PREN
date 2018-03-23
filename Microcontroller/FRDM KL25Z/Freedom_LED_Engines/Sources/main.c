/* ###################################################################
 **     Filename    : main.c
 **     Project     : Freedom_LED_PWM
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2017-12-18, 22:43, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "WAIT1.h"
#include "MCUC1.h"
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
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Including shared modules, which are used for whole project */


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Application.h"
#include "Stepper.h"

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */

   {
	/* Write your local variable definition here */
	unsigned int i, result; 
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */
	StepperInit(); 
	Stepper_TestApplication(); 
	
	
	
	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
