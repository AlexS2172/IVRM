// HyperFeedStructureInfo.h : Declaration of the CHyperFeedStructureInfo

#ifndef __HYPERFEEDSTRUCTUREINFO_H_
#define __HYPERFEEDSTRUCTUREINFO_H_

#include "resource.h"       // main symbols
#include "HyperFeedBase.h"
#include "MasterOptions.h"
#include "Options.h"
#include "Underlyings.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedStructureInfo
class ATL_NO_VTABLE CHyperFeedStructureInfo : 
	public CHyperFeedBase<CHyperFeedStructureInfo>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHyperFeedStructureInfo, &CLSID_HyperFeedStructureInfo>,
	public ISupportErrorInfoImpl<&IID_IStructureProvider>,
	public IConnectionPointContainerImpl<CHyperFeedStructureInfo>,
	public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IConnectionPointImpl<CHyperFeedStructureInfo, &DIID__IMyTrackStructureInfoEvents, CComDynamicUnkArray>,
	public CMasterOptions,
	public COptions,
	public CUnderlyings
{
public:
	CHyperFeedStructureInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HYPERFEEDSTRUCTUREINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHyperFeedStructureInfo)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHyperFeedStructureInfo)
	CONNECTION_POINT_ENTRY(DIID__IMyTrackStructureInfoEvents)
END_CONNECTION_POINT_MAP()

// IStructureProvider
public:
	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	void Attach();
	void Detach();

	void OnResponse(CResponseBasePtr pResponse);
	void OnRequest(CRequestBasePtr pResponse);
private:
	HRESULT OnStock(const CComVariant &varParams,const  CComVariant &varResults);
	HRESULT OnOption(const CComVariant &varParams,const  CComVariant &varResults, const CComVariant &varIsLastOption);

	virtual void PublicResponse(CResponseBasePtr pResponse){ AddResponse(pResponse);}
	virtual void PublicGetOptionError(ErrorNumberEnum nError, BSTR bsDescription);
	virtual void PublicLastOption();
	virtual void PublicOption(DBA_OPTIONS_FILTER_RECORD* pData);
private:
	CComVariant m_vtCurrentRequest;
};

#endif //__HYPERFEEDSTRUCTUREINFO_H_
