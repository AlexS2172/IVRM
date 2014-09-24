// ATFStructureProvider.h : Declaration of the CATFStructureProvider

#ifndef __ATFSTRUCTUREPROVIDER_H_
#define __ATFSTRUCTUREPROVIDER_H_

#include "resource.h"       // main symbols
#include "ATFBaseFeed.h"

/////////////////////////////////////////////////////////////////////////////
// CATFStructureProvider
class ATL_NO_VTABLE CATFStructureProvider : 
	public CATFBaseFeed<CATFStructureProvider>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CATFStructureProvider, &CLSID_ATFStructureProvider>,
	public ISupportErrorInfoImpl<&IID_IStructureProvider>,
	public IConnectionPointContainerImpl<CATFStructureProvider>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_ATFDATAPROVIDERLib>,
	IConnectionPointImpl<CATFStructureProvider, &DIID__IStructureProviderEvents, CComDynamicUnkArray>
{
public:
	CATFStructureProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATFSTRUCTUREPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CATFStructureProvider)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CATFStructureProvider)
	CONNECTION_POINT_ENTRY(DIID__IStructureProviderEvents)
END_CONNECTION_POINT_MAP()

	HRESULT OnStock(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);


	ClientType GetClientType(){return _enClientStructureProvider;}

	void PublicOptionData(const CComVariant& request, PB* pData, int iLotSize, bool bLast);
	void PublicStockData(const CComVariant& request, PB* pbData, ATFI_FUNDAMENTAL *pFD); 

// IATFStructureProvider
public:

	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

};

#endif //__ATFSTRUCTUREPROVIDER_H_
