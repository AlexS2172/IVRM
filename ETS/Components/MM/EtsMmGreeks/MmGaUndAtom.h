// MmGaUndAtom.h : Declaration of the CMmGaUndAtom
#ifndef __MMGAUNDATOM_H__
#define __MMGAUNDATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmGreeks.h"
#include "MmGaExpColl.h"

_COM_SMARTPTR_TYPEDEF(IMmGaUndAtom, IID_IMmGaUndAtom);

struct __MmGaUndAtom
{
	LONG						m_nID;
	CComBSTR					m_bstrSymbol;
	EtsContractTypeEnum			m_enUndType;
	DOUBLE						m_dYield;
	//LONG						m_nDivFreq;
	//DATE						m_dtDivDate;
	//DOUBLE					m_dDivAmt;
	VARIANT_BOOL				m_bIsHTB;
	DOUBLE						m_dPos;
	IVolaControlPtr				m_spVolaSrv;
	DOUBLE						m_dSkew;
	DOUBLE						m_dKurt;
	VARIANT_BOOL				m_bIsAmerican;
	LONG						m_nExpCalendarID;
	IIndexAtomPtr				m_spBasketIndex;
	VARIANT_BOOL				m_bIsTraderContract;
	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;
	DOUBLE						m_dDeltaInShares;
	DOUBLE						m_dDeltaEq;
	DOUBLE						m_dGammaInShares;
	DOUBLE						m_dGammaEq;
	DOUBLE						m_dVegaInShares;
	DOUBLE						m_dTimeValueInShares;
	VARIANT_BOOL				m_bBadDeltaInShares;
	VARIANT_BOOL				m_bBadRhoInShares;
	VARIANT_BOOL				m_bBadDeltaEq;
	VARIANT_BOOL				m_bBadGammaInShares;
	VARIANT_BOOL				m_bBadGammaEq;
	VARIANT_BOOL				m_bBadVegaInShares;
	VARIANT_BOOL				m_bBadTimeValueInShares;
	IMmGaExpCollPtr				m_spExpiry;
	VARIANT_BOOL				m_bHasSynth;
	ISynthRootCollPtr			m_spSynthRoot;
	IMmGaPosCollPtr				m_spSynthPos;
	IMmGaSynthGreekCollPtr		m_spSynthGreek;
	IEtsPriceProfileAtomPtr		m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr		m_spOptPriceProfile;
	IEtsIndexDivAtomPtr			m_spDividend;
	DOUBLE						m_dRhoInShares;


	__MmGaUndAtom()
		: m_nID(0L), m_enUndType(enCtStock), m_dYield(0.)/*, m_nDivFreq(0L), m_dtDivDate(0.), m_dDivAmt(0.)*/,
		m_bIsHTB(VARIANT_FALSE), m_dPos(BAD_DOUBLE_VALUE), m_dSkew(0.), m_dKurt(0.), 
		m_bIsAmerican(VARIANT_FALSE), m_nExpCalendarID(0L), m_bIsTraderContract(VARIANT_FALSE),
		m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE), m_dPriceLast(BAD_DOUBLE_VALUE),
		m_dDeltaInShares(BAD_DOUBLE_VALUE), m_dDeltaEq(BAD_DOUBLE_VALUE),
		m_dGammaInShares(BAD_DOUBLE_VALUE), m_dGammaEq(BAD_DOUBLE_VALUE),
		m_dVegaInShares(BAD_DOUBLE_VALUE), m_dTimeValueInShares(BAD_DOUBLE_VALUE),
		m_bBadDeltaInShares(VARIANT_FALSE), m_bBadDeltaEq(VARIANT_FALSE),
		m_bBadGammaInShares(VARIANT_FALSE), m_bBadGammaEq(VARIANT_FALSE),
		m_bBadVegaInShares(VARIANT_FALSE), m_bBadTimeValueInShares(VARIANT_FALSE),
		m_bHasSynth(VARIANT_FALSE),m_dRhoInShares(BAD_DOUBLE_VALUE),m_bBadRhoInShares(VARIANT_FALSE)
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

		m_bBadRhoInShares = VARIANT_FALSE ;
		m_bBadDeltaInShares = VARIANT_FALSE;
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadGammaInShares = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;
		m_bBadVegaInShares = VARIANT_FALSE;
		m_bBadTimeValueInShares = VARIANT_FALSE;
		m_dRhoInShares = BAD_DOUBLE_VALUE;
	}
};

// CMmGaUndAtom
class ATL_NO_VTABLE CMmGaUndAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmGaUndAtom, &CLSID_MmGaUndAtom>,
	public ISupportErrorInfoImpl<&IID_IMmGaUndAtom>,
	public IDispatchImpl<IMmGaUndAtom, &IID_IMmGaUndAtom, &LIBID_EtsMmGreeksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmGaUndAtom
{
public:
	CMmGaUndAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMGAUNDATOM)


BEGIN_COM_MAP(CMmGaUndAtom)
	COM_INTERFACE_ENTRY(IMmGaUndAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			{
				CComObject<CMmGaExpColl>* pObject = NULL;
				_CHK(CComObject<CMmGaExpColl>::CreateInstance(&pObject), _T("Fail to create expiries."));
				m_spExpiry.Attach(pObject, TRUE);
			}
			__CHECK_HRESULT(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));
			__CHECK_HRESULT(m_spDividend.CreateInstance(CLSID_EtsIndexDivAtom), _T("Fail to create div object."));

		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaUndAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spExpiry = NULL;
		m_spBasketIndex = NULL;
		m_spVolaSrv = NULL;
		m_spSynthRoot = NULL;
		m_spSynthPos = NULL;
		m_spSynthGreek = NULL;
		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;
		m_spDividend = NULL;

	}

private:
	STDMETHOD(_CalcPosition)(IMmGaPosAtom* pPos, IMmGaExpAtom* pExp, IMmGaUndColl* pCollUnd, DOUBLE dUndPriceMid,
							EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid,
							VARIANT_BOOL bUseTheoVolaBadMarket, DOUBLE dUndPriceTolerance, 
							EtsPriceRoundingRuleEnum enPriceRoundingRule,
							ICalculationParametrs* pParams);

	STDMETHOD(_GetSyntheticUnderlyingPrice)(IMmGaUndColl* pCollUnd, ISynthRootAtom* pSynthRoot,
											DOUBLE& dSpotBid, DOUBLE& dSpotAsk, DOUBLE& dSpotLast);

	void _GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();
	void _GetSyntheticRootBasketDividends(ISynthRootAtom* pSynthRoot, REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();
	
public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, UndType, m_enUndType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Yield, m_dYield)
//	IMPLEMENT_SIMPLE_PROPERTY(LONG, DivFreq, m_nDivFreq)
//	IMPLEMENT_SIMPLE_PROPERTY(DATE, DivDate, m_dtDivDate)
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DivAmt, m_dDivAmt)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHTB, m_bIsHTB)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Pos, m_dPos)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolaSrv, m_spVolaSrv)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Kurt, m_dKurt)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpCalendarID, m_nExpCalendarID)
	IMPLEMENT_OBJECT_PROPERTY(IIndexAtom*, BasketIndex, m_spBasketIndex)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsTraderContract, m_bIsTraderContract)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaInShares, m_dVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValueInShares, m_dTimeValueInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInShares, m_bBadDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRhoInShares, m_bBadRhoInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaInShares, m_bBadGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, m_bBadGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaInShares, m_bBadVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTimeValueInShares, m_bBadTimeValueInShares)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmGaExpColl*, Expiry, m_spExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HasSynth, m_bHasSynth)
	IMPLEMENT_OBJECT_PROPERTY(ISynthRootColl*, SynthRoot, m_spSynthRoot)
	IMPLEMENT_OBJECT_PROPERTY(IMmGaPosColl*, SynthPos, m_spSynthPos)
	IMPLEMENT_OBJECT_PROPERTY(IMmGaSynthGreekColl*, SynthGreek, m_spSynthGreek)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsIndexDivAtom*, Dividend, m_spDividend)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares,m_dRhoInShares)

	STDMETHODIMP ClearValues()
	{
		__MmGaUndAtom::ClearValues();
		return S_OK;
	}

	STDMETHOD(CalcPositions)(IMmGaUndColl* collUnd, EtsCalcModelTypeEnum enCalcModel,
							VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
							VARIANT_BOOL bUseTheoVolaBadMarket, DOUBLE dUndPriceTolerance, 
							EtsPriceRoundingRuleEnum enPriceRoundingRule,
							ICalculationParametrs* pParams);
};

OBJECT_ENTRY_AUTO(__uuidof(MmGaUndAtom), CMmGaUndAtom)

#endif //__MMGAUNDATOM_H__