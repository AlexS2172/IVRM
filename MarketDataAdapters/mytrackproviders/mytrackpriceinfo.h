// MyTrackPriceInfo.h : Declaration of the CMyTrackPriceInfo

#ifndef __MYTRACKPRICEINFO_H_
#define __MYTRACKPRICEINFO_H_

#include "resource.h"       // main symbols
#include "MyTrackProviders.h"
#include "BaseNotifier.h"
#include "common.h"
#include "MyTrackCore.h"

#include <map>
#include <vector>

class CResponce
{
public:
	enum enType {enUnknown, enLastQuote, enQuoteUpdate, enQuoteSubscribed } m_enType;
	_bstr_t  m_bsFullSymbol;
	long     m_lStartTime;
	long     m_lSkip;
	CComVariant  m_vtRequest;
	CComVariant  m_vtResponce;

	CResponce():m_enType(enUnknown),m_lSkip(0){ m_lStartTime = GetTickCount(); };
	CResponce(const CResponce& resp){Copy(resp);};
	CResponce& operator=(const CResponce& resp){Copy(resp); return *this;};

private:
	void Copy(const CResponce& resp)
	{
		m_enType =resp.m_enType;
		m_bsFullSymbol = resp.m_bsFullSymbol;
		m_vtRequest = resp.m_vtRequest;
		m_vtResponce = resp.m_vtResponce;
		m_lStartTime = resp.m_lStartTime;
		m_lSkip = resp.m_lSkip;

	}

};
/////////////////////////////////////////////////////////////////////////////
// CMyTrackPriceInfo
class ATL_NO_VTABLE CMyTrackPriceInfo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMyTrackPriceInfo, &CLSID_MyTrackPriceProvider>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_MYTRACKPROVIDERSLib>,
	public IDispatchImpl<IMyTrackInfo, &IID_IMyTrackInfo, &LIBID_MYTRACKPROVIDERSLib>,
	public CBaseNotifier,
	public IConnectionPointImpl<CMyTrackPriceInfo, &DIID__IPriceInfoEvents, CComDynamicUnkArray>,
	public IConnectionPointContainerImpl<CMyTrackPriceInfo>
{
		typedef std::map<_bstr_t, CComVariant> SYMBOLSTORE;
		typedef std::vector<CResponce>         RESPONCE;
public:
	CMyTrackPriceInfo()
	{
		m_hResponce = CreateEvent(NULL, FALSE,FALSE,NULL);
		m_hStop = CreateEvent(NULL, TRUE, FALSE, NULL);

		unsigned id = 0;
		m_hThread = (HANDLE)_beginthreadex(NULL,0,MsgThread, this, 0, &id);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYTRACKPRICEINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMyTrackPriceInfo)
	COM_INTERFACE_ENTRY2(IDispatch, IMyTrackInfo)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IMyTrackInfo)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMyTrackPriceInfo)
	CONNECTION_POINT_ENTRY(DIID__IPriceInfoEvents)
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

	virtual ClientType GetClientType(){return enClientPriceInfo;};
	virtual ResponseType GetResponseType(){return enResponseLazy;};

	
	virtual HRESULT OnLastQuote(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, CComVariant &varRequest);
	
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

private:
	static SYMBOLSTORE m_SymbolsCache;

	RESPONCE    m_Responce;

	HANDLE      m_hResponce;
	HANDLE      m_hStop;
	HANDLE      m_hThread;

	CComAutoCriticalSection m_csResponce;

	void LastQuote(CComVariant &varParams, CComVariant &varResults);
	void QuoteUpdate(CComVariant &varParams, CComVariant &varResults);
	static UINT WINAPI MsgThread(LPVOID pData);
	static CComAutoCriticalSection m_csCache;

};

#endif //__MYTRACKPRICEINFO_H_
