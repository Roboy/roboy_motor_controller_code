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

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "scu.h"
#include "wdt1.h"
#include "lin.h"
#include "bootrom.h"
#include "pmu_defines.h"
#include "cmsis_misra.h"
#include "RTE_Components.h"

/*******************************************************************************
**                      External CallBacks                                    **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
uint32 SystemFrequency = (uint32)SCU_FSYS;


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

void SCU_ClkInit(void)
{
  sint32 int_was_mask;
  /* disable all interrupts                */
  int_was_mask = CMSIS_Irq_Dis();
 /***************************************************************************
  ** NVM Protection Control                                                **
  **************************************************************************/
#if (SCU_CFLASH_WPROT == 1)
  (void)USER_CFLASH_WR_PROT_EN((uint16) SCU_CFLASH_WPROT_PW);
#endif
#if (SCU_DFLASH_WPROT == 1)
  (void)USER_DFLASH_WR_PROT_EN((uint16) SCU_DFLASH_WPROT_PW);
#endif

 /***************************************************************************
  ** PLL/SYSCLK Control                                                    **
  **************************************************************************/
  Field_Mod8(&SCU->NMICON.reg, (uint8)SCU_NMICON_NMIPLL_Pos, (uint8)SCU_NMICON_NMIPLL_Msk, 0);
  Field_Mod8(&SCU->NMICON.reg, (uint8)SCU_NMICON_NMIWDT_Pos, (uint8)SCU_NMICON_NMIWDT_Msk, 0);

  SCU_OpenPASSWD();
  /* select LP_CLK */
  Field_Mod8(&SCU->SYSCON0.reg, (uint8)SCU_SYSCON0_SYSCLKSEL_Pos, (uint8)SCU_SYSCON0_SYSCLKSEL_Msk, 3u);
  SCU_ClosePASSWD();

  /* Oscillator Select */
  SCU_OpenPASSWD();
  Field_Wrt8all(&SCU->OSC_CON.reg, (uint8)SCU_OSC_CON);
  SCU_ClosePASSWD();

  SCU_OpenPASSWD();
  Field_Wrt8all(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON);
  SCU_ClosePASSWD();

  SCU_OpenPASSWD();
  Field_Wrt8all(&SCU->CMCON1.reg, (uint8)SCU_CMCON1);
  SCU_ClosePASSWD();

  Field_Mod8(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON_RESLD_Pos, (uint8)SCU_PLL_CON_RESLD_Msk, 1u);
  Field_Mod8(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON_VCOBYP_Pos, (uint8)SCU_PLL_CON_VCOBYP_Msk, 0u);
  Field_Mod8(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON_OSCDISC_Pos, (uint8)SCU_PLL_CON_OSCDISC_Msk, 0u);

  /* wait for PLL being locked           */
  while (u1_Field_Rd8(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON_LOCK_Pos, (uint8)SCU_PLL_CON_LOCK_Msk) == (uint8)0)
  {
  }

  SCU_OpenPASSWD();
  /* 0u << 6u */
  Field_Wrt8all(&SCU->SYSCON0.reg, 0);
  SCU_ClosePASSWD();

  Field_Mod8(&SCU->NMICLR.reg, (uint8)SCU_NMICLR_NMIPLLC_Pos, (uint8)SCU_NMICLR_NMIPLLC_Msk, 1u);
  
 /***************************************************************************
  ** Analog Clock Control                                                  **
  **************************************************************************/
  /* set factor for MI_CLK */
  Field_Wrt8all(&SCU->APCLK1.reg, SCU_APCLK1);

  /* apply setting by toggling APCLK_SET */
  SCU_OpenPASSWD();
  Field_Mod8(&SCU->APCLK_CTRL1.reg, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Pos, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Msk, 1u);
  SCU_ClosePASSWD();
  CMSIS_NOP();
  SCU_OpenPASSWD();
  Field_Mod8(&SCU->APCLK_CTRL1.reg, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Pos, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Msk, 0u);
  SCU_ClosePASSWD();

  /* set factor for Filt_CLK */
  Field_Wrt8all(&SCU->APCLK2.reg, SCU_APCLK2);

  /* apply setting by toggling APCLK_SET */
  SCU_OpenPASSWD();
  Field_Mod8(&SCU->APCLK_CTRL1.reg, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Pos, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Msk, 1u);
  SCU_ClosePASSWD();
  CMSIS_NOP();
  SCU_OpenPASSWD();
  Field_Mod8(&SCU->APCLK_CTRL1.reg, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Pos, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Msk, 0u);
  SCU_ClosePASSWD();

#ifdef SCU_APCLK_CTRL1
  SCU_OpenPASSWD();
  Field_Wrt8all(&SCU->APCLK_CTRL1.reg, (uint8)SCU_APCLK_CTRL1);
  SCU_ClosePASSWD();
#endif
#ifdef SCU_APCLK_CTRL2
  SCU_OpenPASSWD();
  Field_Wrt8all(&SCU->APCLK_CTRL2.reg, (uint8)SCU_APCLK_CTRL2);
  SCU_ClosePASSWD();
#endif

  /* apply setting by toggling APCLK_SET */
  SCU_OpenPASSWD();
  Field_Mod8(&SCU->APCLK_CTRL1.reg, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Pos, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Msk, 1u);
  SCU_ClosePASSWD();
  CMSIS_NOP();
  SCU_OpenPASSWD();
  Field_Mod8(&SCU->APCLK_CTRL1.reg, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Pos, (uint8)SCU_APCLK_CTRL1_APCLK_SET_Msk, 0u);
  SCU_ClosePASSWD();

  /* enable interrupts                     */
  if (int_was_mask == 0)
  {
    CMSIS_Irq_En();
  }
}

void SCU_Init(void)
{
 /***************************************************************************
  ** System Clock Output Control                                           **
  **************************************************************************/
  SCU->COCON.reg = (uint8) (SCU_COCON);

 /***************************************************************************
  ** Watchdog Control (internal)                                           **
  **************************************************************************/
#if (CONFIGWIZARD == 1)
  SCU->WDTREL.reg = (uint8) SCU_WDTRL;
#else /* (CONFIGWIZARD == 2) */
  SCU->WDTREL.reg = (uint8) SCU_WDTREL;
#endif
  SCU->WDTWINB.reg = (uint8) SCU_WDTWINB;
  SCU->WDTCON.reg = (uint8) SCU_WDTCON;

 /***************************************************************************
  ** Module Pin Select                                                     **
  **************************************************************************/
#ifdef SCU_MODPISEL
  SCU->MODPISEL.reg = (uint8) SCU_MODPISEL;
#endif
#ifdef SCU_MODPISEL1
  SCU->MODPISEL1.reg = (uint8) SCU_MODPISEL1;
#endif
#ifdef SCU_MODPISEL2
  SCU->MODPISEL2.reg = (uint8) SCU_MODPISEL2;
#endif
#ifdef SCU_MODPISEL3
  SCU->MODPISEL3.reg = (uint8) SCU_MODPISEL3;
#endif
  SCU->GPT12PISEL.reg = (uint8) SCU_GPT12PISEL;

 /***************************************************************************
  ** DMA Source Select                                                     **
  **************************************************************************/
#if (SCU_XML_VERSION >= 10109)
#ifdef SCU_DMASRCSEL
  SCU->DMASRCSEL.reg = (uint8) SCU_DMASRCSEL;
#endif
#ifdef SCU_DMASRCSEL2
  SCU->DMASRCSEL2.reg = (uint8) SCU_DMASRCSEL2;
#endif
#endif
}


#if (PMU_SLEEP_MODE == 1)
/* violation: Last value assigned to variable 'dummy' (defined at line 282) not used [MISRA 2012 Rule 2.2, required] */
void SCU_EnterSleepMode(void)
{
  /* violation: Previously assigned value to variable 'dummy' has not been used */
  /* This function assumes the desired     *
   * wake up sources are enabled already   */
  uint8 dummy;
  /* to remove compiler warning of unused  *
   * dummy variable                        */
  dummy = 0;
  dummy = dummy;
  /* set LIN Trx into Sleep Mode           */
  LIN_Set_Mode(LIN_MODE_SLEEP);
  /* wait until LIN Trx switched           *
   * into Sleep Mode                       */
  while (LIN_Get_Mode() != LIN_GET_MODE_SLEEP)
  {
  }
  /* wait approx. 100µs in order to pass   *
   * the filter time + margin inside the   *
   * LIN Trx. to avoid a false wake up     */
  Delay_us(100u);
  /* dummy read to clr wake up status      *
   * to prevent the device from wakeing up *
   * immediately because of still set      *
   * wake up flags                         */
  dummy = u8_Field_Rd8(&PMU->WAKE_STATUS.reg, 0, 0xFF);
  /* dummy read to clr MON wake up status  */
  dummy = u8_Field_Rd8(&PMU->WAKE_STS_MON.reg, 0, 0xFF);
  
  /* stop WDT1 (SysTick) to prevent any    *
   * SysTick interrupt to disturb the      *
   * Sleep Mode Entry sequence             */
  WDT1_Stop();
  /* Trigger a ShortOpenWindow on WDT1     *
   * to prevent an unserviced WDT1 by      *
   * accident                              */
  WDT1_SOW_Service(1u);
  /* disable all interrupts                */
  (void)CMSIS_Irq_Dis();
  /* open passwd to access PMCON0 register */
  SCU_OpenPASSWD();
  /* set SleepMode flag in PMCON0        */
  Field_Wrt8all(&SCU->PMCON0.reg, 0x02);
  CMSIS_WFE();
  CMSIS_WFE();
  /* dont do anything anymore            *
   * device is in SleepMode              */
#ifndef UNIT_TESTING_LV2
  for (;;)
  {
  }
#endif
  /* device is in Sleep Mode now          *
   * wake up performs RESET               */
}
#endif


#if (PMU_STOP_MODE == 1)
/* violation: Last value assigned to variable 'dummy' (defined at line 346) not used [MISRA 2012 Rule 2.2, required] */
void SCU_EnterStopMode(void)
{
  sint32 int_was_mask;
  /* violation: Previously assigned value to variable 'dummy' has not been used */
  uint8 dummy;
  /* to remove compiler warning of unused  *
   * dummy variable                        */
  dummy = 0;
  dummy = dummy;
  /* dummy read to clr wake up status      *
   * to prevent the device from wakeing up *
   * immediately because of still set      *
   * wake up flags                         */
  dummy = u8_Field_Rd8(&PMU->WAKE_STATUS.reg, 0, 0xFF);
  /* dummy read to clr MON wake up status  */
  dummy = u8_Field_Rd8(&PMU->WAKE_STS_MON.reg, 0, 0xFF);
  /* stop WDT1 (SysTick) to prevent any    *
   * SysTick interrupt to disturb the      *
   * Sleep Mode Entry sequence             */
  WDT1_Stop();
  /* Trigger a ShortOpenWindow on WDT1     *
   * to prevent an unserviced WDT1 by      *
   * accident                              */
  WDT1_SOW_Service(1u);
  /* disable all interrupts                */
  int_was_mask = CMSIS_Irq_Dis();
  
  /* open access to SYSCLKSEL register     */
  SCU_OpenPASSWD();
  /* select LP_CLK as sys clock            */
  Field_Mod8(&SCU->SYSCON0.reg, (uint8)SCU_SYSCON0_SYSCLKSEL_Pos, (uint8)SCU_SYSCON0_SYSCLKSEL_Msk, 3U);
  SCU_ClosePASSWD();
  
  /* open access to OSC_CON register       */
  SCU_OpenPASSWD();
  /* select OSC_PLL async.                 */
  Field_Mod8(&SCU->OSC_CON.reg, (uint8)SCU_OSC_CON_OSCSS_Pos, (uint8)SCU_OSC_CON_OSCSS_Msk, 3U);
  SCU_ClosePASSWD();
  
  /* open passwd to access PMCON0 register */
  SCU_OpenPASSWD();
  /* set PowerDown flag in PMCON0          */
  Field_Wrt8all(&SCU->PMCON0.reg, 0x04);
  CMSIS_WFE();
  CMSIS_WFE();
  CMSIS_NOP();
  CMSIS_NOP();
  CMSIS_NOP();
  CMSIS_NOP();
  
  /* dont do anything anymore            *
   * device is in PowerDown Mode         */
  /***************************************/
  /********* DEVICE IN STOP MODE *********/
  /***************************************/
  /* Device has been woken up            */
  /* reset PLL locking                   */
  Field_Mod8(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON_RESLD_Pos, (uint8)SCU_PLL_CON_RESLD_Msk, 1);
  Field_Mod8(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON_VCOBYP_Pos, (uint8)SCU_PLL_CON_VCOBYP_Msk, 0);
  Field_Mod8(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON_OSCDISC_Pos, (uint8)SCU_PLL_CON_OSCDISC_Msk, 0);

  /* wait for PLL being locked           */
  while (u1_Field_Rd8(&SCU->PLL_CON.reg, (uint8)SCU_PLL_CON_LOCK_Pos, (uint8)SCU_PLL_CON_LOCK_Msk) == (uint8)0)
  {
  }
  
  /* switch fSYS back to                 *
   * user configuration                  */
  SCU_OpenPASSWD();
  Field_Wrt8all(&SCU->SYSCON0.reg, 0);
  SCU_ClosePASSWD();
  
  /* enable interrupts                     */
  if (int_was_mask == 0)
  {
    CMSIS_Irq_En();
  }
	
	/* Init SysTick                          */
  SysTick_Init();
	
  /* Init and service WDT1                 */
  WDT1_Init();
}
#endif


void SCU_EnterSlowMode(uint8 divider_scaled)
{
  sint32 int_was_mask;
	uint16 divider[16] = {1, 2, 3, 4, 8, 16, 24, 32, 48, 64, 96, 128, 192, 256, 384, 512};
	uint32 reload_value;
	uint8 divider_scaled_local = divider_scaled;
	
	if(divider_scaled_local > (uint8)0x0F)
	{
		divider_scaled_local = 0x0F;
	}
	
	/* set the Clock Output Divider */
  Field_Mod8(&SCU->CMCON1.reg, (uint8)SCU_CMCON1_CLKREL_Pos, (uint8)SCU_CMCON1_CLKREL_Msk, divider_scaled_local);
	/* stop WDT1 (SysTick) to prevent any    *
   * SysTick interrupt to disturb the      *
   * Sleep Mode Entry sequence             */
  WDT1_Stop();

  /* Trigger a ShortOpenWindow on WDT1     *
   * to prevent an unserviced WDT1 by      *
   * accident                              */
  WDT1_SOW_Service(3u);

  /* disable all interrupts                */
  int_was_mask = CMSIS_Irq_Dis();

  /* open passwd to access PMCON0 register */
  SCU_OpenPASSWD();
  /* set SlowMode flag in PMCON0           */
  Field_Mod8(&SCU->PMCON0.reg, (uint8)SCU_PMCON0_SD_Pos, (uint8)SCU_PMCON0_SD_Msk, 0x01);
  SCU_ClosePASSWD();
	
  /***************************************/
  /********* DEVICE IN SLOW MODE *********/
  /***************************************/
  /* enable interrupts                     */
  if (int_was_mask == 0)
  {
    CMSIS_Irq_En();
  }
	
  /* set reload value of systick           */
  reload_value = ((uint32)SCU_FSYS /  divider[divider_scaled_local]) / SysTickFreq;	
  /* program SysTick Timer */
  SysTick_ReloadValue_Set(reload_value);
  
  /* Init and service WDT1                 */
  WDT1_Init();
}


void SCU_ExitSlowMode(void)
{
  sint32 int_was_mask;
  /* stop WDT1 (SysTick) to prevent any    *
   * SysTick interrupt to disturb the      *
   * Slow Mode Exit sequence               */
  WDT1_Stop();

  /* Trigger a ShortOpenWindow on WDT1     *
   * to prevent an unserviced WDT1 by      *
   * accident                              */
  WDT1_SOW_Service(3);

  /* disable all interrupts                */
  int_was_mask = CMSIS_Irq_Dis();

  /* open passwd to access PMCON0 register */
  SCU_OpenPASSWD();
  /* reset SlowDown flag in PMCON0         */
  Field_Mod8(&SCU->PMCON0.reg, (uint8)SCU_PMCON0_SD_Pos, (uint8)SCU_PMCON0_SD_Msk, 0);
  SCU_ClosePASSWD();
  
  /***************************************/
  /***** DEVICE back in NORMAL MODE ******/
  /***************************************/
  /* enable interrupts                     */
  if (int_was_mask == 0)
  {
    CMSIS_Irq_En();
  }
		
	/* Init SysTick                          */
  SysTick_Init();
  
  /* Init and service WDT1                 */
  WDT1_Init();
}

bool SCU_ChangeNVMProtection(uint32 mode, uint32 action)
{
  bool result;
  result = true;
  if (action == PROTECTION_CLEAR)
  {
    if (mode == NVM_DATA_WRITE)
    {
      result = USER_DFLASH_WR_PROT_DIS((uint16) SCU_DFLASH_WPROT_PW);
    }
    else if (mode == NVM_CODE_WRITE)
    {
      result = USER_CFLASH_WR_PROT_DIS((uint16) SCU_CFLASH_WPROT_PW);
    }
    else if (mode == NVM_DATA_READ)
    {
      result = USER_DFLASH_RD_PROT_DIS((uint16) SCU_DFLASH_WPROT_PW);
    }
    else if (mode == NVM_CODE_READ)
    {
      result = USER_CFLASH_RD_PROT_DIS((uint16) SCU_CFLASH_WPROT_PW);
    }
    else
    {
    }
  }
  else
  {
    if (mode == NVM_DATA_WRITE)
    {
      result = USER_DFLASH_WR_PROT_EN((uint16) SCU_DFLASH_WPROT_PW);
    }
    else if (mode == NVM_CODE_WRITE)
    {
      result = USER_CFLASH_WR_PROT_EN((uint16) SCU_CFLASH_WPROT_PW);
    }
    else if (mode == NVM_DATA_READ)
    {
      result = USER_DFLASH_RD_PROT_EN((uint16) SCU_DFLASH_WPROT_PW);
    }
    else if (mode == NVM_CODE_READ)
    {
      result = USER_CFLASH_RD_PROT_EN((uint16) SCU_CFLASH_WPROT_PW);
    }
    else
    {
    }
  }
  return (result);
}
