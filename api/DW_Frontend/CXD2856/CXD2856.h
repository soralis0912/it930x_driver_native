#ifndef __CXD2856_H__
#define __CXD2856_H__

#include "sony_integ.h"
#include "sony_demod.h"
#include "sony_i2c.h"

#ifdef __cplusplus
extern "C"
{
#endif

BOOL CXD2856_Initialization(Device_Context *DC,unsigned char TunerDeviceID);
void CXD2856_Uninitialization(Device_Context *DC,unsigned char TunerDeviceID);
BOOL CXD2856_ResetInternal(Device_Context *DC,unsigned char TunerDeviceID);
BOOL CXD2856_SetFrequency(Device_Context *DC,unsigned char TunerDeviceID);
BOOL CXD2856_SetTSID(Device_Context *DC,unsigned char TunerDeviceID,unsigned short TSID);
BOOL CXD2856_ChannelSearch(Device_Context *DC,unsigned char TunerDeviceID);
BOOL CXD2856_CheckLock(Device_Context *DC,unsigned char TunerDeviceID,BOOL* pLock);
BOOL CXD2856_GetStrength(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pStrength);
BOOL CXD2856_GetQuality(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pQuality);
BOOL CXD2856_GetCNRRaw(Device_Context *DC,unsigned char TunerDeviceID,unsigned short* pCNRRaw);
BOOL CXD2856_GetErrorRate(Device_Context *DC,unsigned char TunerDeviceID,unsigned char* pErrorRate);

#ifdef __cplusplus
}
#endif

#endif