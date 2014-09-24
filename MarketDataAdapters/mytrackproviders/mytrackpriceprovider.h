// MyTrackPriceProvider.h : Declaration of the CMyTrackPriceProvider

#ifndef __MYTRACKPRICEPROVIDER_H_
#define __MYTRACKPRICEPROVIDER_H_

#include "resource.h"       // main symbols
#include "MyTrackProviders.h"
#include "BaseNotifier.h"
#include "common.h"
#include "MyTrackCore.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTrackPriceProvider
class ATL_NO_VTABLE CMyTrackPriceProvider : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMyTrackPriceProvider, &CLSID_MyTrackPriceProvider>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_MYTRACKPROVIDERSLib>,
	public IDispatchImpl<IMyTrackInfo, &IID_IMyTrackInfo, &LIBID_MYTRACKPROVIDERSLib>,
	public CBaseNotifier,
	public IConnectionPointImpl<CMyTrackPriceProvider, &DIID__IPriceProviderEvents, CComDynamicUnkArray>,
	public IConnectionPointContainerImpl<CMyTrackPriceProvider>
{
public:
	CMyTrackPriceProvider()
	{
		m_enMode = enPPDefault;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYTRACKPRICEPROVIDER)
DECLARE_NOT_AGGREGATABLE(CMyTrackPriceProvider)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMyTrackPriceProvider)
	COM_INTERFACE_ENTRY2(IDispatch, IMyTrackInfo)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IMyTrackInfo)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMyTrackPriceProvider)
	CONNECTION_POINT_ENTRY(DIID__IPriceProviderEvents)
END_CONNECTION_POINT_MAP()

	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
public:
	void FinalRelease() 
	{
//		SetEvent(m_hStop);
		_Module.GetMyTrackCore ()->Disconnect(static_cast<CBaseNotifier*>(this));
/*
		m_Buffer.clear();

		CloseHandle(m_hStop);
		CloseHandle(m_hResponce);
		CloseHandle(m_hDisconnect;
*/
	}
	
	virtual HRESULT OnLastQuote(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, CComVariant &varRequest);

	virtual ClientType GetClientType(){return enClientPriceProvider;};
	
protected:
	void GetParams(CComVariant &varResult, QuoteUpdateParams *Params)
	{
		_QuoteUpdateParams qup;
		qup.Init();
		qup->Symbol = SysAllocString(Params->Symbol);
		qup->Exchange = SysAllocString(Params->Exchange);
		qup->Type = Params->Type;
		qup.CopyTo (varResult);
	}

	PriceProviderTypeEnum m_enMode;
public :
	STDMETHOD(get_PriceProviderMode)(/*[out, retval]*/ PriceProviderTypeEnum *pVal);
	STDMETHOD(put_PriceProviderMode)(/*[in]*/ PriceProviderTypeEnum newVal);
/*
private:

	HRESULT Send_LastQuote(CComVariant &varParams, CComVariant &varResults);
	HRESULT Send_QuoteUpdate(CComVariant &varParams, CComVariant &varResults);

	static UINT WINAPI _RespThread(LPVOID pData);
	RESPONCESTRUCT  m_Buffer;
	CComAutoCriticalSection m_csSend;
	CComAutoCriticalSection m_csEvent;

	HANDLE m_hStop;
	HANDLE m_hDisconnect;
	HANDLE m_hResponce;
*/	
};

#endif //__MYTRACKPRICEPROVIDER_H_
