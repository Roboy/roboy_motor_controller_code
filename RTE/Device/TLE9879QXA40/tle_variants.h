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
 * \file     tle_variants.h
 *
 * \brief    Device specific memory layout defines
 *
 * \version  V0.1.0
 * \date     10. Dec 2018
 */

/*******************************************************************************
**                      Author(s) Identity                                    **
********************************************************************************
**                                                                            **
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** JO           Julia Ott                                                     **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 * V0.1.0: 2018-12-10, JO:   initial version
 */


#ifndef _TLE_VARIANTS_H
#define _TLE_VARIANTS_H

/* Family definitions  */
#define TLE98      (98)

/* Series definitions  */
#define TLE984     (984)
#define TLE985     (985)
#define TLE986     (986)
#define TLE987     (987)

/* Device definitions  */
#define TLE9871    (9871)
#define TLE9873    (9873)
#define TLE9877    (9877)
#define TLE9879    (9879)

/* Package definitions */
#define BGA144     (1)
#define LQFP144    (2)
#define LQFP100    (3)
#define BGA64      (4)
#define LQFP64     (5)
#define VQFN48     (6)
#define TSSOP38    (7)
#define TSSOP28    (8)
#define TSSOP16    (9) 
#define VQFN24     (10)
#define VQFN40     (11)

#if defined(TLE9871QXA20)	
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9871
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (36UL)
#define ProgFlashSize   (0x8000U)
#define RAMSize         (0xC00U)

#elif defined(TLE9873QXW40)	
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9873
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (48UL)
#define ProgFlashSize   (0xB000U)
#define RAMSize         (0xC00U)

#elif defined(TLE9877QXA20)
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9877
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (64UL)
#define ProgFlashSize   (0xF000U)
#define RAMSize         (0x1800U)

#elif defined(TLE9877QXA40)
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9877
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (64UL)
#define ProgFlashSize   (0xF000U)
#define RAMSize         (0x1800U)

#elif defined(TLE9877QXW40)
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9877
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (64UL)
#define ProgFlashSize   (0xF000U)
#define RAMSize         (0x1800U)

#elif defined(TLE9879QXA20)
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9879
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (128UL)
#define ProgFlashSize   (0x1F000U)
#define RAMSize         (0x1800U)

#elif defined(TLE9879QXA40)
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9879
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (128UL)
#define ProgFlashSize   (0x1F000U)
#define RAMSize         (0x1800U)

#elif defined(TLE9879QXW40)
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9879
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (128UL)
#define ProgFlashSize   (0x1F000U)
#define RAMSize         (0x1800U)

#elif defined(TLE9879_2QXA40)
#define UC_FAMILY       TLE98
#define UC_SERIES       TLE987
#define UC_DEVICE       TLE9879
#define UC_PACKAGE      VQFN48
#define UC_FLASH        (128UL)
#define ProgFlashSize   (0x1F000U)
#define RAMSize         (0x1800U)

#else
#error "tle_device.h: device not supported"
#endif 	    
/*----------------------------------------------------------------------------
  Define Memory
 *----------------------------------------------------------------------------*/
#define ProgFlashStart  (0x11000000U)      /* Start Address of the flash      */
#define DataFlashStart  (ProgFlashStart + ProgFlashSize)
#define DataFlashSize   (0x1000U)          /* 4KB Data Flash                  */
#define NACStart        (DataFlashStart - 4U)
#define NADStart        (DataFlashStart - 2U)
#define RAMStart        (0x18000000UL)     /* Start Address of the SRAM       */

#if (UC_SERIES == 987)
  #include "tle987x.h"
#endif

#endif
