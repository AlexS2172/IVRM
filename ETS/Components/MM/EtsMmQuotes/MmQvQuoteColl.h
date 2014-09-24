// MmQvQuoteColl.h : Declaration of the CMmQvQuoteColl
#ifndef __MMQVQUOTECOLL_H__
#define __MMQVQUOTECOLL_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvQuoteAtom.h"

struct SQuoteData
{
public:
	double m_dBid;
	double m_dAsk;
	double m_dPrice;

	SQuoteData(const SQuoteData& quote):
	m_dBid(quote.m_dBid),
		m_dAsk(quote.m_dAsk),
		m_dPrice(quote.m_dPrice){}

	SQuoteData& operator=(const SQuoteData& quote)
	{
		m_dBid   = quote.m_dBid;
		m_dAsk   = quote.m_dAsk;
		m_dPrice = quote.m_dPrice;
		return *this;
	}

	SQuoteData():
	m_dBid(BAD_DOUBLE_VALUE),
		m_dAsk(BAD_DOUBLE_VALUE),
		m_dPrice(BAD_DOUBLE_VALUE){}

public:
	bool IsValid(){return m_dBid > 0 || m_dAsk > 0 || m_dPrice > 0;}
};

struct _DividendsCache:
	public boost::noncopyable
{
public:
	_DividendsCache()
		:m_nDivCount(0L)
		,m_dYield(0){}
public:
	long   m_nDivCount;
	double m_dYield;
	CSafeArrayWrapper<double>  m_saDates;
	CSafeArrayWrapper<double>  m_saAmounts;
};
typedef boost::shared_ptr<_DividendsCache> _DividendsCachePtr;



_COM_SMARTPTR_TYPEDEF(IMmQvQuoteColl, IID_IMmQvQuoteColl);

typedef IDispatchImpl<IMmQvQuoteColl, &IID_IMmQvQuoteColl, &LIBID_EtsMmQuotesLib>													IMmQvQuoteCollDispImpl;
typedef ICollectionOnSTLMapExOfInterfacePtrImpl<IMmQvQuoteCollDispImpl, IMmQvQuoteAtom, LONG, LONG, BSTR, _bstr_t  >	IMmQvQuoteCollImpl;

// CMmQvQuoteColl
class ATL_NO_VTABLE CMmQvQuoteColl : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvQuoteColl, &CLSID_MmQvQuoteColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvQuoteColl>,
	public IMmQvQuoteCollImpl
{
public:
	typedef std::vector<IMmQvQuoteAtomPtr> CQuotesVector;
	CMmQvQuoteColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVQUOTECOLL)


BEGIN_COM_MAP(CMmQvQuoteColl)
	COM_INTERFACE_ENTRY(IMmQvQuoteColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		IMmQvQuoteCollImpl::Clear();
		m_pUnkMarshaler.Release();

	}

private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMmQvQuoteAtomPtr GetQuote(long lExchangeID)
	{
		IMmQvQuoteAtomPtr spReturn;
		CollType::iterator itr = m_collRef.find(lExchangeID);
		if(itr != m_collRef.end())
		{
			spReturn = itr->second->second;
		}
		return spReturn;
	}

	IMmQvQuoteAtomPtr AddNew(long lExchangeID, _bstr_t bsExchangeCode, CComObject<CMmQvQuoteAtom>** ppAtom= NULL);
	STDMETHOD(Add)(LONG Key, BSTR SortKey, IMmQvQuoteAtom* Value, IMmQvQuoteAtom** pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvQuoteColl), CMmQvQuoteColl)

#endif //__MMQVQUOTECOLL_H__