// ISEStructureProvider.h : Declaration of the CISEStructureProvider

#ifndef __ISESTRUCTUREPROVIDER_H_
#define __ISESTRUCTUREPROVIDER_H_

#include "resource.h"       // main symbols
#include "ISEEvents.h"
#include "ObjectID.h"
/////////////////////////////////////////////////////////////////////////////
// CISEStructureProvider
class ATL_NO_VTABLE CISEStructureProvider : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEStructureProvider, &CLSID_ISEStructureProvider>,
	public ISupportErrorInfoImpl<&IID_IISEStructureProvider>,
	public IConnectionPointContainerImpl<CISEStructureProvider>,
	public IDispatchImpl<IISEStructureProvider, &IID_IISEStructureProvider, &LIBID_ISEPROVIDERLib>,
	public IConnectionPointImpl<CISEStructureProvider, &DIID__IISEStructureProviderEvents, CComDynamicUnkArray>,
    public CISEStructureBase
{
public:
	CISEStructureProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISESTRUCTUREPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEStructureProvider)
	COM_INTERFACE_ENTRY(IISEStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISEStructureProvider)
	CONNECTION_POINT_ENTRY(DIID__IISEStructureProviderEvents)
END_CONNECTION_POINT_MAP()

	void FinalRelease() 
	{
		EgTrace("CISEStructureProvider::Release");
		Disconnect();
	}

// ISupportsErrorInfo
	//STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// CP
	virtual HRESULT OnStock(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastStock);
	virtual HRESULT OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);
	virtual HRESULT	OnEvent(const EventNumberEnum EventNumber, _bstr_t Description);

// IISEStructureProvider
public:

	STDMETHOD(RequestStock)(StockParams* Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestAllStocks)();
	STDMETHOD(CancelAllStocks)();
	STDMETHOD(RequestOptions)(OptionParams* Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
};

#endif //__ISESTRUCTUREPROVIDER_H_
