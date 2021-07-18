
#include "it930x-core.h"
#include "R850.h"
#include "RT710.h"
#include "TC90522.h"
#include "CXD2856.h"
#include "MyDebug.h"

BOOL CanTheCaptureDeviceBePowerOff(Device_Context *DC);

//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
Dword DRV_MyFrontend_Initialize(Device_Context *DC, Byte tsSrcIdx) 
{
    DriverDebugPrint("(DRV_MyFrontend_Initialize)\n");
//    DriverDebugPrint("(DRV_MyFrontend_Initialize) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_Initialize) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == False)
		  {
             DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] = tsSrcIdx;

		     DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] = False;
		     DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[tsSrcIdx] = False;
		     DC->JackyFrontend.JackyGlobalFrontendTunerInitializationFlag[tsSrcIdx] = False;
		     DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] = False;
			 DC->JackyFrontend.JackyGlobalFrontendLastTuningFrequencyKhz[tsSrcIdx] = 0;
			 DC->JackyFrontend.JackyGlobalFrontendLastTuningBandwidthKhz[tsSrcIdx] = 0;
			 DC->JackyFrontend.JackyGlobalFrontendLastTuningSignalModulationType[tsSrcIdx] = UNKNOWN_CONSTELLATION;
			 DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulationMode[tsSrcIdx] = UNKNOWN_DTV_SYSTEM;
			 DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulatorLockStatus[tsSrcIdx] = False;

	         DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx]  = True;
		  }
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_DeInitialize(Device_Context *DC, Byte tsSrcIdx)
{
	unsigned char Loop;

    DriverDebugPrint("(DRV_MyFrontend_DeInitialize)\n");
//    DriverDebugPrint("(DRV_MyFrontend_DeInitialize) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_DeInitialize) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
          if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] = 0xFF;

             DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] = False; 
		  }
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_Open(Device_Context *DC, Byte tsSrcIdx)
{
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_Open)\n");
//    DriverDebugPrint("(DRV_MyFrontend_Open) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_Open) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == False)
			 {
                if(DC->JackyFrontend.JackyGlobalFrontendSystemPowerFlag == False)
				{
                   DriverDebugPrint("(DRV_MyFrontend_Open) Frontend Power On !\n");

                   if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5 ||					  
					  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
				   {
					  DriverDebugPrint("(DRV_MyFrontend_Open) Plex products\n");

 	                  IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh2_en, 0x01);    // enable gpio2 output
					  IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh2_on, 0x01);    // gpio2 function on
 	                  IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh7_en, 0x01);    // enable gpio7 output
					  IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh7_on, 0x01);    // gpio7 function on
					  if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT)
					  {
 	                     IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh11_en, 0x01);   // enable gpio11 output
					     IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh11_on, 0x01);   // gpio11 function on
	                     mdelay(20);
					  }

                      IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh2_o, 0x1);      // active : 0 , inactive : 1
                      IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh7_o, 0x0);      // on : 0 , off : 1
	                  mdelay(20);
	                  IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh2_o, 0x0);      // active : 0 , inactive : 1
	                  mdelay(20);
                      IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh2_o, 0x1);      // active : 0 , inactive : 1
	                  mdelay(20);
				   }

                   DC->JackyFrontend.JackyGlobalFrontendSystemPowerFlag = True;
				}

                DriverDebugPrint("(DRV_MyFrontend_Open) DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[%d] : 0x%x\n",tsSrcIdx,DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[tsSrcIdx]);

                if(DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[tsSrcIdx] == False)
				{
                   DriverDebugPrint("(DRV_MyFrontend_Open) DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[%d] : 0x%x\n",tsSrcIdx,DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx]);

                   if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == False)
				   {
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
						 if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
						    Result = CXD2856_Initialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
						 }
						 else
						 {
							if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
							{
		                       Demodulator_initialize((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,StreamType_DVBT_SERIAL);
		                       Demodulator_setMultiplier((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,Multiplier_2X);
					           Demodulator_enableControlPowerSaving((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,0);

							   Result = TRUE;
							}
							else
							{
                               if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
								  (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							   {
                                  Result = CXD2856_Initialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
							   }
							   else
							   {
		                          Demodulator_initialize((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,StreamType_DVBT_SERIAL);
		                          Demodulator_setMultiplier((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,Multiplier_2X);
					              Demodulator_enableControlPowerSaving((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,0);

                                  Result = TRUE;  
							   }
							}
						 }
					  }
					  else
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
					     Result = TC90522_Initialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
					  else
						 Result = FALSE;
	                  if(Result)
				      {
				         DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] = True;
				      }
					  else
						 return Error_FRONTEND_OPERATION_FAILED;
				   }

                   DriverDebugPrint("(DRV_MyFrontend_Open) DC->JackyFrontend.JackyGlobalFrontendTunerInitializationFlag[%d] : 0x%x\n",tsSrcIdx,DC->JackyFrontend.JackyGlobalFrontendTunerInitializationFlag[tsSrcIdx]);

				   if(DC->JackyFrontend.JackyGlobalFrontendTunerInitializationFlag[tsSrcIdx] == False)
				   {
                      if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
					  {
                         Result = R850_Initialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
						 if(Result)
						 {
                            Result = RT710_Initialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
						 }
					  }
					  else
					  {
						 // PXMLT5PE , PXMLT5U , PXMLT8PE and PXMLT8U do not need to initialize tuner
                         Result = TRUE;
					  }
                      if(Result)
				      {
				         DC->JackyFrontend.JackyGlobalFrontendTunerInitializationFlag[tsSrcIdx] = True;
				      }
					  else
						 return Error_FRONTEND_OPERATION_FAILED;
				   }

				   DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[tsSrcIdx] = True;
				}

				DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] = True;
			 }
		  }
	      else
	         return Error_FRONTEND_NOT_INITIALIZATION;
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_Close(Device_Context *DC, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_Close)\n");
//    DriverDebugPrint("(DRV_MyFrontend_Close) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_Close) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			 {
                DriverDebugPrint("(DRV_MyFrontend_Close) DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[%d] : 0x%x\n",tsSrcIdx,DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[tsSrcIdx]);

	            if(DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[tsSrcIdx] == True)
				{
                   if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
				   {
                      if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx] == ISDBT_MODE)
						 R850_Uninitialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
					  else
					  if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx] == ISDBS_MODE)
						 RT710_Uninitialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);

                      TC90522_Uninitialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
				   }
				   else
				   if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				      DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
				   {
					  if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
						 DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
					  {
                         CXD2856_Uninitialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
					  }
					  else					  
					  {
						 if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
						 {
                            Demodulator_acquireChannel((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,6000,(55143000 / 1000));     // Work around for TS package drop issue after the PC wakes up from sleep mode
                            Demodulator_finalize((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev);
						 }
						 else
						 {
                            if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							   (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							{
                               CXD2856_Uninitialization(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
							}
							else
							{
                               Demodulator_acquireChannel((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,6000,(55143000 / 1000));     // Work around for TS package drop issue after the PC wakes up from sleep mode
                               Demodulator_finalize((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev);
							}
						 }
					  }
				   }

                   DC->JackyFrontend.JackyGlobalFrontendTunerInitializationFlag[tsSrcIdx] = FALSE;

				   DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] = FALSE;

                   DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[tsSrcIdx] = False;
				}

                if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] & 0x03) == 1)
                   DC->JackyFrontend.JackyGlobalFrontendS0LNBPowerEnableFlag = False;
                if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] & 0x03) == 3)
                   DC->JackyFrontend.JackyGlobalFrontendS1LNBPowerEnableFlag = False;

			    if(DC->JackyFrontend.JackyGlobalFrontendS0LNBPowerEnableFlag == False && DC->JackyFrontend.JackyGlobalFrontendS1LNBPowerEnableFlag == False)
			    {
                   if(DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag == True)
				   {
                      if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5 ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
						 if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
                            IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh11_o, 0x0);     // on : 1 , off : 0

							DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag = False;
						 }
						 else
						 {
                            if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							   (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							{
                               IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh11_o, 0x0);     // on : 1 , off : 0

							   DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag = False;
							}
						 }
					  }
				   }
			    }

				if(CanTheCaptureDeviceBePowerOff(DC) == TRUE)
			    {
                   DriverDebugPrint("(DRV_MyFrontend_Close) DC->JackyFrontend.JackyGlobalFrontendSystemPowerFlag : 0x%x\n",DC->JackyFrontend.JackyGlobalFrontendSystemPowerFlag);

                   if(DC->JackyFrontend.JackyGlobalFrontendSystemPowerFlag == True)
				   {
	                  DriverDebugPrint("(DRV_MyFrontend_Close) Frontend Power Off !\n");

                      if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5 ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
                         IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh7_o, 0x1);      // on : 0 , off : 1
	                     mdelay(100);
					  }

                      DC->JackyFrontend.JackyGlobalFrontendSystemTsOutputConfigFlag = False;
                      DC->JackyFrontend.JackyGlobalFrontendSystemPowerFlag = False; 
			   	   }
			    }

				DC->JackyFrontend.JackyGlobalFrontendLastTuningFrequencyKhz[tsSrcIdx] = 0;
				DC->JackyFrontend.JackyGlobalFrontendLastTuningBandwidthKhz[tsSrcIdx] = 0;
				DC->JackyFrontend.JackyGlobalFrontendLastTuningSignalModulationType[tsSrcIdx] = UNKNOWN_CONSTELLATION;
				DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulationMode[tsSrcIdx] = UNKNOWN_DTV_SYSTEM;
				DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulatorLockStatus[tsSrcIdx] = False;

				DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] = False;
			 }
		  }
	      else
	         return Error_FRONTEND_NOT_INITIALIZATION;
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

DriverDebugPrint("(DRV_MyFrontend_Close) Succeed !\n");


	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_isOpen(Device_Context *DC, Bool* pOpenFlag, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_Close)\n");
//    DriverDebugPrint("(DRV_MyFrontend_isOpen) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_Close) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(pOpenFlag)
	   {
	      if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
		  {
	         if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
			 {
				*pOpenFlag = DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx];			
			 }
	         else
	            return Error_FRONTEND_NOT_INITIALIZATION;
		  }
	      else
		     return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
		  return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_Reset(Device_Context *DC, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_Reset)\n");
//    DriverDebugPrint("(DRV_MyFrontend_Reset) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_Reset) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			 {
                // Not Support
			 }
			 else
				return Error_FRONTEND_NOT_OPEN;
		  }
	      else
	         return Error_FRONTEND_NOT_INITIALIZATION;
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_SetDemodulationMode(Device_Context *DC, DTV_DEMODULATION_MODE mode, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_SetDemodulationMode)\n");
//    DriverDebugPrint("(DRV_MyFrontend_SetDemodulationMode) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_SetDemodulationMode) mode : 0x%x\n",mode);
    DriverDebugPrint("(DRV_MyFrontend_SetDemodulationMode) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			 {
                DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx] = mode;
			 }
			 else
				return Error_FRONTEND_NOT_OPEN;
		  }
	      else
	         return Error_FRONTEND_NOT_INITIALIZATION;
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_SetSignalModulationType(Device_Context *DC, DTV_MODULATION_CONSTELLATION Type, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_SetSignalModulationType)\n");
//    DriverDebugPrint("(DRV_MyFrontend_SetSignalModulationType) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_SetSignalModulationType) Type : 0x%x\n",Type);
    DriverDebugPrint("(DRV_MyFrontend_SetSignalModulationType) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			 {
                DC->JackyFrontend.JackyGlobalFrontendRequestTuningSignalModulationType[tsSrcIdx] = Type;
			 }
			 else
				return Error_FRONTEND_NOT_OPEN;
		  }
	      else
	         return Error_FRONTEND_NOT_INITIALIZATION;
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_acquireChannel(Device_Context *DC, Word bandwidth, Dword frequency, Byte tsSrcIdx)
{
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_acquireChannel)\n");
//    DriverDebugPrint("(DRV_MyFrontend_acquireChannel) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_acquireChannel) tsSrcIdx : %d\n",tsSrcIdx);
    DriverDebugPrint("(DRV_MyFrontend_acquireChannel) frequency : %d\n",frequency);
    DriverDebugPrint("(DRV_MyFrontend_acquireChannel) bandwidth : %d\n",bandwidth);
		
	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			 {
			    if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
			    {
				   if(DC->JackyFrontend.JackyGlobalFrontendLastTuningFrequencyKhz[tsSrcIdx] != frequency ||
					  DC->JackyFrontend.JackyGlobalFrontendLastTuningBandwidthKhz[tsSrcIdx] != bandwidth ||
                      DC->JackyFrontend.JackyGlobalFrontendLastTuningSignalModulationType[tsSrcIdx] != DC->JackyFrontend.JackyGlobalFrontendRequestTuningSignalModulationType[tsSrcIdx] ||
					  DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulationMode[tsSrcIdx] != DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx] ||
					  DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulatorLockStatus[tsSrcIdx] == False)
				   {			
                      DC->JackyFrontend.JackyGlobalFrontendRequestTuningFrequencyKhz[tsSrcIdx] = frequency;
	                  DC->JackyFrontend.JackyGlobalFrontendRequestTuningBandwidthKhz[tsSrcIdx] = bandwidth;

// DriverDebugPrint("(DRV_MyFrontend_acquireChannel) TunerDeviceID : %d\n",DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);

				      if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
						 if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
							Result = CXD2856_ResetInternal(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
						 }
						 else
						 {
							if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
							{
                               if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
								  (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							   {
                                  Result = CXD2856_ResetInternal(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
							   }
							   else
							   {
							      Result = TRUE;
							   }
							}
						 }
					  }
					  else
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
						 Result = TC90522_ResetInternal(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
                      else
					     Result = FALSE;

	                  if(Result)
					  {
				         if(DC->JackyFrontend.JackyGlobalFrontendTunerInitializationFlag[tsSrcIdx] == True)
						 {
                            if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					           DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					           DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					           DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					           DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
							   DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
							{
                               if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx] == ISDBT_MODE)
								  Result = R850_SetFrequency(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
							   else
							   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx] == ISDBS_MODE)
								  Result = RT710_SetFrequency(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
							   else
								  Result = FALSE;
							   if(Result)
							   {
                                  Result = TC90522_ChannelSearch(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
							   }
							}
							else
				            if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				               DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			           DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			           DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			           DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			           DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
							{
						       if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							      DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
							   {
								  Result = CXD2856_SetFrequency(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
                                  if(Result)
								  {
								     Result = CXD2856_ChannelSearch(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
								  }
							   }
							   else
							   {
							      if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
								  {
									 Demodulator_acquireChannel((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,DC->JackyFrontend.JackyGlobalFrontendRequestTuningBandwidthKhz[tsSrcIdx],DC->JackyFrontend.JackyGlobalFrontendRequestTuningFrequencyKhz[tsSrcIdx]);

									 Result = TRUE;
								  }
						          else
								  {
                                     if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							            (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
									 {
								        Result = CXD2856_SetFrequency(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
                                        if(Result)
										{
						                   Result = CXD2856_ChannelSearch(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
										}
									 }
									 else
									 {
                                        Demodulator_acquireChannel((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,DC->JackyFrontend.JackyGlobalFrontendRequestTuningBandwidthKhz[tsSrcIdx],DC->JackyFrontend.JackyGlobalFrontendRequestTuningFrequencyKhz[tsSrcIdx]);

										Result = TRUE;
									 }
								  }
							   }
							}
                            if(Result == FALSE)
							   return Error_FRONTEND_OPERATION_FAILED;
							else
							{
						       DC->JackyFrontend.JackyGlobalFrontendLastTuningFrequencyKhz[tsSrcIdx] = DC->JackyFrontend.JackyGlobalFrontendRequestTuningFrequencyKhz[tsSrcIdx];
							   DC->JackyFrontend.JackyGlobalFrontendLastTuningBandwidthKhz[tsSrcIdx] = DC->JackyFrontend.JackyGlobalFrontendRequestTuningBandwidthKhz[tsSrcIdx];
						       DC->JackyFrontend.JackyGlobalFrontendLastTuningSignalModulationType[tsSrcIdx] = DC->JackyFrontend.JackyGlobalFrontendRequestTuningSignalModulationType[tsSrcIdx];
							   DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulationMode[tsSrcIdx] = DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx];
							}
						 }
				         else
					        return Error_FRONTEND_TUNER_NOT_INITIALIZATION;
					  }
				      else
					     return Error_FRONTEND_OPERATION_FAILED;
				   }
			    }
				else
				   return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
			 }
	         else
		        return Error_FRONTEND_NOT_OPEN;
		  }
	      else
	         return Error_FRONTEND_NOT_INITIALIZATION;
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;


   DriverDebugPrint("(DRV_MyFrontend_acquireChannel) Succeed !\n");
			
	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_getStatistic(Device_Context *DC, Statistic *statistic, Byte tsSrcIdx)
{
	BOOL Result;
	Statistic it9173statistic;
	BOOL LockStatus;
	char RssiDbm;
	unsigned char Strength;
	unsigned char Quality;

    DriverDebugPrint("(DRV_MyFrontend_getStatistic)\n");
//    DriverDebugPrint("(DRV_MyFrontend_getStatistic) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_getStatistic) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(statistic)
	   {
	      if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	      {
	         if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		     {
                if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			    {
			       if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
				   {
				      if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
					     if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
						    DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
							Result = CXD2856_CheckLock(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&LockStatus);
						 }
						 else
						 {
						    if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
							{
							   Demodulator_getStatistic((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,&it9173statistic);
				               if(it9173statistic.signalPresented == True)
							   {
				                  if(it9173statistic.signalLocked == True)
								  {
                                     LockStatus = TRUE;
								  }
				                  else
								  {
					                 LockStatus = FALSE;
								  }
							   }
				               else
							   {
				                  LockStatus = FALSE;
							   }

							   Result = TRUE;
							}
						    else
							{
                               if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							      (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							   {
						          Result = CXD2856_CheckLock(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&LockStatus);                       
							   }
							   else
							   {
							      Demodulator_getStatistic((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,&it9173statistic);
				                  if(it9173statistic.signalPresented == True)
							      {
				                     if(it9173statistic.signalLocked == True)
								     {
                                        LockStatus = TRUE;
								     }
				                     else
								     {
					                    LockStatus = FALSE;
								     }
							      }
				                  else
							      {
				                     LockStatus = FALSE;
							      }

							      Result = TRUE;
							   }
							}
						 }
					  }
					  else
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
                         Result = TC90522_CheckLock(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&LockStatus);
                      else
						 Result = FALSE;
                      if(Result)
					  {
						 if(LockStatus)
						 {

// DriverDebugPrint("(DRV_MyFrontend_getStatistic) RfLevelDbm : %d\n",RfLevelDbm);
// DriverDebugPrint("(DRV_MyFrontend_getStatistic) fgRfMaxGain : %d\n",fgRfMaxGain);

                            if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					           DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					           DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					           DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					           DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
							   DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
							{
                               if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx] == ISDBT_MODE)
								  RssiDbm = R850_GetRssi(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
                               else
							   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[tsSrcIdx] == ISDBS_MODE)
								  RssiDbm = RT710_GetRssi(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
							   else
								  RssiDbm = -100;

                               if(RssiDbm > 0)
							      Strength = 100;
							   else
							   if(RssiDbm < -100)
							      Strength = 0;
							   else
                                  Strength = (unsigned char)(100 + RssiDbm);

							   Quality = TC90522DemodulatorGetQuality(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);
							}
							else
				            if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				               DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			           DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			           DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			           DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			           DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
							{
						       if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							      DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
							   {
                                  CXD2856_GetStrength(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&Strength);
                                  CXD2856_GetQuality(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&Quality);
							   }
							   else
							   {
                                  if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
								  {
                                     Demodulator_getSignalStrengthIndication((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)&Strength);
									 Demodulator_getSignalQualityIndication((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)&Quality);
								  }
								  else
								  {
                                     if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							            (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
									 {
                                        CXD2856_GetStrength(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&Strength);
                                        CXD2856_GetQuality(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&Quality);
									 }
									 else
									 {
                                        Demodulator_getSignalStrengthIndication((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)&Strength);
									    Demodulator_getSignalQualityIndication((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)&Quality);
									 }
								  }
							   }
							}

                            statistic->signalPresented = True;
							statistic->signalLocked = True;
							statistic->signalStrength = (Byte)Strength;
							statistic->signalQuality = (Byte)Quality;
						 }
						 else
						 {
                            statistic->signalPresented = False;
							statistic->signalLocked = False;
							statistic->signalStrength = 0;
							statistic->signalQuality = 0;
						 }

// DriverDebugPrint("(DRV_MyFrontend_getStatistic) statistic->signalPresented : 0x%x\n",statistic->signalPresented);
// DriverDebugPrint("(DRV_MyFrontend_getStatistic) statistic->signalLocked : 0x%x\n",statistic->signalLocked);
// DriverDebugPrint("(DRV_MyFrontend_getStatistic) statistic->signalStrength : 0x%d\n",statistic->signalStrength);
// DriverDebugPrint("(DRV_MyFrontend_getStatistic) statistic->signalQuality : 0x%d\n",statistic->signalQuality);

					  }
					  else
						 return Error_FRONTEND_OPERATION_FAILED;
				   }
                   else
				      return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
			    }
	            else
		           return Error_FRONTEND_NOT_OPEN;
		     }
	         else
	            return Error_FRONTEND_NOT_INITIALIZATION;
	      }
	      else
		     return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
		  return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_getChannelStatistic(Device_Context *DC, ChannelStatistic* channelStatistic, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_getChannelStatistic)\n");
//    DriverDebugPrint("(DRV_MyFrontend_getChannelStatistic) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_getChannelStatistic) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(channelStatistic)
	   {
	      if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
		  {
	         if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
			 {
                if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
				{
				   channelStatistic->abortCount = 0;
				   channelStatistic->postVitBitCount = 0;
				   channelStatistic->postVitErrorCount = 0;
				   channelStatistic->softBitCount = 0;
				   channelStatistic->softErrorCount = 0;
				   channelStatistic->preVitBitCount = 0;
				   channelStatistic->preVitErrorCount = 0;
				}
	            else
		           return Error_FRONTEND_NOT_OPEN;
			 }
	         else
	            return Error_FRONTEND_NOT_INITIALIZATION;
		  }
	      else
		     return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
		  return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_isLocked(Device_Context *DC, Bool* locked, Byte tsSrcIdx)
{
	BOOL LockStatus;
	Statistic it9173statistic;
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_isLocked)\n");
    DriverDebugPrint("(DRV_MyFrontend_isLocked) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(locked)
	   {
	      if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	      {
	         if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		     {
                if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			    {
			       if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
				   {
				      if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
						 if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
                            Result = CXD2856_CheckLock(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&LockStatus);
						 }
						 else
						 {
                            if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
							{
							   Demodulator_getStatistic((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,&it9173statistic);
				               if(it9173statistic.signalPresented == True)
							   {
				                  if(it9173statistic.signalLocked == True)
								  {
                                     LockStatus = TRUE;
								  }
				                  else
								  {
					                 LockStatus = FALSE;
								  }
							   }
				               else
							   {
				                  LockStatus = FALSE;
							   }

							   Result = TRUE;
							}
						    else
							{
                               if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							       (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							   {
                                  Result = CXD2856_CheckLock(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&LockStatus);
							   }
							   else
							   {
							      Demodulator_getStatistic((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,&it9173statistic);
				                  if(it9173statistic.signalPresented == True)
								  {
				                     if(it9173statistic.signalLocked == True)
									 {
                                        LockStatus = TRUE;
									 }
				                     else
									 {
					                    LockStatus = FALSE;
									 }
								  }
				                  else
								  {
				                     LockStatus = FALSE;
								  }

							      Result = TRUE;
							   }
							}
						 }
					  }
					  else
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
                         Result = TC90522_CheckLock(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&LockStatus);
					  else
						 Result = FALSE;
				      if(Result)
				      {
                         if(LockStatus)
						 {
                            *locked = True;

							DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulatorLockStatus[tsSrcIdx] == True;
						 }
						 else
						 {
							*locked = False;

							DC->JackyFrontend.JackyGlobalFrontendLastTuningDemodulatorLockStatus[tsSrcIdx] == False;
						 }
				      }
				      else
				         return Error_FRONTEND_OPERATION_FAILED;
				   }
                   else
				      return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
				}
	            else
		           return Error_FRONTEND_NOT_OPEN;
			 }
	         else
	            return Error_FRONTEND_NOT_INITIALIZATION;
		  }
	      else
		     return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
          return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}

Dword DRV_MyFrontend_GetCNR(Device_Context *DC, char* pCNR, Byte tsSrcIdx)
{
	unsigned char IT9173CNR;
	unsigned short CNRRaw;
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_GetCNR)\n");
//    DriverDebugPrint("(DRV_MyFrontend_GetCNR) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_GetCNR) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(pCNR)
	   {
	      if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	      {
	         if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		     {
                if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			    {
			       if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
				   {
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
					     Result = TC90522_GetCNR(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],pCNR);
					  else
				      if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
						 if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
							Result = CXD2856_GetCNRRaw(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&CNRRaw);
                            if(Result)
							{
                               *pCNR = (CNRRaw / 100);
							}
						 }
						 else
						 {
							if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
							{
							   Demodulator_getSNR((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)&IT9173CNR);
                               *pCNR = IT9173CNR;

							   Result = TRUE;
							}
						    else
							{
                               if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							      (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							   {
							      Result = CXD2856_GetCNRRaw(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&CNRRaw);
                                  if(Result)
							      {
                                     *pCNR = (CNRRaw / 100);
							      }
							   }
							   else
							   {
                                  Demodulator_getSNR((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)&IT9173CNR);
                                  *pCNR = IT9173CNR;

							      Result = TRUE;
							   }
							}
						 }
					  }
					  else
						 Result = FALSE;					  
				      if(Result == FALSE)
				         return Error_FRONTEND_OPERATION_FAILED;
				   }
                   else
				      return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
				}
	            else
		           return Error_FRONTEND_NOT_OPEN;
			 }
	         else
	            return Error_FRONTEND_NOT_INITIALIZATION;
		  }
	      else
		     return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
          return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_GetCNRaw(Device_Context *DC, int* pCNRaw, Byte tsSrcIdx)
{
	unsigned short CNRRaw;
	unsigned char IT9173CNR;
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_GetCNRaw)\n");
//    DriverDebugPrint("(DRV_MyFrontend_GetCNRaw) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_GetCNRaw) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(pCNRaw)
	   {
	      if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	      {
	         if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		     {
                if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			    {
			       if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
				   {
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
					     Result = TC90522_GetCNRaw(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],pCNRaw);
					  else
				      if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
						 if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
							Result = CXD2856_GetCNRRaw(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&CNRRaw);
                            if(Result)
							{
                               *pCNRaw = CNRRaw;
							}
						 }
						 else
						 {
							if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
							{
							   Demodulator_getSNR((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)&IT9173CNR);
                               *pCNRaw = IT9173CNR;

							   Result = TRUE;
							}
						    else
							{
                               if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							      (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							   {
							      Result = CXD2856_GetCNRRaw(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],&CNRRaw);
                                  if(Result)
							      {
                                     *pCNRaw = CNRRaw;
							      }
							   }
							   else
							   {
                                  Demodulator_getSNR((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)&IT9173CNR);
								  *pCNRaw = IT9173CNR;

							      Result = TRUE;
							   }
							}
						 }
					  }
					  else
						 Result = FALSE;					 
				      if(Result == FALSE)
				         return Error_FRONTEND_OPERATION_FAILED;
				   }
                   else
				      return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
				}
	            else
		           return Error_FRONTEND_NOT_OPEN;
			 }
	         else
	            return Error_FRONTEND_NOT_INITIALIZATION;
		  }
	      else
		     return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
          return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_getSNR(Device_Context *DC, Constellation* pConstellation, char* pSnr, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_getSNR)\n");
//    DriverDebugPrint("(DRV_MyFrontend_getSNR) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_getSNR) tsSrcIdx : %d\n",tsSrcIdx);

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_getSignalStrengthDbm(Device_Context *DC, long* pStrengthDbm, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_getSignalStrengthDbm)\n");
//    DriverDebugPrint("(DRV_MyFrontend_getSignalStrengthDbm) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_getSignalStrengthDbm) tsSrcIdx : %d\n",tsSrcIdx);

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_getBER(Device_Context *DC, long* pBer, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_getBER)\n");
//    DriverDebugPrint("(DRV_MyFrontend_getBER) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_getBER) tsSrcIdx : %d\n",tsSrcIdx);

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_getChannelStrength(Device_Context *DC, unsigned char* pStrength, Byte tsSrcIdx)
{
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_getChannelStrength)\n");
//    DriverDebugPrint("(DRV_MyFrontend_getChannelStrength) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_getChannelStrength) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(pStrength)
	   {
	      if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	      {
	         if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		     {
                if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			    {
			       if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
				   {
				      if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
						 if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
							Result = CXD2856_GetStrength(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],pStrength);
						 }
						 else
						 {
							if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
							{
							   Demodulator_getSignalStrengthIndication((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)pStrength);

							   Result = TRUE;
							}
						    else
							{
                               if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							      (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							   {
							      Result = CXD2856_GetStrength(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],pStrength);
							   }
							   else
							   {
								  Demodulator_getSignalStrengthIndication((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)pStrength);

							      Result = TRUE;
							   }
							}
						 }
                         if(Result == FALSE)
							return Error_FRONTEND_OPERATION_FAILED;
					  }
					  else
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
					  {
					     Result = TC90522_GetStrength(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],pStrength);
                         if(Result == FALSE)
							return Error_FRONTEND_OPERATION_FAILED;
					  }
					  else
                         return Error_FRONTEND_FEATURE_UNSUPPORTED;
				   }
                   else
				      return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
				}
	            else
		           return Error_FRONTEND_NOT_OPEN;
			 }
	         else
	            return Error_FRONTEND_NOT_INITIALIZATION;
		  }
	      else
		     return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
          return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_getChannelQuality(Device_Context *DC, unsigned char* pQuality, Byte tsSrcIdx)
{
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_getChannelQuality)\n");
//    DriverDebugPrint("(DRV_MyFrontend_getChannelQuality) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_getChannelQuality) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(pQuality)
	   {
	      if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	      {
	         if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		     {
                if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			    {
			       if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
				   {
				      if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				         DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT ||
        			     DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					  {
						 if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UT &&
							DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
						 {
							Result = CXD2856_GetQuality(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],pQuality);
						 }
						 else
						 {
							if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UT)
							{
							   Demodulator_getSignalQualityIndication((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)pQuality);

							   Result = TRUE;
							}
						    else
							{
                               if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							      (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
							   {
							      Result = CXD2856_GetQuality(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],pQuality);
							   }
							   else
							   {
								  Demodulator_getSignalQualityIndication((Demodulator*) DC->it9300.tsSource[0][tsSrcIdx].htsDev,(Byte*)pQuality);

							      Result = TRUE;
							   }
							}
						 }
                         if(Result == FALSE)
							return Error_FRONTEND_OPERATION_FAILED;
					  }
					  else
					  if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					     DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
					  {
					     Result = TC90522_GetQuality(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],pQuality);
                         if(Result == FALSE)
							return Error_FRONTEND_OPERATION_FAILED;
					  }
					  else
                         return Error_FRONTEND_FEATURE_UNSUPPORTED;
				   }
                   else
				      return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
				}
	            else
		           return Error_FRONTEND_NOT_OPEN;
			 }
	         else
	            return Error_FRONTEND_NOT_INITIALIZATION;
		  }
	      else
		     return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
          return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_getChannelErrorRate(Device_Context *DC, unsigned char* pErrorRate, Byte tsSrcIdx)
{
	return Error_FRONTEND_FEATURE_UNSUPPORTED;
}
Dword DRV_MyFrontend_SetTSID(Device_Context *DC, unsigned short TSID, Byte tsSrcIdx)
{
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_SetTSID)\n");
//    DriverDebugPrint("(DRV_MyFrontend_SetTSID) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_SetTSID) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			 {
			    if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
				{

// DriverDebugPrint("(DRV_MyFrontend_SetTSID) TunerDeviceID : %d\n",DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx]);

				   if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
                      Result = TC90522_SetTSID(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],TSID);
				   else
				   if(DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
				      DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			  DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
				   {
					  if(DC->JackyFrontend.ReceiverType != EEPROM_PXMLT8UTS)
					  {
                         Result = CXD2856_SetTSID(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],TSID);
					  }
					  else
					  {
                         if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0 || 
							(DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 1)
						 {
                            Result = CXD2856_SetTSID(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],TSID);
						 }
						 else
                            Result = FALSE;
					  }
				   }
				   else
					  Result = FALSE;
                   if(Result == FALSE)
					  return Error_FRONTEND_OPERATION_FAILED;
				}
                else
				   return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
			 }
	         else
		        return Error_FRONTEND_NOT_OPEN;
		  }
	      else
	         return Error_FRONTEND_NOT_INITIALIZATION;
	   } 
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_SetOneSegMode(Device_Context *DC, Bool OneSegModeEnableFlag, Byte tsSrcIdx)
{
	BOOL Result;

    DriverDebugPrint("(DRV_MyFrontend_SetOneSegMode)\n");
//    DriverDebugPrint("(DRV_MyFrontend_SetOneSegMode) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_SetOneSegMode) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendInitializationFlag[tsSrcIdx] == True)
		  {
             if(DC->JackyFrontend.JackyGlobalFrontendOpenFlag[tsSrcIdx] == True)
			 {
			    if(DC->JackyFrontend.JackyGlobalFrontendDemodInitializationFlag[tsSrcIdx] == True)
				{
				   if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
					  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5)
				   {
				      if(OneSegModeEnableFlag == True)
                         Result = TC90522_SetPartialReceiption(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],TRUE);
				      else
                         Result = TC90522_SetPartialReceiption(DC,DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx],FALSE);
				   }
				   else
					  Result = FALSE;
                   if(Result == FALSE)
					  return Error_FRONTEND_OPERATION_FAILED;
				}
                else
				   return Error_FRONTEND_DEMON_NOT_INITIALIZATION;
			 }
	         else
		        return Error_FRONTEND_NOT_OPEN;
		  }
	      else
	         return Error_FRONTEND_NOT_INITIALIZATION;
	   } 
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_SetLNBPower(Device_Context *DC, Bool PowerFlag, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_SetLNBPower)\n");
//    DriverDebugPrint("(DRV_MyFrontend_SetLNBPower) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_SetLNBPower) tsSrcIdx : %d\n",tsSrcIdx);
    DriverDebugPrint("(DRV_MyFrontend_SetLNBPower) PowerFlag : 0x%x\n",PowerFlag);

	if(DC)
	{
	   if(tsSrcIdx < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
          if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] & 0x03) == 1 ||
			 (DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] & 0x03) == 3)
		  {
              if(PowerFlag == True)
			  {
                 if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] & 0x03) == 1)
                    DC->JackyFrontend.JackyGlobalFrontendS0LNBPowerEnableFlag = True;
	             else
                 if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] & 0x03) == 3)
                    DC->JackyFrontend.JackyGlobalFrontendS1LNBPowerEnableFlag = True;

                 if(DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag == False)
				 {
                    if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					   DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					   DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					   DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					   DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
					   DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5 ||
					   DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
        			   DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			   DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			   DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			   DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					{
                       IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh11_o, 0x1);     // on : 1 , off : 0
					}

                    DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag = True;
				 }
			  }
			  else
			  {
                 if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] & 0x03) == 1)
                    DC->JackyFrontend.JackyGlobalFrontendS0LNBPowerEnableFlag = False;
	             else
                 if((DC->JackyFrontend.JackyGlobalFrontendLogicalTunerDeviceID[tsSrcIdx] & 0x03) == 3)
                    DC->JackyFrontend.JackyGlobalFrontendS1LNBPowerEnableFlag = False;

			     if(DC->JackyFrontend.JackyGlobalFrontendS0LNBPowerEnableFlag == False && DC->JackyFrontend.JackyGlobalFrontendS1LNBPowerEnableFlag == False)
				 {
                    if(DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag == True)
					{
                       if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
					      DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
					      DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
					      DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
					      DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
						  DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5 ||
					      DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
        			      DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        			      DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        			      DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        			      DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
					   {
                          IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh11_o, 0x0);     // on : 1 , off : 0
					   }

                       DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag = False;
					}
				 }
			  }
		  }
		  else
			 return Error_FRONTEND_TUNER_INDEX_INVALID;
	   }
	   else
		  return Error_FRONTEND_TUNER_INDEX_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_GetLNBPower(Device_Context *DC, Bool* pPowerFlag, Byte tsSrcIdx)
{
    DriverDebugPrint("(DRV_MyFrontend_GetLNBPower)\n");
//    DriverDebugPrint("(DRV_MyFrontend_GetLNBPower) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_GetLNBPower) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
	   if(pPowerFlag)
	   {
          *pPowerFlag = DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag;
	   }
	   else
	      return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_GetLNBPowerShortCircuitDetect(Device_Context *DC, Bool* pShortCircuitFlag, Byte tsSrcIdx)
{
	Byte temp;

    DriverDebugPrint("(DRV_MyFrontend_GetLNBPowerShortCircuitDetect)\n");
//    DriverDebugPrint("(DRV_MyFrontend_GetLNBPowerShortCircuitDetect) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DRV_MyFrontend_GetLNBPowerShortCircuitDetect) tsSrcIdx : %d\n",tsSrcIdx);

	if(DC)
	{
       if(pShortCircuitFlag)
	   {
          if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
        	 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        	 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        	 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        	 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
		  {
	         IT9300_writeRegister ((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh10_on, 1);
	         IT9300_writeRegister ((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh10_en, 0);
	         IT9300_readRegister ((Endeavour*) &DC->it9300, 0, r_br_reg_top_gpioh10_i, &temp);
		  }

	      if(temp == 1)
		     *pShortCircuitFlag = False;
	      else
		     *pShortCircuitFlag = True;
	   }
	   else
		  return Error_FRONTEND_PARAMETER_INVALID;
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
Dword DRV_MyFrontend_EnforceLNBPowerOff(Device_Context *DC)
{
    DriverDebugPrint("(DRV_MyFrontend_EnforceLNBPowerOff)\n");
//    DriverDebugPrint("(DRV_MyFrontend_EnforceLNBPowerOff) point of DC : 0x%x\n",(unsigned long)DC);

	if(DC)
	{
       if(DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag == True)
	   {
          if(DC->JackyFrontend.ReceiverType == EEPROM_PXW3U4 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE4 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXW3PE5 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3U4 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE4 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXQ3PE5 ||
			 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5PE ||
        	 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT5U ||
        	 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE3 ||
        	 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8PE5 ||
        	 DC->JackyFrontend.ReceiverType == EEPROM_PXMLT8UTS)
		  {
             IT9300_writeRegister((Endeavour*) &DC->it9300, 0, p_br_reg_top_gpioh11_o, 0x0);     // on : 1 , off : 0
		  }

          DC->JackyFrontend.JackyGlobalFrontendS0LNBPowerEnableFlag = False;
          DC->JackyFrontend.JackyGlobalFrontendS1LNBPowerEnableFlag = False;

          DC->JackyFrontend.JackyGlobalFrontendSystemLNBPowerEnableFlag = False;
	   }
	}
	else
       return Error_FRONTEND_PARAMETER_INVALID;

	return Error_NO_ERROR;
}
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
Dword DL_MyFrontend_getStatistic(Device_Context *DC, Statistic *statistic, int br_idx, int ts_idx)
{
	Dword error;

    DriverDebugPrint("(DL_MyFrontend_getStatistic)\n");
//    DriverDebugPrint("(DL_MyFrontend_getStatistic) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DL_MyFrontend_getStatistic) br_idx : %d\n",br_idx);
    DriverDebugPrint("(DL_MyFrontend_getStatistic) ts_idx : %d\n",ts_idx);

	error = DRV_MyFrontend_getStatistic(DC,statistic,ts_idx); 
	
	return error;
}
Dword DL_MyFrontend_getChannelStatistic(Device_Context *DC, ChannelStatistic* channelStatistic, int br_idx, int ts_idx)
{
	Dword error;

    DriverDebugPrint("(DL_MyFrontend_getChannelStatistic)\n");
//    DriverDebugPrint("(DL_MyFrontend_getChannelStatistic) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DL_MyFrontend_getChannelStatistic) br_idx : %d\n",br_idx);
    DriverDebugPrint("(DL_MyFrontend_getChannelStatistic) ts_idx : %d\n",ts_idx);

	error = DRV_MyFrontend_getChannelStatistic(DC,channelStatistic,ts_idx); 
	
	return error;
}
Dword DL_MyFrontend_isLocked(Device_Context *DC, Bool *locked, int br_idx, int ts_idx)
{
	Dword error;

//    DriverDebugPrint("(DL_MyFrontend_isLocked)\n");
//    DriverDebugPrint("(DL_MyFrontend_isLocked) br_idx : %d\n",br_idx);
//    DriverDebugPrint("(DL_MyFrontend_isLocked) ts_idx : %d\n",ts_idx);

	error = DRV_MyFrontend_isLocked(DC,locked,ts_idx); 
	
	return error;
}
Dword DL_MyFrontend_getBER(Device_Context *DC, long* pBer, int br_idx, int ts_idx)
{
	Dword error;

    DriverDebugPrint("(DL_MyFrontend_getBER)\n");
//    DriverDebugPrint("(DL_MyFrontend_getBER) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DL_MyFrontend_getBER) br_idx : %d\n",br_idx);
    DriverDebugPrint("(DL_MyFrontend_getBER) ts_idx : %d\n",ts_idx);

	error = DRV_MyFrontend_getBER(DC,pBer,ts_idx); 
	
	return error;
}
Dword DL_MyFrontend_getChannelStrength(Device_Context *DC, unsigned char* pStrength, int br_idx, int ts_idx)
{
	Dword error;

    DriverDebugPrint("(DL_MyFrontend_getChannelStrength)\n");
//    DriverDebugPrint("(DL_MyFrontend_getChannelStrength) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DL_MyFrontend_getChannelStrength) br_idx : %d\n",br_idx);
    DriverDebugPrint("(DL_MyFrontend_getChannelStrength) ts_idx : %d\n",ts_idx);

	error = DRV_MyFrontend_getChannelStrength(DC,pStrength,ts_idx); 
	
	return error;
}
Dword DL_MyFrontend_getChannelQuality(Device_Context *DC, unsigned char* pQuality, int br_idx, int ts_idx)
{
	Dword error;

    DriverDebugPrint("(DL_MyFrontend_getChannelQuality)\n");
//    DriverDebugPrint("(DL_MyFrontend_getChannelQuality) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DL_MyFrontend_getChannelQuality) br_idx : %d\n",br_idx);
    DriverDebugPrint("(DL_MyFrontend_getChannelQuality) ts_idx : %d\n",ts_idx);

	error = DRV_MyFrontend_getChannelQuality(DC,pQuality,ts_idx); 
	
	return error;
}
Dword DL_MyFrontend_getChannelErrorRate(Device_Context *DC, unsigned char* pErrorRate, int br_idx, int ts_idx)
{
	Dword error;

    DriverDebugPrint("(DL_MyFrontend_getChannelErrorRate)\n");
//    DriverDebugPrint("(DL_MyFrontend_getChannelErrorRate) point of DC : 0x%x\n",(unsigned long)DC);
    DriverDebugPrint("(DL_MyFrontend_getChannelErrorRate) br_idx : %d\n",br_idx);
    DriverDebugPrint("(DL_MyFrontend_getChannelErrorRate) ts_idx : %d\n",ts_idx);

	error = DRV_MyFrontend_getChannelErrorRate(DC,pErrorRate,ts_idx); 
	
	return error;
}
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************
BOOL CanTheCaptureDeviceBePowerOff(Device_Context *DC)
{
    unsigned long Loop;

    for(Loop=0;Loop<JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER;Loop++)
	{
	   if(DC->JackyFrontend.JackyGlobalFrontendLogicalPowerEnableFlag[Loop] == True)
          return FALSE;
	}

	return TRUE;
}
//*******************************************************************************************
//*******************************************************************************************
//*******************************************************************************************

