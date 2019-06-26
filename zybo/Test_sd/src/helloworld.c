/*
 * Copyright (C) EURECOM, Telecom Paris
 *
 * Prof. Renaud PACALET         <renaud.pacalet@telecom-paristech.fr>
 * Alberto ANSELMO              <Alberto.Anselmo@eurecom.fr>
 * Simone Alessandro CHIABERTO	<Simone-Alessandro.Chiaberto@eurecom.fr>
 * Fausto CHIATANTE             <Fausto.Chiatante@eurecom.fr>
 * Giulio ROGGERO               <Giulio.Roggero@eurecom.fr>
 *
 * This file must be used under the terms of the CeCILL. This source
 * file is licensed as described in the file COPYING, which you should
 * have received as part of this distribution. The terms are also
 * available at:
 * http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.txt
*/

/******************************************************************************
*
* Copyright (C) 2013 - 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xilffs_polled_example.c
*
*
* @note This example uses file system with SD to write to and read from
* an SD card using ADMA2 in polled mode.
* To test this example File System should not be in Read Only mode.
* To test this example USE_MKFS option should be true.
*
* This example was tested using SD2.0 card and eMMC (using eMMC to SD adaptor).
*
* To test with different logical drives, drive number should be mentioned in
* both FileName and Path variables. By default, it will take drive 0 if drive
* number is not mentioned in the FileName variable.
* For example, to test logical drive 1
* FileName =  "1:/<file_name>" and Path = "1:/"
* Similarly to test logical drive N, FileName = "N:/<file_name>" and
* Path = "N:/"
*
* None.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who Date     Changes
* ----- --- -------- -----------------------------------------------
* 1.00a hk  10/17/13 First release
* 2.2   hk  07/28/14 Make changes to enable use of data cache.
* 2.5   sk  07/15/15 Used File size as 8KB to test on emulation platform.
* 2.9   sk  06/09/16 Added support for mkfs.
* 3.10  mn  08/18/18 Change file size to 8MB from 8KB for ZynqMP platform
*
*</pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"	/* SDK generated parameters */
#include "xsdps.h"		/* SD device driver */
#include "xtime_l.h"
#include "xil_printf.h"
#include "ff.h"
#include "xil_cache.h"
#include "xplatform_info.h"
#include "bigint.h"
#include "sleep.h"
#include <stdio.h>
#include "xgpiops.h"
#include "sleep.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int FfsSdPolledExample(void);
int scan_files(char* path);

/************************** Variable Definitions *****************************/
static FIL f_time, f_plain;		/* File object */
static FATFS fatfs;
TCHAR *Path = "0:/";
/*
 * To test logical drive 0, FileName should be "0:/<File name>" or
 * "<file_name>". For logical drive 1, FileName should be "1:/<file_name>"
 */
static char time[32] = "time.bin", plain[32] = "plain.bin";
static char *SD_time, *SD_plain;

#if LOG_CIPHER == 1
static char*SD_cypher;
static FIL f_cypher;
static char cypher[32] = "cypher.bin";
#endif

#ifdef __ICCARM__
#pragma data_alignment = 32
u8 DestinationAddress[10 * 1024];
u8 SourceAddress[10 * 1024];
#pragma data_alignment = 4
#else
u8 DestinationAddress[10 * 1024 * 1024] __attribute__ ((aligned(32)));
u8 SourceAddress[10 * 1024 * 1024] __attribute__ ((aligned(32)));
#endif

/*****************************************************************************/
/**
*
* Main function to call the SD example.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int main(void)
{


	XGpioPs_Config ledcfg = {XPAR_PS7_GPIO_0_DEVICE_ID, XPAR_PS7_GPIO_0_BASEADDR};
	XGpioPs led;

	XGpioPs_CfgInitialize(&led, &ledcfg, ledcfg.BaseAddr); // initialize the driver device
	XGpioPs_SetOutputEnablePin(&led, 7, 0b1); // set MIO7 as output
	XGpioPs_SetDirectionPin(&led, 7, 0b1); // set MIO7 tristate as output (idk if both are required)
	XGpioPs_IntrDisablePin(&led, 7); // disable interrupts on MIO7, (just in case)

	XGpioPs_WritePin(&led, 7, 0b0); // set MIO7 low


	FRESULT Res;
	/*
	* Register volume work area, initialize device
	*/
	Res = f_mount(&fatfs, Path, 1);

	if (Res != FR_OK) {
		return XST_FAILURE;
	}

	/*
	 * Open file with required permissions.
	 * Here - Creating new file with read/write permissions. .
	 * To open file with write permissions, file system should not
	 * be in Read Only mode.
	 */
	SD_time = (char *)time;
	SD_plain = (char *)plain;

	Res = f_open(&f_plain, SD_plain, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}

	Res = f_open(&f_time, SD_time, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}

	#if LOG_CIPHER == 1
	SD_cypher = (char *)cypher;
	Res = f_open(&f_cypher, SD_cypher, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}
	#endif


	XTime tStart;
	XTime tEnd;
	XTime delta __attribute__ ((aligned(32)));

	const key_p pair = { .public 	= { .numb = PUBLIC_INIT},
					 	 .modulus 	= { .numb = MODULUS_INIT},
					 	 .private 	= { .numb = PRIVATE_INIT},
					 	 .k0 		= { .numb = K0_INIT},
					 	 .vi 		= { .numb = VI_INIT},
					 	 .vf 		= { .numb = VF_INIT}
						};

	bigint_t message;
	bigint_t enc;

	#if TEST_MESSAGE == 1
	message = rand_b();
	printf("MSG: ");
	print_to_stdout(&message);
	xil_printf("\r\n");

	#if BLINDING == 0
	message = ME_big(pair.public, message, pair.modulus, pair.k0, INT_SIZE);
	#else
	message = ME_big_blind(pair.public, message, pair.modulus, pair.k0, INT_SIZE);
	#endif


	printf("CYT: ");
	print_to_stdout(&message);
	xil_printf("\r\n");

	#if BLINDING == 0
	message = ME_big(pair.private, message, pair.modulus, pair.k0, INT_SIZE);
	#else
	message = ME_big_blind(pair.private, message, pair.modulus, pair.k0, INT_SIZE);
	#endif

	printf("MSG: ");
	print_to_stdout(&message);
	xil_printf("\r\n");
	#endif

	int i;
	srand(949);

	for (i = 0; i < TESTNUM; i++ ) {
		message = rand_b();
		XTime_GetTime(&tStart);

		//ENCRIPTION
		#if BLINDING == 0
		enc = ME_big(pair.private, message, pair.modulus, pair.k0, INT_SIZE);
		#else
		enc = ME_big_blind(pair.private, message, pair.modulus, pair.k0, INT_SIZE)
		#endif

		XTime_GetTime(&tEnd);
		delta = tEnd - tStart;

		Res = f_write(&f_time, (const void*)&delta, sizeof(delta), NULL);
		if (Res) {
			xil_printf("Error failed to write time\n\r");
			return XST_FAILURE;
		}

		Res = f_write(&f_plain, (const void*)message.numb, BYTE_NUMB, NULL);
		if (Res) {
			xil_printf("Error failed to write plain text\n\r");
			return XST_FAILURE;
		}

		f_sync(&f_time);
		f_sync(&f_plain);

		#if LOG_CIPHER == 1
		Res = f_write(&f_cypher, (const void*)enc.numb, BYTE_NUMB, NULL);
		if (Res) {
			xil_printf("Error failed to write cypher text\n\r");
			return XST_FAILURE;
		}
		f_sync(&f_cypher);
		#endif

	}


	/*
	* Close file.
	*/
	Res = f_close(&f_plain);
	if (Res) {
		return XST_FAILURE;
	}

	/*
	* Close file.
	*/
	Res = f_close(&f_time);
	if (Res) {
		return XST_FAILURE;
	}

	#if LOG_CIPHER == 1
	/*
	* Close file.
	*/
	Res = f_close(&f_cypher);
	if (Res) {
		return XST_FAILURE;
	}
	#endif

	xil_printf("Files written successfully\n\r");

	// Switch on a led to have a visual queue of the completed operation on the zybo
	XGpioPs_WritePin(&led, 7, 0b1); // set MIO7 high

	return XST_SUCCESS;

}

