/**
 * @cond
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

/******************************************************************************/
/** BLDC: Motor Drive with block commutation and Hall sensor                 **/
/** ROBOY Motor controller board																						 **/
/******************************************************************************/
/** Motor control over SPI on connector 1                                    **/
/** Motor connection, Maxon 300N                                             **/
/** Phase 1             : Brown                                              **/
/** Phase 2             : Red                                                **/
/** Phase 3             : Orange                                             **/
/** Hall A - P1.3       : Blue                                               **/
/** Hall B - P0.4       : Violet                                             **/
/** Hall C - P2.2       : Grey                                               **/
/** Hall Supply - VDDEXT: Yellow                                             **/
/** Hall Gnd - GND      : Green                                              **/
/******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "tle_device.h"
#include <string.h>
#include "Main.h"
#include "Emo.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
void Poti_Handler(void);
void SPI_slave_react(void);
void HardFaultHdlr(void);
void DMA_complete_handler(void);
void encoder_B_pos(void);

void Neopx_Write(uint8 *color_rgb);
void HSV2RGB(double h, double s, double v, uint8 rgb[]);
void Wheel(uint8 WheelPos, uint8 setcolor[]);
void SetBrightness(uint8 color[], uint8 brightness);
void T2_Rising_Reload(void);
void T4_Falling_Reload(void);

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
uint16 spi_tx_data[11] = {0xCAFE, 0xBABE, 0xDEAD, 0xBEEF, 0xAAAA, 
																					0xB00B, 0x0F0F, 0xC0DE, 0xF00D, 0xFADE,
																					0x1};
uint16 spi_rx_data[DMA_CH3_NoOfTrans];
uint16 count = 0;							
uint16 adc1_result = 0;	
int64 eticks = 0;						

#define NCOLORS 3
uint8 npx_data[NCOLORS];
uint8 npx_index;
uint8 npx_current_byte;
uint8 npx_bit_count;                                          

// Thank you so much @bigjosh2 for allowing me to have a bit more headroom in my
// unoptimized interrupts!
// https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/																					
uint16 npx_T3_high_ticks[2] = {2, 7};    //@10MHz GPTclk, ~0.3 and ~0.6 us 	| Corrected a bit for internal timings
uint16 npx_T3_low_ticks[2] = {49, 49};    //@10MHz GPTclk, 4.9 us						| Max is 5 us																	

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/** \brief Executes main code.
 *
 * \param None
 * \return None
 *
 */
int main(void)
{
	int i;
	uint8 colors[5][3] = {{0,0,0}, {0,0,120}, {120,0,0}, {0,120,0}, {137,137,120}};
	uint8 set_color[3];
	uint8 brightness = 255;
	
  /*****************************************************************************
  ** initialization of the hardware modules based on the configuration done   **
  ** by using the IFXConfigWizard                                             **
  *****************************************************************************/
  TLE_Init();
	
	/* We clear the under/overvoltage interrupt status flags that occur if the board
	 * is started @ 24V. (Reason is the default values loaded at boot in the threshold
	 * register ADC2->TH0_3_* are designed for operating voltages below 18V. They are
	 * only overwriten after TLE_Init())
	 */
	SCUPM->BDRV_ISCLR.reg = 0x11110000u;
	SCUPM->BDRV_ISCLR.reg = 0x0u;					// Extra op so that they are correctly set.
	
  /* Initialize E-Motor application */

  Emo_Init();
	
	/*Activate DMA controller*/
	DMA_Master_En();
	
	/*Clear LED*/
	GPT12E->T3CON.reg &= 0xFFFFFBBFu;			//Just in case some insurance, because I'm going crazy
//	PORT->P1_ALTSEL0.reg &= 0xFB;
//	PORT->P1_ALTSEL1.reg &= 0xFB;
//	PORT->P1_DATA.reg |= 0x4;
	Neopx_Write(colors[0]);
	Delay_us(1000);
	GPT12E->T3CON.reg &= 0xFFFFFBBFu;
//	PORT->P1_DATA.reg &= 0xFB;
//	PORT->P1_ALTSEL0.reg |= 0x4;
//	PORT->P1_ALTSEL1.reg |= 0x4;
	
	/*Start motor*/
	Emo_SetRefSpeed(1000);
	//Emo_StartMotor();
	
	
  while (1)
  { 
		/* Service watch
		-dog */
		WDT1_Service();
		
//		HSV2RGB(h, s, v, set_color);
		Wheel((((uint8) (eticks >> 3)) % 255), set_color);
		SetBrightness(set_color, brightness);
		Neopx_Write(set_color);
//		if (h >= 1.0){ h = 0; }
//		else { h += 0.005; }
		if  (i >= 255) { i = 0; }
		else { i++; }
		Delay_us(20000);
		
//		Neopx_Write(colors[1]);
//		Delay_us(1000000);
//		WDT1_Service();

//		Neopx_Write(colors[2]);
//		Delay_us(1000000);
//		WDT1_Service();
//		
//		Neopx_Write(colors[3]);
//		Delay_us(1000000);
		
		
//		for(i = 0; i < 100; i++)
//		{
//			Delay_us(500000);
//			if(i>=49)
//			{
//				Emo_SetRefSpeed(-2000);
//			}
//			else
//			{
//				Emo_SetRefSpeed(2000);
//			}
//		}
    
  }
} /* End of main() */

void Main_HandleSysTick(void)
{
  /* Callback function executed every ms for speed control */
  Emo_CtrlSpeed();
} /* End of Main_HandleSysTick */

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
void SPI_slave_react(void)
{
	if (SSC2->TB.reg != 0xC001u)
	{
		SSC2->TB.reg = count++;
	}
}

void HardFaultHdlr(void)
{

}

void DMA_complete_handler(void)
{
	spi_tx_data[9] = 0xC001u;
}
	
void encoder_B_pos(void)
{
	if ((PORT->P2_DATA.reg & 0x1u) == 1)				// Check P2.0, if high increment, if low decrement
	{
		eticks++;
	}
	else
	{
		eticks--;
	}
}

void Poti_Handler(void)
{
	uint16 mV;
	
	/* read the value at Ch4 (Poti) in mV     *
	 * values between 0 and 5000 are possible */
	if (ADC1_GetChResult_mV(&mV, ADC1_CH4) == true)
	{	
		//spi_tx_data[10] = mV;
    //Emo_SetRefSpeed(mV / 2); 
    if (mV > 100)
    {
      //Emo_StartMotor();
    }
    else
    {
      //Emo_StopMotor();
    }
	}
}

void Neopx_Write(uint8 *color_rgb)
{
	uint8 tmp_color[3];

	tmp_color[0] = color_rgb[1];								//Account for RGB to GRB conversion (Neopixel specific)
	tmp_color[1] = color_rgb[0];
	tmp_color[2] = color_rgb[2];
	memcpy(npx_data, tmp_color, 3);							//Load our RGB data to the cloud
  
  npx_index = 0;
	npx_bit_count = 0;
	
  npx_current_byte = tmp_color[0];
  
  GPT12E->T3.reg = npx_T3_high_ticks[ (tmp_color[0] >> (sizeof(*tmp_color)*8 - 1)) ];	//color[0].bit0_high
  GPT12E->T4.reg = npx_T3_low_ticks[ (tmp_color[0] >> (sizeof(*tmp_color)*8 - 1)) ];	//color[0].bit0_low
	GPT12E->T2.reg = npx_T3_high_ticks[ ((tmp_color[0] & 0x40u) >> 6) ];						//Constant bit latch low time interval
	GPT12E->T3CON.reg |= 0x440u;			//Set output latch and start timer				**** Both done in one operation with 0x440u
}

//void T2_Rising_Reload(void)
//{
//  if (npx_index >= 3)
//	{
//		GPT12E->T4.reg = 800;      												//Reset Latch low pulse
//	}
//	else
//	{
//		GPT12E->T4.reg = npx_T3_low_ticks[ (npx_current_byte >> (sizeof(npx_current_byte)*8 - 1)) ];	//Reload next low time
//	}
//  
//}

void T4_Falling_Reload(void)
{
	//GPT12E->T3CON.reg &= 0xFFFFFFBFu;		//Stop the timer
	if (npx_bit_count >= 7)										//*1* If we reach end if byte, increase array index and reset data and count
  {
    npx_index++;
		if (npx_index >= 3)														//*2* But if count overflows then configure timers for reset pulse
		{
			if (npx_index >= 5)																	//*3* Also if we already executed the reset pulse then stop the timer and leave
			{
				GPT12E->T3CON.reg &= 0xFFFFFFBFu;		//Stop the timer
				GPT12E->T3CON.reg &= 0xFFFFFBFFu;		//Clear output latch **** Both could be done in one operation with 0xFFFFFBBFu
				return;
			}
			else
			{
				GPT12E->T2.reg = 7;      									//Standard high duration preceding reset pulse (maybe needs to be shorter so as to not be interpreted by the neopixel as data)
				GPT12E->T4.reg = 100;      								//Reset Latch low pulse
				return;
			}
		}
		else																					//*2* Else we carry on to the next data element and reset count
		{    
			npx_current_byte = npx_data[npx_index];
			npx_bit_count = 0;
		}
  }
	else																			//*1* Else we just increment and shift
  {  
		npx_bit_count++;
		npx_current_byte <<= 1;
	}
		
  GPT12E->T2.reg = npx_T3_high_ticks[ (npx_current_byte >> 7) ];	//Reload next high time
	//GPT12E->T4.reg = npx_T3_low_ticks[ (npx_current_byte >> 7) ];	//Reload next low time
	//GPT12E->T3CON.reg |= 0x40u;			//Restart timer
}

void HSV2RGB(double h, double s, double v, uint8 rgb[])
{
    double r, g, b;

    int i = (int) (h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}

void Wheel(uint8 WheelPos, uint8 setcolor[])
{
  WheelPos = 255 - WheelPos;
	
  if(WheelPos < 85) 
	{
    setcolor[0] = 255 - WheelPos * 3;
		setcolor[1] = 0;
		setcolor[2] = WheelPos * 3;
  }
  else if(WheelPos < 170)
	{
		WheelPos -= 85;
    setcolor[0] = 0;
		setcolor[1] = WheelPos * 3;
		setcolor[2] = 255 - WheelPos * 3;
  }
	else
	{
		WheelPos -= 170;
		setcolor[0] = WheelPos * 3;
		setcolor[1] = 255 - WheelPos * 3;
		setcolor[2] = 0;
	}
	
}

void SetBrightness(uint8 color[], uint8 brightness)
{
	uint16 tmpcolor[3];
	uint8 i;

	for (i = 0; i < 3; i++)
	{
		tmpcolor[i] = color[i];			// Cast to 16 bit int to avoid problems with overflow when multiplying
		color[i] = (tmpcolor[i] * brightness) / 255;
	}
}
