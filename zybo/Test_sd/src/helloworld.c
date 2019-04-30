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

#ifdef __ICCARM__
#pragma data_alignment = 32
u8 DestinationAddress[10 * 1024];
u8 SourceAddress[10 * 1024];
#pragma data_alignment = 4
#else
u8 DestinationAddress[10 * 1024 * 1024] __attribute__ ((aligned(32)));
u8 SourceAddress[10 * 1024 * 1024] __attribute__ ((aligned(32)));
#endif

#define TEST 7

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


	int i;

	XTime tStart, tTots, tTote, tot;
	XTime tEnd;
	XTime delta __attribute__ ((aligned(32)));

	XTime_GetTime(&tTots);

	bigint_t modulus = init("0xc26e8d2105e3454baf122700611e915d");
	bigint_t public  = init("0x00000000000000000000000000010001");
	bigint_t private = init("0x0745812bb1ffacf0b5d6200be2ced7d5");
	bigint_t message = init("0x0000004369616f20636f6d652076613f");
	bigint_t k0 	 = init("0x8354f24c98cfac7a6ec8719a1b11ba4f");

	xil_printf("\r\n");

	bigint_t enc;

	for (i = 0; i < 1000000; i++ ) {
		message = rand_b();
		XTime_GetTime(&tStart);
		enc = ME_big(private, message, modulus, k0, 130);
		XTime_GetTime(&tEnd);
		delta = tEnd - tStart;

		Res = f_write(&f_time, (const void*)&delta, sizeof(delta), NULL);
		if (Res) {
			xil_printf("Error failed to write time\n\r");
			return XST_FAILURE;
		}

		Res = f_write(&f_plain, (const void*)message.numb, sizeof(message.numb)-4, NULL);
		if (Res) {
			xil_printf("Error failed to write plain text\n\r");
			return XST_FAILURE;
		}

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

	xil_printf("Files written successfully\n\r");
	XTime_GetTime(&tTote);

	int h,l;

	tot = tTote - tTots;
	h = (int) tot >> 32U;
	l = (int) tot & ~0;
	xil_printf("%x %x\n\r",h,l);

	return XST_SUCCESS;

}

/*****************************************************************************/
/**
*
* File system example using SD driver to write to and read from an SD card
* in polled mode. This example creates a new file on an
* SD card (which is previously formatted with FATFS), write data to the file
* and reads the same data back to verify.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/
// int FfsSdPolledExample(void)
// {
// 	FRESULT Res;
// 	UINT NumBytesRead;
// 	UINT NumBytesWritten;
// 	u32 BuffCnt;
// 	BYTE work[FF_MAX_SS];
// #ifdef __ICCARM__
// 	u32 FileSize = (8 * 1024);
// #else
// 	u32 FileSize = (8 * 1024 * 1024);
// #endif

// 	/*
// 	 * To test logical drive 0, Path should be "0:/"
// 	 * For logical drive 1, Path should be "1:/"
// 	 */


// 	for (BuffCnt = 0; BuffCnt < FileSize; BuffCnt++) {
// 		SourceAddress[BuffCnt] = TEST + BuffCnt;
// 	}

// 	/*
// 	 * Register volume work area, initialize device
// 	 */
// 	Res = f_mount(&fatfs, Path, 0);

// 	if (Res != FR_OK) {
// 		return XST_FAILURE;
// 	}

// 	/*
// 	 * Open file with required permissions.
// 	 * Here - Creating new file with read/write permissions. .
// 	 * To open file with write permissions, file system should not
// 	 * be in Read Only mode.
// 	 */
// 	SD_File = (char *)FileName;

// 	Res = f_open(&fil, SD_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
// 	if (Res) {
// 		return XST_FAILURE;
// 	}

// 	/*
// 	 * Pointer to beginning of file .
// 	 */
// 	Res = f_lseek(&fil, 0);
// 	if (Res) {
// 		return XST_FAILURE;
// 	}

// 	/*
// 	 * Write data to file.
// 	 */
// 	Res = f_write(&fil, (const void*)SourceAddress, FileSize,
// 	              &NumBytesWritten);
// 	if (Res) {
// 		return XST_FAILURE;
// 	}


// 	/*
// 	* Pointer to beginning of file .
// 	*/

// 	Res = f_lseek(&fil, 0);
// 	if (Res) {
// 		return XST_FAILURE;
// 	}

	
// 	 * Read data from file.
	 
// 	Res = f_read(&fil, (void*)DestinationAddress, FileSize,
// 	             &NumBytesRead);
// 	if (Res) {
// 		return XST_FAILURE;
// 	}

// 	/*
// 	 * Data verification
// 	 */
// 	for (BuffCnt = 0; BuffCnt < FileSize; BuffCnt++) {
// 		if (SourceAddress[BuffCnt] != DestinationAddress[BuffCnt]) {
// 			return XST_FAILURE;
// 		}
// 	}

// 	/*
// 	 * Close file.
// 	 */
// 	Res = f_close(&fil);
// 	if (Res) {
// 		return XST_FAILURE;
// 	}

// 	return XST_SUCCESS;
// }

// int scan_files (
//     char* path        /* Start node to be scanned (***also used as work area***) */
// )
// {
// 	int res;
// 	DIR dir;
// 	UINT i;
// 	static FILINFO fno;


// 	res = f_opendir(&dir, path);                       /* Open the directory */
// 	if (res == FR_OK) {
// 		for (;;) {
// 			xil_printf("Start loop\r\n");
// 			res = f_readdir(&dir, &fno);                   /* Read a directory item */
// 			if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
// 			if (fno.fattrib & AM_DIR) {                    /* It is a directory */
// 				i = strlen(path);
// 				sprintf(&path[i], "/%s", fno.fname);
// 				res = scan_files(path);                    /* Enter the directory */
// 				if (res != FR_OK) break;
// 				path[i] = 0;
// 			} else {                                       /* It is a file. */
// 				xil_printf("%s/%s \r\n", path, fno.fname);
// 			}
// 		}
// 		f_closedir(&dir);
// 	}

// 	return res;
// }
