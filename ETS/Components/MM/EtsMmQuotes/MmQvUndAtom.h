// MmQvUndAtom.h : Declaration of the CMmQvUndAtom
#ifndef __MMQVUNDATOM_H__
#define __MMQVUNDATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvExpColl.h"
#include "MmQvQuoteColl.h"
#include "MmQvOptRootColl.h"
#include "MmQvUndColl.h"
#include "MmQvOptColl.h"
#include "MmQvFutCollection.h"

_COM_SMARTPTR_TYPEDEF(IMmQvUndColl, IID_IMmQvUndColl);
_COM_SMARTPTR_TYPEDEF(IMmQvUndAtom, IID_IMmQvUndAtom);

class CMmQvGrpAtom;
struct __MmQvUndAtom
{
	LONG						m_nID;
	_bstr_t						m_bstrSymbol;
	EtsContractTypeEnum			m_enUndType;
	DOUBLE						m_dYield;
	VARIANT_BOOL				m_bIsHTB;
	VARIANT_BOOL				m_bUseCustDivs;
	LONG						m_nQty;
	LONG						m_nQtyInShares;
	IVolaControlPtr				m_spVolaSrv;
	IMmQvExpCollPtr				m_spExp;
	IMmQvQuoteCollPtr			m_spQuote;
	DOUBLE						m_dNetDelta;
	DOUBLE						m_dNetDeltaEq;
	DOUBLE						m_dTotalDelta;
	DOUBLE						m_dTotalGamma;
	DOUBLE						m_dTotalVega;
	DOUBLE						m_dTotalTheta;
	DOUBLE						m_dTotalRho;
	VARIANT_BOOL				m_bIsAmerican;
	LONG						m_nExpCalendarID;
	IIndexAtomPtr				m_spBasketIndex;
	VARIANT_BOOL				m_bIsTraderContract;
	DOUBLE						m_dSkew;
	DOUBLE						m_dKurt;
	VARIANT_BOOL				m_bUseMidRates;
	IMmQvOptRootCollPtr			m_spOptRoot;
	IMmQvUndCollPtr				m_spSynthUnd;
	IExchCollPtr				m_spUndExch;
	IExchCollPtr				m_spOptExch;
	IMmQvOptCollPtr				m_spOpt;
	_bstr_t						m_bstrSymbolName;
	LONG						m_nPosExchID;
	IEtsPriceProfileAtomPtr		m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr		m_spOptPriceProfile;
	DOUBLE						m_dUndPosForRates;
	LONG						m_nOptPosExchID;

	LONG						m_nSleepStep;
	IEtsIndexDivAtomPtr			m_spDividend;

	IMmQvFutCollectionPtr		m_spFut;

	//double						m_dSU_Price;
	DOUBLE						m_dFuturePrice;
	DOUBLE						m_dActivePrice;

	IMmQvFutAtomPtr				m_spActiveFuture;
	LONG						m_nPrimaryExchangeID;
	VARIANT_BOOL				m_bUseManualActivePrice;
	VARIANT_BOOL				m_fIsManualVol;

	VARIANT_BOOL				m_bIsHead;
	VARIANT_BOOL				m_bPriceByHead;
	IMmQvUndAtomPtr				m_spHeadComponent;
	DOUBLE						m_dCoeff;



	__MmQvUndAtom()
		: m_nID(0L)
		, m_enUndType(enCtStock)
		, m_dYield(0.)
		/*,m_nDivFreq(0L), m_dtDivDate(0.), m_dDivAmt(0.)*/
		,m_bIsHTB(VARIANT_FALSE)
		/*,m_bUseCustDivs(VARIANT_FALSE), m_nDivFreqCust(0L), m_dtDivDateCust(0.),*/
		/*,m_dDivAmtCust(0.),*/
		, m_nQty(BAD_LONG_VALUE)
		, m_nQtyInShares(BAD_LONG_VALUE)
		, m_dNetDelta(BAD_DOUBLE_VALUE)
		, m_dNetDeltaEq(BAD_DOUBLE_VALUE)
		, m_dTotalDelta(BAD_DOUBLE_VALUE)
		, m_dTotalGamma(BAD_DOUBLE_VALUE)
		, m_dTotalVega(BAD_DOUBLE_VALUE)
		, m_dTotalTheta(BAD_DOUBLE_VALUE)
		, m_dTotalRho(BAD_DOUBLE_VALUE)
		, m_bIsAmerican(VARIANT_FALSE)
		, m_nExpCalendarID(0L)
		, m_bIsTraderContract(VARIANT_FALSE)
		, m_dSkew(0.)
		, m_dKurt(0.)
		, m_bUseMidRates(VARIANT_FALSE)
		, m_nPosExchID(0L)
		, m_nSleepStep(0L)
		, m_dUndPosForRates(0.)
		, m_nOptPosExchID(0L)
		, m_dFuturePrice(BAD_DOUBLE_VALUE)
		, m_dActivePrice(BAD_DOUBLE_VALUE)
		, m_nPrimaryExchangeID(0)
		, m_bUseManualActivePrice(VARIANT_FALSE)
		, m_fIsManualVol(VARIANT_FALSE)
		, m_bPriceByHead(VARIANT_FALSE)
		, m_bIsHead(VARIANT_FALSE)
		, m_dCoeff(1.0)
	{
	}
};

struct sPriceKey
{
	sPriceKey()
		:m_dPriceUnd(BAD_DOUBLE_VALUE)
		,m_dPriceOpt(BAD_DOUBLE_VALUE){}

	sPriceKey(double dPriceUnd, double dPriceOpt)
		:m_dPriceUnd(dPriceUnd)
		,m_dPriceOpt(dPriceOpt){}

	double m_dPriceUnd;
	double m_dPriceOpt;

};

class CVolasMap
{
	typedef std::map<LONG, double> COptionVola;
	typedef std::map<LONG, COptionVola> CUndVolas;

public:
	CVolasMap(){};

	bool GetVola(double dUndPrice, double dOptPrice, double& dIV)
	{
		bool bRet = false;
		CAutoLock lock(m_csLock);

		CUndVolas::iterator itrUnd = m_VolasCache.find(GetKey(dUndPrice));
		if(itrUnd != m_VolasCache.end())
		{
			COptionVola::iterator itrOpt = itrUnd->second.find(GetKey(dOptPrice));
			if(itrOpt != itrUnd->second.end())
			{
				dIV = itrOpt->second;
				bRet = true;
			}
		}
		return bRet;
	}

	void PutVola(double dUndPrice, double dOptPrice, double dIV)
	{
		CAutoLock lock(m_csLock);
		m_VolasCache[GetKey(dUndPrice)].operator[](GetKey(dOptPrice)) = dIV;
	}
private:
	CUndVolas  m_VolasCache;
	CLock	   m_csLock;

	LONG       GetKey(double dPrice){ return static_cast<LONG>(dPrice*1000.0);}

};
typedef boost::shared_ptr<CVolasMap> CVolasMapPtr;




// CMmQvUndAtom
class CMmQvUndAtom;
typedef CStrategyPoolT<CComPoolThreadT<CMmQvUndAtom> >  CMmQvUndAtomThreadPool;

class CQuotesCalculationAtom
	: public CStrategyPoolObjectBase<CMmQvUndAtom>
{
public:
	CQuotesCalculationAtom(CMmQvUndAtom	*pUndAtom, LPCSTR lpszName)	
		:CStrategyPoolObjectBase<CMmQvUndAtom>(pUndAtom)
		,m_pUndAtom(pUndAtom) 
	{	m_hMutex.Attach(CreateMutex(NULL, FALSE, lpszName));}

	bool Initialize(
		IMmQvOptPairAtom*			pOptPair,
		IMmQvExpAtom*				pExpiry,
		IMmQvOptRootAtom*			pOptRoot,
		IMmQvUndColl*				pSynthUnd,
		double						dVola,
		_DividendsCachePtr&			divs,
		SQuoteData&					undQuote,
		long						nPutGreekMask,
		long						nCallGreekMask,
		MmQvIvCalcEnum				enPutCalcIV,
		MmQvIvCalcEnum				enCallCalcIV,
		EtsCalcModelTypeEnum		enCalcModel,
		VARIANT_BOOL				bUseTheoVolatility,
		DOUBLE						dUndPriceTolerance,
		EtsPriceRoundingRuleEnum	enPriceRoundingRule,
		VARIANT_BOOL				bUseCustomRates, 
		bool						bCalcDelta,
		bool						bIsRootSynthetic,
		bool						bCalculateOption,
		bool						bVisible,
		bool						bIsDirtyUnderlying,
		DATE						dtCalcDate,
		VARIANT_BOOL				bManualEdit, 
		VARIANT_BOOL				bUseTheoVolaNoBid,
		VARIANT_BOOL				bUseTheoVolaBadMarket,
		ICalculationParametrs*		pParams)
	{
		bool bRet = false;
		m_nPutGreekMask			=nPutGreekMask;
		m_nCallGreekMask		=nCallGreekMask;
		m_enPutCalcIV			=enPutCalcIV;
		m_enCallCalcIV			=enCallCalcIV;
		m_enCalcModel			=enCalcModel;
		m_bUseTheoVolatility	=bUseTheoVolatility;
		m_dUndPriceTolerance	=dUndPriceTolerance;
		m_enPriceRoundingRule	=enPriceRoundingRule;
		m_bUseCustomRates		=bUseCustomRates; 
		m_bCalcDelta			=bCalcDelta;
		m_bVisible				=bVisible;
		m_bIsDirtyUnderlying	=bIsDirtyUnderlying;
		m_bIsRootSynthetic		=bIsRootSynthetic;
		m_bCalculateOption		=bCalculateOption;
		m_dtCalcDate			=dtCalcDate;
		m_bManualEdit			=bManualEdit; 
		m_bUseTheoVolaNoBid		=bUseTheoVolaNoBid;
		m_bUseTheoVolaBadMarket	=bUseTheoVolaBadMarket;
		m_dVola					=dVola;

		if(WAIT_TIMEOUT != WaitForSingleObject(m_hMutex, 5000))
		{
			try
			{
				m_spOptPair				=pOptPair;
				m_spExpiry				=pExpiry;
				m_spOptRoot				=pOptRoot;
				m_spSynthUnd			=pSynthUnd;
				m_divs					=divs;
				m_undQuote				=undQuote;
				m_spCalculationParametrs=pParams;
				bRet = true;
			}
			catch(...){}
			ReleaseMutex(m_hMutex);
		}
		return bRet;
	}

	virtual ~CQuotesCalculationAtom()
	{
		if(WAIT_TIMEOUT != WaitForSingleObject(m_hMutex, 5000))
		{
			try
			{
				m_spOptPair = NULL;
				m_spExpiry	= NULL;
				m_spOptRoot = NULL;
				m_spSynthUnd= NULL;
				m_spCalculationParametrs = NULL;
				m_divs.reset();
			}
			catch(...){}
			ReleaseMutex(m_hMutex);
		}
	}
private:
	CQuotesCalculationAtom()
		:CStrategyPoolObjectBase<CMmQvUndAtom>(NULL){}
public:

	CMmQvUndAtom*		m_pUndAtom;

	IMmQvOptPairAtomPtr m_spOptPair;
	IMmQvExpAtomPtr		m_spExpiry;
	IMmQvOptRootAtomPtr m_spOptRoot;
	IMmQvUndCollPtr		m_spSynthUnd;
	_DividendsCachePtr			m_divs;

	double						m_dVola;
	SQuoteData					m_undQuote;
	long						m_nPutGreekMask;
	long						m_nCallGreekMask;
	MmQvIvCalcEnum				m_enPutCalcIV;
	MmQvIvCalcEnum				m_enCallCalcIV;
	EtsCalcModelTypeEnum		m_enCalcModel;
	VARIANT_BOOL				m_bUseTheoVolatility;
	DOUBLE						m_dUndPriceTolerance;
	EtsPriceRoundingRuleEnum	m_enPriceRoundingRule;
	VARIANT_BOOL				m_bUseCustomRates; 
	bool						m_bCalcDelta;
	bool                        m_bIsRootSynthetic;
	bool						m_bCalculateOption;
	bool						m_bVisible;
	bool						m_bIsDirtyUnderlying;
	DATE						m_dtCalcDate;
	VARIANT_BOOL				m_bManualEdit; 
	VARIANT_BOOL				m_bUseTheoVolaNoBid;
	VARIANT_BOOL				m_bUseTheoVolaBadMarket;
	ICalculationParametrsPtr	m_spCalculationParametrs;

	CHandle                     m_hMutex;

	virtual void DoTask();

private:

	void rawCalculate(CMmQvUndAtom*	 pUndAtom);
};

typedef boost::shared_ptr<CQuotesCalculationAtom> CQuotesCalculationAtomPtr;



class ATL_NO_VTABLE CMmQvUndAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvUndAtom, &CLSID_MmQvUndAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvUndAtom>,
	public IDispatchImpl<IMmQvUndAtom, &IID_IMmQvUndAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvUndAtom
{
private:
	typedef std::map<long, _DividendsCachePtr> CDividendsCache;

public:
	CMmQvUndAtom()  
	  :m_hCalcSleepTimer(0) 
	  ,m_nCalculations(0L)
	  ,m_dwAbortTime(0L)
	  ,m_Pool(-1)
	  ,m_pFut(NULL)
	  ,m_pGrp(NULL)
	  ,m_bStopCalc(FALSE)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVUNDATOM)


BEGIN_COM_MAP(CMmQvUndAtom)
	COM_INTERFACE_ENTRY(IMmQvUndAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

public:
	CComObject<CMmQvExpColl>*		m_pExp;
	CComObject<CMmQvQuoteColl>*		m_pQuote;
	CComObject<CMmQvOptRootColl>*   m_pOptRoot;
	CComObject<CMmQvOptColl>*       m_pOpt;
	CComObject<CMmQvFutCollection>* m_pFut;
public:
	CComObject<CMmQvExpColl>*		GetExp(){return m_pExp;}
	CComObject<CMmQvQuoteColl>*		GetQuote(){return m_pQuote;}
	CComObject<CMmQvOptRootColl>*   GetOptRoot(){return m_pOptRoot;}
	CComObject<CMmQvOptColl>*       GetOpt(){return m_pOpt;}
	CComObject<CMmQvFutCollection>* GetFut(){return m_pFut;}

	CMmQvGrpAtom*					m_pGrp;
public:

	HRESULT FinalConstruct()
	{
		try
		{
			SYSTEM_INFO si;
			GetSystemInfo(&si);
			m_lNumThreads = static_cast<long>(/*2 **/ si.dwNumberOfProcessors);
			if(m_lNumThreads <= 1)m_lNumThreads = 2;
			if(m_lNumThreads > 6) m_lNumThreads = 6;
			
			_CHK(CComObject<CMmQvExpColl>::CreateInstance(&m_pExp), _T("Fail to create expiries."));
			m_spExp.Attach(m_pExp, TRUE);

			_CHK(CComObject<CMmQvQuoteColl>::CreateInstance(&m_pQuote), _T("Fail to create quotes."));
			m_spQuote.Attach(m_pQuote, TRUE);

			_CHK(CComObject<CMmQvOptRootColl>::CreateInstance(&m_pOptRoot),  _T("Fail to create option roots."));
			m_spOptRoot.Attach(m_pOptRoot, TRUE);

			_CHK(CComObject<CMmQvOptColl>::CreateInstance(&m_pOpt), _T("Fail to create options."));
			m_spOpt.Attach(m_pOpt, TRUE);

			_CHK(CComObject<CMmQvFutCollection>::CreateInstance(&m_pFut), _T("Fail to create futures."));
			m_spFut.Attach(m_pFut, TRUE);

			_CHK(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));
			_CHK(m_spUndExch.CreateInstance(CLSID_ExchColl), _T("Fail to create underlyings exchanges."));
			_CHK(m_spOptExch.CreateInstance(CLSID_ExchColl), _T("Fail to create options exchanges."));

			m_hCalcSleepTimer = CreateWaitableTimer(NULL, TRUE, NULL);
			if (!m_hCalcSleepTimer)
				CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(GetLastError()), _T("Fail to create timer"));

			
			GUID   guidMachineId;
			RPC_CSTR pszUuid = NULL;
			::CoCreateGuid(&guidMachineId);

			if(UuidToString(&guidMachineId, &pszUuid) == RPC_S_OK)
			{   
				m_strPoolId = (LPCSTR)pszUuid;
				RpcStringFree(&pszUuid);
			}

			_CHK(CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p), _T("Unable to create custom Marshaller"));

			m_eventComplete.Attach(CreateEvent(NULL, FALSE, FALSE, ""));
			m_eventStopCalc.Attach(CreateEvent(NULL, FALSE, FALSE, ""));
			m_Pool.Start();
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvUndAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease()
	{
		m_Pool.Stop();

		m_spExp = NULL;
		m_spQuote = NULL;
		m_spBasketIndex = NULL;
		m_spVolaSrv = NULL;
		m_spOptRoot = NULL;
		m_spSynthUnd = NULL;
		m_spUndExch = NULL;
		m_spOptExch = NULL;
		m_spOpt = NULL;
		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;
		m_spFut = NULL;
		m_spActiveFuture = NULL;
		m_spHeadComponent = NULL;

		m_pUnkMarshaler.Release();
		
		if(m_hCalcSleepTimer)
		{	
			CloseHandle(m_hCalcSleepTimer);
			m_hCalcSleepTimer = 0;
		}

	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;

private:
	void _GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();
	//void _GetSyntheticRootBasketDividends(ISynthRootAtomPtr aSynthRoot,REGULAR_DIVIDENDS* pDivs, 	LONG nMaxCount) throw();
	HRESULT _GetSyntheticUnderlyingPrice(ISynthRootAtomPtr aSynthRoot, DOUBLE& dSpotBid, DOUBLE& dSpotAsk, 
		DOUBLE& dSpotLast, DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule);

	void _CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt);

	HANDLE	        m_hCalcSleepTimer;
	volatile BOOL	m_bStopCalc;

	long   m_lNumThreads;
	DWORD  m_dwAbortTime;

public:
	CLock   m_csCalcTotals;
	CHandle m_eventComplete;
	CHandle m_eventStopCalc;
	bool   IsStopCalc() const {	return m_bStopCalc?true:false;}
	HRESULT CalcFutUnderlyingOptions(LONG nCallGreekMask, LONG nPutGreekMask, MmQvIvCalcEnum enCallCalcIV,
									MmQvIvCalcEnum enPutCalcIV, EtsCalcModelTypeEnum enCalcModel,
									VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
									VARIANT_BOOL bUseTheoVolaBadMarket, VARIANT_BOOL bRecalcGreeks, 
									DOUBLE dUndPriceTolerance, 
									EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
									LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
									VARIANT_BOOL bForceRecalc, ICalculationParametrs* pParams);

	HRESULT CalcEquityOptions(LONG nCallGreekMask, LONG nPutGreekMask, MmQvIvCalcEnum enCallCalcIV,
								MmQvIvCalcEnum enPutCalcIV, EtsCalcModelTypeEnum enCalcModel,
								VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
								VARIANT_BOOL bUseTheoVolaBadMarket, VARIANT_BOOL bRecalcGreeks, 
								IMmQvOptRootColl* collSynthRoots, DOUBLE dUndPriceTolerance, 
								EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
								LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
								VARIANT_BOOL bForceRecalc, ICalculationParametrs* pParams);

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTRT_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, UndType, m_enUndType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Yield, m_dYield)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHTB, m_bIsHTB)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseCustDivs, m_bUseCustDivs)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyInShares, m_nQtyInShares)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolaSrv, m_spVolaSrv)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvExpColl*, Exp, m_spExp)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvQuoteColl*, Quote, m_spQuote)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, m_dNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDeltaEq, m_dNetDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalDelta, m_dTotalDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalGamma, m_dTotalGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalVega, m_dTotalVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalTheta, m_dTotalTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TotalRho, m_dTotalRho)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpCalendarID, m_nExpCalendarID)
	IMPLEMENT_OBJECT_PROPERTY(IIndexAtom*, BasketIndex, m_spBasketIndex)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsTraderContract, m_bIsTraderContract)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Kurt, m_dKurt)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseMidRates, m_bUseMidRates)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvOptRootColl*, OptRoot, m_spOptRoot)
	IMPLEMENT_OBJECT_PROPERTY(IMmQvUndColl*, SynthUnd, m_spSynthUnd)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IExchColl*, UndExch, m_spUndExch)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IExchColl*, OptExch, m_spOptExch)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvOptColl*, Opt, m_spOpt)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, UndPosForRates, m_dUndPosForRates)
	IMPLEMENT_BSTRT_PROPERTY(SymbolName, m_bstrSymbolName)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, PosExchID, m_nPosExchID)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptPosExchID, m_nOptPosExchID)
	IMPLEMENT_OBJECT_PROPERTY(IEtsIndexDivAtom*, Dividend, m_spDividend)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvFutCollection*, Fut, m_spFut)
	//IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, SU_Price, m_dSU_Price)

	IMPLEMENT_SIMPLE_PROPERTY( DOUBLE, FuturePrice, m_dFuturePrice)
	IMPLEMENT_SIMPLE_PROPERTY( DOUBLE, ActivePrice, m_dActivePrice)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, PrimaryExchangeID, m_nPrimaryExchangeID)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseManualActivePrice, m_bUseManualActivePrice)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsManualVol, m_fIsManualVol)


	IMPLEMENT_OBJECT_PROPERTY(IMmQvFutAtom*, ActiveFuture, m_spActiveFuture)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHead, m_bIsHead)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, PriceByHead, m_bPriceByHead)
	IMPLEMENT_OBJECT_PROPERTY(IMmQvUndAtom*, HeadComponent, m_spHeadComponent)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Coeff, m_dCoeff)

	
	STDMETHOD(CalcOptionGreeks)(IMmQvOptAtom* aOpt, IMmQvQuoteAtom* aQuote, IMmQvExpAtom* aExp, 
								IMmQvOptRootAtom* aOptRoot, LONG nGreekMask, MmQvIvCalcEnum enCalcIV, 
								EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility, 
								VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket, 
								DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
								VARIANT_BOOL bUseCustomRates, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
								ICalculationParametrs* pParams);
	STDMETHOD(CalcFutureOptionGreeks)(IMmQvOptAtom* aOpt, IMmQvQuoteAtom* aQuote, IMmQvExpAtom* aExp, 
								LONG nGreekMask, MmQvIvCalcEnum enCalcIV, 
								EtsCalcModelTypeEnum enCalcModel, VARIANT_BOOL bUseTheoVolatility, 
								VARIANT_BOOL bUseTheoVolaNoBid, VARIANT_BOOL bUseTheoVolaBadMarket, 
								DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule,
								VARIANT_BOOL bUseCustomRates, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
								ICalculationParametrs* pParams);

	STDMETHOD(CalcAllOptions)(LONG nCallGreekMask, LONG nPutGreekMask, MmQvIvCalcEnum enCallCalcIV,
							MmQvIvCalcEnum enPutCalcIV, EtsCalcModelTypeEnum enCalcModel,
							VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
							VARIANT_BOOL bUseTheoVolaBadMarket, VARIANT_BOOL bRecalcGreeks,
							IMmQvOptRootColl* collSynthRoots, DOUBLE dUndPriceTolerance, 
							EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL bUseCustomRates,
							LONG nCalcSleepFreq, LONG nCalcSleepAmt, DOUBLE dtCalcDate, VARIANT_BOOL ManualEdit,
							VARIANT_BOOL bForceRecalc, ICalculationParametrs* pParams);

	STDMETHOD(IsValidDivs)(VARIANT_BOOL CheckCustom, VARIANT_BOOL* pVal);
	STDMETHOD(StopCalc)();
	STDMETHOD(CleanUp)();
	
	STDMETHOD(get_AtmVola)(IMmQvExpAtom* pExp, DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL ManualEdit, LONG SurfaceID, DOUBLE* pVola);

	STDMETHOD(get_AtmStrike)(DOUBLE dUndPriceTolerance, enum EtsPriceRoundingRuleEnum enPriceRoundingRule, VARIANT_BOOL ManualEdit, DOUBLE* dStrike);

	STDMETHOD(SetDirty)();
	STDMETHOD(GetUnderlyingPrice)(
		DOUBLE dTolerance, 
		EtsPriceRoundingRuleEnum enPriceRound,
		EtsReplacePriceStatusEnum * penPriceStatus,
		VARIANT_BOOL *bFutureUsed, 
		DOUBLE	*pPrice);

private:
	CMmQvUndAtomThreadPool	  m_Pool;  
	std::string				  m_strPoolId;

public:
	volatile long m_nCalculations;

	void SetProcessorsNum(long nProcessors){m_Pool.SetSize(nProcessors);}
	bool  IsInCalc() const				   {return m_nCalculations >0;}


	HRESULT rawCalcOptionGreeks(
		CMmQvOptAtom*		pOpt, 
		CMmQvQuoteAtom*		pQuote, 
		const SQuoteData&	undQuote,
		CMmQvExpAtom*		pExp,
		CMmQvOptRootAtom*	pOptRoot, 
		LONG				nGreekMask, 
		MmQvIvCalcEnum		enCalcIV, 
		EtsCalcModelTypeEnum enCalcModel, 
		VARIANT_BOOL			bUseTheoVolatility, 
		VARIANT_BOOL			bUseTheoVolaNoBid, 
		VARIANT_BOOL			bUseTheoVolaBadMarket,
		DOUBLE dUndPriceTolerance, 
		EtsPriceRoundingRuleEnum enPriceRoundingRule,
		VARIANT_BOOL bUseCustomRates, 
		long					   nDivCount,
		CSafeArrayWrapper<double>& saDates,
		CSafeArrayWrapper<double>& saAmounts,
		DOUBLE					   dYield,
		DOUBLE					   dSkew,
		DOUBLE					   dKurt,
		bool                       bCalcDelta,
		DATE					   dtCalcDate,
		CVolasMapPtr			   mapVolaCache,
		VARIANT_BOOL			   ManualEdit,
		ICalculationParametrs*	   pParams
		);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvUndAtom), CMmQvUndAtom)

#endif //__MMQVUNDATOM_H__
