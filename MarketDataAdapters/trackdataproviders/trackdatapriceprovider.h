// TrackDataPriceProvider.h : Declaration of the CTrackDataPriceProvider

#ifndef __TRACKDATAPRICEPROVIDER_H_
#define __TRACKDATAPRICEPROVIDER_H_

#include "resource.h"       // main symbols
#include "TrackDataCore.h"
#include "TrackDataProvidersCP.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackDataPriceProvider
class ATL_NO_VTABLE CTrackDataPriceProvider : 
	public CBaseNotifier,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTrackDataPriceProvider, &CLSID_TrackDataPriceProvider>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CTrackDataPriceProvider>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_TRACKDATAPROVIDERSLib>,
	public CProxy_IPriceProviderEvents< CTrackDataPriceProvider >
{
public:
	CTrackDataPriceProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRACKDATAPRICEPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTrackDataPriceProvider)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CTrackDataPriceProvider)
CONNECTION_POINT_ENTRY(DIID__IPriceProviderEvents)
END_CONNECTION_POINT_MAP()

void FinalRelease()
{
	m_Client.Disconnect(this);
}

	virtual ClientType GetClientType(){return 	enClientPriceProvider;};

	virtual HRESULT OnLastQuote(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, CComVariant &varRequest);

// ITrackDataPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

private:
	CTrackDataCore  m_Client;

	void GetParams(CComVariant &varResult, QuoteUpdateParams *Params)
	{
		_QuoteUpdateParams qup;
		qup.Init();
		qup->Symbol = SysAllocString(Params->Symbol);
		qup->Exchange = SysAllocString(Params->Exchange);
		qup->Type = Params->Type;
		qup.CopyTo (varResult);
	}
};

#endif //__TRACKDATAPRICEPROVIDER_H_
