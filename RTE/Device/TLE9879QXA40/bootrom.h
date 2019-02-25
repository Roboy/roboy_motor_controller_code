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
 * \file     bootrom.h
 *
 * \brief    BootROM low level access library
 *
 * \version  V0.2.4
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
 * V0.2.4: 2018-11-27, JO:   Doxygen update, moved revision history from bootrom.c to bootrom.h
 * V0.2.3: 2018-03-12, DM:   The following rules are locally deactivated in addidion:
 *                           Note 929: cast from pointer to pointer [MISRA 2012 Rule 11.3, required], [MISRA 2012 Rule 11.5, advisory]
 *                           Note 9018: declaration of symbol 'CustID' with union based type 'const TCustomerID *' [MISRA 2012 Rule 19.2, advisory]
 *                           The following rules are activated:
 *                           Note 970: Suppressing MISRA 2012 MISRA 2012 Directive 4.6
 * V0.2.2: 2017-09-29, DM:   BootROM functions return values corrected
 *                           GetCustomerID() parameter changed, function simplified
 *                           MISRA 2012 compliance, the following PC-Lint rules are globally deactivated:
 *                           Info 793: ANSI/ISO limit of 6 'significant characters in an external identifier
 *                           Info 835: A zero has been given as right argument to operator
 *                           Info 845: The left argument to operator '&' is certain to be 0
 *                           The following rules are locally deactivated:
 *                           Note 19: Suppressing MISRA 2012 Error 19: Useless Declaration
 *                           Note 970: Suppressing MISRA 2012 MISRA 2012 Directive 4.6
 *                           Note 923: cast from unsigned int to pointer [MISRA Rule 45]
 * V0.2.1: 2016-04-14, DM:   BF-Step BootROM API extension added 
 * V0.2.0: 2015-07-22, DM:   typedef unions added for various return values
 * V0.1.9: 2015-07-03, DM:   BE-Step compatible, USER_MAPRAM_INIT function added
 * V0.1.8: 2015-06-03, DM:   BootROM entry point defines moved into bootrom.h
 * V0.1.7: 2014-09-22, DM:   USER_ECC_CHECK modified, parameter added
 *                           ProgramPage: interrupts disabled over the entire
 *                           NVM programming flow, to prevent from nested
 *                           NVM operations during assembly buffer filling
 * V0.1.6: 2014-06-27, TA:   Changed to type definitions from Types.h
 * V0.1.5: 2014-05-18, DM:   provide a union/struct for the encoding of the
 *                           Customer ID
 * V0.1.4: 2014-05-14, DM:   add NVM protect/unprotect functions (BB-step)
 * V0.1.3: 2013-11-20, DM:   handle WDT1 and Interrupts in ProgramPage()
 *                           function,
 *                           GetCustomerID() function added 
 * V0.1.2: 2012-12-03, DM:   Defining the function pointers to be const
 *                           to save unnecessary RAM usage, 
 *                           BootROM_Init() function removed
 * V0.1.1: 2012-11-30, DM:   Implementation of all BOOTROM functions
 *                           described in the TLE986x AA-Step BootROM UM V0.9
 * V0.1.0: 2012-08-31, DM:   Initial version
 */

#ifndef _BOOTROM_H
#define _BOOTROM_H
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "types.h"
#include "RTE_Components.h"

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#define FlashPageSize 		              (128U)
#define FlashSectorSize		              (4096U)

/*******************************************************************************
** Entry points into the BootROM                                              **
*******************************************************************************/
#define addr_USER_CFLASH_WR_PROT_EN     (0x3925u);
#define addr_USER_CFLASH_WR_PROT_DIS    (0x391Du);
#define addr_USER_CFLASH_RD_PROT_EN     (0x3915u);
#define addr_USER_CFLASH_RD_PROT_DIS    (0x390Du);
#define addr_USER_DFLASH_WR_PROT_EN     (0x3905u);
#define addr_USER_DFLASH_WR_PROT_DIS    (0x38FDu);
#define addr_USER_DFLASH_RD_PROT_EN     (0x38F5u);
#define addr_USER_DFLASH_RD_PROT_DIS    (0x38EDu);
#define addr_USER_OPENAB                (0x38E5u);
#define addr_USER_PROG                  (0x38DDu);
#define addr_USER_ERASEPG               (0x38D5u);
#define addr_USER_ABORTPROG             (0x38CDu);
#define addr_USER_NVMRDY                (0x38C5u);
#define addr_USER_READ_CAL              (0x38BDu);
#define addr_USER_NVM_CONFIG            (0x38B5u);
#define addr_USER_NVM_ECC2ADDR          (0x38ADu);
#define addr_USER_MAPRAM_INIT           (0x389Du);
#define addr_USER_READ_100TP            (0x3875u);
#define addr_USER_100TP_PROG            (0x386Du);
#define addr_USER_ERASE_SECTOR          (0x3865u);
#define addr_USER_RAM_MBIST_START       (0x384Du);
#define addr_USER_NVM_ECC_CHECK         (0x3845u);
#define addr_USER_ECC_CHECK             (0x383Du);
/* BF-Step BootROM API extension */                                      
#ifdef RTE_DEVICE_BF_STEP
  #define addr_USER_ERASEPG_VERIFY      (0x3885u);
  #define addr_USER_ERASE_SECTOR_VERIFY (0x388Du);
  #define addr_USER_VERIFY_PAGE         (0x3895u);
#endif /* RTE_DEVICE_BF_STEP */

/*******************************************************************************
** Customer ID struct                                                         **
*******************************************************************************/
/** \union TCustomerID
 *  \brief This union defines the return parameter of \link GetCustomerID \endlink
 */
typedef union
{
  uint32 reg;
  struct
  {
    uint32           :8;   /*!< [0..7]   rfu                                  */
    uint32 Step      :8;   /*!< [15..8]  design step major/minor number [HEX] */
    uint32 Clock     :2;   /*!< [17..16] clock frequency                      */
    uint32 Package   :2;   /*!< [19..18] package code                         */
    uint32 SalesCode :4;   /*!< [23..20] device sales code [DEC]              */
    uint32 Family    :8;   /*!< [31..24] family code                          */
  } bit;                   /*!< [32] BitSize                                  */
} TCustomerID;

/*******************************************************************************
** USER_100TP_PROG input struct                                               **
*******************************************************************************/
/** \union T100TP_Data
 *  \brief These structs define the input data structure for \link USER_100TP_PROG \endlink
 */
typedef struct
{
	uint8 offset;
	uint8 date;
} TData;

typedef struct
{
	uint8 count;
	TData data[127];
} T100TP_Data;

/*******************************************************************************
** Function USER_100TP_PROG: return value decoding                            **
*******************************************************************************/
/** \union TUser_100TP_Prog
 *  \brief This union defines the return value of \link USER_100TP_PROG \endlink
 */
typedef union {
	uint8   reg;	
	struct {
		uint8 GlobFail  :  1; /**< OR-combination of all fail */
		uint8 OffsetFail:  1; /**< at least one offset witin input struct is out of range */
		uint8 CustIDFail:  1; /**< at least one date addresses the protected CustomerID bytes */
    uint8           :  4;
    uint8 ExecFail  :  1; /**< 100TP pages are write protected, 100x programming reached already */
	} bit;                           
} TUser_100TP_Prog;

/*******************************************************************************
** Function USER_PROG: type definition of the parameter PROG_FLAG             **
*******************************************************************************/
/** \union TProgFlag
 *  \brief This struct defines the option flag for the \link USER_PROG \endlink function
 */
typedef union 
{
  uint8   reg;	
  struct 
  {
    uint8 RAM_Branch   :1;  /*!< [0] 1 = RAM branch enables, 0 = RAM branch disabled */
    uint8 CorrAct      :1;  /*!< [1] 1 = retry/disturb handling enabled, 0 = retry/disturb handling disabled */
  } bit;                           
} TProgFlag;

/*******************************************************************************
** Function USER_PROG: return value decoding                                  **
*******************************************************************************/
/** \union TUser_Prog
 *  \brief This union defines the return value of \link USER_PROG \endlink
 */
typedef union 
{
  uint8   reg;
  struct 
  {
    uint8 GlobFail   :  1; /**< OR-combination of all fail */
    uint8            :  3;
    uint8 VerifyFail :  1; /**< verify has failed */
    uint8 EmergExit  :  1; /**< function was exited because of Emergency Exit */
    uint8 SparePgFail:  1; /**< no new Spare Page was found */
    uint8 ExecFail   :  1; /**< Assembly Buffer not opened */
  } bit;                           
} TUser_Prog;

/*******************************************************************************
** Function USER_OpenAB: return value decoding                                **
*******************************************************************************/
/** \union TUser_OpenAB
 *  \brief This union defines the return value of \link USER_OPENAB \endlink
 */
typedef union 
{
	uint8   reg;	
	struct 
	{
		uint8 ABFail   :  1; /**< addressed flash page is write protected,<br> or address out of range */
		uint8          :  6;
		uint8 ExecFail :  1; /**< Assembly Buffer already opened,<br> or nested NVM operation */
	} bit;                           
} TUser_OpenAB;

/*******************************************************************************
** Function USER_MAPRAM_INIT: return value decoding                           **
*******************************************************************************/
/** \union TUser_MAPRAM_Init
 *  \brief This union defines the return value of \link USER_MAPRAM_INIT \endlink
 */
typedef union 
{
	uint8   reg;	
	struct 
	{
		uint8 GlobFail      :  1; /**< OR-combination of all fail */
		uint8               :  4;
		uint8 DoubleMapping :  1; /**< double mapping detected, at least two physical pages are mapped to the same logical page */
		uint8 FaultyPage    :  1; /**< at least one page doesn't have any margin to the user read threshold */
		uint8 ExecFail      :  1; /**< Assembly Buffer is still open, or nested NVM operation */
	} bit;                           
} TUser_MAPRAM_Init;

/*******************************************************************************
** Function USER_NVM_ECC_CHECK: return value decoding                         **
*******************************************************************************/
/** \union TUser_NVM_ECC_Check
 *  \brief This union defines the return value of \link USER_NVM_ECC_CHECK \endlink
 */
typedef union 
{
	uint8   reg;	
	struct 
	{
		uint8 SBE           :  1; /**< Single Bit Error fail */
		uint8 DBE           :  1; /**< Double Bit Error fail */
		uint8               :  5;
		uint8 ExecFail      :  1; /**< Nested NVM operation */
	} bit;                           
} TUser_NVM_ECC_Check;

/*******************************************************************************
** Function USER_ECC_CHECK: return value decoding                             **
*******************************************************************************/
/** \union TUser_ECC_Check
 *  \brief This union defines the return value of \link USER_ECC_CHECK \endlink
 */
typedef union 
{
	uint8   reg;	
	struct 
	{
		uint8 SBE           :  1; /**< Single Bit Error fail */
		uint8 DBE           :  1; /**< Double Bit Error fail */
		uint8               :  5;
		uint8 ExecFail      :  1; /**< Nested NVM operation */
	} bit;                           
} TUser_ECC_Check;

/*******************************************************************************
** Function USER_ERASE_SECTOR_VERIFY: return value decoding                   **
*******************************************************************************/
/** \union TUser_ERASE_SECTOR_VERIFY
 *  \brief BF-Step only: This union defines the return value of \link USER_ERASE_SECTOR_VERIFY \endlink
 */
typedef union 
{
	uint8   reg;	
	struct 
	{
		uint8 GlobFail      :  1; /**< OR-combination of all fail */
		uint8 VerifyFail    :  1; /**< at least one of the 32 pages within the sector is not erased properly */
		uint8               :  4;
		uint8 MapRAMFail    :  1; /**< in case of Data Flash sector only: MAPRAM initialization failed */
		uint8 ExecFail      :  1; /**< Assembly Buffer is still open,<br> or nested NVM operation,<br> or invalid sector address,<br> or sector write proteced*/
	} bit;                           
} TUser_ERASE_SECTOR_VERIFY;

/*******************************************************************************
** Function USER_ERASEPG_VERIFY: return value decoding                        **
*******************************************************************************/
/** \union TUser_ERASEPG_VERIFY
 *  \brief BF-Step only: This union defines the return value of \link USER_ERASEPG_VERIFY \endlink
 */
typedef union 
{
	uint8   reg;	
	struct 
	{
		uint8 GlobFail      :  1; /**< OR-combination of all fail */
		uint8 VerifyFail    :  1; /**< page not in proper erased state */
		uint8               :  4;
		uint8 MapRAMFail    :  1; /**< MAPRAM initialization failure, invalid SparePage */
		uint8 ExecFail      :  1; /**< Assembly Buffer is still open,<br> or nested NVM operation,<br> or invalid page address,<br> or sector write proteced*/
	} bit;                           
} TUser_ERASEPG_VERIFY;

/*******************************************************************************
** Function USER_VERIFY_PAGE: return value decoding                           **
*******************************************************************************/
/** \union TUser_VERIFY_PAGE
 *  \brief BF-Step only: This union defines the return value of \link USER_VERIFY_PAGE \endlink
 */
typedef union 
{
	uint8   reg;	
	struct 
	{
		uint8 GlobFail       :  1; /**< OR-combination of all fail */
		uint8 VerifyEraseFail:  1; /**< page verify failed on erased bits */
		uint8 VerifyProgFail :  1; /**< page verify failed on programmed bits */
		uint8                :  4;
		uint8 ExecFail       :  1; /**< Assembly Buffer is still open,<br> or nested NVM operation,<br> or invalid page address,<br> or sector write proteced*/
	} bit;                           
} TUser_VERIFY_PAGE;

/********************************************************************************
**                      Global Function Definitions                            **
********************************************************************************/

/* In case of unit testing, these functions are stubbed. 
 * Unit test is performed with Tessy; tessy doesn't interpret the function definitions 
 * which include the pointers to bootROM as functions,
 * so the function definitions are added here for unit testing only */
#ifdef UNIT_TESTING_LV2
extern bool  USER_CFLASH_WR_PROT_EN   (uint16 cflash_pw);
extern bool  USER_CFLASH_WR_PROT_DIS  (uint16 cflash_pw);
extern bool  USER_CFLASH_RD_PROT_EN   (uint16 cflash_pw);
extern bool  USER_CFLASH_RD_PROT_DIS  (uint16 cflash_pw);
extern bool  USER_DFLASH_WR_PROT_EN   (uint16 dflash_pw);
extern bool  USER_DFLASH_WR_PROT_DIS  (uint16 dflash_pw);
extern bool  USER_DFLASH_RD_PROT_EN   (uint16 dflash_pw);
extern bool  USER_DFLASH_RD_PROT_DIS  (uint16 dflash_pw);
extern uint8 USER_OPENAB              (const uint32 NVMPAGEAddr);
extern uint8 USER_PROG                (const uint8 PROG_FLAG);
extern uint8 USER_ERASEPG             (const uint32 * NVMPageAddr, const uint8 XRAM_RTNE_BRNCHNG);
extern bool  USER_ABORTPROG           (void);
extern bool  USER_NVMRDY              (void);
extern uint8 USER_READ_CAL            (const uint8 NumOfBytes, const uint8 CSAddr, const uint16 RAMAddr);
extern bool  USER_NVM_CONFIG          (const uint8 * NVMSize, const uint8 * MapRAMSize);
extern uint8 USER_NVM_ECC2ADDR        (const uint16 * ecc2addr);
extern uint8 USER_MAPRAM_INIT         (void);
extern bool  USER_READ_100TP          (const uint8 OTP_Page_Sel, const uint8 DataOffset, const uint32 * HundredTPData);
extern uint8 USER_100TP_PROG          (const uint8 OTP_Page_Sel);
extern uint8 USER_ERASE_SECTOR        (const uint32 NVMSectorAddr);
extern uint8 USER_RAM_MBIST_START     (const uint16 RAM_MBIST_Stop_Addr, const uint16 RAM_MBIST_Start_addr);
extern uint8 USER_NVM_ECC_CHECK       (void);
extern uint8 USER_ECC_CHECK           (const uint32* ecc2addr);
#else

extern bool  (*const USER_CFLASH_WR_PROT_EN)   (uint16 cflash_pw);
extern bool  (*const USER_CFLASH_WR_PROT_DIS)  (uint16 cflash_pw);
extern bool  (*const USER_CFLASH_RD_PROT_EN)   (uint16 cflash_pw);
extern bool  (*const USER_CFLASH_RD_PROT_DIS)  (uint16 cflash_pw);
extern bool  (*const USER_DFLASH_WR_PROT_EN)   (uint16 dflash_pw);
extern bool  (*const USER_DFLASH_WR_PROT_DIS)  (uint16 dflash_pw);
extern bool  (*const USER_DFLASH_RD_PROT_EN)   (uint16 dflash_pw);
extern bool  (*const USER_DFLASH_RD_PROT_DIS)  (uint16 dflash_pw);
extern uint8 (*const USER_OPENAB)              (const uint32 NVMPAGEAddr);
extern uint8 (*const USER_PROG)                (const uint8 PROG_FLAG);
extern uint8 (*const USER_ERASEPG)             (const uint32 * NVMPageAddr, const uint8 XRAM_RTNE_BRNCHNG);
extern bool  (*const USER_ABORTPROG)           (void);
extern bool  (*const USER_NVMRDY)              (void);
extern uint8 (*const USER_READ_CAL)            (const uint8 NumOfBytes, const uint8 CSAddr, const uint16 RAMAddr);
extern bool  (*const USER_NVM_CONFIG)          (const uint8 * NVMSize, const uint8 * MapRAMSize);
extern uint8 (*const USER_NVM_ECC2ADDR)        (const uint16 * ecc2addr);
extern uint8 (*const USER_MAPRAM_INIT)         (void);
extern bool  (*const USER_READ_100TP)          (const uint8 OTP_Page_Sel, const uint8 DataOffset, const uint32 * HundredTPData);
extern uint8 (*const USER_100TP_PROG)          (const uint8 OTP_Page_Sel);
extern uint8 (*const USER_ERASE_SECTOR)        (const uint32 NVMSectorAddr);
extern uint8 (*const USER_RAM_MBIST_START)     (const uint16 RAM_MBIST_Stop_Addr, const uint16 RAM_MBIST_Start_addr);
extern uint8 (*const USER_NVM_ECC_CHECK)       (void);
extern uint8 (*const USER_ECC_CHECK)           (const uint32* ecc2addr);

/* BF-Step BootROM API extension */
#ifdef RTE_DEVICE_BF_STEP
extern uint8 (*const USER_ERASE_SECTOR_VERIFY) (const uint32 sector_addr);
extern uint8 (*const USER_ERASEPG_VERIFY)      (const uint32 page_addr);
extern uint8 (*const USER_VERIFY_PAGE)         (const uint32 page_addr);
#endif /* RTE_DEVICE_BF_STEP */

uint8 ProgramPage(uint32 addr, const uint8 * buf, uint8 Branch, uint8 Correct, uint8 FailPageErase);
bool GetCustomerID(const TCustomerID * CustID);

#endif
#endif
