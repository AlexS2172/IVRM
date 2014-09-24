// MmRvPosAtom.h : Declaration of the CMmRvPosAtom
#ifndef __MMRVPOSATOM_H__
#define __MMRVPOSATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvSynthGreeksColl.h"
#include "MmRvFutAtom.h"
#include "RvMmQuoteAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmRvUndColl, IID_IMmRvUndColl); 
_COM_SMARTPTR_TYPEDEF(IMmRvPosAtom, IID_IMmRvPosAtom);

class CMmRvAggData;

//////////////////////////////////////////////////////////////////////////
#include "../cv/cvCV.h"

using namespace std;
using namespace CV;

typedef map<cvID, int>	cvIDToIDMap;
typedef map<const char*, int> cvNameToIDMap;

struct cvProvidedID{
	const char* m_strName;
	int			m_DataId;
	cvProvidedID( const char* lpszName, const int id) : m_strName( lpszName), m_DataId( id ){};
};


#define CV_DP_DATA_POSBID			CV_DATAPROVIDED_ID + 1
#define CV_DP_DATA_POSASK			CV_DATAPROVIDED_ID + 2
#define CV_DP_DATA_POSLAST			CV_DATAPROVIDED_ID + 3
#define CV_DP_DATA_POSMARK			CV_DATAPROVIDED_ID + 4
#define CV_DP_DATA_POSSTRIKE		CV_DATAPROVIDED_ID + 5
#define CV_DP_DATA_POSTV			CV_DATAPROVIDED_ID + 6
#define CV_DP_DATA_POSVEGA			CV_DATAPROVIDED_ID + 7
#define CV_DP_DATA_POSDELTA			CV_DATAPROVIDED_ID + 8
#define CV_DP_DATA_POSCHANGE		CV_DATAPROVIDED_ID + 9
#define CV_DP_DATA_POSQTY			CV_DATAPROVIDED_ID + 10
#define CV_DP_DATA_POSLOTSIZE		CV_DATAPROVIDED_ID + 11
#define CV_DP_DATA_POSGAMMA			CV_DATAPROVIDED_ID + 12
#define CV_DP_DATA_POSTHETA			CV_DATAPROVIDED_ID + 13
#define CV_DP_DATA_POSRHO			CV_DATAPROVIDED_ID + 14
#define CV_DP_DATA_POSTHEOVOL		CV_DATAPROVIDED_ID + 15
#define CV_DP_DATA_POSOPTPRICE		CV_DATAPROVIDED_ID + 16
#define CV_DP_DATA_POSPNLMTM		CV_DATAPROVIDED_ID + 17
#define CV_DP_DATA_POSPNLTHEO		CV_DATAPROVIDED_ID + 18
#define CV_DP_DATA_POSRATE			CV_DATAPROVIDED_ID + 19

#define CV_DP_DATA_POSPRICETHEO		CV_DATAPROVIDED_ID + 20
#define CV_DP_DATA_POSTHETADELTA	CV_DATAPROVIDED_ID + 21
#define CV_DP_DATA_POSTHETAGAMMA	CV_DATAPROVIDED_ID + 22
#define CV_DP_DATA_POSVEGADELTA		CV_DATAPROVIDED_ID + 23
#define CV_DP_DATA_POSVEGAGAMMA		CV_DATAPROVIDED_ID + 24
#define CV_DP_DATA_POSSYNTHPRICE	CV_DATAPROVIDED_ID + 25
#define CV_DP_DATA_POSHASSYNTH		CV_DATAPROVIDED_ID + 26

#define CV_DP_DATA_POSMIV			CV_DATAPROVIDED_ID + 27
#define CV_DP_DATA_POSVOLGA			CV_DATAPROVIDED_ID + 28

#define CV_DP_FUNC_POSDTE			CV_DATAPROVIDED_ID + 1
#define CV_DP_FUNC_POSTYPE			CV_DATAPROVIDED_ID + 2
#define CV_DP_FUNC_POSISBUY			CV_DATAPROVIDED_ID + 3
#define CV_DP_FUNC_POSISSELL		CV_DATAPROVIDED_ID + 4
#define CV_DP_FUNC_POSISCALL		CV_DATAPROVIDED_ID + 5
#define CV_DP_FUNC_POSISPUT			CV_DATAPROVIDED_ID + 6


#define CV_PROVIDED_DATA( T, ID )	m_vecInternalVarID.push_back( cvProvidedID(T,(ID)))
#define CV_PROVIDED_FUNC( T, ID )	m_vecInternalFuncID.push_back( cvProvidedID(T,(ID)))
//////////////////////////////////////////////////////////////////////////
struct __MmRvPosAtom
{
	// aggregations
	CMmRvAggData*	AggregationsPtr_[4];		// pointers to aggregations where this position resides (underlying, future, expiry )
												// this is a REALLY WEAK pointers, so its doesn't matter of reference counter

	// custom variables
	LONG						m_nPoolID;
	IcvRTContextPtr				m_spRTContext;

	// main info
	LONG						m_nID;
	EtsContractTypeEnum			m_enContractType;
	_bstr_t						m_bstrSymbol;

	IContractPtr				m_spContract;

	_bstr_t						m_bstrSymbolOposit ;

	_bstr_t						m_bsLogEnhansement ;
	// option related info
	EtsOptionTypeEnum			m_enOptType;

	DATE						m_dtExpiry;
	DATE						m_dtExpiryOV;
	DATE						m_dtTradingClose;
	DOUBLE						m_dStrike;
	DOUBLE						m_dRate;
	DOUBLE						m_dVegaWeight;
	//LONG						m_nExpiryMonth;
	LONG						m_nOptionRootID;
	LONG						m_lExternalPosition ;

	// auxilary	info
	VARIANT_BOOL				m_bVisible;
	VARIANT_BOOL				m_bCalcGreeks;
	VARIANT_BOOL				m_bIsFlex;

	_bstr_t						m_bstrImportId;

	// position info
	DOUBLE						m_dSuPrice;
	LONG						m_nQty;
	LONG						m_nQtyInShares;

	LONG						m_nQtyLTDBuy;
	LONG						m_nQtyLTDSell;
	DOUBLE						m_dPosLTDBuy;
	DOUBLE						m_dPosLTDSell;

	LONG						m_nQtyDailyPrevDateBuy;
	LONG						m_nQtyDailyPrevDateSell;
	DOUBLE						m_dPosDailyPrevDateBuy;
	DOUBLE						m_dPosDailyPrevDateSell;

	LONG						m_nQtyDailyTodayBuy;
	LONG						m_nQtyDailyTodaySell;
	DOUBLE						m_dPosDailyTodayBuy;
	DOUBLE						m_dPosDailyTodaySell;

	// pnl info
	DOUBLE						m_dPnlMtm;
	DOUBLE						m_dPnlTheo;

	// theo in shares info
	DOUBLE						m_dDeltaInShares;
	DOUBLE						m_dGammaInShares;
	DOUBLE						m_dGammaInSharesPerc;
	DOUBLE						m_dVegaInShares;
	DOUBLE						m_dWtdVega;
	DOUBLE						m_dThetaInShares;
	DOUBLE						m_dRhoInShares;
	DOUBLE						m_dThetaDeltaInShares;
	DOUBLE						m_dThetaGammaInShares;
	DOUBLE						m_dVegaDeltaInShares;
	DOUBLE						m_dVegaGammaInShares;
	DOUBLE						m_dTimeValue;
	DOUBLE						m_dNetGamma;

	DOUBLE						m_dBetaWtdDeltaEq;
	VARIANT_BOOL				m_bBadBetaWtdDeltaEq;
	DOUBLE						m_dNetDelta;
	DOUBLE						m_dDeltaEq;

	// synthetic info
	VARIANT_BOOL				m_bIsSynthetic;
	VARIANT_BOOL				m_bIsSyntheticAggregation;
	IMmRvSynthGreeksCollPtr		m_spSynthGreeks;
	IEtsStrategyAtomPtr		    m_spStrategy;
	LONG						m_nUndID;

	VARIANT_BOOL				m_bBadNetGamma;
	VARIANT_BOOL				m_bBadNetDelta;
	VARIANT_BOOL				m_bBadDeltaEq;

	IRvMmQuoteAtomPtr           m_spQuote;    

	IMmRvFutAtomPtr				m_spFut;
	CComObject<CRvMmQuoteAtom>* m_pQuote;
	IMmRvFutAtomPtr				m_spActiveFuture;
	DOUBLE                      m_dInternalEqDelta;       

	DOUBLE						m_dNetExposure;
	VARIANT_BOOL				m_bBadNetExposure;

	DOUBLE						m_dUndPriceSave;	// price of underlying used during calculation for this position
    DOUBLE						m_dHTBRate;

	DOUBLE						m_dKEq;
	VARIANT_BOOL				m_bMultOptDltEq;
	VARIANT_BOOL				m_bUseManualActivePrice;

	IMmRvUndAtomPtr				m_spUnd;

	__MmRvPosAtom() :
		m_nPoolID(BAD_LONG_VALUE),
		m_nID(0L),
		m_enContractType(enCtNone),
		m_enOptType(enOtCall),
		m_dtExpiry(0.),
		m_dtExpiryOV(0),
		m_dtTradingClose(0),
		m_dStrike(0.),
		m_dRate(0.),
		m_dVegaWeight(0.),
		m_nOptionRootID(0L),
		m_lExternalPosition(0L),
		m_pQuote(NULL),

		m_bVisible(VARIANT_FALSE),
		m_bCalcGreeks(VARIANT_FALSE),

		m_nQty(BAD_LONG_VALUE),
		m_nQtyInShares(BAD_LONG_VALUE),

		m_nQtyLTDBuy(BAD_LONG_VALUE),
		m_nQtyLTDSell(BAD_LONG_VALUE),
		m_dPosLTDBuy(BAD_DOUBLE_VALUE),
		m_dPosLTDSell(BAD_DOUBLE_VALUE),

		m_nQtyDailyPrevDateBuy(BAD_LONG_VALUE),
		m_nQtyDailyPrevDateSell(BAD_LONG_VALUE),
		m_dPosDailyPrevDateBuy(BAD_DOUBLE_VALUE),
		m_dPosDailyPrevDateSell(BAD_DOUBLE_VALUE),

		m_nQtyDailyTodayBuy(BAD_LONG_VALUE),
		m_nQtyDailyTodaySell(BAD_LONG_VALUE),
		m_dPosDailyTodayBuy(BAD_DOUBLE_VALUE),
		m_dPosDailyTodaySell(BAD_DOUBLE_VALUE),
		m_bIsSynthetic(VARIANT_FALSE),
		m_bIsSyntheticAggregation(VARIANT_FALSE),
		m_dInternalEqDelta(BAD_DOUBLE_VALUE),
		m_nUndID(0L),
		m_dSuPrice(BAD_DOUBLE_VALUE),
		m_dUndPriceSave(BAD_DOUBLE_VALUE),
		m_spActiveFuture(NULL),
		m_dKEq(1.),
		m_bMultOptDltEq(VARIANT_FALSE),
		m_bUseManualActivePrice(VARIANT_FALSE),
		m_spUnd(NULL),
        m_dHTBRate(BAD_DOUBLE_VALUE),
		m_bIsFlex(VARIANT_FALSE)
	{
		AggregationsPtr_[0] = AggregationsPtr_[1] = AggregationsPtr_[2] = NULL;
		ClearValues();
		ClearValuesInShares();
	}

	void ClearValues()
	{
		if(m_pQuote)
			m_pQuote->ClearValues();
	}

	void ClearValuesInShares()
	{
		m_dNetDelta   = BAD_DOUBLE_VALUE;
		m_dPnlMtm = BAD_DOUBLE_VALUE;
		m_dPnlTheo = BAD_DOUBLE_VALUE;

		m_dDeltaInShares = BAD_DOUBLE_VALUE;
		m_dGammaInShares = BAD_DOUBLE_VALUE;
		m_dGammaInSharesPerc = BAD_DOUBLE_VALUE;
		m_dVegaInShares = BAD_DOUBLE_VALUE;
		m_dWtdVega = BAD_DOUBLE_VALUE;
		m_dThetaInShares = BAD_DOUBLE_VALUE;
		m_dRhoInShares = BAD_DOUBLE_VALUE;
		m_dThetaDeltaInShares = BAD_DOUBLE_VALUE;
		m_dThetaGammaInShares = BAD_DOUBLE_VALUE;
		m_dVegaDeltaInShares = BAD_DOUBLE_VALUE;
		m_dVegaGammaInShares = BAD_DOUBLE_VALUE;
		m_dTimeValue = BAD_DOUBLE_VALUE;

		m_dNetGamma = BAD_DOUBLE_VALUE;
		m_dDeltaEq  = BAD_DOUBLE_VALUE;

		m_bBadNetGamma = VARIANT_FALSE;
		m_bBadNetDelta = VARIANT_FALSE;
		m_bBadDeltaEq  = VARIANT_FALSE;

		m_dBetaWtdDeltaEq = BAD_DOUBLE_VALUE;
		m_bBadBetaWtdDeltaEq = VARIANT_FALSE;
		m_dInternalEqDelta = BAD_DOUBLE_VALUE;
		m_dUndPriceSave = BAD_DOUBLE_VALUE;

		m_dNetExposure = BAD_DOUBLE_VALUE;
		m_bBadNetExposure = VARIANT_FALSE;

	}
};
/////////////////////////////////////////////////////////////////////////////
//class AggregationStrategy{
//public:
//	virtual void AggregateTo(CMmRvAggData* aggregation, __MmRvPosAtom* position);
//};
//
//class SpotAggregationStrategy: public AggregationStrategy{
//public:
//	virtual void AggregateTo(CMmRvAggData* aggregation, __MmRvPosAtom* position);
//};
/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvPosAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvPosAtom, &CLSID_MmRvPosAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvPosAtom>,
	public IDispatchImpl<IMmRvPosAtom, &IID_IMmRvPosAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IcvDataProvider, &IID_IMmRvPosAtom, &__uuidof(__CVLib), /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvPosAtom
{
public:
	CMmRvPosAtom()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMRVPOSATOM)

	DECLARE_NOT_AGGREGATABLE(CMmRvPosAtom)

	BEGIN_COM_MAP(CMmRvPosAtom)
		COM_INTERFACE_ENTRY(IMmRvPosAtom)
		COM_INTERFACE_ENTRY(IcvDataProvider)
		COM_INTERFACE_ENTRY2(IDispatch,IMmRvPosAtom)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		CComObject<CRvMmQuoteAtom>::CreateInstance(&m_pQuote);
		m_spQuote.Attach(m_pQuote, TRUE);

		//////////////////////////////////////////////////////////////////////////
		if(m_vecInternalVarID.empty()){
			CV_PROVIDED_DATA( "PosBid", CV_DP_DATA_POSBID);
			CV_PROVIDED_DATA( "PosAsk", CV_DP_DATA_POSASK);		
			CV_PROVIDED_DATA( "PosLast", CV_DP_DATA_POSLAST);		
			CV_PROVIDED_DATA( "PosMark", CV_DP_DATA_POSMARK);		
			CV_PROVIDED_DATA( "PosStrike", CV_DP_DATA_POSSTRIKE);	
			CV_PROVIDED_DATA( "PosTV", CV_DP_DATA_POSTV);		
			CV_PROVIDED_DATA( "PosVega", CV_DP_DATA_POSVEGA);	
			CV_PROVIDED_DATA( "PosDelta", CV_DP_DATA_POSDELTA);	
			CV_PROVIDED_DATA( "PosChange", CV_DP_DATA_POSCHANGE);	
			CV_PROVIDED_DATA( "PosQty", CV_DP_DATA_POSQTY);		
			CV_PROVIDED_DATA( "PosLotSize", CV_DP_DATA_POSLOTSIZE);
			CV_PROVIDED_DATA( "PosGamma",CV_DP_DATA_POSGAMMA);	
			CV_PROVIDED_DATA( "PosTheta",CV_DP_DATA_POSTHETA);	
			CV_PROVIDED_DATA( "PosRho", CV_DP_DATA_POSRHO);		
			CV_PROVIDED_DATA( "PosTheoVol", CV_DP_DATA_POSTHEOVOL);
			CV_PROVIDED_DATA( "PosOptPrice", CV_DP_DATA_POSOPTPRICE );
			CV_PROVIDED_DATA( "PosPnLMTM", CV_DP_DATA_POSPNLMTM);		
			CV_PROVIDED_DATA( "PosPnLTheo", CV_DP_DATA_POSPNLTHEO);
			CV_PROVIDED_DATA( "PosRate", CV_DP_DATA_POSRATE );

			CV_PROVIDED_DATA( "PosPriceTheo",CV_DP_DATA_POSPRICETHEO);
			CV_PROVIDED_DATA( "PosThetaDelta",CV_DP_DATA_POSTHETADELTA);
			CV_PROVIDED_DATA( "PosThetaGamma",CV_DP_DATA_POSTHETAGAMMA);
			CV_PROVIDED_DATA( "PosVegaDelta",CV_DP_DATA_POSVEGADELTA);
			CV_PROVIDED_DATA( "PosVegaGamma",CV_DP_DATA_POSVEGAGAMMA);
			CV_PROVIDED_DATA( "PosSynthPrice",CV_DP_DATA_POSSYNTHPRICE);
			CV_PROVIDED_DATA( "PosHasSynth",CV_DP_DATA_POSHASSYNTH );

			CV_PROVIDED_DATA( "PosMIV",CV_DP_DATA_POSMIV );
			CV_PROVIDED_DATA( "PosVolga",CV_DP_DATA_POSVOLGA );
		}

		if(m_vecInternalFuncID.empty()){
			CV_PROVIDED_FUNC( "POSDTE", CV_DP_FUNC_POSDTE);
			CV_PROVIDED_FUNC( "POSTYPE", CV_DP_FUNC_POSTYPE);		
			CV_PROVIDED_FUNC( "POSISBUY", CV_DP_FUNC_POSISBUY);		
			CV_PROVIDED_FUNC( "POSISSELL", CV_DP_FUNC_POSISSELL);		
			CV_PROVIDED_FUNC( "POSISCALL",CV_DP_FUNC_POSISCALL );
			CV_PROVIDED_FUNC( "POSISPUT",CV_DP_FUNC_POSISPUT );
		}
		//////////////////////////////////////////////////////////////////////////

		return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease() 
	{
		m_spRTContext = NULL;

		m_spSynthGreeks = NULL;
		m_spFut = NULL;
		m_spActiveFuture = NULL;
		m_spContract = NULL;

		m_pQuote  = NULL;
		m_spQuote = NULL;

		m_spUnd.Detach();

		m_pUnkMarshaler.Release();
	}

	IMmRvPosAtomPtr MakeCopy(bool bStaticDataOnly, bool bAddNewQuote);
	void SimpleCopyForETS_VME (CComObject<CMmRvPosAtom>*) ;

private:
	void _GetBasketIndexDividends(IIndexAtomPtr spBasketIndex, REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();
	void _CalcPnlMtm(VARIANT_BOOL bIsPnlLTD, DOUBLE dOptPriceBid, DOUBLE dOptPriceAsk, bool ReplaceWithZero,DATE _dtCalculationDate);
	void _CalcPnlTheo(VARIANT_BOOL bIsPnlLTD, DATE _dtCalculationDate);

private:

	CComPtr<IUnknown>		m_pUnkMarshaler;

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
	///////////////////////////////////////////////////////////////////////////////////////

public:
	void _CalcOptPositionData(
		IEtsPriceProfileAtomPtr spOptPriceProfile,
		ISynthRootAtomPtr spSynthRoot,
		VARIANT_BOOL bIsPnlLTD,
		VARIANT_BOOL bUseTheoVolatility,
		VARIANT_BOOL bUseTheoVolaNoBid,
		VARIANT_BOOL bUseTheoVolaBadMarket,
		DOUBLE dUndPriceTolerance,
		EtsPriceRoundingRuleEnum enPriceRoundingRule, 
		IMmRvUndCollPtr spUndColl,
		DOUBLE dUndMidPrice,
		LONG   lUndId,
		DATE dtCalcDate,
		ICalculationParametrsPtr spParams
		);

	void _CalcFutPositionData(
		IEtsPriceProfileAtomPtr spOptPriceProfile,
		VARIANT_BOOL bIsPnlLTD,
		VARIANT_BOOL bUseTheoVolatility,
		VARIANT_BOOL bUseTheoVolaNoBid,
		VARIANT_BOOL bUseTheoVolaBadMarket,
		DOUBLE dUndPriceTolerance,
		EtsPriceRoundingRuleEnum enPriceRoundingRule, 
		DOUBLE dUndMidPrice,
		DATE dtCalcDate,
		EtsContractTypeEnum enUndType,
		ICalculationParametrsPtr spParams
		);

	bool IsVisible()const
	{
		return m_bVisible!=VARIANT_FALSE?true:false;
	}

	/*AggregationStrategy* getStrategy();*/
public:

	// main info
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, ContractType, m_enContractType)
	IMPLEMENT_BSTRT_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_BSTRT_PROPERTY(LogEnhansement , m_bsLogEnhansement )
	IMPLEMENT_SIMPLE_PROPERTY(LONG,ExternalPosition,m_lExternalPosition)
	
	// option related info
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum, OptType, m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradingClose, m_dtTradingClose)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rate, m_dRate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaWeight, m_dVegaWeight)
	//IMPLEMENT_SIMPLE_PROPERTY(LONG, ExpiryMonth, m_nExpiryMonth)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptionRootID, m_nOptionRootID)

	// auxilary	info
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Visible, m_bVisible)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsFlex, m_bIsFlex)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, CalcGreeks, m_bCalcGreeks)
	IMPLEMENT_BSTRT_PROPERTY(ImportId, m_bstrImportId)

	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DOUBLE, NetExposure,		m_dNetExposure)
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(VARIANT_BOOL, BadNetExposure,	m_bBadNetExposure)


	// position	info
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyInShares, m_nQtyInShares)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyLTDBuy, m_nQtyLTDBuy)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyLTDSell, m_nQtyLTDSell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PosLTDBuy, m_dPosLTDBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PosLTDSell, m_dPosLTDSell)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyDailyPrevDateBuy, m_nQtyDailyPrevDateBuy)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyDailyPrevDateSell, m_nQtyDailyPrevDateSell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PosDailyPrevDateBuy, m_dPosDailyPrevDateBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PosDailyPrevDateSell, m_dPosDailyPrevDateSell)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyDailyTodayBuy, m_nQtyDailyTodayBuy)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, QtyDailyTodaySell, m_nQtyDailyTodaySell)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PosDailyTodayBuy, m_dPosDailyTodayBuy)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PosDailyTodaySell, m_dPosDailyTodaySell)

	// pnl info
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlMtm, m_dPnlMtm)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlTheo, m_dPnlTheo)

	// theo in shares info
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInShares, m_dGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaInSharesPerc, m_dGammaInSharesPerc)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaInShares, m_dVegaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, WtdVega, m_dWtdVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaInShares, m_dThetaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, RhoInShares, m_dRhoInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaDeltaInShares, m_dThetaDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaGammaInShares, m_dThetaGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaDeltaInShares, m_dVegaDeltaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaGammaInShares, m_dVegaGammaInShares)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValue, m_dTimeValue)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, m_dNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetDelta, m_bBadNetDelta)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsUseManualActivePrice, m_bUseManualActivePrice)
	
	// synthetic info
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsSynthetic, m_bIsSynthetic)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvSynthGreeksColl*, SynthGreeks, m_spSynthGreeks)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsStrategyAtom*,	Strategy,  m_spStrategy)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IRvMmQuoteAtom*,	Quote,  m_spQuote)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsSyntheticAggregation, m_bIsSyntheticAggregation)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, UndID, m_nUndID)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetGamma, m_dNetGamma)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetGamma, m_bBadNetGamma)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, BetaWtdDeltaEq, m_dBetaWtdDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadBetaWtdDeltaEq, m_bBadBetaWtdDeltaEq)
    IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, HTBRate, m_dHTBRate)

	IMPLEMENT_OBJECT_PROPERTY(IMmRvFutAtom*, Fut, m_spFut)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvFutAtom*, ActiveFuture, m_spActiveFuture)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvUndAtom*, Und, m_spUnd)

	STDMETHODIMP ClearValues()
	{
		__MmRvPosAtom::ClearValues();
		return S_OK;
	}

	STDMETHODIMP ClearValuesInShares()
	{
		__MmRvPosAtom::ClearValuesInShares();
		return S_OK;
	}

	STDMETHOD(CalcOptionGreeks)(IMmRvUndColl* aUndColl,
		IMmRvUndAtom* aUnd,
		DOUBLE dUndMidPrice,
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

	STDMETHOD(CalcFutOptionGreeks)(IMmRvUndAtom* aUnd,
		DOUBLE dUndMidPrice,
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

	STDMETHOD(CalcPnlMtm)(VARIANT_BOOL bIsPnlLTD,
		DOUBLE dPriceBid,
		DOUBLE dPriceAsk,
		VARIANT_BOOL bSetToTheoPnlAlso,
		DATE _dtCalculationDate)
	{
		_CalcPnlMtm(bIsPnlLTD, dPriceBid, dPriceAsk, false,_dtCalculationDate);
		if(bSetToTheoPnlAlso)
			m_dPnlTheo = m_dPnlMtm;

		return S_OK;
	}

	STDMETHOD(GetOpositOption)(BSTR*vSymbol);
	STDMETHOD(GetCopyOfObject)(VARIANT_BOOL bNewQuote , IMmRvPosAtom**ppCopy);
	STDMETHOD(InitSyntAndFut)( IMmRvSynthGreeksColl* , IMmRvFutAtom*, IMmRvFutAtom*) ;
	STDMETHOD(IsOTM)(DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule , VARIANT_BOOL*pVal);
	STDMETHOD(GetFormatedXMLForDBUpdate)(LONG vUndID, BSTR*pVal);
	STDMETHOD(DistanceToStrike)(DOUBLE Price,DOUBLE*pDistance);

	//----------------------------------------------------------//
	HRESULT UpdateRisks(IMarketSimulationScenarioPtr SimulationScenario);
	HRESULT UpdateQuotes();
	STDMETHOD(FitToMarketVolatility)(DOUBLE Value);

	STDMETHOD(Calculate)(bool CalcLTDPnL);
	//----------------------------------------------------------//
	// custom variables
	IMPLEMENT_SIMPLE_PROPERTY(LONG, cvPoolID, m_nPoolID)
	IMPLEMENT_OBJECT_PROPERTY(IcvRTContext*, cvRTContext, m_spRTContext)

	STDMETHOD(CalcCV)(void);
	// IcvDataProvider
	STDMETHOD(raw_GetData)(LONG lDataID, VARIANT* Value);
	STDMETHOD(raw_CallFunction)(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value);
	STDMETHOD(raw_Check)(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal);
	//----------------------------------------------------------//
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvPosAtom), CMmRvPosAtom)

#endif 