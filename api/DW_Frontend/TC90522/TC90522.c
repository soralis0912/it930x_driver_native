#include <linux/kernel.h> 
#include <linux/slab.h>
#include <linux/delay.h>

#include "it930x-core.h"
#include "TC90522.h"
#include "MyI2CPortingLayer.h"
#include "MyDebug.h"

// ************************************************************************************
// ************************************************************************************
// ************************************************************************************
BOOL TC90522_Initialization(Device_Context *DC,unsigned char TunerDeviceID)
{
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_Initialization)*******************************************************************************\n");
    DriverDebugPrint("(TC90522_Initialization)*********************************** Stage 1 ***********************************\n");
    DriverDebugPrint("(TC90522_Initialization)*******************************************************************************\n");
    DriverDebugPrint("(TC90522_Initialization)\n");
    DriverDebugPrint("(TC90522_Initialization) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
	   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
	   {
	      if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xB0,0xA0))
	      {
	         if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xB2,0x3D))
		     {
			    if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xB3,0x25))
			    {
	               if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xB4,0x8B))
				   {
	                  if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xB5,0x4B))
				      {
	                     if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xB6,0x3F))
					     {
	                        if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xB7,0xFF))
						    {
	                           if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xB8,0xC0))
							   {
	                              if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x03,0x00))
							      {
	                                 if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x1D,0xA8))                 // disable SRCK/SRDT/SBYTE/PBVAL pin (serial TS sync/data/clock/data valid)
								     {
									    if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x1F,0x00))
									    {
										   Result = TRUE;
									    }
								     }
							      }
							   }
						    }
					     }
				      }
				   }
			    }
		     }
	      }
	   }
       else
	   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
	   {  
	      if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x13,0x00))
	      {
	         if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x15,0x00))
		     {
	            if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x17,0x00))
			    {
	               if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1C,0x80))                                     // disable JPBVAL pin (serial TS data valid)                                                                    
				   {
	                  if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1D,0x00))
				      {
				 	     if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1F,0x22))                               // disable JSBYTE/JSRCK/JSRDT pin (serial TS sync/data/clock)	
					     {
						    Result = TRUE;
					     }
				      }
				   }
			    }
		     }
	      }
	   }
	}

	if(Result)
	{
       if(DC->JackyFrontend.JackyGlobalFrontendSystemTsOutputConfigFlag == FALSE)
	   {
          DriverDebugPrint("(TC90522_Initialization)*******************************************************************************\n");
          DriverDebugPrint("(TC90522_Initialization)*********************************** Stage 2 ***********************************\n");
          DriverDebugPrint("(TC90522_Initialization)*******************************************************************************\n");

          unsigned char A;
		  A = TunerDeviceID / 4;

		  // Terrestrial tuner 0
		  TC90522DemodulatorOFDM_I2C_Write(DC,((A * 4) + 0),0x0E,0x77);
          TC90522DemodulatorOFDM_I2C_Write(DC,((A * 4) + 0),0x0F,0x13);
		  TC90522DemodulatorOFDM_I2C_Write(DC,((A * 4) + 0),0x75,0x00);   // 0x02
/*
		  // Terrestrial tuner 1
		  TC90522DemodulatorOFDM_I2C_Write(DC,((A * 4) + 2),0x0E,0x77);
          TC90522DemodulatorOFDM_I2C_Write(DC,((A * 4) + 2),0x0F,0x13);
*/
		  TC90522DemodulatorOFDM_I2C_Write(DC,((A * 4) + 2),0x75,0x00);   // 0x02
		  
		  // Satellite tuner 0
		  TC90522DemodulatorPSK_I2C_Write(DC,((A * 4) + 1),0x07,0x31);		  
		  TC90522DemodulatorPSK_I2C_Write(DC,((A * 4) + 1),0x08,0x77);
		  TC90522DemodulatorPSK_I2C_Write(DC,((A * 4) + 1),0x04,0x02);

/*
		  // Satellite tuner 1
		  TC90522DemodulatorPSK_I2C_Write(DC,((A * 4) + 3),0x07,0x31);
		  TC90522DemodulatorPSK_I2C_Write(DC,((A * 4) + 3),0x08,0x77);
*/
		  TC90522DemodulatorPSK_I2C_Write(DC,((A * 4) + 3),0x04,0x02);

		  DC->JackyFrontend.JackyGlobalFrontendSystemTsOutputConfigFlag = TRUE;
	   }
	}

    DriverDebugPrint("(TC90522_Initialization) Result : 0x%x\n",Result);

	return Result;
}
void TC90522_Uninitialization(Device_Context *DC,unsigned char TunerDeviceID)
{

}
BOOL TC90522_ResetInternal(Device_Context *DC,unsigned char TunerDeviceID)
{
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_ResetInternal)\n");
    DriverDebugPrint("(TC90522_ResetInternal) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
	   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
	   {
		  if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x01,0x50))             // OFDM demodulation and windows reset
		  {
			 if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x47,0x30))
			 {
				if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x25,0x00))
				{
				   if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x20,0x00))
				   {
					  if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x23,0x4D))
					  {
                         Result = TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x76,0x0C);      // To disable NULL package receiving.
					  }
				   }
				} 
			 }
		  }
	   }
       else 
	   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
	   {
//		  if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x01,0x90))              // PSK system reset
		  {
			 if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x0A,0x00))
			 {
				if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x10,0xB0))
				{
				   if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x11,0x02))
				   {
					  if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x03,0x01))		// PSK demodulation reset
					  {
					     if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x8E,0x06))      // To disable NULL package receiving.   // 0x02 // 20170907
						 {
						    Result = TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0xA3,0xF7);   // 20170907
						 }
					  }
				   }
				}
			 }
		  }
	   }
	}

    DriverDebugPrint("(TC90522_ResetInternal) Result : 0x%x\n",Result);

	return Result;
}
BOOL TC90522_ChannelSearch(Device_Context *DC,unsigned char TunerDeviceID)
{
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_ChannelSearch)\n");
    DriverDebugPrint("(TC90522_ChannelSearch) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
	   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
	   {
          TC90522DemodulatorDelayMilliseconds(250);

	      if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x23,0x4C))
		  {
             if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x01,0x50)) 
			 {
		 	    if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x71,0x21))     // 0x01  // 20170907 // set layer C Null packet replacement
				{
                   if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x72,0x25))           // 20170907
				   {
				      Result = TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x75,0x08);  // 20170907
					  if(Result)        
						 TC90522DemodulatorDelayMilliseconds(350);                            // 20170908
				   }
				}
			 }
		  }
	   }
       else
	   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
	   {
          TC90522DemodulatorDelayMilliseconds(250);

	      if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x0A,0xFF) == TRUE)
		  {
	         if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x10,0xB2) == TRUE)
			 {
	            if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x11,0x00) == TRUE)
				{
	               Result = TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x03,0x01);
//				   if(Result == TRUE)
//					  TC90522DemodulatorDelayMilliseconds(500);
				}
			 }
		  }
	   }
	}

    DriverDebugPrint("(TC90522_ChannelSearch) Result : 0x%x\n",Result);

	return Result;
}
BOOL TC90522_SetTSID(Device_Context *DC,unsigned char TunerDeviceID,unsigned short TSID)
{
	unsigned char ByteData;
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_SetTSID)\n");
    DriverDebugPrint("(TC90522_SetTSID) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
	   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
	   {
          ByteData = (unsigned char)((TSID & 0xFF00) >> 8);
 	      if(TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x8F,ByteData) == TRUE)
		  {
             ByteData = (unsigned char)(TSID & 0x00FF);
	         Result = TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x90,ByteData);
		  }
	   }
	}

    DriverDebugPrint("(TC90522_SetTSID) Result : 0x%x\n",Result);

    return Result;
}
BOOL TC90522_SetPartialReceiption(Device_Context *DC,unsigned char TunerDeviceID,BOOL PartialReceiptionEnableFlag)
{
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_SetPartialReceiption)\n");
    DriverDebugPrint("(TC90522_SetPartialReceiption) TunerDeviceID : 0x%x\n",TunerDeviceID);
    DriverDebugPrint("(TC90522_SetPartialReceiption) PartialReceiptionEnableFlag : 0x%x\n",PartialReceiptionEnableFlag);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
	   if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
	   {
          if(PartialReceiptionEnableFlag)
		  {
			 if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x23,0x4C) == TRUE)
			 {
                if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x01,0x50) == TRUE) 
				{
				   TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x71,0x23);   // 0x03 // 20170907 // set layer B/C Null packet replacement
                   TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x72,0x6C);
                   TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x75,0x09);   // 0x01 // 20170907
				}
			 }
		  }
	      else
		  {
			 if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x23,0x4C) == TRUE)
			 {
                if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x01,0x50) == TRUE) 
				{
				   TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x71,0x21);   // 0x01 // 20170907 // set layer C Null packet replacement
                   TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x72,0x25);     
                   TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x75,0x08);   // 0x00 // 20170907
				}
			 }
		  }

		  Result = TRUE;
	   }
	}

    DriverDebugPrint("(TC90522_SetPartialReceiption) Result : 0x%x\n",Result);

	return Result;
}
BOOL TC90522_CheckLock(Device_Context *DC,unsigned char TunerDeviceID,BOOL* pLock)
{
	unsigned char Data;
	unsigned char BSCS110Quality;
	BOOL Result = FALSE;

    DriverDebugPrint("--------------------------------------------------------\n");
    DriverDebugPrint("(TC90522_CheckLock)\n");
	DriverDebugPrint("(TC90522_CheckLock) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pLock)
	{
	   *pLock = FALSE;

	   if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		  {
             if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x80,&Data))
			 {
//		        DriverDebugPrint("(TC90522_CheckLock) Data(0x80) : 0x%x\n",Data);

                if((Data & 0x08) == 0)
				{			 
				   if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xB0,&Data))
				   {
//					  DriverDebugPrint("(TC90522_CheckLock) Data(0xB0) : 0x%x\n",Data);

					  if((Data & 0x0F) >= 8)
					  {
					     TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x1D,0x00);   // enable SRCK/SRDT/SBYTE/PBVAL pin (serial TS sync/data/clock/data valid)		  
						 
					     *pLock = TRUE;
					  }
					  else
					  {
                         TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x1D,0xA8);   // disable SRCK/SRDT/SBYTE/PBVAL pin (serial TS sync/data/clock/data valid)	
					  }
				 
					  Result = TRUE;
				   }
				}
			    else
				{
				   TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0x1D,0xA8);   // disable SRCK/SRDT/SBYTE/PBVAL pin (serial TS sync/data/clock/data valid)		  

                   Result = TRUE;
				}
			 }
		  }
          else
		  if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
		  {
             if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xC3,&Data) == TRUE)
			 {
//                DriverDebugPrint("(TC90522_CheckLock) Data(0xC3) : 0x%x\n",Data);

                if((Data & 0x10) == 0)
				{
                   BSCS110Quality = TC90522DemodulatorGetQuality(DC,TunerDeviceID);

//				   DriverDebugPrint("(TC90522_CheckLock) BSCS110Quality : %d\n",BSCS110Quality);

                   if(BSCS110Quality >= 10)
				   {
					  TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1C,0x00);   // enable JPBVAL pin (serial TS data valid)		  
					  TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1F,0x00);   // enable JSBYTE/JSRCK/JSRDT pin (serial TS sync/data/clock)		   

		              *pLock = TRUE;
				   }
				   else
				   {
					  TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1C,0x80);   // disable JPBVAL pin (serial TS data valid)		  
					  TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1F,0x22);   // disable JSBYTE/JSRCK/JSRDT pin (serial TS sync/data/clock)			   
				   }
				}
				else
				{
				   TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1C,0x80);   // disable JPBVAL pin (serial TS data valid)		  
			  	   TC90522DemodulatorPSK_I2C_Write(DC,TunerDeviceID,0x1F,0x22);   // disable JSBYTE/JSRCK/JSRDT pin (serial TS sync/data/clock)			   
				}
	   
		        Result = TRUE;
			 }
		  }
		  else
			 Result = TRUE;
	   }

	   DriverDebugPrint("(TC90522_CheckLock) Lock : 0x%x\n",(*pLock));
	}

    DriverDebugPrint("(TC90522_CheckLock) Result : 0x%x\n",Result);

    return Result;
}
BOOL TC90522_GetStrength(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pStrength)
{
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_GetStrength)\n");
	DriverDebugPrint("(TC90522_GetStrength) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pStrength)
	{
       if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
          *pStrength = TC90522DemodulatorGetStrength(DC,TunerDeviceID);
 
	      DriverDebugPrint("(TC90522_GetStrength) Strength : %d\n",(*pStrength));

		  Result = TRUE;
	   }
	}

    return Result;
}
BOOL TC90522_GetQuality(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pQuality)
{
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_GetQuality)\n");
	DriverDebugPrint("(TC90522_GetQuality) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pQuality)
	{
       if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
          *pQuality = TC90522DemodulatorGetQuality(DC,TunerDeviceID);

	      DriverDebugPrint("(TC90522_GetQuality) Quality : %d\n",(*pQuality));

		  Result = TRUE;
	   }
	}

    return Result;
}
BOOL TC90522_GetCNR(Device_Context *DC,unsigned char TunerDeviceID,unsigned long* pCnrValue)
{
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_GetCNR)\n");
	DriverDebugPrint("(TC90522_GetCNR) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pCnrValue)
	{
       if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      *pCnrValue = TC90522DemodulatorGetCNR(DC,TunerDeviceID);

	      DriverDebugPrint("(TC90522_GetCNR) CNR Value : %d\n",(*pCnrValue));

		  Result = TRUE;
	   }
	}
	
    return Result;
}
BOOL TC90522_GetCNRaw(Device_Context *DC,unsigned char TunerDeviceID,int* pCnRawValue)
{
	BOOL Result = FALSE;

    DriverDebugPrint("(TC90522_GetCNRaw)\n");
	DriverDebugPrint("(TC90522_GetCNRaw) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(pCnRawValue)
	{
       if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	   {
	      *pCnRawValue = TC90522DemodulatorGetCNRaw(DC,TunerDeviceID);

	      DriverDebugPrint("(TC90522_GetCNRaw) CN Raw Value : %d\n",(*pCnRawValue));

		  Result = TRUE;
	   }
	}
	
    return Result;
}
// ************************************************************************************
// ************************************************************************************
// ************************************************************************************
unsigned char TC90522DemodulatorGetStrength(Device_Context *DC,unsigned char TunerDeviceID)
{
	unsigned long BerReg;
	unsigned long CycleReg;
	unsigned char sequen;
	unsigned char Data;
	unsigned int BER;
	char BER_return=0;
	
	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
		if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		{
			if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xBA,0x00) == TRUE)
			{
				if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xB0,&Data) == TRUE)
				 {
					sequen = Data & 0x0F;
					if(sequen >= 8)
					{
						if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x9D,&Data) == TRUE)
						{
							BerReg = Data;
							BerReg <<= 8; 
							if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x9E,&Data) == TRUE)
							{
								BerReg |= Data;
								BerReg <<= 8; 
								if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x9F,&Data) == TRUE)
								{
									BerReg |= Data;
//									DriverDebugPrint("(TC90522DemodulatorGetStrength) BerReg : %d\n",BerReg);
									
									if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xA6,&Data) == TRUE)
									{
										CycleReg = Data;
										CycleReg <<= 8; 	
										if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xA7,&Data) == TRUE)
										{					
											CycleReg |= Data;
											BER = (BerReg) * (61274 / CycleReg);
										}
									}									
								}
							}
						}
					}
				}
			}
		}
		else
		if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
		{
			if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xC3,&Data) == TRUE)
			{
				if((Data & 0x10) == 0)
				{ 
					if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xF0,&Data) == TRUE)
					{
						BerReg = Data;
						BerReg <<= 8; 
						if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xF1,&Data) == TRUE)
						{
							BerReg |= Data;
							BerReg <<= 8; 
							if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xF2,&Data) == TRUE)
							{
								BerReg |= Data;
//								DriverDebugPrint("(TC90522DemodulatorGetStrength) BerReg : %d\n",BerReg);

								BER = (BerReg) * 80;
							}
						}
					}
				}
			}
		}
	}

	
	if(BER==0)
		BER_return=100;
	else if((BER>0)&&(BER<=10))
		BER_return=90;
	else if((BER>10)&&(BER<=100))
		BER_return=80;
	else if((BER>100)&&(BER<=1000))
		BER_return=70;
	else if((BER>1000)&&(BER<=10000))
		BER_return=60;
	else if((BER>10000)&&(BER<=50000))
		BER_return=50;
	else if((BER>50000)&&(BER<=100000))
		BER_return=40;
	else if((BER>100000)&&(BER<=250000))
		BER_return=30;
	else if((BER>250000)&&(BER<=500000))
		BER_return=20;
	else if((BER>500000)&&(BER<=1000000))
		BER_return=10;
	else 
		BER_return=5;

	return BER_return;

}
unsigned char TC90522DemodulatorGetQuality(Device_Context *DC,unsigned char TunerDeviceID)
{
	unsigned char Data;
	unsigned char sequen;
	unsigned long cndat = 0;
	unsigned long cnmc = 0;
	unsigned long Value;
	double P = 0;
	//	 double CN = 0;
	unsigned char Quality = 0;

	unsigned long BerReg;
	unsigned long CycleReg;
	unsigned int BER;

//	DriverDebugPrint("(TC90522DemodulatorGetQuality)\n");
//	DriverDebugPrint("(TC90522DemodulatorGetQuality) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
		if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		{
			if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xBA,0x00) == TRUE)
			{
				if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xB0,&Data) == TRUE)
				{
					sequen = Data & 0x0F;
					if(sequen >= 8)
					{
						if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xA0,&Data) == TRUE)
						{
							BerReg = Data;
							BerReg <<= 8; 
							if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xA1,&Data) == TRUE)
							{
								BerReg |= Data;
								BerReg <<= 8; 
								if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xA2,&Data) == TRUE)
								{
									BerReg |= Data;
//									DriverDebugPrint("(TC90522DemodulatorGetStrength) BerReg : %d\n",BerReg);
									
									if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xA8,&Data) == TRUE)
									{
										CycleReg = Data;
										CycleReg <<= 8; 	
										if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xA9,&Data) == TRUE)
										{					
											CycleReg |= Data;
											BER = (BerReg) * (61274 / CycleReg);
										}
									}
									
								}
							}
						}
					}
				}
			}
		}
		else
		if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
		{
			if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xC3,&Data) == TRUE)
			{
				if((Data & 0x10) == 0)
				{ 
					if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xEB,&Data) == TRUE)
					{
						BerReg = Data;
						BerReg <<= 8; 
						if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xEC,&Data) == TRUE)
						{
							BerReg |= Data;
							BerReg <<= 8; 
							if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xED,&Data) == TRUE)
							{
								BerReg |= Data;
//								DriverDebugPrint("(TC90522DemodulatorGetStrength) BerReg : %d\n",BerReg);

								BER = (BerReg) * 80;
							}
						}
					}
				}
			}
		}
	}
	
	if(BER==0)
		Quality=100;
	else if((BER>0)&&(BER<=10))
		Quality=90;
	else if((BER>10)&&(BER<=100))
		Quality=80;
	else if((BER>100)&&(BER<=1000))
		Quality=70;
	else if((BER>1000)&&(BER<=10000))
		Quality=60;
	else if((BER>10000)&&(BER<=50000))
		Quality=50;
	else if((BER>50000)&&(BER<=100000))
		Quality=40;
	else if((BER>100000)&&(BER<=250000))
		Quality=30;
	else if((BER>250000)&&(BER<=500000))
		Quality=20;
	else if((BER>500000)&&(BER<=1000000))
		Quality=10;
	else 
		Quality=5;

	return Quality;
	
//	DriverDebugPrint("(TC90522DemodulatorGetQuality) Quality : %d\n",Quality);
	
	return Quality;
}

//  ##  Modify in 20130123 _ Nick
const unsigned long TC90522_ISDBTCNTable[] = {
     0, 0,    9778432, 5735416, 3903295, 2811328,
     2089200, 1583680, 1216799,  943867,  737184,
      578602,  455722,  359802,  284533,  225191,
      178317,  141219,  111834,   88556,   70125,
       55546,   44026,   34937,   27773,   22134,
       17696,   14204,   11455,    9287,    7573
};
const unsigned long TC90522_ISDBSCNTable[] = {
     0, 0,    35825, 34437, 32889, 31162,
     29249, 27174, 25008,  22846,  20774,
      18842,  17062,  15430,  13934,  12566,
      11321,  10198,  9199,   8323,   7568,
       6924,   6379,   5920,   5532,   5204,
       4925,   4685,   4479,    4300,    4145
};
unsigned long TC90522DemodulatorGetCNR(Device_Context *DC,unsigned char TunerDeviceID)
{
	unsigned char Data;
	unsigned char sequen;
	unsigned long cndat = 0;
	unsigned long cnmc = 0;
	unsigned int merdata;
	unsigned int merInt = 0;
	unsigned int merDec = 0;
	unsigned long MerDt;
	unsigned long d;
	
//	DriverDebugPrint("(TC90522DemodulatorGetCNR)\n");
//	DriverDebugPrint("(TC90522DemodulatorGetCNR) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
		if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		{
			if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xBA,0x00) == TRUE)
			{
				if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xB0,&Data) == TRUE)
				{
					sequen = Data & 0x0F;
					if(sequen >= 8)
					{ 
						if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x8B,&Data) == TRUE)
						{
							cndat = Data;
							cndat <<= 8; 
							if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x8C,&Data) == TRUE)
							{
								cndat |= Data;
								cndat <<= 8; 
								if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x8D,&Data) == TRUE)
								{
									cndat |= Data;
									
									DriverDebugPrint("(TC90522DemodulatorGetCNR) cndat : %d\n",cndat);
									
									MerDt = cndat;
									for (merdata = 2 ; (merdata < 31) && (MerDt <= TC90522_ISDBTCNTable[merdata]) ; merdata++);
									
									switch(merdata) 
									{
									       case 2:
										        merInt	= 0;
										        merDec	= 0;
										        break;
									       case 31:
										        merInt	= 30;
										        merDec	= 0;
										        break;
									       default:
										        d = (TC90522_ISDBTCNTable[merdata-1] - TC90522_ISDBTCNTable[merdata]) / 100;
										        merInt	= merdata - 1;
										        merDec	= (TC90522_ISDBTCNTable[merdata-1] - MerDt) / d;
										        break;
									}
								}
							}
						}
					}
				}
			}
		}
		else
		if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
		{
			if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xC3,&Data) == TRUE)
			{
				if((Data & 0x10) == 0)
				{ 
					if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xBC,&Data) == TRUE)
					{
						cnmc = Data;
						cnmc <<= 8; 
						if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xBD,&Data) == TRUE)
						{
							cnmc |= Data;
							
							DriverDebugPrint("(TC90522DemodulatorGetCNR) cnmc : %d\n",cnmc);

							MerDt = cnmc;
							
							if(MerDt<3000)
								return 0;
								
							for (merdata = 2 ; (merdata < 31) && (MerDt <= TC90522_ISDBSCNTable[merdata]) ; merdata++);
							
							switch(merdata) 
							{
								   case 2:
									    merInt	= 0;
									    merDec	= 0;
									    break;
								   case 31:
									    merInt	= 30;
									    merDec	= 0;
									    break;
								   default:
									    d = (TC90522_ISDBSCNTable[merdata-1] - TC90522_ISDBSCNTable[merdata]) / 100;
									    merInt	= merdata - 1;
									    merDec	= (TC90522_ISDBSCNTable[merdata-1] - MerDt) / d;
									    break;
							}							
						}
					}
				}
			}
		}
	}
	
	return merInt*100+merDec;
}
int TC90522DemodulatorGetCNRaw(Device_Context *DC,unsigned char TunerDeviceID)
{
	unsigned char Data;
	unsigned char sequen;
	unsigned long cndat = 0;
	unsigned long cnmc = 0;
	int ReturnValue = 0;
	
	DriverDebugPrint("(TC90522DemodulatorGetCNRaw)\n");
	DriverDebugPrint("(TC90522DemodulatorGetCNRaw) TunerDeviceID : 0x%x\n",TunerDeviceID);

	if(TunerDeviceID < JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER)
	{
		if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBT_MODE)
		{
			if(TC90522DemodulatorOFDM_I2C_Write(DC,TunerDeviceID,0xBA,0x00) == TRUE)
			{
				if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0xB0,&Data) == TRUE)
				{
					sequen = Data & 0x0F;
					if(sequen >= 8)
					{ 
						if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x8B,&Data) == TRUE)
						{
							cndat = Data;
							cndat <<= 8; 
							if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x8C,&Data) == TRUE)
							{
								cndat |= Data;
								cndat <<= 8; 
								if(TC90522DemodulatorOFDM_I2C_Read(DC,TunerDeviceID,0x8D,&Data) == TRUE)
								{
									cndat |= Data;
									
									DriverDebugPrint("(TC90522DemodulatorGetCNRaw) cndat : %d\n",cndat);

									ReturnValue = (int)cndat;
								}
							}
						}
					}
				}
			}
		}
		else
		if(DC->JackyFrontend.JackyGlobalFrontendDemodulationMode[TunerDeviceID] == ISDBS_MODE)
		{
			if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xC3,&Data) == TRUE)
			{
				if((Data & 0x10) == 0)
				{ 
					if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xBC,&Data) == TRUE)
					{
						cnmc = Data;
						cnmc <<= 8; 
						if(TC90522DemodulatorPSK_I2C_Read(DC,TunerDeviceID,0xBD,&Data) == TRUE)
						{
							cnmc |= Data;
							
							DriverDebugPrint("(TC90522DemodulatorGetCNRaw) cnmc : %d\n",cnmc);

							ReturnValue = (int)cnmc;
						}
					}
				}
			}
		}
	}
	
	return ReturnValue;
}
void TC90522DemodulatorDelayMilliseconds(unsigned long Milliseconds)
{
//    usleep(Milliseconds * 1000); 
	msleep(Milliseconds);
}
BOOL TC90522DemodulatorOFDM_I2C_Write(Device_Context *DC,unsigned char TunerDeviceID,unsigned char SubAddress,unsigned char Data)
{
    unsigned char I2CDeviceAddress;
	unsigned char WriteBuffer[2];
	BOOL Result;
    
    if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 2) 
       I2CDeviceAddress = 0x20;
	else
       I2CDeviceAddress = 0x24;

	WriteBuffer[0] = SubAddress;
	WriteBuffer[1] = Data;

	Result = I2C_PortingLayer_Write((Endeavour*) &DC->it9300,DC->JackyFrontend.JackyGlobalBackendI2CBusIndex[TunerDeviceID],I2CDeviceAddress,WriteBuffer,2);
	if(Result == FALSE)
	{
	   printk("(TC90522DemodulatorOFDM_I2C_Write) Failed !\n");
	}

    return Result;
}
BOOL TC90522DemodulatorOFDM_I2C_Read(Device_Context *DC,unsigned char TunerDeviceID,unsigned char SubAddress,unsigned char *pData)
{
    unsigned char I2CDeviceAddress;
	BOOL Result;

    if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 2) 
       I2CDeviceAddress = 0x20;
	else
       I2CDeviceAddress = 0x24;

	Result = I2C_PortingLayer_ReadNoStop((Endeavour*) &DC->it9300,DC->JackyFrontend.JackyGlobalBackendI2CBusIndex[TunerDeviceID],I2CDeviceAddress,&SubAddress,1,pData,1);
	if(Result == FALSE)
	{
	   printk("(TC90522DemodulatorOFDM_I2C_Read) Failed !\n");
	}

    return Result;
}
BOOL TC90522DemodulatorPSK_I2C_Write(Device_Context *DC,unsigned char TunerDeviceID,unsigned char SubAddress,unsigned char Data)
{
    unsigned char I2CDeviceAddress;
	unsigned char WriteBuffer[2];
	BOOL Result;

    if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
       I2CDeviceAddress = 0x22;
	else
       I2CDeviceAddress = 0x26;

	WriteBuffer[0] = SubAddress;
	WriteBuffer[1] = Data;

    Result = I2C_PortingLayer_Write((Endeavour*) &DC->it9300,DC->JackyFrontend.JackyGlobalBackendI2CBusIndex[TunerDeviceID],I2CDeviceAddress,WriteBuffer,2);
	if(Result == FALSE)
	{
	   printk("(TC90522DemodulatorPSK_I2C_Write) Failed !\n");
	}

    return Result;
}
BOOL TC90522DemodulatorPSK_I2C_Read(Device_Context *DC,unsigned char TunerDeviceID,unsigned char SubAddress,unsigned char *pData)
{
    unsigned char I2CDeviceAddress;
	BOOL Result;

    if((TunerDeviceID % JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER) == 0) 
       I2CDeviceAddress = 0x22;
	else
       I2CDeviceAddress = 0x26;

	Result = I2C_PortingLayer_ReadNoStop((Endeavour*) &DC->it9300,DC->JackyFrontend.JackyGlobalBackendI2CBusIndex[TunerDeviceID],I2CDeviceAddress,&SubAddress,1,pData,1);
	if(Result == FALSE)
	{
	   printk("(TC90522DemodulatorPSK_I2C_Read) Failed !\n");
	}

    return Result;
}

// ************************************************************************************
// ************************************************************************************
// ************************************************************************************

