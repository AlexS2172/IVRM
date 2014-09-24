// MmRiskView.h : Declaration of the CMmRiskView

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmRisks.h"

#include "MmRvRowData.h"
#include "MmRvUndAtom.h"
#include "MmRvGrpAtom.h"
#include "MmRvUndColl.h"
#include "MmRvReqColl.h"
#include "MmRvExpTotalColl.h"

#include "_IMmRiskViewEvents_CP.h"

#include "../cv/cvCV.h"

_COM_SMARTPTR_TYPEDEF(IMmRiskView, __uuidof(IMmRiskView));

class CMmRiskView;

struct null_deleter
{
	void operator()(void const *) const
	{
	}
};

class CRiskViewFieldSort
{
public:
	CRiskViewFieldSort(RisksPosColumnEnum enColumn, EtsSortingEnum enSortType, CMmRiskView* view)
		: m_enSortField(enColumn)
		, m_enSortType(enSortType)
		, m_view(view)
	{

	}


private:
	RisksPosColumnEnum		m_enSortField;
	EtsSortingEnum			m_enSortType;
	CMmRiskView*			m_view;
public:

	// return -1 when field sPtr1 less    than field in sPtr2
	// return  0 when field sPtr1 equal   than field in sPtr2
	// return  1 when field sPtr1 greater than field in sPtr2
	long CompareFields(const RisksPosColumnEnum  enField, const IMmRvRowDataPtr& sPtr1,  const IMmRvRowDataPtr& sPtr2)
	{
		CMmRvRowData* pPtr1 = static_cast<CMmRvRowData*>(sPtr1.GetInterfacePtr());
		CMmRvRowData* pPtr2 = static_cast<CMmRvRowData*>(sPtr2.GetInterfacePtr());
		bool bvt1 = false, bvt2 = false;
		

		if(pPtr1 && pPtr2)
		{
			_variant_t vt1;  bvt1 = pPtr1->GetField(m_view, enField, vt1, true);
			_variant_t vt2;  bvt2 = pPtr2->GetField(m_view, enField, vt2, true);

			if(bvt1 && bvt2)
			{
				switch(vt1.vt)
				{
				case VT_I2:
					return (V_I2(&vt1) < V_I2(&vt2)) ? -1 :
						((V_I2(&vt1) > V_I2(&vt2)) ?  1 : 0);
					break;
				case VT_I4:
					return (V_I4(&vt1) < V_I4(&vt2)) ? -1 :
						((V_I4(&vt1) > V_I4(&vt2)) ?  1 : 0);
					break;
				case VT_R4:
					return (V_R4(&vt1) < V_R4(&vt2)) ? -1 :
						((V_R4(&vt1) > V_R4(&vt2)) ?  1 : 0);
					break;
				case VT_R8:
					return (V_R8(&vt1) < V_R8(&vt2)) ? -1 :
						((V_R8(&vt1) > V_R8(&vt2)) ?  1 : 0);
					break;
				case VT_DATE:
					return (V_DATE(&vt1) < V_DATE(&vt2)) ? -1 :
						((V_DATE(&vt1) > V_DATE(&vt2)) ?  1 : 0);
					break;
				case VT_BSTR:
					return wcscmp(V_BSTR(&vt1) , V_BSTR(&vt2));
				default:
					{
						_bstr_t bsT1(vt1);
						_bstr_t bsT2(vt2);
						return bsT1 < bsT2? -1:(bsT1 > bsT2? 1:0);
					}
				}
			}
		}
		if (!bvt1 && bvt2) 
			return 1;
		if ( bvt1 && !bvt2)
			return -1;

		return 0;
	}

public:
	bool operator()(const IMmRvRowDataPtr& sPtr1,  const IMmRvRowDataPtr& sPtr2)
	{
		
		long lCompare = CompareFields(m_enSortField, sPtr1, sPtr2);
		if( lCompare == -1 )
			return m_enSortType==enSortDescending?true:false;
		if( lCompare == 1  )
			return m_enSortType==enSortDescending?false:true;
		return false;
	}
};

class ATL_NO_VTABLE CMmRiskView : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmRiskView, &CLSID_MmRiskView>,
	public ISupportErrorInfoImpl<&IID_IMmRiskView>,
	public IConnectionPointContainerImpl<CMmRiskView>,
	public CProxy_IMmRiskViewEvents<CMmRiskView>, 
	public IDispatchImpl<IMmRiskView, &IID_IMmRiskView, &LIBID_EtsMmRisksLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<VSFLEX::IVSFlexDataSource, &__uuidof(VSFLEX::IVSFlexDataSource), &VSFLEX::LIBID_VSFLEX, /*wMajor =*/ 1, /*wMinor =*/ 0>

{
	
	typedef std::map<long,long>				type_opositoptions ;
	typedef std::pair<long,long>				type_opositoptions_pair ;
	typedef type_opositoptions::iterator	type_opositoptionsI ;
	typedef std::map<_bstr_t, int>			ExpandedRowsMap;
	typedef std::map<long, double>			ContractPriceMap;
	typedef ContractPriceMap::iterator		CMPItr;

	type_opositoptions							m_opositoptions ;
	ExpandedRowsMap								m_mapExpandedRows;
	RisksPosColumnEnum							m_SortColumn;
	EtsSortingEnum									m_SortOrder;

public:
	typedef std::vector<IMmRvRowDataPtr>	CRowsData;

	CMmRiskView()
		:m_pIdx(NULL)
		,m_pGrp(NULL)
		,m_pUnd(NULL)
		,m_pQuoteReqsAll(NULL)
		,m_pExpTotal(NULL)
		,m_lUndCount(0L)
		,m_lOptCount(0L)
		,m_lFutCount(0L)
		,m_bIsStopped(false)
		,m_nOptPositions(0L)
		,m_nUndPositions(0L)
		,m_SortColumn(RPC_UND)
		,m_SortOrder(enSortDescending)
		,m_enViewMode(RV_RISKS)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMRISKVIEW)


BEGIN_COM_MAP(CMmRiskView)
	COM_INTERFACE_ENTRY(IMmRiskView)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(VSFLEX::IVSFlexDataSource)
	COM_INTERFACE_ENTRY2(IDispatch, IMmRiskView)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMmRiskView)
	CONNECTION_POINT_ENTRY(__uuidof(_IMmRiskViewEvents))
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		try
		{
			TRACE_INFO("MmRiskViev %x is being created", (long)(this));
					
			__Realtime = __RealtimeCalculateCv = false;

			m_spCvRTContext = NULL;

			_CHK(CComObject<CMmRvUndAtom>::CreateInstance(&m_pIdx), _T("Fail to create Index"));
			m_spIdx.Attach(m_pIdx, TRUE);

			_CHK(CComObject<CMmRvGrpAtom>::CreateInstance(&m_pGrp), _T("Fail to create Group"));
			m_spGrp.Attach(m_pGrp, TRUE);

			_CHK(CComObject<CMmRvUndColl>::CreateInstance(&m_pUnd), _T("Fail to create Underlying Collection"));
			m_spUnd.Attach(m_pUnd, TRUE);

			_CHK(CComObject<CMmRvReqColl>::CreateInstance(&m_pQuoteReqsAll), _T("Fail to create Quotes reqest Collection"));
			m_spQuoteReqsAll.Attach(m_pQuoteReqsAll, TRUE);

			_CHK(CComObject<CMmRvExpTotalColl>::CreateInstance(&m_pExpTotal), _T("Fail to create Expirations Totals"));
			m_spExpTotal.Attach(m_pExpTotal, TRUE);

			_CHK(m_spExp.CreateInstance(__uuidof(EtsMmEntityAtomColl)), _T("Fail to create Expirations Collection"));

			return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
		}
		catch (_com_error& e)
		{
			TRACE_COM_ERROR_EX(e, "Failed to create MmRiskViewObject", "");
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRiskView, e.Error());
		}

		TRACE_INFO("MmRiskViev %x is created", (long)(this));

		return S_OK;
	}
	
	void FinalRelease() 
	{

		__Realtime = __RealtimeCalculateCv = false;

		m_spIdx= NULL ;
		m_spGrp= NULL ;
		m_spUnd= NULL ;
		m_spQuoteReqsAll= NULL ;
		m_spVolatilitySource= NULL ;
		m_spExp= NULL ;
		m_spExpTotal= NULL ;

		m_Rows.clear();

		if (m_spCvRTContext)
			m_spCvRTContext->Clear();

		m_spCvRTContext = NULL;

		m_pUnkMarshaler.Release();

		__SimulationScenario = NULL;
	}

private:
	void DecreaseOutline( CRowsData::reverse_iterator& begin, CRowsData::reverse_iterator& end);
	IMmRvUndAtomPtr _AddNewUnderlying(IUndAtomPtr spEtsUndAtom/*IMmTradeInfoAtomPtr spTradeAtom*/, IMmRvReqColl* pRequestColl = NULL);

	void AddActiveFuture( IUndAtomPtr spUnd, CComObject<CMmRvUndAtom>* pUndAtom, IMmRvReqColl* pRequestColl = NULL );


	IMmRvPosAtomPtr _AddNewPosition(IMmTradeInfoAtomPtr spTradeAtom, IMmRvUndAtomPtr spUndAtom, LONG lOptionOposit, IMmRvReqColl* pRequestColl = NULL, VARIANT_BOOL bUpdateVola = VARIANT_FALSE);
	HRESULT  _AddNewTradeToPosition(IMmRvUndAtomPtr spUndAtom, IMmRvPosAtomPtr spPosAtom, IMmTradeInfoAtomPtr spTradeAtom);
	HRESULT InitPositionSynthetics(CMmRvUndAtom* pUndAtom, CMmRvPosAtom *pPosAtom, IMmRvReqColl* pNewReqsAll = NULL);
	void InitVola(CMmRvUndAtom* pUnd);
	void AppendTradeToPosition(IMmRvPosAtomPtr& spPosAtom,const IMmTradeInfoAtomPtr& spTradeAtom);
	HRESULT _RemoveOldTradeFromPosition(CMmRvUndAtom* pUnd, /*CMmRvStrategyAtom*,*/ IMmRvPosAtom* pPosAtom, IMmTradeInfoAtom* pTrd);

private:
	CRowsData    m_Rows;

	bool		m_bIsStopped;

	IMmRvUndAtomPtr			m_spIdx;
	IMmRvGrpAtomPtr			m_spGrp;
	IMmRvUndCollPtr			m_spUnd;
	IMmRvReqCollPtr			m_spQuoteReqsAll;
	IVolatilitySourcePtr	m_spVolatilitySource; 
	IEtsMmEntityAtomCollPtr m_spExp;
	IMmRvExpTotalCollPtr    m_spExpTotal;

	CComPtr<IUnknown>		m_pUnkMarshaler;

	IEtsMainPtr				m_spEtsMain;
	IMmTradeChannelPtr		m_spTradesCache;

	CComObject<CMmRvUndAtom>*		m_pIdx;
	CComObject<CMmRvGrpAtom>*		m_pGrp;
	CComObject<CMmRvUndColl>*		m_pUnd;
	CComObject<CMmRvReqColl>*		m_pQuoteReqsAll;
	CComObject<CMmRvExpTotalColl>*	m_pExpTotal;

	long m_lUndCount;
	long m_lOptCount;
	long m_lFutCount;

	long m_nOptPositions;
	long m_nUndPositions;
	RiskViewModeEnum m_enViewMode;

	long					m_lVisibleColumns;
	long					m_ColumnsEncoding[512/*RPC_COLUMN_COUNT*/];

	_bstr_t                   m_bsConnectionString;
	EgLib::CDBConnection	  m_Connection;
	ContractPriceMap		  m_mapManualPrice;

public:
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvUndAtom*,			Idx,			m_spIdx)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvGrpAtom*,			Grp,			m_spGrp)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvUndColl*,			Und,			m_spUnd)
	IMPLEMENT_OBJECT_PROPERTY(		  IEtsMain*,				EtsMain,		m_spEtsMain)
	IMPLEMENT_OBJECT_PROPERTY(		  IMmTradeChannel*,			TradesCache,	m_spTradesCache)
	IMPLEMENT_OBJECT_PROPERTY(IVolatilitySource*,				VolaSource,		m_spVolatilitySource)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsMmEntityAtomColl*,	Exp,			m_spExp)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvExpTotalColl*,		ExpTotal,		m_spExpTotal)		

	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG,	OptionPositions, m_nOptPositions)
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG,	UndPositions,    m_nUndPositions)

	IMPLEMENT_SIMPLE_PROPERTY(RiskViewModeEnum,	ViewMode,    m_enViewMode)
	
	
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmRvReqColl*, QuoteReqsAll, m_spQuoteReqsAll)

	STDMETHOD(Refresh)(RisksPosColumnEnum SortField, long lExpiryFilter,SAFEARRAY** arrAggregationCols/* AggregationTypeEnum enAggregation, SubAggregationTypeEnum enSubAggregation*/,DOUBLE dUndPriceToleranceValue = 0., EtsPriceRoundingRuleEnum enPriceRoundingRule = enPrrRoundNone);
	STDMETHOD(Stop)()
	{
		m_bIsStopped = true;
		return S_OK;
	}

	STDMETHOD(get_NetExposureAUM)(DOUBLE* pVal)
	{
		HRESULT hr = S_OK;
		if(!pVal)
			return E_POINTER;
		if(m_pGrp)
			hr = m_pGrp->get_NetExposureAUM(pVal);
		else
			*pVal = 0.0;
		return hr;
	}
	STDMETHOD(put_NetExposureAUM)(DOUBLE newVal)
	{
		HRESULT hr = S_OK;
		if(m_pGrp)
			hr = m_pGrp->put_NetExposureAUM(newVal);
		if(m_spUnd)
			m_pUnd->SetNetExposureAUM(newVal);

		return hr;
	}

	STDMETHOD(PositionsLoad)(IN IMmTradeInfoColl* pTradesColl);
	IMPLEMENT_BSTRT_PROPERTY(ConnectionString, m_bsConnectionString);
	STDMETHOD (SaveTheoPricesAsClose)(IN IMmTradeInfoColl* pTradesColl = NULL);

	STDMETHOD(FitToMarketVolatility)();

	STDMETHOD(AddOpositSymbol)( LONG ContractID , LONG ContractOfOposit );
	STDMETHOD(AddNewPosition)(IN IMmTradeInfoAtom* pTradeAtom, IN IMmRvUndAtom* pUndAtom, IN IMmRvReqColl* pRequestColl, LONG lOptionOposit, VARIANT_BOOL bUpdateVola, OUT IMmRvPosAtom** ppPosition)
	{
		if(!ppPosition)
			return E_POINTER;
		HRESULT hr = S_OK;
		try
		{
			*ppPosition = _AddNewPosition(pTradeAtom, pUndAtom, lOptionOposit,pRequestColl, bUpdateVola).Detach();
		}
		catch (_com_error &err)
		{
			hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), __uuidof(IMmRiskView), err.Error());
		}
		return hr;

	}
	STDMETHOD(AddNewTradeToPosition)(IN IMmRvUndAtom* pUndAtom, IN IMmRvPosAtom* pPosAtom, IN IMmTradeInfoAtom* pTradeAtom)
	{
		HRESULT hr = S_OK;
		try
		{
			_AddNewTradeToPosition(pUndAtom, pPosAtom, pTradeAtom);
		}
		catch (_com_error &err)
		{
			hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), __uuidof(IMmRiskView), err.Error());
		}
		return hr;
	}


	STDMETHOD(AddNewUnderlying)(IN IMmTradeInfoAtom* spTradeAtom, IN IMmRvReqColl* pRequests, OUT IMmRvUndAtom** ppRet)
	{
		if(!ppRet)
			return E_POINTER;
		HRESULT hr = S_OK;
		try
		{
			*ppRet = _AddNewUnderlying(spTradeAtom->Und, pRequests).Detach();
		}
		catch (_com_error &err)
		{
			hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), __uuidof(IMmRiskView), err.Error());
		}
		return hr;
	}

	STDMETHOD(RemoveOldTradeFromPosition)(IMmRvUndAtom* pUndAtom, IMmRvPosAtom* pPosAtom, IMmTradeInfoAtom* pTrd);


	STDMETHOD(Calc)(VARIANT_BOOL  bTotals,
					LONG nMask,
					VARIANT_BOOL bCalcGreeks,
					VARIANT_BOOL bUpdateVola,
					VARIANT_BOOL bRealtimeCalc,
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

public:
	STDMETHOD(AddNewActiveFutures)(IMmRvUndAtom* spUndAtom, IMmRvReqColl* pRequests);
	STDMETHOD(put_PosColumnsOrder)(SAFEARRAY** colOrder);
	STDMETHOD(get_PosRowData)(LONG lRow,  IMmRvRowData** ppVal);
	STDMETHOD(raw_GetFieldCount) (/*[out,retval]*/ long * pFields );
	STDMETHOD(raw_GetRecordCount)(/*[out,retval]*/ long * pRecords );
	STDMETHOD(raw_GetFieldName)  (/*[in]*/ long Field, /*[out,retval]*/ BSTR * pFieldName );
	STDMETHOD(raw_GetData)		 (/*[in]*/ long Field, /*[in]*/ long Record, /*[out,retval]*/ BSTR * pData );
	STDMETHOD(raw_SetData)		 (/*[in]*/ long Field,	/*[in]*/ long Record,/*[in]*/ BSTR newData );
	STDMETHOD(get_IsRowExpanded)( IN long Record,  OUT VARIANT_BOOL *pRetVal ){
		if ( !pRetVal || !Record) return E_POINTER;

		if (static_cast<size_t>(Record) <= m_Rows.size() ) {
			IMmRvRowDataPtr spAtom = m_Rows[Record-1];
			CMmRvRowData* pAtom = static_cast<CMmRvRowData*>(spAtom.GetInterfacePtr());
			if ( pAtom && (pAtom->m_Type == RDT_UNDAGG || RDT_AGG == pAtom->m_Type ) && pAtom->m_pAgg ) 
				*pRetVal =  (m_mapExpandedRows.find(pAtom->m_pAgg->internalName_) != m_mapExpandedRows.end() ? VARIANT_TRUE : VARIANT_FALSE );
			else
				*pRetVal = VARIANT_TRUE;
			return S_OK;
		}
		return E_INVALIDARG;
	}
	STDMETHOD(put_IsRowExpanded)(IN long Record, IN VARIANT_BOOL bExpanded){
		if ( !Record) return E_INVALIDARG;
		if (static_cast<size_t>(Record) <= m_Rows.size() ) {
			IMmRvRowDataPtr spAtom = m_Rows[Record-1];
			CMmRvRowData* pAtom = static_cast<CMmRvRowData*>(spAtom.GetInterfacePtr());
			if ( pAtom && ( pAtom->m_Type == RDT_UNDAGG || RDT_AGG == pAtom->m_Type )  && pAtom->m_pAgg) {
				if ( bExpanded) 
					m_mapExpandedRows[pAtom->m_pAgg->internalName_] = 1;
				else
					m_mapExpandedRows.erase(pAtom->m_pAgg->internalName_);
			}
			return S_OK;
		}
		else
			return E_INVALIDARG;
	}
	STDMETHOD(SetAllRowsCollapsed)(){
		m_mapExpandedRows.clear();
		return S_OK;
	}
	STDMETHOD(SetAllRowsExpanded)(){
		m_mapExpandedRows.clear();
		if ( !m_Rows.empty() ) {
			for( CRowsData::iterator itrCrnt = m_Rows.begin(), itrEnd = m_Rows.end(); itrCrnt != itrEnd; ++itrCrnt ){
				CMmRvRowData* pRowData = static_cast<CMmRvRowData*>((*itrCrnt).GetInterfacePtr());
				if (
					(pRowData->m_Type == RDT_AGG || pRowData->m_Type == RDT_UNDAGG) && 
					pRowData->m_pAgg &&
					CMmRvAggData::IsAggregationRowVisible(pRowData->m_pAgg.get()) 
					)
				{
					m_mapExpandedRows[pRowData->m_pAgg->internalName_] = 1;
				}
			}
		}
		return S_OK;
	}
	STDMETHOD(put_SortField)(RisksPosColumnEnum enField){
		if (enField != m_SortColumn ) {
			m_SortOrder = enSortAscending;
			m_SortColumn = enField;
		}
		else
			m_SortOrder = m_SortOrder == enSortAscending ? enSortDescending : enSortAscending;
		return S_OK;
	}
	STDMETHOD(get_SortField)(RisksPosColumnEnum* pRetVal){
		if ( !pRetVal) return E_POINTER;
		*pRetVal = m_SortColumn;
		return S_OK;
	}
	STDMETHOD(get_SortOrder)(EtsSortingEnum* pRetVal ){
		if (!pRetVal) return E_POINTER;
		*pRetVal = m_SortOrder;
		return S_OK;
	}
	STDMETHOD(SetDirty)()
	{
		if(m_pUnd)
			m_pUnd->SetDirty();
		return S_OK;
	}

	STDMETHOD(LoadPositions)(IN IEtsFilterData* spFilter);

	//-------------------------------------------------------//
	IcvRTContextPtr			m_spCvRTContext;
	LONG					m_lContextID;

	LONG					*m_lCvLevels;

	//LONG					m_nGrpLevelID; //= 1
	//LONG					m_nUndLevelID; //= 2
	//LONG					m_nOptLevelID; //= 3

	LONG					**m_lcvLevelIDs; //1 = optIDs, 2 = grpIDs
	
	//LONG					m_cvOptIDs As Collection
	//LONG					m_cvGrpIDs As Collection
	
	LONG&	getCvLevelIdImpl(CvLevelEnum enLevel){
		try{
			return m_lCvLevels[static_cast<LONG>(enLevel)];
		}
		catch (...){
			throw;			
		}
	};
	
	typedef std::map<LONG/*columnID*/, LONG/*cvID*/> column2cv;
	typedef column2cv::iterator column2cv_iterator;
	typedef std::map<LONG/*CvLevelEnum*/, column2cv> view_levels;
	typedef view_levels::iterator view_level_iterator;
	view_levels viewLevels;

	void
	getConnectionString(BSTR& ConnectionString){
		try {
			_CHK(m_spEtsMain->get_DatabaseString(&ConnectionString), 
				_T("Fail to get Connection String."));
		}
		catch(...){
			throw;
		}
	};

	LONG
	getContextID(std::string ContextName){

		try{

			BSTR bsDBConnectionString = NULL;
			getConnectionString(bsDBConnectionString);		

			CDBConnection	m_DbConnection;
			if(!m_DbConnection.IsOpened())
				m_DbConnection.Open(_bstr_t(bsDBConnectionString), 10, 120, 300, 300);

			/*init request*/
			CStoredProc<> sp(m_DbConnection, L"usp_cvContextID_Get");

			/*Set procedure params*/
			sp << _bstr_t(ContextName.c_str());

			/*execute query*/
			sp.Execute();
			
			return static_cast<LONG>(sp.GetResult());
		}
		catch (...){
			throw;
		}
		return 0L;
	};

	LONG
	InitializeCvContextImpl(std::string ContextName){
		try{

			_CHK(m_spCvRTContext.CreateInstance(__uuidof(cvRTContext)),
				_T("Fail to create cvRTConext."));

			long levels_count = (m_enViewMode == RV_GREEKS) ? CVLEVEL_COUNT : CVLEVEL_COUNT - 2;

			m_lCvLevels = new LONG [levels_count];

			LONG m_lContextID = getContextID ( ContextName );
			if (m_lContextID == 0L){
				return 0L;
			}
			else{		
				BSTR bsDBConnectionString = NULL;
				getConnectionString (bsDBConnectionString);

				_CHK(m_spCvRTContext->put_ConnectionString(bsDBConnectionString), 
					_T("Fail to set ConnectionString to cvRTContext."));

				_CHK(m_spCvRTContext->raw_InitContext(m_lContextID),
					_T("InitContext failure."));

				std::string levelName = "Totals";
				_CHK(m_spCvRTContext->raw_GetLevelIDByName((BSTR)_bstr_t(levelName.c_str()), 
															&m_lCvLevels[CVLEVEL_GRP]),
					_T("Fail to get level ID for TOTATLS."));

				levelName = "Underlyings";
				_CHK(m_spCvRTContext->raw_GetLevelIDByName((BSTR)_bstr_t(levelName.c_str()), 
															&m_lCvLevels[CVLEVEL_UND]),
					_T("Fail to get level ID for UNDERLYINGS."));

				levelName = "Positions";
				_CHK(m_spCvRTContext->raw_GetLevelIDByName((BSTR)_bstr_t(levelName.c_str()), 
															&m_lCvLevels[CVLEVEL_POS]),
					_T("Fail to get level ID for POSITIONS."));
				
				if (m_enViewMode == RV_GREEKS){

					levelName = "GroupingTotals";
					_CHK(m_spCvRTContext->raw_GetLevelIDByName((BSTR)_bstr_t(levelName.c_str()), 
																&m_lCvLevels[CVLEVEL_EXPIRY_TOTAL]),
						_T("Fail to get level ID for ASSET EXPIRY."));

					levelName = "Grouping";
					_CHK(m_spCvRTContext->raw_GetLevelIDByName((BSTR)_bstr_t(levelName.c_str()), 
																&m_lCvLevels[CVLEVEL_EXPIRY_ASSET]),
						_T("Fail to get level ID for EXPIRY TOTAL."));
				};

				for (long i = 0; i < levels_count; i++){
					if (m_lCvLevels[i] == 0){
						_CHK(m_spCvRTContext->raw_Halt(), 
							_T("Fail to Halt cvRTContext."));

						return 0L;
					}
				}

				return 1L;
			};
		}
		catch (...){
			throw;
		}

		return 0L;
	};

	LONG
	setCvID2ColumnImpl(CvLevelEnum enCvLevel, LONG cvID, LONG columnID){
		(viewLevels[static_cast<LONG>(enCvLevel)])[columnID] = cvID;
		return 1L;
	};

	LONG
	getCvIDbyColumnImpl(CvLevelEnum enCvLevel, LONG columnID){
		return (viewLevels[static_cast<LONG>(enCvLevel)])[columnID];
	};

	LONG
	getIsCvInitializedImpl(CvLevelEnum cvLevel, LONG cvID){
		
		view_level_iterator it = viewLevels.begin();
		view_level_iterator itEnd = viewLevels.end();
		
		it = viewLevels.find(cvLevel);
		if (it != itEnd){
			column2cv& columns = it->second;

			column2cv_iterator itC = columns.begin();
			column2cv_iterator itCEnd = columns.end();

			for (;itC != itCEnd; itC++){
				if (itC->second == cvID)
					return 1L;
			}
		}
		return 0L;
	};

	STDMETHOD(InitializeCvContext)(BSTR ContextName){
		HRESULT hrStatus = S_OK;
		try{

			char* contextNamePtr = _com_util::ConvertBSTRToString(ContextName);	
			std::string contextName(contextNamePtr);
			delete [] contextNamePtr;

			hrStatus = InitializeCvContextImpl(contextName) != 1L ? E_FAIL : S_OK;
		}
		catch(...){
			throw;
		};
		return hrStatus;
	};

	
	STDMETHOD(setCvID2Column)(	CvLevelEnum cvLevel,
								LONG columnID,
								LONG cvID)
	{
		HRESULT hrStatus = S_OK;
		try{
			hrStatus = setCvID2ColumnImpl(cvLevel, cvID, columnID) != 1 ? E_FAIL : S_OK;
		}
		catch(...){
			throw;
		};
		return hrStatus;
	};

	STDMETHOD(getCvIDbyColumn)(	CvLevelEnum cvLevel,
								LONG columnID,
								LONG* cvID)
	{
		HRESULT hrStatus = S_OK;
		try{
			if (cvID == NULL)
				return E_POINTER;

			*cvID = getCvIDbyColumnImpl(cvLevel, columnID);
		}
		catch(...){
			throw;
		};
		return hrStatus;
	};

	STDMETHOD(getCvLevelID) (	CvLevelEnum cvLevel,
								LONG* levelID)
	{
		HRESULT hrStatus = S_OK;
		
		try {
			if (levelID == NULL)
				return E_POINTER;

			*levelID = getCvLevelIdImpl(cvLevel);
		}
		catch (...) {
			throw;
		}
		return hrStatus;
	};

	STDMETHOD(getIsCvInitialized) (	CvLevelEnum cvLevel,
									LONG cvID,
									LONG* isInit)
	{
		HRESULT hrStatus = S_OK;

		try {
			if (isInit == NULL)
				return E_POINTER;

			*isInit = getIsCvInitializedImpl(cvLevel, cvID);
		}
		catch (...){
			throw;
		}
		return hrStatus;
	};

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IcvRTContext*, cvRTContext, m_spCvRTContext)
	

	STDMETHOD(setRealtime)(VARIANT_BOOL Flag)
	{
		HRESULT hrStatus = S_OK;

		try {
			__Realtime = (Flag == VARIANT_TRUE ? true : false); 
		}
		catch (...){
			throw;
		}
		return hrStatus;
	};

	STDMETHOD(setRealtimeCalculateCv)(VARIANT_BOOL Flag)
	{
		HRESULT hrStatus = S_OK;

		try {
			__RealtimeCalculateCv = (Flag == VARIANT_TRUE ? true : false); 
		}
		catch (...){
			throw;
		}
		return hrStatus;
	};

	IMPLEMENT_OBJECT_PROPERTY(IMarketSimulationScenario*, SimulationScenario, __SimulationScenario)

private:
	bool	__Realtime;
	bool	__RealtimeCalculateCv;
	IMarketSimulationScenarioPtr	__SimulationScenario;
	//-------------------------------------------------------//
};

OBJECT_ENTRY_AUTO(__uuidof(MmRiskView), CMmRiskView)
