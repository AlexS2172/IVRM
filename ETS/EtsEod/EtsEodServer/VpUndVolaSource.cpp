#include "StdAfx.h"
#include "VpUndVolaSource.h"

namespace EODVP
{

/////////////////////////////////////////////////////////////////////////////
//
CVpUndVolaSource::CVpUndVolaSource(void)
{
}

/////////////////////////////////////////////////////////////////////////////
//
CVpUndVolaSource::~CVpUndVolaSource(void)
{
	m_spSymbolVola = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
void CVpUndVolaSource::Init(IVSSymbolVolatilityPtr spSymbolVola)
{
	m_spSymbolVola = spSymbolVola;
}

/////////////////////////////////////////////////////////////////////////////
//
double CVpUndVolaSource::Vola(double dUndPrice, DATE dtExpiry, double dStrike) const
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(m_spSymbolVola == NULL)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Underlying volatility source is not initialized."));
		throw exception( _T("Underlying volatility source is not initialized."));
	}		
	COleDateTime odtExpiry(dtExpiry);
	COleDateTime odtFirstDayMonth(odtExpiry.GetYear(),odtExpiry.GetMonth(),1,0,0,0);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return m_spSymbolVola->GetVolatility(dUndPrice, dStrike, odtFirstDayMonth, 0) /100.;
}

};
