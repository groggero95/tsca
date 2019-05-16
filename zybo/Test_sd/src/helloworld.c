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
static FIL f_time, f_plain, f_cypher;		/* File object */
static FATFS fatfs;
TCHAR *Path = "0:/";
/*
 * To test logical drive 0, FileName should be "0:/<File name>" or
 * "<file_name>". For logical drive 1, FileName should be "1:/<file_name>"
 */
static char time[32] = "time.bin", plain[32] = "plain.bin", cypher[32] = "cypher.bin";
static char *SD_time, *SD_plain, *SD_cypher;

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
	SD_cypher = (char *)cypher;

	Res = f_open(&f_plain, SD_plain, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}

	Res = f_open(&f_time, SD_time, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}


	Res = f_open(&f_cypher, SD_cypher, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}


	int i;

	XTime tStart;
	XTime tEnd;
	XTime delta __attribute__ ((aligned(32)));


	// bigint_t modulus = init("0xc26e8d2105e3454baf122700611e915d");
	// bigint_t modulus = init("0x5f4c6991b0042610e525a5c4981737a1");
	// bigint_t modulus = init("0xc8aed04da6c85dd4638add6c6fc04a59");
	// bigint_t modulus = init("0x8bd09d3203b60a2255885d348eb020af8d2c040a399c6e07e40ee478ac4a2881");
	bigint_t modulus = init("0x4da608e6f07e83d2e96d24d2926bcb7428fab686240e82fa20da0a9b855df395");
	// bigint_t modulus = init("0x674b89bb51449c6281854973613618a189e52553b7974cc31026f50ecd3df6af1a2b1c086d1cbc9a9527536b9ebbb81fba8e751de1e408391e42a1e4451beba1");
	// bigint_t modulus = init("0xe65b47e568e75923e7815e93173f5e5a5eac67c589a9b5140a08eed202b68fc2f28bc0648403bc8fab1463c018df350d5ee91c6ed0e05a6298715bcb5ac25833");
	// bigint_t modulus = init("0xd9ab1a2608b02d939f8119200a599acb229d7a78c196463d519cd2057e9ba913dbc41b8d6a879a8ef3d0bce5766903f9be119980508252cb89d039605d400dceca702d5381d2288f97f2e7b04c1afdd10b9d0928b3102130661bb9758a45b7d32d5927c0a44e564e19b7fe32ae9417436f4a79a1414f3ed59c2d32608cbfe239");
	// bigint_t modulus = init("0xdb5f0bd7c4e3c35af6dc04d877be3b987a8728de7d6df7efa3720080306f5dfcc01a519cb155e2f8e85459fbb637852d42f18918702dae3bea3eed40d6c7b222ab68483ceca509b5c74f0c4b05a00ac50121995f8d9731021f64f0eef06a51fb95035346db4ea77df17738e81b6f24913ed02814b06877615f3f0309f71b6a79");
	// bigint_t modulus = init("0xc068948d0d0ae42f9d7116c6650cef879a2d6314912e8ca5c0b49c124f2a81481f0430ec2d2f3651a432e4d83a1e1cc2c1d6489589540aec1d77aa93ef6011c2d899e5eb00a02bc4bab1c449141f6f18392cc16d83e36dea93de095853a205fbbf74228823f3abd53c19e63164aaaa49564d79d5b7404d7de14166657ff038d1de37b74d7ec89c57208fc747669842faa3c49cdbe1c5dee32b0e044e18f6a06f5acb6c440a86ccc4247a86456448892659c87b86cda8e5a670f35555e74b556a6ece4d06f898024d96e7563374b14a3b52846930ac29d85d2dd747abfc125e9de2213aacb65b9242e469bae628629f0d1b195d48832bf528e1c55c6b58c02f31");
	// bigint_t modulus = init("0xda2fbef83c89087ce75f35795ec138cffeb1b59a313737d60c0e2fd59633a95e6b9d1c526fd974dbd17a894c4a4703592fa108530ac629f2ad6a605ad3032925d3bc63635e63b37ac2acdfcf475ab27af16b4ae0cc7358114959402e09bd0920329e9ebb16f9f949b5bc7c7b3d8be597976c86d36d339f9ce3a37c5af5109e3ed9fce199b3c8a58461f9838f778a6bdb5db59372e773af39350df939832ad3cc50ee512c1948f3e8af317f1df4f5fd79c8c8b2906bce961381132f1a4e5ff0eeb8757a1fa8cfba0b6e545dde7c74b8c09e2afda7fcd14ff2524ba7900f7b8ab5ffe0adf454f61ed90b1fbad976f26720f5b5a0ca33086c990cf4158bf4a4d317");
	// bigint_t public  = init("0x00000000000000000000000000010001");
	// bigint_t public  = init("0x3a4ed057cd61b97bae9c104f6eac35ab80b1cd11b4a90828fc5089de2389a661");
	bigint_t public  = init("0x1a9381041236491002e2144b8faf2e4ce1e5e0a78c1e1db02f33c7f0e5152dcf"); // wrong one i don't know it anymore
	// bigint_t public  = init("0x577184ec9665075ff2eec5219d49d98f58db2b1d6cbf461559a98a7440f1fec82984fa61c881f11eaa8cf6593e5be1019df7c0af87d6edc287e7990ca2f96233");
	// bigint_t public  = init("0xa5a9a570e769ed60af6f56682176c069fbdca43e2baaf44c5516a266535f7147a497ee28a2329ae01c6b5ac2af4dd6fd5adfad587ab018d8a8d25a3b288a5689");
	// bigint_t public  = init("0x09ca73ee6bfb1096a80b8ca3cd54e8dd9b3c5b123bde5c49c242f7d296a0e696d520dd9f5b80908f2bb54907873bde84f4ac5c0f6b42568e094b2ce4c8942d8dae8fe9317db27badf5ef98fd51e5a3397e72641db48241f5a9cecaa0c8d72020239a03c56e6a0955cefe2c4c6fad35f2993b1c0c34bf4d52f11a307e7fd1a9e7");
	// bigint_t public  = init("0x673ccf463a6a97f9ad1d63abf4e223e5897d30035a53b8b264d0cfe91b69905841267856207546b0149af6125cbe052d31841652038b5c304532be5dde4584ed9e92b4c55a8158c476a7f11d41382aa83388b0fb881a00d5a18fce23525009e16a29b90521ded7c5905d6c484681423795d48edebcbfb639f823eaa63de787af");
	// bigint_t public  = init("0xa0aae4fccd409be11e117b2b59b217adc8087c6b8d8573672c480e7fb67cdef92b2e7ce4d6d9478456fe2e72d77eabc74ea740b349dcc9677a9532ec421382b7e0c10a3b86fdaf119f828ec796fb0c0936e629347008722cbdfef10202fcc17eda736fa79452eb8ffa13227f81fbdb1804ab031f8a304f37c8438d2a2fad5e51faed8f4c91d5aabaa2c48a1bf17ab55aa7617b10273174593ace266f5f65f564909fbd981981c38dd470a53d80fcc5894badedc21c58991c2e93eeaecacd3d2bb0b62300fae256c7406cc1ede135756bd1a8f957a544a8607755ea52d06eb0effcf5933a5360ead3531b2d85ddcbb45d054837a0f1ea8ae5172e872172dd817f");
	// bigint_t public  = init("0x3e4557b5beff3c667959d828fa8096bbefde9a94864921fe3b814bee3ea68db3c93fa515aa257e3e1798ff5361297e374b0b49f6a05cddadcbebbbccfa20f316f848201d886794b41c8815985e6fc8bac4dc4370644827872a03ae259e572c695b962a082a5613d904d05fa19fc63319bee2b211f00c0c82824ce83148d1c07138b9b88574a5556bd82a1dd12db92832091a998bca2b57de6c94a084ed326c2e75e540f3af7d95646ff77abc91361b77d1603757ec7065000dfa75ea6c38347ed42b0b67a85599f7805225c737db47aa64bbe600c219c5f9a81851debe8c54ccda0968d3d815b724868b2385ef059f34c6bf72417466670bd1bb414464cbbeab");
	// bigint_t private = init("0x0745812bb1ffacf0b5d6200be2ced7d5");
	// bigint_t private = init("0x4c0929ca2608895dc42fbc87ef853e09");
	// bigint_t private = init("0x2845ecc7a890cd4356ef00ff86e63f81");
	// bigint_t private = init("0x60c0fdfc71f6698b0a2529c4d64712fb135a399d27fd9f1b720aa52268c90a91");
	bigint_t private = init("0x03c71d14ee6b177683a32751c239becb0504e087ee52293d402ed6a3e01053cb");
	// bigint_t private = init("0x10a51386ea3dd4035647c8644a335ae399d6b5e5175934c63aba1eca948f367b53c00e59ca21937a03792a96005b57c8106ad20e2af98b69ca2fd5cda48a37fb");
	// bigint_t private = init("0x1b87c092b8c74c1dc4ddfa70c45e49dd109e561cc9508eaaf20293d421776d5db1f180d0c735d321362823ec248702060f3a94e785b714cfc10696043e29c911");
	// bigint_t private = init("0x8c9f600a82919b3b70cf10e575eeefce627f41662ddd27c86ad8dc23edb0da8ab819a72ff73ea2df71122a90208dd7476a732c96b3b3fa69f6ffed727858dc5cdc64812ad6f748763a87c6ad0883209e2a6d31ca2cca61f5ce00a5830699b3aa0876d0ccb2a795b2c951e224d4cd68d663b071aac221f55ea3fd69287246d633");
	// bigint_t private = init("0x4a230c0a9898d9c82ed847007bebda7732f44ab30339fcdbb7314c18006b04c589d4437d676edf66cacae20981ae5a7eb8467c910ffca43860a350c4bdfb27925c3b0d3a8fbddb64b905a8ba9401afabba47294bc0bf5f57eb449d16210a59731f9fe66605007ae431daf1d502eee11a3568b0397b62632ac4a81cf953db424f");
	// bigint_t private = init("0xa6b0054c8ea44e430d942290ebb86da47971c8b19072cd0ffe9baafa3edb74a5d4d5783ae31d3aeca2bb5ed37541cde626a667ce080827e11563fb57ecb28e482c5260441bc12ca5cb230f7b856ed383329fb5139f043f1f20bb41c32faa76e84e1fdc3e9ed09436bab292e09e486794aed2607cffca687dd20eab7107be0e38ad2ac2b1a90dbec94257b6bb2d21c22b1d19427eb22a9912207a8ee37e9a00d89cec1551e5f7f7eb6155d913d686a84cba265daa5e8230b5b62e3aed8fd4bdc7690852c9aa00ce01bffe70c93cdf94584c417c165a86654c91ea8c3aefedccd1aab4143f626e06f6d2767ae7aa83be310e33ab0a08e765d1b4eee04860ce8e7f");
	// bigint_t private = init("0x12bd18063a3246ca88f3bacbc1b916eb3c2d5be79f2e0b482174b87db21729a0275104b715bff7a221fb41d10b9cefb2fa1a84f10b019db9f0efda2ebd2a2bfd48633cf23b26c80703673b621264b510df517883f0759e30f0c291902ed8f0f8f94b099e96869f89f75ab570b7208ef6b87aaec1b35880c6d82e41f1b03454c0331c46cbaf86e91df32fc40483984f801291ee0286b76000fa2997094eb28f719bfc827fead62818f61157ce777b187cbfc66885476fb82d517f3da4090334fe75d49d49569f5e0e0a4458c0812f4176cfc5f9b8608ed0d5d419f29a270c69b8a354a7e3e4730aced6f0e60fe3018f7088d98beb72876a4039c73ca048ed1c83");
	bigint_t message; // = init("0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004369616f20636f6d652076613f");
	// bigint_t k0 	 = init("0x8354f24c98cfac7a6ec8719a1b11ba4f");
	// bigint_t k0 	 = init("0x0424eafd3cea52419284de3f6da92934");
	// bigint_t k0 	 = init("0x64d8149d5c75b7137c099ce764ab8335");
	// bigint_t k0 	 = init("0x2801ebbfecc8bf74941d23e9fb4edc7fb51a964a86ae40027ad15a791325ffc5");
	bigint_t k0 	 = init("0x31b4f8009bba93da537be726cb9ae710df71cb8a1a7e66c8ffb5e15ba5d7cbf5");
	// bigint_t k0 	 = init("0x4e45781ff3ebd436b6497fcc94150fe0a90e5cbeabc2a017830d7a33362fb1537a75823a04729682d0501a92c5b689236df22ce1b4a0cff93ba6708a4924ff8f");
	// bigint_t k0 	 = init("0xaa1a0fca84d8ecc72a7c3608802b73dc25b2bd8cbf8fe05e4c3ba100fc5437cb95f1b40bd815e62a5b7a0fbeaee08d694ea7d41bb6fb23db0c748800b3201da7");
	// bigint_t k0 	 = init("0x3701ca8f3275a47f80a11d09d059236773ece4719a60ccd3e17122197a9b623d6b2597ba4cf8f4fb254995266669df05546e56ac01f3ce1433185ced12de3e0c6fa6e04585c14ef77555bf7c4b25623c1ed3385bda2b998121909556b968e01efe9036634ecc602f0b742cce201442ce20b0022acf96c2f5e634d0117bff26c6");
	// bigint_t k0 	 = init("0x295365ad8d77a9d142385a0c498ca294f100cb700ca9457aa862b751ab5f6983ff70a05c2370ae0437c35324142a8c7e2c712859e380e0ab97660d270fae46a77be094dc33be6fb250884feea4835398061e2a6a6b1a6901bf09533d65fc90b23ac1bdb14fb5ddccb23829ba4974eb4ad7e78bfe3840e5b43a2b9c79504e3726");
	// bigint_t k0 	 = init("0x7eb894b9738d721a8178647a4db22289b5cf18c01d1de7d7ee0705f6756d3f6b312e27be9fb47535ada3c778c7aa8799e4fc66b6462b7b017cb2b78a33315974c034ca9e29d9741ebd1415477fc371c2ada16bdca1428419796a308d58e2f3a245742b8b6973c0c67b1b1b34a546efe90bfd5c85a69c1f15cadb7dfd7609e87249576acbfb1929a508fa1dc1a65636211e946e4da9cb0a6b27f59f6c99c625185f1288aa984f5bb4c41c7d89c6ffb9f45474974ff44ad957604701600e6ae686aa3289223913a186029f5354f4730aa2d919d9d3342251af66407fa8daa493e601e4d3adc7729cebee270be5b4a3fd8541cae02b0660fe642cbcda3cace2d14d");
	// bigint_t k0 	 = init("0x441d82ea5cce3297c7267e5e2065ab5c1ee3c3eec5af7b4683b560835e0f87279f85cdd826a085e02cbdf3b738eae81fa7674a271a3b33d169a9574756d5ebb3f767f43a5c8e050e81b768ff3b1513c943d7cb3f8b1c6083629598b3126dae9261da6c61bf0489e5346854ec1917e3957c2e1511b91a49a3855bdc3951766f6d032f54af608a57c2e01ba8f7edbd3aa13d7d22adbc2389cbb20ea328195bb4cb265272cc1253fda0028e83aa2eec88b7be93f152bf86119ee0e79898c4cc2f2378a0ca0c8fc1da751e7ce435016de36683cf0f3e890fb9db5f4f1d4b9bee58e0e1ae1d53ee3961a2c4da3fe2f98ca31c11deea04b0fcb8932addfe1b627265a2");

	// xil_printf("\r\n");
	// print_to_stdout(&modulus);
	// xil_printf("\r\n");
	// print_to_stdout(&public);
	// xil_printf("\r\n");
	// print_to_stdout(&private);
	// xil_printf("\r\n");	
	// print_to_stdout(&k0);
	// xil_printf("\r\n");

	message = rand_b();
	print_to_stdout(&message);
	xil_printf("\r\n");

	message = ME_big(public,message,modulus,k0,INT_SIZE+2);

	print_to_stdout(&message);
	xil_printf("\r\n");

	message = ME_big(private,message,modulus,k0,INT_SIZE+2);

	print_to_stdout(&message);
	xil_printf("\r\n");	

	srand(21);


	bigint_t enc;

	for (i = 0; i < 700000; i++ ) {
		message = rand_b();
		XTime_GetTime(&tStart);
		enc = ME_big(private, message, modulus, k0, INT_SIZE+2);
		XTime_GetTime(&tEnd);
		delta = tEnd - tStart;

		Res = f_write(&f_time, (const void*)&delta, sizeof(delta), NULL);
		if (Res) {
			xil_printf("Error failed to write time\n\r");
			return XST_FAILURE;
		}

		Res = f_write(&f_plain, (const void*)message.numb, INT_SIZE/8, NULL);
		if (Res) {
			xil_printf("Error failed to write plain text\n\r");
			return XST_FAILURE;
		}

		Res = f_write(&f_cypher, (const void*)enc.numb, INT_SIZE/8, NULL);
		if (Res) {
			xil_printf("Error failed to write cypher text\n\r");
			return XST_FAILURE;
		}


		// f_sync(&f_time);
		// f_sync(&f_plain);
		// f_sync(&f_cypher);

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
