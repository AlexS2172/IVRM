// MmRpUndAtom.h : Declaration of the CMmRpUndAtom
#ifndef __MM_RP_UND_ATOM__
#define __MM_RP_UND_ATOM__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmReports.h"
#include "MmRpOptColl.h"
#include "MmRpPosAtom.h"
#include "MmRpUndColl.h"
#include "MmRpFutColl.h"
#include "MmRpFutAtom.h"
#include "RpPrice.h"

#define __USD_SYMBOL__			L"$"
#define __USD_ID__				-1L
#define __USD_PRICE__			1.

_COM_SMARTPTR_TYPEDEF(IMmRpUndAtom, IID_IMmRpUndAtom);

struct __MmRpUndAtom
{
	LONG						m_nID;
	IIndexAtomPtr				m_spBasketIndex;
	CComBSTR					m_sTrader ;
	CComBSTR					m_sStrategy ;
	//DOUBLE						m_dDivAmt;
	//DATE						m_dtDivDate;
	//LONG						m_nDivFreq;
	DOUBLE						m_dYield;
	VARIANT_BOOL				m_vbIsAmerican;
	VARIANT_BOOL				m_vbIsHTB;
	DOUBLE						m_dKurt;
	IMmRpOptCollPtr				m_spOpt;
	
	DOUBLE						m_dSkew;
	CComBSTR					m_bstrSymbol;
	EtsContractTypeEnum			m_enUndType;
	IVolaControlPtr				m_spVolaSrv;
	VARIANT_BOOL				m_vbHasSynthetic;
	ISynthRootCollPtr			m_spSyntheticRoots;
	IMmRpPosAtomPtr				m_spPos;
	DOUBLE						m_dHV;
	LONG						m_nLotSize;
	LONG						m_nPrev5ExpAvgVol;
	LONG						m_nPrev10TrdAvgVol;
	IEtsPriceProfileAtomPtr		m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr		m_spOptPriceProfile;
	VARIANT_BOOL				m_vbVisible;
	LONG						m_nExpCalendarID;
	IEtsIndexDivAtomPtr			m_spDividend;

	IMmRpFutCollPtr				m_spFut;
	IMmRpFutAtomPtr				m_spActiveFuture;
	IMMRpPricePtr					m_spPrice ;

	__MmRpUndAtom() :
		m_nID(0L), /*m_dDivAmt(0.), m_dtDivDate(0.), m_nDivFreq(0L), */
		m_dYield(0.), m_vbIsAmerican(VARIANT_FALSE), m_vbIsHTB(VARIANT_FALSE), 
        m_dKurt(0.), m_dSkew(0.), 
		m_enUndType(enCtIndex), m_vbHasSynthetic(VARIANT_FALSE), m_dHV(0.),
		m_nLotSize(0L), m_vbVisible(VARIANT_FALSE), m_nExpCalendarID(0),
		m_sTrader(""),m_sStrategy("")
	{
	}
};

struct __MmRpSynthTotalValues
{
	LONG					m_nID;
	EtsContractTypeEnum		m_enUndType;
	DOUBLE					m_dTotalGamma;
	DOUBLE					m_dOptionsDelta;
	DOUBLE					m_dOTMDelta;
	DOUBLE					m_dPrice;
	DOUBLE					m_dHV30d;
    DOUBLE					m_dSTD;
	CComBSTR				m_bstrSymbol;
	DOUBLE					m_dTotalNetGamma;

	__MmRpSynthTotalValues() : 
		m_nID(0L), m_dTotalGamma(0.), m_dTotalNetGamma(0.), m_dOptionsDelta(0.), m_dOTMDelta(0.), 
		m_dPrice(0.), m_enUndType(enCtIndex), m_dHV30d(0.), m_dSTD(0.)
	{
	}
};

typedef std::map<LONG, __MmRpSynthTotalValues>	__SynthTotalValuesMap;

// CMmRpUndAtom

class ATL_NO_VTABLE CMmRpUndAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRpUndAtom, &CLSID_MmRpUndAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRpUndAtom>,
	public IDispatchImpl<IMmRpUndAtom, &IID_IMmRpUndAtom, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRpUndAtom
{
public:
	CMmRpUndAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRPUNDATOM)

DECLARE_NOT_AGGREGATABLE(CMmRpUndAtom)

BEGIN_COM_MAP(CMmRpUndAtom)
	COM_INTERFACE_ENTRY(IMmRpUndAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{	
			__CHECK_HRESULT(m_spOpt.CreateInstance(CLSID_MmRpOptColl), 
				_T("Failed to create options collection."));

			__CHECK_HRESULT(m_spPos.CreateInstance(CLSID_MmRpPosAtom), 
				_T("Failed to create position."));

			__CHECK_HRESULT(m_spFut.CreateInstance(CLSID_MmRpFutColl), 
				_T("Failed to create futures collection."));

			__CHECK_HRESULT(m_spDividend.CreateInstance(CLSID_EtsIndexDivAtom), _T("Fail to create div object."));
			__CHECK_HRESULT(m_spVolaSrv.CreateInstance(CLSID_VolaControl), 
				_T("Failed to create VolaControl object."));			

		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpUndAtom, e.Error());
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spBasketIndex = NULL;
		m_spOpt = NULL;
 		m_spSyntheticRoots = NULL;
		m_spPos = NULL;
		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;
		m_spDividend = NULL;
		m_spVolaSrv = NULL;
		m_spFut = NULL;
		m_spActiveFuture = NULL;
		m_spPrice = NULL;
	}

	void	_FormatSeries(DATE dtExpiry, CComBSTR& bstrSeries) throw();

	DOUBLE	_AdjustFractionalDigits(DOUBLE dValue, UINT uDigits);

	void	_GetSyntheticPrice(	ISynthRootAtom* pRootAtom, 
								IMmRpUndColl* pUndColl,
								DOUBLE& dSpotBid, 
								DOUBLE& dSpotAsk, 
								DOUBLE& dSpotLast) throw();

	LONG	_CalcOptionGreeks(EtsCalcModelTypeEnum enCalcModel, IMmRpOptAtom* pOptAtom, 
						DOUBLE dRate, DOUBLE dHTBRate, VARIANT_BOOL bUseTheoVolatility, VARIANT_BOOL bUseTheoVolaNoBid, 
						VARIANT_BOOL bUseTheoVolaBadMarket, IMmRpUndColl* pUndColl, DOUBLE dUndPriceTolerance,
						EtsPriceRoundingRuleEnum enPriceRoundingRule, GREEKS &aGreeks,
						ICalculationParametrsPtr spParams,
						DOUBLE dUndPrice =0.0, EtsOptionTypeEnum*	_penOptType = NULL, 
						bool useFuture = true, DOUBLE dUndPriceShift = 0.) throw();

	DOUBLE	_InterpolateRate(	DOUBLE dYTE,
								SAFEARRAY* psaRates, 
								SAFEARRAY* psaYTEs,
								ICalculationParametrsPtr spParams) throw();

	void	_GetBasketIndexDividends(REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();

	void	_GetSyntheticRootBasketDividends(	ISynthRootAtomPtr& spSynthRoot, 
												REGULAR_DIVIDENDS* pDivs, 
												LONG nMaxCount) throw();
	
	DOUBLE	_CalcRegularForwardPrice(	DOUBLE dSpotPrice, 
										DATE dtExpiryOV, 
										DATE tmCloseTime, 
										DATE dtNow, 
										DOUBLE dForeignRate, 
										SAFEARRAY* psaRates, 
										SAFEARRAY* psaYTEs,
										ICalculationParametrsPtr spParams) throw();	

	DOUBLE	_CalcSyntheticForwardPrice(	ISynthRootAtom* pSynthRoot,
										DOUBLE dSpotPrice, 
										DATE dtExpiryOV, 
										DATE tmCloseTime, 
										DATE dtNow, 
										DOUBLE dForeignRate, 
										SAFEARRAY* psaRates, 
										SAFEARRAY* psaYTEs,
										ICalculationParametrsPtr spParams) throw();

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ID,					m_nID)
	IMPLEMENT_OBJECT_PROPERTY(IIndexAtom*,			BasketIndex,		m_spBasketIndex)
	IMPLEMENT_BSTR_PROPERTY(						Trader,				m_sTrader)	
	IMPLEMENT_BSTR_PROPERTY(						Strategy,			m_sStrategy)	
//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				DivAmt,				m_dDivAmt)
//	IMPLEMENT_SIMPLE_PROPERTY(DATE,					DivDate,			m_dtDivDate)
//	IMPLEMENT_SIMPLE_PROPERTY(LONG,					DivFreq,			m_nDivFreq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Yield,				m_dYield)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,			IsAmerican,			m_vbIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,			IsHTB,				m_vbIsHTB)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Kurt,				m_dKurt)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpOptColl*,		Opt,		m_spOpt)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				Skew,				m_dSkew)
	IMPLEMENT_BSTR_PROPERTY(Symbol,		m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	UndType,			m_enUndType)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*,	VolaSrv,	m_spVolaSrv)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,			HasSynthetic,		m_vbHasSynthetic)
	IMPLEMENT_OBJECT_PROPERTY(ISynthRootColl*,		SyntheticRoots,		m_spSyntheticRoots)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpPosAtom*,Pos,		m_spPos)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,				HV,					m_dHV)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					LotSize,			m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					Prev5ExpAvgVol,		m_nPrev5ExpAvgVol)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					Prev10TrdAvgVol,	m_nPrev10TrdAvgVol)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*,UndPriceProfile,	m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*,OptPriceProfile,	m_spOptPriceProfile)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,			Visible,			m_vbVisible)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,					ExpCalendarID,		m_nExpCalendarID)
	IMPLEMENT_OBJECT_PROPERTY(IEtsIndexDivAtom*,	Dividend, m_spDividend )
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRpFutColl*,		Fut,		m_spFut)
	IMPLEMENT_OBJECT_PROPERTY(IMmRpFutAtom*,	ActiveFuture, m_spActiveFuture )
	IMPLEMENT_OBJECT_PROPERTY(IMMRpPrice*,	Price, m_spPrice )

	STDMETHOD(CalcGreeksSummary)(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket, 
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpGreeksSummaryColl** pRetVal);

	STDMETHOD(CalcGreeksByMonth)(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket,
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpGreeksByMonthColl** pRetVal);

	STDMETHOD(CalcGreeksByMonthExt)(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket,
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					DOUBLE dUndPriceMultiplier,
					ICalculationParametrs* pParams,
					IMmRpGreeksByMonthColl** pRetVal);

	STDMETHOD(CalcPnLs)(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket, 
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance, 
					EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpPnLColl** pRetVal);

	STDMETHOD(CalcRiskMatrix)(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket, 
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpRiskMatrixColl** pRetVal);

	STDMETHOD(CalcPosWithEarlyExercise)(EtsCalcModelTypeEnum enCalcModel, 
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket,
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					IMmRpPosWithEarlyExerciseAtom* pPosWithEarlyExerciseAtom,
					VARIANT_BOOL EAOptWithPos,
					VARIANT_BOOL EAUpComDivs,
					LONG EADaysToDiv,
					//IMmRpPosWithEarlyExerciseAtom* pPosWithEarlyExerciseAtom,
					ICalculationParametrs* pParams,
					IMmRpPosWithEarlyExerciseAtom** ppRetVal);

	STDMETHOD(CalcMatrixByStock)(DOUBLE dUndPriceTolerance, 
					EtsPriceRoundingRuleEnum enPriceRoundingRule,
					IMmRpMatrixByStockAtom* pMatrixByStockAtom,
					ICalculationParametrs* pParams,
					IMmRpMatrixByStockAtom** ppRetVal);

	STDMETHOD(CalcSynthetics)(EtsCalcModelTypeEnum enCalcModel,
					SAFEARRAY** psaRates, 
					SAFEARRAY** psaYTEs, 
					SAFEARRAY** psaHTBRates, 
					SAFEARRAY** psaHTBYTEs, 
					VARIANT_BOOL bUseTheoVolatility, 
					VARIANT_BOOL bUseTheoVolaNoBid, 
					VARIANT_BOOL bUseTheoVolaBadMarket, 
					IMmRpUndColl* pUndColl,
					DOUBLE dUndPriceTolerance, 
					EtsPriceRoundingRuleEnum enPriceRoundingRule,
					IMmRpSyntheticColl* pSyntheticColl,
					ICalculationParametrs* pParams,
					IMmRpSyntheticColl** ppRetVal);
	STDMETHOD(CalcExercisedStocks)(IMmRpUndColl* pUndColl, 
					DOUBLE dUndPriceTolerance,
			        EtsPriceRoundingRuleEnum enPriceRoundingRule,
					ICalculationParametrs* pParams,
					IMmRpExercisedStockColl** pRetVal);

	STDMETHOD(CalcRiskMatrixTotals)(IMmRpRiskMatrixColl* pVal);

	STDMETHOD(GetUnderlyingPrice)(	DOUBLE dTolerance,
												EtsPriceRoundingRuleEnum enPriceRound,
												EtsReplacePriceStatusEnum* penPriceStatus,
												VARIANT_BOOL *bFutureUsed,
												DOUBLE *pPrice);
private:

	_bstr_t	_GetOptionRoot(BSTR bsSymbol);
	void _CopyOptions(IMmRpSyntheticAtom* pSyntheticAtom, IMmRpSyntheticColl* spSyntheticColl, LONG nRootID);	
};

OBJECT_ENTRY_AUTO(__uuidof(MmRpUndAtom), CMmRpUndAtom)

#endif //__MM_RP_UND_ATOM__