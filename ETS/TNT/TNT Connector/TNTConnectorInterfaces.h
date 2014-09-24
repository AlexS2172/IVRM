#pragma once


[
	object,
	uuid("3ED1F72B-48C6-436d-BC2D-F09833980283"),
	dual,
	helpstring("IMMHHTAdapter Interface"),
	pointer_default(unique)
]
__interface IMMHHTAdapter : IDispatch
{
	[id(1), helpstring("method Logon")] HRESULT Logon();
	[id(2), helpstring("method Logoff")] HRESULT Logoff();

	[id(3), helpstring("method TradeAdd")] HRESULT TradeAdd([in]TNTData Data, [out] MessageDescriptor *pMessage);
	//	[id(4), helpstring("method TradeNotification")] HRESULT TradeNotification([in]TNTData Data, [out] MessageDescriptor *pMessage);	
};

[
	dispinterface
	uuid(317D27B9-62EB-4d15-9B1F-E1FE56F51B0F),
	helpstring("_IMMHHTAdapterEvents Interface")
]
 __interface _IMMHHTAdapterEvents
{
properties:
methods:
	[id(1), helpstring("method TerminateConnection")] HRESULT TerminateConnection();
	[id(2), helpstring("method TerminateConnection")] HRESULT ErrorMessage([in]MessageDescriptor Message, [in]HRESULT sCode, [in] BSTR Description);
	[id(3), helpstring("method TerminateConnection")] HRESULT TradeNotification([in]TNTData Data);
	[id(4), helpstring("method TerminateConnection")] HRESULT LogonResponse([in]HRESULT sCode, [in] SAFEARRAY(struct ErrorDescription)* saErrors);
	[id(5), helpstring("method TerminateConnection")] HRESULT LogoffResponse([in]HRESULT sCode, [in] SAFEARRAY(struct ErrorDescription)* saErrors);
	[id(6), helpstring("method TerminateConnection")] HRESULT TradeAddResponse([in]MessageDescriptor Message, [in]HRESULT sCode, [in] SAFEARRAY(struct ErrorDescription)* saErrors);
	//	[id(7), helpstring("method TerminateConnection")] HRESULT TradeNotifficationResponse([in]MessageDescriptor Message, [in]HRESULT sCode, [in] SAFEARRAY(struct ErrorDescription)* saErrors);
};
