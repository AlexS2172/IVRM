#include "StdAfx.h"
#include "MsRate.h"

namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
//
void CMsRate::AddRate(long nDTE, double dShortRate, double dLongRate, double dHTBRate)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	RATE aRate;
	
	aRate.nDTE = nDTE;

	aRate.dRate = dShortRate;
	m_vecShort.push_back(aRate);

	aRate.dRate = dLongRate;
	m_vecLong.push_back(aRate);

	aRate.dRate = (dShortRate + dLongRate) / 2.;
	m_vecNeutral.push_back(aRate);

	aRate.dRate = dHTBRate;
	m_vecHTB.push_back(aRate);

	aRate.dRate = (dHTBRate + dLongRate) / 2.;
	m_vecNeutralHTB.push_back(aRate);
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CMsRate::ClearRates()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	m_vecShort.clear();
	m_vecLong.clear();
	m_vecNeutral.clear();
	m_vecHTB.clear();
	m_vecNeutralHTB.clear();
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
double CMsRate::_InterpolateRate(long nDTE, MsRateVec& vecRate)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	long nCount = static_cast<long>(vecRate.size());
	if(nCount > 0L)
		return InterpolateRates(nCount, &vecRate.front(), nDTE);
	else
		return 0.;
}

};