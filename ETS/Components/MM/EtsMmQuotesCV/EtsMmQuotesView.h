// EtsMmQuotesView.h : Declaration of the CEtsMmQuotesView

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmQuotes.h"
#include "MmQvGrpAtom.h"
#include "MmQvRequestColl.h"
#include "MmQvRowData.h"


// CEtsMmQuotesView
class CQuotesViewSort
{
public:
	CQuotesViewSort(MmQvOptColumnEnum enColumn, EtsSortingEnum enSortType)
		:m_enSortField(enColumn)
		,m_enSortType(enSortType){}


private:
	MmQvOptColumnEnum m_enSortField;
	EtsSortingEnum		 m_enSortType;
public:
	bool operator()(const IMmQvRowDataPtr& sPtr1,  const IMmQvRowDataPtr& sPtr2)
	{
		CMmQvRowData* pPtr1 = static_cast<CMmQvRowData*>(sPtr1.GetInterfacePtr());
		CMmQvRowData* pPtr2 = static_cast<CMmQvRowData*>(sPtr2.GetInterfacePtr());
		if(pPtr1 && pPtr2)
		{
			_variant_t vt1;  bool bvt1 = pPtr1->GetField(m_enSortField, vt1, true);
			_variant_t vt2;  bool bvt2 = pPtr2->GetField(m_enSortField, vt2, true);

			if(bvt1 && bvt2)
			{
				switch(vt1.vt)
				{
				case VT_I2:
					if(m_enSortType != enSortDescending)
						return V_I2(&vt1) < V_I2(&vt2);
					else
						return V_I2(&vt2) < V_I2(&vt1);
					break;
				case VT_I4:
					if(m_enSortType != enSortDescending)
						return V_I4(&vt1) < V_I4(&vt2);
					else
						return V_I4(&vt2) < V_I4(&vt1);
					break;
				case VT_R4:
					if(m_enSortType != enSortDescending)
						return V_R4(&vt1) < V_R4(&vt2);
					else
						return V_R4(&vt2) < V_R4(&vt1);
					break;
				case VT_R8:
					if(m_enSortType != enSortDescending)
						return V_R8(&vt1) < V_R8(&vt2);
					else
						return V_R8(&vt2) < V_R8(&vt1);
					break;
				case VT_DATE:
					if(m_enSortType != enSortDescending)
						return V_DATE(&vt1) < V_DATE(&vt2);
					else
						return V_DATE(&vt2) < V_DATE(&vt1);
				case VT_BSTR:
					if(m_enSortType != enSortDescending)
						return wcscmp(V_BSTR(&vt1) , V_BSTR(&vt2))<0;
					else
						return wcscmp(V_BSTR(&vt2) , V_BSTR(&vt1))<0;
				default:
					{
						_bstr_t bsT1(vt1);
						_bstr_t bsT2(vt2);
						if(m_enSortType != enSortDescending)
							return bsT1 < bsT2;
						else
							return bsT2 < bsT1;
					}
				}
			}
		}
		return true;
	}
};



class ATL_NO_VTABLE CEtsMmQuotesView : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsMmQuotesView, &CLSID_EtsMmQuotesView>,
	public ISupportErrorInfoImpl< &IID_IEtsMmQuotesView>,
	public IConnectionPointContainerImpl<CEtsMmQuotesView>,
	public IDispatchImpl<IEtsMmQuotesView, &IID_IEtsMmQuotesView, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IConnectionPointImpl<CEtsMmQuotesView, &__uuidof(_IEtsMmQuotesViewEvents), EgLib::CComDynamicUnkGITArray>,
	public IDispatchImpl<VSFLEX::IVSFlexDataSource, &__uuidof(VSFLEX::IVSFlexDataSource), &VSFLEX::LIBID_VSFLEX, /*wMajor =*/ 1, /*wMinor =*/ 0>

{
	typedef std::vector< CAdapt<IMmQvRowDataPtr> > CRowsData;
	typedef std::map<long, IExchAtomPtr> CExchanges;
public:
	CEtsMmQuotesView()
		:m_pGrp(NULL)
		,m_pQuoteRequestAll(NULL)
		,m_nUpdatedOptCount(0L)
		,m_nUpdatedFutCount(0L)
		,m_nUpdatedUndCount(0L)
		,m_lVisibleColumns(0L)
		,m_enSortField(QOC_NONE)
		,m_enSortType(enSortDescending)
		,m_bMarketDataChanged(VARIANT_FALSE)
		,m_nUndQuotesCount(0L)
		,m_nOptQuotesCount(0L)
		,m_nFutQuotesCount(0L)
		,m_nProcessors(-1)

	{
		ZeroMemory(m_ColumnsEncoding, sizeof(m_ColumnsEncoding));
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSMMQUOTESVIEW)

BEGIN_COM_MAP(CEtsMmQuotesView)
	COM_INTERFACE_ENTRY(IEtsMmQuotesView)
	COM_INTERFACE_ENTRY(VSFLEX::IVSFlexDataSource)
	COM_INTERFACE_ENTRY2(IDispatch, IEtsMmQuotesView)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CEtsMmQuotesView)
	CONNECTION_POINT_ENTRY(__uuidof(_IEtsMmQuotesViewEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		HRESULT hr = S_OK;
		try
		{
			m_hCancelEvent.Attach(::CreateEvent(NULL, FALSE, FALSE, NULL));
			_CHK(CComObject<CMmQvGrpAtom>::CreateInstance(&m_pGrp), _T("Fail to create Quotes View Group."));
			m_spGrp.Attach(m_pGrp, TRUE);

			_CHK(CComObject<CMmQvRequestColl>::CreateInstance(&m_pQuoteRequestAll), _T("Fail to create Quotes View Group."));
			m_spQuoteRequestAll.Attach(m_pQuoteRequestAll, TRUE);

			_CHK(m_spCustRates.CreateInstance(CLSID_EtsMmEntityAtomColl),_T("Fail to create Cust Rates Collection"));
			_CHK(m_spVisibleExp.CreateInstance(CLSID_EtsMmFilterAtomColl),_T("Fail to create Visible Expirations Collection"));
			_CHK(m_spVisibleRoot.CreateInstance(CLSID_EtsMmFilterAtomColl),_T("Fail to create Visible Option Roots Collection"));
			_CHK(m_spVisibleUndExch.CreateInstance(CLSID_EtsMmFilterAtomColl),_T("Fail to create Visible Und Exchanges Collection"));
			_CHK(m_spVisibleOptExch.CreateInstance(CLSID_EtsMmFilterAtomColl),_T("Fail to create Visible Opt Exchanges Collection"));
			_CHK(m_spVisibleStr.CreateInstance(CLSID_EtsMmEntityAtomColl),_T("Fail to create Visible Opt Strikes Collection"));

			_CHK(m_spTradeFilter.CreateInstance(CLSID_EtsFilterData),_T("Fail to create Trade Filter"));

			if (m_spTradeFilter){
				m_spTradeFilter->Initialize(0, 6);
			}

			_CHK(CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p), _T("Unable to create custom Marshaller"));

		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IEtsMmQuotesView, e.Error());
		}
		return hr;
	}
	
	void FinalRelease() 
	{
		m_spGrp = NULL;
		m_spBatchPriceProvider = NULL;
		m_spQuoteRequestAll = NULL;
		m_spEtsMain = NULL;
		m_spCustRates = NULL;

		m_spVisibleExp = NULL;
		m_spVisibleRoot = NULL;
		m_spVisibleUndExch = NULL;
		m_spVisibleOptExch = NULL;

		m_spVisibleStr = NULL;
		m_spTradeFilter	=	NULL;

		m_pUnkMarshaler.Release();
	}

	void AddQuoteExchange(IExchAtomPtr spExchAtom,
				  		  vt_date dtExpiry,
						  IMmQvExpAtomPtr spExpiry,
						  CComObject<CMmQvOptAtom>* pOptionAtom,
						  CComObject<CMmQvOptRootAtom>* pOptRootAtom);


public:
	STDMETHOD(raw_GetFieldCount) (/*[out,retval]*/ long * pFields );
	STDMETHOD(raw_GetRecordCount)(/*[out,retval]*/ long * pRecords );
	STDMETHOD(raw_GetFieldName)  (/*[in]*/ long Field, /*[out,retval]*/ BSTR * pFieldName );
	STDMETHOD(raw_GetData)		 (/*[in]*/ long Field, /*[in]*/ long Record, /*[out,retval]*/ BSTR * pData );
	STDMETHOD(raw_SetData)		 (/*[in]*/ long Field,	/*[in]*/ long Record,/*[in]*/ BSTR newData );

private:
	CComPtr<IUnknown> m_pUnkMarshaler;


	IMmQvGrpAtomPtr			m_spGrp;
	IBatchPriceProviderPtr  m_spBatchPriceProvider;
	IMmQvRequestCollPtr     m_spQuoteRequestAll;
	IEtsMainPtr				m_spEtsMain;
	IEtsMmEntityAtomCollPtr m_spCustRates;

	IEtsMmFilterAtomCollPtr m_spVisibleExp;
	IEtsMmFilterAtomCollPtr m_spVisibleRoot;
	IEtsMmFilterAtomCollPtr m_spVisibleUndExch;
	IEtsMmFilterAtomCollPtr m_spVisibleOptExch;

	IEtsMmEntityAtomCollPtr m_spVisibleStr;
	IEtsFilterDataPtr		m_spTradeFilter;

	volatile long           m_nUpdatedOptCount;
	volatile long           m_nUpdatedFutCount;
	volatile long           m_nUpdatedUndCount;

	long                    m_lVisibleColumns;
	long					m_ColumnsEncoding[QOC_COLUMN_COUNT];

private:
	_bstr_t                   m_bsConnectionString;
	CHandle					  m_hCancelEvent;
	EgLib::CDBConnection	  m_Connection;	
	CRowsData                 m_Rows;  

	CComObject<CMmQvGrpAtom>*		m_pGrp;
	CComObject<CMmQvRequestColl>*   m_pQuoteRequestAll;
	MmQvOptColumnEnum				m_enSortField;
	EtsSortingEnum					m_enSortType;
	VARIANT_BOOL					m_bMarketDataChanged;

	long m_nUndQuotesCount;
	long m_nOptQuotesCount;
	long m_nFutQuotesCount;
	long m_nProcessors;
private:
	HRESULT LoadOptions(long lGroupID);
	HRESULT AcceptQuote(const QuoteUpdateParams& Params, const QuoteUpdateInfo& Info);


	HRESULT OnProgress(LONG lProgress,  LONG lMaxProgress);
	HRESULT InitVisibleOptExchanges(bool bInitialize, CExchanges& vecVisibleExchanges, bool& bAllExchanges);
	HRESULT InitVisibleFutOptExchanges(bool bInitialize, CExchanges& vecVisibleExchanges, bool& bAllExchanges);
	HRESULT RefreshStockOptions(bool bInitialize);
	HRESULT RefreshFuturesOptions(bool bInitialize);
	HRESULT AddExch(IExchCollPtr aExchColl, long lID, _bstr_t sCode, _bstr_t sName);
	double PriceMidEx(double dPriceBid, double dPriceAsk, double dPriceLast, bool bUseLast = true);

	void PerformSorting()
	{
		if(!m_Rows.empty())
		{
			CQuotesViewSort sortingClass(m_enSortField, m_enSortType);
			std::stable_sort(m_Rows.begin(), m_Rows.end(), sortingClass);
			RenumberItems();
		}
	}

	void RenumberItems()
	{
		int iPos = 1;
		for(CRowsData::iterator itr = m_Rows.begin(); itr!= m_Rows.end(); ++itr, ++iPos)
		{
			IMmQvRowDataPtr& spPtr = *itr;
			CMmQvRowData* pPtr = static_cast<CMmQvRowData*>(spPtr.GetInterfacePtr());
			if(pPtr)
				pPtr->SetRowPosition(iPos);
		}
	}

public:
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvGrpAtom*, Grp, m_spGrp)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvRequestColl*, QuoteReqsAll, m_spQuoteRequestAll)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsMmEntityAtomColl*, CustRates, m_spCustRates)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsMarketDataChanged, m_bMarketDataChanged);
	STDMETHOD(StartRealtime)(); 
	STDMETHOD(RequestLastQuotes)(); 

	IMPLEMENT_OBJECT_PROPERTY(IEtsMain*, EtsMain, m_spEtsMain)
	IMPLEMENT_OBJECT_PROPERTY(IBatchPriceProvider*, BatchPriceProvider, m_spBatchPriceProvider)


	IMPLEMENT_OBJECT_PROPERTY(IEtsMmFilterAtomColl*,	VisibleExp,     m_spVisibleExp)
	IMPLEMENT_OBJECT_PROPERTY(IEtsMmFilterAtomColl*,	VisibleRoot,    m_spVisibleRoot)
	IMPLEMENT_OBJECT_PROPERTY(IEtsMmFilterAtomColl*,	VisibleUndExch, m_spVisibleUndExch)
	IMPLEMENT_OBJECT_PROPERTY(IEtsMmFilterAtomColl*,	VisibleOptExch, m_spVisibleOptExch)
	IMPLEMENT_OBJECT_PROPERTY(IEtsMmEntityAtomColl*,	VisibleStr,     m_spVisibleStr)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, UndQuotesCount, m_nUndQuotesCount);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptQuotesCount, m_nOptQuotesCount);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, FutQuotesCount, m_nFutQuotesCount);

	IMPLEMENT_SIMPLE_PROPERTY(LONG, UndResponses, m_nUpdatedUndCount);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptResponses, m_nUpdatedOptCount);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, FutResponses, m_nUpdatedFutCount);



	IMPLEMENT_BSTRT_PROPERTY(ConnectionString, m_bsConnectionString)

	STDMETHOD(Load)(LONG lGroupID); 
	STDMETHOD(LoadQuoteUpdates)(); 
	STDMETHOD(Cancel)(); 

	STDMETHOD(put_ProcessorsLoad)(LONG nProcessors);

	STDMETHOD(put_OptsColumnsOrder)(SAFEARRAY** colOrder);
	STDMETHOD(get_OptsRowData)(LONG lRow,  IMmQvRowData** ppVal);
	STDMETHOD(OptsSort)(MmQvOptColumnEnum enField, EtsSortingEnum enSort);
	STDMETHOD(OptsRefresh)(VARIANT_BOOL vbInitialize);
	STDMETHOD(put_Quote)(QuoteUpdateParams* Params, QuoteUpdateInfo* Info);
	STDMETHOD(CleanUp)(VARIANT_BOOL vbAll);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsFilterData*, TradeFilter, m_spTradeFilter)

};

OBJECT_ENTRY_AUTO(__uuidof(EtsMmQuotesView), CEtsMmQuotesView)
