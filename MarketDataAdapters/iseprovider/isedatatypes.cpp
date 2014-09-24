#include "StdAfx.h"

#include "ISESession.h"
#include "ISEConst.h"
#include "ISEData.h"
#include "Trace.h"

bool	CCountryMarket::UpdateInstrumentClasses(CISEData* pAllData) 
{
	bool bRes = true;

	CAutoLock	ClassesLock(&m_InstrumentClasses);
	CAutoLock	UndLock(&m_Underlyings);
	CAutoLock   GroupLock(&m_Instruments);

	series_t	Series;
	Series.country_c = m_uiCountry;
	Series.market_c = m_uiMarket;

	for(set<CUnderlying>::iterator itUnd = m_Underlyings.m_setData.begin(); 
			itUnd != m_Underlyings.m_setData.end(); itUnd++)
	{
		// for each underlying
		CUnderlying& Und = *itUnd;

		Und.m_pInstrumentClass = NULL;

		Series.commodity_n = Und.m_uiCommodity;

		int iTradableClasses = 0;

		for(set<CInstrument>::const_iterator itClass = m_Instruments.m_setData.begin();
			itClass != m_Instruments.m_setData.end(); itClass++)
		{
			// for each instrument group
			const CInstrument& Instrument = *itClass;
			Series.instrument_group_c = Instrument.m_uiGroup;

			// find instrument class for the group of specific underlying
			CInstrumentClass* pClass = m_InstrumentClasses.FindBySeries(Series);
			if(pClass != NULL)
			{
				if(pClass->m_cTraded == ISEBOOL_YES)
				{
					iTradableClasses++;
					Und.m_pInstrumentClass = pClass;
				}
			}
			else
			{
				 /* missing this instrument class
				This is OK, if say the underlying was purely defined by the ISE
				so that the broadcasts of prices for a component of a synthetic
				will occur e.g. at least one component of a synthetic is an underlying
				which the ISE does NOT trade options on, but at least one
				leg is/was traded by the ISE */
			}
		}

		if((iTradableClasses & 1) == 1)
		{
			pAllData->IseTrace(enWarning, "[%x] Underlying '%s' has odd # of tradable classes = %d.",
				pAllData->m_pSession->m_hSession, 
				Und.m_sSymbol.c_str(), 
				iTradableClasses);

			bRes = false;
		}
	}

	return bRes;
};

bool	CCountryMarket::UpdateMarketStructure(CISEData* pAllData)
{
	CAutoLock	UndLock(&m_Underlyings);
	CAutoLock	SeriesLock(&m_Series);

	bool bRes = true;

	// clear all series vectors in underlyings base
	for(set<CUnderlying>::iterator itUnd = m_Underlyings.m_setData.begin();
		itUnd != m_Underlyings.m_setData.end(); itUnd++)
	{
		CUnderlying& Und = *itUnd;
		Und.m_Series.clear();
	}

	// loop for each series
	for(CSeriesDatabase::itSeriesT itSeries = m_Series.m_setData.begin();
			itSeries != m_Series.m_setData.end(); itSeries++)
	{
		CSeries& Series = *itSeries;

		// Find underlying for the series by commodity
		CUnderlying* pUnderlying = m_Underlyings.FindByCommodity(Series.m_Series.commodity_n);
		if(pUnderlying)
		{
			Series.m_pUnderlying = pUnderlying;
			pUnderlying->m_Series.push_back(itSeries);
		}
		else
		{
			pAllData->IseTrace(enWarning, "[%x] Underlying not found for series : %s", 
				pAllData->m_pSession->m_hSession, Series.m_sSymbol.c_str());
			Series.m_pUnderlying = NULL;
			bRes = false;
		}
	}

	return bRes;
}

bool CCountryMarket::UpdateDefaultBin(CISEData* pAllData)
{
	CAutoLock BinsLock(&m_Bins);

	m_uiDefaultBin = 0;

	for(set<CBin>::const_iterator it = m_Bins.m_setData.begin(); it != m_Bins.m_setData.end(); it++)
	{
		const CBin& Bin = *it;

		CAutoLock UserLock(&(pAllData->m_pSession->m_User));

		if(Bin.m_sPMM == pAllData->m_pSession->m_User.m_sCustomerID)
		{
			pAllData->IseTrace(enInfo, "You are PMM for bin %u.", (int)Bin.m_uiID);
			m_uiDefaultBin = Bin.m_uiID;
			m_TraderRoleInDefaultBin = enTrPMM;
			return true;
		}

		for(vector<string>::const_iterator it2 = Bin.m_vecCMMs.begin(); 
			it2 != Bin.m_vecCMMs.end(); it2++)
		{
			if(*it2 == pAllData->m_pSession->m_User.m_sCustomerID)
			{
				pAllData->IseTrace(enInfo, "You are CMM for bin %u.", (int)Bin.m_uiID);
				m_uiDefaultBin = Bin.m_uiID;
				m_TraderRoleInDefaultBin = enTrCMM;
				return true;
			}
		}
	}

	m_TraderRoleInDefaultBin = enTrEAM;
	return false;
}