#include "type.h"
#include <linux/stddef.h>
#include <linux/slab.h>

#define VCOM_CIRCULARBUFFER_COUNT 10
#define VCOM_PACKETLENGTH 184
//#define VCOM_CIRCULARBUFFER_SIZE VCOM_CIRCULARBUFFER_COUNT * VCOM_PACKETLENGTH
#define VCOM_CIRCULARBUFFER_SIZE 1840 //CIRCULARBUFFER_COUNT * RETURNCHANNEL_PACKETLENGTH



typedef struct {
    Dword   dwStart;  /* index of oldest element              */
    Dword   dwEnd;    /* index at which to write new element  */
    Byte		byFlag;	/* Full Flag */
    Byte		abyBuffer[VCOM_CIRCULARBUFFER_SIZE];  /* vector of elements                   */
} VCircularBuffer;
 
void cb_Init(VCircularBuffer *cb) {
    cb->dwStart = VCOM_PACKETLENGTH;
    cb->dwEnd   = VCOM_PACKETLENGTH;
    cb->byFlag = 0;
    //memset(cb->abyBuffer, 0, VCOM_CIRCULARBUFFER_SIZE);
}


void cb_Print(VCircularBuffer *cb) {
	int i = 0, j = 0;
    printk("[tty_Virtual] start=%ld, end=%ld \n", cb->dwStart, cb->dwEnd);
    
    for(i = 0; i < VCOM_CIRCULARBUFFER_COUNT; i ++)
    {
		for(j = 0; j < VCOM_PACKETLENGTH; j++)
		{
			printk("[%02x] %02x ", j, cb->abyBuffer[i * VCOM_PACKETLENGTH + j]);	
			if(j%10 == 9)
				printk("\n");
		}
		printk("\n");
	}
	
}



int cb_Write(VCircularBuffer *cb, Byte* pbyData) {
	
	//Buffer is Full
	if((cb->dwStart == cb->dwEnd) && (cb->byFlag)) {
		//printk("[tty_Virtual] Ring Buffer is Full!\n");
		return -1;
	}else {
		cb->dwEnd  = (cb->dwEnd + VCOM_PACKETLENGTH) % VCOM_CIRCULARBUFFER_SIZE;
		memcpy(cb->abyBuffer + cb->dwEnd, pbyData, VCOM_PACKETLENGTH);	
		if(cb->dwStart == cb->dwEnd) {
			cb->byFlag = 1;
		}	
	}
	return 0;
}

int cb_Read(VCircularBuffer *cb, Byte *pbydata) {
    //buffer is empty
    if((cb->dwStart == cb->dwEnd) && (!cb->byFlag)) {
		//printk("[tty_Virtual] Ring Buffer is Empty!\n");
		return -1;
	}else {
		cb->dwStart = (cb->dwStart + VCOM_PACKETLENGTH) % VCOM_CIRCULARBUFFER_SIZE;
		
		memcpy(pbydata ,cb->abyBuffer + cb->dwStart, VCOM_PACKETLENGTH);
		
		if(cb->dwStart == cb->dwEnd) {
			cb->byFlag = 0;
		}		
	}	
	return 0;
		
}


