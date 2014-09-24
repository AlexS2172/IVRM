// ISEStructureInfo.h : Declaration of the CISEStructureInfo

#ifndef __ISESTDSTRUCTUREINFO_H_
#define __ISESTDSTRUCTUREINFO_H_

#include "resource.h"       // main symbols
#include "ISEEvents.h"
#include "ObjectID.h"
/////////////////////////////////////////////////////////////////////////////
// CISEStdStructureInfo
class ATL_NO_VTABLE CISEStdStructureInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEStdStructureInfo, &CLSID_ISEStdStructureProvider>,
	public ISupportErrorInfoImpl<&IID_IStructureProvider>,
	public IConnectionPointContainerImpl<CISEStdStructureInfo>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_ISEPROVIDERLib>,
	public IConnectionPointImpl<CISEStdStructureInfo, &DIID__IMyTrackStructureInfoEvents, CComDynamicUnkArray>,
    public CISEStructureBase
{
public:
	CISEStdStructureInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISESTDSTRUCTUREINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEStdStructureInfo)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CISEStdStructureInfo)
	CONNECTION_POINT_ENTRY(DIID__IMyTrackStructureInfoEvents)
END_CONNECTION_POINT_MAP()

	void FinalRelease() 
	{
		EgTrace("CISEStdStructureInfo::Release");
		Disconnect();
	}

// ISupportsErrorInfo
//	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

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

#endif //__ISESTDSTRUCTUREINFO_H_
