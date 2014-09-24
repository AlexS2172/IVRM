// ISEInfo.h : Declaration of the CISEInfo

#ifndef __ISEINFO_H_
#define __ISEINFO_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CISEInfo
class ATL_NO_VTABLE CISEInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEInfo, &CLSID_ISEInfo>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CISEInfo>,
	public IDispatchImpl<IISEInfo, &IID_IISEInfo, &LIBID_ISEPROVIDERLib>,
	IConnectionPointImpl<CISEInfo, &DIID__IISEInfoEvents, CComDynamicUnkArray>
{
public:
	CISEInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISEINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEInfo)
	COM_INTERFACE_ENTRY(IISEInfo)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISEInfo)
	CONNECTION_POINT_ENTRY(DIID__IISEInfoEvents)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// CP
	HRESULT OnEvent(const ISEInfoTypeEnum Type,const BSTR bsText);

// IISEInfo
public:

	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
};

#endif //__ISEINFO_H_
