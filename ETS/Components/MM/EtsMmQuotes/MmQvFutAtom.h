// MmQvFutAtom.h : Declaration of the CMmQvFutAtom
#ifndef __MMQVFUTATOM_H__
#define __MMQVFUTATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvExpColl.h"
#include "MmQvQuoteColl.h"
#include "MmQvOptColl.h"
#include "MmQvFutRootAtom.h"
//#include "MmQvUndAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmQvFutAtom, IID_IMmQvFutAtom);

class  CMmQvUndAtom; 
class  CMmQvOptAtom;
class  CMmQvQuoteAtom;
class  CMmQvExpAtom;
struct SQuoteData;

struct __MmQvFutAtom
{
	LONG						m_nID;
	LONG						m_nUndID;
	_bstr_t						m_bstrSymbol;
	_bstr_t						m_bstrContractName;
	LONG						m_nFutRootID;
	LONG						m_nExpCalendarID;
	IEtsPriceProfileAtomPtr		m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr		m_spOptPriceProfile;
	DATE						m_dtMaturityDate;
	DATE						m_dtMaturityMonth;
	VARIANT_BOOL				m_bIsAmerican;

	IExchCollPtr				m_spFutExch;
	IExchCollPtr				m_spOptExch;
	IMmQvQuoteCollPtr			m_spQuote;
	IMmQvExpCollPtr				m_spExp;
	IMmQvOptCollPtr				m_spOpt;

	IVolaControlPtr				m_spVolaSrv;

	LONG						m_nQty;
	LONG						m_nQtyInShares;
	LONG						m_nLotSize;

	_bstr_t						m_bstrQuotationUnit;

	DOUBLE						m_dNetDelta;
	DOUBLE						m_dNetDeltaEq;
	DOUBLE						m_dTotalDelta;
	DOUBLE						m_dTotalDeltaEq;
	DOUBLE						m_dTotalGamma;
	DOUBLE						m_dTotalGammaEq;
	DOUBLE						m_dTotalVega;
	DOUBLE						m_dTotalTheta;
	DOUBLE						m_dTotalRho;

	LONG						m_nPosExchID;
	LONG						m_nOptPosExchID;

	LONG						m_nSleepStep;

	VARIANT_BOOL                m_bShowInFutureContractEq;
	VARIANT_BOOL				m_bVisible;
	DOUBLE						m_dRatio;
	DOUBLE						m_dBasis;				
	DOUBLE						m_dActivePrice;
	VARIANT_BOOL				m_bUseManualActivePrice;


	__MmQvFutAtom()
		: m_nID(0L)
		, m_nUndID(0L)
		, m_nFutRootID(0L)
		, m_nExpCalendarID(0L)
		, m_dtMaturityDate(0.)
		, m_dtMaturityMonth(0.)
		, m_bIsAmerican(VARIANT_FALSE)
		, m_nQty(BAD_LONG_VALUE)
		, m_nQtyInShares(BAD_LONG_VALUE)
		, m_nLotSize(BAD_LONG_VALUE)
		, m_dNetDelta(BAD_DOUBLE_VALUE)
		, m_dTotalDelta(BAD_DOUBLE_VALUE)
		, m_dTotalGamma(BAD_DOUBLE_VALUE)
		, m_dTotalVega(BAD_DOUBLE_VALUE)
		, m_dTotalTheta(BAD_DOUBLE_VALUE)
		, m_dTotalRho(BAD_DOUBLE_VALUE)
		, m_nPosExchID(0L)
		, m_nOptPosExchID(0L)
		, m_nSleepStep(0L)
		, m_dNetDeltaEq(BAD_DOUBLE_VALUE)
		, m_dTotalDeltaEq(BAD_DOUBLE_VALUE)
		, m_dTotalGammaEq(BAD_DOUBLE_VALUE)
		, m_bShowInFutureContractEq(VARIANT_FALSE)
		, m_bVisible(VARIANT_FALSE)
		, m_dRatio(BAD_DOUBLE_VALUE)
		, m_dBasis(BAD_DOUBLE_VALUE)
		, m_dActivePrice(BAD_DOUBLE_VALUE)
		, m_bUseManualActivePrice(VARIANT_FALSE)
	{
	}
};

// CMmQvFutAtom

class ATL_NO_VTABLE CMmQvFutAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvFutAtom, &CLSID_MmQvFutAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvFutAtom>,
	public IDispatchImpl<IMmQvFutAtom, &IID_IMmQvFutAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvFutAtom
{
public:
	CMmQvFutAtom()  
	  :m_hCalcSleepTimer(0)
	  ,m_bStopCalc(FALSE)
	  ,m_pQuote(NULL)
	  ,m_pExp(NULL)
	  ,m_pOpt(NULL)
	  ,m_pFutureRoot(NULL)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVFUTATOM)


BEGIN_COM_MAP(CMmQvFutAtom)
	COM_INTERFACE_ENTRY(IMmQvFutAtom)
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
			_CHK(m_spFutExch.CreateInstance(CLSID_ExchColl), _T("Fail to create futures exchanges."));
			_CHK(m_spOptExch.CreateInstance(CLSID_ExchColl), _T("Fail to create options exchanges."));

			_CHK(CComObject<CMmQvQuoteColl>::CreateInstance(&m_pQuote), _T("Fail to create quotes."));
			m_spQuote.Attach(m_pQuote, TRUE);

			_CHK(CComObject<CMmQvExpColl>::CreateInstance(&m_pExp), _T("Fail to create expiries."));
			m_spExp.Attach(m_pExp, TRUE);

			_CHK(CComObject<CMmQvOptColl>::CreateInstance(&m_pOpt), _T("Fail to create options."));
			m_spOpt.Attach(m_pOpt, TRUE);

			_CHK(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));

			m_hCalcSleepTimer = ::CreateWaitableTimer(NULL, TRUE, NULL);
			if(!m_hCalcSleepTimer)
				CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(GetLastError()), _T("Fail to create timer"));

			hr = CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p);

		}
		catch(const _com_error& e)
		{
			hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvFutAtom, e.Error());
		}
		return hr;
	}
	
	void FinalRelease()
	{
		StopCalc();

		m_spFutExch = NULL;
		m_spOptExch = NULL;
		m_spQuote = NULL;
		m_spExp = NULL;
		m_spOpt = NULL;
		m_spVolaSrv = NULL;

		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;

		if(m_hCalcSleepTimer)
		{	
			CloseHandle(m_hCalcSleepTimer);
			m_hCalcSleepTimer = 0;
		}
		m_pUnkMarshaler.Release();
	}

private:
	void _CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt);

	HANDLE				m_hCalcSleepTimer;
	BOOL				m_bStopCalc;
	CComPtr<IUnknown>	m_pUnkMarshaler;

public:
	CComObject<CMmQvFutRootAtom>* m_pFutureRoot;
	CComObject<CMmQvQuoteColl>* m_pQuote;
	CComObject<CMmQvExpColl>*   m_pExp; 
	CComObject<CMmQvOptColl>*	m_pOpt;

	IMmQvFutRootAtomPtr         m_spFutureRoot;

	HRESULT rawCalcOptionGreeks(
							CMmQvUndAtom*		aUnd, 
							SQuoteData*			futPrice,
							_DividendsCachePtr  spDividend,
							CMmQvOptAtom*		aOpt, 
							CMmQvQuoteAtom*		aQuote,
							CMmQvExpAtom*		aExp, 
							LONG nGreekMask, 
							MmQvIvCalcEnum enCalcIV,
							EtsCalcModelTypeEnum enCalcModel, 
							VARIANT_BOOL bUseTheoVolatility,
							VARIANT_BOOL bUseTheoVolaNoBid, 
							VARIANT_BOOL bUseTheoVolaBadMarket,
							DOUBLE dUndPriceTolerance, 
							EtsPriceRoundingRuleEnum enPriceRoundingRule,
							double dRate, 
							DATE dtCalcDate, 
							VARIANT_BOOL ManualEdit,
							ICalculationParametrs* pParams);

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, UndID, m_nUndID)
	IMPLEMENT_BSTRT_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_BSTRT_PROPERTY(ContractName, m_bstrContractName)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, FutRootID, m_nFutRootID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpCalendarID, m_nExpCalendarID)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolaSrv, m_spVolaSrv)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, MaturityDate, m_dtMaturityDate)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, MaturityMonth, m_dtMaturityMonth)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IExchColl*, FutExch, m_spFutExch)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IExchColl*, OptExch, m_spOptExch)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvQuoteColl*, Quote, m_spQuote)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvExpColl*, Exp, m_spExp)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvOptColl*, Opt, m_spOpt)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyInShares, m_nQtyInShares)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)

	IMPLEMENT_BSTRT_PROPERTY(QuotationUnit, m_bstrQuotationUnit)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, m_dNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDeltaEq, m_dNetDeltaEq)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalDelta, m_dTotalDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalDeltaEq, m_dTotalDeltaEq)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalGamma, m_dTotalGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalGammaEq, m_dTotalGammaEq)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalVega, m_dTotalVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalTheta, m_dTotalTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalRho, m_dTotalRho)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, PosExchID, m_nPosExchID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptPosExchID, m_nOptPosExchID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Ratio, m_dRatio)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Basis, m_dBasis)

	
	IMPLEMENT_SIMPLE_PROPERTY( DOUBLE, ActivePrice, m_dActivePrice)
	IMPLEMENT_SIMPLE_PROPERTY( VARIANT_BOOL, IsUseManualActivePrice, m_bUseManualActivePrice)

	STDMETHOD(CalcOptionGreeks)(IMmQvUndAtom* aUnd, IMmQvOptAtom* aOpt, IMmQvQuoteAtom* aQuote,
							IMmQvExpAtom* aExp, LONG nGreekMask, MmQvIvCalcEnum enCalcIV,
							EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility,
							VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
							DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
							VARIANT_BOOL bUseCustomRates, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit, 
							ICalculationParametrs* pParams);

	STDMETHOD(CalcAllOptions)(IMmQvUndAtom* aUnd, LONG nCallGreekMask, LONG nPutGreekMask,
							MmQvIvCalcEnum enCallCalcIV, MmQvIvCalcEnum enPutCalcIV, 
							EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility,
							VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket,
							VARIANT_BOOL bRecalcGreeks, DOUBLE dUndPriceTolerance,
							EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
							LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
							ICalculationParametrs* pParams);

	STDMETHOD(GetFuturePrice)(  IMmQvUndAtom* aUnd,DOUBLE dTolerance, 
								EtsPriceRoundingRuleEnum enPriceRound,
								EtsReplacePriceStatusEnum * penPriceStatus,
								VARIANT_BOOL *bFutureUsed, 
								DOUBLE	*pPrice);

	STDMETHOD(SetDirty)();

	STDMETHOD(StopCalc)();
	STDMETHOD(CleanUp)();

	STDMETHOD(get_AtmVola)(IMmQvExpAtom* pExp, DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule, LONG SurfaceID, DOUBLE* pVola);
	STDMETHOD(get_AtmStrike)(DOUBLE dUndPriceTolerance, enum EtsPriceRoundingRuleEnum enPriceRoundingRule, DOUBLE* dStrike);

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,	ShowInFutureContractEq, m_bShowInFutureContractEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Visible, m_bVisible)
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvFutAtom), CMmQvFutAtom)

#endif //__MMQVFUTATOM_H__
