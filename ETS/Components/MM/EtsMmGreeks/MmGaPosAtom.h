// MmGaPosAtom.h : Declaration of the CMmGaPosAtom
#ifndef __MMGAPOSATOM_H__
#define __MMGAPOSATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaSynthGreekColl.h"

_COM_SMARTPTR_TYPEDEF(IMmGaPosAtom, IID_IMmGaPosAtom);

struct __MmGaPosAtom
{
	LONG					m_nID;
	EtsContractTypeEnum		m_enContractType;
	CComBSTR				m_bstrSymbol;
	EtsOptionTypeEnum		m_enOptType;
	DATE					m_dtExpiry;
	DATE					m_dtExpiryOV;
	DATE					m_dtTradingClose;
	DOUBLE					m_dStrike;
	DOUBLE					m_dVola;
	LONG					m_nRootID;
	DOUBLE					m_dPriceBid;
	DOUBLE					m_dPriceAsk;
	DOUBLE					m_dPriceLast;
	DOUBLE					m_dPriceClose;
	DOUBLE					m_dPriceTheoClose;
	LONG					m_nLotSize;
	LONG					m_nQtyInShares;
	DOUBLE					m_dDeltaInShares;
	DOUBLE					m_dDeltaEq;
	DOUBLE					m_dGammaInShares;
	DOUBLE					m_dGammaEq;
	DOUBLE					m_dVegaInShares;
	DOUBLE					m_dTimeValueInShares;
	VARIANT_BOOL			m_bIsSynth;
	DOUBLE					m_dRhoInShares ;
	DOUBLE					m_dRho ;
	IMmGaSynthGreekCollPtr	m_spSynthGreek;

	__MmGaPosAtom()
		: m_nID(0L), m_enContractType(enCtStock), m_enOptType(enOtPut),
		m_dtExpiry(0.), m_dStrike(0.), m_dVola(0.), m_nRootID(0L),
		m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE),
		m_dPriceLast(BAD_DOUBLE_VALUE), m_dPriceClose(BAD_DOUBLE_VALUE),
		m_dPriceTheoClose(BAD_DOUBLE_VALUE), m_nLotSize(BAD_LONG_VALUE),
		m_nQtyInShares(BAD_LONG_VALUE), m_dDeltaInShares(BAD_DOUBLE_VALUE),
		m_dDeltaEq(BAD_DOUBLE_VALUE), m_dGammaEq(BAD_DOUBLE_VALUE),
		m_dGammaInShares(BAD_DOUBLE_VALUE), m_dVegaInShares(BAD_DOUBLE_VALUE),
		m_dTimeValueInShares(BAD_DOUBLE_VALUE), m_bIsSynth(VARIANT_FALSE), m_dRho(0.0),m_dRhoInShares(0.0),
		m_dtExpiryOV(0),
		m_dtTradingClose(0)
	{
	}

	void ClearValues()
	{
		m_dDeltaInShares = BAD_DOUBLE_VALUE;
		m_dDeltaEq = BAD_DOUBLE_VALUE;
		m_dGammaInShares = BAD_DOUBLE_VALUE;
		m_dGammaEq = BAD_DOUBLE_VALUE;
		m_dVegaInShares = BAD_DOUBLE_VALUE;
		m_dTimeValueInShares = BAD_DOUBLE_VALUE;
		m_dRhoInShares = BAD_DOUBLE_VALUE;
		m_dRho = BAD_DOUBLE_VALUE;
	}
};

// CMmGaPosAtom
class ATL_NO_VTABLE CMmGaPosAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaPosAtom, &CLSID_MmGaPosAtom>,
	public ISupportErrorInfoImpl<&IID_IMmGaPosAtom>,
	public IDispatchImpl<IMmGaPosAtom, &IID_IMmGaPosAtom, &LIBID_EtsMmGreeksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmGaPosAtom
{
public:
	CMmGaPosAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAPOSATOM)


BEGIN_COM_MAP(CMmGaPosAtom)
	COM_INTERFACE_ENTRY(IMmGaPosAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spSynthGreek = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, ContractType, m_enContractType)
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum, OptType, m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradingClose, m_dtTradingClose)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vola, m_dVola)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, RootID, m_nRootID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceClose, m_dPriceClose)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceTheoClose, m_dPriceTheoClose)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyInShares, m_nQtyInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaInShares, m_dVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValueInShares, m_dTimeValueInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsSynth, m_bIsSynth)
	IMPLEMENT_OBJECT_PROPERTY(IMmGaSynthGreekColl*, SynthGreek, m_spSynthGreek)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares, m_dRhoInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rho, m_dRho)

	STDMETHODIMP ClearValues()
	{
		__MmGaPosAtom::ClearValues();
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaPosAtom), CMmGaPosAtom)

#endif //__MMGAPOSATOM_H__