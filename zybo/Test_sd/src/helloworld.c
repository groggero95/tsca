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
#define BLINDING 1

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


	// Res = f_open(&f_cypher, SD_cypher, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	// if (Res) {
	// 	return XST_FAILURE;
	// }


	int i;

	XTime tStart;
	XTime tEnd;
	XTime delta __attribute__ ((aligned(32)));


	bigint_t public  = init("0x00000000000000000000000000010001");
	// bigint_t public  = init("0x171f87ecffdafedd7a96a03f3606dffc0acd8d8c0618fe72ae3e61b598558527");
	// bigint_t public  = init("0x7821e76988333ca6edb5d53aa695054fa469381ee8b285b703a4714daeee3251");
	// bigint_t public  = init("0xb65efa9be7541a10f0e0ac0b296a6006267189bb59bec3b2624c7ac94871526b6fc597b682b7ccf678b636d58547533458afeb2607789b18ad7caad8c959b781");
	// bigint_t public  = init("0x1e907e1781857506bc54209287d871dfd08ae1811a676347fec59b4395e5af21b2ddc4df6fe2dfba93f38ca398f34bba12b7268ac5a09b785ffd04aa855a0171");
	// bigint_t public  = init("0x9cd409338b62fed2424b0cfaa82cba98b3c7750c68932c007676e8fc5fada6bfd95936b5132851b0d67819a6226210151f508794cf88069931760338fc543694f6e86a7dcae4821726a210e5228d1e11e3b1bb4d6a5feec1b09b19907123875c82ac85460aa84c48b7e2c3b4a2f738e47eb72b10cbd65c70d9cbab3a5b05077d");
	// bigint_t public  = init("0x778f547e808b4c1238a889c69653cbb92f144abc8e322600d8c8728fa64716a13a535130b95386395401506ba18508548a274581a991df0bbd1cb0504ca503158b550ae4178c14f394d3017761cb94583e704da3a311fd628a0a38ed4825439c770bdd615c08a691f6cf7b14a463f3b584e65b8b21d1f159ac2024949b8d5d2f");
	// bigint_t public  = init("0x12f267156fae0798d5432e6b8e7dae859982a41bff534efc6e22cfe04483f2257da83eb9ef111ee0252c7934b8cbd5a3222f7da7336d3147ff0a3ea261d1c787ccd5447de14662e7f0608f939c63df008d93b73fa1c779370f0b2e63e872fde95047b3cb4f4d7200d5b2dafc46ef039fa0628652cd74d65895d91816c66d820cfd0f97b2bef892d1c9392bed0ffffc22fd9d683fc826e43fa485c7a9860cd1b792a9dd909d36bf3bd53bc6cf18b40c23862b5145cbecf5970f988341d30ba4299a110a8f674dffb4a51df6cf712ff6c2a793e649b660ec1e31c4a26ed6fe5140cb5f3af164452c1e4f3c539888f4cf8b4a940b65a1ab2b3aab2e7cc1cf35ac41");
	// bigint_t public  = init("0x015d3168e9dd7d6874c0037872402e2c20103df363ecc49c77b435c888fec235bb00519c02de5de47a5d87447950a62ba735830b110adbd025a4954adf13f1e9540616e3a150a9957a93f837a2b0d1fe94b945929b61a0265609ee817aa7c7ec73d784b0a688c88af0aa8bd0a016d756b895334ec56ef8b62202b4cd51444add26eed5854b7b01478cfb18e70a5810efa6e4c82722ab02e3def85efecd1011cf58a0af858aaae7bca5b2ba74cb621e1a6941a0beb87410c0d3ad1dd4208aaedbba48c95b1c09ade78a6667c8bb9a6569e3edeaefdbb407f2207aa98738e06a21a31392427166fc4cf2a767a628bd981e2e66d3a9a21fb85bde960bc60649fcd3");

	bigint_t message, message_blind;

	// mod0 128
	bigint_t modulus = init("0xc26e8d2105e3454baf122700611e915d");
	// mod1 128
	// bigint_t modulus = init("0x5f4c6991b0042610e525a5c4981737a1");
	// mod2 128
	// bigint_t modulus = init("0xc8aed04da6c85dd4638add6c6fc04a59");
	// mod3 256
	// bigint_t modulus = init("0x8ae8fe509879989bb1314c5a1ac92506a95a284a336cfe29d9a60b5113eeb33b");
	// mod4 256
	// bigint_t modulus = init("0xbde5d7cd907ff14d2745b26d7f83ffa00254b33822c4d131724ae2dd4b7e8663");
	// mod5 512
	// bigint_t modulus = init("0xf16ea639730a5881f346ac3c065d11322b3be9c7def104269d88ed0c73bba1b05c84712bca9f0880916cab52ea3a88f04cbc2d45ee00647293b1a7c43e1acc3d");
	// mod6 512
	// bigint_t modulus = init("0xb8db0a56ef03b60c5171980681d05dd81a297500e9b06ad3f3b05bb2e1985e8de10f8eeccd0d962bf993c383092d510a5e44e43e4b2dd74ac3295f2fdc6204f1");
	// mod7 1024
	// bigint_t modulus = init("0x9e1770c77f71106d352173ad61a54b8db2884e29fa19b2094fe53c2ad2ccfa4b027068ac9b0740535a3dbb08c9bb7e8cf67d45b44f433ee5d412282c8df46f27f8b7963e1ba718b8cf4b6436e00cd15e9723a1ddb7b57a707be972b7410d14ac6ee727e593036aff4783c0e708e1f49fdc288a803f0cc0d244924ba12cbbee1b");
	// mod8 1024
	// bigint_t modulus = init("0xfd149e15687d6430ef21f0c083619c2baa6fa6435e3b83a7b00238e752d610d83b7d6459e6caaa138577df7b4dfe1cbe6001df3862f89a2454c1b5925c18884ddb0a5e0e71e92fea90aba974e58914a153578efcd36999ec069a03724b7df922a69218eca4ed02c5b49d05c3ef71641418857b8d9ca724799d6109ae1b0c3a13");
	// mod9 2048
	// bigint_t modulus = init("0xad669106fc7885226094033a92b6eb3254329bdf67c49faae96d3dd1977974e62caaea5b733cede828599e64d5da60d255ecf965d801ae36644fad1a09589116abc4719ca96a265acfb8b9fd6b57ae0a4a9bfb7c62526400851ce2c6c6644388b5a16528e500d69447bb297914098761446424f01d33c837300da1b1513d0d0e721d33f8f316f6f79741b28c077cc23de9757ccafbb47010ccf81b72697d3566dccc257297ac5084387345142c19e1f3d58f87e8b66b7003eadb103de1c6848937260177bf8d2d074e76e1c8576b29b81c1106cb7d6a29c0a219569e7735f6abfbba68c61a55c1928ba746f68e013936108a23692e7ea7600ca88975a3e57d27");
	// mod10 2048
	// bigint_t modulus = init("0xc1390f382355b6e015c83e6eee0988851a7dcac8124fcbf78935030ba8abde34eae77c20888b4d8213f8707cb178385a1f550b6e1174fe2c04b848a00bd4ddd7c38922cb4e98c6657d76742e9ea475ea849e256eee3d7f9cd7562a6736f14d234b2cc0c9cbcb39fa7c8ce8ead278d2dd2cde78dee7f2fa16e9022f62f83861673e07c08a4973b61f0fb09dde49dd20d9cf05ec4f498debf1e63c7bb3b316eb45e3c930238839ba861acd6fa39b6d7535075df1ef853f640cffb235d78a1e97a36899c19a436cd639a2a19539ecb8f4627acc4bb1789d81c817ac49d900a1fdab52504a60f890fd2bd7e2d325e63a6fa9c82c94365766ed9982271f23838a885d");


	// key0 128
	bigint_t private = init("0x0745812bb1ffacf0b5d6200be2ced7d5");
	// key1 128
	// bigint_t private = init("0x4c0929ca2608895dc42fbc87ef853e09");
	// key2 128
	// bigint_t private = init("0x2845ecc7a890cd4356ef00ff86e63f81");
	// key3 256
	// bigint_t private = init("0x1d8232d4f22da2e0705cac6d27a4d839149c23bd63746e9957bef4e85ae8f2b7");
	// // key4 256
	// bigint_t private = init("0x34350b132f9ec752124e3763a04b39e7320565f3b79aa947f114e510d9e166a1");
	// // key5 512
	// bigint_t private = init("0x05ffea52c644ee4e589eef5d6ddaf173eeee2c1ee520e88af79b3e974c5ef1d5814ea45a531a7afa872f02990ced7ef59709e4aff18f0c6ed18724cb8e505ef1");
	// // key6 512
	// bigint_t private = init("0x191858e17018a85a8de29403596a33b53792f691026d0dc2f21661ff48f558f0693fa9b206eb139a7cb8f5df1a33d1c7227dd94b837fe64cf3a977d1ec5c5cd1");
	// // key7 1024
	// bigint_t private = init("0x70b7ad9ba748fff86b9b8bfd20b97afe06f9eed59e34c8fd40b5cf614c2201687cbe6ce266502682b723eae67220c440f98db2967c7db53bc1095d035636232766b2e16e26eb94caf7af56286d82cec2824b1509591d60ed08e9b2db50cec84cbcab83b191878628c4daea65d21051ce916f9941d603a96b349c83ae120ee3b5");
	// // key8 1024
	// bigint_t private = init("0x63eee9ff13ee48d66c85787113622e5933ae4f2e8bdff62549a94800c21ba4f3b4406c06bc2b63ac706b4a2e47b783102fac11e119be2b08ce0aad36246c4d8ed5de93e701f1cb4c6e035f3dd0284e737a5c758bf8606508f19c16cf37f17bc1cfad7dbaff50f0e6e30236b7fcf54478da35def9f2311bc50387ed6236e7adff");
	// // key9 2048
	// bigint_t private = init("0x7a13785f56f48c54d12466cf178f96d3c8b7ed526b1e632874d877eea7c6e8cdc998d166bf923e24f941330adca4b281c0cdee1e82d5a54c3da8fa3325c902a94ef978ea5ebb1e693e4451bb1acd8054f7921d66c0be60abc99d7dee9e8c3e56acc17df5a8dd9ecc5588a7554fe9d0782eeda3042bee1b1321589fa0e48527dc0a80b56011c02dd5e05227e4be96a5c46a88819732f236ed50a9a792d5e8817e658bc86b988066d1031b82d2612e20a94dc8e97900eda4e153b1a2e6b2b0a05fb6a967880fde47743a12dc7aa652e195d6938310a2f9163a365081aae6e4c8acd2a64c2c60dc7e63c106de551846e7be16515c3ff99d4f956651d968b078c7e1");
	// // key10 2048
	// bigint_t private = init("0x9f4fa78ebad8a50f508199722031859a2cf8bc232124712fc6eafb54e84dec7cdd378f6a4fcaff333c928382157fbb4935c53241f207ff0fd0944dffe8b2cdfece689b43af7570806cbbf1a594770b3d24739f44530a6015c91faa9e305bc0a3de9ae4105bc0abec0245d4b6de232326bb9704f5cba348e5439fb1db31868fe760941d11aa42ec80eb6ee7345893678b9b1ac32f142f42d60f983b1d0b29adf5abaf84125e238992c8036af7e341f31a9425eddfcf8a42e835b4e6edd974106f8790f7dbc9c8453cde55b51e794928975c5258af49ec06c154598f3d811626414d672ea98880d91f7dbf1dea0e28aeee15ab67674c2ef6b97510712a0a63025b");


	// k0 0 128
	bigint_t k0 	 = init("0x8354f24c98cfac7a6ec8719a1b11ba4f");
	// k0 1 128
	// bigint_t k0 	 = init("0x0424eafd3cea52419284de3f6da92934");
	// k0 2 128
	// bigint_t k0 	 = init("0x64d8149d5c75b7137c099ce764ab8335");
	// k0 3 256
	// bigint_t k0 	 = init("0x2792adec387c47a694860bac38ca8ed7fb28e4c04f74695f7f35a915e3ed6b07");
	// k0 4 256
	// bigint_t k0 	 = init("0x87f71b22bb897d388a925b9bf69c0f7e66cbd8bac0a6a828d7da960946958aae");
	// k0 5 512
	// bigint_t k0 	 = init("0x3160493be94f6b0dac60adf43f295163782d4a53c9e811b6ccd10bb12c62f57cc3ff9aee5272bb775cf4aa1626ee023a1f90c86e3ffb3baa4ca499fd3e6e6cea");
	// k0 6 512
	// bigint_t k0 	 = init("0x30bed69e48d03d9ef171d7925d216d4097aa79e7ad72bc61afb1a94df9eb5ca35d51e52f2528bb9f1d123887bdefa2cf63e8b38c8a711239114468e4f381389b");
	// k0 7 1024
	// bigint_t k0 	 = init("0x56e99a7f5165f2ad889074dc920fc29891272bd0ad379398ee99e8e5de538405304e40c7bee16178ba5a3cb9103771dd7d37c1559b76ef4bb4fcdd54bceef2655f1fa8da45e2ef3a875661ed2dd4c4204ec82260014531ee3ee92defdfa706b0ab51e81d2993bfdaf93bb8f3fe2b12e8d214d70be7f2c7204216615f34a573d5");
	// k0 8 1024
	// bigint_t k0 	 = init("0xdd2e7e49c50226b01744188a44297147807a6d9df1de6d513319d5aaa9bc0954073700984a25ff70672c79d50a3c5639ecee092762c4ee103e731a0c92e38913d81bd35e72c33c9a19fa5eaaebb024ce8959b6849c5a91ccc1d05bb35b88015289fc928ebabd935e5aacce6f0a6a586162253e65477f207988b83fdd3d950dd6");
	// k0 9 2048
	// bigint_t k0 	 = init("0x5f35b93a6a3fe3a4c24a943705532506878fd75b6e0d8a9fe6cf90b98b9432bd712cc18a838d329050585ccb02cf7c7f11a7f94fecb5ef51383d745aa2998c0228b92b3ca085599b8a7c3c8e2f5bd8f48449cd3eb652edbfcbb507025c83a6ac9f85bdc609440b5cd6a81974f2f4683af114418b913dec92e28c3af9124b09873f5cf3ec38ff01dcd2ee7da67701486e9f91da8c18ae44ffd6726ea7ed56a50fd932facbffab0647c035635a522263b0449fdbd98e96c3dd8e059d7159a8ba42107e62320e3233d2586849ad1bf4970ac451a0f4e31ca56232f775aeaedb90a28997e09ec5296f209b17f4460cd8430c10d044103fe3ad1f2e23fd276b534214");
	// k0 10 2048
	// bigint_t k0 	 = init("0xa762c8e7ca2644cb1f83eeab33a84fd5a34fa9c5aecfb1bb7022ba1c4b0918ccd98e48d13684bdaaecdfc2a0ac60d346d006d4b739eb671c2bb16ea439b53e289b2106ff749f6aec6f6aac8bffaadfc09bc72092d8032b8a248abe0333dcecf24dbe9a8e8acea13ee105469cef59dc5af3344d2b93db5530a5a3251ee448613a295bc6b1f16ff7fb6592ede0c42dd205b98d80ab7cd6baebcc06e029da0cf7f96bb2f26aa8b25107343820e9989c12c3430659b1248e8a8b6e701793d808cf2cecc3bb1e2b188c84f8aca78f195f672c7567d96cc213521d5b3eb1797123eecf5d3258e2b75bdc1a42f47ac98c3c86670a150f895836c6c61dee56dfe9b2e3da");

	//vi 0 128
	bigint_t vi_M	= init("0x02bbe7e3a87bff652d8c132e4179d9f8");
							
	//vf 0 128
	bigint_t vf_M	= init("0x987641e5532d7be197729a00e4aa88f4");

	// xil_printf("\r\n");
	// print_to_stdout(&modulus);
	// xil_printf("\r\n");
	// print_to_stdout(&public);
	// xil_printf("\r\n");
	// print_to_stdout(&private);
	// xil_printf("\r\n");
	// print_to_stdout(&k0);
	// xil_printf("\r\n");

	vi_M = MM_big(vi_M,vi_M,modulus,INT_SIZE+2); 
	vf_M = MM_big(vf_M,vf_M,modulus,INT_SIZE+2); 

	message = rand_b();
	// printf("MSG: ");
	print_to_stdout(&message);
	xil_printf("\r\n");


	message = ME_big(public,message,modulus,k0,INT_SIZE+2);


	// printf("CYT: ");
	print_to_stdout(&message);
	xil_printf("\r\n");

	// printf("CTB: ");
	// print_to_stdout(&message_blind);
	// xil_printf("\r\n");

	message_blind = MM_big(message,vi_M,modulus,INT_SIZE+2);
	print_to_stdout(&message_blind);
	xil_printf("\r\n");
	message_blind = ME_big(private,message_blind,modulus,k0,INT_SIZE+2);
	print_to_stdout(&message_blind);
	xil_printf("\r\n");
	message_blind = MM_big(message_blind,vf_M,modulus,INT_SIZE+2);
	print_to_stdout(&message_blind);
	xil_printf("\r\n");

	message = ME_big(private,message,modulus,k0,INT_SIZE+2);

	print_to_stdout(&message);
	xil_printf("\r\n");
	print_to_stdout(&message_blind);
	xil_printf("\r\n");


	srand(949);


	bigint_t enc;

	XGpioPs_Config ledcfg = {XPAR_PS7_GPIO_0_DEVICE_ID, XPAR_PS7_GPIO_0_BASEADDR};
	XGpioPs led;

	XGpioPs_CfgInitialize(&led, &ledcfg, ledcfg.BaseAddr); // initialize the driver device
	XGpioPs_SetOutputEnablePin(&led, 7, 0b1); // set MIO7 as output
	XGpioPs_SetDirectionPin(&led, 7, 0b1); // set MIO7 tristate as output (idk if both are required)
	XGpioPs_IntrDisablePin(&led, 7); // disable interrupts on MIO7, (just in case)

	XGpioPs_WritePin(&led, 7, 0b0); // set MIO7 low

	for (i = 0; i < 100000; i++ ) {
		message = rand_b();
		XTime_GetTime(&tStart);

		//BLINDING
		#if BLINDING == 1
		message_blind = MM_big(vi_M, message, modulus, INT_SIZE+2);
		#endif

		//ENCRIPTION
		#if BLINDING == 1
		enc = ME_big(private, message_blind, modulus, k0, INT_SIZE+2);
		#else
		enc = ME_big(private, message, modulus, k0, INT_SIZE+2);
		#endif
		
		//BLINDING CORRECTION
		#if BLINDING == 1
		enc = MM_big(vf_M, enc, modulus, INT_SIZE+2);
		#endif

		XTime_GetTime(&tEnd);
		delta = tEnd - tStart;

		//BLINDING squaring
		// #if BLINDING == 1
		vi_M = MM_big(vi_M, vi_M, modulus, INT_SIZE+2);
		vf_M = MM_big(vf_M, vf_M, modulus, INT_SIZE+2);
		// #endif

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

		// Res = f_write(&f_cypher, (const void*)enc.numb, INT_SIZE/8, NULL);
		// if (Res) {
		// 	xil_printf("Error failed to write cypher text\n\r");
		// 	return XST_FAILURE;
		// }


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

	XGpioPs_WritePin(&led, 7, 0b1); // set MIO7 high

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
