// MmRvUndAtom.h : Declaration of the CMmRvUndAtom
#ifndef __MMRVUNDATOM_H__
#define __MMRVUNDATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvPosColl.h"
#include "MmRvFutColl.h"
#include "MmRvExpColl.h"
#include "MmRvAggregationData.h"

_COM_SMARTPTR_TYPEDEF(IMmRvUndAtom, IID_IMmRvUndAtom);
_COM_SMARTPTR_TYPEDEF(IMmRvAggregationDataAtom, IID_IMmRvAggregationDataAtom);
//////////////////////////////////////////////////////////////////////////
#include "../cv/cvCV.h"

using namespace std;
using namespace CV;

typedef map<cvID, int>	cvIDToIDMap;
typedef map<const char*, int> cvNameToIDMap;
typedef	vector<IMmRvPosAtomPtr> cvPosVector;
typedef	vector<IMmRvPosAtomPtr>::iterator cvPosVectorIt;

#define CV_DP_FUNC_SQRT				CV_DATAPROVIDED_ID + 1
#define CV_DP_FUNC_EXP				CV_DATAPROVIDED_ID + 2

#define CV_DP_DATA_UNDBID			CV_DATAPROVIDED_ID + 1
#define CV_DP_DATA_UNDASK			CV_DATAPROVIDED_ID + 2
#define CV_DP_DATA_UNDLAST			CV_DATAPROVIDED_ID + 3
#define CV_DP_DATA_UNDMARK			CV_DATAPROVIDED_ID + 4
#define CV_DP_DATA_UNDBETA			CV_DATAPROVIDED_ID + 5
#define CV_DP_DATA_UNDPNLMTM		CV_DATAPROVIDED_ID + 6
#define CV_DP_DATA_UNDPNLTHEO		CV_DATAPROVIDED_ID + 7
#define CV_DP_DATA_UNDPRICE			CV_DATAPROVIDED_ID + 8

#define CV_DP_DATA_UNDPOS			CV_DATAPROVIDED_ID + 9
#define CV_DP_DATA_UNDOPTQTY		CV_DATAPROVIDED_ID + 10
#define CV_DP_DATA_UNDTV			CV_DATAPROVIDED_ID + 11
#define CV_DP_DATA_UNDTHETA			CV_DATAPROVIDED_ID + 12
#define CV_DP_DATA_UNDTHETADELTA	CV_DATAPROVIDED_ID + 13
#define CV_DP_DATA_UNDTHETAGAMMA	CV_DATAPROVIDED_ID + 14
#define CV_DP_DATA_UNDVEGA			CV_DATAPROVIDED_ID + 15
#define CV_DP_DATA_UNDVEGADELTA		CV_DATAPROVIDED_ID + 16
#define CV_DP_DATA_UNDVEGAGAMMA		CV_DATAPROVIDED_ID + 17
#define CV_DP_DATA_UNDWTDVEGA		CV_DATAPROVIDED_ID + 18
#define CV_DP_DATA_UNDDIVS			CV_DATAPROVIDED_ID + 19
#define CV_DP_DATA_UNDDIVSDATE		CV_DATAPROVIDED_ID + 20

#define CV_PROVIDED_DATA( T, ID )	m_vecInternalVarID.push_back( cvProvidedID(T,(ID)))
#define CV_PROVIDED_FUNC( T, ID )	m_vecInternalFuncID.push_back( cvProvidedID(T,(ID)))

//////////////////////////////////////////////////////////////////////////
//
struct __MmRvUndAtom
	: public CMmRvAggData
{
	LONG							m_nID;
	EtsContractTypeEnum				m_enContractType;
	_bstr_t							m_bstrSymbol;

	DOUBLE							m_dYield;

	IContractPtr					m_spContract;

    VARIANT_BOOL                    m_bShouldMultOptDeltaEq;
	DOUBLE                          m_dKOptDeltaEq;

	VARIANT_BOOL					m_bIsAmerican;
	VARIANT_BOOL					m_bIsHTB;

	DOUBLE							m_dSkew;
	DOUBLE							m_dKurt;

	VARIANT_BOOL					m_bCalcGreeks;
	VARIANT_BOOL					m_bCalcTotals;

	VARIANT_BOOL					m_bVolaUpdated;
	DOUBLE							m_dSuPrice;

	LONG							m_nLotSize;

	IMmRvPosCollPtr					m_spPos;
	IMmRvExpCollPtr                 m_spExp;
	VARIANT_BOOL					m_bHasOptPos;
	IVolaControlPtr					m_spVolaSrv;

	IIndexAtomPtr					m_spBasketIndex;

	VARIANT_BOOL					m_bUseMidRates;

	VARIANT_BOOL					m_bHasSynthetic;

	ISynthRootCollPtr				m_spSynthRoots;
	IMmRvSynthGreeksCollPtr			m_spSynthGreeks;
	IMmRvPosCollPtr					m_spSynthPos;
	IEtsPriceProfileAtomPtr			m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr			m_spOptPriceProfile;
	EtsReplacePriceStatusEnum		m_enReplacePriceStatus;
	IEtsIndexDivAtomPtr				m_spDividend;

	IMmRvFutCollPtr					m_spFut;
	IMmRvFutAtomPtr					m_spActiveFuture;

	IMmRvPricePtr					m_spPrice;
	CComObject<CMmRvPrice>*			m_pPrice;

	IMmRvPricePtr					m_spSuPrice;
	CComObject<CMmRvPrice>*			m_pSuPrice;

	LONG							m_nPrimaryExchangeID;
	_bstr_t							m_bstrPrimaryExchangeCode;

	IMmRvUndAtomPtr					m_spHeadComponent;
	DOUBLE							m_dCoeff;
	VARIANT_BOOL					m_bPriceByHead;
	VARIANT_BOOL					m_bIsHead;

	LONG							m_nPoolID;
	IcvRTContextPtr					m_spRTContext;

	void Clear(bool bErase, bool bClearPrice)
	{
		if(bClearPrice && m_pPrice)
			m_pPrice->Clear();
		ClearAggregatedValues();
	}

	__MmRvUndAtom()
		:CMmRvAggData(CMmRvAggData::enUndAgg)
		,m_pPrice(NULL)
		,m_pSuPrice(NULL)
	{
		Clear(true, true);
		Clear(true);

		m_nPoolID = BAD_LONG_VALUE;
	}

	void Clear(bool bErase)
	{
		Clear(bErase, true);

		m_nID = 0L;
		m_enContractType = enCtNone;
		m_bstrSymbol = L"";
		m_bUseMidRates = VARIANT_FALSE;
		m_bHasOptPos = VARIANT_FALSE;
		m_nPrimaryExchangeID = 0L;
		m_bstrPrimaryExchangeCode = L"";

		m_dYield = 0.;
		m_bIsAmerican = VARIANT_FALSE;
		m_dSkew = 0.;
		m_dKurt = 0.;

		m_bCalcGreeks = VARIANT_FALSE;
		m_bCalcTotals = VARIANT_FALSE;

		m_dSuPrice = BAD_DOUBLE_VALUE;
		m_nLotSize = BAD_LONG_VALUE;

		m_bHasSynthetic = VARIANT_FALSE;

		if(!bErase && m_spPos != NULL)
			m_spPos->Clear();
		else
			m_spPos = NULL;

		if(!bErase && m_spVolaSrv != NULL)
			m_spVolaSrv->CleanUp();
		else
			m_spVolaSrv = NULL;

		m_spBasketIndex = NULL;
		m_spSynthRoots = NULL;
		m_spSynthGreeks = NULL;
		m_spSynthPos = NULL;

		m_spUndPriceProfile = NULL;
		m_spOptPriceProfile = NULL;
		m_spFut = NULL;

		m_enReplacePriceStatus = enRpsNone;
		m_spDividend = NULL;
		m_spActiveFuture = NULL;

		m_spHeadComponent = NULL;
		m_bIsHead	=	VARIANT_FALSE;
		m_bPriceByHead = VARIANT_FALSE;
		m_dCoeff = 1.0;

		m_spContract = NULL;
		m_spRTContext = NULL;
		
		ClearAggregatedValues();
	}

	// adds position to aggregations under underlying
	void AddPosToAggregations(MmRvPosAtomPtr& pPos);

	void AggregatePositionQuantity(CMmRvPosAtom* pPos);
	void AggregatePosition(CMmRvPosAtom* pPos, double underlyingPtrice,DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule);
	void AggregatePositionDeltas(CMmRvPosAtom* pPos);
	void AggregatePositionGammas(CMmRvPosAtom* pPos, double underlyingPtrice,DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule);
	void AggregatePositionPnLs(CMmRvPosAtom* pPos);
	void AggregatePositionVegas(CMmRvPosAtom* pPos);
	void AggregatePositionThetas(CMmRvPosAtom* pPos);
	void AggregatePositionRhos(CMmRvPosAtom* pPos);
	void AggregatePositionTmVal(CMmRvPosAtom* pPos);
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvUndAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvUndAtom, &CLSID_MmRvUndAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvUndAtom>,
	public IDispatchImpl<IMmRvUndAtom, &IID_IMmRvUndAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IcvDataProvider, &IID_IMmRvUndAtom, &__uuidof(__CVLib), /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IMmRvAggregationDataAtom, &IID_IMmRvAggregationDataAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvUndAtom
{
public:
	

	CMmRvUndAtom()
		:m_pPos(NULL)
		,m_dNetExposureAUM(BAD_DOUBLE_VALUE)
	{
		pUnd_ = this;
		__MmRvUndAtom::Clear(true, false);

		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVUNDATOM)

	//DECLARE_NOT_AGGREGATABLE(CMmRvUndAtom)

	BEGIN_COM_MAP(CMmRvUndAtom)
		COM_INTERFACE_ENTRY(IMmRvUndAtom)
		COM_INTERFACE_ENTRY(IMmRvAggregationDataAtom)	
		COM_INTERFACE_ENTRY(IcvDataProvider)
		COM_INTERFACE_ENTRY2(IDispatch,IMmRvUndAtom)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		try
		{
			_CHK(CComObject<CMmRvPosColl>::CreateInstance(&m_pPos), _T("Fail to create positions."));
			m_spPos.Attach(m_pPos, TRUE);

			_CHK(CComObject<CMmRvExpColl>::CreateInstance(&m_pExp), _T("Fail to create Expirations."));
			m_spExp.Attach(m_pExp, TRUE);

			_CHK(CComObject<CMmRvPrice>::CreateInstance(&m_pPrice), _T("Fail to create Price Info."));
			m_spPrice.Attach(m_pPrice, TRUE);

			_CHK(CComObject<CMmRvPrice>::CreateInstance(&m_pSuPrice), _T("Fail to create Price Info."));
			m_spSuPrice.Attach(m_pSuPrice, TRUE);

			_CHK(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));
			_CHK(m_spDividend.CreateInstance(CLSID_EtsIndexDivAtom), _T("Fail to create dividend object"));

			//////////////////////////////////////////////////////////////////////////
			if(m_vecInternalVarID.empty()){
				CV_PROVIDED_DATA( "UndBid",			CV_DP_DATA_UNDBID );
				CV_PROVIDED_DATA( "UndAsk",			CV_DP_DATA_UNDASK );
				CV_PROVIDED_DATA( "UndLast",		CV_DP_DATA_UNDLAST );
				CV_PROVIDED_DATA( "UndMark",		CV_DP_DATA_UNDMARK );
				CV_PROVIDED_DATA( "UndBeta",		CV_DP_DATA_UNDBETA );
				CV_PROVIDED_DATA( "UndPnLMTM",		CV_DP_DATA_UNDPNLMTM );
				CV_PROVIDED_DATA( "UndPnLTheo",		CV_DP_DATA_UNDPNLTHEO);
				CV_PROVIDED_DATA( "UndPrice",		CV_DP_DATA_UNDPRICE);		

				CV_PROVIDED_DATA( "UndPos",			CV_DP_DATA_UNDPOS );			
				CV_PROVIDED_DATA( "UndOptPos",		CV_DP_DATA_UNDOPTQTY );
				CV_PROVIDED_DATA( "UndTmValue",		CV_DP_DATA_UNDTV );
				CV_PROVIDED_DATA( "UndTheta",		CV_DP_DATA_UNDTHETA );
				CV_PROVIDED_DATA( "UndThetaDelta",	CV_DP_DATA_UNDTHETADELTA );
				CV_PROVIDED_DATA( "UndThetaGamma",	CV_DP_DATA_UNDTHETAGAMMA );
				CV_PROVIDED_DATA( "UndVega",		CV_DP_DATA_UNDVEGA );
				CV_PROVIDED_DATA( "UndVegaDelta",	CV_DP_DATA_UNDVEGADELTA );
				CV_PROVIDED_DATA( "UndVegaGamma",	CV_DP_DATA_UNDVEGAGAMMA );
				CV_PROVIDED_DATA( "UndWtdVega",		CV_DP_DATA_UNDWTDVEGA );
				CV_PROVIDED_DATA( "UndDivs",		CV_DP_DATA_UNDDIVS );
				CV_PROVIDED_DATA( "UndDivsDate",	CV_DP_DATA_UNDDIVSDATE );
			};
			//////////////////////////////////////////////////////////////////////////

			return 
				CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
		}
		return S_OK;
	}

	void FinalRelease()
	{
		m_vecPos.clear();

		__MmRvUndAtom::Clear(true);
		
		m_pUnkMarshaler.Release();
	}

	static void Date2Text( DATE dt, _bstr_t& OutString )
	{
		EgLib::vt_date		vrntDate(dt);
		wchar_t	wchBuf[100];

		if ( 0 == static_cast<long>(dt) ) 
			_snwprintf_s(wchBuf, sizeof wchBuf/sizeof(wchar_t) ,L"");
		else
			_snwprintf_s(wchBuf, sizeof wchBuf/sizeof(wchar_t) ,L"%s%02d", bsMonth[vrntDate.get_month()-1], vrntDate.get_year()-2000 );
		OutString = wchBuf;
	}

private:
	static const WCHAR* bsMonth[12];
	void _ClearSynthGreeks() throw();
	void _CalcTotalsForOptionPos(CMmRvPosAtom* pPos) throw();

	CComPtr<IUnknown>			m_pUnkMarshaler;

	DOUBLE						m_dNetExposureAUM;

public:
	CComObject<CMmRvPosColl>* m_pPos;
	CComObject<CMmRvExpColl>* m_pExp;

	///////////////////////////////////////////////////////////////////////////////////////
	// IcvDataProvider 
	static vector<cvProvidedID>		m_vecInternalVarID;
	static cvIDToIDMap					m_mapSupportedCVID;
	static vector<cvProvidedID>		m_vecInternalFuncID;
	static cvIDToIDMap					m_mapSupportedFuncID;

	size_t FindDataByName( const string& strDataName )const{
		for ( size_t s = 0; s < m_vecInternalVarID.size(); s++ ) {
			if ( strDataName == m_vecInternalVarID[s].m_strName) 
				return s;
		}		

		return CV_DATANOTSUPPORTED;
	}

	size_t FindFuncByName( const string& strFuncName )const{
		for ( size_t s = 0; s < m_vecInternalFuncID.size(); s++ ) {
			if ( strFuncName == m_vecInternalFuncID[s].m_strName) 
				return s;
		}		

		return CV_DATANOTSUPPORTED;
	}

	int	GetDataID( cvID _cvID){
		cvIDToIDMap::iterator itr = m_mapSupportedCVID.find(_cvID); 
		if ( itr == m_mapSupportedCVID.end() ) 
			return CV_DATAPROVIDER_BAD_ID;
		else
			return itr->second;
	}

	int	GetFuncID( cvID _cvID){
		cvIDToIDMap::iterator itr = m_mapSupportedFuncID.find(_cvID); 
		if ( itr == m_mapSupportedFuncID.end() ) 
			return CV_DATAPROVIDER_BAD_ID;
		else
			return itr->second;
	}

	cvPosVector m_vecPos;

	///////////////////////////////////////////////////////////////////////////////////////

public:
	void VisiblePositions(bool bVisible)
	{
		if(m_pPos)
		{
			CMmRvPosColl::CollType::iterator itr = m_pPos->m_collRef.begin();
			CMmRvPosColl::CollType::iterator itrEnd = m_pPos->m_collRef.end();
			for(;itr!=itrEnd;++itr)
			{
				CMmRvPosAtom* pPosAtom = dynamic_cast<CMmRvPosAtom*>(itr->second->second);
				if(pPosAtom)
					pPosAtom->m_bVisible = bVisible?VARIANT_TRUE:VARIANT_FALSE;
			}
		}
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, ContractType, m_enContractType)

	STDMETHODIMP put_Symbol(BSTR NewVal){
		ObjectLock lock(this);
		m_bstrSymbol = NewVal;
		Name_ = m_bstrSymbol;
		return S_OK;
	}
	
	STDMETHODIMP get_Symbol(BSTR* pVal){
		if(!pVal) return E_POINTER;
		if(*pVal)
			::SysFreeString(*pVal);
		ObjectLock lock(this);
		*pVal = m_bstrSymbol.copy();
		return S_OK;
	}

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Yield, m_dYield)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, bShouldMultOptDeltaEq, m_bShouldMultOptDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, dKOptDeltaEq, m_dKOptDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHTB, m_bIsHTB)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Kurt, m_dKurt)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, CalcGreeks, m_bCalcGreeks)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, CalcTotals, m_bCalcTotals)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, VolaUpdated, m_bVolaUpdated)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Beta, Beta_)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Qty, UndPos_)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptQty, OptQty_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, UndPosForRates, UndPosForRates_)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, FutQty, FutQty_)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlMtm, PnlMTM_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlTheo, PnLTheo_)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, NetDlt_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, OptDelta, OptDlt_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Gamma, Gma1$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaPerc, Gma1P_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vega, Vga$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, WtdVega, WtdVga$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Theta, Tht$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, NetDlt$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, NetGma$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rho, Rho$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaDelta, ThtDlt_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaGamma, ThtGma_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaDelta, VgaDlt_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaGamma, VgaGma_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValue, TmVal_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, BetaWtdDeltaEq, BetaWtdDelta$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValueInShares, TmValInShares_)


	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadPnlMtm, BadPnlMtm_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadPnlTheo, BadPnlTheo_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetDelta, BadNetDelta_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadOptDelta, BadOptDelta_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGamma, BadGma1$_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaPerc, BadGma1P_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVega, BadVga$_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadWtdVega, BadWtdVga$_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTheta, BadTht$_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, BadNetDlt$_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, BadNetGma$_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRho, BadRho$_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadThetaDelta, BadThtDlt_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadThetaGamma, BadThtGma_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaDelta, BadVgaDlt_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaGamma, BadVgaGma_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTimeValue, BadTmVal_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadBetaWtdDeltaEq, BadBetaWtdDelta$_)

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPosColl*, Pos, m_spPos)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HasOptPos, m_bHasOptPos)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolaSrv, m_spVolaSrv)

	IMPLEMENT_OBJECT_PROPERTY(IIndexAtom*, BasketIndex, m_spBasketIndex)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseMidRates, m_bUseMidRates)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HasSynthetic, m_bHasSynthetic)

	IMPLEMENT_OBJECT_PROPERTY(ISynthRootColl*, SynthRoots, m_spSynthRoots)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvSynthGreeksColl*, SynthGreeks, m_spSynthGreeks)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvPosColl*, SynthPos, m_spSynthPos)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_SIMPLE_PROPERTY(EtsReplacePriceStatusEnum, ReplacePriceStatus, m_enReplacePriceStatus)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetGamma, NetGma_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetGamma, BadNetGma_)

	IMPLEMENT_OBJECT_PROPERTY(IMmRvFutColl*, Fut, m_spFut)
	IMPLEMENT_OBJECT_PROPERTY(IEtsIndexDivAtom*, Dividend, m_spDividend)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvFutAtom*, ActiveFuture, m_spActiveFuture)

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPrice*, Price,   m_spPrice);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPrice*, SuPrice, m_spSuPrice);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvExpColl*, ExpColl, m_spExp);

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsSyntheticAggregation,	 m_bIsSyntheticAggregation);

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadOptDeltaEq,  BadOptDlt$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		OptDeltaEq,		OptDlt$_)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, SuPrice, m_dSuPrice)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetExposure,		NetExposure_)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetExposure,		BadNetExposure_)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, PrimaryExchangeID, m_nPrimaryExchangeID)

	IMPLEMENT_OBJECT_PROPERTY(IMmRvUndAtom*, HeadComponent, m_spHeadComponent)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsHead,  m_bIsHead)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, PriceByHead,  m_bPriceByHead)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Coeff,  m_dCoeff)

	STDMETHODIMP put_PrimaryExchangeCode(BSTR NewVal){
		ObjectLock lock(this);
		m_bstrPrimaryExchangeCode = NewVal;
		Name_ = m_bstrPrimaryExchangeCode;
		return S_OK;
	}

	STDMETHODIMP get_PrimaryExchangeCode(BSTR* pVal){
		if(!pVal) return E_POINTER;
		if(*pVal)
			::SysFreeString(*pVal);
		ObjectLock lock(this);
		*pVal = m_bstrPrimaryExchangeCode.copy();
		return S_OK;
	}

	STDMETHODIMP Clear()
	{
		__MmRvUndAtom::Clear(false);
		return S_OK;
	}

	STDMETHODIMP ClearValues()
	{
		
		ClearAggregatedValues();
		return S_OK;
	}

	STDMETHODIMP SetAllValuesAsBad()
	{
		SetAllValuesAsBad();
		return S_OK;
	}

	void  SetNetExposureAUM(DOUBLE dNewVal)
	{
		m_dNetExposureAUM = dNewVal;
	}

	DOUBLE GetNetExposureAUM() const { return m_dNetExposureAUM;}

	STDMETHOD(GetSyntheticUnderlyingPrice)(IMmRvUndColl* aUndColl,
		ISynthRootAtom* aSynthRoot,
		DOUBLE* pdSynthBid,
		DOUBLE* pdSynthAsk,
		DOUBLE* pdSynthLast);

	STDMETHOD(Calc)(IMmRvUndColl* aUndColl,
		LONG nMask,
		VARIANT_BOOL bCalcGreeks,
		VARIANT_BOOL bUpdateVola,
		VARIANT_BOOL bRealtimeCalc,
		VARIANT_BOOL bClearSynthGreeks,
		IEtsProcessDelay* aDelay,
		VARIANT_BOOL bIsPnlLTD,
		EtsCalcModelTypeEnum enCalcModel,
		VARIANT_BOOL bUseTheoVolatility,
		VARIANT_BOOL bUseTheoVolaNoBid,
		VARIANT_BOOL bUseTheoVolaBadMarket,
		DOUBLE dUndPriceTolerance,
		EtsPriceRoundingRuleEnum enPriceRoundingRule,
		LONG* pnOptUpdated,
		LONG* pnUndUpdated,
		LONG* pnFutUpdated,
		DATE dtCalcDate,
		ICalculationParametrs* pParams);

	HRESULT CalcEx( IMmRvUndColl* aUndColl,
					LONG nMask,
					VARIANT_BOOL bCalcGreeks,
					VARIANT_BOOL bUpdateVola,
					VARIANT_BOOL bRealtimeCalc,
					VARIANT_BOOL bClearSynthGreeks,
					IEtsProcessDelay* aDelay,
					VARIANT_BOOL bIsPnlLTD,
					EtsCalcModelTypeEnum enCalcModel,
					VARIANT_BOOL bUseTheoVolatility,
					VARIANT_BOOL bUseTheoVolaNoBid,
					VARIANT_BOOL bUseTheoVolaBadMarket,
					DOUBLE dUndPriceTolerance,
					EtsPriceRoundingRuleEnum enPriceRoundingRule,
					LONG* pnOptUpdated,
					LONG* pnUndUpdated,
					LONG* pnFutUpdated,
					DATE dtCalcDate,
					ICalculationParametrs* pParams,
					IMarketSimulationScenarioPtr SimulationScenario);

	STDMETHOD(CalcWtdVega)(IEtsExpCalColl* aExpCalColl);
	STDMETHOD(FindPosition)(DOUBLE dStrike, DATE dtExpiration, EtsOptionTypeEnum enType, IMmRvPosAtom** pPosAtom);

	STDMETHOD(CalcSyntheticUnderlying)(IMmRvUndColl* aUndColl,
		LONG nMask,
		VARIANT_BOOL bIsPnlLTD,
		EtsCalcModelTypeEnum enCalcModel,
		VARIANT_BOOL bUseTheoVolatility,
		VARIANT_BOOL bUseTheoVolaNoBid,
		VARIANT_BOOL bUseTheoVolaBadMarket,
		DOUBLE dUndPriceTolerance,
		EtsPriceRoundingRuleEnum enPriceRoundingRule,
		DATE dtCalcDate,
		ICalculationParametrs* pParams);

	STDMETHOD(GetUnderlyingPrice)(DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound,EtsReplacePriceStatusEnum * penPriceStatus, VARIANT_BOOL *bFutureUsed, DOUBLE *pPrice);

	STDMETHOD(ClearRatios)();
	STDMETHOD(UpdateQuotes)();
	STDMETHOD(UpdateRisks)();

	////////////////////////////////////////////////////////////////////////
	IMPLEMENT_SIMPLE_PROPERTY(LONG, cvPoolID, m_nPoolID)
	IMPLEMENT_OBJECT_PROPERTY(IcvRTContext*, cvRTContext, m_spRTContext)

	STDMETHOD(CalcCV)(void);
	void _GetDividendsForCV(VARIANT& dividendsValue, bool bIsDivValue = true);

	// IcvDataProvider
	STDMETHOD(raw_GetData)(LONG lDataID, VARIANT* Value);
	STDMETHOD(raw_CallFunction)(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value);
	STDMETHOD(raw_Check)(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvUndAtom), CMmRvUndAtom)

#endif //__MMRVUNDATOM_H__