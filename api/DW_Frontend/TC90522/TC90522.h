#ifndef _TC90522_DEMOD_H_
#define _TC90522_DEMOD_H_

#ifdef __cplusplus
extern "C" {
#endif

BOOL TC90522_Initialization(Device_Context *DC,unsigned char TunerDeviceID);
void TC90522_Uninitialization(Device_Context *DC,unsigned char TunerDeviceID);
BOOL TC90522_ResetInternal(Device_Context *DC,unsigned char TunerDeviceID);
BOOL TC90522_ChannelSearch(Device_Context *DC,unsigned char TunerDeviceID);
BOOL TC90522_SetTSID(Device_Context *DC,unsigned char TunerDeviceID,unsigned short TSID);
BOOL TC90522_SetPartialReceiption(Device_Context *DC,unsigned char TunerDeviceID,BOOL PartialReceiptionEnableFlag);
BOOL TC90522_CheckLock(Device_Context *DC,unsigned char TunerDeviceID,BOOL* pLock);
BOOL TC90522_GetStrength(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pStrength);
BOOL TC90522_GetQuality(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pQuality);
BOOL TC90522_GetCNR(Device_Context *DC,unsigned char TunerDeviceID,unsigned long* pCnrValue);
BOOL TC90522_GetCNRaw(Device_Context *DC,unsigned char TunerDeviceID,int* pCnRawValue);
// ************************************************************************************
// ************************************************************************************
// ************************************************************************************
unsigned char TC90522DemodulatorGetStrength(Device_Context *DC,unsigned char TunerDeviceID);
unsigned char TC90522DemodulatorGetQuality(Device_Context *DC,unsigned char TunerDeviceID);
unsigned long TC90522DemodulatorGetCNR(Device_Context *DC,unsigned char TunerDeviceID);
int TC90522DemodulatorGetCNRaw(Device_Context *DC,unsigned char TunerDeviceID);
void TC90522DemodulatorDelayMilliseconds(unsigned long Milliseconds);
BOOL TC90522DemodulatorOFDM_I2C_Write(Device_Context *DC,unsigned char TunerDeviceID,unsigned char SubAddress,unsigned char Data);
BOOL TC90522DemodulatorOFDM_I2C_Read(Device_Context *DC,unsigned char TunerDeviceID,unsigned char SubAddress,unsigned char *pData);
BOOL TC90522DemodulatorPSK_I2C_Write(Device_Context *DC,unsigned char TunerDeviceID,unsigned char SubAddress,unsigned char Data);
BOOL TC90522DemodulatorPSK_I2C_Read(Device_Context *DC,unsigned char TunerDeviceID,unsigned char SubAddress,unsigned char *pData);
// ************************************************************************************
// ************************************************************************************
// ************************************************************************************

#ifdef __cplusplus
}
#endif

#endif // _TC90522_DEMOD_H_










