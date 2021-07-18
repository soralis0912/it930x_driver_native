#include <linux/kernel.h> 
#include "it930x-core.h"
#include "CXD2856.h"
#include "MyI2CPortingLayer.h"
#include "MyDebug.h"

#include "sony_common.c"
#include "sony_demod.c"
#include "sony_integ.c"
#include "sony_stopwatch_port.c"
#include "sony_i2c.c"
#include "sony_math.c"
#include "sony_tuner_helene.c"
#include "sony_helene.c"
#include "sony_demod_isdbt.c"
#include "sony_demod_isdbt_monitor.c"
#include "sony_integ_isdbt.c"
#include "sony_demod_isdbc.c"
#include "sony_demod_isdbc_monitor.c"
#include "sony_integ_isdbc.c"
#include "sony_demod_isdbs.c"
#include "sony_demod_isdbs_monitor.c"
#include "sony_integ_isdbs.c"

// ************************************************************************************
// ************************************************************************************
// ************************************************************************************
sony_result_t I2C_JackyPorting_Read(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t* pData, uint32_t size, uint8_t mode)
{
	PCXD2856_USER_DATA pMyUserData = (PCXD2856_USER_DATA)pI2c->user;
	Device_Context* pDC;
	BOOL Result = FALSE;

	if(pMyUserData)
	{
       pDC = (Device_Context*)pMyUserData->pDeviceContext;
	   if(pDC)
	   {
          Result = I2C_PortingLayer_Read((Endeavour*) &pDC->it9300,pDC->JackyFrontend.JackyGlobalBackendI2CBusIndex[pMyUserData->TunerDeviceID],deviceAddress,pData,(unsigned char)size);
	      if(Result == FALSE)
		  {
	         DriverDebugPrint("(I2C_JackyPorting_Read) Failed !\n");
		  }
	   }
	}

    if(Result)
       return (SONY_RESULT_OK);
	else
       return (SONY_RESULT_ERROR_I2C);
}
sony_result_t I2C_JackyPorting_Write(sony_i2c_t* pI2c, uint8_t deviceAddress, const uint8_t * pData, uint32_t size, uint8_t mode)
{
	PCXD2856_USER_DATA pMyUserData = (PCXD2856_USER_DATA)pI2c->user;
	Device_Context* pDC;
	BOOL Result = FALSE;

	if(pMyUserData)
	{
       pDC = (Device_Context*)pMyUserData->pDeviceContext;
	   if(pDC)
	   {
          Result = I2C_PortingLayer_Write((Endeavour*) &pDC->it9300,pDC->JackyFrontend.JackyGlobalBackendI2CBusIndex[pMyUserData->TunerDeviceID],deviceAddress,pData,size);
	      if(Result == FALSE)
		  {
	         DriverDebugPrint("(I2C_JackyPorting_Write) Failed !\n");
		  }
	   }
	}

    if(Result)
       return (SONY_RESULT_OK);
	else
       return (SONY_RESULT_ERROR_I2C);
}
sony_result_t I2C_JackyPorting_ReadRegister(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress, uint8_t* pData, uint32_t size)
{
	PCXD2856_USER_DATA pMyUserData = (PCXD2856_USER_DATA)pI2c->user;
	Device_Context* pDC;
	BOOL Result = FALSE;

	if(pMyUserData)
	{
       pDC = (Device_Context*)pMyUserData->pDeviceContext;
	   if(pDC)
	   {
	      Result = I2C_PortingLayer_ReadNoStop((Endeavour*) &pDC->it9300,pDC->JackyFrontend.JackyGlobalBackendI2CBusIndex[pMyUserData->TunerDeviceID],deviceAddress,&subAddress,1,pData,(unsigned char)size);
	      if(Result == FALSE)
		  {
	         DriverDebugPrint("(I2C_JackyPorting_ReadRegister) Failed !\n");
		  }
	   }
	}

    if(Result)
       return (SONY_RESULT_OK);
	else
       return (SONY_RESULT_ERROR_I2C);
}
// ************************************************************************************
// ************************************************************************************
// ************************************************************************************
BOOL CXD2856_Initialization(Device_Context *DC,unsigned char TunerDeviceID)
{
    sony_demod_create_param_t createParam;
	sony_demod_iffreq_config_t IffreqConfig;
    sony_result_t ret;
	BOOL Result = FALSE;

    DriverDebugPrint("(CXD2856_Initialization)\n");
    DriverDebugPrint("(CXD2856_Initialization) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
	   DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].pDeviceContext = (void*)DC;
	   DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].TunerDeviceID = TunerDeviceID;

       createParam.xtalFreq = SONY_DEMOD_XTAL_24000KHz; // 24MHz Xtal 

	   if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
		  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U)
	   {
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
             createParam.i2cAddressSLVT = 0xCA; /* I2C slave address is 0xCA */
	      else
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1) 
             createParam.i2cAddressSLVT = 0xD8; /* I2C slave address is 0xD8 */
	      else
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 2) 
             createParam.i2cAddressSLVT = 0xC8; /* I2C slave address is 0xC8 */
	      else
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 3) 
             createParam.i2cAddressSLVT = 0xD8; /* I2C slave address is 0xD8 */
	      else
             createParam.i2cAddressSLVT = 0xC8; /* I2C slave address is 0xC8 */
	   }
       else
	   if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3)
	   {
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
             createParam.i2cAddressSLVT = 0xCA; /* I2C slave address is 0xCA */
	      else
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1) 
             createParam.i2cAddressSLVT = 0xD8; /* I2C slave address is 0xD8 */
	      else
             createParam.i2cAddressSLVT = 0xC8; /* I2C slave address is 0xC8 */
	   }
	   else
	   if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5)
	   {
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
             createParam.i2cAddressSLVT = 0xCA; /* I2C slave address is 0xCA */
	      else
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1) 
             createParam.i2cAddressSLVT = 0xC8; /* I2C slave address is 0xD8 */
	      else
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 2) 
             createParam.i2cAddressSLVT = 0xD8; /* I2C slave address is 0xC8 */
	      else
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 3) 
             createParam.i2cAddressSLVT = 0xD8; /* I2C slave address is 0xD8 */
	      else
             createParam.i2cAddressSLVT = 0xC8; /* I2C slave address is 0xC8 */
	   }
       else
	   {
          // EEPROM_PXMLT8UTS 
	      if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
             createParam.i2cAddressSLVT = 0xC8; /* I2C slave address is 0xCA */
	      else
             createParam.i2cAddressSLVT = 0xD8; /* I2C slave address is 0xD8 */
	   }

       createParam.tunerI2cConfig = SONY_DEMOD_TUNER_I2C_CONFIG_REPEATER; // SONY_DEMOD_TUNER_I2C_CONFIG_REPEATER; // SONY_DEMOD_TUNER_I2C_CONFIG_DISABLE; 

	   DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].Read = I2C_JackyPorting_Read;
	   DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].Write = I2C_JackyPorting_Write;
	   DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].ReadRegister = I2C_JackyPorting_ReadRegister;
	   DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].WriteRegister = sony_i2c_CommonWriteRegister;
	   DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].WriteOneRegister = sony_i2c_CommonWriteOneRegister;
	   DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].gwAddress = 0;
	   DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].gwSub = 0;
	   DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].user = (void*)&DC->JackyFrontend.CXD2856_UserData[TunerDeviceID];

       ret = sony_tuner_helene_Create(&DC->JackyFrontend.CXD2856_SonyTunerTerrestrialCable[TunerDeviceID],&DC->JackyFrontend.CXD2856_SonyTunerSatellite[TunerDeviceID],SONY_HELENE_XTAL_16000KHz,0xC0,&DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID],0,&DC->JackyFrontend.CXD2856_SonyHelene[TunerDeviceID]);
       if(ret == SONY_RESULT_OK)
	   {
          ret = sony_integ_Create(&DC->JackyFrontend.CXD2856_SonyInteg[TunerDeviceID],&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID],&createParam,&DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID],&DC->JackyFrontend.CXD2856_SonyTunerTerrestrialCable[TunerDeviceID],&DC->JackyFrontend.CXD2856_SonyTunerSatellite[TunerDeviceID]);               
          if(ret == SONY_RESULT_OK)
	      {
             ret = sony_integ_Initialize(&DC->JackyFrontend.CXD2856_SonyInteg[TunerDeviceID]);
             if(ret == SONY_RESULT_OK)
		     {

                ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_PARALLEL_SEL, 0);
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_PARALLEL_SEL)) : 0x%x\n",ret);

			    ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_SER_DATA_ON_MSB, 0);
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_SER_DATA_ON_MSB)) : 0x%x\n",ret);

			    ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_LATCH_ON_POSEDGE, 1);  
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_LATCH_ON_POSEDGE)) : 0x%x\n",ret);

			    ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_TSCLK_CONT, 0);   
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_TSCLK_CONT)) : 0x%x\n",ret);

			    ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_TSCLK_MASK, 0x03);    
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_TSCLK_MASK)) : 0x%x\n",ret);

			    ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_TSVALID_MASK, 0x03); 
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_TSVALID_MASK)) : 0x%x\n",ret);

			    ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_TSERR_MASK, 0x03);
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_TSERR_MASK)) : 0x%x\n",ret);


	              ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_SAT_IFAGCNEG, 1);
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_SAT_IFAGCNEG)) : 0x%x\n",ret);

	              ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_TERR_CABLE_TS_SERIAL_CLK_FREQ, 5);   // 38.4Mbps for ISDB-T
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_TERR_CABLE_TS_SERIAL_CLK_FREQ)) : 0x%x\n",ret);


	            ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_TERR_CABLE_TS_SERIAL_CLK_FREQ, 3);   // 64Mbps TS clock  
    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_TERR_CABLE_TS_SERIAL_CLK_FREQ)) : 0x%x\n",ret);


//	            ret = sony_demod_SetConfig(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], SONY_DEMOD_CONFIG_SPECTRUM_INV, 1);   // input IF spectrum
//    DriverDebugPrint("(CXD2856_Initialization) ret(sony_demod_SetConfig(SONY_DEMOD_CONFIG_SPECTRUM_INV)) : 0x%x\n",ret);


	            Result = TRUE;
		     }
	      }
	   }
	}

	return Result;
}
void CXD2856_Uninitialization(Device_Context *DC,unsigned char TunerDeviceID)
{
    DriverDebugPrint("(CXD2856_Uninitialization)\n");
}
BOOL CXD2856_ResetInternal(Device_Context *DC,unsigned char TunerDeviceID)
{
    DriverDebugPrint("(CXD2856_ResetInternal)\n");

    return TRUE;
}
BOOL CXD2856_SetFrequency(Device_Context *DC,unsigned char TunerDeviceID)
{
    sony_isdbt_tune_param_t ISDBTTuneParameter;
    sony_result_t ret;
 	BOOL Result = FALSE;

    DriverDebugPrint("(CXD2856_SetFrequency)\n");
    DriverDebugPrint("(CXD2856_SetFrequency) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
	   DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].pDeviceContext = (void*)DC;
	   DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].TunerDeviceID = TunerDeviceID;
       DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].user = (void*)&DC->JackyFrontend.CXD2856_UserData[TunerDeviceID];

       if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
	   {
          ISDBTTuneParameter.centerFreqKHz = DC->JackyFrontend.JackyGlobalFrontendRequestTuningFrequencyKhz[TunerDeviceID];                
		  if(DC->JackyFrontend.JackyGlobalFrontendRequestTuningBandwidthKhz[TunerDeviceID] == 6000)
             ISDBTTuneParameter.bandwidth = SONY_DTV_BW_6_MHZ;                                                                         /**< Bandwidth of the ISDB-T channel */
		  else
		  if(DC->JackyFrontend.JackyGlobalFrontendRequestTuningBandwidthKhz[TunerDeviceID] == 7000)
             ISDBTTuneParameter.bandwidth = SONY_DTV_BW_7_MHZ;                                                                         /**< Bandwidth of the ISDB-T channel */
		  else
             ISDBTTuneParameter.bandwidth = SONY_DTV_BW_8_MHZ;                                                                         /**< Bandwidth of the ISDB-T channel */

          /* Perform ISDB-T Tune */
           ret = sony_integ_isdbt_Tune(&DC->JackyFrontend.CXD2856_SonyInteg[TunerDeviceID], &ISDBTTuneParameter);
		   if(ret == SONY_RESULT_OK || ret == SONY_RESULT_ERROR_TIMEOUT || ret == SONY_RESULT_ERROR_UNLOCK)
              Result = TRUE;
	   }
	   else
		  Result = TRUE;
	}

    return Result;
}
BOOL CXD2856_SetTSID(Device_Context *DC,unsigned char TunerDeviceID,unsigned short TSID)
{
    sony_isdbs_tune_param_t ISDBSTuneParameter;
    sony_result_t ret;
	BOOL Result = FALSE;

    DriverDebugPrint("(CXD2856_SetTSID)\n");
    DriverDebugPrint("(CXD2856_SetTSID) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
	   DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].pDeviceContext = (void*)DC;
	   DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].TunerDeviceID = TunerDeviceID;
       DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].user = (void*)&DC->JackyFrontend.CXD2856_UserData[TunerDeviceID];

       if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
	   {
          DriverDebugPrint("(CXD2856_SetTSID) TuningFrequencyKhz : %d\n",DC->JackyFrontend.JackyGlobalFrontendRequestTuningFrequencyKhz[TunerDeviceID]);

          ISDBSTuneParameter.centerFreqKHz = (DC->JackyFrontend.JackyGlobalFrontendRequestTuningFrequencyKhz[TunerDeviceID] - 10678000);             
          ISDBSTuneParameter.tsid = TSID;                                                                  
          ISDBSTuneParameter.tsidType = SONY_ISDBS_TSID_TYPE_TSID;                                          

          DriverDebugPrint("(CXD2856_SetTSID) ISDBSTuneParameter.centerFreqKHz : %d\n",ISDBSTuneParameter.centerFreqKHz);
          DriverDebugPrint("(CXD2856_SetTSID) ISDBSTuneParameter.tsid : 0x%x\n",ISDBSTuneParameter.tsid);

          /* Perform ISDBS Tune */
          ret = sony_integ_isdbs_Tune(&DC->JackyFrontend.CXD2856_SonyInteg[TunerDeviceID], &ISDBSTuneParameter);
		  if(ret == SONY_RESULT_OK || ret == SONY_RESULT_ERROR_TIMEOUT || ret == SONY_RESULT_ERROR_UNLOCK)
             Result = TRUE;
	   }
	}

    DriverDebugPrint("(CXD2856_SetTSID) Result : 0x%x\n",Result);

    return Result;
}
BOOL CXD2856_ChannelSearch(Device_Context *DC,unsigned char TunerDeviceID)
{
    DriverDebugPrint("(CXD2856_ChannelSearch)\n");

    return TRUE;
}
BOOL CXD2856_CheckLock(Device_Context *DC,unsigned char TunerDeviceID,BOOL* pLock)
{
	sony_demod_lock_result_t LockResult;
    sony_result_t ret = SONY_RESULT_ERROR_OTHER;
	BOOL Result = FALSE;

    DriverDebugPrint("(CXD2856_CheckLock)\n");
    DriverDebugPrint("(CXD2856_CheckLock) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pLock)
	{
	   *pLock = FALSE;

	   if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].pDeviceContext = (void*)DC;
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].TunerDeviceID = TunerDeviceID;
          DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].user = (void*)&DC->JackyFrontend.CXD2856_UserData[TunerDeviceID];

	      if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		  {
	         ret = IsISDBTDmdLocked(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID]);
		     if(ret == SONY_RESULT_OK)
                *pLock = TRUE; 
		  }
	      else
		  {
             ret = IsISDBSDmdLocked(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID]);

    DriverDebugPrint("(CXD2856_CheckLock) ret(IsISDBSDmdLocked) : 0x%x\n",ret);

		     if(ret == SONY_RESULT_OK)
                *pLock = TRUE; 
		  }

	      DriverDebugPrint("(CXD2856_CheckLock) *pLock : 0x%x\n",(*pLock));

		  Result = TRUE;
	   }
	}

    DriverDebugPrint("(CXD2856_CheckLock) Result : 0x%x\n",Result);

    return Result;
}
BOOL CXD2856_GetStrength(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pStrength)
{
	int32_t rfLevel;
	int32_t A;
    sony_result_t ret;
	BOOL Result = FALSE;

    DriverDebugPrint("(CXD2856_GetStrength)\n");
    DriverDebugPrint("(CXD2856_GetStrength) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pStrength)
	{
	   *pStrength = 0;

	   if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].pDeviceContext = (void*)DC;
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].TunerDeviceID = TunerDeviceID;
          DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].user = (void*)&DC->JackyFrontend.CXD2856_UserData[TunerDeviceID];

		  if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		  {
		     ret = sony_integ_isdbt_monitor_RFLevel(&DC->JackyFrontend.CXD2856_SonyInteg[TunerDeviceID], &rfLevel);
	        if(ret == SONY_RESULT_OK) 
			{
			   DriverDebugPrint("(MultiTuner_TunerGetStrength) rfLevel : %d",rfLevel);

               A = (rfLevel / 1000);
               A += 100;
		       *pStrength = (unsigned char)(A & 0xFF);
			}
			else
			   *pStrength = 0;
		  }
		  else
		  {
			 ret = sony_integ_isdbs_monitor_RFLevel(&DC->JackyFrontend.CXD2856_SonyInteg[TunerDeviceID], &rfLevel);
	         if(ret == SONY_RESULT_OK) 
			 {
				DriverDebugPrint("(MultiTuner_TunerGetStrength) rfLevel : %d",rfLevel);

                A = (rfLevel / 1000);
                A += 100;
		        *pStrength = (unsigned char)(A & 0xFF);
			 }
			 else
			    *pStrength = 0;
		  }

	      DriverDebugPrint("(CXD2856_GetStrength) *pStrength : %d\n",(*pStrength));

		  Result = TRUE;
	   }
	}

    return Result;
} 
BOOL CXD2856_GetQuality(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pQuality)
{
    int32_t snr = 0;
    int32_t cnr = 0;
	int32_t A;
    sony_result_t ret = SONY_RESULT_ERROR_OTHER;
	BOOL Result = FALSE;

    DriverDebugPrint("(CXD2856_GetQuality)\n");
    DriverDebugPrint("(CXD2856_GetQuality) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pQuality)
	{
	   *pQuality = 0;

	   if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].pDeviceContext = (void*)DC;
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].TunerDeviceID = TunerDeviceID;
          DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].user = (void*)&DC->JackyFrontend.CXD2856_UserData[TunerDeviceID];

	      if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		  {
             ret = sony_demod_isdbt_monitor_SNR(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], &snr);
	         if(ret == SONY_RESULT_OK) 
			 {
				DriverDebugPrint("(CXD2856_GetQuality) snr : %d",snr);

                A = (snr / 1000);
                A *= 100;
		        A /= 30;
		        *pQuality = (unsigned char)(A & 0xFF);
			 }
			 else
			    *pQuality = 0;
		  }
	      else
		  {
             ret = sony_demod_isdbs_monitor_CNR(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], &cnr);
	         if(ret == SONY_RESULT_OK) 
			 {
				DriverDebugPrint("(CXD2856_GetQuality) cnr : %d",snr);

                A = (cnr / 1000);
                A *= 100;
		        A /= 30;
		        *pQuality = (unsigned char)(A & 0xFF);
			 }
			 else
			    *pQuality = 0;
		  }

	      DriverDebugPrint("(CXD2856_GetQuality) *pQuality : %d\n",(*pQuality));

		  Result = TRUE;
	   }
	}

	return Result;
}
BOOL CXD2856_GetCNRRaw(Device_Context *DC,unsigned char TunerDeviceID,unsigned short* pCNRRaw)
{
    int32_t snr = 0;
    int32_t cnr = 0;
    sony_result_t ret;
	BOOL Result = FALSE;

    DriverDebugPrint("(CXD2856_GetCNRRaw)\n");
    DriverDebugPrint("(CXD2856_GetCNRRaw) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pCNRRaw)
	{
	   *pCNRRaw = 0;

	   if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].pDeviceContext = (void*)DC;
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].TunerDeviceID = TunerDeviceID;
          DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].user = (void*)&DC->JackyFrontend.CXD2856_UserData[TunerDeviceID];

	      if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		  {
			 ret = sony_demod_isdbt_monitor_SNR(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], &snr);
			 if(ret == SONY_RESULT_OK)
			 {
                *pCNRRaw = (snr / 10);
			 }
		  }
	      else
		  {
			 ret = sony_demod_isdbs_monitor_CNR(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], &cnr);
			 if(ret == SONY_RESULT_OK)
			 {
                *pCNRRaw = (cnr / 10);
			 }
		  }

	      DriverDebugPrint("(CXD2856_GetCNRRaw) *pCNRRaw : %d\n",(*pCNRRaw));

		  Result = TRUE;
	   }
	}

	return Result;
}
BOOL CXD2856_GetErrorRate(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pErrorRate)
{
	uint32_t perA, perB, perC;
	uint32_t perH, perL;
	uint64_t per64;
    sony_result_t ret = SONY_RESULT_ERROR_OTHER;
 	BOOL Result = FALSE;

    DriverDebugPrint("(CXD2856_GetErrorRate)\n");
    DriverDebugPrint("(CXD2856_GetErrorRate) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pErrorRate)
	{
	   *pErrorRate = 100;

	   if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].pDeviceContext = (void*)DC;
	      DC->JackyFrontend.CXD2856_UserData[TunerDeviceID].TunerDeviceID = TunerDeviceID;
          DC->JackyFrontend.CXD2856_SonyI2c[TunerDeviceID].user = (void*)&DC->JackyFrontend.CXD2856_UserData[TunerDeviceID];

	      if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		  {
             ret = sony_demod_isdbt_monitor_PER(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], &perA, &perB, &perC);
		     if(ret == SONY_RESULT_OK)
                *pErrorRate = (perB /10000);
		  }
	      else
		  {
             ret = sony_demod_isdbs_monitor_PER(&DC->JackyFrontend.CXD2856_SonyDemod[TunerDeviceID], &perH, &perL);
             if(ret == SONY_RESULT_OK)
			 {
				per64 = perH;
				per64 <<= 32;
				per64 |= perL;
                *pErrorRate = (uint32_t)(per64 /10000);     // Fixme
			 }
		  }

	      DriverDebugPrint("(CXD2856_GetErrorRate) *pErrorRate : %d\n",(*pErrorRate));

	      Result = TRUE;
	   }
	}

	return Result;
}