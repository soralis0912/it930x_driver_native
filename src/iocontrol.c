/*++
Copyright (c) 2006-2009 ITEtech Corporation. All rights reserved.

Module Name:
    iocontrol.cpp

Abstract:
    Demodulator IOCTL Query and Set functions
--*/
#include "it930x-core.h"
#include <linux/string.h>

/***************************************************************************/
/************************ Jacky Han Insertion Start ************************/
/***************************************************************************/
RECPX4PX5PX6_FREQUENCY_PARAMETER Recpx4px5px6IsdbsFrequencyConvertTableArray[] =
{
    /* ch.1 */
    {   0,  0  },    // 151
    {   0,  1  },    // 161
    {   0,  2  },    // 171
    /* ch.3 */
    {   1,  0  },    // 191
    {   1,  1  },    // 103
    {   1,  2  },    // 256
    /* ch.5 */
    {   2,  0  },    // 192
    {   2,  1  },    // 193
    /* ch.7 */
    {   3,  0  },    // 451
    {   3,  1  },    // 471
    {   3,  2  },    // 441
    /* ch.9 */
    {   4,  0  },    // 211
    {   4,  1  },    // 200
    {   4,  2  },    // 222
    /* ch.11 */
    {   5,  0  },    // 241
    {   5,  1  },    // 231
    {   5,  1  },    // 232
    {   5,  1  },    // 531
    /* ch.13 */
    {   6,  0  },    // 141
    {   6,  1  },    // 181
    {   6,  2  },    // 236
    /* ch.15 */
    {   7,  0  },    // 101
    {   7,  0  },    // 102
    {   7,  1  },    // 201
    {   7,  1  },    // 202
    /* ch.17 */
    {   8,  0  },    // 401
    {   8,  1  },    // 461
    {   8,  2  },    // 481
    /* ch.19 */
    {   9,  0  },    // 234
    {   9,  1  },    // 242
    {   9,  2  },    // 243
    /* ch.21 */
    {  10,  0  },    // 252
    {  10,  1  },    // 244
    {  10,  2  },    // 245
    /* ch.23 */
    {  11,  0  },    // 251
    {  11,  1  },    // 255
    {  11,  2  },    // 258
    /* CS */
    {  12,  0  },  // CS2
    {  13,  0  },  // CS4
    {  14,  0  },  // CS6
    {  15,  0  },  // CS8
    {  16,  0  },  // CS10
    {  17,  0  },  // CS12
    {  18,  0  },  // CS14
    {  19,  0  },  // CS16
    {  20,  0  },  // CS18
    {  21,  0  },  // CS20
    {  22,  0  },  // CS22
    {  23,  0  }  // CS24
};
RECPX4PX5PX6_ISDB_FREQUENCY_TABLE Recpx4px5px6IsdbFrequencyTableArray[] = 
{
    /* ch.1 */
    {   11727480, 34500, 0x4010},  /* 151ch：BS朝日 */
    {   11727480, 34500, 0x4011},  /* 161ch：BS-TBS */
    {   11727480, 34500, 0x4012},  /* 171ch：BSテレ東 */
    /* ch.3 */
    {   11765840, 34500, 0x4030},  /* 191ch：WOWOW prime */
    {   11765840, 34500, 0x4031},  /* 103ch：NHK-BSプレミアム*/
    {   11765840, 34500, 0x4632},  /* 256ch：ディズニー・チャンネル */
    /* ch.5 */
    {   11804200, 34500, 0x4450},  /* 192ch：WOWOWライブ */
    {   11804200, 34500, 0x4451},  /* 193ch：WOWOWシネマ */
    /* ch.7 */
    {   11842560, 34500, 0x4470},  /* 451ch：BS朝日4K */
    {   11842560, 34500, 0x4671},  /* 471ch：BSジャパン 4K */
    {   11842560, 34500, 0x4672},  /* 441ch：BS日テレ 4K */
    /* ch.9 */
    {   11880920, 34500, 0x4090},  /* 211ch：BS11デジタル */
    {   11880920, 34500, 0x4091},  /* 200ch：スター・チャンネル1 */
    {   11880920, 34500, 0x4092},  /* 222ch：TwellV */
    /* ch.11 */
    {   11919280, 34500, 0x46b1},  /* 241ch：BSスカパー！ */
    {   11919280, 34500, 0x46b2},  /* 231ch：放送大学ex */
    {   11919280, 34500, 0x46b2},  /* 232ch：放送大学on */
    {   11919280, 34500, 0x46b2},  /* 531ch：放送大学ラジオ */
    /* ch.13 */
    {   11957640, 34500, 0x40d0},  /* 141ch：BS日テレ */
    {   11957640, 34500, 0x40d1},  /* 181ch：BSフジ */
    {   11957640, 34500, 0x46d2},  /* 236ch：BSアニマックス */
    /* ch.15 */
    {   11996000, 34500, 0x40f1},  /* 101ch：NHK-BS1 */
    {   11996000, 34500, 0x40f1},  /* 102ch：NHK-BS1臨時 */
    {   11996000, 34500, 0x40f2},  /* 201ch：スターチャンネル2 */
    {   11996000, 34500, 0x40f2},  /* 202ch：スターチャンネル3 */
    /* ch.17 */
    {   12034360, 34500, 0x4310},  /* 401ch：NHK SHV 4K */
    {   12034360, 34500, 0x4311},  /* 461ch：BS-TBS 4K */
    {   12034360, 34500, 0x4312},  /* 481ch：BSフジ 4K */   /* fixme 20201124 */ 
    /* ch.19 */
    {   12072720, 34500, 0x4730},  /* 234ch：グリーンチャンネル */
    {   12072720, 34500, 0x4731},  /* 242ch：J SPORTS 1 */
    {   12072720, 34500, 0x4732},  /* 243ch：J SPORTS 2 */
    /* ch.21 */
    {  12111080, 34500, 0x4750},  /* 252ch：IMAGICA BS */
    {  12111080, 34500, 0x4751},  /* 244ch：J SPORTS 3 */
    {  12111080, 34500, 0x4752},  /* 245ch：J SPORTS 4 */
    /* ch.23 */
    {  12149440, 34500, 0x4770},  /* 251ch：BS釣りビジョン */
    {  12149440, 34500, 0x4771},  /* 255ch：日本映画専門チャンネル */
    {  12149440, 34500, 0x4772},  /* 258ch：D-Life */
	/* CS */
    {  12291000, 34500, 0x6020 },  /* ND2： CS2
                                          * 237ch：スター・チャンネル プラス

                                          * 239ch：日本映画専門チャンネルHD
                                          * 306ch：フジテレビNEXT */
    {  12331000, 34500, 0x7040 },  /* ND4： CS4
                                          * 100ch：e2プロモ

                                          * 256ch：J sports ESPN
                                          * 312ch：FOX
                                          * 322ch：スペースシャワーTV
                                          * 331ch：カートゥーンネットワーク

                                          * 294ch：ホームドラマチャンネル
                                          * 334ch：トゥーン・ディズニー */
    {  12371000, 34500, 0x7060 },  /* ND6： CS6
                                          * 221ch：東映チャンネル
                                          * 222ch：衛星劇場

                                          * 223ch：チャンネルNECO
                                          * 224ch：洋画★シネフィル・イマジカ
                                          * 292ch：時代劇専門チャンネル

                                          * 238ch：スター・チャンネル クラシック

                                          * 310ch：スーパー！ドラマTV
                                          * 311ch：AXN
                                          * 250ch：スカイ・Asports＋ */
    {  12411000, 34500, 0x6080 },  /* ND8： CS8
                                          * 055ch：ショップ チャンネル

                                          * 335ch：キッズステーションHD */
    {  12451000, 34500, 0x60a0 }, /* ND10： CS10
                                          * 228ch：ザ・シネマ
                                          * 800ch：スカチャンHD800
                                          * 801ch：スカチャン801
                                          * 802ch：スカチャン802 */
    {  12491000, 34500, 0x70c0 }, /* ND12： CS12
                                          * 260ch：ザ・ゴルフ・チャンネル
                                          * 303ch：テレ朝チャンネル

                                          * 323ch：MTV 324ch：大人の音楽専門TV◆ミュージック・エア
                                          * 352ch：朝日ニュースター

                                          * 353ch：BBCワールドニュース
                                          * 354ch：CNNj
                                          * 110ch：ワンテンポータル */
    {  12531000, 34500, 0x70e0 }, /* ND14： CS14
                                          * 251ch：J sports 1
                                          * 252ch：J sports 2
                                          * 253ch：J sports Plus
                                          * 254ch：GAORA */
    {  12571000, 34500, 0x7100 }, /* ND16： CS16
                                          * 305ch：チャンネル銀河

                                          * 333ch：アニメシアターX(AT-X)
                                          * 342ch：ヒストリーチャンネル

                                          * 290ch：TAKARAZUKA SKYSTAGE
                                          * 803ch：スカチャン803
                                          * 804ch：スカチャン804 */
    {  12611000, 34500, 0x7120 }, /* ND18： CS18
                                          * 240ch：ムービープラスHD
                                          * 262ch：ゴルフネットワーク
                                          * 314ch：LaLa HDHV */
    {  12651000, 34500, 0x7140 }, /* ND20： CS20
                                          * 258ch：フジテレビONE
                                          * 302ch：フジテレビTWO
                                          * 332ch：アニマックス

                                          * 340ch：ディスカバリーチャンネル

                                          * 341ch：アニマルプラネット */
    {  12691000, 34500, 0x7160 }, /* ND22：

                                          * 160ch：C-TBSウェルカムチャンネル
                                          * 161ch：QVC
                                          * 185ch：プライム365.TV
                                          * 293ch：ファミリー劇場
                                          * 301ch：TBSチャンネル

                                          * 304ch：ディズニー・チャンネル
                                          * 325ch：MUSIC ON! TV
                                          * 351ch：TBSニュースバード

                                          * 343ch：ナショナルジオグラフィックチャンネル */
    {  12731000, 34500, 0x7180 }, /* ND24： CS24
                                          * 257ch：日テレG+ HD
                                          * 291ch：fashiontv
                                          * 300ch：日テレプラス

                                          * 315ch：FOXプラス

                                          * 321ch：MusicJapan TV
                                          * 350ch：日テレNEWS24 
                                          * 362ch：旅チャンネル*/
    {   93143, 6000, 0xFFFF },  /*   1 */
	{   99143, 6000, 0xFFFF },  /*   2 */
    {  105143, 6000, 0xFFFF },  /*   3 */ 
	{  111143, 6000, 0xFFFF },  /* C13 */
    {  117143, 6000, 0xFFFF },  /* C14 */ 
	{  123143, 6000, 0xFFFF },  /* C15 */
    {  129143, 6000, 0xFFFF },  /* C16 */ 
	{  135143, 6000, 0xFFFF },  /* C17 */
    {  141143, 6000, 0xFFFF },  /* C18 */ 
	{  147143, 6000, 0xFFFF },  /* C19 */
    {  153143, 6000, 0xFFFF },  /* C20 */ 
	{  159143, 6000, 0xFFFF },  /* C21 */
    {  167143, 6000, 0xFFFF },  /* C22 */
	{  173143, 6000, 0xFFFF },  /*   4 */
    {  179143, 6000, 0xFFFF },  /*   5 */ 
	{  185143, 6000, 0xFFFF },  /*   6 */
    {  191143, 6000, 0xFFFF },  /*   7 */ 
	{  195143, 6000, 0xFFFF },  /*   8 */
    {  201143, 6000, 0xFFFF },  /*   9 */ 
	{  207143, 6000, 0xFFFF },  /*  10 */
    {  213143, 6000, 0xFFFF },  /*  11 */ 
	{  219143, 6000, 0xFFFF },  /*  12 */
    {  225143, 6000, 0xFFFF },  /* C23 */ 
	{  231143, 6000, 0xFFFF },  /* C24 */
    {  237143, 6000, 0xFFFF },  /* C25 */ 
	{  243143, 6000, 0xFFFF },  /* C26 */
    {  249143, 6000, 0xFFFF },  /* C27 */
	{  255143, 6000, 0xFFFF },  /* C28 */
    {  261143, 6000, 0xFFFF },  /* C29 */ 
	{  267143, 6000, 0xFFFF },  /* C30 */
    {  273143, 6000, 0xFFFF },  /* C31 */ 
	{  279143, 6000, 0xFFFF },  /* C32 */
    {  285143, 6000, 0xFFFF },  /* C33 */ 
	{  291143, 6000, 0xFFFF },  /* C34 */
    {  297143, 6000, 0xFFFF },  /* C35 */ 
	{  303143, 6000, 0xFFFF },  /* C36 */
    {  309143, 6000, 0xFFFF },  /* C37 */ 
	{  315143, 6000, 0xFFFF },  /* C38 */
    {  321143, 6000, 0xFFFF },  /* C39 */ 
	{  327143, 6000, 0xFFFF },  /* C40 */
    {  333143, 6000, 0xFFFF },  /* C41 */ 
	{  339143, 6000, 0xFFFF },  /* C42 */
    {  345143, 6000, 0xFFFF },  /* C43 */ 
	{  351143, 6000, 0xFFFF },  /* C44 */
    {  357143, 6000, 0xFFFF },  /* C45 */ 
	{  363143, 6000, 0xFFFF },  /* C46 */
    {  369143, 6000, 0xFFFF },  /* C47 */ 
	{  375143, 6000, 0xFFFF },  /* C48 */
    {  381143, 6000, 0xFFFF },  /* C49 */ 
	{  387143, 6000, 0xFFFF },  /* C50 */
    {  393143, 6000, 0xFFFF },  /* C51 */ 
	{  399143, 6000, 0xFFFF },  /* C52 */
    {  405143, 6000, 0xFFFF },  /* C53 */ 
	{  411143, 6000, 0xFFFF },  /* C54 */
    {  417143, 6000, 0xFFFF },  /* C55 */ 
	{  423143, 6000, 0xFFFF },  /* C56 */
    {  429143, 6000, 0xFFFF },  /* C57 */ 
	{  435143, 6000, 0xFFFF },  /* C58 */
    {  441143, 6000, 0xFFFF },  /* C59 */ 
	{  447143, 6000, 0xFFFF },  /* C60 */
    {  453143, 6000, 0xFFFF },  /* C61 */ 
	{  459143, 6000, 0xFFFF },  /* C62 */
    {  465143, 6000, 0xFFFF },  /* C63 */ 
	{  473143, 6000, 0xFFFF },  /*  13 */
    {  479143, 6000, 0xFFFF },  /*  14 */ 
	{  485143, 6000, 0xFFFF },  /*  15 */
    {  491143, 6000, 0xFFFF },  /*  16 */ 
	{  497143, 6000, 0xFFFF },  /*  17 */
    {  503143, 6000, 0xFFFF },  /*  18 */ 
	{  509143, 6000, 0xFFFF },  /*  19 */
    {  515143, 6000, 0xFFFF },  /*  20 */ 
	{  521143, 6000, 0xFFFF },  /*  21 */ 
    {  527143, 6000, 0xFFFF },  /*  22 */  
	{  533143, 6000, 0xFFFF },  /*  23 */ 
    {  539143, 6000, 0xFFFF },  /*  24 */  
	{  545143, 6000, 0xFFFF },  /*  25 */ 
    {  551143, 6000, 0xFFFF },  /*  26 */  
	{  557143, 6000, 0xFFFF },  /*  27 */ 
    {  563143, 6000, 0xFFFF },  /*  28 */  
	{  569143, 6000, 0xFFFF },  /*  29 */ 
    {  575143, 6000, 0xFFFF },  /*  30 */  
	{  581143, 6000, 0xFFFF },  /*  31 */ 
    {  587143, 6000, 0xFFFF },  /*  32 */  
	{  593143, 6000, 0xFFFF },  /*  33 */ 
    {  599143, 6000, 0xFFFF },  /*  34 */  
	{  605143, 6000, 0xFFFF },  /*  35 */ 
    {  611143, 6000, 0xFFFF },  /*  36 */  
	{  617143, 6000, 0xFFFF },  /*  37 */ 
    {  623143, 6000, 0xFFFF },  /*  38 */  
	{  629143, 6000, 0xFFFF },  /*  39 */ 
    {  635143, 6000, 0xFFFF },  /*  40 */  
	{  641143, 6000, 0xFFFF },  /*  41 */ 
    {  647143, 6000, 0xFFFF },  /*  42 */  
	{  653143, 6000, 0xFFFF },  /*  43 */ 
    {  659143, 6000, 0xFFFF },  /*  44 */  
	{  665143, 6000, 0xFFFF },  /*  45 */ 
    {  671143, 6000, 0xFFFF },  /*  46 */  
	{  677143, 6000, 0xFFFF },  /*  47 */ 
    {  683143, 6000, 0xFFFF },  /*  48 */  
	{  689143, 6000, 0xFFFF },  /*  49 */ 
    {  695143, 6000, 0xFFFF },  /*  50 */  
	{  701143, 6000, 0xFFFF },  /*  51 */ 
    {  707143, 6000, 0xFFFF },  /*  52 */  
	{  713143, 6000, 0xFFFF },  /*  53 */ 
    {  719143, 6000, 0xFFFF },  /*  54 */  
	{  725143, 6000, 0xFFFF },  /*  55 */ 
    {  731143, 6000, 0xFFFF },  /*  56 */  
	{  737143, 6000, 0xFFFF },  /*  57 */ 
    {  743143, 6000, 0xFFFF },  /*  58 */  
	{  749143, 6000, 0xFFFF },  /*  59 */ 
    {  755143, 6000, 0xFFFF },  /*  60 */  
	{  761143, 6000, 0xFFFF },  /*  61 */ 
    {  767143, 6000, 0xFFFF }   /*  62 */ 
};

unsigned short FindFrequencyTableIndexForISDBS(RECPX4PX5PX6_FREQUENCY_PARAMETER FrequencyParameter)
{
	unsigned short MaxTableNumber;
	unsigned short Loop;

	MaxTableNumber = (sizeof(Recpx4px5px6IsdbsFrequencyConvertTableArray) / sizeof(RECPX4PX5PX6_FREQUENCY_PARAMETER));

	for(Loop=0;Loop<MaxTableNumber;Loop++)
	{
        if(FrequencyParameter.frequencyno == Recpx4px5px6IsdbsFrequencyConvertTableArray[Loop].frequencyno &&
		   FrequencyParameter.slot == Recpx4px5px6IsdbsFrequencyConvertTableArray[Loop].slot)
		   return Loop;
	}

    return 0xFFFF;
}
/***************************************************************************/
/************************* Jacky Han Insertion End *************************/
/***************************************************************************/

/*****************************************************************************
*
*  Function:   ioctrol_IT9300_GPIO
*
*  Arguments:  pdev             	- The device struct for get the handle.
* 			   iGPIOH				- The index of GPIOH 
*              byInOutput			- Input (0)/ Output (1) setting 
*              byValue				- Output Value Low (0) / High (1)
*
*  Returns:    Error_NO_ERROR: successful, non-zero error code otherwise.
*
*  Notes:
*
*****************************************************************************/
Dword ioctrol_IT9300_GPIO(IT930x_Device* pdev, int iGPIOH, Byte byInOutput, Byte byValue)
{
	Dword dwError = Error_NO_ERROR;
	Dword p_br_reg_top_gpiohx_en = 0;
	Dword p_br_reg_top_gpiohx_on = 0;
	Dword p_br_reg_top_gpiohx_o = 0;
	int chip = 0;
	
	switch(iGPIOH)
	{
		case 1:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh1_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh1_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh1_o;
			break;
		case 2:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh2_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh2_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh2_o;
			break;
		case 3:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh3_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh3_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh3_o;
			break;
		case 4:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh4_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh4_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh4_o;
			break;
		case 5:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh5_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh5_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh5_o;
			break;
		case 6:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh6_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh6_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh6_o;
			break;
		case 7:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh7_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh7_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh7_o;
			break;
		case 8:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh8_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh8_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh8_o;
			break;
		case 9:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh9_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh9_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh9_o;
			break;
		case 10:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh10_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh10_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh10_o;
			break;	
		case 11:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh11_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh11_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh11_o;
			break;	
		case 12:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh12_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh12_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh12_o;
			break;	
		case 13:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh13_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh13_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh13_o;
			break;	
		case 14:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh14_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh14_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh14_o;
			break;	
		case 15:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh15_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh15_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh15_o;
			break;	
		case 16:
			p_br_reg_top_gpiohx_en = p_br_reg_top_gpioh16_en;
			p_br_reg_top_gpiohx_on = p_br_reg_top_gpioh16_on;
			p_br_reg_top_gpiohx_o = p_br_reg_top_gpioh16_o;
			break;		
		default:
			dwError = Error_NOT_SUPPORT;
			goto GPIOFAIL;					
	}
	
	dwError = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpiohx_en, 0x01);
	if (dwError) goto GPIOFAIL;
	dwError = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpiohx_on, byInOutput);
	if (dwError) goto GPIOFAIL;
	dwError = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpiohx_o, byValue);
	if (dwError) goto GPIOFAIL;

GPIOFAIL:	
	return dwError;
}
/*****************************************************************************
*
*  Function:   DemodIOCTLFun
*
*  Arguments:  dev             			- The device struct for get the handle.
*              IOCTLCode               - Device IO control code
*              pIOBuffer               - buffer containing data for the IOCTL
* 				 number					- Chip_Index
*
*  Returns:    Error_NO_ERROR: successful, non-zero error code otherwise.
*
*  Notes:
*
*****************************************************************************/
// IRQL:DISPATCH_LEVEL
Dword DemodIOCTLFun(
    IN void *			dev,
    IN Dword        IOCTLCode,
    IN unsigned long        pIOBuffer,
    IN Byte 		number)
{
    Dword error = Error_NO_ERROR;
    int br_idx = 0, ts_idx = 0;
    int i = 0;
	IT930x_Device* pdev = (IT930x_Device*) dev;
	
    switch (IOCTLCode) 
	{
		case IOCTL_ITE_DEMOD_ACQUIRECHANNEL:
		{			
			AcquireChannelRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;
			int br_idx = 0, ts_idx = 0;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			deb_data("br_idx: %d   tsSrcIdx: %u\n", br_idx, ts_idx);
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(AcquireChannelRequest)))
				return -EFAULT;
			
			if (Request.frequency == 0)
				break;
			
			deb_data("[iocontrol] bandwith = %d\n", Request.bandwidth);
						
			if ((pdev->DC->chip_Type[br_idx][ts_idx] != EEPROM_MXL691) ||
				(pdev->DC->chip_Type[br_idx][ts_idx] != EEPROM_MXL691_DUALP) ||
				(pdev->DC->chip_Type[br_idx][ts_idx] != EEPROM_MXL692) ||
				(pdev->DC->chip_Type[br_idx][ts_idx] != EEPROM_MXL248)) {
				if (Request.bandwidth == 0)
					break;
			}
							
//			if (!pdev->chip[number]->if_chip_start_capture) 
			{			
				Request.error = DL_Demodulator_acquireChannel(pdev->DC, &Request, br_idx, ts_idx);
				
				if (!Request.error)
					printk("set Mode =%d, Freq = %d, BW = %d, success\n", Request.mode, Request.frequency, Request.bandwidth);
				else
					printk("set Mode =%d, Freq = %d, BW = %d, fail = 0x%x\n", Request.mode, Request.frequency, Request.bandwidth, Request.error);
			}
/*
			else 
			{			
				Request.error = 2;
			}
*/
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(AcquireChannelRequest)))
				return -EFAULT;
					
			return Request.error;
		}
#if 1
        case IOCTL_ITE_DEMOD_ISLOCKED:
        {
			IsLockedRequest Request; 
			IT930x_Device* pdev = (IT930x_Device*) dev;
			int br_idx = 0, ts_idx = 0;
			Bool islocked = 0;
						
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			Request.error = DL_Demodulator_isLocked(pdev->DC, &islocked, br_idx, ts_idx);
			Request.locked = islocked;
						
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(IsLockedRequest)))
				return -EFAULT;
				
    		break;
        }
#endif
        case IOCTL_ITE_DEMOD_GETCHANNELSTATISTIC:
        {
			GetChannelStatisticRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;
			int br_idx = 0, ts_idx = 0;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4	
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			Request.error = DL_Demodulator_getChannelStatistic (pdev->DC, ts_idx, &Request.channelStatistic, br_idx, ts_idx);
			if (Request.error)
				printk("[IOCTL] - Get channel statistic fail...!!!\n");
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(GetChannelStatisticRequest)))
				return -EFAULT;
			
    		break;
        }
        case IOCTL_ITE_DEMOD_GETSTATISTIC:
        {
			GetStatisticRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;
			int br_idx = 0, ts_idx = 0;			

//printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DEMOD_GETSTATISTIC) memory address of pdev : 0x%x\n",(unsigned long)pdev);


			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

//printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DEMOD_GETSTATISTIC) br_idx : %d\n",br_idx);
//printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DEMOD_GETSTATISTIC) ts_idx : %d\n",ts_idx);
			
			Request.error = DL_Demodulator_getStatistic (pdev->DC, &Request.statistic, br_idx, ts_idx);
			if (Request.error)
				printk("[IOCTL] - Get statistic fail...!!!\n");
#ifdef	PATCH_FOR_NX
			if (Request.statistic.signalStrength < 100) {
					Request.statistic.signalStrength = 100;
					//deb_data(">>>>>>>>>> Strength change to  : %d\n\n", Request.statistic.signalStrength);	
			}
#endif
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(GetStatisticRequest)))
				return -EFAULT;
			
    		break;
        }
        case IOCTL_ITE_DEMOD_GETDRIVERINFO:
        {
			DemodDriverInfo DriverInfo;
			IT930x_Device* pdev = (IT930x_Device*) dev;
			Dword dwFWVersionLink = 0;
			Dword dwFWVersionOFDM = 0;
			int br_idx = 0, ts_idx = 0;			

			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
		
			DriverInfo.JackyDriverVersionNumber = JACKY_DRIVER_VERSION_NUMBER;                // Jacky Han Added
			
            strcpy((char *)DriverInfo.IteDriverVersion, DRIVER_RELEASE_VERSION);
			sprintf((char *)DriverInfo.APIVersion, "%x.%x.%x.%x", (Byte)(Version_NUMBER>>8), (Byte)(Version_NUMBER), Version_DATE, Version_BUILD);
			sprintf((char *)DriverInfo.FWVersionLink, "%d.%d.%d.%d", DVB_LL_VERSION1, DVB_LL_VERSION2, DVB_LL_VERSION3, DVB_LL_VERSION4);
			sprintf((char *)DriverInfo.FWVersionOFDM, "%d.%d.%d.%d", DVB_OFDM_VERSION1, DVB_OFDM_VERSION2, DVB_OFDM_VERSION3, DVB_OFDM_VERSION4);
			strcpy((char *)DriverInfo.Company, "ITEtech");
			strcpy((char *)DriverInfo.SupportHWInfo, "Endeavour");
			DriverInfo.error = Error_NO_ERROR;
			DriverInfo.BoardId = pdev->DC->board_id;
			DriverInfo.chip_Type = pdev->DC->chip_Type[br_idx][ts_idx];
			
			if (copy_to_user((void *)pIOBuffer, (void *)&DriverInfo, sizeof(DemodDriverInfo)))
				return -EFAULT;
			
    		break;
        }
        case IOCTL_ITE_DEMOD_WRITEREGISTERS:
        {
			RegistersRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;
			int br_idx = 0, ts_idx = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(RegistersRequest)))
				return -EFAULT;
			
			Request.chip = number;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			Request.error = DL_Demodulator_writeRegisters(pdev->DC, Request.processor, Request.registerAddress, Request.bufferLength, Request.buffer, br_idx, ts_idx);
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(RegistersRequest)))
				return -EFAULT;
			
			break;
		}
        case IOCTL_ITE_DEMOD_READREGISTERS:
        {
			RegistersRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;
			int br_idx = 0, ts_idx = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(RegistersRequest)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			Request.error = DL_Demodulator_readRegisters(pdev->DC, Request.processor, Request.registerAddress, Request.bufferLength, Request.buffer, br_idx, ts_idx);
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(RegistersRequest)))
				return -EFAULT;
			
			break;
		}
		case IOCTL_ITE_ENDEAVOUR_WRITEREGISTERS:
		{
			RegistersRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;			
			int br_idx = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(RegistersRequest)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4	
			
			mutex_lock(&pdev->DC->dev_mutex);
            //****************************************************
			//*********** Jacky Han Modification Start ***********
            //****************************************************
			//Request.error = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, br_idx, Request.registerAddress, *Request.buffer);
              Request.error = IT9300_writeRegisters((Endeavour*) &pdev->DC->it9300, br_idx, Request.registerAddress,Request.bufferLength, Request.buffer);
            //****************************************************
			//************ Jacky Han Modification End ************
            //****************************************************
			mutex_unlock(&pdev->DC->dev_mutex);
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(RegistersRequest)))
				return -EFAULT;
			
			break;
		}
		case IOCTL_ITE_ENDEAVOUR_READREGISTERS:
		{
			RegistersRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			int br_idx = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(RegistersRequest)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4	
			
			mutex_lock(&pdev->DC->dev_mutex);
            //****************************************************
			//*********** Jacky Han Modification Start ***********
            //****************************************************
			//Request.error = IT9300_readRegister((Endeavour*) &pdev->DC->it9300, br_idx, Request.registerAddress, Request.buffer);
              Request.error = IT9300_readRegisters((Endeavour*) &pdev->DC->it9300, br_idx, Request.registerAddress,Request.bufferLength, Request.buffer);
            //****************************************************
			//************ Jacky Han Modification End ************
            //****************************************************
			mutex_unlock(&pdev->DC->dev_mutex);
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(RegistersRequest)))
				return -EFAULT;
			
			break;
		}				
		case IOCTL_ITE_ENDEAVOUR_SETPIDFILTER:
		{
			PIDFilter Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;
			Word tsIndex[5];
			int br_idx = 0, ts_idx = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(PIDFilter)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			tsIndex[1] = p_br_mp2if_ts1_pid_index;
			tsIndex[2] = p_br_mp2if_ts2_pid_index;
			tsIndex[3] = p_br_mp2if_ts3_pid_index;
			tsIndex[4] = p_br_mp2if_ts4_pid_index;
			
			if (Request.tableLen != 0) {
				pdev->DC->it9300.tsSource[br_idx][ts_idx].tableLen    = Request.tableLen;
				pdev->DC->it9300.tsSource[br_idx][ts_idx].orgPidTable = Request.oldPID;
				pdev->DC->it9300.tsSource[br_idx][ts_idx].newPidTable = Request.newPID;
				//pdev->DC->endeavour.tsSource[0][number].tsPort      = (TsPort)(number+1);			
				
				deb_data("handle_index = %d\n",  number);
				
				mutex_lock(&pdev->DC->dev_mutex);
				Request.error = IT9300_enPidFilter((Endeavour*) &pdev->DC->it9300, br_idx, (Byte)ts_idx);
				mutex_unlock(&pdev->DC->dev_mutex);
			}
			else {					
				mutex_lock(&pdev->DC->dev_mutex);
				Request.error = IT9300_writeRegister ((Endeavour*) &pdev->DC->it9300, br_idx, p_br_mp2if_pid_index_en, 0);
				Request.error = IT9300_writeRegister ((Endeavour*) &pdev->DC->it9300, br_idx, tsIndex[ts_idx+1], 1);
				mutex_unlock(&pdev->DC->dev_mutex);
			}
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(PIDFilter)))
				return -EFAULT;
			
			break;
		}
		case IOCTL_ITE_ENDEAVOUR_SETUARTBAUDRATE:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;
			SetUartBaudrate Request;
			Dword dwBaudRate = 9600; //Default Baud Rate
			UartBaudRate baudrate = UART_BAUDRATE_9600; //Default Baud Rate

			if (copy_from_user((void *)&dwBaudRate, (void *)pIOBuffer, sizeof(unsigned long))) {
				dwBaudRate = *((unsigned long *)pIOBuffer);	
				//return -EFAULT;
			}
			

			switch(dwBaudRate)
			{
				case 9600:
					baudrate = UART_BAUDRATE_9600;
					break;
				case 19200:
					baudrate = UART_BAUDRATE_19200;
					break;
				case 38400:
					baudrate = UART_BAUDRATE_38400;
					break;
				case 57600:
					baudrate = UART_BAUDRATE_57600;
					break;
				default:
					baudrate = UART_BAUDRATE_9600;
					break;
			}
			
			mutex_lock(&pdev->DC->dev_mutex);			
				error = IT9300_setUARTBaudrate(&pdev->DC->it9300, 0, baudrate);	//0 = which endeavour
			mutex_unlock(&pdev->DC->dev_mutex);	
			
				if (error) 
				{
					printk("IT9300_setUARTBaudrate error!\n");
				}
				else
				{
					printk("IT9300_setUARTBaudrate OK!\n");
				}
			
			//copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(RegistersRequest));
			
			break;
		}
		case IOCTL_ITE_ENDEAVOUR_SENTUARTDATA:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;
			Byte ReturnChannelCommand[RETURNCHANNEL_PACKETLENGTH];
			Word RXDeviceID = 0xFFFF;
			Word TXDeviceID = 0xFFFF;
			SentUartData* pRequest;
			SentUartData Request;
			Dword dwLength = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(SentUartData))) {
				pRequest = (SentUartData*) pIOBuffer;
				//return -EFAULT;
			}
			else {
				pRequest = &Request;
			}
			
			if (pRequest->len > RETURNCHANNEL_PACKETLENGTH)
				dwLength = RETURNCHANNEL_PACKETLENGTH;
			else
				dwLength = pRequest->len;
			
			if (copy_from_user((void*)ReturnChannelCommand, (void*)pRequest->cmd, dwLength)) {
				memcpy(ReturnChannelCommand, (Byte*)pRequest->cmd, dwLength);
				//return -EFAULT;
			}
			
			RXDeviceID = ((ReturnChannelCommand[1] & 0xFF) << 8) + (ReturnChannelCommand[2] & 0xFF);
			TXDeviceID = ((ReturnChannelCommand[3] & 0xFF) << 8) + (ReturnChannelCommand[4] & 0xFF);
		
			if (ReturnChannelCommand[0] != '#') {
				printk("IOCTL_ITE_ENDEAVOUR_SENTUARTDATA: NO leading TAG !!!!\n");
				break;
			}			
			
			if(pdev->DC->board_id == 0x54) { //Multi-thread virtual com
				if (pdev->DC->rx_device_id[number] != RXDeviceID) {
					printk("IOCTL_ITE_ENDEAVOUR_SENTUARTDATA: RX DEVICE ERROR, SKIP !!!!\n");
					break;
				}			
			}else {
				if ((pdev->DC->rx_device_id[number] != RXDeviceID) && (TXDeviceID != 0xFFFF)) {
					printk("IOCTL_ITE_ENDEAVOUR_SENTUARTDATA: RX DEVICE ERROR, SKIP !!!!\n");
					break;
				}			
				
				if ((TXDeviceID == 0xFFFF) && (pdev->DC->rx_device_id[number] != pdev->DC->rx_device_id[0])) {	
					printk("IOCTL_ITE_ENDEAVOUR_SENTUARTDATA: Command Re-sent !!!! handle_number = %d\n", pdev->DC->rx_device_id[number]);
					break;
				}
			}
			
			mutex_lock(&pdev->DC->dev_mutex);
			if(pdev->DC->board_id == 0x54) 
			{ 
			    //Multi-thread virtual com
				//printk("IT9300_sentUARTData switch to [%d]!\n", number);
				
				switch(number)
				{
					case 0:
						error = ioctrol_IT9300_GPIO(pdev, 11, 1, 1);	
						if(error) goto SWITCHFAIL;
						error = ioctrol_IT9300_GPIO(pdev, 10, 1, 1);	
						if(error) goto SWITCHFAIL;
						break;
					case 1:
						error = ioctrol_IT9300_GPIO(pdev, 10, 1, 0);	
						if(error) goto SWITCHFAIL;
						error = ioctrol_IT9300_GPIO(pdev, 11, 1, 1);	
						if(error) goto SWITCHFAIL;
						break;
					case 2:
						error = ioctrol_IT9300_GPIO(pdev, 10, 1, 1);	
						if(error) goto SWITCHFAIL;
						error = ioctrol_IT9300_GPIO(pdev, 11, 1, 0);	
						if(error) goto SWITCHFAIL;
						break;
					case 3:
						error = ioctrol_IT9300_GPIO(pdev, 10, 1, 0);	
						if(error) goto SWITCHFAIL;
						error = ioctrol_IT9300_GPIO(pdev, 11, 1, 0);	
						if(error) goto SWITCHFAIL;
						break;
					default:
						goto SWITCHFAIL;
						
				}
				mdelay(10);
				
				
			}
			error = IT9300_sentUARTData(&pdev->DC->it9300, 0, dwLength, (Byte *)ReturnChannelCommand);
SWITCHFAIL:
			mutex_unlock(&pdev->DC->dev_mutex);
			
			if (error) {
				printk("IT9300_sentUARTData error!\n");
			}
			else {
				printk("IT9300_sentUARTData OK!\n");
			}			
			
			break;
		}
		case IOCTL_ITE_DEMOD_DEALWITH_RETURNCHANNELPID: // From virtual com
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;
			
			if (copy_from_user((void *)&pdev->DealWith_ReturnChannelPacket, (void *)pIOBuffer, sizeof(unsigned long))) {
				pdev->DealWith_ReturnChannelPacket = *((unsigned long *)pIOBuffer);
				//return -EFAULT;
			}
			
			if(pdev->DealWith_ReturnChannelPacket)
				deb_data("- Enter %s Function - dev->DealWith_ReturnChannelPacket == Open\n", __func__);
			else
				deb_data("- Enter %s Function - dev->DealWith_ReturnChannelPacket == Close\n", __func__);

			return 0;
		}
		case IOCTL_ITE_DEMOD_RESETPID:
		{
	//****************************************************
    //*********** Jacky Han Modification Start ***********
	//****************************************************
			
printk("(Jacky)(DemodIOCTLFun) IOCTL_ITE_DEMOD_RESETPID\n");

#if 0
			IT930x_Device* pdev = (IT930x_Device*) dev;
			ResetPidRequest Request;
			int br_idx = 0, ts_idx = 0;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4	
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			Request.chip = number;
			Request.error = DL_Demodulator_resetPidFilter(pdev->DC, br_idx, ts_idx);
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(ResetPidRequest)))
				return -EFAULT;
#else
			IT930x_Device* pdev = (IT930x_Device*) dev;
            Device_Context *pDC = (Device_Context*) pdev->DC;  
			ResetPidRequest Request;
	        Dword result = Error_NO_ERROR;
			int br_idx = 0, ts_idx = 0;
	
	        br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
	        ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pDC->dev_mutex);		
			   
	        result = IT9300_simplePidFilter_ResetPidTable((Endeavour *) &pDC->it9300, br_idx, ts_idx);

			mutex_unlock(&pDC->dev_mutex);

            Request.chip = number;
			Request.error = result;

			if(copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(ResetPidRequest)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_RESETPID]  - copy_to_user Failed !\n");

				return -EFAULT; 
			}			
#endif
	//****************************************************
    //************ Jacky Han Modification End ************
	//****************************************************
			
			break;
		}
		case IOCTL_ITE_DEMOD_CONTROLPIDFILTER:
		{
	//****************************************************
    //*********** Jacky Han Modification Start ***********
	//****************************************************
			
printk("(Jacky)(DemodIOCTLFun) IOCTL_ITE_DEMOD_CONTROLPIDFILTER\n");

#if 0
			IT930x_Device* pdev = (IT930x_Device*) dev;
			ControlPidFilterRequest Request;
			int br_idx = 0, ts_idx = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(ControlPidFilterRequest)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			Request.chip = number;
			Request.error = DL_Demodulator_controlPidFilter(pdev->DC, Request.control, br_idx, ts_idx);
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(ControlPidFilterRequest)))
				return -EFAULT;
			
#else
			IT930x_Device* pdev = (IT930x_Device*) dev;
            Device_Context *pDC = (Device_Context*) pdev->DC;  
            ControlPidFilterRequest Request;
	        Dword result = Error_NO_ERROR;
			int br_idx = 0, ts_idx = 0;
	
	        br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
	        ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			if(copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(ControlPidFilterRequest)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_CONTROLPIDFILTER]  - copy_from_user Failed !\n");

				return -EFAULT; 
			}						

	        if(Request.control) 
			{ 
			   mutex_lock(&pDC->dev_mutex);			

		       result = IT9300_simplePidFilter_SetMode((Endeavour *) &pDC->it9300, br_idx, ts_idx, PID_FILTER_MODE_PASS);

			   mutex_unlock(&pDC->dev_mutex);
			} 
			else 
			{
			   mutex_lock(&pDC->dev_mutex);	

			   result = IT9300_simplePidFilter_ResetPidTable((Endeavour *) &pDC->it9300, br_idx, ts_idx);
		
		       result = IT9300_simplePidFilter_SetMode((Endeavour *) &pDC->it9300, br_idx, ts_idx, PID_FILTER_MODE_DISABLE);

			   mutex_unlock(&pDC->dev_mutex);
			}

            Request.chip = number;
			Request.error = result;

			if(copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(ControlPidFilterRequest)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_CONTROLPIDFILTER]  - copy_to_user Failed !\n");

				return -EFAULT; 
			}			
#endif
	//****************************************************
    //************ Jacky Han Modification End ************
	//****************************************************

			break;
		}	
		case IOCTL_ITE_DEMOD_ADDPIDAT:
		{
	//****************************************************
    //*********** Jacky Han Modification Start ***********
	//****************************************************
			
printk("(Jacky)(DemodIOCTLFun) IOCTL_ITE_DEMOD_ADDPIDAT\n");

#if 0
			IT930x_Device* pdev = (IT930x_Device*) dev;
			AddPidAtRequest Request;
			int br_idx = 0, ts_idx = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(AddPidAtRequest)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			Request.chip = number;
			Request.error = DL_Demodulator_addPidToFilter(pdev->DC, Request.index, Request.pid.value, br_idx, ts_idx);
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(AddPidAtRequest)))
				return -EFAULT;
#else
			IT930x_Device* pdev = (IT930x_Device*) dev;
            Device_Context *pDC = (Device_Context*) pdev->DC;  
			AddPidAtRequest Request;
	        Dword result = Error_NO_ERROR;
			int br_idx = 0, ts_idx = 0;
	
	        br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
	        ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			if(copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(AddPidAtRequest)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_ADDPIDAT]  - copy_from_user Failed !\n");

				return -EFAULT; 
			}			

            mutex_lock(&pDC->dev_mutex);	


printk("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_ADDPIDAT] Request.pid.value : 0x%x\n",Request.pid.value);
printk("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_ADDPIDAT] Request.index : %d\n",Request.index);

            result = IT9300_simplePidFilter_AddPid((Endeavour *) &pDC->it9300, br_idx, ts_idx, Request.pid.value, Request.index);

printk("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_ADDPIDAT] result : 0x%x\n",result);

			mutex_unlock(&pDC->dev_mutex);


            Request.chip = number;
			Request.error = result;

			if(copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(AddPidAtRequest)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_ADDPIDAT]  - copy_to_user Failed !\n");

				return -EFAULT; 
			}
#endif
	//****************************************************
    //************ Jacky Han Modification End ************
	//****************************************************			

			break;
		}	
		case IOCTL_ITE_DEMOD_REMOVEPIDAT:
		{
	//****************************************************
    //*********** Jacky Han Modification Start ***********
	//****************************************************
			
printk("(Jacky)(DemodIOCTLFun) IOCTL_ITE_DEMOD_REMOVEPIDAT\n");

#if 0
			IT930x_Device* pdev = (IT930x_Device*) dev;
			RemovePidAtRequest Request;
			int br_idx = 0, ts_idx = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(RemovePidAtRequest)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			Request.chip = number;
			Request.error = DL_Demodulator_removePidFromFilter(pdev->DC, Request.index, Request.pid.value, br_idx, ts_idx);
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(RemovePidAtRequest)))
				return -EFAULT;
#else
			IT930x_Device* pdev = (IT930x_Device*) dev;
            Device_Context *pDC = (Device_Context*) pdev->DC;  
			RemovePidAtRequest Request;
	        Dword result = Error_NO_ERROR;
			int br_idx = 0, ts_idx = 0;
	
	        br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
	        ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			if(copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(RemovePidAtRequest)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_REMOVEPIDAT]  - copy_from_user Failed !\n");

				return -EFAULT; 
			}			

            mutex_lock(&pDC->dev_mutex);	

	        result = IT9300_simplePidFilter_RemovePid((Endeavour *) &pDC->it9300, br_idx, ts_idx, Request.index);

			mutex_unlock(&pDC->dev_mutex);

            Request.chip = number;
			Request.error = result;

			if(copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(RemovePidAtRequest)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DEMOD_REMOVEPIDAT]  - copy_to_user Failed !\n");

				return -EFAULT; 
			}			
#endif			
	//****************************************************
    //************ Jacky Han Modification End ************
	//****************************************************
			
			break;
		}
		case IOCTL_ITE_DEMOD_READEEPROMVALUES:
		{
			ReadEepromValuesRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;			
			int br_idx = 0, i, checksum = 0;
			Byte wBuffer[2];
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(ReadEepromValuesRequest)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4	
			
			//printk("addr = %d \n", pRequest->registerAddress);			
#if 1			
			/**********read 256 value(byte) from eeprom***********/
			wBuffer[0] = (Byte)Request.registerAddress;
			
			mutex_lock(&pdev->DC->dev_mutex);
			Request.error = IT9300_writeGenericRegisters((Endeavour*) &pdev->DC->it9300, br_idx, 2, 0xA0, 1, wBuffer);
			Request.error = IT9300_readGenericRegisters((Endeavour*) &pdev->DC->it9300, br_idx, 2, 0xA0, 1, Request.buffer);
			mutex_unlock(&pdev->DC->dev_mutex);
#endif
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(ReadEepromValuesRequest)))
				return -EFAULT;
			
			break;
		}
		case IOCTL_ITE_DEMOD_WRITEEEPROMVALUES:
		{
			WriteEepromValuesRequest Request;
			IT930x_Device* pdev = (IT930x_Device*) dev;			
			int br_idx = 0, i, checksum = 0;
			Byte wBuffer[2];
			Byte rBuffer[256];

			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(WriteEepromValuesRequest)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4				

			for(i = 0; i < 256; i++) {
				//deb_data ("[0x%02x] : 0x%02x\n", i, pRequest->buffer[i]);
				wBuffer[0] = (Byte)i;
				wBuffer[1] = Request.buffer[i];
				
				mutex_lock(&pdev->DC->dev_mutex);
				Request.error = IT9300_writeGenericRegisters((Endeavour*) &pdev->DC->it9300, br_idx, 2, 0xA0, 2, wBuffer);
				mutex_unlock(&pdev->DC->dev_mutex);

				if(Request.error) {
					deb_data("Write EEPROM fail!!!\n");
				}
				
				msleep(1);
			}			

			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(WriteEepromValuesRequest)))
				return -EFAULT;
			
			deb_data("Write EEPROM finesh!!!\n");				

			break;	
		}
		case IOCTL_ITE_DEMOD_GETBOARDINPUTPOWER:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;
			Word Request;			
			Byte value = 0, loop_flag;
			int br_idx = 0, ts_idx = 0;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			//error = DL_Demodulator_getBoardInputPower((Demodulator *)pdev->DC->it9300.tsSource[br_idx][ts_idx].htsDev, &value);
			error = DL_Demodulator_getBoardInputPower(pdev->DC, &value, br_idx, ts_idx);
			
			if(error) {
				deb_data("Get Board Input Power fail!!!\n");
				Request = 0;
				return error;
			}
				
			Request = value;			
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(Word)))
				return -EFAULT;
			
			return error;
		}
		case IOCTL_ITE_ENDEAVOUR_RXRESET:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;
			Word Request;			
			Byte value = 0;
			int br_idx = 0, ts_idx = 0;
			int chip = 0; //= br_chip
			int i = 0, j = 0;
			
			if (copy_from_user((void *)&Request, (void *)pIOBuffer, sizeof(Word)))
				return -EFAULT;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			//error = DL_Demodulator_getBoardInputPower((Demodulator *)pdev->DC->it9300.tsSource[br_idx][ts_idx].htsDev, &value);
			//error = DL_Demodulator_getBoardInputPower(pdev->DC, &value, br_idx, ts_idx);
			
			//deb_data("---------RESET RX..!!    %d\n", *pRequest);
			//return 0;
			
			//Reset 
			mutex_lock(&pdev->DC->dev_mutex);
			



 	        //****************************************************
            //************* Jacky Han Insertion Start ************
	        //****************************************************
	        unsigned short VendorId;
	        unsigned short ProductId;

	        // Read Vendor ID
            IT9300_readRegisters((Endeavour*) &pdev->DC->it9300, 0, EEPROM_VENDOR_ID, 2, (Byte*)&VendorId);

//            printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_ENDEAVOUR_RXRESET) VendorId : 0x%x\n",VendorId);

	        // Read Product ID
	        IT9300_readRegisters((Endeavour*) &pdev->DC->it9300, 0, EEPROM_PRODUCT_ID, 2, (Byte*)&ProductId);

//            printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_ENDEAVOUR_RXRESET) ProductId : 0x%x\n",ProductId);
	        //****************************************************
            //************** Jacky Han Insertion End *************
	        //****************************************************


	        //****************************************************
            //*********** Jacky Han Modification Start ***********
	        //****************************************************
		    if(VendorId == USB_VID_DIGITALWARRIOR && 
		       (ProductId == USB_PID_DIGITALWARRIOR_PXW3U4 ||
			    ProductId == USB_PID_DIGITALWARRIOR_PXW3PE4 ||
				ProductId == USB_PID_DIGITALWARRIOR_PXW3PE5 ||
		   	    ProductId == USB_PID_DIGITALWARRIOR_PXQ3U4 ||
			    ProductId == USB_PID_DIGITALWARRIOR_PXQ3PE4 ||
                ProductId == USB_PID_DIGITALWARRIOR_PXQ3PE5 ||
				ProductId == USB_PID_DIGITALWARRIOR_PXMLT5PE ||
        		ProductId == USB_PID_DIGITALWARRIOR_PXMLT5U ||
        		ProductId == USB_PID_DIGITALWARRIOR_PXMLT8PE3 ||
        		ProductId == USB_PID_DIGITALWARRIOR_PXMLT8PE5 ||
        		ProductId == USB_PID_DIGITALWARRIOR_PXMLT8UT ||
        		ProductId == USB_PID_DIGITALWARRIOR_PXMLT8UTS))
	        {
               // Do Nothing
	        }
	        else
	        {
			   error = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpioh2_en, 0x01);
			   if (error) goto resetfail;
			   error = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpioh2_on, 0x01);
			   if (error) goto resetfail;
			   //error = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpioh2_o, 0x1);
		   	   //if (error) goto exit;
			   error = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpioh2_o, 0x0);
			   if (error) goto resetfail;

			   //BrUser_delay((Endeavour*) &pdev->DC->it9300, 200);
			   mdelay(200);
			   error = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpioh2_o, 0x1);
			   if (error) goto resetfail;
			   //BrUser_delay((Endeavour*) &pdev->DC->it9300, 200);
			   mdelay(200);

			   //BrUser_delay((Endeavour*) &pdev->DC->it9300, 100);
			   mdelay(200);
			   error = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpioh2_o, 0x0);
			   if (error) goto resetfail;
			   //BrUser_delay((Endeavour*) &pdev->DC->it9300, 100);
			   mdelay(200);
			   error = IT9300_writeRegister((Endeavour*) &pdev->DC->it9300, chip, p_br_reg_top_gpioh2_o, 0x1);
			   if (error) goto resetfail;	
			}
	        //****************************************************
            //************ Jacky Han Modification End ************
	        //****************************************************
			
			for(i = 0; i < pdev->DC->it9300_Number; i++) 
			{
				for(j = 0; j < pdev->DC->it9300.receiver_chip_number[i]; j++) 
				{
					switch (pdev->DC->chip_Type[i][j])
					{
					        case EEPROM_IT913X:
						         error = DRV_IT913x_Initialize(pdev->DC, (Byte)i, j);
						         break;

					        case EEPROM_Si2168B:
						         error = DRV_Si2168B_Initialize(pdev->DC, (Byte)i, j);
						         break;

					        case EEPROM_Dibcom9090:
						         error = DRV_Dib9090_Initialize(pdev->DC, EEPROM_Dibcom9090);
						         break;
						
					        case EEPROM_Dibcom9090_4chan:
						         error = DRV_Dib9090_Initialize(pdev->DC, EEPROM_Dibcom9090_4chan);
						         break;
						
					        case EEPROM_MXL691: //MXL69x ATSC, Sean
						         error = DRV_MXL69X_Initialize(pdev->DC, (Byte)i, j, EEPROM_MXL691);
						         break;
			
					        case EEPROM_MXL691_DUALP: 
						         error = DRV_MXL69X_Initialize(pdev->DC, (Byte)i, j, EEPROM_MXL691_DUALP);
						         break;
						
					        case EEPROM_MXL692: 
						         error = DRV_MXL69X_Initialize(pdev->DC, (Byte)i, j, EEPROM_MXL692);
						         break;
						
					        case EEPROM_MXL248: 
						         error = DRV_MXL69X_Initialize(pdev->DC, (Byte)i, j, EEPROM_MXL248);
						         break;
					
					        case EEPROM_CXD285X:
						         error = DRV_CXD285X_Initialize(pdev->DC, (Byte)i, j, EEPROM_CXD285X);
						         break;
						
	                        //****************************************************
                            //************* Jacky Han Insertion Start ************
	                        //****************************************************
		                    case EEPROM_PXW3U4 :
		                    case EEPROM_PXW3PE4 :
		                    case EEPROM_PXW3PE5 :
		                    case EEPROM_PXQ3U4 :
		                    case EEPROM_PXQ3PE4 :
		                    case EEPROM_PXQ3PE5 :
		                    case EEPROM_PXMLT5PE :
		                    case EEPROM_PXMLT5U :
		                    case EEPROM_PXMLT8PE3 :
		                    case EEPROM_PXMLT8PE5 :
		                    case EEPROM_PXMLT8UT :
		                    case EEPROM_PXMLT8UTS :
                                 error = DRV_MyFrontend_Reset(pdev->DC,j);       // Jacky Han Inserted
						         break;                                          // Jacky Han Inserted
	                        //****************************************************
                            //************** Jacky Han Insertion End *************
	                        //****************************************************
						
					        default:
						         deb_data("Unknown chip type [%d]\n", pdev->DC->chip_Type[i][j]);
						         break;
					}
				}
			}
			
			mutex_unlock(&pdev->DC->dev_mutex);
resetfail:	
			if (error) 
			{
				deb_data("--------  RESET FAIL....[0x%lx]\n", error);
			}
			else 
			{
				deb_data("--------  RESET SUCCESS...!\n");
			}
			
			if (copy_to_user((void *)pIOBuffer, (void *)&Request, sizeof(Word)))
				return -EFAULT;
			
			return error;
		}
		case IOCTL_ITE_ENDEAVOUR_GETBOARDID:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;
			Byte byRequest = 0;

			byRequest = (Byte) pdev->DC->board_id;			
			
			if (copy_to_user((void *)pIOBuffer, (void *)&byRequest, sizeof(Byte)))
				return -EFAULT;
			
			break;
		}
		case IOCTL_ITE_ENDEAVOUR_GETBOARDID_KERNEL:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;
			Byte byRequest = 0;
			Byte* pRequest;

			pRequest = (Byte *)pIOBuffer;
			 
			byRequest = (Byte) pdev->DC->board_id;			
			deb_data("IOCTL_ITE_ENDEAVOUR_GETBOARDID_KERNEL = 0x%02x\n", byRequest);
			
			*pRequest = byRequest;
			
			break;
		}
		case IOCTL_ITE_DIVERSITY_GETDETAILDATA:
		{	
			IT930x_Device* pdev = (IT930x_Device*) dev;
			int br_idx = 0, ts_idx = 0;
			DibcomDiversityData* pRequest;
			
			pRequest = (DibcomDiversityData*) kmalloc(sizeof(DibcomDiversityData), GFP_KERNEL);			
			if(pRequest == NULL) {
				printk("[IOCTL: IOCTL_ITE_DIVERSITY_GETDETAILDATA] - kmalloc Fail!!\n");
				return -EFAULT; 
			}
			
			memset(pRequest, 0, sizeof(DibcomDiversityData));

			
			pRequest->chip = number;

			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
		

			pRequest->error = DL_Dib9090_getDiversityDetailData(pdev->DC, pRequest, pdev->DC->chip_Type[br_idx][ts_idx]);
			if(pRequest->error)
			{
				printk("DL_Dib9090_getDiversityDetailData fail!!!\n");

				kfree(pRequest);             // Jacky Han Added

				return -EFAULT;
			}
			if(copy_to_user((void *)pIOBuffer, (void *)pRequest, sizeof(DibcomDiversityData)))
			{
				printk("[IOCTL: IOCTL_ITE_DIVERSITY_GETDETAILDATA] - copy_to_user Fail!!\n");

				kfree(pRequest);             // Jacky Han Added

				return -EFAULT; 
			}
			kfree(pRequest);
			
			break;
		}	
/***************************************************************************/
/************************ Jacky Han Insertion Start ************************/
/***************************************************************************/
		case IOCTL_ITE_DIGITALWARRIOR_GET_VENDOR_ID:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
	   	    unsigned short VendorId;

			mutex_lock(&pdev->DC->dev_mutex);
            IT9300_readRegisters((Endeavour*) &pdev->DC->it9300, 0, EEPROM_VENDOR_ID, 2, (Byte*)&VendorId);
			mutex_unlock(&pdev->DC->dev_mutex);

            printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_VENDOR_ID) VendorId : 0x%x\n",VendorId);

			if(copy_to_user((void *)pIOBuffer, (void *)&VendorId, sizeof(unsigned short)))
			   return -EFAULT;

			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_GET_PRODUCT_ID:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
		    unsigned short ProductId;

			mutex_lock(&pdev->DC->dev_mutex);
            IT9300_readRegisters((Endeavour*) &pdev->DC->it9300, 0, EEPROM_PRODUCT_ID, 2, (Byte*)&ProductId);
			mutex_unlock(&pdev->DC->dev_mutex);

            printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_PRODUCT_ID) ProductId : 0x%x\n",ProductId);

			if(copy_to_user((void *)pIOBuffer, (void *)&ProductId, sizeof(unsigned short)))
			   return -EFAULT;

			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_GET_SERIAL_NUMBER:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
		    unsigned char SerialNumber[16];
		    unsigned long long DeviceSerialNumber = 0;
		    unsigned char* pByte;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_SERIAL_NUMBER)\n"); 

            memset(SerialNumber,0x30,15);
			SerialNumber[15] = 0;

			mutex_lock(&pdev->DC->dev_mutex);
		    IT9300_readRegisters((Endeavour*) &pdev->DC->it9300, 0, EEPROM_SERIAL_NUMBER, 15, (Byte*)&SerialNumber);
			mutex_unlock(&pdev->DC->dev_mutex);

            printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_SERIAL_NUMBER) SerialNumber : %s\n",SerialNumber);

		    pByte = (unsigned char*)&DeviceSerialNumber;
			if(SerialNumber[14] >= 0x41)
			{
			   SerialNumber[14] -= 0x41;
			   SerialNumber[14] += 0x0A;
			}
		    else
			   SerialNumber[14] -= 0x30;
            SerialNumber[14] &= 0x0F;
			if(SerialNumber[13] >= 0x41)
			{
			   SerialNumber[13] -= 0x41;
			   SerialNumber[13] += 0x0A;
			}
		    else
			   SerialNumber[13] -= 0x30;
            SerialNumber[13] &= 0x0F;
		    pByte[0] = (unsigned char)((SerialNumber[13] << 4) | SerialNumber[14]);

			if(SerialNumber[12] >= 0x41)
			{
			   SerialNumber[12] -= 0x41;
			   SerialNumber[12] += 0x0A;
			}
			else
			   SerialNumber[12] -= 0x30;
            SerialNumber[12] &= 0x0F;
			if(SerialNumber[11] >= 0x41)
			{
			   SerialNumber[11] -= 0x41;
			   SerialNumber[11] += 0x0A;
			}
		    else
			   SerialNumber[11] -= 0x30;
            SerialNumber[11] &= 0x0F;
		    pByte[1] = (unsigned char)((SerialNumber[11] << 4) | SerialNumber[12]);

		    if(SerialNumber[10] >= 0x41)
			{
			   SerialNumber[10] -= 0x41;
			   SerialNumber[10] += 0x0A;
			}
		    else
			   SerialNumber[10] -= 0x30;
            SerialNumber[10] &= 0x0F;
		    if(SerialNumber[9] >= 0x41)
			{
			   SerialNumber[9] -= 0x41;
			   SerialNumber[9] += 0x0A;
			}
		    else
			   SerialNumber[9] -= 0x30;
            SerialNumber[9] &= 0x0F;
		    pByte[2] = (unsigned char)((SerialNumber[9] << 4) | SerialNumber[10]);

		    if(SerialNumber[8] >= 0x41)
			{
			   SerialNumber[8] -= 0x41;
			   SerialNumber[8] += 0x0A;
			}
			else
			   SerialNumber[8] -= 0x30;
            SerialNumber[8] &= 0x0F;
			if(SerialNumber[7] >= 0x41)
			{
			   SerialNumber[7] -= 0x41;
			   SerialNumber[7] += 0x0A;
			}
			else
			   SerialNumber[7] -= 0x30;
            SerialNumber[7] &= 0x0F;
		    pByte[3] = (unsigned char)((SerialNumber[7] << 4) | SerialNumber[8]);

		    if(SerialNumber[6] >= 0x41)
			{
			   SerialNumber[6] -= 0x41;
			   SerialNumber[6] += 0x0A;
			}
			else
			   SerialNumber[6] -= 0x30;
            SerialNumber[6] &= 0x0F;
			if(SerialNumber[5] >= 0x41)
			{
			   SerialNumber[5] -= 0x41;
			   SerialNumber[5] += 0x0A;
			}
			else
			   SerialNumber[5] -= 0x30;
            SerialNumber[5] &= 0x0F;
			pByte[4] = (unsigned char)((SerialNumber[5] << 4) | SerialNumber[6]);

			if(SerialNumber[4] >= 0x41)
			{
			   SerialNumber[4] -= 0x41;
			   SerialNumber[4] += 0x0A;
			}
			else
			   SerialNumber[4] -= 0x30;
            SerialNumber[4] &= 0x0F;
		    if(SerialNumber[3] >= 0x41)
			{
			   SerialNumber[3] -= 0x41;
			   SerialNumber[3] += 0x0A;
			}
			else
			   SerialNumber[3] -= 0x30;
            SerialNumber[3] &= 0x0F;
			pByte[5] = (unsigned char)((SerialNumber[3] << 4) | SerialNumber[4]);

		    if(SerialNumber[2] >= 0x41)
			{
			   SerialNumber[2] -= 0x41;
			   SerialNumber[2] += 0x0A;
			}
			else
			   SerialNumber[2] -= 0x30;
            SerialNumber[2] &= 0x0F;
			if(SerialNumber[1] >= 0x41)
			{
			   SerialNumber[1] -= 0x41;
			   SerialNumber[1] += 0x0A;
			}
			else
			   SerialNumber[1] -= 0x30;
            SerialNumber[1] &= 0x0F;
		    pByte[6] = (unsigned char)((SerialNumber[1] << 4) | SerialNumber[2]);

			if(SerialNumber[0] >= 0x41)
			{
			   SerialNumber[0] -= 0x41;
			   SerialNumber[0] += 0x0A;
			}
			else
			   SerialNumber[0] -= 0x30;
            SerialNumber[0] &= 0x0F;
            pByte[7] = SerialNumber[0];


            printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_SERIAL_NUMBER) DeviceSerialNumber : 0x%llx\n",DeviceSerialNumber);

			if(copy_to_user((void *)pIOBuffer, (void *)&DeviceSerialNumber, sizeof(unsigned long long)))
			   return -EFAULT;

			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_SET_FRONTEND_OPEN:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_SET_FRONTEND_OPEN)\n"); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);			
            DRV_MyFrontend_Open(pdev->DC,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_SET_FRONTEND_CLOSE:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_SET_FRONTEND_CLOSE)\n"); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);			
			DRV_MyFrontend_Close(pdev->DC,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	

			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_SET_LNB_POWER:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			Bool bPower;
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_SET_LNB_POWER)\n"); 
						
			if (copy_from_user((void *)&bPower, (void *)pIOBuffer, sizeof(Bool)))
				return -EFAULT;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_SET_LNB_POWER) bPower : 0x%x\n",bPower); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);
			DRV_MyFrontend_SetLNBPower(pdev->DC,bPower,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_GET_LNB_POWER:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			Bool bPower = False;
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_LNB_POWER)\n"); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);			
			DRV_MyFrontend_GetLNBPower(pdev->DC,&bPower,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	

			if(copy_to_user((void *)pIOBuffer, (void *)&bPower, sizeof(Bool)))
				return -EFAULT; 

			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_GET_LNB_POWER_SHORT_CIRCUIT_DETECT:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			Bool bResult = False;
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_LNB_POWER_SHORT_CIRCUIT_DETECT)\n"); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
																
			mutex_lock(&pdev->DC->dev_mutex);			
			DRV_MyFrontend_GetLNBPowerShortCircuitDetect(pdev->DC,&bResult,ts_idx);	
			mutex_unlock(&pdev->DC->dev_mutex);	
					
			if(copy_to_user((void *)pIOBuffer, (void *)&bResult, sizeof(Bool)))
				return -EFAULT; 
			              
			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_ENFORCE_LNB_POWER_OFF:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_ENFORCE_LNB_POWER_OFF)\n"); 
																
			mutex_lock(&pdev->DC->dev_mutex);			
			DRV_MyFrontend_EnforceLNBPowerOff(pdev->DC);	
			mutex_unlock(&pdev->DC->dev_mutex);	
			              
			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_GET_CNR:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			unsigned long CNR = 0;
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_CNR)\n"); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
																
			mutex_lock(&pdev->DC->dev_mutex);			
			DRV_MyFrontend_GetCNR(pdev->DC,&CNR,ts_idx);	
			mutex_unlock(&pdev->DC->dev_mutex);	
					
			if(copy_to_user((void *)pIOBuffer, (void *)&CNR, sizeof(unsigned long)))
				return -EFAULT; 

			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_SET_TSID:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			unsigned short TSID;
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_SET_TSID)\n"); 
			
			if (copy_from_user((void *)&TSID, (void *)pIOBuffer, sizeof(unsigned short)))
				return -EFAULT;
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);
			DRV_MyFrontend_SetTSID(pdev->DC,TSID,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	

			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_SET_ONE_SEG_MODE:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			Bool OneSegModeFlag;
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_SET_ONE_SEG_MODE)\n"); 
			
			if (copy_from_user((void *)&OneSegModeFlag, (void *)pIOBuffer, sizeof(Bool)))
				return -EFAULT;
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);
			DRV_MyFrontend_SetOneSegMode(pdev->DC,OneSegModeFlag,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	

			break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_GET_CNRAW:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			int ts_idx = 0;
			int CNRaw = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_ITE_DIGITALWARRIOR_GET_CNRAW)\n"); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);
            DRV_MyFrontend_GetCNRaw(pdev->DC,&CNRaw,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	
					
			if(copy_to_user((void *)pIOBuffer, (void *)&CNRaw, sizeof(int)))
				return -EFAULT; 

			break;
		}
		case IOCTL_RECPX4PX5PX6_SET_CHANNEL:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			RECPX4PX5PX6_FREQUENCY_PARAMETER FrequencyParameter;
			Bool OpenFlag = False;
			int ts_idx = 0;
			unsigned short FrequencyTableIndex;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_RECPX4PX5PX6_SET_CHANNEL)\n"); 
			
			if (copy_from_user((void *)&FrequencyParameter, (void *)pIOBuffer, sizeof(RECPX4PX5PX6_FREQUENCY_PARAMETER)))
				return -EFAULT;

printk("(Jacky)(DemodIOCTLFun)(IOCTL_RECPX4PX5PX6_SET_CHANNEL) number : %d\n",number); 
			

			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);
            DRV_MyFrontend_isOpen(pdev->DC,&OpenFlag,ts_idx);
			if(OpenFlag == False)
			   DRV_MyFrontend_Open(pdev->DC,ts_idx);
			
			if(pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
			   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
			   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
			   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
			   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
			   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
			{
			   if(pdev->DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[ts_idx] == ISDBS_MODE)
			      FrequencyTableIndex = FindFrequencyTableIndexForISDBS(FrequencyParameter);
			   else
			   {
			      if(FrequencyParameter.frequencyno <= 112)
				  {
			         FrequencyTableIndex = (FrequencyParameter.frequencyno + 49);
				  }
			      else
				     FrequencyTableIndex = 0xFFFF;
			   }
			}
			else
			if(pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
			   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        	   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        	   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        	   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        	   pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
			{
			   if((pdev->DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT && pdev->DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS) || 
				  (pdev->DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS && ((pdev->DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[ts_idx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || (pdev->DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[ts_idx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)))
			   {
                  FrequencyTableIndex = FindFrequencyTableIndexForISDBS(FrequencyParameter);
			      if(FrequencyTableIndex != 0xFFFF)
				     DRV_MyFrontend_SetDemodulationMode(pdev->DC,ISDBS_MODE,ts_idx);
			      else
				  {
			         if(FrequencyParameter.frequencyno <= 112)
					 {
			            FrequencyTableIndex = (FrequencyParameter.frequencyno + 49);

					    DRV_MyFrontend_SetDemodulationMode(pdev->DC,ISDBT_MODE,ts_idx);
					 }
			         else
				        FrequencyTableIndex = 0xFFFF;
				  }
			   }
			   else
			   {
			      if(FrequencyParameter.frequencyno <= 112)
				  {
			         FrequencyTableIndex = (FrequencyParameter.frequencyno + 49);
				  }
			      else
				     FrequencyTableIndex = 0xFFFF;
			   }
			}
            if(FrequencyTableIndex != 0xFFFF)
			{
               DRV_MyFrontend_acquireChannel(pdev->DC,Recpx4px5px6IsdbFrequencyTableArray[FrequencyTableIndex].BandwidthKhz,Recpx4px5px6IsdbFrequencyTableArray[FrequencyTableIndex].FrequencyKhz,ts_idx);
			   if(Recpx4px5px6IsdbFrequencyTableArray[FrequencyTableIndex].TSID != 0xFFFF)
			      DRV_MyFrontend_SetTSID(pdev->DC,Recpx4px5px6IsdbFrequencyTableArray[FrequencyTableIndex].TSID,ts_idx);
			}
			mutex_unlock(&pdev->DC->dev_mutex);	

			break;
		}
		case IOCTL_RECPX4PX5PX6_GET_SIGNAL_STRENGTH:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			int ts_idx = 0;
			int CNRaw = 0;

            printk("(Jacky)(DemodIOCTLFun)(IOCTL_RECPX4PX5PX6_GET_SIGNAL_STRENGTH)\n"); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);
            DRV_MyFrontend_GetCNRaw(pdev->DC,&CNRaw,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	
					
			if(copy_to_user((void *)pIOBuffer, (void *)&CNRaw, sizeof(int)))
				return -EFAULT; 

			break;
		}
		case IOCTL_RECPX4PX5PX6_LNB_ENABLE:	
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			int ts_idx = 0;
			int LnbVoltageType;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_RECPX4PX5PX6_LNB_ENABLE)\n"); 
			
			copy_from_user((void *)&LnbVoltageType, (void *)pIOBuffer, sizeof(int));

			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);
            DRV_MyFrontend_SetLNBPower(pdev->DC,True,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	

			break;
		}
		case IOCTL_RECPX4PX5PX6_LNB_DISABLE:
		{
			IT930x_Device* pdev = (IT930x_Device*) dev;	
			int ts_idx = 0;

			printk("(Jacky)(DemodIOCTLFun)(IOCTL_RECPX4PX5PX6_LNB_DISABLE)\n"); 
			
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

			mutex_lock(&pdev->DC->dev_mutex);
            DRV_MyFrontend_SetLNBPower(pdev->DC,False,ts_idx);
			mutex_unlock(&pdev->DC->dev_mutex);	

			break;
		}
/***************************************************************************/
/************************* Jacky Han Insertion End *************************/
/***************************************************************************/
		case IOCTL_ITE_ENDEAVOUR_BCAS_RESETCARD:
		{
			int br_idx = 0;
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4	
			
			mutex_lock(&pdev->DC->dev_mutex);			
				error = IT9300_bcasResetCard(&pdev->DC->it9300, br_idx);	
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			if(error) {
				deb_data("IT9300_bcasResetCard chip = %d fail!!!\n", br_idx);
				return -EFAULT; 
			}
			
//			deb_data("IT9300_bcasResetCard chip = %d OK!!!\n", br_idx);
			break;
		}	
		case IOCTL_ITE_ENDEAVOUR_BCAS_SENDDATA:
		{
			BCASData data;
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			if (copy_from_user((void *)&data, (void *)pIOBuffer, sizeof(BCASData)))
				return -EFAULT;
				
									
			mutex_lock(&pdev->DC->dev_mutex);			
				error = IT9300_bcasSendData(&pdev->DC->it9300, br_idx, data.byLength, data.abyBuffer);	
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			if(error) {
				deb_data("IT9300_bcasSendData chip = %d fail!!!\n", br_idx);
				return -EFAULT; 
			}
			
//			deb_data("IT9300_bcasSendData chip = %d Length = %d OK!!!\n", br_idx, data.byLength);
			
			              
			break;
		}	
		case IOCTL_ITE_ENDEAVOUR_BCAS_SETBAUDRATE:
		{
			Dword dwBaudRate = UART_BAUDRATE_9600;
			UartBaudRate baudrate = UART_BAUDRATE_9600; //Default Baud Rate
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4	
			
			if (copy_from_user((void *)&dwBaudRate, (void *)pIOBuffer, sizeof(Dword)))
				return -EFAULT;
			
			deb_data("Set BaudRate = %lu !!!\n", dwBaudRate);
			switch(dwBaudRate)
			{
				case 9600:
					baudrate = UART_BAUDRATE_9600;
					break;
				case 19200:
					baudrate = UART_BAUDRATE_19200;
					break;
				default:
					deb_data("Baudrate not supporting!!!\n");
					baudrate = UART_BAUDRATE_9600;
					break;
			}
						
			mutex_lock(&pdev->DC->dev_mutex);			
				error = IT9300_bcasSetBaudrate(&pdev->DC->it9300, br_idx, baudrate);	
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			if(error) {
				deb_data("IT9300_bcasSetBaudrate chip = %d fail!!!\n", br_idx);
				return -EFAULT; 
			}
			
//			deb_data("IT9300_bcasSetBaudrate chip = %d OK!!!\n", br_idx);
			
			
			break;
		}		
		case IOCTL_ITE_ENDEAVOUR_BCAS_GETDATA:
		{
			BCASData data;
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
			memset(&data, 0, sizeof(data));
			
			if (copy_from_user((void *)&data, (void *)pIOBuffer, sizeof(BCASData)))
				return -EFAULT;
															
			mutex_lock(&pdev->DC->dev_mutex);			
				error = IT9300_bcasGetData(&pdev->DC->it9300, br_idx, &data.byLength, data.abyBuffer);	
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			if(error) {
				deb_data("IT9300_bcasGetData chip = %d fail!!! error = 0x%lx\n", br_idx, error);
				return -EFAULT; 
			}
			
//			deb_data("IT9300_bcasGetData chip = %d Length = %d OK!!!\n", br_idx, data.byLength);
			
			if(copy_to_user((void *)pIOBuffer, (void *)&data, sizeof(BCASData)))
			{
				printk("[IOCTL: IOCTL_ITE_ENDEAVOUR_BCAS_GETDATA] - copy_to_user Fail!!\n");
				return -EFAULT; 
			}
			              
			break;
		}
		case IOCTL_ITE_ENDEAVOUR_BCAS_GETISREADY:
		{
			Byte byResult = 0;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
													
			mutex_lock(&pdev->DC->dev_mutex);			
				error = IT9300_readRegister(&pdev->DC->it9300, br_idx, OVA_UART_RX_READY, &byResult);	
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			if(error) {
				deb_data("IOCTL_ITE_ENDEAVOUR_BCAS_GETISREADY chip = %d fail!!!\n", br_idx);
				return -EFAULT; 
			}

//			deb_data("IT9300_bcasGetIsReady chip = %d IsReady = %d OK!!!\n", br_idx, byResult);
		
			if(copy_to_user((void *)pIOBuffer, (void *)&byResult, sizeof(Byte)))
			{
				printk("[IOCTL: IOCTL_ITE_ENDEAVOUR_BCAS_GETISREADY] - copy_to_user Fail!!\n");
				return -EFAULT; 
			}
			              
			break;
		}	
		case IOCTL_ITE_ENDEAVOUR_BCAS_DETECTCARD:
		{
			Bool bResult = False;
			
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			
													
			mutex_lock(&pdev->DC->dev_mutex);			
				error = IT9300_bcasDetectCard(&pdev->DC->it9300, br_idx, &bResult);	
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			if(error) 
			{
				deb_data("IT9300_bcasDetectCard chip = %d fail!!!\n", br_idx);
				return -EFAULT; 
			}
			
//			deb_data("IT9300_bcasDetectCard chip = %d bResult = %d OK!!!\n", br_idx, bResult);
		
			if(copy_to_user((void *)pIOBuffer, (void *)&bResult, sizeof(Bool)))
			{
				printk("[IOCTL: IOCTL_ITE_ENDEAVOUR_BCAS_DETECTCARD] - copy_to_user Fail!!\n");
				return -EFAULT; 
			}
			              
			break;
		}
		case IOCTL_ITE_ENDEAVOUR_BCAS_GETATR:
		{
			Byte byResult = 0;
			BCASData data;
			
			memset(&data, 0, sizeof(data));
			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
													
			mutex_lock(&pdev->DC->dev_mutex);			
				error = IT9300_readRegister(&pdev->DC->it9300, br_idx, OVA_UART_RX_LENGTH, &byResult);	
			mutex_unlock(&pdev->DC->dev_mutex);	
			
			if(error) {
				deb_data("IOCTL_ITE_ENDEAVOUR_BCAS_GETATR chip = %d fail!!!\n", br_idx);
				return -EFAULT; 
			}
			if(byResult == 13)	{
				mutex_lock(&pdev->DC->dev_mutex);			
					error = IT9300_bcasGetData(&pdev->DC->it9300, br_idx, &data.byLength, data.abyBuffer);	
				mutex_unlock(&pdev->DC->dev_mutex);	
				
			}else{
				deb_data("IOCTL_ITE_ENDEAVOUR_BCAS_GETATR chip = %d  Wrong Length(receiving data length : %d)!!!\n", br_idx,byResult);
				return -EFAULT; 
			}
					
			if(copy_to_user((void *)pIOBuffer, (void *)&data, sizeof(BCASData)))
			{
				printk("[IOCTL: IOCTL_ITE_ENDEAVOUR_BCAS_GETATR] - copy_to_user Fail!!\n");
				return -EFAULT; 
			}
			              
			break;
		}
		
//**********************************************************************************
//************************* Jacky Han Insertion Start ******************************
//**********************************************************************************
		case IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_STRENGTH:
		{
            unsigned char Strength;
			IT930x_Device* pdev = (IT930x_Device*) dev;
            Device_Context *pDC = (Device_Context*) pdev->DC;  
			int br_idx = 0, ts_idx = 0;			
			Dword error;

			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

            error = DL_Demodulator_getChannelStrength(pDC, &Strength, br_idx, ts_idx);      
			if(error) 
			{
			   JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_STRENGTH] - DL_Demodulator_getChannelStrength() failed !\n");
			}
			
			if(copy_to_user((void *)pIOBuffer, (void *)&Strength, sizeof(unsigned char)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_STRENGTH] - copy_to_user Fail!!\n");

				return -EFAULT; 
			}
			
    		break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_QUALITY:
		{
            unsigned char Quality;
			IT930x_Device* pdev = (IT930x_Device*) dev;
            Device_Context *pDC = (Device_Context*) pdev->DC;  
			int br_idx = 0, ts_idx = 0;			
			Dword error;

			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

            error = DL_Demodulator_getChannelQuality(pDC, &Quality, br_idx, ts_idx);      
			if(error) 
			{
			   JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_QUALITY] - DL_Demodulator_getChannelQuality() failed !\n");
			}
			
			if(copy_to_user((void *)pIOBuffer, (void *)&Quality, sizeof(unsigned char)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_QUALITY] - copy_to_user Fail!!\n");

				return -EFAULT; 
			}
			
    		break;
		}
		case IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_ERROR_RATE:
		{
            unsigned char ErrorRate;
			IT930x_Device* pdev = (IT930x_Device*) dev;
            Device_Context *pDC = (Device_Context*) pdev->DC;  
			int br_idx = 0, ts_idx = 0;			
			Dword error;

			br_idx = number / JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4
			ts_idx = number % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;    // Jacky Han Modified // original is 4

            error = DL_Demodulator_getChannelErrorRate(pDC, &ErrorRate, br_idx, ts_idx);      
			if(error) 
			{
			   JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_ERROR_RATE] - DL_Demodulator_getChannelErrorRate() failed !\n");
			}
			
			if(copy_to_user((void *)pIOBuffer, (void *)&ErrorRate, sizeof(unsigned char)))
			{
				JackyIT930xDriverDebug("(Jacky)(DemodIOCTLFun) [IOCTL_ITE_DIGITALWARRIOR_GET_CHANNEL_ERROR_RATE] - copy_to_user Fail!!\n");

				return -EFAULT; 
			}
			
    		break;
		}
//**********************************************************************************
//************************* Jacky Han Insertion Start ******************************
//**********************************************************************************

		default:
		{
			deb_data("\t Error: command[0x%08lu] not support\n", IOCTLCode);
			return ENOTTY;
		}
    }  
	
    return error;
}
