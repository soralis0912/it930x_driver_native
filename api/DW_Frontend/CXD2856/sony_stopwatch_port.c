/*------------------------------------------------------------------------------
  Copyright 2016 Sony Semiconductor Solutions Corporation

  Last Updated    : 2016/06/24
  Modification ID : 3b74e280b7ad8ce430b6a9419ac53e8f2e3737f9
------------------------------------------------------------------------------*/

#include "sony_common.h"


unsigned int sony_system_time(void)        
{
	static struct timeval curr_time;
	unsigned int ulRet;

	do_gettimeofday(&curr_time);
	ulRet = curr_time.tv_sec*1000 + curr_time.tv_usec/1000;

    return ulRet;
}

sony_result_t sony_stopwatch_start (sony_stopwatch_t * pStopwatch)
{
    if(pStopwatch)
	{
       pStopwatch->startTime = sony_system_time();

	   SONY_TRACE_RETURN(SONY_RESULT_OK);
	}
	else
	{
	   SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
	}
}

sony_result_t sony_stopwatch_sleep (sony_stopwatch_t * pStopwatch, uint32_t ms)
{
    if(pStopwatch)
	{
	   SONY_ARG_UNUSED(*pStopwatch);
       SONY_SLEEP(ms);

       SONY_TRACE_RETURN(SONY_RESULT_OK);
	}
	else
	{
	   SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
	}
}

sony_result_t sony_stopwatch_elapsed (sony_stopwatch_t * pStopwatch, uint32_t* pElapsed)
{
	uint32_t currentTime;

    if(pStopwatch != NULL && pElapsed != NULL)
	{
	   currentTime = sony_system_time();
       *pElapsed = (currentTime - pStopwatch->startTime);         // Jacky Han Modified // *pElapsed = ((currentTime - pStopwatch->startTime) / 1563); 

       SONY_TRACE_RETURN(SONY_RESULT_OK);
	}
	else
	{
	   SONY_TRACE_RETURN(SONY_RESULT_ERROR_ARG);
	}
}
