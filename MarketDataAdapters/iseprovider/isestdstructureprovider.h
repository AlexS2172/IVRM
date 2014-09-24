// ISEStructureProvider.h : Declaration of the CISEStructureProvider

#ifndef __ISESTDSTRUCTUREPROVIDER_H_
#define __ISESTDSTRUCTUREPROVIDER_H_

#include "resource.h"       // main symbols
#include "ISEEvents.h"
#include "ObjectID.h"
/////////////////////////////////////////////////////////////////////////////
// CISEStdStructureProvider
class ATL_NO_VTABLE CISEStdStructureProvider : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEStdStructureProvider, &CLSID_ISEStdStructureProvider>,
	public ISupportErrorInfoImpl<&IID_IStructureProvider>,
	public IConnectionPointContainerImpl<CISEStdStructureProvider>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_ISEPROVIDERLib>,
	public IConnectionPointImpl<CISEStdStructureProvider, &DIID__IStructureProviderEvents, CComDynamicUnkArray>,
    public CISEStructureBase
{
public:
	CISEStdStructureProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISESTDSTRUCTUREPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEStdStructureProvider)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISEStdStructureProvider)
	CONNECTION_POINT_ENTRY(DIID__IStructureProviderEvents)
END_CONNECTION_POINT_MAP()

	void FinalRelease() 
	{
		EgTrace("CISEStdStructureProvider::Release");
		Disconnect();
	}

// ISupportsErrorInfo
	//STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// CP
	virtual HRESULT OnStock(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastStock );
	virtual HRESULT OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);
    virtual HRESULT	OnEvent(const EventNumberEnum EventNumber, _bstr_t Description);

// IStructureProvider
public:

	STDMETHOD(RequestStock)(StockParams* Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams* Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
};

#endif //__ISESTDSTRUCTUREPROVIDER_H_
