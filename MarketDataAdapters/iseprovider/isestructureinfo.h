// ISEStructureInfo.h : Declaration of the CISEStructureInfo

#ifndef __ISESTRUCTUREINFO_H_
#define __ISESTRUCTUREINFO_H_

#include "resource.h"       // main symbols
#include "ISEEvents.h"

/////////////////////////////////////////////////////////////////////////////
// CISEStructureInfo
class ATL_NO_VTABLE CISEStructureInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEStructureInfo, &CLSID_ISEStructureInfo>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CISEStructureInfo>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_ISEPROVIDERLib>,
	IConnectionPointImpl<CISEStructureInfo, &DIID__IMyTrackStructureInfoEvents, CComDynamicUnkArray>
{
public:
	CISEStructureInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISESTRUCTUREINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEStructureInfo)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISEStructureInfo)
	CONNECTION_POINT_ENTRY(DIID__IMyTrackStructureInfoEvents)
END_CONNECTION_POINT_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// CP
	HRESULT OnStock(const CComVariant &varParams,const  CComVariant &varResults);
	HRESULT OnOption(const CComVariant &varParams,const  CComVariant &varResults, const CComVariant &varIsLastOption);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
						RequestsTypeEnum enumRequest,const CComVariant &varRequest);

// IISEStructureInfo
public:
	STDMETHOD(RequestStock)(StockParams* Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams* Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
};

#endif //__ISESTRUCTUREINFO_H_
