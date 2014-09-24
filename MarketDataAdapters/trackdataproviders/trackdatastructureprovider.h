// TrackDataStructureProvider.h : Declaration of the CTrackDataStructureProvider

#ifndef __TRACKDATASTRUCTUREPROVIDER_H_
#define __TRACKDATASTRUCTUREPROVIDER_H_

#include "resource.h"       // main symbols
#include "TrackDataCore.h"
#include "TrackDataProvidersCP.h"


/////////////////////////////////////////////////////////////////////////////
// CTrackDataStructureProvider
class ATL_NO_VTABLE CTrackDataStructureProvider : 
	public CBaseNotifier,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTrackDataStructureProvider, &CLSID_TrackDataStructureProvider>,
	public ISupportErrorInfoImpl<&__uuidof(IStructureProvider)>,
	public IConnectionPointContainerImpl<CTrackDataStructureProvider>,
	public IDispatchImpl<IStructureProvider, &__uuidof(IStructureProvider), &LIBID_TRACKDATAPROVIDERSLib>,
	public CProxy_IStructureProviderEvents< CTrackDataStructureProvider >
{
public:
	CTrackDataStructureProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRACKDATASTRUCTUREPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTrackDataStructureProvider)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CTrackDataStructureProvider)
CONNECTION_POINT_ENTRY(DIID__IStructureProviderEvents)
END_CONNECTION_POINT_MAP()

void FinalRelease()
{
	m_Client.Disconnect(this);
}
// ITrackDataStructureProvider
public:
	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	virtual ClientType GetClientType(){return 	enClientStructureProvider;};
	
	virtual HRESULT OnStock(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnOption(CComVariant &varParams, CComVariant &varResults, CComVariant &varIsLastOption);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, CComVariant &varRequest);

private:
	CTrackDataCore          m_Client;
	CComAutoCriticalSection m_csData;
};

#endif //__TRACKDATASTRUCTUREPROVIDER_H_
