// ATFStructureInfo.h : Declaration of the CATFStructureInfo

#ifndef __ATFSTRUCTUREINFO_H_
#define __ATFSTRUCTUREINFO_H_

#include "resource.h"       // main symbols
#include "ATFBaseFeed.h"

/////////////////////////////////////////////////////////////////////////////
// CATFStructureInfo
class ATL_NO_VTABLE CATFStructureInfo : 
	public CATFBaseFeed<CATFStructureInfo>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CATFStructureInfo, &CLSID_ATFStructureInfo>,
	public ISupportErrorInfoImpl<&IID_IStructureProvider>,
	public IConnectionPointContainerImpl<CATFStructureInfo>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_ATFDATAPROVIDERLib>,
	IConnectionPointImpl<CATFStructureInfo, &DIID__IMyTrackStructureInfoEvents, CComDynamicUnkArray>

{
public:
	CATFStructureInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATFSTRUCTUREINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CATFStructureInfo)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CATFStructureInfo)
	CONNECTION_POINT_ENTRY(DIID__IMyTrackStructureInfoEvents)
END_CONNECTION_POINT_MAP()


	ClientType GetClientType(){return _enClientStructureInfo;}
	void PublicOptionData(const CComVariant& request, PB* pData, int LotSize, bool bLast);
	void PublicStockData(const CComVariant& request, PB* pbData, ATFI_FUNDAMENTAL *pFD);


	HRESULT OnStock(const CComVariant &varParams,const  CComVariant &varResults);
	HRESULT OnOption(const CComVariant &varParams,const  CComVariant &varResults, const CComVariant &varIsLastOption);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
						RequestsTypeEnum enumRequest,const CComVariant &varRequest);

// IATFStructureInfo
public:
	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	HRESULT QuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);

	PriceProviderTypeEnum m_enMode;
//	CTrackDataCore  m_Client;

	void GetParams(CComVariant &varResult, QuoteUpdateParams *Params)
	{
		_QuoteUpdateParams qup;
		qup.Init();
		qup->Symbol = SysAllocString(Params->Symbol);
		qup->Exchange = SysAllocString(Params->Exchange);
		qup->Type = Params->Type;
		qup.CopyTo (varResult);
	}

};

#endif //__ATFSTRUCTUREINFO_H_
