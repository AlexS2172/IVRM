// MmRvGrpAtom.h : Declaration of the CMmRvGrpAtom
#ifndef __MMRVGRPATOM_H__
#define __MMRVGRPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmRisks.h"
#include "MmRvUndColl.h"
#include "MmRvGroupingTotalColl.h"
#include <map>
#include "../cv/cvCV.h"

using namespace std;
using namespace CV;

_COM_SMARTPTR_TYPEDEF(IMmRvGrpAtom, IID_IMmRvGrpAtom);

#define CV_DATAPROVIDER_BAD_ID	0xFFFFFFFF
#define CV_DATAPROVIDED_ID			0
#define CV_DATANOTSUPPORTED ( size_t) -1


#define CV_DP_FUNC_SQRT				CV_DATAPROVIDED_ID + 1
#define CV_DP_FUNC_EXP				CV_DATAPROVIDED_ID + 2
#define CV_DP_FUNC_ABS				CV_DATAPROVIDED_ID + 3
#define CV_DP_FUNC_LOG				CV_DATAPROVIDED_ID + 4
#define CV_DP_FUNC_LOG10			CV_DATAPROVIDED_ID + 5
#define CV_DP_FUNC_INT				CV_DATAPROVIDED_ID + 6

#define CV_DP_DATA_GRPPNLMTM				CV_DATAPROVIDED_ID + 1
#define CV_DP_DATA_GRPPNLTHEO				CV_DATAPROVIDED_ID + 2
#define CV_DP_DATA_GRPSTRATEGY			CV_DATAPROVIDED_ID + 3
#define CV_DP_DATA_GRPNETDELTA			CV_DATAPROVIDED_ID + 4
#define CV_DP_DATA_GRPNETDELTA$			CV_DATAPROVIDED_ID + 5
#define CV_DP_DATA_GRPVEGA					CV_DATAPROVIDED_ID + 6
#define CV_DP_DATA_GRPNETGAMMA$			CV_DATAPROVIDED_ID + 7
#define CV_DP_DATA_GRPNETGAMMA$1			CV_DATAPROVIDED_ID + 8
#define CV_DP_DATA_GRPOPTDELTA$			CV_DATAPROVIDED_ID + 9
#define CV_DP_DATA_GRPWTDVEGA				CV_DATAPROVIDED_ID + 10
#define CV_DP_DATA_GRPTHETA				CV_DATAPROVIDED_ID + 11
#define CV_DP_DATA_GRPRHO					CV_DATAPROVIDED_ID + 12
#define CV_DP_DATA_GRPINDXDELTAEQ		CV_DATAPROVIDED_ID + 13
#define CV_DP_DATA_GRPBETAWTDDELTA$		CV_DATAPROVIDED_ID + 14

typedef map<cvID, int>	cvIDToIDMap;
typedef map<const char*, int> cvNameToIDMap;

/*struct cvProvidedID{
	const char* m_strName;
	int			m_DataId;
	cvProvidedID( const char* lpszName, const int id) : m_strName( lpszName), m_DataId( id ){};
};*/

#define CV_PROVIDED_DATA( T, ID )	m_vecInternalVarID.push_back( cvProvidedID(T,(ID)))
#define CV_PROVIDED_FUNC( T, ID )	m_vecInternalFuncID.push_back( cvProvidedID(T,(ID)))

/////////////////////////////////////////////////////////////////////////////
//
struct __MmRvGrpAtom
{
	LONG							m_nID;
	LONG							m_nGroupType;
	CComBSTR						m_bstrName;

	DOUBLE							m_dPnlMtm;
	DOUBLE							m_dPnlTheo;
	DOUBLE							m_dVega;
	DOUBLE							m_dWtdVega;
	DOUBLE							m_dTheta;
	DOUBLE							m_dDeltaEq;
	DOUBLE							m_dGammaEq;
	DOUBLE							m_dRho;
	DOUBLE							m_dIdxDeltaEq;
	DOUBLE							m_dBetaWtdDelta;
	DOUBLE							m_dOptDelta;
	DOUBLE							m_dNetDelta;
	DOUBLE							m_dNetGamma;
	DOUBLE							m_dBetaWtdDeltaEq;

	VARIANT_BOOL					m_bBadPnlMtm;
	VARIANT_BOOL					m_bBadPnlTheo;
	VARIANT_BOOL					m_bBadVega;
	VARIANT_BOOL					m_bBadWtdVega;
	VARIANT_BOOL					m_bBadTheta;
	VARIANT_BOOL					m_bBadDeltaEq;
	VARIANT_BOOL					m_bBadGammaEq;
	VARIANT_BOOL					m_bBadRho;
	VARIANT_BOOL					m_bBadIdxDeltaEq;
	VARIANT_BOOL					m_bBadBetaWtdDelta;
	VARIANT_BOOL					m_bBadOptDelta;
	VARIANT_BOOL					m_bBadNetDelta;
	VARIANT_BOOL					m_bBadNetGamma;
	VARIANT_BOOL					m_bBadBetaWtdDeltaEq;

	DOUBLE							m_dTimeValue;
	DOUBLE							m_dDeltaInShares;

	VARIANT_BOOL					m_bBadTimeValue;
	VARIANT_BOOL					m_bBadDeltaInShares;

	LONG								m_nPoolID;
	IcvRTContextPtr				m_spRTContext;

	IMmRvGrpAtomPtr				m_spSimulation;

//	DOUBLE							m_dNetGamma;
//	VARIANT_BOOL					m_bBadNetGamma;

	MmRvGroupingTypeEnum			m_enGroupingType;
	IMmRvGroupingTotalCollPtr		m_spDataGrouping;

	__MmRvGrpAtom()
	{
		Clear();
		m_nPoolID = BAD_LONG_VALUE;
		m_spSimulation = NULL;
		m_enGroupingType = enGroupingNone;
	}

	void Clear()
	{
		m_nID = 0L;
		m_nGroupType = 0L;
		m_bstrName = L"";

		ClearValues();
	}

	void ClearValues()
	{
		m_dPnlMtm = BAD_DOUBLE_VALUE;
		m_dPnlTheo = BAD_DOUBLE_VALUE;
		m_dVega = BAD_DOUBLE_VALUE;
		m_dWtdVega = BAD_DOUBLE_VALUE;
		m_dTheta = BAD_DOUBLE_VALUE;
		m_dDeltaEq = BAD_DOUBLE_VALUE;
		m_dGammaEq = BAD_DOUBLE_VALUE;
		m_dRho = BAD_DOUBLE_VALUE;
		m_dIdxDeltaEq = BAD_DOUBLE_VALUE;
		m_dBetaWtdDelta = BAD_DOUBLE_VALUE;
		m_dOptDelta = BAD_DOUBLE_VALUE;
		m_dNetDelta = BAD_DOUBLE_VALUE;
		m_dNetGamma = BAD_DOUBLE_VALUE;
		m_dBetaWtdDeltaEq = BAD_DOUBLE_VALUE;

		m_bBadPnlMtm = VARIANT_FALSE;
		m_bBadPnlTheo = VARIANT_FALSE;
		m_bBadVega = VARIANT_FALSE;
		m_bBadWtdVega = VARIANT_FALSE;
		m_bBadTheta = VARIANT_FALSE;
		m_bBadDeltaEq = VARIANT_FALSE;
		m_bBadGammaEq = VARIANT_FALSE;
		m_bBadRho = VARIANT_FALSE;
		m_bBadIdxDeltaEq = VARIANT_FALSE;
		m_bBadBetaWtdDelta = VARIANT_FALSE;
		m_bBadOptDelta = VARIANT_FALSE;
		m_bBadNetDelta = VARIANT_FALSE;
		m_bBadNetGamma = VARIANT_FALSE;
		m_bBadBetaWtdDeltaEq = VARIANT_FALSE;
		
		m_dNetGamma = BAD_DOUBLE_VALUE;
		m_bBadNetGamma = VARIANT_FALSE;

		m_dTimeValue = BAD_DOUBLE_VALUE;
		m_dDeltaInShares = BAD_DOUBLE_VALUE;

		m_bBadTimeValue = VARIANT_FALSE;
		m_bBadDeltaInShares = VARIANT_FALSE;

	}
};

/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CMmRvGrpAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMmRvGrpAtom, &CLSID_MmRvGrpAtom>,
	public ISupportErrorInfoImpl<&IID_IMmRvGrpAtom>,
	public IDispatchImpl<IMmRvGrpAtom, &IID_IMmRvGrpAtom, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IcvDataProvider, &IID_IMmRvGrpAtom, &__uuidof(__CVLib), /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmRvGrpAtom
{
public:
	CMmRvGrpAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVGRPATOM)

DECLARE_NOT_AGGREGATABLE(CMmRvGrpAtom)

BEGIN_COM_MAP(CMmRvGrpAtom)
	COM_INTERFACE_ENTRY(IMmRvGrpAtom)
	COM_INTERFACE_ENTRY(IcvDataProvider)
	COM_INTERFACE_ENTRY2(IDispatch,IMmRvGrpAtom)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{

		//////////////////////////////////////////////////////////////////////////
		
		if(m_vecInternalFuncID.empty())
		{
			CV_PROVIDED_FUNC( "SQRT", CV_DP_FUNC_SQRT );
			CV_PROVIDED_FUNC( "EXP", CV_DP_FUNC_EXP );
			CV_PROVIDED_FUNC( "ABS", CV_DP_FUNC_ABS );
			CV_PROVIDED_FUNC( "LOG", CV_DP_FUNC_LOG);
			CV_PROVIDED_FUNC( "LOG10", CV_DP_FUNC_LOG10);
			CV_PROVIDED_FUNC( "INT", CV_DP_FUNC_INT);
		}

		if(m_vecInternalVarID.empty())
		{
			CV_PROVIDED_DATA( "GrpPnLMTM", CV_DP_DATA_GRPPNLMTM);
			CV_PROVIDED_DATA( "GrpPnLTheo", CV_DP_DATA_GRPPNLTHEO);
			CV_PROVIDED_DATA( "GrpStrategy", CV_DP_DATA_GRPSTRATEGY);
			CV_PROVIDED_DATA( "GrpNetDelta",CV_DP_DATA_GRPNETDELTA);
			CV_PROVIDED_DATA( "GrpNeDelta_D",CV_DP_DATA_GRPNETDELTA$);
			CV_PROVIDED_DATA( "GrpVega",CV_DP_DATA_GRPVEGA);
			CV_PROVIDED_DATA( "GrpNetGamma_D",CV_DP_DATA_GRPNETGAMMA$);
			CV_PROVIDED_DATA( "GrpNetGamma_D1",CV_DP_DATA_GRPNETGAMMA$1);
			CV_PROVIDED_DATA( "GrpDelta_D",CV_DP_DATA_GRPOPTDELTA$);
			CV_PROVIDED_DATA( "GrpWtdVega",CV_DP_DATA_GRPWTDVEGA);
			CV_PROVIDED_DATA( "GrpTheta",CV_DP_DATA_GRPTHETA);
			CV_PROVIDED_DATA( "GrpRho",CV_DP_DATA_GRPRHO);
			CV_PROVIDED_DATA( "GrpIndxDeltaEq",CV_DP_DATA_GRPINDXDELTAEQ);
			CV_PROVIDED_DATA( "GrpBetaWtdDelta_D",CV_DP_DATA_GRPBETAWTDDELTA$);
		}

		//////////////////////////////////////////////////////////////////////////
		
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spRTContext = NULL;
		m_spSimulation = NULL;
	}

private:
	void _CalcUndSynthValues(IMmRvUndCollPtr spUndColl, IMmRvUndAtomPtr spUnd, DOUBLE dIdxPrice,
				DOUBLE dUndPriceTolerance, EtsPriceRoundingRuleEnum enPriceRoundingRule) throw();


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
	
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY_WITH_SIMULATION(LONG, GroupType, m_nGroupType)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlMtm, m_dPnlMtm)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PnlTheo, m_dPnlTheo)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Vega, m_dVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, WtdVega, m_dWtdVega)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Theta, m_dTheta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaEq, m_dDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, GammaEq, m_dGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Rho, m_dRho)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, IdxDeltaEq, m_dIdxDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, BetaWtdDelta, m_dBetaWtdDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, OptDelta, m_dOptDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetDelta, m_dNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetGamma, m_dNetGamma)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, BetaWtdDeltaEq, m_dBetaWtdDeltaEq)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadPnlMtm, m_bBadPnlMtm)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadPnlTheo, m_bBadPnlTheo)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadVega, m_bBadVega)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadWtdVega, m_bBadWtdVega)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTheta, m_bBadTheta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaEq, m_bBadDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadGammaEq, m_bBadGammaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadRho, m_bBadRho)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadIdxDeltaEq, m_bBadIdxDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadBetaWtdDelta, m_bBadBetaWtdDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadOptDelta, m_bBadOptDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetDelta, m_bBadNetDelta)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetGamma, m_bBadNetGamma)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadBetaWtdDeltaEq, m_bBadBetaWtdDeltaEq)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TimeValue, m_dTimeValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DeltaInShares, m_dDeltaInShares)
	
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadTimeValue, m_bBadTimeValue)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadDeltaInShares, m_bBadDeltaInShares)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, cvPoolID, m_nPoolID)
	IMPLEMENT_OBJECT_PROPERTY(IcvRTContext*, cvRTContext, m_spRTContext)

	IMPLEMENT_OBJECT_PROPERTY(IMmRvGrpAtom*, SimulationGrp, m_spSimulation)

//	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetGamma, m_dNetGamma)
//	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, BadNetGamma, m_bNetGamma)

	// greeks screen
	IMPLEMENT_SIMPLE_PROPERTY(MmRvGroupingTypeEnum,	GroupingType, m_enGroupingType)
	IMPLEMENT_OBJECT_PROPERTY(IMmRvGroupingTotalColl*, DataGroupingTotal, m_spDataGrouping)

	STDMETHODIMP Clear()
	{
		__MmRvGrpAtom::Clear();
		return S_OK;
	}

	STDMETHODIMP ClearValues()
	{
		__MmRvGrpAtom::ClearValues();
		return S_OK;
	}

	STDMETHOD(Calc)(IMmRvUndColl* aUndColl,
				IMmRvUndAtom* aIdx,
				VARIANT_BOOL  bTotals,
				LONG nMask,
				VARIANT_BOOL bCalcGreeks,
				VARIANT_BOOL bUpdateVola,
				VARIANT_BOOL bRealtimeCalc,
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

	STDMETHOD(CalcWtdVega)(IMmRvUndColl* aUndColl,
				IEtsExpCalColl* aExpCalColl);

	// Greeks view
	HRESULT CalcGroupingTotals();

	// IcvDataProvider
	STDMETHOD(raw_GetData)(LONG lDataID, VARIANT* Value);
	STDMETHOD(raw_CallFunction)(LONG lFunctionID, SAFEARRAY** arrParameters, VARIANT *Value);
	STDMETHOD(raw_Check)(SAFEARRAY **arrSysVars, SAFEARRAY **arrSysFuncs, LONG *pRetVal); 
};

OBJECT_ENTRY_AUTO(__uuidof(MmRvGrpAtom), CMmRvGrpAtom)

#endif //__MMRVGRPATOM_H__