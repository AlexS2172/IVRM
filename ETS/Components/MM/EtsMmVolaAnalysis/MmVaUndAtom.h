// MmVaUndAtom.h : Declaration of the CMmVaUndAtom
#ifndef __MMVAUNDATOM_H__
#define __MMVAUNDATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmVolaAnalysis.h"
#include "MmVaExpColl.h"
#include "MmVaOptRootColl.h"

struct __MmVaUndAtom
{
	LONG						m_nID;
	_bstr_t						m_bstrSymbol;
	EtsContractTypeEnum			m_enUndType;
	DOUBLE						m_dYield;
	//LONG						m_nDivFreq;
	//DATE						m_dtDivDate;
	//DOUBLE					m_dDivAmt;
	VARIANT_BOOL				m_bIsHTB;
	VARIANT_BOOL				m_bIsAmerican;
	LONG						m_nExpCalendarID;
	IIndexAtomPtr				m_spBasketIndex;
	VARIANT_BOOL				m_bIsTraderContract;
	IVolaControlPtr				m_spVolaSrv;
	IMmVaExpCollPtr				m_spExpiry;
	DOUBLE						m_dSkew;
	DOUBLE						m_dKurt;
	LONG						m_nQtyInShares;
	DOUBLE						m_dPos;
	VARIANT_BOOL				m_bUseMidRates;
	IMmVaOptRootCollPtr			m_spOptRoot;
	IMmVaExpAtomPtr				m_spCurExpiry;
	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;
	DOUBLE						m_dPriceClose;
	LONG						m_nLotSize;
	EtsMmVaNewTradeTypeEnum		m_enNewTradeType;
	IExchAtomPtr				m_spExch;
	DOUBLE						m_dLoadValue1;
	DOUBLE						m_dLoadValue2;
	DOUBLE						m_dCalcValue1;
	DOUBLE						m_dCalcValue2;
	_bstr_t						m_bstrCalcExpression1;
	_bstr_t						m_bstrCalcExpression2;

	IEtsIndexDivAtomPtr			m_spDividend;


	__MmVaUndAtom()
		: m_nID(0L), m_enUndType(enCtStock), m_dYield(0.), /*m_nDivFreq(0L), m_dtDivDate(0.), m_dDivAmt(0.),*/
		m_bIsHTB(VARIANT_FALSE), m_bIsAmerican(VARIANT_FALSE), m_nExpCalendarID(0L), m_bIsTraderContract(VARIANT_FALSE),
		m_dSkew(0.), m_dKurt(0.), m_nQtyInShares(BAD_LONG_VALUE), m_dPos(BAD_DOUBLE_VALUE), m_bUseMidRates(VARIANT_FALSE),
		m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE), m_dPriceLast(BAD_DOUBLE_VALUE), m_nLotSize(BAD_LONG_VALUE),
		m_enNewTradeType(enMmVaNewTradeNone), m_dLoadValue1(BAD_DOUBLE_VALUE), m_dLoadValue2(BAD_DOUBLE_VALUE),
		m_dCalcValue1(BAD_DOUBLE_VALUE), m_dCalcValue2(BAD_DOUBLE_VALUE), m_dPriceClose(BAD_DOUBLE_VALUE)
	{
	}
};

// CMmVaUndAtom

class ATL_NO_VTABLE CMmVaUndAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmVaUndAtom, &CLSID_MmVaUndAtom>,
	public ISupportErrorInfoImpl<&IID_IMmVaUndAtom>,
	public IDispatchImpl<IMmVaUndAtom, &IID_IMmVaUndAtom, &LIBID_EtsMmVolaAnalysisLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmVaUndAtom
{
public:
	CMmVaUndAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMVAUNDATOM)


BEGIN_COM_MAP(CMmVaUndAtom)
	COM_INTERFACE_ENTRY(IMmVaUndAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			__CHECK_HRESULT(m_spExpiry.CreateInstance(CLSID_MmVaExpColl), _T("Fail to create expiries."));
			__CHECK_HRESULT(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));
			__CHECK_HRESULT(m_spOptRoot.CreateInstance(CLSID_MmVaOptRootColl), _T("Fail to create option roots."));
			__CHECK_HRESULT(m_spDividend.CreateInstance(CLSID_EtsIndexDivAtom),  _T("Fail to create div object."));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaUndAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spCurExpiry = NULL;
		m_spExpiry = NULL;
		m_spBasketIndex = NULL;
		m_spVolaSrv = NULL;
		m_spOptRoot = NULL;
		m_spExch = NULL;
	}

private:
	void _GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();
	

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTRT_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, UndType, m_enUndType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Yield, m_dYield)
	//IMPLEMENT_SIMPLE_PROPERTY(LONG, DivFreq, m_nDivFreq)
	//IMPLEMENT_SIMPLE_PROPERTY(DATE, DivDate, m_dtDivDate)
	//IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DivAmt, m_dDivAmt)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHTB, m_bIsHTB)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpCalendarID, m_nExpCalendarID)
	IMPLEMENT_OBJECT_PROPERTY(IIndexAtom*, BasketIndex, m_spBasketIndex)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsTraderContract, m_bIsTraderContract)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolaSrv, m_spVolaSrv)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmVaExpColl*, Expiry, m_spExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Kurt, m_dKurt)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyInShares, m_nQtyInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Pos, m_dPos)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseMidRates, m_bUseMidRates)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmVaOptRootColl*, OptRoot, m_spOptRoot)
	IMPLEMENT_OBJECT_PROPERTY(IMmVaExpAtom*, CurExpiry, m_spCurExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceClose, m_dPriceClose)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(EtsMmVaNewTradeTypeEnum, NewTradeType, m_enNewTradeType)
	IMPLEMENT_OBJECT_PROPERTY(IExchAtom*, Exch, m_spExch)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, LoadValue1, m_dLoadValue1)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, LoadValue2, m_dLoadValue2)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, CalcValue1, m_dCalcValue1)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, CalcValue2, m_dCalcValue2)
	IMPLEMENT_BSTRT_PROPERTY(CalcExpression1, m_bstrCalcExpression1)
	IMPLEMENT_BSTRT_PROPERTY(CalcExpression2, m_bstrCalcExpression2)
	IMPLEMENT_OBJECT_PROPERTY(IEtsIndexDivAtom*, Dividend, m_spDividend)

	STDMETHOD(CalcCurrentValues)(DOUBLE dVegaAmount, EtsCalcModelTypeEnum enCalcModel,
								VARIANT_BOOL bUseTheoVolatility,
								ICalculationParametrs* pParams);
};

OBJECT_ENTRY_AUTO(__uuidof(MmVaUndAtom), CMmVaUndAtom)

#endif //__MMVAUNDATOM_H__
