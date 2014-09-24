// MmQvOptAtom.h : Declaration of the CMmQvOptAtom
#ifndef __MMQVOPTATOM_H_
#define __MMQVOPTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvQuoteColl.h"

_COM_SMARTPTR_TYPEDEF(IMmQvOptAtom, IID_IMmQvOptAtom);

typedef CComRecPtr<MmQvMarketDepthAtom, &LIBID_EtsMmQuotesLib>	__MmQvMarketDepthAtom;

struct __MmQvOptAtom
{
	LONG				m_nID;
	_bstr_t				m_bstrSymbol;
	DATE				m_dtExpiry;
	EtsOptionTypeEnum	m_enOptType;
	DOUBLE				m_dStrike;
	DOUBLE				m_dVola;
	LONG				m_nQty;
	LONG				m_nQtyInShares;
	IMmQvQuoteCollPtr	m_spQuote;
	LONG				m_nRootID;	
	LONG				m_nIsFlex;

	DATE				m_dtExpiryOV;
	DATE				m_dtTradingClose;

	__MmQvOptAtom()
		: m_nID(0L), m_dtExpiry(0.), m_enOptType(enOtPut), m_dStrike(0.),
		m_dVola(0.), m_nQty(BAD_LONG_VALUE), m_nQtyInShares(BAD_LONG_VALUE),m_nRootID(0L), m_nIsFlex(0),
		m_dtExpiryOV(0), m_dtTradingClose(0)
	{
	}

protected:
	IMmQvQuoteAtomPtr	m_spDefQuote;
};

// CMmQvOptAtom

class ATL_NO_VTABLE CMmQvOptAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvOptAtom, &CLSID_MmQvOptAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvOptAtom>,
	public IDispatchImpl<IMmQvOptAtom, &IID_IMmQvOptAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvOptAtom
{
public:
	CMmQvOptAtom()
		:m_pQuote(NULL)
		,m_pDefaultQuote(NULL)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVOPTATOM)


BEGIN_COM_MAP(CMmQvOptAtom)
	COM_INTERFACE_ENTRY(IMmQvOptAtom)
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
			m_pDefaultQuote = NULL;

			_CHK(CComObject<CMmQvQuoteColl>::CreateInstance(&m_pQuote), _T("Fail to create quotes."));
			m_spQuote.Attach(m_pQuote, TRUE);

			_CHK(CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p), _T("Unable to create custom Marshaller"));
		}
		catch(const _com_error& e)
		{
			hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvOptAtom, e.Error());
		}
		return hr;
	}
	
	void FinalRelease() 
	{
		m_spQuote = NULL;
		m_spDefQuote = NULL;
		m_pUnkMarshaler.Release();

	}

private:
	CMmQvQuoteAtom* m_pDefaultQuote;
	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	CComObject<CMmQvQuoteColl>* m_pQuote;

	const CMmQvQuoteAtom* GetDefaultQuote() const {return m_pDefaultQuote;}
	void  SetDefaultQuote(IMmQvQuoteAtomPtr& spDefQuote)
	{
		m_spDefQuote    = spDefQuote;
		m_pDefaultQuote = dynamic_cast<CMmQvQuoteAtom*>(m_spDefQuote.GetInterfacePtr());
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTRT_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum, OptType, m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vola, m_dVola)
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyInShares, m_nQtyInShares)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvQuoteColl*, Quote, m_spQuote)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvQuoteAtom*, DefQuote, m_spDefQuote)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, RootID, m_nRootID)

	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradingClose, m_dtTradingClose)

	STDMETHOD(put_Qty)(long nQty);

	STDMETHOD(putref_DefQuote)(IMmQvQuoteAtom* pAtom)
	{
		IMmQvQuoteAtomPtr spAtom(pAtom);
		SetDefaultQuote(spAtom);
		return S_OK;

	}
	IMPLEMENT_SIMPLE_PROPERTY(LONG, IsFlex, m_nIsFlex)

	STDMETHOD(CalcMarketDepth)(SAFEARRAY** BidMarketDepth, SAFEARRAY** AskMarketDepth/*,LONG* lBidCount, LONG* lAskCount, LONG *retDiffPrcCount*/);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvOptAtom), CMmQvOptAtom)

#endif //__MMQVOPTATOM_H_
