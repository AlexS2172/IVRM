// PPBatchInfo.h : Declaration of the CPPBatchInfo

#pragma once
#include "resource.h"       // main symbols
#include "PriceProviderPerfCounter.h"


// IPPBatchInfo
[
	object,
	uuid("76CC39ED-C213-4368-8D7A-9DB1C0E92A7A"),
	dual,	helpstring("IPPBatchInfo Interface"),
	pointer_default(unique)
]
__interface IPPBatchInfo : IDispatch
{

	[id(1), helpstring("method Initialize")] HRESULT Initialize(BSTR UserName);
	[id(2), helpstring("method AddRealTimeUpdate")] HRESULT AddRealTimeUpdate(void);
	[id(3), helpstring("method IncrementIncomingCounter")] HRESULT AddClientRealTimeUpdate(void);
	[id(4), helpstring("method AddProcessedUpdate")] HRESULT AddProcessedUpdate(void);

	[propput, id(5), helpstring("property RTClients")] HRESULT RTClients([in] ULONG newVal);
	[propput, id(6), helpstring("property Clients")] HRESULT Clients([in] ULONG newVal);
	[propput, id(7), helpstring("property QuoteUpdatesQueueLength")] HRESULT QuoteUpdatesQueueSnapshot([in] ULONG newVal);
	[id(8), helpstring("method AddGroupRequestResponce")] HRESULT AddGroupRequestResponce(void);
	[id(9), helpstring("method AddMultipleRequestResponce")] HRESULT AddMultipleRequestResponce(void);
	[id(10), helpstring("method AddMultipleUnsubscriptionResponce")] HRESULT AddMultipleUnsubscriptionResponce(void);
	[id(11), helpstring("method AddClientUpdate")] HRESULT AddClientUpdate(void);
	[id(12), helpstring("method AddSubscription")]    HRESULT AddSubscription(void);
	[id(13), helpstring("method DeleteSubscription")] HRESULT DeleteSubscription(int iNumber);
};

// CPPBatchInfo
[
	coclass,
	threading("free"),
	support_error_info("IPPBatchInfo"),
	aggregatable("never"),
	vi_progid("PPPerformanceObjects.PPBatchInfo"),
	progid("PPPerformanceObjects.PPBatchInfo.1"),
	version(1.0),
	uuid("55E76C4F-B135-4801-A3EF-835584847826"),
	helpstring("PPBatchInfo Class")
]
class ATL_NO_VTABLE CPPBatchInfo : 
	public IPPBatchInfo
{
public:
	CPPBatchInfo()
		:m_pObject(NULL)
	{
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		if(m_pObject)
			m_Counter.ReleaseInstance(m_pObject);
	}

private:
	PriceProviderPerfCounter m_Counter;
	PriceProviderPerfCounterObject* m_pObject;

public:

	STDMETHOD(Initialize)(BSTR UserName);
	STDMETHOD(AddRealTimeUpdate)(void);
	STDMETHOD(AddClientRealTimeUpdate)(void);

	STDMETHOD(put_RTClients)(ULONG newVal);
	STDMETHOD(put_Clients)(ULONG newVal);
	STDMETHOD(put_QuoteUpdatesQueueSnapshot)(ULONG newVal);

	STDMETHOD(AddProcessedUpdate)(void);
	STDMETHOD(AddGroupRequestResponce)(void);
	STDMETHOD(AddMultipleRequestResponce)(void);
	STDMETHOD(AddMultipleUnsubscriptionResponce)(void);
	STDMETHOD(AddClientUpdate)(void);
	STDMETHOD(AddSubscription)(void);
	STDMETHOD(DeleteSubscription)(int iNumber);
};

