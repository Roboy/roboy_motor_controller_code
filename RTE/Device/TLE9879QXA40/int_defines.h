/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2018, Infineon Technologies AG
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
#ifndef INT_DEFINES_H
#define INT_DEFINES_H

/* XML Version 2.0.2 */
#define INT_XML_VERSION (20002u)

#define CPU_BUSFAULT_EN (0x0u) /*decimal 0*/

#define CPU_HARDFAULT_EN (0x1u) /*decimal 1*/

#define CPU_MEMMANAGE_EN (0x0u) /*decimal 0*/

#define CPU_NVIC_IPR0 (0xF0000000u) /*decimal 4026531840*/

#define CPU_NVIC_IPR1 (0x10u) /*decimal 16*/

#define CPU_NVIC_IPR2 (0x2000u) /*decimal 8192*/

#define CPU_NVIC_IPR3 (0x0u) /*decimal 0*/

#define CPU_NVIC_ISER0 (0x2819u) /*decimal 10265*/

#define CPU_SHPR3 (0x40000000u) /*decimal 1073741824*/

#define CPU_SYSTICK_EN (0x1u) /*decimal 1*/

#define CPU_USAGEFAULT_EN (0x0u) /*decimal 0*/

#define PMU_VDDC_OL_EN (0x0u) /*decimal 0*/

#define PMU_VDDC_OV_EN (0x0u) /*decimal 0*/

#define PMU_VDDP_OL_EN (0x0u) /*decimal 0*/

#define PMU_VDDP_OV_EN (0x0u) /*decimal 0*/

#define SCUPM_BDRV_IRQ_CTRL (0x0u) /*decimal 0*/

#define SCUPM_SYS_IRQ_CTRL (0x0u) /*decimal 0*/

#define SCUPM_SYS_SUPPLY_IRQ_CTRL (0x0u) /*decimal 0*/

#define SCU_DMAIEN1 (0x0u) /*decimal 0*/

#define SCU_DMAIEN2 (0x0u) /*decimal 0*/

#define SCU_EDCCON (0x0u) /*decimal 0*/

#define SCU_EXICON0 (0x24u) /*decimal 36*/

#define SCU_GPT12IEN (0x5u) /*decimal 5*/

#define SCU_MODIEN1 (0x0u) /*decimal 0*/

#define SCU_MODIEN2 (0x20u) /*decimal 32*/

#define SCU_NMICON (0x0u) /*decimal 0*/

#endif /* INT_DEFINES_H */
