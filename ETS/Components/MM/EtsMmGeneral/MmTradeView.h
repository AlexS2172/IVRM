// MmTradeView.h : Declaration of the CMmTradeView

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmGeneral.h"
//#include "MmTradeChannel.h"
#include "_IMmTradeViewEvents_CP.h"


// CMmTradeView

typedef std::set<long> SelectionSet;
class CTradeViewSort
{
public:
	CTradeViewSort(TradesListColumnEnum enColumn, EtsSortingEnum enSortType)
		:m_enSortField(enColumn)
		,m_enSortType(enSortType){}


private:
	TradesListColumnEnum m_enSortField;
	EtsSortingEnum		 m_enSortType;
public:
	bool operator()(const IMmTradeInfoAtomPtr& Ptr1,  const IMmTradeInfoAtomPtr& sPtr2);
};

class ATL_NO_VTABLE CMmTradeView : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTradeView, &CLSID_MmTradeView>,
	public ISupportErrorInfoImpl<&IID_IMmTradeView>,
	public IConnectionPointContainerImpl<CMmTradeView>,
	public CProxy_IMmTradeViewEvents<CMmTradeView>, 
	public IDispatchImpl<IMmTradeView, &IID_IMmTradeView, &LIBID_EtsMmGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0> ,
	public IDispatchImpl<VSFLEX::IVSFlexDataSource, &__uuidof(VSFLEX::IVSFlexDataSource), &VSFLEX::LIBID_VSFLEX, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMmTradeView()
		:m_spTradeChannel(NULL)
		,m_enSortField(TLC_DATE)
		,m_enSortType(enSortDescending)
		,m_lVisibleColumns(0L)
	{
		ZeroMemory(m_ColumnsEncoding, sizeof(m_ColumnsEncoding));
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMTRADEVIEW)


	BEGIN_COM_MAP(CMmTradeView)
		COM_INTERFACE_ENTRY(IMmTradeView)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(VSFLEX::IVSFlexDataSource)
		COM_INTERFACE_ENTRY2(IDispatch, IMmTradeView)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CMmTradeView)
		CONNECTION_POINT_ENTRY(__uuidof(_IMmTradeViewEvents))
	END_CONNECTION_POINT_MAP()
	// ISupportsErrorInfo

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		try {
			_CHK(m_spFilterData.CreateInstance(__uuidof(EtsFilterData) ), _T("Failed to create filter data"));
			_CHK(m_spFilterData->Initialize(TFC_UNDERLYING,TFC_COLUMN_COUNT), _T("Failed to initialize filter"));
		}
		catch (_com_error& e) {
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmTradeView, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease() 
	{
		m_spFilterData = NULL;
		m_pUnkMarshaler.Release();
	}

public:

	CComPtr<IUnknown> m_pUnkMarshaler;

	STDMETHOD(raw_GetFieldCount) (/*[out,retval]*/ long * pFields );
	STDMETHOD(raw_GetRecordCount)(/*[out,retval]*/ long * pRecords );
	STDMETHOD(raw_GetFieldName)  (/*[in]*/ long Field, /*[out,retval]*/ BSTR * pFieldName );
	STDMETHOD(raw_GetData)		 (/*[in]*/ long Field, /*[in]*/ long Record, /*[out,retval]*/ BSTR * pData );
	STDMETHOD(raw_SetData)		 (/*[in]*/ long Field,	/*[in]*/ long Record,/*[in]*/ BSTR newData );


public:
	STDMETHOD(get_TradeViewFilter)(/*[in]*/ EtsTradesFilterEnum Filter, /*[out, retval]*/ LONG* pVal);	
	STDMETHOD(put_TradeViewFilter)(/*[in]*/ EtsTradesFilterEnum Filter, /*[in]*/ LONG newVal);
	IMPLEMENT_OBJECTREADONLY_PROPERTY( IEtsFilterData*, FilterData, m_spFilterData);
	STDMETHOD(put_TradeViewColumnsOrder)(SAFEARRAY** colOrder);
	STDMETHOD(Sort)(TradesListColumnEnum enField, EtsSortingEnum enSort);
	IMPLEMENT_OBJECT_PROPERTY(IEtsMain*, EtsMain, m_spEtsMain)
	IMPLEMENT_OBJECT_PROPERTY(IMmTradeChannel*, TradeChannel, m_spTradeChannel)

	STDMETHOD(Refresh)()
	{
		try
		{
			m_vecShow.clear();
			FilterData(m_spFilterData, m_vecShow);
			PerformSorting();
		}
		catch (_com_error& err)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(err), IID_IMmTradeView, err.Error());
		}
		return S_OK;
	}

	STDMETHOD(ClearSelected)()
	{
		m_SelSet.clear();
		return S_OK;
	}
	STDMETHOD(SetTradeAsSelected)(LONG TradeID)
	{
		m_SelSet.insert(TradeID);
		return S_OK;
	}
	STDMETHOD(IsSelectedTrade)(LONG TradeID, VARIANT_BOOL* bIsSel)
	{
		__CHECK_POINTER(bIsSel);

		*bIsSel = FALSE;

		if(m_SelSet.find(TradeID)!=m_SelSet.end())
		{
			*bIsSel = TRUE;
		}
		return S_OK;
	}

	STDMETHOD(get_RowData)(LONG lRow, IMmTradeInfoAtom** ppVal);

private:
	IMmTradeChannelPtr		m_spTradeChannel;
	long					m_ColumnsEncoding[TLC_COLUMN_COUNT];
	CTradeInfoViewData		m_vecShow;
	SelectionSet			m_SelSet;

	TradesListColumnEnum	m_enSortField;
	EtsSortingEnum			m_enSortType;
	long					m_lVisibleColumns;
	IEtsFilterDataPtr		m_spFilterData;
	IEtsMainPtr				m_spEtsMain;

private:
	void PerformSorting()
	{
		if(!m_vecShow.empty())
		{
			CTradeViewSort sortingClass(m_enSortField, m_enSortType);
			std::sort(m_vecShow.begin(), m_vecShow.end(), sortingClass);
		}
	}
public:
	void SetTradeChannel(IMmTradeChannelPtr spDataSource){ m_spTradeChannel = spDataSource;}
	void FilterData(IEtsFilterDataPtr spFilter, CTradeInfoViewData& refTrades);
};

OBJECT_ENTRY_AUTO(__uuidof(MmTradeView), CMmTradeView)
