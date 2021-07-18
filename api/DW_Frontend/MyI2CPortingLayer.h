#ifndef __MYI2CPORTINGlAYER_H__
#define __MYI2CPORTINGlAYER_H__

BOOL I2C_PortingLayer_Write(
    Endeavour* endeavour,
	unsigned char Bus,
    unsigned char I2CDeviceAddress,
    unsigned char* pWriteBuffer,
    unsigned short WriteBufferLength
);

BOOL I2C_PortingLayer_Read(
	Endeavour* endeavour,
	unsigned char Bus,
	unsigned char I2CDeviceAddress,
	unsigned char* pReadBuffer, 
	unsigned char ReadBufferLength
);

BOOL I2C_PortingLayer_ReadNoStop(
    Endeavour* endeavour,
	unsigned char Bus,
    unsigned char I2CDeviceAddress,
	unsigned char* pWriteBuffer,
	unsigned char WriteBufferLength,
    unsigned char* pReadBuffer, 
	unsigned char ReadBufferLength
);

#endif