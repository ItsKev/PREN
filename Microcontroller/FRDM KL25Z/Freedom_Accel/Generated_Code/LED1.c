/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : LED1.c
**     Project     : ProcessorExpert
**     Processor   : MKL25Z128VLK4
**     Component   : LED
**     Version     : Component 01.047, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2012-12-25, 18:56, # CodeGen: 13
**     Abstract    :
**          This component implements a universal driver for a single LED.
**     Settings    :
**          Component name                                 : LED1
**          LED on with initialization                     : no
**          HW Interface                                   : 
**            LED Pin                                      : LEDpin
**            Anode on port side                           : no
**     Contents    :
**         On     - void LED1_On(void);
**         Off    - void LED1_Off(void);
**         Neg    - void LED1_Neg(void);
**         Get    - byte LED1_Get(void);
**         Put    - void LED1_Put(byte val);
**         Init   - void LED1_Init(void);
**         Deinit - void LED1_Deinit(void);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2012, all rights reserved.
**     This an open source software implementing an LED driver using Processor Expert.
**     This is a free software and is opened for education, research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/

/* MODULE LED1. */

#include "LED1.h"

/*
** ===================================================================
**     Method      :  LED1_On (component LED)
**
**     Description :
**         This turns the LED on.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/* This method is implemented as macro in the header file */

/*
** ===================================================================
**     Method      :  LED1_Off (component LED)
**
**     Description :
**         This turns the LED off.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/* This method is implemented as macro in the header file */

/*
** ===================================================================
**     Method      :  LED1_Neg (component LED)
**
**     Description :
**         This negates/toggles the LED
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/* This method is implemented as macro in the header file */

/*
** ===================================================================
**     Method      :  LED1_Get (component LED)
**
**     Description :
**         This returns logical 1 in case the LED is on, 0 otherwise.
**     Parameters  : None
**     Returns     :
**         ---             - Status of the LED (on or off)
** ===================================================================
*/
/* This method is implemented as macro in the header file */

/*
** ===================================================================
**     Method      :  LED1_Init (component LED)
**
**     Description :
**         Performs the LED driver initialization.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/* This method is implemented as macro in the header file */

/*
** ===================================================================
**     Method      :  LED1_Put (component LED)
**
**     Description :
**         Turns the LED on or off.
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - value to define if the LED has to be on or
**                           off.
**     Returns     : Nothing
** ===================================================================
*/
/* This method is implemented as macro in the header file */

/*
** ===================================================================
**     Method      :  LED1_Deinit (component LED)
**
**     Description :
**         Deinitializes the driver
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void LED1_Deinit(void)
{
}

/* END LED1. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 10.0 [05.03]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/