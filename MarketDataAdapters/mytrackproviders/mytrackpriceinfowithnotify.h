// MyTrackPriceInfoWithNotify.h : Declaration of the CMyTrackPriceInfoWithNotify

#ifndef __MYTRACKPRICEINFOWITHNOTIFY_H_
#define __MYTRACKPRICEINFOWITHNOTIFY_H_

#include "resource.h"       // main symbols
#include "MyTrackProviders.h"
#include "BaseNotifier.h"
#include "common.h"
#include "MyTrackCore.h"
#include "MyTrackPriceInfo.h"

#include <map>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CMyTrackPriceInfoWithNotify
class ATL_NO_VTABLE CMyTrackPriceInfoWithNotify : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMyTrackPriceInfoWithNotify, &CLSID_MyTrackPriceInfoWithNotify>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CMyTrackPriceInfoWithNotify>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_MYTRACKPROVIDERSLib>,
	public IDispatchImpl<IMyTrackInfo, &IID_IMyTrackInfo, &LIBID_MYTRACKPROVIDERSLib>,
	public CBaseNotifier,
	public IConnectionPointImpl<CMyTrackPriceInfoWithNotify, &DIID__IPriceInfoWithNotifyEvents, CComDynamicUnkArray>
{
	typedef std::map<_bstr_t, CComVariant> SYMBOLSTORE;
	typedef std::vector<CResponce>         RESPONCE;
public:
	CMyTrackPriceInfoWithNotify()
	{
		m_hResponce = CreateEvent(NULL, FALSE,FALSE,NULL);
		m_hStop = CreateEvent(NULL, TRUE, FALSE, NULL);

		unsigned id = 0;
		m_hThread = (HANDLE)_beginthreadex(NULL,0,MsgThread, this, 0, &id);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MYTRACKPRICEINFOWITHNOTIFY)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CMyTrackPriceInfoWithNotify)
		COM_INTERFACE_ENTRY2(IDispatch, IMyTrackInfo)
		COM_INTERFACE_ENTRY(IPriceProvider)
		COM_INTERFACE_ENTRY(IMyTrackInfo)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CMyTrackPriceInfoWithNotify)
			CONNECTION_POINT_ENTRY(DIID__IPriceInfoWithNotifyEvents)
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
		_Module.GetMyTrackCore ()->Disconnect(static_cast<CBaseNotifier*>(this));
		SetEvent(m_hStop);

		WaitWithEvents(m_hThread);
		CloseHandle(m_hResponce);
		CloseHandle(m_hStop);
	}

	virtual ClientType GetClientType(){return enClientPriceInfoWithNotify;};
	virtual ResponseType GetResponseType(){return enResponseLazy;};


	virtual HRESULT OnLastQuote(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
		RequestsTypeEnum enumRequest, CComVariant &varRequest);
	virtual HRESULT OnSubscribed(CComVariant &varParams);

protected:
	void GetParams(CComVariant &varResult, QuoteUpdateParams *Params)
	{
		_QuoteUpdateParams qup;
		qup.Init(Params);
		qup.CopyTo (varResult);
	}

	PriceProviderTypeEnum m_enMode;
public :
	STDMETHOD(get_PriceProviderMode)(/*[out, retval]*/ PriceProviderTypeEnum *pVal);
	STDMETHOD(put_PriceProviderMode)(/*[in]*/ PriceProviderTypeEnum newVal);

private:
	static SYMBOLSTORE m_SymbolsCache;

	RESPONCE    m_Responce;

	HANDLE      m_hResponce;
	HANDLE      m_hStop;
	HANDLE      m_hThread;

	CComAutoCriticalSection m_csResponce;

	void LastQuote(CComVariant &varParams, CComVariant &varResults);
	void QuoteUpdate(CComVariant &varParams, CComVariant &varResults);
	void Subscribed(CComVariant &varParams);
	static UINT WINAPI MsgThread(LPVOID pData);
	static CComAutoCriticalSection m_csCache;

};

#endif //__MYTRACKPRICEINFOWITHNOTIFY_H_
