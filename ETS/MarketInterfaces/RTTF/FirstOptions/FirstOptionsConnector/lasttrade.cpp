#include "stdafx.h"

#include <time.h>
#include "LastTrade.h"
#include "crs.h"
#include "trace.h"


CLastTrade::CLastTrade()
{
}

CLastTrade::~CLastTrade()
{
}

bool CLastTrade::ResetTradeTime()
{
	time_t now = time(NULL);
	struct tm* pDttm = localtime(&now);
	FOC_DATETIME dttm;

	memset(&dttm, 0, sizeof(dttm));
	dttm.day = pDttm->tm_mday;
	dttm.month = pDttm->tm_mon;
	dttm.year = pDttm->tm_year + 1900;

	DWORD dwErr;
	if((dwErr = theApp.m_Publisher.SetLastTradeTime(dttm)) != 0)
	{
		TraceError(dwErr, "Failed to set last trade time.");
		return false;
	}

	return true;
}

bool CLastTrade::GetDate(FOC_DATETIME& dttm) const
{
	if(theApp.m_Publisher.GetLastTradeTime(dttm) != 0)
		return false;

	return true;
}

bool CLastTrade::Init()
{
	// Assuming that we were already launched today
	m_bIsFirstStartOfToday = false;
	FOC_DATETIME dttm;
	
	// if no info about last trade time then store time of today 
	if(theApp.m_Publisher.GetLastTradeTime(dttm) != 0)
	{
		m_bIsFirstStartOfToday = true;

		return ResetTradeTime();
	}


	time_t now = time(NULL);
	struct tm* pDttm = localtime(&now);

	// if date is not today then

	if (dttm.year < pDttm->tm_year + 1900
		|| (dttm.year == pDttm->tm_year + 1900
			&& dttm.month < pDttm->tm_mon)
		|| (dttm.year == pDttm->tm_year + 1900
			&& dttm.month == pDttm->tm_mon
			&& dttm.day < pDttm->tm_mday))
	{
			// This is not a restart if the date is old
			m_bIsFirstStartOfToday = true;

			// If the date is old, re-initialize the last date
			return ResetTradeTime();

	}

	return true;
}

bool CLastTrade::SetDate(const FOC_DATETIME& tradetime)
{
	FOC_DATETIME dttm;
	if(theApp.m_Publisher.GetLastTradeTime(dttm) != 0)
		return false;

	if (tradetime > dttm)
	{
		DWORD dwErr;
		if((dwErr = theApp.m_Publisher.SetLastTradeTime(tradetime)) != 0)
			Trace("Persist storage error : %d", dwErr);

		// Once the time is updated, this is a restart
		m_bIsFirstStartOfToday = false;
	}

	return TRUE;	
}


