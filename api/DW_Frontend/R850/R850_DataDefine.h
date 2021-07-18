#ifndef _R850_DATADEFINE_H_ 
#define _R850_DATADEFINE_H_

#define DEBUG_MODE  FALSE

#define R850_VERSION   "R850_GUI_v1.2C"
#define VER_NUM   0
#define CHIP_ID       0x98

#define R850_I2C_DEVICE_ADDRESS               0xF8
//----------------------------------------------------------//
//                   Define                                            //
//----------------------------------------------------------//
//===========Clock out Setting=========//
#define R850_CLOCK_OUT_ENABLE_AFTER_INIT  FALSE

//===========Share Xtal=========//
#define R850_SHARE_XTAL    FALSE

//===========RSSI Type============//
//TRUE : Front-end circuit is not "no power loop-through" solution.
//FALSE: Front-end circuitry adds "no power loop-through" solution.
#define R850_RSSI_NORMAL_TYPE    TRUE


#define R850_REG_NUM         48  //R0~R7: read only
#define R850_RING_POWER_FREQ 115000
#define R850_IMR_IF          5300
#define R850_IMR_TRIAL       9
#define R850_IMR_GAIN_REG    20
#define R850_IMR_PHASE_REG   21
#define R850_IMR_IQCAP_REG   21
#define R850_IMR_POINT_NUM   10
//----------------------------------------------------------//
//                   Internal Structure                     //
//----------------------------------------------------------//

typedef struct _R850_Sys_Info_Type
{
	UINT8          BW;
	UINT8		   HPF_COR;
	UINT8          FILT_EXT_ENA;
	UINT8          HPF_NOTCH;  
	UINT8          AGC_CLK; 
	UINT8          IMG_GAIN;  
	UINT16		   FILT_COMP;
	UINT16		   IF_KHz;            
	UINT16		   FILT_CAL_IF; 
	
}R850_Sys_Info_Type;

typedef struct _R850_Freq_Info_Type
{
	UINT8		RF_POLY;
	UINT8		LNA_BAND;
	UINT8		LPF_CAP;
	UINT8		LPF_NOTCH;
	UINT8		TF_DIPLEXER;
	UINT8		TF_HPF_BPF;
	UINT8		TF_HPF_CNR;
	UINT8		IMR_MEM_NOR;
	UINT8		IMR_MEM_REV;
	UINT8       TEMP;    

}R850_Freq_Info_Type;

typedef struct _R850_SysFreq_Info_Type
{
	UINT8	   LNA_TOP;
	UINT8	   LNA_VTL_H;
	UINT8      RF_TOP;
	UINT8      RF_VTL_H;
	UINT8      RF_GAIN_LIMIT;
	UINT8      NRB_TOP;
	UINT8      NRB_BW_HPF;
	UINT8      NRB_BW_LPF;
	UINT8	   MIXER_TOP;
	UINT8	   MIXER_VTH;
	UINT8	   MIXER_VTL;
	UINT8      MIXER_GAIN_LIMIT;
	UINT8      FILTER_TOP;
	UINT8      FILTER_VTH;
	UINT8      FILTER_VTL;
	UINT8      LNA_RF_DIS_MODE;
	UINT8      LNA_RF_DIS_CURR;
	UINT8      LNA_DIS_SLOW_FAST;
	UINT8      RF_DIS_SLOW_FAST;
	UINT8      BB_DET_MODE;
	UINT8      BB_DIS_CURR;
	UINT8      MIXER_FILTER_DIS;
	UINT8      IMG_NRB_ADDER;
	UINT8      LNA_NRB_DET;
	UINT8      ENB_POLY_GAIN;
	UINT8	   MIXER_AMP_LPF;
	UINT8	   NA_PWR_DET; 
	UINT8	   FILT_3TH_LPF_CUR;
	UINT8	   FILT_3TH_LPF_GAIN;
	UINT8	   RF_LTE_PSG;
	UINT8	   HPF_COMP;
	UINT8	   FB_RES_1ST;
	UINT8	   MIXER_DETBW_LPF;
	UINT8	   LNA_RF_CHARGE_CUR;
	UINT8      TEMP;

}R850_SysFreq_Info_Type;

typedef struct _R850_Cal_Info_Type
{
	UINT8       TF_PATH;
	UINT8		MIXER_AMP_GAIN;
	UINT8		MIXER_BUFFER_GAIN;
	UINT8		LNA_GAIN;
	UINT8		LNA_POWER;
	UINT8		RFBUF_POWER;
	UINT8		RFBUF_OUT;

}R850_Cal_Info_Type;

typedef struct _R850_SectType
{
	UINT8   Phase_Y;
	UINT8   Gain_X;
	UINT8   Iqcap;
	UINT8   Value;

}R850_SectType;

typedef enum _R850_IMR_Type  
{
	R850_IMR_NOR=0,
	R850_IMR_REV,

}R850_IMR_Type;

typedef enum _R850_Cal_Type
{
	R850_IMR_CAL = 0,
	R850_IMR_LNA_CAL,
	R850_LPF_CAL,
	R850_LPF_LNA_CAL

}R850_Cal_Type;

typedef enum _R850_BW_Type
{
	BW_6M = 0,
	BW_7M,
	BW_8M,
	BW_1_7M,
	BW_10M,
	BW_200K

}R850_BW_Type;

typedef enum _R850_NRB_DET_Type  
{
	R850_NRB_DET_ON=0,
	R850_NRB_DET_OFF,

}R850_NRB_DET_Type;

enum XTAL_PWR_VALUE
{
	XTAL_LOWEST = 0,
    XTAL_LOW,
    XTAL_HIGH,
    XTAL_HIGHEST,
	XTAL_CHECK_SIZE
};

typedef enum _Rafael_Chip_Type  //Don't modify chip list
{
	R850_MP = 0,
	R850_MT1 = 1

}Rafael_Chip_Type;



//----------------------------------------------------------//
//                   R850 Public Parameter                     //
//----------------------------------------------------------//

typedef enum _R850_ErrCode
{
	R850_RT_Success = TRUE,
	R850_RT_Fail = FALSE

}R850_ErrCode;

typedef enum _R850_Standard_Type  //Don't remove standand list!!
{
	//DTV
	R850_DVB_T_6M = 0,  
	R850_DVB_T_7M,
	R850_DVB_T_8M, 
    R850_DVB_T2_6M,       //IF=4.57M
	R850_DVB_T2_7M,       //IF=4.57M
	R850_DVB_T2_8M,       //IF=4.57M
	R850_DVB_T2_1_7M,
	R850_DVB_T2_10M,
	R850_DVB_C_8M,
	R850_DVB_C_6M, 
	R850_J83B,
	R850_ISDB_T_4063,           //IF=4.063M
	R850_ISDB_T_4570,           //IF=4.57M
	R850_DTMB_8M_4570,      //IF=4.57M
	R850_DTMB_6M_4500,      //IF=4.5M, BW=6M
	R850_ATSC,  
	//DTV IF=5M
	R850_DVB_T_6M_IF_5M,
	R850_DVB_T_7M_IF_5M,
	R850_DVB_T_8M_IF_5M,
	R850_DVB_T2_6M_IF_5M,
	R850_DVB_T2_7M_IF_5M,
	R850_DVB_T2_8M_IF_5M,
	R850_DVB_T2_1_7M_IF_5M,
	R850_DVB_C_8M_IF_5M,
	R850_DVB_C_6M_IF_5M, 
	R850_J83B_IF_5M,
	R850_ISDB_T_IF_5M,            
	R850_DTMB_8M_IF_5M,     
	R850_DTMB_6M_IF_5M,     
	R850_ATSC_IF_5M,  
	R850_FM,
	R850_STD_SIZE,
	R850_UNKNOWN_STANDARD,           // Jacky Han Added

}R850_Standard_Type;

typedef enum _R850_LoopThrough_Type
{
	LT_ON = TRUE,
	LT_OFF= FALSE

}R850_LoopThrough_Type;

typedef enum _R850_ClkOutMode_Type
{
	CLK_OUT_OFF = 0,
	CLK_OUT_ON

}R850_ClkOutMode_Type;

typedef enum _R850_RF_Gain_TYPE
{
	RF_AUTO = 0,
	RF_MANUAL

}R850_RF_Gain_TYPE;

typedef enum _R850_Xtal_Div_TYPE
{
	XTAL_DIV1 = 0,
	XTAL_DIV1_2,	//1st_div2=0(R34[0]), 2nd_div2=1(R34[1])  ; same AGC clock
	XTAL_DIV2_1,	//1st_div2=1(R34[0]), 2nd_div2=0(R34[1])  ; diff AGC clock
	XTAL_DIV4

}R850_Xtal_Div_TYPE;

typedef struct _R850_Set_Info
{
	UINT32                   RF_KHz;
	R850_Standard_Type       R850_Standard;
	R850_LoopThrough_Type    R850_LT;
	R850_ClkOutMode_Type     R850_ClkOutMode;

}R850_Set_Info;

typedef struct _R850_RF_Gain_Info
{
	UINT16  RF_gain_comb;
	UINT8   RF_gain1;
	UINT8   RF_gain2;
	UINT8   RF_gain3;
	UINT8   RF_gain4;

}R850_RF_Gain_Info;

typedef enum _R850_Vga_Mode_TYPE
{
	VGA_AUTO = 0,
	VGA_MANUAL

}R850_Vga_Mode_TYPE;

typedef enum _R850_HpfNotch_Type
{
	HPF_NOTCH_HIGH = 0,
	HPF_NOTCH_LOW

}R850_HpfNotch_Type;

typedef enum _R850_ClkSwing_Type
{
	CLK_SWING_LOW = 0,
	CLK_SWING_HIGH

}R850_ClkSwing_Type;

typedef struct _R850_I2C_LEN_TYPE
{
	UINT8 RegAddr;
	UINT8 Data[50];
	UINT8 Len;

}R850_I2C_LEN_TYPE;

typedef struct _R850_I2C_TYPE
{
	UINT8 RegAddr;
	UINT8 Data;

}R850_I2C_TYPE;

#endif // _R850_DATADEFINE_H_