// ISEMMManager.h : Declaration of the CISEMMManager

#ifndef __ISEMMManager_H_
#define __ISEMMManager_H_

#include "resource.h"       // main symbols
#include "ObjectID.h"

/////////////////////////////////////////////////////////////////////////////
// CISEMMManager
class ATL_NO_VTABLE CISEMMManager : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CISEMMManager, &CLSID_ISEMMManager>,
	public ISupportErrorInfoImpl<&IID_IISEMMManager>,
	public IConnectionPointContainerImpl<CISEMMManager>,
	public IDispatchImpl<IISEMMManager, &IID_IISEMMManager, &LIBID_ISEPROVIDERLib>,
	IConnectionPointImpl<CISEMMManager, &DIID__IISEMMManagerEvents, CComDynamicUnkArray>,
	public CObjectID
{
public:
	CISEMMManager()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ISEMMMANAGER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CISEMMManager)
	COM_INTERFACE_ENTRY(IISEMMManager)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CISEMMManager)
CONNECTION_POINT_ENTRY(DIID__IISEMMManagerEvents)
END_CONNECTION_POINT_MAP()

	void FinalRelease() 
	{
		EgTrace("CISEMMManager::Release");
		Disconnect();
	}


// ISupportsErrorInfo
	//STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// CP

	HRESULT OnQuoteRejection(const CComVariant& varParams, bstr_t bsReason);
	HRESULT OnQuoteStepUp(const CComVariant& varParams);
	HRESULT OnQuoteTickWorse(const CComVariant& varParams, 
		long lTickWorseLevel, bstr_t bsReason);
	HRESULT OnQuotesSent(const CComVariant& varParams, 
		ErrorNumberEnum Error, _bstr_t bsOrderID);

	HRESULT OnLastMmQuote(const CComVariant &varParams, const CComVariant &varResults, const long TickWorseLevel);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);

	HRESULT OnDeal(const CComVariant& varParams);
	HRESULT OnTrade(const CComVariant& varParams);

	HRESULT	OnEvent(const EventNumberEnum EventNumber, _bstr_t Description);

// IISEMMManager
public:
	STDMETHOD(EnterOrder)(ISEOrder* Order, BSTR* OrderID);
	STDMETHOD(Disconnect)();
	STDMETHOD(Connect)();
	STDMETHOD(SetPmmParameters)(/*[in]*/ IMMParameters* Parameters);
	STDMETHOD(SetCmmParameters)(/*[in]*/ IMMParameters* Parameters);
	STDMETHOD(SubmitQuotes)(/*[in]*/ LPSAFEARRAY* saQuotes);	
	STDMETHOD(RequestLastMmQuote)(ISEMMQuoteParams* Params);
	STDMETHOD(CancelLastMmQuote)(VARIANT Params);
};

#endif //__ISEMMManager_H_
