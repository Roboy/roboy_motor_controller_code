/*
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************************************************/
/**
 * \file     wdt1.h
 *
 * \brief    Window Watchdog 1 low level access library
 *
 * \version  V0.2.8
 * \date     27. Nov 2018
 */

/*******************************************************************************
**                      Author(s) Identity                                    **
********************************************************************************
**                                                                            **
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** DM           Daniel Mysliwitz                                              **
** TA           Thomas Albersinger                                            **
** JO           Julia Ott                                                     **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 * V0.2.8: 2018-11-27, JO:   Doxygen update, moved revision history from wdt1.c to wdt1.h
 *                           splitted WDT1_Init to WDT1_Init and SysTick_Init
 *                           Changed Delay_us so that delays >=1000us are possible
 *                           Changed __STATIC_INLINE to INLINE
 * V0.2.7: 2017-10-20, DM:   MISRA 2012 compliance, the following PC-Lint rules are globally deactivated:
 *                           Info 793: ANSI/ISO limit of 6 'significant characters in an external identifier
 *                           Info 835: A zero has been given as right argument to operator
 *                           Info 845: The left argument to operator '&' is certain to be 0
 * V0.2.6: 2017-04-11, DM:   Delay_us() function updated
 * V0.2.5: 2016-11-30, DM:   macros One_us and SysTickRL are moved into header
 *                           file
 * V0.2.4: 2015-02-10, DM:   individual header file added
 * V0.2.3: 2014-07-24, DM:   Delay_us() SysTick overrun fixed
 * V0.2.2: 2014-06-27, TA:   Changed to types from Types.h
 * V0.2.1: 2014-05-16, DM:   Delay_us() function added
 *                           WD_Trig_Time calculation done as #define
 * V0.2.0: 2013-10-18, DM:   reset of bSOWactive flag in WDT1_Service()
 * V0.1.0: 2012-11-12, DM:   Initial version                                   
 */

#ifndef _WDT1_H
#define _WDT1_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "types.h"
#include "tle987x.h"

/*******************************************************************************
**                      External Variable Declarations                        **
*******************************************************************************/
extern uint32 WD_Counter;

/******************************************************************************
**                      Global Macro Definitions                             **
*******************************************************************************/
#define One_us ((uint32)SCU_FSYS / 1000000u)
#define SysTickRL ((sint32)SCU_FSYS / (sint32)SysTickFreq)

/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/
/**\brief SysTick 1kHz*/
#define SysTickFreq     1000u

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
/** \brief Performs the initial service of the WDT1 (closes the long open window). 
 *\brief Resets the \link WD_Counter \endlink to '0'.
 *
 * \ingroup wdt1_api
 */
void WDT1_Init(void);

/** \brief Initializes the SysTick timer to 1ms interval 
 *\brief and enables the SysTick interrupt.
 *
 * \ingroup wdt1_api
 */
void SysTick_Init(void);

/** \brief Stops the service of WDT1 by stopping the SysTick timer.
 * 
 *\warning Handle this function with care, as a WDT1 itself will not be stopped,
 *\warning just the service of it is stopped. This might lead to device resets.
 *
 * \brief <b>Example</b><br>
 * \brief This example Stops the WDT1.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     WDT1_Stop();
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup wdt1_api
 */
void WDT1_Stop(void);

/** \brief Services the WDT1 in the open window.
 *
 * \retval TRUE WDT1 serviced
 * \retval FALSE WDT1 was not serviced (not in open window)
 *
 * \brief <b>Example</b><br>
 * \brief This example services the WDT1.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     (void)WDT1_Service();
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup wdt1_api 
 */
bool WDT1_Service(void);

/** \brief Triggers a short-window (~30ms) of the WDT1.
 * 
 * \param NoOfSOW number of consecutive Short open windows allowed
 *
 * \brief <b>Example</b><br>
 * \brief This example sets one successive Short Open Window to be allowed and triggers a SOW.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     WDT1_SOW_Service(1u);
 * }
 * ~~~~~~~~~~~~~~~  
 * \ingroup wdt1_api 
 */
void WDT1_SOW_Service(uint32 NoOfSOW);

/** \brief Delays the regular program execution by a given number of Microseconds
 * \brief the function returns if the given time has elapsed
 * \brief smaller Microsecond delay times are getting falsen by the execution time
 * \brief of the function itself
 *
 * \warning Handle this function with care, as WDT1/WDT will not be serviced during the delay time. 
 * \warning The user has to take care of WDT1/WDT service by himself.
 * 
 * \param delay_time_us Delay time in Microseconds
 *
 * \brief <b>Example</b><br>
 * \brief This example sets a delay of 100 us.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     Delay_us(100);
 * }
 * ~~~~~~~~~~~~~~~ 
 * \ingroup wdt1_api
 */
void Delay_us(uint32 delay_time_us);

INLINE void WDT1_Window_Count(void);
INLINE uint32 Systick_Value_Get(void);
INLINE void SysTick_ReloadValue_Set(uint32 val);
/*******************************************************************************
**                      Global INLINE Function Definitions                    **
*******************************************************************************/

/** \brief increments the WDT1 Window Counter
 *
 * \brief <b>Example</b><br>
 * \brief This example increments the WDT1 Window Counter.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     WDT1_Window_Count();
 * }
 * ~~~~~~~~~~~~~~~   
 * \ingroup wdt1_api
 */
INLINE void WDT1_Window_Count(void)
{
  WD_Counter++;
}


/** \brief returns the current SysTick timer count value
 *
 * \return current SysTick timer count value
 *
 * \ingroup wdt1_api
 */
INLINE uint32 Systick_Value_Get(void)
{
	return CPU->SYSTICK_CUR.reg;
}


/** \brief sets the SysTick Reload value
 *
 * \param val reload value for SysTick timer
 *
 * \ingroup wdt1_api
 */
INLINE void SysTick_ReloadValue_Set(uint32 val)
{
	CPU->SYSTICK_RL.reg = val;
}

#endif
