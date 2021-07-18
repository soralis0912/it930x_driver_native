#ifndef _RT710_DATADEFINE_H_ 
#define _RT710_DATADEFINE_H_

#define RT710_VERSION   "RT710(720)_GUI_v3.6"

#define RT710_I2C_DEVICE_ADDRESS               0xF4

#define RT710_Reg_Num   16

//Xtal Frequency Support 16MHz(16000), 27MHz(27000), 28.8MHz(28800), 24MHz(24000) 
#define RT710_Xtal	24000

#define RT710_0DBM_SETTING 	FALSE
//#define RT710_0DBM_SETTING 	TRUE  

typedef enum _RT710_Err_Type
{
	RT710_RT_Success = TRUE,
	RT710_RT_Fail    = FALSE

}RT710_Err_Type;

typedef enum _RT710_LoopThrough_Type
{
	LOOP_THROUGH = TRUE,
	SIGLE_IN     = FALSE

}RT710_LoopThrough_Type;

typedef enum _RT710_ClockOut_Type
{
	ClockOutOn = TRUE,
	ClockOutOff= FALSE

}RT710_ClockOut_Type;

typedef enum _RT710_OutputSignal_Type
{
	DifferentialOut = TRUE,
	SingleOut     = FALSE

}RT710_OutputSignal_Type;

typedef enum _RT710_AGC_Type
{
	AGC_Negative = TRUE,
	AGC_Positive = FALSE

}RT710_AGC_Type;

typedef enum _RT710_AttenVga_Type
{
	ATTENVGAON = TRUE,
	ATTENVGAOFF= FALSE

}RT710_AttenVga_Type;

typedef enum _R710_FineGain_Type
{
	FINEGAIN_3DB = 0,
	FINEGAIN_2DB,
	FINEGAIN_1DB,
	FINEGAIN_0DB

}R710_FineGain_Type;

typedef enum _RT710_RollOff_Type
{
	ROLL_OFF_0_15 = 0,	//roll-off = 0.15
	ROLL_OFF_0_20,		//roll-off = 0.2
	ROLL_OFF_0_25,		//roll-off = 0.25
	ROLL_OFF_0_30,		//roll-off = 0.3
	ROLL_OFF_0_35,		//roll-off = 0.35
	ROLL_OFF_0_40,		//roll-off = 0.4
	UNKNOWN_ROLL_OFF,                      // Jacky Han Added

}RT710_RollOff_Type;

typedef enum _RT710_Scan_Type
{
	RT710_AUTO_SCAN = TRUE,	//Blind Scan 
	RT710_MANUAL_SCAN = FALSE	//Normal(Default)

}RT710_Scan_Type;

typedef struct _RT710_RF_Gain_Info
{
	UINT8   RF_gain;

}RT710_RF_Gain_Info;

typedef struct _RT710_INFO_Type
{
	UINT32 RF_KHz;
	UINT32 SymbolRate_Kbps;
	RT710_RollOff_Type RT710_RollOff_Mode;
	RT710_LoopThrough_Type RT710_LoopThrough_Mode;
	RT710_ClockOut_Type RT710_ClockOut_Mode;
	RT710_OutputSignal_Type RT710_OutputSignal_Mode;
	RT710_AGC_Type RT710_AGC_Mode;
	RT710_AttenVga_Type RT710_AttenVga_Mode;
	R710_FineGain_Type R710_FineGain;
	RT710_Scan_Type RT710_Scan_Mode;	//only support RT720

}RT710_INFO_Type;

typedef struct _RT710_I2C_LEN_TYPE
{
	UINT8 RegAddr;
	UINT8 Data[50];
	UINT8 Len;

}RT710_I2C_LEN_TYPE;

typedef struct _RT710_I2C_TYPE
{
	UINT8 RegAddr;
	UINT8 Data;

}RT710_I2C_TYPE;

#endif // _RT710_DATADEFINE_H_