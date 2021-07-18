#include <linux/time.h>

#include "it930x-core.h"


Dword DRV_IT913x_Initialize (Device_Context *DC, Byte br_idx, Byte tsSrcIdx) {
	Dword error = Error_NO_ERROR;
    
    error = Demodulator_initialize ((Demodulator*) DC->it9300.tsSource[br_idx][tsSrcIdx].htsDev, (StreamType) DC->it9300.tsSource[br_idx][tsSrcIdx].tsType+4);
	    
    if (error) goto exit;
    
	deb_data("%d, %d  Ts Source initialize ok! \n",br_idx, tsSrcIdx);
	deb_data(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n");
	
exit:
	if (error) 
	{
		deb_data("%d, %d  Ts Source initialize error = 0x%08lx \n",br_idx, tsSrcIdx, error);
		deb_data(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n");
	}
	
	//reg_value = 1; // = 0: for debug
	//FDI : omega ofdm 0xa5 close.
	//Demodulator_writeRegisters((Demodulator *) DC->it9300.tsSource[br_idx][ts_idx].htsDev, 8, 0xa5, 1, &reg_value);
	
	return error;
}

Dword DL_Demodulator_acquireChannel (Device_Context *DC, AcquireChannelRequest *request, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	DTV_DEMODULATION_MODE mode;              // Jacky Han Added
	DTV_MODULATION_CONSTELLATION Type;       // Jacky Han Added
	
	mutex_lock(&DC->dev_mutex);
	
	switch (DC->chip_Type[br_idx][ts_idx])
	{
		case EEPROM_IT913X:
			error = Demodulator_acquireChannel ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, request->bandwidth, request->frequency);
			break;

		case EEPROM_Si2168B:
			error = DRV_Si2168B_acquireChannel (DC, request->bandwidth, request->frequency, br_idx, ts_idx);
			break;

		case EEPROM_Dibcom9090:
			error = DRV_Dib9090_acquireChannel(DC, request->bandwidth, request->frequency, EEPROM_Dibcom9090);
			break;
			
		case EEPROM_Dibcom9090_4chan:
			error = DRV_Dib9090_acquireChannel(DC, request->bandwidth, request->frequency, EEPROM_Dibcom9090_4chan);
			break;

		case EEPROM_MXL691:
		case EEPROM_MXL691_DUALP:
		case EEPROM_MXL692:
		case EEPROM_MXL248:
			error = DRV_MXL69X_acquireChannel (DC, request, br_idx, ts_idx);
			break;
		case EEPROM_CXD285X:
			error = DRV_CXD285X_acquireChannel(DC, request, br_idx, ts_idx);
			break;
			
	    //****************************************************
        //************* Jacky Han Insertion Start ************
	    //****************************************************
		case EEPROM_PXW3U4 :
		case EEPROM_PXW3PE4 :
		case EEPROM_PXW3PE5 :
		case EEPROM_PXQ3U4 :
	    case EEPROM_PXQ3PE4 :
	    case EEPROM_PXQ3PE5 :
	    case EEPROM_PXMLT5PE :
		case EEPROM_PXMLT5U :
		case EEPROM_PXMLT8PE3 :
		case EEPROM_PXMLT8PE5 :
		case EEPROM_PXMLT8UT :
		case EEPROM_PXMLT8UTS :

			Type = IGNORE_CONSTELLATION;

			if(request->mode == 0)
               mode = ATSC_MODE;
			else
			if(request->mode == 1)
               mode = DVBC_MODE;
			else
			if(request->mode == 2)
               mode = CLEAR_QAM_MODE;
			else
			if(request->mode == 3)
               mode = DVBT_MODE;
			else
			if(request->mode == 4)
               mode = ISDBT_MODE;
			else
			if(request->mode == 5)
               mode = ISDBS_MODE;
			else
			if(request->mode == 6)
               mode = DVBT2_MODE;
			else
			if(request->mode == 7)
               mode = DTMB_MODE;
			else
			if(request->mode == 8)
			{
               mode = CLEAR_QAM_MODE;
			   Type = CONSTELLATION_64_QAM;
			}
			else
			if(request->mode == 9)
			{
               mode = CLEAR_QAM_MODE;
			   Type = CONSTELLATION_256_QAM;
			}
			else
			   mode = UNKNOWN_DTV_SYSTEM;

			error = DRV_MyFrontend_SetDemodulationMode(DC, mode, ts_idx);
			error = DRV_MyFrontend_SetSignalModulationType(DC, Type, ts_idx);
            error = DRV_MyFrontend_acquireChannel(DC,request->bandwidth,request->frequency,ts_idx);   
			break;                                                                  
	    //****************************************************
        //************** Jacky Han Insertion End *************
	    //****************************************************
			
		default:
			deb_data("Unknown chip type [%d]\n", DC->chip_Type[br_idx][ts_idx]);
			break;
	}
				
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_getStatistic (Device_Context *DC, Statistic* statistic, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	

//printk("(Jacky)(DemodIOCTLFun)(DL_Demodulator_getStatistic)\n");
//printk("(Jacky)(DemodIOCTLFun)(DL_Demodulator_getStatistic) DC->chip_Type[%d][%d] : %d\n",br_idx,ts_idx,DC->chip_Type[br_idx][ts_idx]);


	mutex_lock(&DC->dev_mutex);
		switch (DC->chip_Type[br_idx][ts_idx]) {
		case EEPROM_IT913X:
			error = Demodulator_getStatistic ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, statistic);
			break;

		case EEPROM_Si2168B:
			error = DRV_Si2168B_getStatistic (DC, statistic, br_idx, ts_idx);
			break;

		case EEPROM_Dibcom9090:
			error = DRV_Dib9090_getStatistic (DC, statistic, EEPROM_Dibcom9090, False);
			break;
			
		case EEPROM_Dibcom9090_4chan:
			error = DRV_Dib9090_getStatistic (DC, statistic, EEPROM_Dibcom9090_4chan, False);
			break;

		case EEPROM_MXL691:
		case EEPROM_MXL691_DUALP:
		case EEPROM_MXL692:
		case EEPROM_MXL248:
			error = DRV_MXL69X_getStatistic (DC, statistic, br_idx, ts_idx);
			break;
		case EEPROM_CXD285X:
			error = DRV_CXD285X_getStatistic (DC, statistic, br_idx, ts_idx);
			break;
			
	    //****************************************************
        //************* Jacky Han Insertion Start ************
	    //****************************************************
		case EEPROM_PXW3U4 :
		case EEPROM_PXW3PE4 :
		case EEPROM_PXW3PE5 :
		case EEPROM_PXQ3U4 :
	    case EEPROM_PXQ3PE4 :
	    case EEPROM_PXQ3PE5 :
		case EEPROM_PXMLT5PE :
	    case EEPROM_PXMLT5U :
		case EEPROM_PXMLT8PE3 :
		case EEPROM_PXMLT8PE5 :
		case EEPROM_PXMLT8UT :
		case EEPROM_PXMLT8UTS :
            error = DRV_MyFrontend_getStatistic(DC,statistic,ts_idx);  
			break;                                                      
	    //****************************************************
        //************** Jacky Han Insertion End *************
	    //****************************************************
			
		default:
			deb_data("Unknown chip type [%d]\n", DC->chip_Type[br_idx][ts_idx]);
			break;
		}
	
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_getChannelStatistic (Device_Context *DC, Byte chip, ChannelStatistic* channelStatistic, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);

	switch (DC->chip_Type[br_idx][ts_idx]) {
	case EEPROM_IT913X:
		error = Demodulator_getChannelStatistic ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, chip, channelStatistic);
		break;

	case EEPROM_Si2168B:
		error = DRV_Si2168B_getChannelStatistic (DC, channelStatistic, br_idx, ts_idx);
		break;

	case EEPROM_Dibcom9090:
	case EEPROM_Dibcom9090_4chan:
	case EEPROM_MXL691:
	case EEPROM_MXL691_DUALP:
	case EEPROM_MXL692:
	case EEPROM_MXL248:
		error = Error_NO_ERROR;
		break;

	//****************************************************
    //************* Jacky Han Insertion Start ************
	//****************************************************
	case EEPROM_PXW3U4 :
	case EEPROM_PXW3PE4 :
	case EEPROM_PXW3PE5 :
	case EEPROM_PXQ3U4 :
	case EEPROM_PXQ3PE4 :
	case EEPROM_PXQ3PE5 :
	case EEPROM_PXMLT5PE :
	case EEPROM_PXMLT5U :
	case EEPROM_PXMLT8PE3 :
	case EEPROM_PXMLT8PE5 :
	case EEPROM_PXMLT8UT :
	case EEPROM_PXMLT8UTS :
        error = DRV_MyFrontend_getChannelStatistic(DC,channelStatistic,ts_idx);   
		break;                                                                    
	//****************************************************
    //************** Jacky Han Insertion End *************
	//****************************************************

	default:
		deb_data("Unknown chip type [%d]\n", DC->chip_Type[br_idx][ts_idx]);
		break;
	}
		
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_writeRegisters (Device_Context *DC, Processor processor, Dword registerAddress, Byte bufferLength, Byte* buffer, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
		error = Demodulator_writeRegisters((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, processor, registerAddress, bufferLength, buffer);
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_readRegisters(Device_Context *DC, Processor processor, Dword registerAddress, Byte bufferLength, Byte* buffer, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
		error = Demodulator_readRegisters((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, processor, registerAddress, bufferLength, buffer);
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_resetPidFilter(Device_Context *DC, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
		error = Demodulator_resetPidFilter((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev);
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_controlPidFilter(Device_Context *DC, Byte control, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
		error = Demodulator_controlPidFilter((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, control);
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_addPidToFilter(Device_Context *DC, Byte index, Word value, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
		error = Demodulator_addPidToFilter((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, index, value);
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_isTpsLocked(Device_Context *DC, Bool *locked, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
	
	error = Standard_isTpsLocked((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, locked);
	
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_isMpeg2Locked(Device_Context *DC, Bool *locked, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
	
	error = Standard_isMpeg2Locked((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, locked);
	
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword Demodulator_isLocked_Ex(Device_Context *DC, Bool* locked, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;

	Word emptyLoop = 0;
	Word tpsLoop = 0;
	Word mpeg2Loop = 0;
	Byte channels;
	Byte emptyChannel = 1;
	Byte tpsLocked = 0;
    Demodulator* demodulator;
	DefaultDemodulator* demod;
	Bool retry = False;
	struct timeval begin, end;
	unsigned long val;
	
	demodulator = (Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev;
	demod = (DefaultDemodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev;
	
	*locked = False;
	demod->statistic.signalPresented = False;
	demod->statistic.signalLocked = False;
	demod->statistic.signalQuality = 0;
	demod->statistic.signalStrength = 0;

	do_gettimeofday(&begin);
	val = 0;

    channels = 2;
	//while (emptyLoop < 30)
	while (val < 1500) 
	{
		error = DL_Demodulator_readRegisters(DC, Processor_OFDM, empty_channel_status, 1, &channels, br_idx, ts_idx);
		if (error) goto exit;
		if (channels == 1) 
		{
			emptyChannel = 0;
			break;
		}
		User_delay (demodulator, 50);
		emptyLoop++;
		
		do_gettimeofday(&end);

		// Get milliseconds 
		val = (end.tv_sec - begin.tv_sec) * 1000;
		val += ((end.tv_usec - begin.tv_usec) / 1000);
	}
	
	//deb_data("empty_channel_status %d, %d\n", channels, emptyLoop);
	
	if (channels == 2) 
		goto exit;

	do_gettimeofday(&begin);
	val = 0;

	//while (tpsLoop < 30) 
	while (val < 1800)
	{
		error = DL_Demodulator_isTpsLocked (DC, &demod->statistic.signalPresented, br_idx, ts_idx);
		if (error) goto exit;
		if (demod->statistic.signalPresented == True) {
			tpsLocked = 1;
			break;
		}
		if (tpsLocked == 1) break;
		User_delay (demodulator, 60);
		tpsLoop++;
		
		do_gettimeofday(&end);

		// Get milliseconds 
		val = (end.tv_sec - begin.tv_sec) * 1000;
		val += ((end.tv_usec - begin.tv_usec) / 1000);
	}
	
	//deb_data("isTpsLocked %d, %d\n", tpsLocked, tpsLoop);
	
	if (tpsLocked == 0) 
		goto exit;

	do_gettimeofday(&begin);
	val = 0;

	mpeg2Loop = 0;
#ifdef	PATCH_FOR_NX
	while (mpeg2Loop < 70) 
#else
	while (val < 7000)
#endif
	{
		error = DL_Demodulator_isMpeg2Locked (DC, &demod->statistic.signalLocked, br_idx, ts_idx);
		if (error) goto exit;
		if (demod->statistic.signalLocked == True) {
			demod->statistic.signalQuality = 80;
			demod->statistic.signalStrength = 80;
			*locked = True;
			break;
		}
		User_delay (demodulator, 100);
		mpeg2Loop++;
		
		do_gettimeofday(&end);

		// Get milliseconds 
		val = (end.tv_sec - begin.tv_sec) * 1000;
		val += ((end.tv_usec - begin.tv_usec) / 1000);
	}

	//deb_data("isMpeg2Locked %d, %d\n", tpsLocked, *locked);
	
	demod->statistic.signalQuality = 0;
	demod->statistic.signalStrength = 20;
  
exit:

	return (error);
}


Dword DL_Demodulator_isLocked(Device_Context *DC, Bool* locked, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	switch (DC->chip_Type[br_idx][ts_idx])
	{
		case EEPROM_IT913X:
			error = Demodulator_isLocked_Ex(DC, locked, br_idx, ts_idx);
			break;

		case EEPROM_Si2168B:
			mutex_lock(&DC->dev_mutex);
	
			error = DRV_Si2168B_isLocked(DC, locked, br_idx, ts_idx);
			
			mutex_unlock(&DC->dev_mutex);
	
			break;
		
		case EEPROM_Dibcom9090:
			mutex_lock(&DC->dev_mutex);
	
			error = DRV_Dib9090_isLocked(DC, locked, EEPROM_Dibcom9090);
			
			mutex_unlock(&DC->dev_mutex);
	
			break;
			
		case EEPROM_Dibcom9090_4chan:
			mutex_lock(&DC->dev_mutex);
	
			error = DRV_Dib9090_isLocked(DC, locked, EEPROM_Dibcom9090_4chan);
			
			mutex_unlock(&DC->dev_mutex);
	
			break;		

		case EEPROM_MXL691:
		case EEPROM_MXL691_DUALP:
		case EEPROM_MXL692:
		case EEPROM_MXL248:
			mutex_lock(&DC->dev_mutex);
	
			error = DRV_MXL69X_isLocked(DC, locked, br_idx, ts_idx);
			
			mutex_unlock(&DC->dev_mutex);
			break;
		case EEPROM_CXD285X:
			mutex_lock(&DC->dev_mutex);
	
			error = DRV_CXD285X_isLocked(DC, locked, br_idx, ts_idx);
			
			mutex_unlock(&DC->dev_mutex);
			break;	

		//****************************************************
        //************* Jacky Han Insertion Start ************
	    //****************************************************
	    case EEPROM_PXW3U4 :
	    case EEPROM_PXW3PE4 :
	    case EEPROM_PXW3PE5 :
	    case EEPROM_PXQ3U4 :
	    case EEPROM_PXQ3PE4 :
	    case EEPROM_PXQ3PE5 :
		case EEPROM_PXMLT5PE :
		case EEPROM_PXMLT5U :
		case EEPROM_PXMLT8PE3 :
		case EEPROM_PXMLT8PE5 :
		case EEPROM_PXMLT8UT :
		case EEPROM_PXMLT8UTS :
			mutex_lock(&DC->dev_mutex);                          
           
            error = DRV_MyFrontend_isLocked(DC,locked,ts_idx);    

			mutex_unlock(&DC->dev_mutex);                         
			break;	
	    //****************************************************
        //************** Jacky Han Insertion End *************
	    //****************************************************

		default:
			deb_data("Unknown chip type [%d]\n", DC->chip_Type[br_idx][ts_idx]);
			break;
	}
	
	return error;
}

Dword DL_Demodulator_removePidFromFilter(Device_Context *DC, Byte index, Word value, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
		error = Demodulator_removePidFromFilter((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, index, value);
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

Dword DL_Demodulator_getBoardInputPower(Device_Context *DC, Byte* boardInputPower, int br_idx, int ts_idx)
{
	Dword error = Error_NO_ERROR;
	
	mutex_lock(&DC->dev_mutex);
		error = Demodulator_getBoardInputPower((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, boardInputPower);
	mutex_unlock(&DC->dev_mutex);
	
	return error;
}

