#ifndef _IT930x_H_
#define _IT930x_H_

#define _GNU_SOURCE

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/usb.h>
#include <linux/version.h>
#include <linux/mutex.h>
#include <linux/stddef.h>
#include <linux/spinlock.h>
#include <linux/gfp.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/reboot.h>
#include <linux/notifier.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0)
#include <linux/uaccess.h>
#else
#include <asm/uaccess.h>
#endif

#include "MyDependency.h"        // Jacky Han Added

#include "iocontrol.h"
#include "IT9300.h"
#include "IT9133.h"
#include "userdef.h"
#include "type.h"
#include "brType.h"

#include "firmware.h"
#include "firmware_V2.h"
#include "firmware_V2I.h"
#include "version.h"
#include "brfirmware.h"
#include "brRegister.h"

#include "usb2impl.h"

#include "dibx09x_common.h"

#include "mxl_eagle_apis.h"
#include "sony_cxd_apis.h"

#include <linux/stddef.h>
#include <linux/delay.h>
#include <linux/types.h>

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 39)
	#include <linux/smp_lock.h>
#endif

/* Get a minor range for your devices from the usb maintainer */
#ifdef CONFIG_USB_DYNAMIC_MINORS
	#define USB_it930X_MINOR_BASE	0
#else
	#define USB_it930X_MINOR_BASE	192
#endif

#define DRIVER_RELEASE_VERSION	"v18.11.01.1"

/*********************Rx_ID_Length***************/
#define RX_ID_LENGTH 5

/***************** customization *****************/

#define URB_COUNT   8        // Jacky Han Modified // 8 // original value : 4
#define BR_URB_COUNT   8     // Jacky Han Modified // 8 // original value : 4

#define DEBUG 0          // Jacky Han Modified
#if DEBUG 
	#define deb_data(args...)   printk(KERN_NOTICE args)
#else
	#define deb_data(args...)
#endif

#define BCAS_ENABLE 1  //For bcas enable/disable : 1/0     // Jacky Han Enabled BCAS functionality In 07/13/2017

#define WORK_QUEUE_INIT		1
//#define PATCH_FOR_NX		1

/***************** from compat.h *****************/

#if LINUX_VERSION_CODE <=  KERNEL_VERSION(2,6,18)
typedef int bool;
#define true	1
#define false	0
#endif


#define USB_VID_UNKNOWN                           0x0000         /* Jacky Han */
#define USB_PID_UNKNOWN                           0x0000         /* Jacky Han */
#define USB_VID_DIGITALWARRIOR                    0x0511         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXW3U4             0x083F         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXW3PE4            0x023F         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXW3PE5            0x073F         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXQ3U4             0x084A         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXQ3PE4            0x024A         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXQ3PE5            0x074A         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXMLT5PE           0x024E         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXMLT5U            0x084E         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXMLT8PE3          0x0252         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXMLT8PE5          0x0253         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXMLT8UT           0x0051         /* Jacky Han */
#define USB_PID_DIGITALWARRIOR_PXMLT8UTS          0x0050         /* Jacky Han */

/***************** from device.h *****************/
#define SLAVE_DEMOD_2WIREADDR		0x3A

#define TS_PACKET_SIZE				188
#define TS_PACKET_COUNT_FU			21

/***************** from driver.h *****************/
#define TS_FRAMES_HI 128
#define TS_FRAMES_FU 128
#define MAX_USB20_IRP_NUM  5
#define MAX_USB11_IRP_NUM  2

/***************** from afdrv.h *****************/
//#define GANY_ONLY 0x42F5
#define EEPROM_FLB_OFS  8

#define EEPROM_IRMODE		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x10)	//00:disabled, 01:HID
#define EEPROM_SELSUSPEND	(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28)	//Selective Suspend Mode
#define EEPROM_TSMODE		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 1)	//0:one ts, 1:dual ts
#define EEPROM_2WIREADDR		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 2)	//MPEG2 2WireAddr
#define EEPROM_SUSPEND		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 3)	//Suspend Mode
#define EEPROM_IRTYPE		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 4)	//0:NEC, 1:RC6
#define EEPROM_SAWBW1		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 5)
#define EEPROM_XTAL1			(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 6)	//0:28800, 1:20480
#define EEPROM_SPECINV1		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 7)
#define EEPROM_TUNERID		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30 + 4)
#define EEPROM_IFFREQL		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30) 
#define EEPROM_IFFREQH		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30 + 1)   
#define EEPROM_IF1L			(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30 + 2)   
#define EEPROM_IF1H			(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30 + 3)
#define EEPROM_SHIFT			(0x10)	//EEPROM Addr Shift for slave front end
#define EEPROM_EVBNUM		(OVA_EEPROM_CFG + 0x38)	//0x4994 + 0x38
#define EEPROM_CHIPTYPE_1stNLC_1	(OVA_EEPROM_CFG + 0x31)
#define EEPROM_CHIPTYPE_1stNLC_2	(OVA_EEPROM_CFG + 0x32)
#define EEPROM_CHIPTYPE_1stNLC_3	(OVA_EEPROM_CFG + 0x35)
#define EEPROM_CHIPTYPE_1stNLC_4	(OVA_EEPROM_CFG + 0x36)
#define EEPROM_CHIPTYPE_otherNLC	(OVA_EEPROM_CFG + 0x37)
#define EEPROM_BOARDID	(OVA_EEPROM_CFG + 0x54)

#define EEPROM_VENDOR_ID     (OVA_EEPROM_CFG + 0x08)                               // Jacky Han Added
#define EEPROM_PRODUCT_ID    (OVA_EEPROM_CFG + 0x0A)                               // Jacky Han Added
#define EEPROM_SERIAL_NUMBER (OVA_EEPROM_CFG + 0x19)                               // Jacky Han Added

#define to_afa_dev(d) container_of(d, IT930x_Device it930x_dev, kref)

//belowd enum is for recognizing different receiver from EEPROM
/**********************************************************************
* 
*             Chip Type                   |    enum name 
* ----------------------------------------|--------------------
*  chip_Type[][] = 1 : IT913X             |    EEPROM_IT913X
*                = 5 : Si2168B            |    EPROM_Si2168B
*                = 6 : MxL608 + Atbm782x  |    EEPROM_MxL608_Atbm782x
*                = 7 : Dibcom9090         |    EEPROM_Dibcom9090
*                = a : Dibcom9090_4chan   |    EEPROM_Dibcom9090_4chan
*
************************************************************************/
typedef enum{
	EEPROM_IT913X	= 1,
	EEPROM_Si2168B = 5,
	EEPROM_MxL608_Atbm782x,
	EEPROM_Dibcom9090,
	EEPROM_Dibcom9090_4chan = 10,
	//EEPROM_MXL69X = 15,
	EEPROM_MXL691 = 15,
	EEPROM_MXL691_DUALP = 20,//691
	EEPROM_MXL692 = 21,
	EEPROM_MXL248 = 22,
	EEPROM_CXD285X = 23,
	
	EEPROM_PXW3U4 = 100,                          // Jacky Han Inserted         
	EEPROM_PXW3PE4 = 101,                         // Jacky Han Inserted  
	EEPROM_PXW3PE5 = 102,                         // Jacky Han Inserted  
	EEPROM_PXQ3U4 = 103,                          // Jacky Han Inserted         
	EEPROM_PXQ3PE4 = 104,                         // Jacky Han Inserted         
	EEPROM_PXQ3PE5 = 105,                         // Jacky Han Inserted         
	EEPROM_PXMLT5PE = 106,                        // Jacky Han Inserted       
	EEPROM_PXMLT5U = 107,                         // Jacky Han Inserted 
	EEPROM_PXMLT8PE3 = 108,                       // Jacky Han Inserted 
	EEPROM_PXMLT8PE5 = 109,                       // Jacky Han Inserted 
	EEPROM_PXMLT8UT = 110,                        // Jacky Han Inserted 
	EEPROM_PXMLT8UTS = 111,                       // Jacky Han Inserted 
	EEPROM_UNKNOWN = 255,                         // Jacky Han Inserted        

}EEPROM_RECEIVER_TYPE;


/***************** debug message *****************/
//define EEPROM_DEBUG_MESSAGE
//define PROBE_DEBUG_MESSAGE

//#define CHECK_SHIFT	//check buffer shift

extern struct usb_device *udevs;

/* AirHD no use, RC, after kernel 38 support */
struct it930x_config {
	u8  dual_mode:1;
	u16 mt2060_if1[2];
	u16 firmware_size;
	u16 firmware_checksum;
	u32 eeprom_sum;
};

struct it930x_ofdm_channel {
	u32 RF_kHz;
	u8  Bw;
	s16 nfft;
	s16 guard;
	s16 nqam;
	s16 vit_hrch;
	s16 vit_select_hp;
	s16 vit_alpha;
	s16 vit_code_rate_hp;
	s16 vit_code_rate_lp;
	u8  intlv_native;
};

/* AirHD no use, RC, after kernel 38 support */
struct tuner_priv {
	struct tuner_config *cfg;
	struct i2c_adapter *i2c;
	
	u32 frequency;
	u32 bandwidth;
	u16 if1_freq;
	u8  fmfreq;
};

typedef struct _TUNER_INFO {
    Bool bTunerInited;
    Bool bSettingFreq;
    Byte TunerId;
    Bool bTunerOK;
	
	Bool bTunerLock;//AirHD
	
    //Tuner_struct MXL5005_Info;

} TUNER_INFO, *PTUNER_INFO;

typedef struct _FILTER_CONTEXT_HW {
    Dword ulCurrentFrequency;
    Word  ucCurrentBandWidth;  
    Dword ulDesiredFrequency;
    Word  ucDesiredBandWidth;     
    Bool bTimerOn;
    Byte GraphBuilt;
    TUNER_INFO tunerinfo; 
    int  bEnPID;
    Bool bApOn;
    int bResetTs;
    Byte OvrFlwChk;
} FILTER_CONTEXT_HW, *PFILTER_CONTEXT_HW;

struct DIB9090_DEVICE {
	struct dibDataBusHost *i2c;
	struct dibFrontend frontend[4]; //dibFrontend
	struct dibDemodMonitor monitor[4]; //dibDemodMonitor
	Dword frequency;
	Word bandwidth;
};

/***************************************************************************/
/************************ Jacky Han Insertion Start ************************/
/***************************************************************************/
struct JACKY_FRONTEND_DEVICE
{
	 EEPROM_RECEIVER_TYPE ReceiverType;

	 unsigned char JackyGlobalBackendI2CBusIndex[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];

     Bool JackyGlobalFrontendInitializationFlag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
	 unsigned char JackyGlobalFrontendLogicalTunerDeviceID[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     Bool JackyGlobalFrontendOpenFlag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     Bool JackyGlobalFrontendLogicalPowerEnableFlag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     Bool JackyGlobalFrontendTunerInitializationFlag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     Bool JackyGlobalFrontendDemodInitializationFlag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];

     Dword JackyGlobalFrontendLastTuningFrequencyKhz[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
	 Word JackyGlobalFrontendLastTuningBandwidthKhz[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
	 DTV_MODULATION_CONSTELLATION JackyGlobalFrontendLastTuningSignalModulationType[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];

     DTV_DEMODULATION_MODE JackyGlobalFrontendLastTuningDemodulationMode[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     DTV_DEMODULATION_MODE JackyGlobalFrontendDemodulationMode[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];

	 Bool JackyGlobalFrontendLastTuningDemodulatorLockStatus[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];

     Dword JackyGlobalFrontendRequestTuningFrequencyKhz[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
	 Word JackyGlobalFrontendRequestTuningBandwidthKhz[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
	 DTV_MODULATION_CONSTELLATION JackyGlobalFrontendRequestTuningSignalModulationType[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];

     Bool JackyGlobalFrontendSystemPowerFlag;
     Bool JackyGlobalFrontendSystemTsOutputConfigFlag;
     Bool JackyGlobalFrontendSystemLNBPowerEnableFlag;
     Bool JackyGlobalFrontendS0LNBPowerEnableFlag;
     Bool JackyGlobalFrontendS1LNBPowerEnableFlag;

     UINT8 RT710_R0TOR3_Write_Arry[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][4];
     UINT8 RT710_Chip_type_flag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];  
     UINT8 RT710_Reg_Arry[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][RT710_Reg_Num];

     UINT8 R850_Chip[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];			         
     UINT32 R850_IF_GOLOBAL[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_Xtal_Pwr[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];			 
     UINT8 R850_Xtal_Pwr_tmp[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];		 
     UINT8 R850_Mixer_Mode[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_Mixer_Amp_LPF_IMR_CAL[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     R850_SysFreq_Info_Type R850_SysFreq_Info1[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];         
     R850_Sys_Info_Type R850_Sys_Info1[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];           
     R850_Freq_Info_Type R850_Freq_Info1[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_Array[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][R850_REG_NUM];
     R850_SectType R850_IMR_Data[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][R850_IMR_POINT_NUM];
     UINT8 R850_IMR_point_num[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_Initial_done_flag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_IMR_done_flag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_Bandwidth[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];   
     UINT8 R850_Lpf_Lsb[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_Fil_Cal_flag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][R850_STD_SIZE];
     UINT8 R850_Fil_Cal_BW[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][R850_STD_SIZE];
     UINT8 R850_Fil_Cal_code[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][R850_STD_SIZE];
     UINT8 R850_Fil_Cal_LpfLsb[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][R850_STD_SIZE];    
     R850_Standard_Type R850_pre_standard[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_SBY[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER][R850_REG_NUM];
     UINT8 R850_Standby_flag[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     UINT8 R850_IMR_Cal_Result[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER]; 

	 CXD2856_USER_DATA CXD2856_UserData[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER]; 
     sony_integ_t CXD2856_SonyInteg[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     sony_demod_t CXD2856_SonyDemod[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     sony_tuner_terr_cable_t  CXD2856_SonyTunerTerrestrialCable[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     sony_tuner_sat_t         CXD2856_SonyTunerSatellite[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     sony_helene_t            CXD2856_SonyHelene[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];
     sony_i2c_t CXD2856_SonyI2c[JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER];     
};
//***************************************************************************
//************************ Jacky Han Insertion End **************************
//***************************************************************************


typedef struct _GPIO_STRUCT {
    Dword GpioOut;
    Dword GpioEn;
    Dword GpioOn;
} GPIO_STRUCT, *PGPIO_STRUCT;

typedef struct Endeavour_DEVICE_CONTEXT{
    FILTER_CONTEXT_HW fc[2];
    struct usb_device *usb_dev;
    
    Byte DeviceNo;
    Bool bBootCode;
    Bool bEP12Error;
    Bool bEP45Error;
    Bool bDualTs;
    Bool bIrTblDownload;    
    Byte BulkOutData[256];
    u32 WriteLength;
    Bool bSurpriseRemoval;
    Bool bDevNotResp;
    Bool bEnterSuspend;
    Bool bSupportSuspend;
    Bool bSupportSelSuspend;
    u16 regIdx; 
    Byte eepromIdx;
    u16 UsbMode;
    u16 MaxPacketSize;
    u32 MaxIrpSize;
    u32 TsFrames;
    u32 TsFrameSize;
    u32 TsFrameSizeDw;
    u32 TsPacketCount;
    Bool bSelectiveSuspend;
    u32 ulActiveFilter;
    Architecture architecture;
    StreamType StreamType;
    Bool bDCAPIP;
    Bool bSwapFilter;
  
    Bool bTunerPowerOff;
    Byte UsbCtrlTimeOut;
	
	//Endeavour endeavour;
	Byte it9300_Number;
	Byte rx_number;
	Endeavour it9300;
	DefaultDemodulator it9133[4][5];
	Byte Si2168B_standardName[4][5];
	Dword *Si2168B_FE[5];
	Byte is_rx_init[4][5];
	Byte chip_Type[4][5];
	MXL_EAGLE_DEV_CONTEXT_T	MxL_EAGLE_Devices[5];
	sony_cxd_CXD285x_driver_instance_t CXD285x_driver[5];   
	
	Word rx_device_id[32];                      // Jacky Han Modified // original value is 8
	
    Bool ForceWrite;
    Byte chip_version;
    Bool bProprietaryIr;
    Byte bIrType;
	Byte *temp_read_buffer;
	Byte *temp_write_buffer;
	Byte *temp_urb_buffer;
	bool power_state[2];
	Dword parsebufflen;
	Dword device_id;
	Dword board_id;
	bool disconnect;
	
	char filter_name[16][40];
	bool map_enalbe;
	Byte hihg_byte;
	Byte low_byte;	
	
	atomic_t filter_count;
	bool auto_regist;
	
	struct mutex dev_mutex;
	
	int if_degug;

	struct DIB9090_DEVICE dib9090;

	struct JACKY_FRONTEND_DEVICE JackyFrontend;           // Jacky Han Added

} Device_Context;

/* Structure for urb context */
typedef struct _RX_URB_CONTEXT{
	//struct file *file;
	void *br_chip;
	Byte index;
} Rx_Urb_Context;

/* Structure to hold all of our device specific stuff */
typedef struct _CHIP{
	Byte chip_index;
	
	bool urb_streaming;
	struct urb *urbs[BR_URB_COUNT];	
	Rx_Urb_Context urb_context[BR_URB_COUNT];	
	Byte urbstatus[BR_URB_COUNT];
	Byte *pRingBuffer;
	Byte *br_pRingBuffer[BR_URB_COUNT];
	Dword dwTolBufferSize;
	Byte urb_count;
	Dword CurrBuffPointAddr;
	Dword ReadBuffPointAddr;
	Dword dwReadRemaingBufferSize;			
	
	spinlock_t read_buf_lock;
	void *dev;
	
	///for WriteRingBuffer	
	Dword TxWriteBuffPointAddr;
	Dword dwTxRemaingBufferSize;
	
	int handle_startcapture[4];		//chip open flag
	bool if_chip_start_capture;		//chip open flag	
	Byte chip_open_count;
	Byte acq_transfer_count;	//acq_transfer_count
	
	bool if_search_syncbyte;
	
	wait_queue_head_t read_wait_queue;
	bool g_if_wait_read;
	int read_wait_len;
} Chip;

typedef struct _IT930x_DEVICE{
	struct usb_interface *interface;
	struct kref kref;
	struct file *file[16];
	Device_Context *DC;
    struct notifier_block reboot_notifier;  /* default mode before reboot */
	Chip *chip[16];
	Chip *br_chip;
	Byte minor[16];
	bool Tuner_inited[4];
	Byte syncByte;
	bool SetNULLPacket;
	
	///for returnchannel
	unsigned long DealWith_ReturnChannelPacket;
	
	// Device Init work queue thread
	struct work_struct init_work;
	//struct sk_buff_head init_qhead;
	spinlock_t init_lock;
	unsigned long init_state;
	wait_queue_head_t init_wait;
	struct workqueue_struct *init_wq;
} IT930x_Device;

typedef struct _FILE_INFORMATION{
	IT930x_Device *dev;
	Byte chip_index;
	bool if_file_start_capture; //ap open flag
} File_Information;


extern int it930x_device_count;
extern int dvb_usb_it930x_debug;
extern Dword IsLock(Device_Context *DC, Bool* locked, int br_idx, int ts_idx);
//extern void UsbBus(Device_Context *DC);
//extern Dword Device_init(Device_Context *DC, Bool bBoot);
//extern Dword DL_ApCtrl(Device_Context *DC, Byte chip_index, Bool bOn);
//extern Dword DL_CheckTunerInited(Device_Context *DC, Byte chip_index, bool *Tuner_inited);
extern Dword DL_GetEEPROMConfig(Device_Context *DC);
extern Dword DL_device_communication_test(Device_Context *DC);
extern Dword DL_IT930x_device_init(Device_Context *DC);
extern Dword DL_IT930x_device_deinit(Device_Context *DC);



extern Dword DL_DemodIOCTLFun(void* demodulator, Dword IOCTLCode, unsigned long pIOBuffer, Byte chip_index);

extern Dword DL_Demodulator_acquireChannel(Device_Context *DC, AcquireChannelRequest* request, int br_idx, int ts_idx);
extern Dword DL_Demodulator_isLocked (Device_Context *DC, Bool* locked, int br_idx, int ts_idx);
extern Dword DL_Demodulator_getStatistic(Device_Context *DC, Statistic* statistic, int br_idx, int ts_idx);
extern Dword DL_Demodulator_getChannelStatistic (Device_Context *DC, Byte chip, ChannelStatistic* channelStatistic, int br_idx, int ts_idx);
extern Dword DL_Demodulator_writeRegisters(Device_Context *DC, Processor processor, Dword registerAddress, Byte bufferLength, Byte* buffer, int br_idx, int ts_idx);
extern Dword DL_Demodulator_readRegisters(Device_Context *DC, Processor processor, Dword registerAddress, Byte bufferLength, Byte* buffer, int br_idx, int ts_idx);
extern Dword DL_Demodulator_resetPidFilter(Device_Context *DC, int br_idx, int ts_idx);
extern Dword DL_Demodulator_controlPidFilter(Device_Context *DC, Byte control, int br_idx, int ts_idx);
extern Dword DL_Demodulator_addPidToFilter(Device_Context *DC, Byte index, Word value, int br_idx, int ts_idx);
extern Dword DL_Demodulator_removePidFromFilter(Device_Context *DC, Byte index, Word value, int br_idx, int ts_idx);
extern Dword DL_Demodulator_getBoardInputPower(Device_Context *DC, Byte* boardInputPower, int br_idx, int ts_idx);

extern Dword DL_Demodulator_getChannelStrength(Device_Context *DC, unsigned char* pStrength, int br_idx, int ts_idx);                                                     // Jacky Han Added
extern Dword DL_Demodulator_getChannelQuality(Device_Context *DC, unsigned char* pQuality, int br_idx, int ts_idx);                                                       // Jacky Han Added
extern Dword DL_Demodulator_getChannelErrorRate(Device_Context *DC, unsigned char* pErrorRate, int br_idx, int ts_idx);                                                   // Jacky Han Added

extern Dword DRV_IT913x_Initialize(Device_Context *DC, Byte br_idx, Byte tsSrcIdx);

extern Dword DRV_Si2168B_Initialize(Device_Context *DC, Byte br_idx, Byte tsSrcIdx);
extern Dword DRV_Si2168B_acquireChannel(Device_Context *DC, Word bandwidth, Dword frequency, int br_idx, int ts_idx);
extern Dword DRV_Si2168B_getStatistic(Device_Context *DC, Statistic *statistic, int br_idx, int ts_idx);
extern Dword DRV_Si2168B_getChannelStatistic (Device_Context *DC, ChannelStatistic* channelStatistic, int br_idx, int ts_idx);
extern Dword DRV_Si2168B_isLocked(Device_Context *DC, Bool *locked, int br_idx, int ts_idx);

//***************************************************************************
//*********************** Jacky Han Insertion Start *************************
//***************************************************************************
extern Dword DRV_MyFrontend_Initialize(Device_Context *DC, Byte tsSrcIdx); 
extern Dword DRV_MyFrontend_DeInitialize(Device_Context *DC, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_Open(Device_Context *DC, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_Close(Device_Context *DC, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_isOpen(Device_Context *DC, Bool* pOpenFlag, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_Reset(Device_Context *DC, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_SetDemodulationMode(Device_Context *DC, DTV_DEMODULATION_MODE mode, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_SetSignalModulationType(Device_Context *DC, DTV_MODULATION_CONSTELLATION Type, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_acquireChannel(Device_Context *DC, Word bandwidth, Dword frequency,  Byte tsSrcIdx);
extern Dword DRV_MyFrontend_getStatistic(Device_Context *DC, Statistic *statistic,  Byte tsSrcIdx);
extern Dword DRV_MyFrontend_getChannelStatistic(Device_Context *DC, ChannelStatistic* channelStatistic,  Byte tsSrcIdx);
extern Dword DRV_MyFrontend_isLocked(Device_Context *DC, Bool* locked,  Byte tsSrcIdx);
extern Dword DRV_MyFrontend_GetCNR(Device_Context *DC, char* pCNR, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_GetCNRaw(Device_Context *DC, int* pCNRaw, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_getSNR(Device_Context *DC, Constellation* pConstellation, char* pSnr, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_getSignalStrengthDbm(Device_Context *DC, long* pStrengthDbm, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_getBER(Device_Context *DC, long* pBer, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_getChannelStrength(Device_Context *DC, unsigned char* pStrength, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_getChannelQuality(Device_Context *DC, unsigned char* pQuality, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_getChannelErrorRate(Device_Context *DC, unsigned char* pErrorRate, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_SetTSID(Device_Context *DC, unsigned short TSID, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_SetOneSegMode(Device_Context *DC, Bool OneSegModeEnableFlag, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_SetLNBPower(Device_Context *DC, Bool PowerFlag, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_GetLNBPower(Device_Context *DC, Bool* pPowerFlag, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_GetLNBPowerShortCircuitDetect(Device_Context *DC, Bool* pShortCircuitFlag, Byte tsSrcIdx);
extern Dword DRV_MyFrontend_EnforceLNBPowerOff(Device_Context *DC);

extern Dword DL_MyFrontend_getStatistic(Device_Context *DC, Statistic *statistic, int br_idx, int ts_idx);
extern Dword DL_MyFrontend_getChannelStatistic(Device_Context *DC, ChannelStatistic* channelStatistic, int br_idx, int ts_idx);
extern Dword DL_MyFrontend_isLocked(Device_Context *DC, Bool *locked, int br_idx, int ts_idx);
extern Dword DL_MyFrontend_getBER(Device_Context *DC, long* pBer, int br_idx, int ts_idx);
extern Dword DL_MyFrontend_getChannelStrength(Device_Context *DC, unsigned char* pStrength, int br_idx, int ts_idx);
extern Dword DL_MyFrontend_getChannelQuality(Device_Context *DC, unsigned char* pQuality, int br_idx, int ts_idx);
extern Dword DL_MyFrontend_getChannelErrorRate(Device_Context *DC, unsigned char* pErrorRate, int br_idx, int ts_idx);
//***************************************************************************
//************************ Jacky Han Insertion End **************************
//***************************************************************************

extern Dword DL_ResetRxPort(Device_Context *DC, int is_open);

extern Dword DL_Dib9090_getDiversityDetailData(Device_Context *DC, DibcomDiversityData* data, Byte byChipType);

extern Dword DRV_Dib9090_Initialize(Device_Context *DC, Byte byChipType);
extern Dword DRV_Dib9090_acquireChannel(Device_Context *DC, Word bandwidth, Dword frequency, Byte byChipType);
extern Dword DRV_Dib9090_getStatistic(Device_Context *DC, Statistic *statistic, Byte byChipType, Bool bIsRescan);
extern Dword DRV_Dib9090_getChannelStatistic (Device_Context *DC, ChannelStatistic* channelStatistic, int br_idx, int ts_idx);
extern Dword DRV_Dib9090_isLocked(Device_Context *DC, Bool *locked, Byte byChipType);
extern Dword DRV_Dib9090_DeInitialize(Device_Context *DC, Byte byChipType);
extern Dword DRV_Dib9090_getDiversityDetailData(Device_Context *DC, DibcomDiversityData* data, Byte byChipType);

extern Dword DRV_ResetRxPort(Device_Context *DC, int is_open);

extern Dword DRV_MXL69X_Initialize(Device_Context *DC, Byte br_idx, Byte tsSrcIdx, Byte ChipType);
extern Dword DRV_MXL69X_acquireChannel(Device_Context *DC, AcquireChannelRequest *request, Byte br_idx, Byte tsSrcIdx);
extern Dword DRV_MXL69X_getStatistic(Device_Context *DC, Statistic *statistic, Byte br_idx, Byte tsSrcIdx);
extern Dword DRV_MXL69X_getChannelStatistic (Device_Context *DC, ChannelStatistic* channelStatistic, int br_idx, int tsSrcIdx);
extern Dword DRV_MXL69X_isLocked(Device_Context *DC, Bool *locked, Byte br_idx, Byte tsSrcIdx);
extern Dword DRV_MXL69X_DeInitialize(Device_Context *DC, Byte br_idx, Byte tsSrcIdx);

extern Dword DRV_CXD285X_Initialize(Device_Context *DC, Byte br_idx, Byte tsSrcIdx, Byte ChipType);
extern Dword DRV_CXD285X_acquireChannel(Device_Context *DC, AcquireChannelRequest *request, Byte br_idx, Byte tsSrcIdx);
extern Dword DRV_CXD285X_getStatistic(Device_Context *DC, Statistic *statistic, Byte br_idx, Byte tsSrcIdx);
extern Dword DRV_CXD285X_isLocked(Device_Context *DC, Bool *locked, Byte br_idx, Byte tsSrcIdx);
#endif
