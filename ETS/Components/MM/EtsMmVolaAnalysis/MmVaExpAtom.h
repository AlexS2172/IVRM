// MmVaExpAtom.h : Declaration of the CMmVaExpAtom
#ifndef __MMVAEXPATOM_H__
#define __MMVAEXPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaStrikeColl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

_COM_SMARTPTR_TYPEDEF(IMmVaExpAtom, IID_IMmVaExpAtom);

struct __MmVaExpAtom
{
	//DATE					m_dtExpiryMonth;
	DATE					m_dtExpiry;
	DOUBLE					m_dRate;
	IMmVaStrikeCollPtr		m_spStrike;
	IMmVaStrikeAtomPtr		m_spAtmStrike;
	DOUBLE					m_dHTBRate;

	DATE					m_dtExpiryOV;
	DATE					m_dtTradingClose;
	DATE					m_dtLocalExpiryOV;

	__MmVaExpAtom()
		: /*m_dtExpiryMonth(0.), */m_dtExpiry(0.), m_dRate(0.),
		  m_dHTBRate(BAD_DOUBLE_VALUE),
		  m_dtExpiryOV(0), m_dtTradingClose(0)
	{
	}
};

// CMmVaExpAtom

class ATL_NO_VTABLE CMmVaExpAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaExpAtom, &CLSID_MmVaExpAtom>,
	public ISupportErrorInfoImpl<&IID_IMmVaExpAtom>,
	public IDispatchImpl<IMmVaExpAtom, &IID_IMmVaExpAtom, &LIBID_EtsMmVolaAnalysisLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmVaExpAtom
{
public:
	CMmVaExpAtom():m_pStrikeColl(NULL)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAEXPATOM)


BEGIN_COM_MAP(CMmVaExpAtom)
	COM_INTERFACE_ENTRY(IMmVaExpAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			__CHECK_HRESULT(CComObject<CMmVaStrikeColl>::CreateInstance(&m_pStrikeColl), _T("Fail to create strikes."));
			m_spStrike.Attach(m_pStrikeColl, true);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaExpAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spStrike = NULL;
		m_spAtmStrike = NULL;
	}
private:
	CComObject<CMmVaStrikeColl>* m_pStrikeColl;
public:

	//IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryMonth, m_dtExpiryMonth)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rate, m_dRate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, HTBRate, m_dHTBRate)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmVaStrikeColl*, Strike, m_spStrike)
	IMPLEMENT_OBJECT_PROPERTY(IMmVaStrikeAtom*, AtmStrike, m_spAtmStrike)
	STDMETHOD(FindAtmStrike)(DOUBLE UnderlyingSpot);

	//IMPLEMENT_SIMPLE_PROPERTY(DATE,		ExpiryOV,		m_dtExpiryOV)
	STDMETHODIMP put_ExpiryOV(DATE NewVal) {
		ObjectLock lock(this);
		m_dtExpiryOV = NewVal;

		vt_date dtExpiryOV(m_dtExpiryOV);
		typedef boost::date_time::c_local_adjustor<ptime> local_time;
		typedef boost::date_time::local_adjustor<ptime, -5, us_dst> us_eastern;
		ptime	ptExpiryOV( date( dtExpiryOV.get_year(), dtExpiryOV.get_month(), dtExpiryOV.get_day() ), 
							hours( dtExpiryOV.get_hour() ) + minutes( dtExpiryOV.get_minute() ) );
		ptExpiryOV = us_eastern::local_to_utc(ptExpiryOV);
		ptime	ptLocalExpiryOV = local_time::utc_to_local(ptExpiryOV);
		tm		tmLocalExpiryOV = to_tm(ptLocalExpiryOV);
		vt_date dtLocalExpiryOV(tmLocalExpiryOV.tm_year + 1900, tmLocalExpiryOV.tm_mon + 1, tmLocalExpiryOV.tm_mday,
									tmLocalExpiryOV.tm_hour, tmLocalExpiryOV.tm_min);

		m_dtLocalExpiryOV = (DATE)dtLocalExpiryOV;

		return S_OK;
	}
	
	STDMETHODIMP get_ExpiryOV(DATE* pVal) {
		if(!pVal) {
			return E_POINTER;
		}
	
		ObjectLock lock(this);
		*pVal = m_dtExpiryOV;
		return S_OK;
	}

	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DATE, LocalExpiryOV, m_dtLocalExpiryOV)
	
	IMPLEMENT_SIMPLE_PROPERTY(DATE,		TradingClose,	m_dtTradingClose)
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaExpAtom), CMmVaExpAtom)

#endif //__MMVAEXPATOM_H__
