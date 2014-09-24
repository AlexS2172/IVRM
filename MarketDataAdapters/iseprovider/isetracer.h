// ISEInfo.h : Declaration of the CISETracer

#ifndef __ISETRACER_H_
#define __ISETRACER_H_

#include "resource.h"       // main symbols
#include "ObjectID.h"
/////////////////////////////////////////////////////////////////////////////
// CISETracer
class ATL_NO_VTABLE CISETracer : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISETracer, &CLSID_ISETracer>,
	public ISupportErrorInfoImpl<&IID_IISETracer>,
	public IConnectionPointContainerImpl<CISETracer>,
	public IDispatchImpl<IISETracer, &IID_IISETracer, &LIBID_ISEPROVIDERLib>,
	IConnectionPointImpl<CISETracer, &DIID__IISETracerEvents, CComDynamicUnkArray>,
	public CObjectID
{
public:
	CISETracer()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISEINFO)
DECLARE_CLASSFACTORY_SINGLETON(CISETracer)
DECLARE_PROTECT_FINAL_CONSTRUCT()


BEGIN_COM_MAP(CISETracer)
	COM_INTERFACE_ENTRY(IISETracer)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISETracer)
	CONNECTION_POINT_ENTRY(DIID__IISETracerEvents)
END_CONNECTION_POINT_MAP()

	void FinalRelease() 
	{
		EgTrace("CISETracer::Unbind");
		Unbind();
	}

// ISupportsErrorInfo
	//STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// CP
	HRESULT OnMessage(const ISEInfoTypeEnum Type,const BSTR bsText);
	HRESULT OnHeartbeat(const long lInstance);

// IISETracer
public:

	STDMETHOD(Bind)();
	STDMETHOD(Unbind)();
};

#endif //__ISETRACER_H_
