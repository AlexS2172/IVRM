// ATFProvider.h : Declaration of the CATFProvider

#pragma once
#include "resource.h"       // main symbols

#include "ATFDataProvider.h"
#include "ATFBaseFeed.h"


// CATFProvider

class ATL_NO_VTABLE CATFProvider : 
	public CATFBaseFeed<CATFProvider>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CATFProvider, &CLSID_ATFProvider>,
	public ISupportErrorInfoImpl<&IID_IATFProvider>,
	public IConnectionPointContainerImpl<CATFProvider>,
	public IDispatchImpl<IATFProvider, &IID_IATFProvider, &LIBID_ATFDATAPROVIDERLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	IConnectionPointImpl<CATFProvider, &__uuidof(_IATFProviderEvents), CComDynamicUnkArray>
{
public:
	CATFProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATFPROVIDER)


BEGIN_COM_MAP(CATFProvider)
	COM_INTERFACE_ENTRY(IATFProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CATFProvider)
	CONNECTION_POINT_ENTRY(__uuidof(_IATFProviderEvents))
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

	void PublicGroupData(const CComVariant& request, PB* pData, int iLotSize, bool bLast);
	void PublicStockData(const CComVariant& request, PB* pbData, ATFI_FUNDAMENTAL *pFD);


	HRESULT OnStock(const CComVariant &varParams,const  CComVariant &varResults);
	HRESULT OnOption(const CComVariant &varParams,const  CComVariant &varResults, const CComVariant &varIsLastOption);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);

public:

	STDMETHOD(Connect)(void);
	STDMETHOD(Disconnect)(void);
	STDMETHOD(RequestStock)(StockParams* Params);
	STDMETHOD(RequestOptions)(StockParams* Params, ATProviderRequestType enType);
	STDMETHOD(CancelStocks)(VARIANT vtRequest);
	STDMETHOD(CancelOptions)(VARIANT vtRequest);
};

OBJECT_ENTRY_AUTO(__uuidof(ATFProvider), CATFProvider)
