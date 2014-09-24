// MmIhIndexAtom.h : Declaration of the CMmIhIndexAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmIndexHedge.h"
#include "MmIhOptColl.h"
#include "MmIhUndColl.h"

_COM_SMARTPTR_TYPEDEF(IMmIhIndexAtom, IID_IMmIhIndexAtom);

struct __MmIhIndexAtom
{
	LONG						m_nID;
	CComBSTR					m_bstrSymbol;
	DOUBLE						m_dYield;
	VARIANT_BOOL				m_bIsHTB;
	VARIANT_BOOL				m_bIsAmerican;
	DOUBLE						m_dSkew;
	DOUBLE						m_dKurt;
	IIndexAtomPtr				m_spBasketIndex;

	IEtsPriceProfileAtomPtr		m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr		m_spOptPriceProfile;
	IVolaControlPtr				m_spVolaSrv;
	IMmIhOptCollPtr				m_spOpt;

	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;

	IMmIhUndCollPtr				m_spUnd;
	DOUBLE						m_dNetDelta;

	LONG						m_nSleepStep;

	__MmIhIndexAtom()
		: m_nID(0L), m_dYield(0.), m_bIsHTB(VARIANT_FALSE), m_bIsAmerican(VARIANT_FALSE),
		m_dSkew(0.), m_dKurt(0.), m_dPriceBid(BAD_DOUBLE_VALUE), m_dPriceAsk(BAD_DOUBLE_VALUE),
		m_dPriceLast(BAD_DOUBLE_VALUE), m_dNetDelta(BAD_DOUBLE_VALUE), m_nSleepStep(0L)  
	{
	}
};

// CMmIhIndexAtom

class ATL_NO_VTABLE CMmIhIndexAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmIhIndexAtom, &CLSID_MmIhIndexAtom>,
	public IDispatchImpl<IMmIhIndexAtom, &IID_IMmIhIndexAtom, &LIBID_EtsMmIndexHedgeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmIhIndexAtom
{
public:
	CMmIhIndexAtom():
		m_hCalcSleepTimer(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMIHINDEXATOM)


BEGIN_COM_MAP(CMmIhIndexAtom)
	COM_INTERFACE_ENTRY(IMmIhIndexAtom)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			_CHK(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));
			_CHK(m_spOpt.CreateInstance(CLSID_MmIhOptColl), _T("Fail to create options."));
			_CHK(m_spUnd.CreateInstance(CLSID_MmIhUndColl), _T("Fail to create underlying."));

			m_hCalcSleepTimer = CreateWaitableTimer(NULL, TRUE, NULL);
			if (!m_hCalcSleepTimer)
				CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(GetLastError()), _T("Fail to create timer"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmIhIndexAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spVolaSrv = NULL;
		m_spOpt = NULL;
		m_spUnd = NULL;
		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;
		m_spBasketIndex = NULL;

		if(m_hCalcSleepTimer)
		{	
			CloseHandle(m_hCalcSleepTimer);
			m_hCalcSleepTimer = 0;
		}
	}

private:
	void _CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt);

	HRESULT CalcOptionGreeks(IMmIhOptAtomPtr aOpt, DOUBLE dSpotPriceMid,
				EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility, 
				VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
				DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
				ICalculationParametrs* pParams);
	
	void _GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();


	HANDLE	m_hCalcSleepTimer;
	

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Yield, m_dYield)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHTB, m_bIsHTB)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Kurt, m_dKurt)
	IMPLEMENT_OBJECT_PROPERTY(IIndexAtom*, BasketIndex, m_spBasketIndex)
	
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolaSrv, m_spVolaSrv)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmIhOptColl*, Opt, m_spOpt)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceLast, m_dPriceLast)

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmIhUndColl*, Und, m_spUnd)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, m_dNetDelta)

	STDMETHOD(CalcAllOptions)(EtsCalcModelTypeEnum enCalcModel,
								VARIANT_BOOL bUseTheoVolatility,
								VARIANT_BOOL bUseTheoVolaNoBid,
								VARIANT_BOOL bUseTheoVolaBadMarket,
								DOUBLE dUndPriceTolerance,
								enum EtsPriceRoundingRuleEnum enPriceRoundingRule,
								LONG nCalcSleepFreq,
								LONG nCalcSleepAmt,
								ICalculationParametrs* pParams);
};

OBJECT_ENTRY_AUTO(__uuidof(MmIhIndexAtom), CMmIhIndexAtom)
