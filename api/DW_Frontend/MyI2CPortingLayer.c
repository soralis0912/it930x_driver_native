#include <linux/kernel.h> 

#include "it930x-core.h"
#include "MyI2CPortingLayer.h"
#include "MyDebug.h"

BOOL I2C_PortingLayer_Write(
    Endeavour* endeavour,
	unsigned char Bus,
    unsigned char I2CDeviceAddress,
    unsigned char* pWriteBuffer,
    unsigned short WriteBufferLength
)
{
	Dword error;

//    DriverDebugPrint("(I2C_PortingLayer_Write)\n");
//    DriverDebugPrint("(I2C_PortingLayer_Write) Bus : 0x%x\n",Bus);
//    DriverDebugPrint("(I2C_PortingLayer_Write) I2CDeviceAddress : 0x%x\n",I2CDeviceAddress);
//    DriverDebugPrint("(I2C_PortingLayer_Write) WriteBufferLength : %d\n",WriteBufferLength);

    error = IT9300_writeGenericRegisters(endeavour,0,(Byte)Bus,(Byte)I2CDeviceAddress,(Byte)WriteBufferLength,(Byte*)pWriteBuffer);
    if(error != Error_NO_ERROR)
	{
	   printk("(I2C_PortingLayer_Write) Failed !\n");

       return FALSE;
	}
	else
       return TRUE;
}

BOOL I2C_PortingLayer_Read(
	Endeavour* endeavour,
	unsigned char Bus,
	unsigned char I2CDeviceAddress,
	unsigned char* pReadBuffer, 
	unsigned char ReadBufferLength
)
{
	Dword error;

//    DriverDebugPrint("(I2C_PortingLayer_Read)\n");
//    DriverDebugPrint("(I2C_PortingLayer_Read) Bus : 0x%x\n",Bus);
//    DriverDebugPrint("(I2C_PortingLayer_Read) I2CDeviceAddress : 0x%x\n",I2CDeviceAddress);
//    DriverDebugPrint("(I2C_PortingLayer_Read) ReadBufferLength : %d\n",ReadBufferLength);

    error = IT9300_readGenericRegisters(endeavour,0,(Byte)Bus,(Byte)I2CDeviceAddress,(Byte)ReadBufferLength,(Byte*)pReadBuffer);
    if(error != Error_NO_ERROR)
	{
	   printk("(I2C_PortingLayer_Read) Failed !\n");

       return FALSE;
	}
	else
       return TRUE;
}
BOOL I2C_PortingLayer_ReadNoStop(
    Endeavour* endeavour,
	unsigned char Bus,
    unsigned char I2CDeviceAddress,
	unsigned char* pWriteBuffer,
	unsigned char WriteBufferLength,
    unsigned char* pReadBuffer, 
	unsigned char ReadBufferLength
)
{
	Dword error;

//    DriverDebugPrint("(I2C_PortingLayer_ReadNoStop)\n");
//    DriverDebugPrint("(I2C_PortingLayer_ReadNoStop) Bus : 0x%x\n",Bus);
//    DriverDebugPrint("(I2C_PortingLayer_ReadNoStop) I2CDeviceAddress : 0x%x\n",I2CDeviceAddress);
//    DriverDebugPrint("(I2C_PortingLayer_ReadNoStop) WriteBufferLength : %d\n",WriteBufferLength);
//    DriverDebugPrint("(I2C_PortingLayer_ReadNoStop) ReadBufferLength : %d\n",ReadBufferLength);

    error = IT9300_readGenericRegistersExtend(endeavour,0,(Byte)Bus,(Byte)I2CDeviceAddress,1,(Byte)WriteBufferLength,(Byte*)pWriteBuffer,(Byte)ReadBufferLength,(Byte*)pReadBuffer);
    if(error != Error_NO_ERROR)
	{
	   printk("(I2C_PortingLayer_ReadNoStop) Failed !\n");

       return FALSE;
	}
	else
       return TRUE;
}
