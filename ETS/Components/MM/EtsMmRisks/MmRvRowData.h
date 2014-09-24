// MmRvRowData.h : Declaration of the CMmRvRowData

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"
#include "MmRvUndAtom.h"
#include "MmRvPosAtom.h"
#include "MmRvSynthGreeksAtom.h"
#include "MmRvAggregationData.h"

#include <boost/shared_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

using namespace	boost;


class CMmRiskView;
// CMmRvRowData
struct _CMmRvRowData
{
public:
	IMmRvUndAtomPtr			m_spUnd;
	IMmRvPosAtomPtr			m_spPos;
	IMmRvSynthGreeksAtomPtr	m_spSynthGreeks;
};

class ATL_NO_VTABLE CMmRvRowData : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRvRowData, &CLSID_MmRvRowData>,
	public ISupportErrorInfoImpl<&IID_IMmRvRowData>,
	public IDispatchImpl<IMmRvRowData, &IID_IMmRvRowData, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public _CMmRvRowData
{
public:
	CMmRvRowData()
		:m_pUnd(NULL)
		,m_pPos(NULL)
		,m_pSynthGreeks(NULL)
		//,m_pStrategy(NULL)
		,m_dTolerance(1.0)
		,m_nLevel(0)
		,m_enPriceRoundingRule(enPrrRoundNone)
		//,m_nMapKey(0)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRVROWDATA)


BEGIN_COM_MAP(CMmRvRowData)
	COM_INTERFACE_ENTRY(IMmRvRowData)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_pUnd = NULL;
		m_pPos = NULL;
		m_pSynthGreeks = NULL;

		m_spUnd = NULL;
		m_spPos = NULL;
		m_spSynthGreeks = NULL;

		m_pUnkMarshaler.Release();
	}

public:
	bool GetField(CMmRiskView* view, RisksPosColumnEnum enCol, _variant_t& vt, bool bForSorting = false);
	bool SetField(long lColumnId, _variant_t& vtData){return false;}

public:
	CMmRvUndAtom*								m_pUnd;
	CMmRvPosAtom*								m_pPos;
	CMmRvSynthGreeksAtom*					m_pSynthGreeks;
	//shared_ptr<__MmRvAggregationData>	m_pAggregation;
	CMmRvAggData::MmRvAggDataPtr			m_pAgg;
	unsigned									m_nLevel;
	RisksRowDataType							m_Type;
	//unsigned									m_nMapKey;
	
public:
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvUndAtom*,		Und,		m_spUnd)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvPosAtom*,		Pos,		m_spPos)
	//IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvStrategyAtom*,	Strategy,	m_spStrategy)

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvSynthGreeksAtom*, SynthGreeks, m_spSynthGreeks)
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG,	OutlineLevel, m_nLevel)
	STDMETHOD(GetAggregationTradeSymbolID)(LONG* pID);
	STDMETHOD(GetAggregationPriceReplaceStatus)( EtsReplacePriceStatusEnum *pRetVal);

	//IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvGroupAtom*, Group, m_spGroup)

	STDMETHOD(get_IsAggregation)(VARIANT_BOOL* pRetVal)
	{
		if ( !pRetVal) return E_POINTER;
		if (m_Type == RDT_UNDAGG && m_nLevel > 0) 
			if (m_pUnd->m_bIsHead==VARIANT_TRUE||m_pUnd->m_spHeadComponent!=NULL)
				*pRetVal = VARIANT_TRUE;
			else
				*pRetVal = VARIANT_FALSE;
		else
			*pRetVal =  m_pAgg ? VARIANT_TRUE  : VARIANT_FALSE;
		return S_OK;
	}
	//---------------------------------------------
	/*STDMETHOD(putref_Group)(IMmRvGroupAtom* pGroup)
	{
		m_spGroup = pGroup;

		m_pGroup  = NULL;
		if(m_spGroup!=NULL)
			m_pGroup = dynamic_cast<CMmRvGroupAtom*>(pGroup);
		return S_OK;
	}*/

	STDMETHOD(get_Fut)(IMmRvFutAtom** pFut)
	{
		if(!pFut)
			return E_POINTER;
		if(m_pAgg && m_pAgg->pFut_)
			m_pAgg->pFut_->QueryInterface(__uuidof(IMmRvFutAtom), (void**)pFut);
		else
			*pFut = NULL;

		return S_OK;
	}

	//---------------------------------------------
	STDMETHOD(putref_Und)(IMmRvUndAtom* pUnd)
	{
		m_spUnd = pUnd;

		m_pUnd  = NULL;
		if(m_spUnd!=NULL)
			m_pUnd = dynamic_cast<CMmRvUndAtom*>(pUnd);
		return S_OK;
	}
	//---------------------------------------------
	/*STDMETHOD(putref_Strategy)(IMmRvStrategyAtom* pStrategy)
	{
		m_spStrategy = pStrategy;

		m_pStrategy  = NULL;
		if(m_spStrategy!=NULL)
			m_pStrategy = dynamic_cast<CMmRvStrategyAtom*>(pStrategy);
		return S_OK;
	}*/

	//---------------------------------------------
	STDMETHOD(putref_Pos)(IMmRvPosAtom* pPos)
	{
		m_spPos = pPos;

		m_pPos  = NULL;
		if(m_spPos!=NULL)
			m_pPos = dynamic_cast<CMmRvPosAtom*>(pPos);
		return S_OK;
	}
	//---------------------------------------------
	STDMETHOD(putref_SynthGreeks)(IMmRvSynthGreeksAtom* pSynthGreeks)
	{
		m_spSynthGreeks = pSynthGreeks;

		m_pSynthGreeks  = NULL;
		if(m_spSynthGreeks!=NULL)
			m_pSynthGreeks = dynamic_cast<CMmRvSynthGreeksAtom*>(pSynthGreeks);
		return S_OK;
	}

private:
	double						m_dTolerance;
	EtsPriceRoundingRuleEnum	m_enPriceRoundingRule;

	CComPtr<IUnknown>			m_pUnkMarshaler;

private:
	void GetValue(double dValue, bool bForSorting, bool bNegativeFormatting, long lRound, _variant_t& vtRet);
	void GetValue(long lValue, bool bForSorting, bool bNegativeFormatting, _variant_t& vtRet);

	bool GetFutOptionField(CMmRiskView* view, RisksPosColumnEnum enCol, _variant_t& vtRet, bool bForSorting, bool bNegativeFormatting, long lRound);
	bool GetOptionField(CMmRiskView* view, RisksPosColumnEnum enCol, _variant_t& vtRet, bool bForSorting, bool bNegativeFormatting, long lRound);
	bool GetUndField(CMmRiskView* view, RisksPosColumnEnum enCol, _variant_t& vtRet, bool bForSorting, bool bNegativeFormatting, long lRound);
	bool GetFutField(CMmRiskView* view, RisksPosColumnEnum enCol, _variant_t& vtRet, bool bForSorting, bool bNegativeFormatting, long lRound);
	//bool GetAggField(RisksPosColumnEnum enCol, _variant_t& vtRet, bool bForSorting, bool bNegativeFormatting, long lRound);
	//bool GetUndAggField(RisksPosColumnEnum enCol, _variant_t& vtRet, bool bForSorting, bool bNegativeFormatting, long lRound);
	bool GetAggregationField(CMmRiskView* view, RisksPosColumnEnum enCol, _variant_t& vtRet, bool bForSorting, bool bNegativeFormatting, long lRound);

};

OBJECT_ENTRY_AUTO(__uuidof(MmRvRowData), CMmRvRowData)

