/*******************************************************************************
* File Name: Button.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Button_ALIASES_H) /* Pins Button_ALIASES_H */
#define CY_PINS_Button_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Button_0			(Button__0__PC)
#define Button_0_PS		(Button__0__PS)
#define Button_0_PC		(Button__0__PC)
#define Button_0_DR		(Button__0__DR)
#define Button_0_SHIFT	(Button__0__SHIFT)
#define Button_0_INTR	((uint16)((uint16)0x0003u << (Button__0__SHIFT*2u)))

#define Button_1			(Button__1__PC)
#define Button_1_PS		(Button__1__PS)
#define Button_1_PC		(Button__1__PC)
#define Button_1_DR		(Button__1__DR)
#define Button_1_SHIFT	(Button__1__SHIFT)
#define Button_1_INTR	((uint16)((uint16)0x0003u << (Button__1__SHIFT*2u)))

#define Button_2			(Button__2__PC)
#define Button_2_PS		(Button__2__PS)
#define Button_2_PC		(Button__2__PC)
#define Button_2_DR		(Button__2__DR)
#define Button_2_SHIFT	(Button__2__SHIFT)
#define Button_2_INTR	((uint16)((uint16)0x0003u << (Button__2__SHIFT*2u)))

#define Button_3			(Button__3__PC)
#define Button_3_PS		(Button__3__PS)
#define Button_3_PC		(Button__3__PC)
#define Button_3_DR		(Button__3__DR)
#define Button_3_SHIFT	(Button__3__SHIFT)
#define Button_3_INTR	((uint16)((uint16)0x0003u << (Button__3__SHIFT*2u)))

#define Button_4			(Button__4__PC)
#define Button_4_PS		(Button__4__PS)
#define Button_4_PC		(Button__4__PC)
#define Button_4_DR		(Button__4__DR)
#define Button_4_SHIFT	(Button__4__SHIFT)
#define Button_4_INTR	((uint16)((uint16)0x0003u << (Button__4__SHIFT*2u)))

#define Button_5			(Button__5__PC)
#define Button_5_PS		(Button__5__PS)
#define Button_5_PC		(Button__5__PC)
#define Button_5_DR		(Button__5__DR)
#define Button_5_SHIFT	(Button__5__SHIFT)
#define Button_5_INTR	((uint16)((uint16)0x0003u << (Button__5__SHIFT*2u)))

#define Button_INTR_ALL	 ((uint16)(Button_0_INTR| Button_1_INTR| Button_2_INTR| Button_3_INTR| Button_4_INTR| Button_5_INTR))


#endif /* End Pins Button_ALIASES_H */


/* [] END OF FILE */
