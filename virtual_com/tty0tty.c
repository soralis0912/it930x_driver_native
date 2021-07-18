/* ########################################################################

   tty0tty - linux null modem emulator (module) 

   ########################################################################

   Copyright (c) : 2010  Luis Claudio Gambôa Lopes
 
    Based in Tiny TTY driver -  Copyright (C) 2002-2004 Greg Kroah-Hartman (greg@kroah.com)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */


#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#include <linux/sched/signal.h>
#else
#include <linux/sched.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0)
#include <linux/uaccess.h>
#else
#include <asm/uaccess.h>
#endif
#include "iocontrol.h"
#include "tty0tty.h"
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/delay.h>
#include "VCircularBuffer.h"

#define DRIVER_VERSION "v16.03.23.1"
#define DRIVER_AUTHOR "Luis Claudio Gamboa Lopes <lcgamboa@yahoo.com>"
#define DRIVER_DESC "tty0tty null modem driver"

/* Module information */
MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
MODULE_LICENSE("GPL");


#define TINY_TTY_MAJOR		PTY_MASTER_MAJOR	/* experimental range */
#ifdef CONFIG_COMPAT
#define TINY_TTY_MINORS		8	/* device number, always even*/
#else
#define TINY_TTY_MINORS		1	/* device number, always even*/
#endif
//#define SCULL_DEBUG

/* fake UART values */
//out
#define MCR_DTR		0x01
#define MCR_RTS		0x02
#define MCR_LOOP	0x04
//in
#define MSR_CTS		0x10
#define MSR_CD		0x20
#define MSR_DSR		0x40
#define MSR_RI		0x80

//#define VCOM_ONLY_SUPPORT_HOST_RX
#define RETURNCHANNEL_PID  0x201
#define TRANSPARENT_UART_PID 0x1FED
#define RETURNCHANNEL_PACKETLENGTH 184
#define TRANSPARENT_UART_PACKETLENGTH 184
#define TSPACKET_SIZE 188

//typedef unsigned char Byte;


#define NameString_size 50
#define MAX_RXDEVICE_NUMBER 32

struct file *Device_Handle_Filp[MAX_RXDEVICE_NUMBER] = {NULL};
spinlock_t Write_RingBuffer_Lock;
spinlock_t lock_circularbuffer[MAX_RXDEVICE_NUMBER];

static struct tty_port tport[TINY_TTY_MINORS];

typedef struct {
	u8 is_status_change;
	wait_queue_head_t read_wait;
}interrupt_params;

struct tty0tty_serial {
	struct tty_struct	*tty;		/* pointer to the tty for this device */
	int			open_count;	/* number of times this port has been opened */

	/* for tiocmget and tiocmset functions */
	int			msr;		/* MSR shadow */
	int			mcr;		/* MCR shadow */

	/* for ioctl fun */
	struct serial_struct	serial;
	wait_queue_head_t	wait;
	struct async_icount	icount;

	Byte *ToAP_DataBuffer;
	Byte *ToTxDriver_DataBuffer;
	
	Byte byBoardID;
	int iIsRun;	
	interrupt_params interrupt_data[MAX_RXDEVICE_NUMBER];
	VCircularBuffer cbuffer[MAX_RXDEVICE_NUMBER];
};


typedef struct{
	int iRxDeviceID;
	struct tty0tty_serial* ptty0tty;
}thread_data;

static struct tty0tty_serial *tty0tty_table[TINY_TTY_MINORS];	/* initially all NULL */
static struct task_struct* thread_send[MAX_RXDEVICE_NUMBER];  
static int thread_function(void *data)  
{  	
    SentUartData request;
    thread_data* pthread_data = (thread_data*) data;
    struct tty0tty_serial* tty0tty = pthread_data->ptty0tty;
    int iRxDeviceID = pthread_data->iRxDeviceID; 
	unsigned long flags;
	Byte pbyBuffer[184];
	int iRetValue = 0;
	
	printk("[tty_Virtual] thread_function iRxDeviceID = %d\n", iRxDeviceID);
	
	do{
		//printk("[tty_Virtual] thread %d is prepare ckeck \n", iRxDeviceID);	
		wait_event_interruptible(tty0tty->interrupt_data[iRxDeviceID].read_wait, tty0tty->interrupt_data[iRxDeviceID].is_status_change == true);
		
		//printk("[tty_Virtual] thread %d is Wakeup\n", iRxDeviceID);	
		
		spin_lock_irqsave(&lock_circularbuffer[iRxDeviceID], flags);
		
		iRetValue = cb_Read(&tty0tty->cbuffer[iRxDeviceID], pbyBuffer);

		
		spin_unlock_irqrestore(&lock_circularbuffer[iRxDeviceID], flags);
	
		if(!iRetValue)
		{
			//cb_Print(&tty0tty->cbuffer[iRxDeviceID]);	
			
			request.len = RETURNCHANNEL_PACKETLENGTH;
			request.cmd = (Dword)pbyBuffer;
			
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 35)
			Device_Handle_Filp[iRxDeviceID]->f_op->unlocked_ioctl(
				Device_Handle_Filp[iRxDeviceID],
				IOCTL_ITE_ENDEAVOUR_SENTUARTDATA,
				(unsigned long)&request);
#else
			Device_Handle_Filp[iRxDeviceID]->f_op->ioctl(
				Device_Handle_Filp[iRxDeviceID]->f_dentry->d_inode,
				Device_Handle_Filp[iRxDeviceID],
				IOCTL_ITE_ENDEAVOUR_SENTUARTDATA,
				(unsigned long)&request);
#endif		
			

		}else
		{
			tty0tty->interrupt_data[iRxDeviceID].is_status_change = false;
		}

		
		
	}while(tty0tty->iIsRun == 1);
	
	
	printk("[tty_Virtual] thread_function iRxDeviceID = %d End !!\n", iRxDeviceID);
	
	kfree(pthread_data);
    return 0;  

}  

struct file* file_open(const char* path, int index, int flags, int rights) {

    mm_segment_t oldfs;
    int err = 0;


    oldfs = get_fs();
    set_fs(KERNEL_DS);
    Device_Handle_Filp[index] = filp_open(path, flags, rights);
    set_fs(oldfs);

    if(IS_ERR(Device_Handle_Filp[index])) {
    	err = PTR_ERR(Device_Handle_Filp[index]);
		printk("%s is NULL !!!\n", path);

		Device_Handle_Filp[index] = NULL;
    	return NULL;
    }

    return Device_Handle_Filp[index];
}

void file_close(struct file* file, int index) {
	if(file)
	{
		filp_close(file, NULL);

		if(Device_Handle_Filp[index])
			Device_Handle_Filp[index] = NULL;
	}
}

#ifndef VCOM_ONLY_SUPPORT_HOST_RX


int TxCommand_out(struct tty0tty_serial *tty0tty, Byte *DataBuffer, Dword BufferLength)
{
	int iReturnLength = 0;
	int i=0;
	int iRxDeviceID = 0;
	mm_segment_t oldfs;
	SentUartData request;
	unsigned long flags;
	int iRetValue = 0;
	int iRetryCount = 0;
	
	
	if(BufferLength != RETURNCHANNEL_PACKETLENGTH)
	{
		printk("\n\n\n!!!!!!!!!!!!!! BufferLength != %d !!!!!!!!!!!!!!\n\n\n", RETURNCHANNEL_PACKETLENGTH);
		return -1;
	}

	oldfs = get_fs();
	set_fs(KERNEL_DS);
	
	request.len = BufferLength;
	request.cmd = (Dword)DataBuffer;
	
	if(tty0tty->byBoardID == 0x54) { //Multi-thread virtual com
		iRxDeviceID = (int)((DataBuffer[1] << 8 | DataBuffer[2]) & 0xFFFF);
		//printk("[tty_Virtual] - RxDevice ID = %x \n", iRxDeviceID);
		if(iRxDeviceID >= MAX_RXDEVICE_NUMBER ) {
			printk("[tty_Virtual] - %s - RxDevice ID is out of range! \n", __FUNCTION__);
			return -1;
		}
		
		if( Device_Handle_Filp[iRxDeviceID] != NULL ) {
			if(iRxDeviceID < MAX_RXDEVICE_NUMBER) {
				iRetryCount = 0;
				
				while(1) {				
					spin_lock_irqsave(&lock_circularbuffer[iRxDeviceID], flags);
				
					iRetValue = cb_Write(&tty0tty->cbuffer[iRxDeviceID], DataBuffer);
					//cb_Print(&tty0tty->cbuffer[iRxDeviceID]);	
				
					spin_unlock_irqrestore(&lock_circularbuffer[iRxDeviceID], flags);
					
					if(!iRetValue) {//Check if ring buffer is full
													
						tty0tty->interrupt_data[iRxDeviceID].is_status_change = true;
						wake_up_interruptible(&tty0tty->interrupt_data[iRxDeviceID].read_wait);
						
						iReturnLength =	RETURNCHANNEL_PACKETLENGTH;
						break;

					}else { //retry
						msleep(100);
						printk("[tty_Virtual] Retry %d .....\n", iRxDeviceID);
						iRetryCount ++;
						if(iRetryCount > 10) {
							printk("[tty_Virtual] Retry %d Fail.....\n", iRxDeviceID);
							iReturnLength = -1;
							break;
						}
					}
				}
			
			
			} else {
				printk("[tty_Virtual] RxDeviceID out of range\n");
			}
		}

	} else {
		for(i=0; i<MAX_RXDEVICE_NUMBER; i++)
		{
			//printk("sent command no: %d \n", i);
			
			if( Device_Handle_Filp[i] != NULL )
			{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 35)
				Device_Handle_Filp[i]->f_op->unlocked_ioctl(
					Device_Handle_Filp[i],
					IOCTL_ITE_ENDEAVOUR_SENTUARTDATA,
					(unsigned long)&request);
#else
				Device_Handle_Filp[i]->f_op->ioctl(
					Device_Handle_Filp[i]->f_dentry->d_inode,
					Device_Handle_Filp[i],
					IOCTL_ITE_ENDEAVOUR_SENTUARTDATA,
					(unsigned long)&request);
#endif
			}
		}		
	}
	

	set_fs(oldfs);

	return iReturnLength;
}
#endif

int SaveReturnChannelPackets(Byte *DataBuffer, int BufferLength)
{
	int retval = 0;
	struct tty_struct  *ttyx = NULL;

	//Word Receive_PID = 0;
	int cpy_offset = TSPACKET_SIZE-RETURNCHANNEL_PACKETLENGTH;//12;


	//printk("[tty_Virtual] - %s - \n", __FUNCTION__);
	//confirm Return channel packet
	if( ((DataBuffer[BufferLength-1] & 0xFF) != 0x0D))
	{
		printk("[tty_Virtual] - %s - NOT Return Channel Packet No Ending Tag!!!\n", __FUNCTION__);
		return -EPERM;
	}
	if( ((DataBuffer[cpy_offset] & 0xFF) != 0x23))
	{
		printk("[tty_Virtual] - %s - NOT Return Channel Packet No Leading Tag!!!\n", __FUNCTION__);
		return -EPERM;
	}
	
	if(tty0tty_table[0] != NULL)
	{

		unsigned long flags;

		if(tty0tty_table[0] != NULL)
		if(tty0tty_table[0]->open_count > 0)
			ttyx=tty0tty_table[0]->tty;

		spin_lock_irqsave(&Write_RingBuffer_Lock, flags);

		if(ttyx != NULL)
		{
			memset(tty0tty_table[0]->ToAP_DataBuffer, 0, TSPACKET_SIZE);
			memcpy(tty0tty_table[0]->ToAP_DataBuffer, DataBuffer+cpy_offset, RETURNCHANNEL_PACKETLENGTH);
			
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)			
			tty_insert_flip_string(ttyx->port, tty0tty_table[0]->ToAP_DataBuffer, RETURNCHANNEL_PACKETLENGTH);
			tty_flip_buffer_push(ttyx->port);
#else
			tty_insert_flip_string(ttyx, tty0tty_table[0]->ToAP_DataBuffer, RETURNCHANNEL_PACKETLENGTH);
			tty_flip_buffer_push(ttyx);
#endif
		}
		else
			printk("[tty_Virtual] - %s - ttyx == NULL !!!!!!!!!!!!!!!!!!!!\n", __FUNCTION__);

		spin_unlock_irqrestore(&Write_RingBuffer_Lock, flags);
		//printk("[tty_Virtual] - %s - END #####\n", __FUNCTION__);
	}
	else
		printk("NEED TO OPEN tty_Virtual DEVICE NODE !!!\n");

	return retval;
}

int SaveTransparentUARTPackets(Byte *DataBuffer, int BufferLength)
{
	int iRetval = 0;
	struct tty_struct  *ttyx = NULL;

	int iOffset = TSPACKET_SIZE-TRANSPARENT_UART_PACKETLENGTH;//Cut TS header
	Byte byDataLength = 0;


	if(tty0tty_table[0] != NULL)
	{

		unsigned long flags;

		if(tty0tty_table[0] != NULL)
		if(tty0tty_table[0]->open_count > 0)
			ttyx=tty0tty_table[0]->tty;

		spin_lock_irqsave(&Write_RingBuffer_Lock, flags);

		if(ttyx != NULL)
		{
			memset(tty0tty_table[0]->ToAP_DataBuffer, 0, TSPACKET_SIZE);
			byDataLength = DataBuffer[iOffset];
			if(byDataLength > TRANSPARENT_UART_PACKETLENGTH - 1)
				byDataLength = TRANSPARENT_UART_PACKETLENGTH - 1;
				
			memcpy(tty0tty_table[0]->ToAP_DataBuffer, DataBuffer+iOffset+1, byDataLength);
			
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)			
			tty_insert_flip_string(ttyx->port, tty0tty_table[0]->ToAP_DataBuffer, byDataLength);
			tty_flip_buffer_push(ttyx->port);
#else
			tty_insert_flip_string(ttyx, tty0tty_table[0]->ToAP_DataBuffer, byDataLength);
			tty_flip_buffer_push(ttyx);
#endif
		}
		else
			printk("[tty_Virtual] - %s - ttyx == NULL !!!!!!!!!!!!!!!!!!!!\n", __FUNCTION__);

		spin_unlock_irqrestore(&Write_RingBuffer_Lock, flags);
		//printk("[tty_Virtual] - %s - END #####\n", __FUNCTION__);
	}
	else
		printk("NEED TO OPEN tty_Virtual DEVICE NODE !!!\n");

	return iRetval;
}

EXPORT_SYMBOL(SaveReturnChannelPackets);
EXPORT_SYMBOL(SaveTransparentUARTPackets);



static int tty0tty_open(struct tty_struct *tty, struct file *file)
{
	struct tty0tty_serial *tty0tty;
    int index;
	int msr=0;	
	int mcr=0;
	unsigned long flags;
	int i=0;
	thread_data* data[MAX_RXDEVICE_NUMBER];
	
		
#ifdef SCULL_DEBUG
	printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif

	tty->driver_data = NULL;
	index = tty->index;

	tty0tty = tty0tty_table[index];
	if (tty0tty == NULL) {
		/* first time accessing this device, let's create it */
		tty0tty = kmalloc(sizeof(*tty0tty), GFP_KERNEL);
		if (!tty0tty)
		return -ENOMEM;
		
		tty0tty->open_count = 0;

		tty0tty_table[index] = tty0tty;

		tty0tty->ToAP_DataBuffer = kmalloc(TSPACKET_SIZE, GFP_KERNEL);
		tty0tty->ToTxDriver_DataBuffer = kmalloc(TSPACKET_SIZE, GFP_KERNEL);
	}
	
#ifdef CONFIG_COMPAT	
	tport[index].tty=tty;
	tty->port = &tport[index];
#endif

	if( (index % 2) == 0)
	{
		if(index+1 < TINY_TTY_MINORS)
		{
			if(tty0tty_table[index+1] != NULL)
			if(tty0tty_table[index+1]->open_count > 0)
				mcr=tty0tty_table[index+1]->mcr;
		}
	}
	else
	{
		if(index-1 >= 0)
		{
			if(tty0tty_table[index-1] != NULL)
			if(tty0tty_table[index-1]->open_count > 0)
				mcr=tty0tty_table[index-1]->mcr;	
		}
		
	}

	//null modem connection

	if( (mcr & MCR_RTS) == MCR_RTS )
	{
		msr |= MSR_CTS;
	}
	
	if( (mcr & MCR_DTR) == MCR_DTR )
	{
		msr |= MSR_DSR;
		msr |= MSR_CD;
	}
	
	tty0tty->msr = msr;
	
	tty0tty->byBoardID = 0;
	tty0tty->iIsRun = 1;
	
	spin_lock_irqsave(&Write_RingBuffer_Lock, flags);

	/* save our structure within the tty structure */
	tty->driver_data = tty0tty;
	tty0tty->tty = tty;

	++tty0tty->open_count;
	
	spin_unlock_irqrestore(&Write_RingBuffer_Lock, flags);

	for(i = 0; i < MAX_RXDEVICE_NUMBER; i++)
	{
		char NameBuffer[NameString_size] = {0};
		unsigned long DealWith_ReturnChannelPacket = 1;

		
		
		sprintf(NameBuffer, "/dev/px4-DTV%d", i);	            // Jacky Han Modified // original : "usb-it930x"	
		printk("Open Handle: %s\n", NameBuffer);
		if( file_open(NameBuffer, i, O_RDWR, 0) != NULL )
		{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 35)

	Device_Handle_Filp[i]->f_op->unlocked_ioctl(
			Device_Handle_Filp[i],
			IOCTL_ITE_DEMOD_DEALWITH_RETURNCHANNELPID,
			(unsigned long)&DealWith_ReturnChannelPacket);

	Device_Handle_Filp[i]->f_op->unlocked_ioctl(
			Device_Handle_Filp[i],
			IOCTL_ITE_ENDEAVOUR_GETBOARDID_KERNEL,
			(unsigned long)&tty0tty->byBoardID);
			
#else
	Device_Handle_Filp[i]->f_op->ioctl(
			Device_Handle_Filp[i]->f_dentry->d_inode,
			Device_Handle_Filp[i],
			IOCTL_ITE_DEMOD_DEALWITH_RETURNCHANNELPID,
			(unsigned long)&DealWith_ReturnChannelPacket);

	Device_Handle_Filp[i]->f_op->ioctl(
			Device_Handle_Filp[i]->f_dentry->d_inode,
			Device_Handle_Filp[i],
			IOCTL_ITE_ENDEAVOUR_GETBOARDID_KERNEL,
			(unsigned long)&tty0tty->byBoardID);	
							
#endif
			printk("[tty_Virtaul] Open Handle [%d] BoardID = %d \n", i, tty0tty->byBoardID);	
		}
	}

	if(tty0tty->byBoardID == 0x54) {	//Multi-thread virtual com
		for(i = 0; i < MAX_RXDEVICE_NUMBER; i++) {
			data[i] = (thread_data* )kmalloc(sizeof(thread_data), GFP_KERNEL);
			cb_Init(&tty0tty->cbuffer[i]);
			tty0tty->interrupt_data[i].is_status_change = false;
			init_waitqueue_head(&tty0tty->interrupt_data[i].read_wait);
			
			data[i]->iRxDeviceID = i;
			data[i]->ptty0tty = tty0tty;
			printk("[tty_Virtaul] data[%d].iRxDeviceID = %d!\n", i, data[i]->iRxDeviceID);  
			thread_send[i] = kthread_run(thread_function, (void*) data[i], "thread_send%d", i);  
			if (IS_ERR(thread_send[i])) {  
				printk("[tty_Virtaul] create kthread [%d] failed!\n", i);  
				break;
			}  
			else {  
				printk("[tty_Virtaul] create ktrhead [%d] ok!\n", i);  
			} 
			
		}
		
		

	}
	

	return 0;
}

static void do_close(struct tty0tty_serial *tty0tty)
{
	unsigned long flags;
	
	spin_lock_irqsave(&Write_RingBuffer_Lock, flags);

#ifdef SCULL_DEBUG
	printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif
	if (!tty0tty->open_count) {
		/* port was never opened */
		goto exit;
	}



	--tty0tty->open_count;
exit:
	spin_unlock_irqrestore(&Write_RingBuffer_Lock, flags);
   return;
}

static void tty0tty_close(struct tty_struct *tty, struct file *file)
{
	struct tty0tty_serial *tty0tty = tty->driver_data;
	int i = 0;
	unsigned long DealWith_ReturnChannelPacket = 0;
	
#ifdef SCULL_DEBUG
        printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif


	if(tty0tty->byBoardID == 0x54) { //Multi-thread virtual com

		
		tty0tty->iIsRun = 0;
		for(i = 0; i < MAX_RXDEVICE_NUMBER; i++) {
			tty0tty->interrupt_data[i].is_status_change = true;
			wake_up_interruptible(&tty0tty->interrupt_data[i].read_wait);	
			
		}	
	}

	
	for(i = 0; i < MAX_RXDEVICE_NUMBER; i++)
	{
		if( Device_Handle_Filp[i] != NULL )
		{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 35)
			Device_Handle_Filp[i]->f_op->unlocked_ioctl(
					Device_Handle_Filp[i],
					IOCTL_ITE_DEMOD_DEALWITH_RETURNCHANNELPID,
					(unsigned long)&DealWith_ReturnChannelPacket);
#else
			Device_Handle_Filp[i]->f_op->ioctl(
					Device_Handle_Filp[i]->f_dentry->d_inode,
					Device_Handle_Filp[i],
					IOCTL_ITE_DEMOD_DEALWITH_RETURNCHANNELPID,
					(unsigned long)&DealWith_ReturnChannelPacket);
#endif
			file_close(Device_Handle_Filp[i], i);
		}
	}

    if (tty0tty)
		do_close(tty0tty);
}	

static int tty0tty_write(struct tty_struct *tty, const unsigned char *buffer, int count)
{
	struct tty0tty_serial *tty0tty = tty->driver_data;
	int retval = -EINVAL;
	
	//struct tty_struct  *ttyx = NULL;	

#ifdef SCULL_DEBUG
    printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif

    if (!tty0tty)
    {
    	printk("[tty_Virtual] - %s - !tty0tty !!!!!!!!!!!!!\n", __FUNCTION__);
    	return -ENODEV;
    }
	
    //spin_lock_irqsave(&Write_RingBuffer_Lock, flags);
	
	if (!tty0tty->open_count)
	{
		/* port was not opened */
		printk("[tty_Virtual] - %s - !tty0tty->open_count !!!!!!!!!!!!!\n", __FUNCTION__);
		goto exit;
	}

#ifndef VCOM_ONLY_SUPPORT_HOST_RX
	retval = TxCommand_out(tty0tty, (Byte *)buffer, (Dword)count);
#endif

exit:	
	//spin_unlock_irqrestore(&Write_RingBuffer_Lock, flags);
	return retval;
}

static int tty0tty_write_room(struct tty_struct *tty) 
{
	struct tty0tty_serial *tty0tty = tty->driver_data;
	int room = -EINVAL;
	unsigned long flags;

#ifdef SCULL_DEBUG
    printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif

	if (!tty0tty)
		return -ENODEV;
	
	spin_lock_irqsave(&Write_RingBuffer_Lock, flags);
	
	if (!tty0tty->open_count) {
		/* port was not opened */
		goto exit;
	}

	/* calculate how much room is left in the device */
	room = 255;

exit:	
	spin_unlock_irqrestore(&Write_RingBuffer_Lock, flags);
	return room;
}

#define RELEVANT_IFLAG(iflag) ((iflag) & (IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK))

static void tty0tty_set_termios(struct tty_struct *tty, struct ktermios *old_termios)
{
	unsigned int cflag;


#ifdef SCULL_DEBUG
    printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
	cflag = tty->termios.c_cflag;
#else
	cflag = tty->termios->c_cflag;
#endif

	/* check that they really want us to change something */
	if (old_termios) {
		if ((cflag == old_termios->c_cflag) &&
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 7, 0)
		    (RELEVANT_IFLAG(tty->termios.c_iflag) == 
#else
			(RELEVANT_IFLAG(tty->termios->c_iflag) == 
#endif
		     RELEVANT_IFLAG(old_termios->c_iflag))) {
#ifdef SCULL_DEBUG
			printk(KERN_DEBUG " - nothing to change...\n");
#endif
			return;
		}
	}

#ifdef SCULL_DEBUG
	/* get the byte size */
	switch (cflag & CSIZE) {
		case CS5:
			printk(KERN_DEBUG " - data bits = 5\n");
			break;
		case CS6:
			printk(KERN_DEBUG " - data bits = 6\n");
			break;
		case CS7:
			printk(KERN_DEBUG " - data bits = 7\n");
			break;
		default:
		case CS8:
			printk(KERN_DEBUG " - data bits = 8\n");
			break;
	}
	
	/* determine the parity */
	if (cflag & PARENB)
		if (cflag & PARODD)
			printk(KERN_DEBUG " - parity = odd\n");
		else
			printk(KERN_DEBUG " - parity = even\n");
	else
		printk(KERN_DEBUG " - parity = none\n");

	/* figure out the stop bits requested */
	if (cflag & CSTOPB)
		printk(KERN_DEBUG " - stop bits = 2\n");
	else
		printk(KERN_DEBUG " - stop bits = 1\n");

	/* figure out the hardware flow control settings */
	if (cflag & CRTSCTS)
		printk(KERN_DEBUG " - RTS/CTS is enabled\n");
	else
		printk(KERN_DEBUG " - RTS/CTS is disabled\n");
	
	/* determine software flow control */
	/* if we are implementing XON/XOFF, set the start and 
	 * stop character in the device */
	if (I_IXOFF(tty) || I_IXON(tty)) {
		unsigned char stop_char  = STOP_CHAR(tty);
		unsigned char start_char = START_CHAR(tty);

		/* if we are implementing INBOUND XON/XOFF */
		if (I_IXOFF(tty))
			printk(KERN_DEBUG " - INBOUND XON/XOFF is enabled, "
				"XON = %2x, XOFF = %2x\n", start_char, stop_char);
		else
			printk(KERN_DEBUG" - INBOUND XON/XOFF is disabled\n");

		/* if we are implementing OUTBOUND XON/XOFF */
		if (I_IXON(tty))
			printk(KERN_DEBUG" - OUTBOUND XON/XOFF is enabled, "
				"XON = %2x, XOFF = %2x\n", start_char, stop_char);
		else
			printk(KERN_DEBUG" - OUTBOUND XON/XOFF is disabled\n");
	}

	/* get the baud rate wanted */
	printk(KERN_DEBUG " - baud rate = %d\n", tty_get_baud_rate(tty));
	
	baudrate = tty_get_baud_rate(tty);
	
	for(i = 0; i < MAX_RXDEVICE_NUMBER; i++)
	{
		if(Device_Handle_Filp[i] == NULL)
			continue;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 35)

	Device_Handle_Filp[i]->f_op->unlocked_ioctl(
			Device_Handle_Filp[i],
			IOCTL_ITE_ENDEAVOUR_SETUARTBAUDRATE,
			(unsigned long)&baudrate);
#else

	Device_Handle_Filp[i]->f_op->ioctl(
			Device_Handle_Filp[i]->f_dentry->d_inode,
			Device_Handle_Filp[i],
			IOCTL_ITE_ENDEAVOUR_SETUARTBAUDRATE,
			(unsigned long)&baudrate);
#endif
	}

	printk("[tty_Virtual] - %s done - \n", __FUNCTION__);
#endif
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 38)
static int tty0tty_tiocmget(struct tty_struct *tty)
#else
static int tty0tty_tiocmget(struct tty_struct *tty, struct file *file)
#endif
{
	struct tty0tty_serial *tty0tty = tty->driver_data;

	unsigned int result = 0;
	unsigned int msr = tty0tty->msr;
	unsigned int mcr = tty0tty->mcr;
	

	result = ((mcr & MCR_DTR)  ? TIOCM_DTR  : 0) |	/* DTR is set */
             ((mcr & MCR_RTS)  ? TIOCM_RTS  : 0) |	/* RTS is set */
             ((mcr & MCR_LOOP) ? TIOCM_LOOP : 0) |	/* LOOP is set */
             ((msr & MSR_CTS)  ? TIOCM_CTS  : 0) |	/* CTS is set */
             ((msr & MSR_CD)   ? TIOCM_CAR  : 0) |	/* Carrier detect is set*/
             ((msr & MSR_RI)   ? TIOCM_RI   : 0) |	/* Ring Indicator is set */
             ((msr & MSR_DSR)  ? TIOCM_DSR  : 0);	/* DSR is set */

	return result;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 38)
static int tty0tty_tiocmset(struct tty_struct *tty, 
			unsigned int set, unsigned int clear)
#else
static int tty0tty_tiocmset(struct tty_struct *tty, struct file *file,
                         unsigned int set, unsigned int clear)
#endif
{
	struct tty0tty_serial *tty0tty = tty->driver_data;
	unsigned int mcr = tty0tty->mcr;
	unsigned int msr=0;

#ifdef SCULL_DEBUG
        printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif

	if( (tty0tty->tty->index % 2) == 0)
	{ 
	   if(tty0tty_table[tty0tty->tty->index+1] != NULL) 
	 if (tty0tty_table[tty0tty->tty->index+1]->open_count > 0)
		   msr=tty0tty_table[tty0tty->tty->index+1]->msr;
	}
	else
	{
	   if(tty0tty_table[tty0tty->tty->index-1] != NULL) 
	 if (tty0tty_table[tty0tty->tty->index-1]->open_count > 0)
		   msr=tty0tty_table[tty0tty->tty->index-1]->msr;
	}
	
//null modem connection

	if (set & TIOCM_RTS)
        {  
	   mcr |= MCR_RTS;
	   msr |= MSR_CTS;
        }
 
	if (set & TIOCM_DTR)
        {
	   mcr |= MCR_DTR;
	   msr |= MSR_DSR;
	   msr |= MSR_CD;
        }

	if (clear & TIOCM_RTS)
        {
	   mcr &= ~MCR_RTS;
	   msr &= ~MSR_CTS;
        }
 
	if (clear & TIOCM_DTR)
        {
  	   mcr &= ~MCR_DTR;
	   msr &= ~MSR_DSR;
	   msr &= ~MSR_CD;
        }        

	/* set the new MCR value in the device */
	tty0tty->mcr = mcr;
    
	if( (tty0tty->tty->index % 2) == 0)
	{  
	   if(tty0tty_table[tty0tty->tty->index+1] != NULL) 
	 if (tty0tty_table[tty0tty->tty->index+1]->open_count > 0)
		   tty0tty_table[tty0tty->tty->index+1]->msr=msr;
	}
	else
	{
	   if(tty0tty_table[tty0tty->tty->index-1] != NULL) 
	 if (tty0tty_table[tty0tty->tty->index-1]->open_count > 0)
		   tty0tty_table[tty0tty->tty->index-1]->msr=msr;
	}

	return 0;
}


static int tty0tty_ioctl_tiocgserial(struct tty_struct *tty, 
                      unsigned int cmd, unsigned long arg)
{
	struct tty0tty_serial *tty0tty = tty->driver_data;

#ifdef SCULL_DEBUG
        printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif
	if (cmd == TIOCGSERIAL) {
		struct serial_struct tmp;

		if (!arg)
			return -EFAULT;

		memset(&tmp, 0, sizeof(tmp));

		tmp.type		= tty0tty->serial.type;
		tmp.line		= tty0tty->serial.line;
		tmp.port		= tty0tty->serial.port;
		tmp.irq			= tty0tty->serial.irq;
		tmp.flags		= ASYNC_SKIP_TEST | ASYNC_AUTO_IRQ;
		tmp.xmit_fifo_size	= tty0tty->serial.xmit_fifo_size;
		tmp.baud_base		= tty0tty->serial.baud_base;
		tmp.close_delay		= 5*HZ;
		tmp.closing_wait	= 30*HZ;
		tmp.custom_divisor	= tty0tty->serial.custom_divisor;
		tmp.hub6		= tty0tty->serial.hub6;
		tmp.io_type		= tty0tty->serial.io_type;

		if (copy_to_user((void __user *)arg, &tmp, sizeof(struct serial_struct)))
			return -EFAULT;
		return 0;
	}
	return -ENOIOCTLCMD;
}

static int tty0tty_ioctl_tiocmiwait(struct tty_struct *tty, 
                      unsigned int cmd, unsigned long arg)
{
	struct tty0tty_serial *tty0tty = tty->driver_data;

#ifdef SCULL_DEBUG
        printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif
	if (cmd == TIOCMIWAIT) {
		DECLARE_WAITQUEUE(wait, current);
		struct async_icount cnow;
		struct async_icount cprev;

		cprev = tty0tty->icount;
		while (1) {
			add_wait_queue(&tty0tty->wait, &wait);
			set_current_state(TASK_INTERRUPTIBLE);
			schedule();
			remove_wait_queue(&tty0tty->wait, &wait);

			/* see if a signal woke us up */
			if (signal_pending(current))
				return -ERESTARTSYS;

			cnow = tty0tty->icount;
			if (cnow.rng == cprev.rng && cnow.dsr == cprev.dsr &&
			    cnow.dcd == cprev.dcd && cnow.cts == cprev.cts)
				return -EIO; /* no change => error */
			if (((arg & TIOCM_RNG) && (cnow.rng != cprev.rng)) ||
			    ((arg & TIOCM_DSR) && (cnow.dsr != cprev.dsr)) ||
			    ((arg & TIOCM_CD)  && (cnow.dcd != cprev.dcd)) ||
			    ((arg & TIOCM_CTS) && (cnow.cts != cprev.cts)) ) {
				return 0;
			}
			cprev = cnow;
		}

	}
	return -ENOIOCTLCMD;
}

static int tty0tty_ioctl_tiocgicount(struct tty_struct *tty, 
                      unsigned int cmd, unsigned long arg)
{
	struct tty0tty_serial *tty0tty = tty->driver_data;

#ifdef SCULL_DEBUG
        printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif
	if (cmd == TIOCGICOUNT) {
		struct async_icount cnow = tty0tty->icount;
		struct serial_icounter_struct icount;

		icount.cts	= cnow.cts;
		icount.dsr	= cnow.dsr;
		icount.rng	= cnow.rng;
		icount.dcd	= cnow.dcd;
		icount.rx	= cnow.rx;
		icount.tx	= cnow.tx;
		icount.frame	= cnow.frame;
		icount.overrun	= cnow.overrun;
		icount.parity	= cnow.parity;
		icount.brk	= cnow.brk;
		icount.buf_overrun = cnow.buf_overrun;

		if (copy_to_user((void __user *)arg, &icount, sizeof(icount)))
			return -EFAULT;
		return 0;
	}
	return -ENOIOCTLCMD;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 38)
static int tty0tty_ioctl(struct tty_struct *tty,
                      unsigned int cmd, unsigned long arg)
#else
static int tty0tty_ioctl(struct tty_struct *tty, struct file *file,
                      unsigned int cmd, unsigned long arg)
#endif
{

#ifdef SCULL_DEBUG
	printk(KERN_DEBUG "%s - %04X \n", __FUNCTION__,cmd);
#endif

	switch (cmd) {
		case TIOCGSERIAL:
			return tty0tty_ioctl_tiocgserial(tty, cmd, arg);
		case TIOCMIWAIT:
			return tty0tty_ioctl_tiocmiwait(tty, cmd, arg);
		case TIOCGICOUNT:
			return tty0tty_ioctl_tiocgicount(tty, cmd, arg);
	}
	

	return -ENOIOCTLCMD;
}

static struct tty_operations serial_ops = {
	.open = tty0tty_open,
	.close = tty0tty_close,
	.write = tty0tty_write,
	.write_room = tty0tty_write_room,
	.set_termios = tty0tty_set_termios,
	.tiocmget = tty0tty_tiocmget,
	.tiocmset = tty0tty_tiocmset,
	.ioctl = tty0tty_ioctl,
};

static struct tty_driver *tty0tty_tty_driver;


static int __init tty0tty_init(void)
{

	int retval, i;
#ifdef SCULL_DEBUG
	printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif
	/* allocate the tty driver */
	tty0tty_tty_driver = alloc_tty_driver(TINY_TTY_MINORS);
	if (!tty0tty_tty_driver)
		return -ENOMEM;

	/* initialize the tty driver */
	tty0tty_tty_driver->owner = THIS_MODULE;
	tty0tty_tty_driver->driver_name = "tty_Virtual";
	tty0tty_tty_driver->name = "tnt";
    /* no more devfs subsystem */
	tty0tty_tty_driver->major = TINY_TTY_MAJOR;
	tty0tty_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	tty0tty_tty_driver->subtype = SERIAL_TYPE_NORMAL;
    tty0tty_tty_driver->flags = TTY_DRIVER_RESET_TERMIOS | TTY_DRIVER_REAL_RAW ;
    /* no more devfs subsystem */
	tty0tty_tty_driver->init_termios = tty_std_termios;
	tty0tty_tty_driver->init_termios.c_iflag = 0;
	tty0tty_tty_driver->init_termios.c_oflag = 0;
	tty0tty_tty_driver->init_termios.c_cflag = B38400 | CS8 | CREAD;
	tty0tty_tty_driver->init_termios.c_lflag = 0;
	tty0tty_tty_driver->init_termios.c_ispeed = 38400;
	tty0tty_tty_driver->init_termios.c_ospeed = 38400;
	
	


	tty_set_operations(tty0tty_tty_driver, &serial_ops);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39) 
	for(i=0;i<TINY_TTY_MINORS;i++)
	{
		tty_port_init(&tport[i]);		
	}
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0) 	
	for(i=0;i<TINY_TTY_MINORS;i++)
	{
		tty_port_link_device(&tport[i],tty0tty_tty_driver, i);
	}
#endif
	/* register the tty driver */
	retval = tty_register_driver(tty0tty_tty_driver);
	if (retval) {
		printk(KERN_ERR "failed to register tty0tty tty driver");
		put_tty_driver(tty0tty_tty_driver);
		return retval;
	}

	spin_lock_init(&Write_RingBuffer_Lock);
	for(i = 0; i < MAX_RXDEVICE_NUMBER; i ++)
	{
		spin_lock_init(&lock_circularbuffer[i]);
	}
	
	printk(KERN_INFO DRIVER_DESC " " DRIVER_VERSION "\n");
	return retval;
}

static void __exit tty0tty_exit(void)
{
	struct tty0tty_serial *tty0tty;
	int i;

#ifdef SCULL_DEBUG
        printk("[tty_Virtual] - %s - \n", __FUNCTION__);
#endif



	for (i = 0; i < TINY_TTY_MINORS; ++i)
	{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0) 
		tty_port_destroy(&(tport[i]));
#endif  
		tty_unregister_device(tty0tty_tty_driver, i);
	}

	tty_unregister_driver(tty0tty_tty_driver);

	/* shut down all of the timers and free the memory */
	for (i = 0; i < TINY_TTY_MINORS; ++i) {
		tty0tty = tty0tty_table[i];
		if (tty0tty) {
			/* close the port */
			while (tty0tty->open_count)
				do_close(tty0tty);



			//Don't put this in "do_close" function,
			//because it will not be created in "tty0tty_open" again until tty_Virtual module is removed.
			if(tty0tty->ToAP_DataBuffer)
				kfree(tty0tty->ToAP_DataBuffer);
			if(tty0tty->ToTxDriver_DataBuffer)
				kfree(tty0tty->ToTxDriver_DataBuffer);

			/* shut down our timer and free the memory */
			kfree(tty0tty);
			tty0tty_table[i] = NULL;
		}
	}
}

module_init(tty0tty_init);
module_exit(tty0tty_exit);
