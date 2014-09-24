// MmRvPosAtom.h : Declaration of the CMmRvPosAtom
#ifndef __MMRVPOSATOM_H__
#define __MMRVPOSATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvSynthGreeksColl.h"
#include "MmRvFutAtom.h"
#include <map>
#include "../cv/cvCV.h"

using namespace std;
using namespace CV;

#define CV_DATAPROVIDER_BAD_ID	0xFFFFFFFF
#define CV_DATAPROVIDED_ID			0
#define CV_DATANOTSUPPORTED ( size_t) -1

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
#define CV_DP_DATA_POSDELTA		CV_DATAPROVIDED_ID + 8
#define CV_DP_DATA_POSCHANGE		CV_DATAPROVIDED_ID + 9
#define CV_DP_DATA_POSQTY			CV_DATAPROVIDED_ID + 10
#define CV_DP_DATA_POSLOTSIZE		CV_DATAPROVIDED_ID + 11
#define CV_DP_DATA_POSGAMMA		CV_DATAPROVIDED_ID + 12
#define CV_DP_DATA_POSTHETA		CV_DATAPROVIDED_ID + 13
#define CV_DP_DATA_POSRHO			CV_DATAPROVIDED_ID + 14
#define CV_DP_DATA_POSTHEOVOL		CV_DATAPROVIDED_ID + 15
#define CV_DP_DATA_POSOPTPRICE	CV_DATAPROVIDED_ID + 16
#define CV_DP_DATA_POSPNLMTM		CV_DATAPROVIDED_ID + 17
#define CV_DP_DATA_POSPNLTHEO		CV_DATAPROVIDED_ID + 18
#define CV_DP_DATA_POSRATE			CV_DATAPROVIDED_ID + 19

#define CV_DP_DATA_POSPRICETHEO	CV_DATAPROVIDED_ID + 20
#define CV_DP_DATA_POSTHETADELTA	CV_DATAPROVIDED_ID + 21
#define CV_DP_DATA_POSTHETAGAMMA	CV_DATAPROVIDED_ID + 22
#define CV_DP_DATA_POSVEGADELTA	CV_DATAPROVIDED_ID + 23
#define CV_DP_DATA_POSVEGAGAMMA	CV_DATAPROVIDED_ID + 24
#define CV_DP_DATA_POSSYNTHPRICE	CV_DATAPROVIDED_ID + 25
#define CV_DP_DATA_POSHASSYNTH	CV_DATAPROVIDED_ID + 26

#define CV_DP_DATA_POSMIV		CV_DATAPROVIDED_ID + 27
#define CV_DP_DATA_POSVOLGA		CV_DATAPROVIDED_ID + 28

#define CV_DP_FUNC_POSDTE			CV_DATAPROVIDED_ID + 1
#define CV_DP_FUNC_POSTYPE			CV_DATAPROVIDED_ID + 2
#define CV_DP_FUNC_POSISBUY		CV_DATAPROVIDED_ID + 3
#define CV_DP_FUNC_POSISSELL		CV_DATAPROVIDED_ID + 4
#define CV_DP_FUNC_POSISCALL		CV_DATAPROVIDED_ID + 5
#define CV_DP_FUNC_POSISPUT		CV_DATAPROVIDED_ID + 6


#define CV_PROVIDED_DATA( T, ID )	m_vecInternalVarID.push_back( cvProvidedID(T,(ID)))
#define CV_PROVIDED_FUNC( T, ID )	m_vecInternalFuncID.push_back( cvProvidedID(T,(ID)))

_COM_SMARTPTR_TYPEDEF(IMmRvPosAtom, IID_IMmRvPosAtom);

/////////////////////////////////////////////////////////////////////////////
//
struct __MmRvPosAtom
{
	// main info
	LONG						m_nID;
	EtsContractTypeEnum			m_enContractType;
	CComBSTR					m_bstrSymbol;

	// option related info
	EtsOptionTypeEnum			m_enOptType;
	DATE						m_dtExpiry;
	DATE						m_dtExpiryOV;
	DATE						m_dtTradingClose;
	DOUBLE						m_dStrike;
	DOUBLE						m_dVola;
	DOUBLE						m_dMIV;
	DOUBLE						m_dRate;
	DOUBLE						m_dVegaWeight;
	LONG						m_nExpiryMonth;
	LONG						m_nOptionRootID;

	// auxilary	info
	VARIANT_BOOL				m_bVisible;
	VARIANT_BOOL				m_bCalcGreeks;
	CComBSTR					m_bstrImportId;

	// price info
	DOUBLE						m_dPriceBid;
	DOUBLE						m_dPriceAsk;
	DOUBLE						m_dPriceLast;
	DOUBLE						m_dPriceClose;

	// position	info
	LONG						m_nLotSize;
	LONG						m_nQty;
	LONG						m_nQtyInShares;

	LONG						m_nQtyLTDBuy;
	LONG						m_nQtyLTDSell;									 // quantity in sell bargain
	DOUBLE						m_dPosLTDBuy;
	DOUBLE						m_dPosLTDSell;								 // total price of sell bargain

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

	// theo info
	DOUBLE						m_dPriceTheo;
	DOUBLE						m_dDelta;
	DOUBLE						m_dGamma;
	DOUBLE						m_dVega;
	DOUBLE						m_dTheta;
	DOUBLE						m_dRho;
	DOUBLE						m_dThetaDelta;
	DOUBLE						m_dThetaGamma;
	DOUBLE						m_dVegaDelta;
	DOUBLE						m_dVegaGamma;
	DOUBLE						m_dVolga;
	
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
	DOUBLE						m_dNetChange;

	// custom variables
	LONG							m_nPoolID;
	IcvRTContextPtr			m_spRTContext;
	double						m_dOptPrice;
	DOUBLE						m_dSynthUndPrice;

	// synthetic info
	VARIANT_BOOL					m_bIsSynthetic;
	IMmRvSynthGreeksCollPtr		m_spSynthGreeks;
	LONG								m_nUndID;
	//DOUBLE						m_dSynthBeta;
	EtsReplacePriceStatusEnum	m_enReplacePriceStatus;

	DOUBLE							m_dNetGamma;
	VARIANT_BOOL					m_bBadNetGamma;


	IMmRvFutAtomPtr				m_spFut;

	// simulations
	IMmRvPosAtomPtr				m_spSimulation;
	MmRvSimulationParams			m_simulationParams;

	// call <-> put position
	IMmRvPosAtomPtr				m_spOpposite;
	DOUBLE						m_dOppPriceBid;
	DOUBLE						m_dOppPriceAsk;
	DOUBLE						m_dOppPriceLast;

	DOUBLE						m_dHTBRate;

	VARIANT_BOOL				m_bIsDirty;
	VARIANT_BOOL				m_bIsRecalculated;
	VARIANT_BOOL				m_bIsFlex;

	__MmRvPosAtom() :
		m_nID(0L), m_enContractType(enCtNone),
		m_enOptType(enOtCall),
		m_dtExpiry(0.),
		m_dtExpiryOV(0),
		m_dtTradingClose(0),
		m_dStrike(0.),
		m_dVola(BAD_DOUBLE_VALUE),
		m_dMIV(BAD_DOUBLE_VALUE),
		m_dRate(0.),
		m_dVegaWeight(0.),
		m_nExpiryMonth(0L),
		m_nOptionRootID(0L),

		m_bVisible(VARIANT_FALSE),
		m_bCalcGreeks(VARIANT_FALSE),

		m_dPriceBid(BAD_DOUBLE_VALUE),
		m_dPriceAsk(BAD_DOUBLE_VALUE),
		m_dPriceLast(BAD_DOUBLE_VALUE),
		m_dPriceClose(BAD_DOUBLE_VALUE),

		m_nLotSize(0L),
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
		m_dNetChange(BAD_DOUBLE_VALUE),
		m_bIsSynthetic(VARIANT_FALSE),
		m_nUndID(0L),
		//m_dSynthBeta(0.),
		m_nPoolID(BAD_LONG_VALUE),
		m_enReplacePriceStatus(enRpsNone),
		m_dOptPrice(BAD_DOUBLE_VALUE),
		m_dSynthUndPrice( BAD_DOUBLE_VALUE),
		m_spSimulation(NULL),

		m_spOpposite(NULL),
		m_dOppPriceBid(BAD_DOUBLE_VALUE),
		m_dOppPriceAsk(BAD_DOUBLE_VALUE),
		m_dOppPriceLast(BAD_DOUBLE_VALUE),

		m_dHTBRate(BAD_DOUBLE_VALUE),

		m_bIsDirty(VARIANT_FALSE),
		m_bIsRecalculated(VARIANT_TRUE),
		m_bIsFlex(VARIANT_FALSE)
	{
		ClearValues();
		ClearValuesInShares();
		m_simulationParams.m_dDate = 0;
		m_simulationParams.m_dSpotChangePercent = 0.;
		m_simulationParams.m_dVolaChangePercent = 0.;
	}

	void ClearValues()
	{
		m_dPriceTheo = BAD_DOUBLE_VALUE;
		m_dDelta = BAD_DOUBLE_VALUE;
		m_dGamma = BAD_DOUBLE_VALUE;
		m_dVega = BAD_DOUBLE_VALUE;
		m_dTheta = BAD_DOUBLE_VALUE;
		m_dRho = BAD_DOUBLE_VALUE;
		m_dThetaDelta = BAD_DOUBLE_VALUE;
		m_dThetaGamma = BAD_DOUBLE_VALUE;
		m_dVegaDelta = BAD_DOUBLE_VALUE;
		m_dVegaGamma = BAD_DOUBLE_VALUE;
		m_dVolga = BAD_DOUBLE_VALUE;
		m_dOptPrice = BAD_DOUBLE_VALUE;
		m_dSynthUndPrice = BAD_DOUBLE_VALUE;
	}

	void ClearValuesInShares()
	{
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
	}
};


/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvPosAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvPosAtom, &CLSID_MmRvPosAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvPosAtom>,
	public IDispatchImpl<IMmRvPosAtom, &IID_IMmRvPosAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IcvDataProvider, &IID_IMmRvPosAtom, &__uuidof(__CVLib), /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvPosAtom
{
public:
	CMmRvPosAtom() : m_hCalcSleepTimer(0),
					 m_nSleepStep(0L)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVPOSATOM)

DECLARE_NOT_AGGREGATABLE(CMmRvPosAtom)

BEGIN_COM_MAP(CMmRvPosAtom)
	COM_INTERFACE_ENTRY(IMmRvPosAtom)
	COM_INTERFACE_ENTRY(IcvDataProvider)
	COM_INTERFACE_ENTRY2(IDispatch,IMmRvPosAtom)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{

		//////////////////////////////////////////////////////////////////////////
		if(m_vecInternalVarID.empty())
		{
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

		if(m_vecInternalFuncID.empty())
		{
			CV_PROVIDED_FUNC( "POSDTE", CV_DP_FUNC_POSDTE);
			CV_PROVIDED_FUNC( "POSTYPE", CV_DP_FUNC_POSTYPE);		
			CV_PROVIDED_FUNC( "POSISBUY", CV_DP_FUNC_POSISBUY);		
			CV_PROVIDED_FUNC( "POSISSELL", CV_DP_FUNC_POSISSELL);		

			CV_PROVIDED_FUNC( "POSISCALL",CV_DP_FUNC_POSISCALL );
			CV_PROVIDED_FUNC( "POSISPUT",CV_DP_FUNC_POSISPUT );
		}
	
		//////////////////////////////////////////////////////////////////////////

		m_hCalcSleepTimer = CreateWaitableTimer(NULL, TRUE, NULL);
		if (!m_hCalcSleepTimer)
			CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(GetLastError()), _T("Fail to create timer"));

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spSimulation = NULL;
		m_spRTContext = NULL;
		m_spSynthGreeks = NULL;
		m_spFut = NULL;
		m_spOpposite = NULL;

		if(m_hCalcSleepTimer)
		{	
			CloseHandle(m_hCalcSleepTimer);
			m_hCalcSleepTimer = 0;
		}
	}

private:
	void _GetBasketIndexDividends(IIndexAtomPtr spBasketIndex, REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();
	void _GetSyntheticRootBasketDividends(ISynthRootAtomPtr spSynthRoot, REGULAR_DIVIDENDS* pDivs, LONG nMaxCount) throw();
	void _CalcPnlMtm(VARIANT_BOOL bIsPnlLTD, DOUBLE dOptPriceBid, DOUBLE dOptPriceAsk, bool ReplaceWithZero);
	void _CalcPnlTheo(VARIANT_BOOL bIsPnlLTD);

	void _CalcPnlTheoExercItmDailyPrevDate(DOUBLE dUndPriceBid, DOUBLE dUndPriceAsk);
	void _CalcPnlTheoExercItmDailyToday(DOUBLE dUndPriceBid, DOUBLE dUndPriceAsk);
	void _CalcPnlTheoExercItmLTD(DOUBLE dUndPriceBid, DOUBLE dUndPriceAsk);
	void _CalcPnlTheoExercOtm(VARIANT_BOOL bIsPnlLTD);
	void _CalcPnlTheoExerc(DOUBLE dUndSpot, DOUBLE dUndPriceBid, DOUBLE dUndPriceAsk, VARIANT_BOOL bIsPnlLTD);

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

	void CMmRvPosAtom::_CalcSleep(LONG nCalcSleepFreq, LONG nCalcSleepAmt);

private:

	LONG	m_nSleepStep;
	HANDLE	m_hCalcSleepTimer;

public:
	
	// main info
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(EtsContractTypeEnum, ContractType, m_enContractType)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)

	// option related info
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(EtsOptionTypeEnum, OptType, m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DATE, Expiry, m_dtExpiry)
        IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DATE, TradingClose, m_dtTradingClose)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, Vola, m_dVola)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, MIV, m_dMIV)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, Rate, m_dRate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaWeight, m_dVegaWeight)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, ExpiryMonth, m_nExpiryMonth)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, OptionRootID, m_nOptionRootID)

	// auxilary	info
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, Visible, m_bVisible)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, CalcGreeks, m_bCalcGreeks)
	IMPLEMENT_BSTR_PROPERTY(ImportId, m_bstrImportId)

	// price info
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PriceBid,		m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PriceAsk,		m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PriceLast,	m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PriceClose,	m_dPriceClose)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, NetChange,	m_dNetChange)

	// position	info
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, LotSize, m_nLotSize)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, QtyInShares, m_nQtyInShares)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, QtyLTDBuy, m_nQtyLTDBuy)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, QtyLTDSell, m_nQtyLTDSell)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PosLTDBuy, m_dPosLTDBuy)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PosLTDSell, m_dPosLTDSell)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, QtyDailyPrevDateBuy, m_nQtyDailyPrevDateBuy)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, QtyDailyPrevDateSell, m_nQtyDailyPrevDateSell)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PosDailyPrevDateBuy, m_dPosDailyPrevDateBuy)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PosDailyPrevDateSell, m_dPosDailyPrevDateSell)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, QtyDailyTodayBuy, m_nQtyDailyTodayBuy)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, QtyDailyTodaySell, m_nQtyDailyTodaySell)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PosDailyTodayBuy, m_dPosDailyTodayBuy)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PosDailyTodaySell, m_dPosDailyTodaySell) 

	// pnl info
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlMtm, m_dPnlMtm)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlTheo, m_dPnlTheo)

	// theo info
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceTheo, m_dPriceTheo)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Delta, m_dDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Gamma, m_dGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vega, m_dVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Theta, m_dTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rho, m_dRho)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaDelta, m_dThetaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaGamma, m_dThetaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaDelta, m_dVegaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaGamma, m_dVegaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Volga, m_dVolga)
	
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG, cvPoolID, m_nPoolID)
	IMPLEMENT_OBJECT_PROPERTY(IcvRTContext*, cvRTContext, m_spRTContext)

	// synthetic info
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, IsSynthetic, m_bIsSynthetic)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvSynthGreeksColl*, SynthGreeks, m_spSynthGreeks)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, UndID, m_nUndID)
	//IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, SynthBeta, m_dSynthBeta)
	IMPLEMENT_SIMPLE_PROPERTY(EtsReplacePriceStatusEnum, ReplacePriceStatus, m_enReplacePriceStatus)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetGamma, m_dNetGamma)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetGamma, m_bBadNetGamma)

	IMPLEMENT_OBJECT_PROPERTY(IMmRvFutAtom*, Fut, m_spFut)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvPosAtom*, SimulationPos, m_spSimulation)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,		SimulationDate, m_simulationParams.m_dDate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	VolaChange,		m_simulationParams.m_dVolaChangePercent)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	SpotChange,		m_simulationParams.m_dSpotChangePercent)
	
	IMPLEMENT_OBJECT_PROPERTY(IMmRvPosAtom*, OppositePos, m_spOpposite)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, OppPriceBid,		m_dOppPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, OppPriceAsk,		m_dOppPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, OppPriceLast,		m_dOppPriceLast)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, HTBRate, m_dHTBRate)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsDirty, m_bIsDirty)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsRecalculated, m_bIsRecalculated)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsFlex, m_bIsFlex)

	STDMETHODIMP ClearValues()
	{
		m_nSleepStep = 0;
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
								VARIANT_BOOL bFitToImpFlatNoBid,
								MmRvFitToImpCurveTypeEnum enFitToImpCurve,
								EtsCalcModelTypeEnum enCalcModel,
								VARIANT_BOOL bUseTheoVolatility,
								VARIANT_BOOL bUseTheoVolaNoBid,
								VARIANT_BOOL bUseTheoVolaBadMarket,
								DOUBLE dUndPriceTolerance,
								EtsPriceRoundingRuleEnum enPriceRoundingRule,
								IMmRvUndColl* aSimulatedUndColl,
								VARIANT_BOOL bSimulate,
								VARIANT_BOOL bCalcSimulation,
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
								VARIANT_BOOL bSimulate,
								VARIANT_BOOL bCalcSimulation,
								ICalculationParametrs* pParams);

	STDMETHOD(CalcPnlMtm)(VARIANT_BOOL bIsPnlLTD,
							DOUBLE dPriceBid,
							DOUBLE dPriceAsk,
							VARIANT_BOOL bSetToTheoPnlAlso)
	{
		_CalcPnlMtm(bIsPnlLTD, dPriceBid, dPriceAsk, false);
		if(bSetToTheoPnlAlso)
			m_dPnlTheo = m_dPnlMtm;
		//if ( m_spRTContext != NULL && m_nPoolID != BAD_LONG_VALUE ) 
		//	_CHK(m_spRTContext->Recalculate(m_nPoolID));
		return S_OK;
	}

	STDMETHOD(CalcCV)(void);

	STDMETHOD(AddTrade)(IMmTradeInfoAtom* pTrade, IMmRvUndAtom* pUnd);

	// IcvDataProvider
	STDMETHOD(raw_GetData)(LONG lDataID, VARIANT* Value);
	STDMETHOD(raw_CallFunction)(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value);
	STDMETHOD(raw_Check)(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal); 

};

OBJECT_ENTRY_AUTO(__uuidof(MmRvPosAtom), CMmRvPosAtom)

#endif //__MMRVPOSATOM_H__