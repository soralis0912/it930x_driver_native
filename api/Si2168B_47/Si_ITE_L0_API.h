#ifndef __SI_ITE_L0_API_H
#define __SI_ITE_L0_API_H

#include "IT9300.h"

#include <linux/kernel.h>
#include <linux/module.h>
#define SiTRACE(...) 
//#define SiTRACE	printk
#define SiTraceConfiguration(...) 
#define sprintf(...)
#define strcat(...)
#define SiERROR(...)


typedef struct SiliconLabs_ITE_L0_Context
{
	Handle			endeavour_si;
    int				address;
	unsigned char	bus;
	unsigned char	bridx;
    int             indexSize;
    int             mustReadWithoutStop;
} SiliconLabs_ITE_L0_Context;


unsigned int siLabs_ite_system_time (void);
void siLabs_ite_system_wait (int time_ms);

void siLabs_ite_L0_SetAddress (SiliconLabs_ITE_L0_Context* i2c, int add, Endeavour* endeavour, unsigned char chip, unsigned char i2cbus);
unsigned char siLabs_ite_L0_ReadCommandBytes (SiliconLabs_ITE_L0_Context* i2c, unsigned char iNbBytes, unsigned char *pucDataBuffer);
unsigned char siLabs_ite_L0_WriteCommandBytes (SiliconLabs_ITE_L0_Context* i2c, unsigned char iNbBytes, unsigned char *pucDataBuffer);












#endif
