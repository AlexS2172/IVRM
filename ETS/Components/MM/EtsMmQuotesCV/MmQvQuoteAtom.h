// MmQvQuoteAtom.h : Declaration of the CMmQvQuoteAtom
#ifndef __MMQVQUOTEATOM_H__
#define __MMQVQUOTEATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"

/////////////////////////////////////////////////////////////////////////////
//
#define IMPLEMENT_SIMPLE_NUMERICWITHCHECK_PROPERTY(Type, Name, Member)	\
	\
	STDMETHODIMP put_##Name(Type NewVal)							\
{																	\
	ObjectLock lock(this);											\
	if(Member != NewVal)											\
		m_bDirtyQuote = true;										\
	Member = NewVal;												\
	return S_OK;													\
}																	\
	\
	STDMETHODIMP get_##Name(Type* pVal)								\
{																	\
	if(!pVal)														\
	{																\
	return E_POINTER;												\
	}																\
	\
	ObjectLock lock(this);											\
	*pVal = Member;													\
	return S_OK;													\
}													


_COM_SMARTPTR_TYPEDEF(IMmQvQuoteAtom, IID_IMmQvQuoteAtom);

struct __MmQvQuoteAtom
{
	IExchAtomPtr		m_spExch;
	DOUBLE				m_dPriceBid;
	DOUBLE				m_dPriceAsk;
	DOUBLE				m_dPriceOpen;
	DOUBLE				m_dPriceClose;
	DOUBLE				m_dPriceHigh;
	DOUBLE				m_dPriceLow;
	DOUBLE				m_dPriceLast;
	DOUBLE				m_dNetChange;
	LONG				m_nSizeBid;
	LONG				m_nSizeAsk;
	LONG				m_nVolume;
	LONG				m_nLotSize;
	LONG				m_nOpenInterest;
	DOUBLE				m_dPriceBidTick;
	DOUBLE				m_dPriceAskTick;
	DOUBLE				m_dIV;
	DOUBLE				m_dIVBid;
	DOUBLE				m_dIVAsk;
	DOUBLE				m_dPriceTheo;
	DOUBLE				m_dVega;
	DOUBLE				m_dDelta;
	DOUBLE				m_dGamma;
	DOUBLE				m_dTheta;
	DOUBLE				m_dRho;
	DOUBLE				m_dVegaDelta;
	DOUBLE				m_dVegaGamma;
	DOUBLE				m_dThetaDelta;
	DOUBLE				m_dThetaGamma;
	DOUBLE				m_dGammaPerTheta;
	DATE				m_dtUpdateTime;
	DOUBLE				m_dIVSpread;
	LONG				m_nCalcGreeksIVType;

	DOUBLE				m_dVolga;

	_bstr_t				m_bstrSeries;
	_bstr_t            m_sBidExchange;
	_bstr_t            m_sAskExchange;

	EtsReplacePriceStatusEnum	m_enReplacePriceStatus;
	MmQvPriceUpdateStatusEnum	m_enPriceUpdateStatus;

	__MmQvQuoteAtom()
		: m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE), m_dPriceOpen(BAD_DOUBLE_VALUE),
		m_dPriceClose(BAD_DOUBLE_VALUE), m_dPriceHigh(BAD_DOUBLE_VALUE), m_dPriceLow(BAD_DOUBLE_VALUE),
		m_dPriceLast(BAD_DOUBLE_VALUE), m_dNetChange(BAD_DOUBLE_VALUE), m_nSizeBid(BAD_LONG_VALUE),
		m_nSizeAsk(BAD_LONG_VALUE), m_nVolume(BAD_LONG_VALUE), m_nLotSize(BAD_LONG_VALUE), m_nOpenInterest(BAD_LONG_VALUE),
		m_dPriceBidTick(0.), m_dPriceAskTick(0.), m_dIV(BAD_DOUBLE_VALUE), m_dIVBid(BAD_DOUBLE_VALUE),
		m_dIVAsk(BAD_DOUBLE_VALUE), m_dPriceTheo(BAD_DOUBLE_VALUE), m_dVega(BAD_DOUBLE_VALUE),
		m_dDelta(BAD_DOUBLE_VALUE), m_dGamma(BAD_DOUBLE_VALUE), m_dTheta(BAD_DOUBLE_VALUE), m_dRho(BAD_DOUBLE_VALUE),
		m_dVegaDelta(BAD_DOUBLE_VALUE), m_dVegaGamma(BAD_DOUBLE_VALUE), m_dThetaDelta(BAD_DOUBLE_VALUE),
		m_dThetaGamma(BAD_DOUBLE_VALUE), m_dGammaPerTheta(BAD_DOUBLE_VALUE), m_dtUpdateTime(0.), m_dIVSpread(BAD_DOUBLE_VALUE),
		m_nCalcGreeksIVType(enMmQvCalcGreeksIVNone), m_enReplacePriceStatus(enRpsNone), m_enPriceUpdateStatus(enMmQvPusNone),
		m_dVolga(BAD_DOUBLE_VALUE)
	{
	}

	void ClearValues(VARIANT_BOOL bWithIV, VARIANT_BOOL bWithPriceStatus = VARIANT_TRUE)
	{
		if(VARIANT_TRUE == bWithIV)
		{
			m_dIV = BAD_DOUBLE_VALUE;
			m_dIVBid = BAD_DOUBLE_VALUE;
			m_dIVAsk = BAD_DOUBLE_VALUE;
			m_dIVSpread = BAD_DOUBLE_VALUE;
			m_nCalcGreeksIVType = enMmQvCalcGreeksIVNone;
		}

		m_dPriceTheo = BAD_DOUBLE_VALUE;
		m_dVega = BAD_DOUBLE_VALUE;
		m_dDelta = BAD_DOUBLE_VALUE;
		m_dGamma = BAD_DOUBLE_VALUE;
		m_dTheta = BAD_DOUBLE_VALUE;
		m_dRho = BAD_DOUBLE_VALUE;
		m_dVegaDelta = BAD_DOUBLE_VALUE;
		m_dVegaGamma = BAD_DOUBLE_VALUE;
		m_dThetaDelta = BAD_DOUBLE_VALUE;
		m_dThetaGamma = BAD_DOUBLE_VALUE;
		m_dGammaPerTheta = BAD_DOUBLE_VALUE;
		m_dVolga = BAD_DOUBLE_VALUE;

		if (bWithPriceStatus == VARIANT_TRUE)
		{
			m_sBidExchange = L"";
			m_sAskExchange = L"";
			m_enReplacePriceStatus = enRpsNone;
		}

	}
};

// CMmQvQuoteAtom

class ATL_NO_VTABLE CMmQvQuoteAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvQuoteAtom, &CLSID_MmQvQuoteAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvQuoteAtom>,
	public IDispatchImpl<IMmQvQuoteAtom, &IID_IMmQvQuoteAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvQuoteAtom
{
public:
	CMmQvQuoteAtom()
		:m_bCacheExchangeCodeInitialized(false)
		,m_lExchangeID(-1L)
		,m_bDirtyQuote(true)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVQUOTEATOM)


BEGIN_COM_MAP(CMmQvQuoteAtom)
	COM_INTERFACE_ENTRY(IMmQvQuoteAtom)
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
		m_spExch = NULL;
		m_pUnkMarshaler.Release();
	}
	_bstr_t GetExchangeCode()
	{
		if(!m_bCacheExchangeCodeInitialized)
		{
			m_bsCacheExchangeCode = m_spExch->Code;
			m_bCacheExchangeCodeInitialized = true;
		}
		return m_bsCacheExchangeCode;
	}

	long GetExchangeID()
	{
		if(m_lExchangeID<0)
			m_lExchangeID = m_spExch->ID;
		return m_lExchangeID;
	}
private:
	_bstr_t m_bsCacheExchangeCode;
	bool    m_bCacheExchangeCodeInitialized;
	long    m_lExchangeID;
	bool    m_bDirtyQuote;

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	void    SetDirty()     { m_bDirtyQuote = true; }
	void    SetProcessed() { m_bDirtyQuote = false;}
	bool    IsDirty() const{ return m_bDirtyQuote; }

	void    SetExchangeID(long lExchangeID) { m_lExchangeID = lExchangeID;}
	void    SetExchangeCode(const _bstr_t& bsCode)
	{
		m_bCacheExchangeCodeInitialized = true;
		m_bsCacheExchangeCode = bsCode;
	}
public:
	IMPLEMENT_OBJECT_PROPERTY(IExchAtom*, Exch, m_spExch)
	IMPLEMENT_SIMPLE_NUMERICWITHCHECK_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_NUMERICWITHCHECK_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_NUMERICWITHCHECK_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceOpen, m_dPriceOpen)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceClose, m_dPriceClose)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceHigh, m_dPriceHigh)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLow, m_dPriceLow)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetChange, m_dNetChange)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, SizeBid, m_nSizeBid)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, SizeAsk, m_nSizeAsk)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Volume, m_nVolume)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OpenInterest, m_nOpenInterest)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBidTick, m_dPriceBidTick)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAskTick, m_dPriceAskTick)
	IMPLEMENT_SIMPLE_NUMERICWITHCHECK_PROPERTY(DOUBLE, IV, m_dIV)
	IMPLEMENT_SIMPLE_NUMERICWITHCHECK_PROPERTY(DOUBLE, IVBid, m_dIVBid)
	IMPLEMENT_SIMPLE_NUMERICWITHCHECK_PROPERTY(DOUBLE, IVAsk, m_dIVAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceTheo, m_dPriceTheo)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vega, m_dVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Delta, m_dDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Gamma, m_dGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Theta, m_dTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rho, m_dRho)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaDelta, m_dVegaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaGamma, m_dVegaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaDelta, m_dThetaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaGamma, m_dThetaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaPerTheta, m_dGammaPerTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, UpdateTime, m_dtUpdateTime)
	IMPLEMENT_SIMPLE_NUMERICWITHCHECK_PROPERTY(DOUBLE, IVSpread, m_dIVSpread)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, CalcGreeksIVType, m_nCalcGreeksIVType)
	IMPLEMENT_SIMPLE_PROPERTY(EtsReplacePriceStatusEnum, ReplacePriceStatus, m_enReplacePriceStatus)
	IMPLEMENT_SIMPLE_PROPERTY(MmQvPriceUpdateStatusEnum, PriceUpdateStatus, m_enPriceUpdateStatus)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Volga, m_dVolga)

	IMPLEMENT_BSTRT_PROPERTY(Series, m_bstrSeries)
	IMPLEMENT_BSTRT_PROPERTY(BidExchange, m_sBidExchange)
	IMPLEMENT_BSTRT_PROPERTY(AskExchange, m_sAskExchange)

	STDMETHOD(get_IsDirty)(VARIANT_BOOL* vbValue)
	{
		if(!vbValue)
			return E_POINTER;
		*vbValue = IsDirty()?VARIANT_TRUE:VARIANT_FALSE;

		return S_OK;
	}

	STDMETHODIMP ClearValues(VARIANT_BOOL WithIV, VARIANT_BOOL WithPriceStatus)
	{
		__MmQvQuoteAtom::ClearValues(WithIV, WithPriceStatus);
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvQuoteAtom), CMmQvQuoteAtom)

#endif //__MMQVQUOTEATOM_H__
