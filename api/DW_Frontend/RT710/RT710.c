/****************************************************************
                           RT710.c
Copyright 2014 by Rafaelmicro., Inc.
                 C H A N G E   R E C O R D
  Date        Author           Version      Description
--------   --------------    --------   ----------------------
01/29/2015	   Ryan				v3.0		add 720 setting
01/29/2015	   Ryan				v3.1		add scan mode for RT720 (RT710 not support)
02/17/2015	   Ryan				v3.2		Change the pll execution time
02/12/2015	   Ryan				v3.2a		Standby function add clock parameter
03/30/2015	   Ryan				v3.3		Add RT720 filter table (with different RT710)
04/07/2015	   Ryan				v3.3a		optimize PLL Setting
04/07/2015	   Ryan				v3.4c		Add Xtal Cap Option only for R720 chip
10/30/2015     Vincent          v3.5        modify IF Fillter bandwidth
03/21/2016     Vincent          v3.6        Modify vth vtl parameter range
											Remove floating calculation
											Standby LT off,xtal ldo off,RT720 lna off,LNA match change(s11)
											
/****************************************************************/

#include <linux/kernel.h> 
#include <linux/slab.h>
#include <linux/delay.h>

#include "it930x-core.h"
#include "RT710.h"
#include "MyI2CPortingLayer.h"
#include "MyDebug.h"

//===========Xtal Cap set (Range is 0~30pF) ==============
//Suggest Xtal_cap use 30pF when xtal CL value is 16pF , Default.
#define  RT710_XTAL_CAP               30

RT710_Err_Type RT710_PLL(Device_Context *DC,unsigned char TunerDeviceID,UINT32 PLL_Freq);


#if(RT710_0DBM_SETTING == TRUE)
	//0dBm ; LT:lna output ; LT:HPF off  ; LT Current High
	UINT8 RT710_Reg_Arry_Init[RT710_Reg_Num] ={0x40, 0x1C, 0xA0, 0x90, 0x41, 0x50, 0xED, 0x25, 0x47, 0x58, 0x39, 0x60, 0x38, 0xE7, 0x90, 0x35};
#else
	//-10~-30dBm ; LT:lna center ; LT:HPF on  ; LT Current Low
	UINT8 RT710_Reg_Arry_Init[RT710_Reg_Num] ={0x40, 0x1D, 0x20, 0x10, 0x41, 0x50, 0xED, 0x25, 0x07, 0x58, 0x39, 0x64, 0x38, 0xE7, 0x90, 0x35};
#endif

UINT8 RT720_Reg_Arry_Init[RT710_Reg_Num] ={0x00, 0x1C, 0x00, 0x10, 0x41, 0x48, 0xDA, 0x4B, 0x07, 0x58, 0x38, 0x40, 0x37, 0xE7, 0x4C, 0x59};


static UINT16 RT710_Lna_Acc_Gain[19] = 
{
 0, 26, 42, 74, 103, 129, 158, 181, 188, 200,  //0~9
 220, 248, 280, 312, 341, 352, 366, 389, 409,	//10~19
};
static UINT16 RT710_Lna2_Acc_Gain[32] = 
{
 0, 27, 53, 81, 109, 134, 156, 176, 194, 202,   //0~9
 211, 221, 232, 245, 258, 271, 285, 307, 326, 341,	//10~19
 357, 374, 393, 410, 428, 439, 445, 470, 476, 479, 	//20~29
 495, 507	//30~31
};

RT710_Err_Type RT710_Init(Device_Context *DC,unsigned char TunerDeviceID);
RT710_Err_Type RT710_Setting(Device_Context *DC,unsigned char TunerDeviceID,RT710_INFO_Type RT710_Set_Info);
RT710_Err_Type RT710_PLL(Device_Context *DC,unsigned char TunerDeviceID,UINT32 Freq);
RT710_Err_Type RT710_Standby(Device_Context *DC,unsigned char TunerDeviceID,RT710_LoopThrough_Type RT710_LTSel, RT710_ClockOut_Type RT710_CLKSel );
UINT8 RT710_PLL_Lock(Device_Context *DC,unsigned char TunerDeviceID);
RT710_Err_Type RT710_GetRfGain(Device_Context *DC,unsigned char TunerDeviceID,RT710_RF_Gain_Info *RT710_rf_gain);
RT710_Err_Type RT710_GetRfRssi(Device_Context *DC,unsigned char TunerDeviceID,UINT32 RF_Freq_Khz, INT32 *RfLevelDbm, UINT8 *fgRfGainflag);
void RT710_Sleep(UINT32 Milliseconds);
BOOL RT710_I2C_Write_Len(Device_Context *DC,unsigned char TunerDeviceID,RT710_I2C_LEN_TYPE *I2C_Info);
BOOL RT710_I2C_Read_Len(Device_Context *DC,unsigned char TunerDeviceID,RT710_I2C_LEN_TYPE *I2C_Info);
BOOL RT710_I2C_Write(Device_Context *DC,unsigned char TunerDeviceID,RT710_I2C_TYPE *I2C_Info);
int RT710_Convert(int InvertNum);

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
BOOL RT710_Initialization(Device_Context *DC,unsigned char TunerDeviceID)
{
    RT710_Err_Type RT710ErrorType;
    unsigned char TunerDeviceIndex;
	BOOL Result = FALSE;

    DriverDebugPrint("(RT710_Initialization)\n");
    DriverDebugPrint("(RT710_Initialization) TunerDeviceID : 0x%x\n",TunerDeviceID);

    TunerDeviceIndex = TunerDeviceID;

    DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][0] = 0; 
    DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][1] = 0; 
    DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][2] = 0; 
    DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][3] = 0; 
	DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] = TRUE;

    RT710ErrorType = RT710_Init(DC,TunerDeviceID);
	if(RT710ErrorType == RT710_RT_Success)
	   Result = TRUE;

	return Result;
}
void RT710_Uninitialization(Device_Context *DC,unsigned char TunerDeviceID)
{

}
BOOL RT710_SetFrequency(Device_Context *DC,unsigned char TunerDeviceID)
{
	RT710_INFO_Type RT710InfoType;
    RT710_Err_Type RT710ErrorType;
    unsigned char TunerDeviceIndex;
	BOOL Result = FALSE;

    TunerDeviceIndex = TunerDeviceID;
  
	RT710InfoType.RF_KHz = (DC->JackyFrontend.JackyGlobalFrontendRequestTuningFrequencyKhz[TunerDeviceIndex] - 10678000);
	RT710InfoType.SymbolRate_Kbps = 28860;
	RT710InfoType.RT710_RollOff_Mode = ROLL_OFF_0_35;
	RT710InfoType.RT710_LoopThrough_Mode = SIGLE_IN;
	RT710InfoType.RT710_ClockOut_Mode = ClockOutOff;
	RT710InfoType.RT710_OutputSignal_Mode = DifferentialOut;
	RT710InfoType.RT710_AGC_Mode = AGC_Positive;
	RT710InfoType.RT710_AttenVga_Mode = ATTENVGAOFF;
	RT710InfoType.R710_FineGain = FINEGAIN_3DB;
	RT710InfoType.RT710_Scan_Mode = RT710_MANUAL_SCAN;

    RT710ErrorType = RT710_Setting(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[TunerDeviceIndex],RT710InfoType);
    if(RT710ErrorType == RT710_RT_Success)
       Result = TRUE;

	return Result;
}
char RT710_GetRssi(Device_Context *DC,unsigned char TunerDeviceID)
{
    INT32 RfLevelDbm;
	UINT8 fgRfMaxGain;
    unsigned char TunerDeviceIndex;
	char Rssi;

    TunerDeviceIndex = TunerDeviceID;

	RT710_GetRfRssi(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[TunerDeviceIndex],DC->JackyFrontend.JackyGlobalFrontendLastTuningFrequencyKhz[TunerDeviceIndex],&RfLevelDbm,&fgRfMaxGain);

	Rssi = (RfLevelDbm / 1000);

	return Rssi;
}
//****************************************************************************************
//****************************************************************************************
//****************************************************************************************
RT710_Err_Type RT710_Init(Device_Context *DC,unsigned char TunerDeviceID)
{
    DriverDebugPrint("(RT710_Init)\n");
    DriverDebugPrint("(RT710_Init) TunerDeviceID : 0x%x\n",TunerDeviceID);

    return RT710_RT_Success;
}
RT710_Err_Type RT710_Setting(Device_Context *DC,unsigned char TunerDeviceID,RT710_INFO_Type RT710_Set_Info)
{
  	    unsigned char TunerDeviceIndex;
        RT710_I2C_TYPE RT710_Write_Byte;
        RT710_I2C_LEN_TYPE RT710_Write_Len;
		RT710_I2C_LEN_TYPE RT710_Read_Len;
        UINT32 R710_satellite_bw;
        UINT32 R710_pre_satellite_bw;
		int icount;
		int i;

		UINT8 fine_tune = 0;
		UINT8 Coarse = 0;
		UINT16 bw_offset = 20000;
		UINT32 offset_range = 0;

        DriverDebugPrint("(RT710_Init)\n");
        DriverDebugPrint("(RT710_Init) TunerDeviceID : 0x%x\n",TunerDeviceID);
        DriverDebugPrint("(RT710_Init) RT710_Set_Info.RF_KHz : %d\n",RT710_Set_Info.RF_KHz);
        DriverDebugPrint("(RT710_Init) RT710_Set_Info.SymbolRate_Kbps : %d\n",RT710_Set_Info.SymbolRate_Kbps);
        DriverDebugPrint("(RT710_Init) RT710_Set_Info.RT710_RollOff_Mode : 0x%x\n",RT710_Set_Info.RT710_RollOff_Mode);

        TunerDeviceIndex = TunerDeviceID;

		RT710_Write_Len.RegAddr=0x00;
		RT710_Write_Len.Len=0x10;

		RT710_Read_Len.RegAddr=0x00;
		RT710_Read_Len.Len=0x04;

//		if(1)
		{

            DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][0] = 0; 
            DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][1] = 0; 
            DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][2] = 0; 
            DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][3] = 0; 
		    DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] = TRUE;


			R710_pre_satellite_bw = 0; //Init BW Value
			R710_satellite_bw = 0;

			
			RT710_I2C_Read_Len(DC,TunerDeviceID,&RT710_Read_Len) ; // read data length
			if((RT710_Read_Len.Data[3]&0xF0)==0x70) //TRUE:RT710(R3[7:4]=7) ; FALSE;RT720(R3[7:4]=0)
				DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] = TRUE;
			else
				DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] = FALSE;
			


			if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == TRUE)//TRUE:RT710 ; FALSE;RT720
			{
				for(icount=0;icount<RT710_Reg_Num;icount++)
				{
					DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][icount] = RT710_Reg_Arry_Init[icount];
				}
			}
			else
			{
				for(icount=0;icount<RT710_Reg_Num;icount++)
				{
					DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][icount] = RT720_Reg_Arry_Init[icount];
				}
			}


			// LOOP_THROUGH(0=on ; 1=off)
			if(RT710_Set_Info.RT710_LoopThrough_Mode != LOOP_THROUGH)
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][1] |=0x04;
			}
			else
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][1] &=0xFB;
			}
			
			//Clock out(1=off ; 0=on)     
			if(RT710_Set_Info.RT710_ClockOut_Mode != ClockOutOn)
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][3] |=0x10;
			}
			else
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][3] &=0xEF;
			}

			//Output Signal Mode   
			if(RT710_Set_Info.RT710_OutputSignal_Mode != DifferentialOut)
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][11] |=0x10;
			}
			else
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][11] &=0xEF;
			}
			
			//AGC Type  //R13[4] Negative=0 ; Positive=1;
			if(RT710_Set_Info.RT710_AGC_Mode != AGC_Positive)
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][13] &=0xEF;
			}
			else
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][13] |=0x10;
			}

			//RT710_Vga_Sttenuator_Type
			if(RT710_Set_Info.RT710_AttenVga_Mode != ATTENVGAON)
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][11] &= 0xF7;
			}
			else
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][11] |= 0x08;
			}

			
			if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == TRUE)//TRUE:RT710 ; FALSE;RT720
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14] = (DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14] & 0xFC) | RT710_Set_Info.R710_FineGain;
			}
			else
			{
				if( RT710_Set_Info.R710_FineGain > 1)
					DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14] = (DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14] & 0xFE) | 0x01;
				else
					DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14] = (DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14] & 0xFE) | 0x00;

				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][3] = ((DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][3] & 0xF0) | ((30 - RT710_XTAL_CAP)>>1));
			}


			for(i=0;i<RT710_Write_Len.Len;i++)
			{
				RT710_Write_Len.Data[i] = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][i];
			}

			if(RT710_I2C_Write_Len(DC,TunerDeviceID,&RT710_Write_Len) != RT710_RT_Success)
				return RT710_RT_Fail;
		}


		// Check Input Frequency Range
		if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == TRUE)//TRUE:RT710 ; FALSE;RT720
		{
			if((RT710_Set_Info.RF_KHz<600000) || (RT710_Set_Info.RF_KHz>2400000))
				return RT710_RT_Fail;
		}
		else
		{
			if((RT710_Set_Info.RF_KHz<200000) || (RT710_Set_Info.RF_KHz>2400000))
				return RT710_RT_Fail;
		}

		if(RT710_PLL(DC,TunerDeviceID,RT710_Set_Info.RF_KHz)!=RT710_RT_Success)
			 return RT710_RT_Fail;

		RT710_Sleep(10);

		if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == TRUE)
		{
			if((RT710_Set_Info.RF_KHz >= 1600000) && (RT710_Set_Info.RF_KHz < 1950000))
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][2] |= 0x40; //LNA Mode with att
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][8] |= 0x80; //Mixer Buf -3dB
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10] =  (RT710_Reg_Arry_Init[10]); //AGC VTH:1.24V  ;  VTL:0.64V
				RT710_Write_Byte.RegAddr = 0x0A;
				RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10];
				if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
				return RT710_RT_Fail;
			}
			else
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][2] &= 0xBF; //LNA Mode no att
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][8] &= 0x7F; //Mixer Buf off
				if(RT710_Set_Info.RF_KHz >= 1950000)
				{
					DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10] = ((DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10] & 0xF0) | 0x08); //AGC VTH:1.14V
					DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10] = ((DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10] & 0x0F) | 0x30); //AGC VTL:0.64V
					RT710_Write_Byte.RegAddr = 0x0A;
					RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10];
					if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
						return RT710_RT_Fail;
				}
				else
				{
					DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10] =  (RT710_Reg_Arry_Init[10]); //AGC VTH:1.24V  ;  VTL:0.64V
					RT710_Write_Byte.RegAddr = 0x0A;
					RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][10];
					if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
						return RT710_RT_Fail;
				}
			}
			RT710_Write_Byte.RegAddr = 0x02;
			RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][2];
			if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
				return RT710_RT_Fail;
			RT710_Write_Byte.RegAddr = 0x08;
			RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][8];
			if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
				return RT710_RT_Fail;

			if(RT710_Set_Info.RF_KHz >= 2000000)
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14]=( DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14]& 0xF3)|0x08;
			}
			else
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14]=( DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14]& 0xF3)|0x00;
			}
			RT710_Write_Byte.RegAddr = 0x0E;
			RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][14];
			if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
				return RT710_RT_Fail;
		}
		else
		{
			//Scan_Type
			if(RT710_Set_Info.RT710_Scan_Mode != RT710_AUTO_SCAN)
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][11] &= 0xFC;
				if(RT710_Set_Info.SymbolRate_Kbps >= 15000)
				RT710_Set_Info.SymbolRate_Kbps += 6000;
			}
			else
			{
				DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][11] |= 0x02;
				RT710_Set_Info.SymbolRate_Kbps += 10000;

			}
			RT710_Write_Byte.RegAddr = 0x0B;
			RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][11];
			if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
				return RT710_RT_Fail;

		}


		switch (RT710_Set_Info.RT710_RollOff_Mode)
		{
		case ROLL_OFF_0_15:
			R710_satellite_bw = (UINT32)((RT710_Set_Info.SymbolRate_Kbps * 115)/10); 
			break;
		case ROLL_OFF_0_20:
			R710_satellite_bw = (UINT32)((RT710_Set_Info.SymbolRate_Kbps * 120)/10);
			break;
		case ROLL_OFF_0_25:
			R710_satellite_bw = (UINT32)((RT710_Set_Info.SymbolRate_Kbps * 125)/10);
			break;
		case ROLL_OFF_0_30:
			R710_satellite_bw = (UINT32)((RT710_Set_Info.SymbolRate_Kbps * 130)/10);
			break;
		case ROLL_OFF_0_35:
			R710_satellite_bw = (UINT32)((RT710_Set_Info.SymbolRate_Kbps * 135)/10);
			break;
		case ROLL_OFF_0_40:
			R710_satellite_bw = (UINT32)((RT710_Set_Info.SymbolRate_Kbps * 140)/10);
			break;
		}


		if(R710_satellite_bw != R710_pre_satellite_bw)
		{
			if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == TRUE)//TRUE:RT710 ; FALSE;RT720
			{
				if((R710_satellite_bw) >=380000)
				{
					fine_tune=1;
					Coarse =(UINT8) (( R710_satellite_bw -380000) /17400)+16;
					if((( R710_satellite_bw -380000) % 17400) > 0)
					Coarse+=1;
				}
				else if(R710_satellite_bw<=50000)
				{
					Coarse=0;
					fine_tune=0;
				}
				else if((R710_satellite_bw>50000) && (R710_satellite_bw<=73000))
				{
					Coarse=0;
					fine_tune=1;
				}
				else if((R710_satellite_bw>73000) && (R710_satellite_bw<=96000))
				{
					Coarse=1;
					fine_tune=0;
				}
				else if((R710_satellite_bw>96000) && (R710_satellite_bw<=104000))
				{
					Coarse=1;
					fine_tune=1;
				}
				else if((R710_satellite_bw>104000) && (R710_satellite_bw<=116000))
				{
					Coarse=2;
					fine_tune=0;
				}
				else if((R710_satellite_bw>116000) && (R710_satellite_bw<=126000))
				{
					Coarse=2;
					fine_tune=1;
				}
				else if((R710_satellite_bw>126000) && (R710_satellite_bw<=134000))
				{
					Coarse=3;
					fine_tune=0;
				}
				else if((R710_satellite_bw>134000) && (R710_satellite_bw<=146000))
				{
					Coarse=3;
					fine_tune=1;
				}
				else if((R710_satellite_bw>146000) && (R710_satellite_bw<=158000))
				{
					Coarse=4;
					fine_tune=0;
				}
				else if((R710_satellite_bw>158000) && (R710_satellite_bw<=170000))
				{
					Coarse=4;
					fine_tune=1;
				}
				else if((R710_satellite_bw>170000) && (R710_satellite_bw<=178000))
				{
					Coarse=5;
					fine_tune=0;
				}
				else if((R710_satellite_bw>178000) && (R710_satellite_bw<=190000))
				{
					Coarse=5;
					fine_tune=1;
				}
				else if((R710_satellite_bw>190000) && (R710_satellite_bw<=202000))
				{
					Coarse=6;
					fine_tune=0;
				}
				else if((R710_satellite_bw>202000) && (R710_satellite_bw<=212000))
				{
					Coarse=6;
					fine_tune=1;
				}
				else if((R710_satellite_bw>212000) && (R710_satellite_bw<=218000))
				{
					Coarse=7;
					fine_tune=0;
				}
				else if((R710_satellite_bw>218000) && (R710_satellite_bw<=234000))
				{
					Coarse=7;
					fine_tune=1;
				}
				else if((R710_satellite_bw>234000) && (R710_satellite_bw<=244000))
				{
					Coarse=9;
					fine_tune=1;
				}
				else if((R710_satellite_bw>244000) && (R710_satellite_bw<=246000))
				{
					Coarse=10;
					fine_tune=0;
				}
				else if((R710_satellite_bw>246000) && (R710_satellite_bw<=262000))
				{
					Coarse=10;
					fine_tune=1;
				}
				else if((R710_satellite_bw>262000) && (R710_satellite_bw<=266000))
				{
					Coarse=11;
					fine_tune=0;
				}
				else if((R710_satellite_bw>266000) && (R710_satellite_bw<=282000))
				{
					Coarse=11;
					fine_tune=1;
				}
				else if((R710_satellite_bw>282000) && (R710_satellite_bw<=298000))
				{
					Coarse=12;
					fine_tune=1;
				}
				else if((R710_satellite_bw>298000) && (R710_satellite_bw<=318000))
				{
					Coarse=13;
					fine_tune=1;
				}
				else if((R710_satellite_bw>318000) && (R710_satellite_bw<=340000))
				{
					Coarse=14;
					fine_tune=1;
				}
				else if((R710_satellite_bw>340000) && (R710_satellite_bw<=358000))
				{
					Coarse=15;
					fine_tune=1;
				}
				else if((R710_satellite_bw>358000) && (R710_satellite_bw<380000))
				{
					Coarse=16;
					fine_tune=1;
				}
			}
			else
			{
				if(RT710_Set_Info.RT710_RollOff_Mode>= 2 )
				fine_tune=1;
				else
				fine_tune=0;

				offset_range = fine_tune * bw_offset ;

				/*
				Symbol Rate <= 15MHz =>  +3MHz
				Symbol Rate 15MHz ~ 20MHz  =>  +2MHz
				Symbol Rate <= 30MHz =>  +1MHz
				*/
				if(RT710_Set_Info.SymbolRate_Kbps<=15000)
					RT710_Set_Info.SymbolRate_Kbps += 3000;
				else if(RT710_Set_Info.SymbolRate_Kbps<=20000)
					RT710_Set_Info.SymbolRate_Kbps += 2000;
				else if(RT710_Set_Info.SymbolRate_Kbps<=30000)
					RT710_Set_Info.SymbolRate_Kbps += 1000;
				else
					RT710_Set_Info.SymbolRate_Kbps += 0;

				if((RT710_Set_Info.SymbolRate_Kbps*12) < (88000+offset_range) )
				{
					Coarse=0;
				}
				else if((RT710_Set_Info.SymbolRate_Kbps*12) > (88000+offset_range) && (RT710_Set_Info.SymbolRate_Kbps*12) <= (368000+offset_range) )
				{
					Coarse =(UINT8) (( (RT710_Set_Info.SymbolRate_Kbps*12) - (88000+offset_range)) / 20000);
					if(((UINT16)((RT710_Set_Info.SymbolRate_Kbps*12) -(88000+offset_range)) % 20000) > 0)
						Coarse+=1;
					if(Coarse>=7)
						Coarse+=1;
				}		
				else if((RT710_Set_Info.SymbolRate_Kbps*12) > (368000+offset_range) && (RT710_Set_Info.SymbolRate_Kbps*12)<=(764000+offset_range))
				{
					Coarse =(UINT8) (( (RT710_Set_Info.SymbolRate_Kbps*12) -(368000+offset_range)) /20000) +15;
					if(((UINT16)( (RT710_Set_Info.SymbolRate_Kbps*12) -(368000+offset_range)) % 20000) > 0)
						Coarse+=1;
					if(Coarse>=33)
						Coarse+=3;
					else if(Coarse >= 29)
						Coarse+=2;
					else if(Coarse>=27)
						Coarse+=3;
					else if(Coarse>=24)
						Coarse+=2;
					else if(Coarse>=19)
						Coarse+=1;
					else
						Coarse+=0;

				}
				else
				{
					Coarse=42;
				}
				
			}
			//fine tune and coras filter write
			RT710_Write_Byte.RegAddr = 0x0F;
			RT710_Read_Len.Data[15] &= 0x00;
			DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][15] = ((RT710_Read_Len.Data[15] | ( fine_tune  ) ) | ( Coarse<<2));
			RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][15];
			if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
				return RT710_RT_Fail;
		}

		R710_pre_satellite_bw = R710_satellite_bw;

		for(icount=0;icount<4;icount++)
		{
			DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][icount] = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][icount];
		}
		return RT710_RT_Success;
}


RT710_Err_Type RT710_PLL(Device_Context *DC,unsigned char TunerDeviceID,UINT32 Freq)
{
	unsigned char TunerDeviceIndex;
    RT710_I2C_TYPE RT710_Write_Byte;

	UINT8  MixDiv   = 2;
	UINT8  DivBuf   = 0;
	UINT8  Ni       = 0;
	UINT8  Si       = 0;
	UINT8  DivNum   = 0;
	UINT8  Nint     = 0;
	UINT32 VCO_Min  = 2350000;
	if (Freq>=2350000)
	{
		VCO_Min  = Freq;
	}
	UINT32 VCO_Max  = VCO_Min*2;
	UINT32 VCO_Freq = 0;
	UINT32 PLL_Ref	= RT710_Xtal;	
	UINT32 VCO_Fra	= 0;		//VCO contribution by SDM (kHz)
	UINT16 Nsdm		= 2;
	UINT16 SDM		= 0;
	UINT16 SDM16to9	= 0;
	UINT16 SDM8to1  = 0;
	//UINT8  Judge    = 0;
	UINT8  VCO_fine_tune = 0;

    TunerDeviceIndex = TunerDeviceID;

	while(MixDiv <= 16)  // 2 -> 4 -> 8 -> 16
	{
		if(((Freq * MixDiv) >= VCO_Min) && ((Freq * MixDiv) <= VCO_Max))// Lo_Freq->Freq
		{
			if(MixDiv==2)
				DivNum = 1;
			else if(MixDiv==4)
				DivNum = 0;
			else if(MixDiv==8)
				DivNum = 2;
			else   //16
				DivNum = 3;
			break;
		}
		MixDiv = MixDiv << 1;
	}

	//Divider
	RT710_Write_Byte.RegAddr = 0x04;
	DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4] &= 0xFE;
	DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4] |= (DivNum & 0x01) ;
	RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4];
	if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
		return RT710_RT_Fail;

	if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == FALSE)//TRUE:RT710 ; FALSE;RT720
	{
		RT710_Write_Byte.RegAddr = 0x08;
		DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][8] &= 0xEF;
		DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][8] |= ((DivNum & 0x02)>>1) << 4;
		RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][8];
		if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
			return RT710_RT_Fail;
		//Depend on divider setting
		//If Div /2 or /4 : Div Current 150uA(10); other : Div Current 100uA(01) R4[7:6]
		//If Div /2 or /4 : PLL IQ Buf High(1); other : PLL IQ Buf Low(0) R12[4]
		if(DivNum <= 1)	// Div/2 or /4
		{
			DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4] &= 0x3F;
			DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4] |= 0x40;
			DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][12] |= 0x10;

		}
		else	//Div /8 or /16
		{
			DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4] &= 0x3F;
			DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4] |= 0x80;
			DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][12] &= 0xEF;

		}
		RT710_Write_Byte.RegAddr = 0x0C;
		RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][12];
		if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
			return RT710_RT_Fail;
		RT710_Write_Byte.RegAddr = 0x04;
		RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4];
		if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
			return RT710_RT_Fail;
	}

	VCO_Freq = Freq * MixDiv;		// Lo_Freq->Freq
	Nint     = (UINT8) (VCO_Freq / 2 / PLL_Ref);
	VCO_Fra  = (UINT16) (VCO_Freq - 2 * PLL_Ref * Nint);

	//boundary spur prevention
	if (VCO_Fra < PLL_Ref/64)             // 2*PLL_Ref/128
		VCO_Fra = 0;
	else if (VCO_Fra > PLL_Ref*127/64)    // 2*PLL_Ref*127/128
	{
		VCO_Fra = 0;
		Nint ++;
	}
	else if((VCO_Fra > PLL_Ref*127/128) && (VCO_Fra < PLL_Ref)) //> 2*PLL_Ref*127/256,  < 2*PLL_Ref*128/256
		VCO_Fra = PLL_Ref*127/128;      // VCO_Fra = 2*PLL_Ref*127/256
	else if((VCO_Fra > PLL_Ref) && (VCO_Fra < PLL_Ref*129/128)) //> 2*PLL_Ref*128/256,  < 2*PLL_Ref*129/256
		VCO_Fra = PLL_Ref*129/128;      // VCO_Fra = 2*PLL_Ref*129/256
	else
		VCO_Fra = VCO_Fra;

	//N & S	
	Ni       = (Nint - 13) / 4;
	Si       = Nint - 4 *Ni - 13;
	RT710_Write_Byte.RegAddr = 0x05;
	DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][5]  = 0x00;
	DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][5] |= (Ni + (Si << 6));
	RT710_Write_Byte.Data  = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][5];
	if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
		return RT710_RT_Fail;

	//pw_sdm 
	RT710_Write_Byte.RegAddr = 0x04;
	DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4] &= 0xFD;
	if(VCO_Fra == 0)
		DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4] |= 0x02;
	RT710_Write_Byte.Data = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][4];
	if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
		return RT710_RT_Fail;

	//SDM calculator
	while(VCO_Fra > 1)
	{			
		if (VCO_Fra > (2*PLL_Ref / Nsdm))
		{		
			SDM = SDM + 32768 / (Nsdm/2);
			VCO_Fra = VCO_Fra - 2*PLL_Ref / Nsdm;
			if (Nsdm >= 0x8000)
				break;
		}
		Nsdm = Nsdm << 1;
	}

	SDM16to9 = SDM >> 8;
	SDM8to1 =  SDM - (SDM16to9 << 8);

	RT710_Write_Byte.RegAddr = 0x07;
	DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][7]   = (UINT8) SDM16to9;
	RT710_Write_Byte.Data    = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][7];
	if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
		return RT710_RT_Fail;
	RT710_Write_Byte.RegAddr = 0x06;
	DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][6]   = (UINT8) SDM8to1;
	RT710_Write_Byte.Data    = DC->JackyFrontend.RT710_Reg_Arry[TunerDeviceIndex][6];
	if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
		return RT710_RT_Fail;
    return RT710_RT_Success;
}



RT710_Err_Type RT710_Standby(Device_Context *DC,unsigned char TunerDeviceID,RT710_LoopThrough_Type RT710_LTSel, RT710_ClockOut_Type RT710_CLKSel )
{
	unsigned char TunerDeviceIndex;
    RT710_I2C_TYPE RT710_Write_Byte;
    RT710_I2C_LEN_TYPE RT710_Write_Len;
	UINT8 RT710_Standby_Reg_Arry[RT710_Reg_Num]={0xFF, 0x5C, 0x88, 0x30, 0x41, 0xC8, 0xED, 0x25, 0x47, 0xFC, 0x48, 0xA2, 0x08, 0x0F, 0xF3, 0x59};
    int icount;
	int i;

    TunerDeviceIndex = TunerDeviceID;

	//Clock out(1=off ; 0=on)     
	if(RT710_CLKSel != ClockOutOn)
	{
		RT710_Standby_Reg_Arry[3] |=0x10;
	}
	else
	{
		RT710_Standby_Reg_Arry[3] &=0xEF;
	}
	if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == FALSE)//TRUE:RT710 ; FALSE;RT720
	{
		RT710_Standby_Reg_Arry[1] |=0x02;//lna off ;can off together
	
		RT710_Standby_Reg_Arry[3] |=0x20;//Xtal ldo off
		RT710_Write_Byte.RegAddr = 0x03;
		RT710_Write_Byte.Data = RT710_Standby_Reg_Arry[3];
		if(RT710_I2C_Write(DC,TunerDeviceID,&RT710_Write_Byte) != RT710_RT_Success)
			return RT710_RT_Fail;
		
	}


	RT710_Write_Len.RegAddr=0x00;
	RT710_Write_Len.Len=0x10;
	for(i=0;i<RT710_Write_Len.Len;i++)
	{
		RT710_Write_Len.Data[i]=RT710_Standby_Reg_Arry[i];
	}
		
	if(RT710_I2C_Write_Len(DC,TunerDeviceID,&RT710_Write_Len) != RT710_RT_Success)
		return RT710_RT_Fail;

	for(icount=0;icount<4;icount++)
	{
		DC->JackyFrontend.RT710_R0TOR3_Write_Arry[TunerDeviceIndex][icount] = RT710_Standby_Reg_Arry[icount];
	}

	return RT710_RT_Success;
}

//------------------------------------------------------------------//
//  RT710_PLL_Lock( ): Read PLL lock status (R2[7])        //
//  Return: 1: PLL lock                                                    //
//          0: PLL unlock                                                //
//------------------------------------------------------------------//
UINT8 RT710_PLL_Lock(Device_Context *DC,unsigned char TunerDeviceID)
{
	unsigned char TunerDeviceIndex;
	UINT8 fg_lock = 0;
	RT710_I2C_LEN_TYPE Dlg_I2C_Len;

    TunerDeviceIndex = TunerDeviceID;

	Dlg_I2C_Len.RegAddr = 0x00;
	Dlg_I2C_Len.Len = 3;
	if(RT710_I2C_Read_Len(DC,TunerDeviceID,&Dlg_I2C_Len) != RT710_RT_Success)
	{
	    RT710_I2C_Read_Len(DC,TunerDeviceID,&Dlg_I2C_Len);
	}

	if( (Dlg_I2C_Len.Data[2] & 0x80) == 0x00 ) 		
		fg_lock = 0;		
	else
        fg_lock = 1;

	return fg_lock;
}



RT710_Err_Type RT710_GetRfGain(Device_Context *DC,unsigned char TunerDeviceID,RT710_RF_Gain_Info *RT710_rf_gain)
{
	unsigned char TunerDeviceIndex;
	RT710_I2C_LEN_TYPE Dlg_I2C_Len;

    TunerDeviceIndex = TunerDeviceID;

	Dlg_I2C_Len.RegAddr = 0x00;
	Dlg_I2C_Len.Len     = 0x04;
	RT710_I2C_Read_Len(DC,TunerDeviceID,&Dlg_I2C_Len) ; // read data length
	RT710_rf_gain->RF_gain = (Dlg_I2C_Len.Data[1]>>4); //get rf gain
	RT710_rf_gain->RF_gain +=((Dlg_I2C_Len.Data[1] & 0x01)<<4);

	/*0~5: mixeramp
	  6~7: mix-buf
	  29~30:mix-buf
	  other:lna
	*/
	if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == TRUE)
	{
		if (RT710_rf_gain->RF_gain <= 2)
		{
			RT710_rf_gain->RF_gain=0;
		}
		else if(RT710_rf_gain->RF_gain > 2 && RT710_rf_gain->RF_gain <= 9) 
		{
			RT710_rf_gain->RF_gain -=2;
		}
		else if(RT710_rf_gain->RF_gain >9 && RT710_rf_gain->RF_gain <=12)
		{
			RT710_rf_gain->RF_gain = 7;
		}
		else if(RT710_rf_gain->RF_gain>12 && RT710_rf_gain->RF_gain <= 22)
		{
			RT710_rf_gain->RF_gain -= 5;
		}
		else if(RT710_rf_gain->RF_gain > 22)
		{
			RT710_rf_gain->RF_gain = 18;
		}
	}

	return RT710_RT_Success;
}

//----------------------------------------------------------------------//
//  RT710_GetRfRssi( ): Get RF RSSI                                      //
//  1st parameter: input RF Freq    (KHz)                               //
//  2rd parameter: output signal level (dBm*1000)                       //
//  3th parameter: output RF max gain indicator (0:min gain, 1:max gain, 2:active gain)	//
//-----------------------------------------------------------------------//
RT710_Err_Type RT710_GetRfRssi(Device_Context *DC,unsigned char TunerDeviceID,UINT32 RF_Freq_Khz, INT32 *RfLevelDbm, UINT8 *fgRfGainflag)
{ 
	unsigned char TunerDeviceIndex;
	RT710_RF_Gain_Info rf_gain_info;
	UINT8  u1Gain1;
	UINT16  acc_lna_gain;
	UINT16  rf_total_gain;
	UINT16  u2FreqFactor;
	INT32     rf_rssi;
	INT32    fine_tune = 0;    //for find tune 

    TunerDeviceIndex = TunerDeviceID;

	RT710_GetRfGain(DC,TunerDeviceID,&rf_gain_info);

	u1Gain1 = rf_gain_info.RF_gain;

   //max gain indicator
	if(((u1Gain1==18)&&(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == TRUE))||((u1Gain1==31)&&(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == FALSE)))  
		*fgRfGainflag = 1;
	else if(u1Gain1==0)
		*fgRfGainflag = 0;
	else
		*fgRfGainflag = 2;

	u2FreqFactor = 0;

	//Select LNA freq table
	if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == FALSE)
	{
		u2FreqFactor = 70;
	}
	else if(RF_Freq_Khz<1200000)   //<1200M
	{
		u2FreqFactor = 190;
	}
	else if((RF_Freq_Khz>=1200000)&&(RF_Freq_Khz<1800000))   //1200~1800M
	{
		u2FreqFactor = 170;
	}
	else    // >=2000MHz
	{
		u2FreqFactor = 140;
	}

	//LNA Gain
	if(DC->JackyFrontend.RT710_Chip_type_flag[TunerDeviceIndex] == FALSE)
		acc_lna_gain = RT710_Lna2_Acc_Gain[u1Gain1];
	else
		acc_lna_gain = RT710_Lna_Acc_Gain[u1Gain1];

	//Add Rf Buf and Mixer Gain
	rf_total_gain = acc_lna_gain;

	rf_rssi = fine_tune - (INT32) (rf_total_gain + u2FreqFactor);
	*RfLevelDbm = (rf_rssi*100);


    return RT710_RT_Success;
}

// ************************************************************************************
// ************************************************************************************
// ************************************************************************************
void RT710_Sleep(UINT32 Milliseconds)
{
//    usleep(Milliseconds * 1000); 
	msleep(Milliseconds);
}
BOOL RT710_I2C_Write_Len(Device_Context *DC,unsigned char TunerDeviceID,RT710_I2C_LEN_TYPE *I2C_Info)
{
    unsigned char I2CDeviceAddress;
	unsigned char* pWriteBuffer;
	BOOL Result = FALSE;

//    DriverDebugPrint("(RT710_I2C_Write_Len)\n");

    if(DC)
	{
 	   if(I2C_Info)
	   {
          pWriteBuffer = (unsigned char*)kmalloc(((sizeof(unsigned char) * (1 + I2C_Info->Len)) + 2), GFP_KERNEL);

	      if(pWriteBuffer)
		  {
             if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
                I2CDeviceAddress = 0x22;
	         else
                I2CDeviceAddress = 0x26;

             pWriteBuffer[0] = 0xFE;
             pWriteBuffer[1] = RT710_I2C_DEVICE_ADDRESS;
		     pWriteBuffer[2] = I2C_Info->RegAddr;
             memcpy(&pWriteBuffer[3],  I2C_Info->Data,  (sizeof(unsigned char) * I2C_Info->Len));
			
		     Result = I2C_PortingLayer_Write((Endeavour*) &DC->it9300,DC->JackyFrontend.JackyGlobalBackendI2CBusIndex[TunerDeviceID],I2CDeviceAddress,pWriteBuffer,((unsigned short)(sizeof(unsigned char) * (1 + I2C_Info->Len)) + 2));		  
		     if(Result == FALSE)
			 {
	            printk("(RT710_I2C_Write_Len) Failed !\n");
			 }

		     kfree(pWriteBuffer);
		  }
	   }
	}

 	return Result;
}
BOOL RT710_I2C_Read_Len(Device_Context *DC,unsigned char TunerDeviceID,RT710_I2C_LEN_TYPE *I2C_Info)
{
	unsigned long Loop;
	unsigned char ReadLength;
	unsigned char Data[50];
    unsigned char I2CDeviceAddress;
	unsigned char ReadCommand[3];
	BOOL Result = FALSE;

//    DriverDebugPrint("(RT710_I2C_Read_Len)\n");

    if(DC)
	{
   	   if(I2C_Info)
	   {
          if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
             I2CDeviceAddress = 0x22;
	      else
             I2CDeviceAddress = 0x26;

          ReadCommand[0] = 0xFE;
	      ReadCommand[1] = RT710_I2C_DEVICE_ADDRESS;
	      ReadCommand[2] = I2C_Info->RegAddr;

		  Result = I2C_PortingLayer_Write((Endeavour*) &DC->it9300,DC->JackyFrontend.JackyGlobalBackendI2CBusIndex[TunerDeviceID],I2CDeviceAddress,ReadCommand,3);
		  if(Result)
		  {
		     ReadLength = (unsigned char)I2C_Info->Len;
		     if(ReadLength > 50)
			    ReadLength = 50;

             ReadCommand[0] = 0xFE;
	         ReadCommand[1] = (RT710_I2C_DEVICE_ADDRESS | 0x01);

	         Result = I2C_PortingLayer_ReadNoStop((Endeavour*) &DC->it9300,DC->JackyFrontend.JackyGlobalBackendI2CBusIndex[TunerDeviceID],I2CDeviceAddress,ReadCommand,2,Data,ReadLength);
	         if(Result)
			 {
	            for(Loop=0;Loop<ReadLength;Loop++)
				{
		            I2C_Info->Data[Loop] = (unsigned char)RT710_Convert(Data[Loop]);
				}
			 }
		     else
			 {
	            printk("(RT710_I2C_Read_Len) Failed(2) !\n");
			 }
		  }
		  else
		  {
	         printk("(RT710_I2C_Read_Len) Failed(1) !\n");
		  }
	   }
	}

	return Result;
}
BOOL RT710_I2C_Write(Device_Context *DC,unsigned char TunerDeviceID,RT710_I2C_TYPE *I2C_Info)
{
    unsigned char I2CDeviceAddress;
	unsigned char  WriteBuffer[4];
	BOOL Result = FALSE;

//	DriverDebugPrint("(RT710_I2C_Write)\n");

    if(DC)
	{
	   if(I2C_Info)
	   {
          if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
             I2CDeviceAddress = 0x22;
	      else
             I2CDeviceAddress = 0x26;

          WriteBuffer[0] = 0xFE;
          WriteBuffer[1] = RT710_I2C_DEVICE_ADDRESS;
          WriteBuffer[2] = I2C_Info->RegAddr;
          WriteBuffer[3] = I2C_Info->Data;

		  Result = I2C_PortingLayer_Write((Endeavour*) &DC->it9300,DC->JackyFrontend.JackyGlobalBackendI2CBusIndex[TunerDeviceID],I2CDeviceAddress,WriteBuffer,4);
	      if(Result == FALSE)
		  {
	         printk("(RT710_I2C_Write) Failed !\n");
		  }
	   }
	}

	return Result;
}
int RT710_Convert(int InvertNum)
{
	int ReturnNum = 0;
	int AddNum    = 0x80;
	int BitNum    = 0x01;
	int CuntNum   = 0;

	for(CuntNum = 0;CuntNum < 8;CuntNum ++)
	{
		if(BitNum & InvertNum)
			ReturnNum += AddNum;

		AddNum /= 2;
		BitNum *= 2;
	}

	return ReturnNum;
}
// ************************************************************************************
// ************************************************************************************
// ************************************************************************************

























