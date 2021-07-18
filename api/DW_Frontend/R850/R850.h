#ifndef _R850_H_ 
#define _R850_H_

#ifdef __cplusplus
extern "C" {
#endif

BOOL R850_Initialization(Device_Context *DC,unsigned char TunerDeviceID);
void R850_Uninitialization(Device_Context *DC,unsigned char TunerDeviceID);
BOOL R850_SetFrequency(Device_Context *DC,unsigned char TunerDeviceID);
char R850_GetRssi(Device_Context *DC,unsigned char TunerDeviceID);

#ifdef __cplusplus
}
#endif

#endif // _R850_H_