// MmRvUndAtom.h : Declaration of the CMmRvUndAtom
#ifndef __MMRVUNDATOM_H__
#define __MMRVUNDATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvPosColl.h"
#include "MmRvFutColl.h"
#include "MmRvGroupingColl.h"
#include <map>
#include "../cv/cvCV.h"

#include "../../../../EgarCommonLibrary/Include/OptionPriceModels/common.h"

using namespace std;
using namespace CV;

_COM_SMARTPTR_TYPEDEF(IMmRvUndAtom, IID_IMmRvUndAtom);

#define CV_DATAPROVIDER_BAD_ID	0xFFFFFFFF
#define CV_DATAPROVIDED_ID			0
#define CV_DATANOTSUPPORTED ( size_t) -1

typedef map<cvID, int>	cvIDToIDMap;
typedef map<const char*, int> cvNameToIDMap;
typedef	vector<IMmRvPosAtomPtr> cvPosVector;
typedef	vector<IMmRvPosAtomPtr>::iterator cvPosVectorIt;

/*struct cvProvidedID{
	const char* m_strName;
	int			m_DataId;
	cvProvidedID( const char* lpszName, const int id) : m_strName( lpszName), m_DataId( id ){};
};*/

#define CV_DP_FUNC_SQRT				CV_DATAPROVIDED_ID + 1
#define CV_DP_FUNC_EXP				CV_DATAPROVIDED_ID + 2

#define CV_DP_DATA_UNDBID			CV_DATAPROVIDED_ID + 1
#define CV_DP_DATA_UNDASK			CV_DATAPROVIDED_ID + 2
#define CV_DP_DATA_UNDLAST			CV_DATAPROVIDED_ID + 3
#define CV_DP_DATA_UNDMARK			CV_DATAPROVIDED_ID + 4
#define CV_DP_DATA_UNDBETA			CV_DATAPROVIDED_ID + 5
#define CV_DP_DATA_UNDPNLMTM		CV_DATAPROVIDED_ID + 6
#define CV_DP_DATA_UNDPNLTHEO		CV_DATAPROVIDED_ID + 7
#define CV_DP_DATA_UNDPRICE		CV_DATAPROVIDED_ID + 8

#define CV_DP_DATA_UNDPOS			CV_DATAPROVIDED_ID + 9
#define CV_DP_DATA_UNDOPTQTY		CV_DATAPROVIDED_ID + 10
#define CV_DP_DATA_UNDTV			CV_DATAPROVIDED_ID + 11
#define CV_DP_DATA_UNDTHETA		CV_DATAPROVIDED_ID + 12
#define CV_DP_DATA_UNDTHETADELTA	CV_DATAPROVIDED_ID + 13
#define CV_DP_DATA_UNDTHETAGAMMA	CV_DATAPROVIDED_ID + 14
#define CV_DP_DATA_UNDVEGA			CV_DATAPROVIDED_ID + 15
#define CV_DP_DATA_UNDVEGADELTA	CV_DATAPROVIDED_ID + 16
#define CV_DP_DATA_UNDVEGAGAMMA	CV_DATAPROVIDED_ID + 17
#define CV_DP_DATA_UNDWTDVEGA		CV_DATAPROVIDED_ID + 18
#define CV_DP_DATA_UNDDIVS			CV_DATAPROVIDED_ID + 19
#define CV_DP_DATA_UNDDIVSDATE		CV_DATAPROVIDED_ID + 20

#define CV_PROVIDED_DATA( T, ID )	m_vecInternalVarID.push_back( cvProvidedID(T,(ID)))
#define CV_PROVIDED_FUNC( T, ID )	m_vecInternalFuncID.push_back( cvProvidedID(T,(ID)))

/////////////////////////////////////////////////////////////////////////////
//
struct __MmRvUndAtom
{
	LONG							m_nID;
	EtsContractTypeEnum				m_enContractType;
	CComBSTR						m_bstrSymbol;

	DOUBLE							m_dYield;
	//LONG							m_nDivFreq;
	//DATE							m_dtDivDate;
	//DOUBLE						m_dDivAmt;
	VARIANT_BOOL					m_bIsAmerican;
	VARIANT_BOOL					m_bIsHTB;
	LONG							m_nExpCalendarID;

	DOUBLE							m_dSkew;
	DOUBLE							m_dKurt;

	VARIANT_BOOL					m_bCalcGreeks;
	VARIANT_BOOL					m_bCalcTotals;

	VARIANT_BOOL					m_bVolaUpdated;
	
	DOUBLE							m_dPriceBid;
	DOUBLE							m_dPriceAsk;
	DOUBLE							m_dPriceLast;
	DOUBLE							m_dPriceClose;
	DOUBLE							m_dNetChange;
	
	LONG							m_nLotSize;

	DOUBLE							m_dBeta;

	LONG							m_nQty;
	LONG							m_nOptQty;
	DOUBLE							m_dUndPosForRates;
	LONG							m_nFutQty;

	DOUBLE							m_dPnlMtm;
	DOUBLE							m_dPnlTheo;

	DOUBLE							m_dNetDelta;
	DOUBLE							m_dOptDelta;
	DOUBLE							m_dOptDeltaEq;
	DOUBLE							m_dGamma;
	DOUBLE							m_dGammaPerc;
	DOUBLE							m_dVega;
	DOUBLE							m_dWtdVega;
	DOUBLE							m_dTheta;
	DOUBLE							m_dDeltaEq;
	DOUBLE							m_dGammaEq;
	DOUBLE							m_dRho;
	DOUBLE							m_dThetaDelta;
	DOUBLE							m_dThetaGamma;
	DOUBLE							m_dVegaDelta;
	DOUBLE							m_dVegaGamma;
	DOUBLE							m_dTimeValue;
	DOUBLE							m_dBetaWtdDeltaEq;

	VARIANT_BOOL					m_bBadPnlMtm;
	VARIANT_BOOL					m_bBadPnlTheo;
	VARIANT_BOOL					m_bBadNetDelta;
	VARIANT_BOOL					m_bBadOptDelta;
	VARIANT_BOOL					m_bBadOptDeltaEq;
	VARIANT_BOOL					m_bBadGamma;
	VARIANT_BOOL					m_bBadGammaPerc;
	VARIANT_BOOL					m_bBadVega;
	VARIANT_BOOL					m_bBadWtdVega;
	VARIANT_BOOL					m_bBadTheta;
	VARIANT_BOOL					m_bBadDeltaEq;
	VARIANT_BOOL					m_bBadGammaEq;
	VARIANT_BOOL					m_bBadRho;
	VARIANT_BOOL					m_bBadThetaDelta;
	VARIANT_BOOL					m_bBadThetaGamma;
	VARIANT_BOOL					m_bBadVegaDelta;
	VARIANT_BOOL					m_bBadVegaGamma;
	VARIANT_BOOL					m_bBadTimeValue;
	VARIANT_BOOL					m_bBadBetaWtdDeltaEq;

	LONG							m_nPoolID;
	IcvRTContextPtr					m_spRTContext;
	DOUBLE							m_dPrice;

	IMmRvPosCollPtr					m_spPos;
	VARIANT_BOOL					m_bHasOptPos;
	IVolaControlPtr					m_spVolaSrv;

	IIndexAtomPtr					m_spBasketIndex;

	VARIANT_BOOL					m_bUseMidRates;

	VARIANT_BOOL					m_bHasSynthetic;

	DOUBLE							m_dNetGamma;
	VARIANT_BOOL					m_bBadNetGamma;

	VARIANT_BOOL					m_bIsDirty;
	VARIANT_BOOL					m_bIsRecalculated;

	ISynthRootCollPtr				m_spSynthRoots;
	IMmRvSynthGreeksCollPtr			m_spSynthGreeks;
	IMmRvPosCollPtr					m_spSynthPos;
	IEtsPriceProfileAtomPtr			m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr			m_spOptPriceProfile;
	EtsReplacePriceStatusEnum		m_enReplacePriceStatus;
	IEtsIndexDivAtomPtr				m_spDividend;

	IMmRvFutCollPtr					m_spFut;

	IMmRvUndAtomPtr					m_spSimulation;
	MmRvSimulationParams				m_simulationParams;

	MmRvGroupingTypeEnum			m_enGroupingType;
	IMmRvGroupingCollPtr			m_spDataGrouping;

	__MmRvUndAtom()
	{
		Clear(true);					 
		m_nPoolID = BAD_LONG_VALUE;
		m_spSimulation = NULL;
		m_simulationParams.m_dDate = 0;
		m_simulationParams.m_dSpotChangePercent = 0.;
		m_simulationParams.m_dVolaChangePercent = 0.;
		m_enGroupingType = enGroupingNone;
	}

	void Clear(bool bErase)
	{
		m_nID = 0L;
		m_enContractType = enCtNone;
		m_bstrSymbol = L"";
		m_bUseMidRates = VARIANT_FALSE;
		m_bHasOptPos = VARIANT_FALSE;

		m_dYield = 0.;
//		m_nDivFreq = 0;
//		m_dtDivDate = 0.;
//		m_dDivAmt = 0.;
		m_bIsAmerican = VARIANT_FALSE;
		m_dSkew = 0.;
		m_dKurt = 0.;

		m_bCalcGreeks = VARIANT_FALSE;
		m_bCalcTotals = VARIANT_FALSE;

		m_dPriceBid = BAD_DOUBLE_VALUE;
		m_dPriceAsk = BAD_DOUBLE_VALUE;
		m_dPriceLast = BAD_DOUBLE_VALUE;
		m_dPriceClose = BAD_DOUBLE_VALUE;
		m_nLotSize = BAD_LONG_VALUE;
		m_dNetChange = BAD_DOUBLE_VALUE;

		m_dBeta = BAD_DOUBLE_VALUE;

		m_nQty = BAD_LONG_VALUE;
		m_nOptQty = BAD_LONG_VALUE;
		m_nFutQty = BAD_LONG_VALUE;
		m_dUndPosForRates = 0.;

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
		m_dPrice = BAD_DOUBLE_VALUE;

		m_bIsDirty = VARIANT_FALSE;
		m_bIsRecalculated = VARIANT_TRUE;

		ClearValues();
	}

	void ClearValues()
	{
		m_nQty = BAD_LONG_VALUE;
		m_nOptQty = BAD_LONG_VALUE;
		m_nFutQty = BAD_LONG_VALUE;
		m_dPnlMtm = BAD_DOUBLE_VALUE;
		m_dPnlTheo = BAD_DOUBLE_VALUE;
		m_dNetDelta = BAD_DOUBLE_VALUE;
		m_dOptDelta = BAD_DOUBLE_VALUE;
		m_dOptDeltaEq = BAD_DOUBLE_VALUE;
		m_dGamma = BAD_DOUBLE_VALUE;
		m_dGammaPerc = BAD_DOUBLE_VALUE;
		m_dVega = BAD_DOUBLE_VALUE;
		m_dWtdVega = BAD_DOUBLE_VALUE;
		m_dTheta = BAD_DOUBLE_VALUE;
		m_dDeltaEq = BAD_DOUBLE_VALUE;
		m_dGammaEq = BAD_DOUBLE_VALUE;
		m_dRho = BAD_DOUBLE_VALUE;
		m_dThetaDelta = BAD_DOUBLE_VALUE;
		m_dThetaGamma = BAD_DOUBLE_VALUE;
		m_dVegaDelta = BAD_DOUBLE_VALUE;
		m_dVegaGamma = BAD_DOUBLE_VALUE;
		m_dTimeValue = BAD_DOUBLE_VALUE;
		m_dBetaWtdDeltaEq = BAD_DOUBLE_VALUE;

		m_dNetGamma = BAD_DOUBLE_VALUE;
		m_bBadNetGamma = VARIANT_FALSE;


		m_bBadPnlMtm = VARIANT_FALSE;
		m_bBadPnlTheo = VARIANT_FALSE;
		m_bBadNetDelta = VARIANT_FALSE;
		m_bBadOptDelta = VARIANT_FALSE;
		m_bBadOptDeltaEq = VARIANT_FALSE;
		m_bBadGamma = VARIANT_FALSE;
		m_bBadGammaPerc = VARIANT_FALSE;
		m_bBadVega = VARIANT_FALSE;
		m_bBadWtdVega = VARIANT_FALSE;
		m_bBadTheta = VARIANT_FALSE;
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;
		m_bBadRho = VARIANT_FALSE;
		m_bBadThetaDelta = VARIANT_FALSE;
		m_bBadThetaGamma = VARIANT_FALSE;
		m_bBadVegaDelta = VARIANT_FALSE;
		m_bBadVegaGamma = VARIANT_FALSE;
		m_bBadTimeValue = VARIANT_FALSE;
		m_bBadBetaWtdDeltaEq = VARIANT_FALSE;
		
	}

	void SetAllValuesAsBad()
	{
		m_bBadPnlMtm = VARIANT_TRUE;
		m_bBadPnlTheo = VARIANT_TRUE;
		m_bBadNetDelta = VARIANT_TRUE;
		m_bBadOptDelta = VARIANT_TRUE;
		m_bBadOptDeltaEq = VARIANT_TRUE;
		m_bBadGamma = VARIANT_TRUE;
		m_bBadGammaPerc = VARIANT_TRUE;
		m_bBadVega = VARIANT_TRUE;
		m_bBadWtdVega = VARIANT_TRUE;
		m_bBadTheta = VARIANT_TRUE;
		m_bBadDeltaEq = VARIANT_TRUE;
		m_bBadGammaEq = VARIANT_TRUE;
		m_bBadRho = VARIANT_TRUE;
		m_bBadThetaDelta = VARIANT_TRUE;
		m_bBadThetaGamma = VARIANT_TRUE;
		m_bBadVegaDelta = VARIANT_TRUE;
		m_bBadVegaGamma = VARIANT_TRUE;
		m_bBadTimeValue = VARIANT_TRUE;
		m_bBadWtdVega = VARIANT_TRUE;
		m_bBadBetaWtdDeltaEq = VARIANT_TRUE;
		m_bBadNetGamma = VARIANT_TRUE;

	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvUndAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvUndAtom, &CLSID_MmRvUndAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvUndAtom>,
	public IDispatchImpl<IMmRvUndAtom, &IID_IMmRvUndAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IcvDataProvider, &IID_IMmRvUndAtom, &__uuidof(__CVLib), /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvUndAtom
{
public:
	CMmRvUndAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVUNDATOM)

DECLARE_NOT_AGGREGATABLE(CMmRvUndAtom)

BEGIN_COM_MAP(CMmRvUndAtom)
	COM_INTERFACE_ENTRY(IMmRvUndAtom)
	COM_INTERFACE_ENTRY(IcvDataProvider)
	COM_INTERFACE_ENTRY2(IDispatch,IMmRvUndAtom)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// COM_INTERFACE_ENTRY(IDispatch)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			CComObject<CMmRvPosColl>* pObject = NULL;
			_CHK(CComObject<CMmRvPosColl>::CreateInstance(&pObject), _T("Fail to create positions."));
			m_spPos.Attach(pObject, TRUE);

			_CHK(m_spVolaSrv.CreateInstance(CLSID_VolaControl), _T("Fail to create volatility control."));
			_CHK(m_spDividend.CreateInstance(CLSID_EtsIndexDivAtom), _T("Fail to create dividend object"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndAtom, e.Error());
		}

		//////////////////////////////////////////////////////////////////////////

		if(m_vecInternalVarID.empty())
		{
			CV_PROVIDED_DATA( "UndBid",  CV_DP_DATA_UNDBID );
			CV_PROVIDED_DATA( "UndAsk",  CV_DP_DATA_UNDASK );
			CV_PROVIDED_DATA( "UndLast", CV_DP_DATA_UNDLAST );
			CV_PROVIDED_DATA( "UndMark", CV_DP_DATA_UNDMARK );
			CV_PROVIDED_DATA( "UndBeta", CV_DP_DATA_UNDBETA );
			CV_PROVIDED_DATA( "UndPnLMTM", CV_DP_DATA_UNDPNLMTM );
			CV_PROVIDED_DATA( "UndPnLTheo",CV_DP_DATA_UNDPNLTHEO);
			CV_PROVIDED_DATA( "UndPrice",CV_DP_DATA_UNDPRICE);		

			CV_PROVIDED_DATA( "UndPos",CV_DP_DATA_UNDPOS );			
			CV_PROVIDED_DATA( "UndOptPos",CV_DP_DATA_UNDOPTQTY );
			CV_PROVIDED_DATA( "UndTmValue",CV_DP_DATA_UNDTV );
			CV_PROVIDED_DATA( "UndTheta",CV_DP_DATA_UNDTHETA );
			CV_PROVIDED_DATA( "UndThetaDelta",CV_DP_DATA_UNDTHETADELTA );
			CV_PROVIDED_DATA( "UndThetaGamma",CV_DP_DATA_UNDTHETAGAMMA );
			CV_PROVIDED_DATA( "UndVega",CV_DP_DATA_UNDVEGA );
			CV_PROVIDED_DATA( "UndVegaDelta",CV_DP_DATA_UNDVEGADELTA );
			CV_PROVIDED_DATA( "UndVegaGamma",CV_DP_DATA_UNDVEGAGAMMA );
			CV_PROVIDED_DATA( "UndWtdVega",CV_DP_DATA_UNDWTDVEGA );
			CV_PROVIDED_DATA( "UndDivs", CV_DP_DATA_UNDDIVS );
			CV_PROVIDED_DATA( "UndDivsDate",CV_DP_DATA_UNDDIVSDATE );
		}
		//////////////////////////////////////////////////////////////////////////

		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spRTContext = NULL;
		m_spSimulation = NULL;
		__MmRvUndAtom::Clear(true);
	}

private:
	void _ClearSynthGreeks() throw();
	void _CalcTotalsForOptionPos(IMmRvPosAtomPtr spPos, DOUBLE dUndPriceMid) throw();
	void _GetDividendsForCV(VARIANT& dividendsValue, bool bIsDivValue = true);

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

	cvPosVector m_vecPos;


public:
	
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(EtsContractTypeEnum, ContractType, m_enContractType)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, Yield, m_dYield)
	//IMPLEMENT_SIMPLE_PROPERTY(LONG, DivFreq, m_nDivFreq)
	//IMPLEMENT_SIMPLE_PROPERTY(DATE, DivDate, m_dtDivDate)
	//IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DivAmt, m_dDivAmt)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, IsHTB, m_bIsHTB)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, ExpCalendarID, m_nExpCalendarID)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, Kurt, m_dKurt)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, CalcGreeks, m_bCalcGreeks)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, CalcTotals, m_bCalcTotals)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, VolaUpdated, m_bVolaUpdated)
	
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PriceBid, m_dPriceBid)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, NetChange, m_dNetChange)
	
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PriceAsk, m_dPriceAsk)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PriceLast, m_dPriceLast)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, PriceClose, m_dPriceClose)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, LotSize, m_nLotSize)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, Beta, m_dBeta)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, Qty, m_nQty)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, OptQty, m_nOptQty)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(DOUBLE, UndPosForRates, m_dUndPosForRates)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, FutQty, m_nFutQty)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlMtm, m_dPnlMtm)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlTheo, m_dPnlTheo)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, m_dNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, OptDelta, m_dOptDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, OptDeltaEq, m_dOptDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Gamma, m_dGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaPerc, m_dGammaPerc)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vega, m_dVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, WtdVega, m_dWtdVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Theta, m_dTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rho, m_dRho)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaDelta, m_dThetaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ThetaGamma, m_dThetaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaDelta, m_dVegaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, VegaGamma, m_dVegaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValue, m_dTimeValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, BetaWtdDeltaEq, m_dBetaWtdDeltaEq)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadPnlMtm, m_bBadPnlMtm)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadPnlTheo, m_bBadPnlTheo)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetDelta, m_bBadNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadOptDelta, m_bBadOptDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadOptDeltaEq, m_bBadOptDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGamma, m_bBadGamma)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaPerc, m_bBadGammaPerc)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVega, m_bBadVega)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadWtdVega, m_bBadWtdVega)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTheta, m_bBadTheta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, m_bBadGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRho, m_bBadRho)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadThetaDelta, m_bBadThetaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadThetaGamma, m_bBadThetaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaDelta, m_bBadVegaDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVegaGamma, m_bBadVegaGamma)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTimeValue, m_bBadTimeValue)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadBetaWtdDeltaEq, m_bBadBetaWtdDeltaEq)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, cvPoolID, m_nPoolID)
	IMPLEMENT_OBJECT_PROPERTY(IcvRTContext*, cvRTContext, m_spRTContext)

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPosColl*, Pos, m_spPos)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, HasOptPos, m_bHasOptPos)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IVolaControl*, VolaSrv, m_spVolaSrv)

	IMPLEMENT_OBJECT_PROPERTY(IIndexAtom*, BasketIndex, m_spBasketIndex)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, UseMidRates, m_bUseMidRates)

	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(VARIANT_BOOL, HasSynthetic, m_bHasSynthetic)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsDirty, m_bIsDirty)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsRecalculated, m_bIsRecalculated)

	IMPLEMENT_OBJECT_PROPERTY(ISynthRootColl*, SynthRoots, m_spSynthRoots)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvSynthGreeksColl*, SynthGreeks, m_spSynthGreeks)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvPosColl*, SynthPos, m_spSynthPos)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(EtsReplacePriceStatusEnum, ReplacePriceStatus, m_enReplacePriceStatus)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetGamma, m_dNetGamma)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetGamma, m_bBadNetGamma)

	IMPLEMENT_OBJECT_PROPERTY(IMmRvFutColl*, Fut, m_spFut)
	IMPLEMENT_OBJECT_PROPERTY(IEtsIndexDivAtom*, Dividend, m_spDividend)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvUndAtom*, SimulationUnd, m_spSimulation)

	IMPLEMENT_SIMPLE_PROPERTY(DATE,		SimulationDate, m_simulationParams.m_dDate)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	VolaChange,		m_simulationParams.m_dVolaChangePercent)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	SpotChange,		m_simulationParams.m_dSpotChangePercent)

	// greeks screen
	IMPLEMENT_SIMPLE_PROPERTY(MmRvGroupingTypeEnum,	GroupingType, m_enGroupingType)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvGroupingColl*, DataGrouping, m_spDataGrouping)

	STDMETHODIMP Clear()
	{
		m_vecPos.clear();
		__MmRvUndAtom::Clear(false);
		return S_OK;
	}

	STDMETHODIMP ClearValues()
	{
		__MmRvUndAtom::ClearValues();
		return S_OK;
	}

	STDMETHODIMP SetAllValuesAsBad()
	{
		__MmRvUndAtom::SetAllValuesAsBad();
		return S_OK;
	}

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
					VARIANT_BOOL bCalcOnlyDirty,
					ICalculationParametrs* pParams,
					LONG* pnOptUpdated,
					LONG* pnUndUpdated,
					LONG* pnFutUpdated);

	STDMETHOD(CalcWtdVega)(IEtsExpCalColl* aExpCalColl);

	STDMETHOD(CalcSyntheticUnderlying)(IMmRvUndColl* aUndColl,
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
										ICalculationParametrs* pParams);

	STDMETHOD(CalcCV)(void);

	// Greeks view
	HRESULT CalcGroupingTotals(double dUndPriceMid);

	// IcvDataProvider
	STDMETHOD(raw_GetData)(LONG lDataID, VARIANT* Value);
	STDMETHOD(raw_CallFunction)(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value);
	STDMETHOD(raw_Check)(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal); 
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvUndAtom), CMmRvUndAtom)

#endif //__MMRVUNDATOM_H__