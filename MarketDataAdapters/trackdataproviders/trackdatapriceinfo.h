// TrackDataPriceInfo.h : Declaration of the CTrackDataPriceInfo

#ifndef __TRACKDATAPRICEINFO_H_
#define __TRACKDATAPRICEINFO_H_

#include "resource.h"       // main symbols
#include "TrackDataCore.h"
#include "TrackDataProvidersCP.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackDataPriceInfo
class ATL_NO_VTABLE CTrackDataPriceInfo : 
	public CBaseNotifier,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTrackDataPriceInfo, &CLSID_TrackDataPriceInfo>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CTrackDataPriceInfo>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_TRACKDATAPROVIDERSLib>,
	public CProxy_IPriceInfoEvents< CTrackDataPriceInfo >
{
public:
	CTrackDataPriceInfo()
	{
		m_enMode = enPPDefault;
	}

	PriceProviderTypeEnum m_enMode;

DECLARE_REGISTRY_RESOURCEID(IDR_TRACKDATAPRICEINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTrackDataPriceInfo)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CTrackDataPriceInfo)
CONNECTION_POINT_ENTRY(DIID__IPriceInfoEvents)
END_CONNECTION_POINT_MAP()

void FinalRelease()
{
	m_Client.Disconnect(this);
}

	virtual ClientType GetClientType(){return enClientPriceInfo;};
	virtual ResponseType GetResponseType(){return enResponseLazy;};
	
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

#endif //__TRACKDATAPRICEINFO_H_
