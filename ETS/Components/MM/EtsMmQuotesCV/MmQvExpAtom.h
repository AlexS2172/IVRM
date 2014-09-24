// MmQvExpAtom.h : Declaration of the CMmQvExpAtom
#ifndef __MMQVEXPATOM_H__
#define __MMQVEXPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvStrikeColl.h"

_COM_SMARTPTR_TYPEDEF(IMmQvExpAtom, IID_IMmQvExpAtom);

struct __MmQvExpAtom
{
	DATE					m_dtExpiryMonth;
	DATE					m_dtExpiry;
	DOUBLE					m_dRate;
	DOUBLE					m_dRateCust;
	VARIANT_BOOL			m_bVisible;
	IMmQvStrikeCollPtr		m_spStrike;
	IMmQvStrikeAtomPtr		m_spAtmStrike;
	_bstr_t					m_bstrRootNames;
	DOUBLE					m_dHTBRate;
	DATE					m_dtExpiryOV;
	DATE					m_dtTradingClose;

	__MmQvExpAtom()
		: m_dtExpiryMonth(0.), m_dtExpiry(0.), m_dRate(0.), m_dRateCust(0.), m_bVisible(VARIANT_FALSE),
		  m_dHTBRate(BAD_DOUBLE_VALUE),
                  m_dtExpiryOV(0), m_dtTradingClose(0)
	{
	}
};

// CMmQvExpAtom

class ATL_NO_VTABLE CMmQvExpAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvExpAtom, &CLSID_MmQvExpAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvExpAtom>,
	public IDispatchImpl<IMmQvExpAtom, &IID_IMmQvExpAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvExpAtom
{
	typedef  std::set<_bstr_t>  CRootNamesCollection;
public:
	CMmQvExpAtom()
		:m_pStrike(NULL)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVEXPATOM)


BEGIN_COM_MAP(CMmQvExpAtom)
	COM_INTERFACE_ENTRY(IMmQvExpAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		HRESULT hr = S_OK;
		try
		{
			
			_CHK(CComObject<CMmQvStrikeColl>::CreateInstance(&m_pStrike), _T("Fail to create strikes."));
			m_spStrike.Attach(m_pStrike, TRUE);

			hr = CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p);

		}
		catch(const _com_error& e)
		{
			hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvExpAtom, e.Error());
		}
		return hr;
	}
	
	void FinalRelease() 
	{
		m_spStrike = NULL;
		m_spAtmStrike = NULL;
		m_pUnkMarshaler.Release();

	}
	bool AddRoot(_bstr_t& bsRootName) 
	{
		CRootNamesCollection::iterator itrFind = m_Roots.find(bsRootName);
		if(itrFind != m_Roots.end())
		{
			if(m_Roots.empty())
				m_bstrRootNames = bsRootName;
			else
				m_bstrRootNames += _bstr_t(L",") + bsRootName;

			m_Roots.insert(bsRootName);
			return true;
		}
		return false;
	}
public:
	CComObject<CMmQvStrikeColl>* m_pStrike;
private:
	CRootNamesCollection  m_Roots;
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryMonth, m_dtExpiryMonth)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rate, m_dRate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, HTBRate, m_dHTBRate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RateCust, m_dRateCust)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Visible, m_bVisible)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvStrikeColl*, Strike, m_spStrike)
	IMPLEMENT_OBJECT_PROPERTY(IMmQvStrikeAtom*, NearAtmStrike, m_spAtmStrike)
	IMPLEMENT_BSTRT_PROPERTY(RootNames, m_bstrRootNames)
	STDMETHOD(get_NearAtmVola)(DOUBLE* pVal);
	STDMETHOD(FindAtmStrike)(DOUBLE UnderlyingSpot);

        IMPLEMENT_SIMPLE_PROPERTY(DATE,		ExpiryOV,		m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,		TradingClose,	m_dtTradingClose)

};

OBJECT_ENTRY_AUTO(__uuidof(MmQvExpAtom), CMmQvExpAtom)

#endif //__MMQVEXPATOM_H__
