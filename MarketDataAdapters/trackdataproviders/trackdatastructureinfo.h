// TrackDataStructureInfo.h : Declaration of the CTrackDataStructureInfo

#ifndef __TRACKDATASTRUCTUREINFO_H_
#define __TRACKDATASTRUCTUREINFO_H_

#include "resource.h"       // main symbols
#include "TrackDataCore.h"
#include "TrackDataProvidersCP.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackDataStructureInfo
class ATL_NO_VTABLE CTrackDataStructureInfo : 
	public CBaseNotifier,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTrackDataStructureInfo, &CLSID_TrackDataStructureInfo>,
	public ISupportErrorInfoImpl<&__uuidof(IStructureProvider)>,
	public IConnectionPointContainerImpl<CTrackDataStructureInfo>,
	public IDispatchImpl<IStructureProvider, &__uuidof(IStructureProvider), &LIBID_TRACKDATAPROVIDERSLib>,
	public CProxy_IMyTrackStructureInfoEvents< CTrackDataStructureInfo >
{
public:
	CTrackDataStructureInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRACKDATASTRUCTUREINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTrackDataStructureInfo)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CTrackDataStructureInfo)
CONNECTION_POINT_ENTRY(DIID__IMyTrackStructureInfoEvents)
END_CONNECTION_POINT_MAP()

void FinalRelease()
{
	m_Client.Disconnect(this);
}


	virtual ClientType GetClientType(){return 	enClientStructureInfo;};
	virtual HRESULT OnStock(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnOption(CComVariant &varParams, CComVariant &varResults, CComVariant &varIsLastOption);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
						RequestsTypeEnum enumRequest, CComVariant &varRequest);

// ITrackDataStructureProvider
public:
	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();


private:
	CTrackDataCore  m_Client;
};

#endif //__TRACKDATASTRUCTUREINFO_H_
