#ifndef __MYDEPENDENCY_H__
#define __MYDEPENDENCY_H__

#include "MyTypedef.h"
#include "RT710_DataDefine.h"
#include "R850_DataDefine.h"
#include "CXD2856_DataDefine.h"

#define JACKY_DRIVER_VERSION_NUMBER                                   0x0100
#define JACKY_FRONTEND_MAX_TUNER_DEVICE_NUMBER                        5

typedef enum
{
    ATSC_MODE =          0,
	CLEAR_QAM_MODE =     1,
    ATSC3_MODE =         2,
	DVBT_MODE =          3,
	DVBH_MODE =          4,
	DVBT2_MODE =         5,
	DVBT2_LITE_MODE =    6,
	DVBC_MODE =          7,
	DVBC2_MODE =         8,
	DVBS_MODE =          9,
	DVBS2_MODE =         10,
	ISDBT_MODE =         11,
	ISDBT_1_SEG_MODE =   12,
	ISDBT_3_SEG_MODE =   13,
	ISDBS_MODE =         14,
	ISDBS3_MODE =        15,
	DTMB_MODE =          16,
	CMMB_MODE =          17,
	UNKNOWN_DTV_SYSTEM = 255 

}DTV_DEMODULATION_MODE;

typedef enum
{
    CONSTELLATION_DQPSK,
    CONSTELLATION_QPSK,
    CONSTELLATION_4_QAM,
    CONSTELLATION_4_QAM_NR,
	CONSTELLATION_16_QAM,
	CONSTELLATION_32_QAM,
	CONSTELLATION_64_QAM,
	CONSTELLATION_128_QAM,
	CONSTELLATION_256_QAM,
	CONSTELLATION_8_VSB,
	CONSTELLATION_16_VSB,
	IGNORE_CONSTELLATION,
    UNKNOWN_CONSTELLATION
	
}DTV_MODULATION_CONSTELLATION;

typedef struct 
{
    unsigned long FrequencyKhz;
	unsigned long BandwidthKhz;
	unsigned short TSID;

}RECPX4PX5PX6_ISDB_FREQUENCY_TABLE;

#endif                 // __MYDEPENDENCY_H__