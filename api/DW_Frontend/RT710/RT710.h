#ifndef  _RT710_H_ 
#define _RT710_H_

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------//
//                   RT710 Function                         //
//----------------------------------------------------------//
BOOL RT710_Initialization(Device_Context *DC,unsigned char TunerDeviceID);
void RT710_Uninitialization(Device_Context *DC,unsigned char TunerDeviceID);
BOOL RT710_SetFrequency(Device_Context *DC,unsigned char TunerDeviceID);
char RT710_GetRssi(Device_Context *DC,unsigned char TunerDeviceID);

// ************************************************************************************
// ************************************************************************************
// ************************************************************************************

#ifdef __cplusplus
}
#endif

#endif


