// MmShUndAtom.h : Declaration of the CMmShUndAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmStockHedge.h"
#include "MmShOptRootColl.h"
#include "MmShOptColl.h"

_COM_SMARTPTR_TYPEDEF(IMmShUndAtom, IID_IMmShUndAtom);
_COM_SMARTPTR_TYPEDEF(IMmShUndColl, IID_IMmShUndColl);

struct __MmShUndAtom
{
	LONG						m_nID;
	CComBSTR					m_bstrSymbol;
	EtsContractTypeEnum			m_enUndType;
	//LONG						m_nDivFreq;
	//DATE						m_dtDivDate;
	//DOUBLE						m_dDivAmt;
	VARIANT_BOOL				m_bIsHTB;
	VARIANT_BOOL				m_bIsAmerican;
	DOUBLE						m_dSkew;
	DOUBLE						m_dKurt;
	VARIANT_BOOL				m_bUseMidRates;
	DOUBLE						m_dUndPosForRates;
	LONG						m_nTotalQtyInShares;
	CComBSTR					m_bstrPrimaryExch;

	IEtsPriceProfileAtomPtr		m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr		m_spOptPriceProfile;
	IVolaControlPtr				m_spVolaSrv;
	IMmShOptRootCollPtr			m_spOptRoot;
	IMmShOptCollPtr				m_spOpt;

	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;

	LONG						m_nSleepStep;
	IEtsIndexDivAtomPtr			m_spDividend;

	__MmShUndAtom()
		: m_nID(0L), m_enUndType(enCtStock)/*, m_nDivFreq(0L), m_dtDivDate(0.), m_dDivAmt(0.)*/,
		m_bIsHTB(VARIANT_FALSE), m_bIsAmerican(VARIANT_FALSE), m_dSkew(0.), m_dKurt(0.),
		m_bUseMidRates(VARIANT_FALSE), m_dUndPosForRates(0.), m_nTotalQtyInShares(BAD_LONG_VALUE),
		m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE),
		m_dPriceLast(BAD_DOUBLE_VALUE), m_nSleepStep(0L) 
	{
	}
};

// CMmShUndAtom

class ATL_NO_VTABLE CMmShUndAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmShUndAtom, &CLSID_MmShUndAtom>,
	public IDispatchImpl<IMmShUndAtom, &IID_IMmShUndAtom, &LIBID_EtsMmStockHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmShUndAtom
{
public:
	CMmShUndAtom():
	  m_hCalcSleepTimer(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMSHUNDATOM)


BEGIN_COM_MAP(CMmShUndAtom)
	COM_INTERFACE_ENTRY(IMmShUndAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			_CHK(m_spOptRoot.CreateInstance(CLSID_MmShOptRootColl), _T("Fail to create option roots."));
			_CHK(m_spOpt.CreateInstance(CLSID_MmShOptColl), _T("Fail to create options."));

			_CHK(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));
			_CHK(m_spDividend.CreateInstance(CLSID_EtsIndexDivAtom), _T("Fail to create div object."));

			m_hCalcSleepTimer = CreateWaitableTimer(NULL, TRUE, NULL);
			if (!m_hCalcSleepTimer)
				CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(GetLastError()), _T("Fail to create timer"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShUndAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spVolaSrv = NULL;
		m_spOptRoot = NULL;
		m_spOpt = NULL;
		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;

		if(m_hCalcSleepTimer)
		{	
			CloseHandle(m_hCalcSleepTimer);
			m_hCalcSleepTimer = 0;
		}
	}
private:

	HRESULT _GetSyntheticUnderlyingPrice(IMmShUndCollPtr collUndColl, ISynthRootAtomPtr aSynthRoot, 
					DOUBLE& dSpotBid, DOUBLE& dSpotAsk, DOUBLE& dSpotLast, 
					DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule);

	HRESULT CalcOptionGreeks(IMmShUndCollPtr collUndColl, IMmShOptAtomPtr aOpt, DOUBLE dSpotPriceMid,
		EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility, 
		VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
		DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
		ICalculationParametrsPtr spParams);

	void _GetSyntheticRootBasketDividends(ISynthRootAtomPtr aSynthRoot, REGULAR_DIVIDENDS* pDivs, 
		LONG nMaxCount) throw();

	void _CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt);

	HANDLE	m_hCalcSleepTimer;
	BOOL	m_bStopCalc;

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, UndType, m_enUndType)
//	IMPLEMENT_SIMPLE_PROPERTY(LONG, DivFreq, m_nDivFreq)
//	IMPLEMENT_SIMPLE_PROPERTY(DATE, DivDate, m_dtDivDate)
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DivAmt, m_dDivAmt)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHTB, m_bIsHTB)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Kurt, m_dKurt)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseMidRates, m_bUseMidRates)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, UndPosForRates, m_dUndPosForRates)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, TotalQtyInShares, m_nTotalQtyInShares)
	IMPLEMENT_BSTR_PROPERTY(PrimaryExch, m_bstrPrimaryExch)

	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolaSrv, m_spVolaSrv)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmShOptRootColl*, OptRoot, m_spOptRoot)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmShOptColl*, Opt, m_spOpt)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)
	IMPLEMENT_OBJECT_PROPERTY(IEtsIndexDivAtom * , Dividend, m_spDividend)

	STDMETHOD(CalcAllOptions)(IMmShUndColl* collUndColl,
							EtsCalcModelTypeEnum enCalcModel,
							VARIANT_BOOL bUseTheoVolatility,
							VARIANT_BOOL bUseTheoVolaNoBid,
							VARIANT_BOOL bUseTheoVolaBadMarket,
							DOUBLE dUndPriceTolerance,
							enum EtsPriceRoundingRuleEnum enPriceRoundingRule,
							LONG nCalcSleepFreq,
							LONG nCalcSleepAmt,
							ICalculationParametrs* Params);
};

OBJECT_ENTRY_AUTO(__uuidof(MmShUndAtom), CMmShUndAtom)
