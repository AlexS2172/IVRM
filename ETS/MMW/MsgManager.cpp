// MsgManager.cpp : Implementation of CMsgManager
#include "stdafx.h"
#include "MMW.h"
#include "MsgManager.h"

#include <TransportServer\Constants.h>

#include "XMLParamsHelper.h"

//void LoadAspSettings()
//{
//	g_bstrSubjectPrefix = _bstr_t(L"");
//
//	CXMLParamsHelper XMLParams;
//	XMLParams.LoadXMLParams();
//
//	BSTR bsSubectPrefix;
//	XMLParams.GetUserGroup(&bsSubectPrefix);
//	g_bstrSubjectPrefix.Attach(bsSubectPrefix);
//}


// ---------------------------------------------------------------------

#define ERR_INVALID_SUBJECT		L"Can't generate correct subject"

#define CASE_IMPL1(_object)													        \
		case enMt##_object:															\
			{																		\
				I##_object##Ptr		spObjectMessage;								\
				if (FAILED(spObjectMessage.CreateInstance(L"MsgStruct."L#_object)))	\
					return S_OK;													\
																					\
				spUnPacker = spObjectMessage;										\
																					\
				if (spUnPacker == NULL)												\
					return S_OK;													\
																					\
				if (FAILED(spUnPacker->Unpack(Message)))							\
					return S_OK;													\
																					\
				Fire_On##_object(spObjectMessage);									\
																					\
				return S_OK;														\
			}
													    
#define CASE_IMPL2(_method, _object)												\
		case enMt##_method:															\
			{																		\
				I##_object##Ptr		spObjectMessage;								\
				if (FAILED(spObjectMessage.CreateInstance(L"MsgStruct."L#_object)))	\
					return S_OK;													\
																					\
				spUnPacker = spObjectMessage;										\
																					\
				if (spUnPacker == NULL)												\
					return S_OK;													\
																					\
				if (FAILED(spUnPacker->Unpack(Message)))							\
					return S_OK;													\
																					\
				Fire_On##_method(spObjectMessage);									\
																					\
				return S_OK;														\
			}
													    
//////////////////////////////////////////////////////////////////////////////

_ATL_FUNC_INFO CMsgManager::m_OnMessageParams = {CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_BSTR}};
_ATL_FUNC_INFO CMsgManager::m_OnLogonParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};
_ATL_FUNC_INFO CMsgManager::m_OnLogoffParams = {CC_STDCALL, VT_EMPTY, 1, {VT_BSTR}};
_ATL_FUNC_INFO CMsgManager::m_OnSubjectStatus = {CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_UI4}};

//////////////////////////////////////////////////////////////////////////////

CMsgManager::CMsgManager()  :
    m_SessionEvents(this)
{

}

STDMETHODIMP CMsgManager::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IPubManager,
		&IID_ISubManager
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}

	return S_FALSE;
}

HRESULT CMsgManager::FinalConstruct()
{
	//LoadAspSettings();

	g_bstrSubjectPrefix = _bstr_t(L"");
	{
		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		BSTR bsSubectPrefix;
		XMLParams.GetUserGroup(&bsSubectPrefix);
		g_bstrSubjectPrefix.Attach(bsSubectPrefix);
	}

	if(g_bstrSubjectPrefix.length() == 0)
		return Error(L"Can't get user group from xml settings", IID_ISubManager, E_INVALIDARG);

	HRESULT	hRes = S_OK;
    Lock();

    if (FAILED(hRes = m_Transport.CreateInstance(L"EgTpPrx.HostProxy")))
    {
        Unlock();
	    return Error(L"Can't access to transport server", IID_ISubManager, hRes);
    }

    if (FAILED(hRes = m_Transport->Logon(L"")))
    {
        Unlock();
	    return Error(L"Can't start transport server", IID_ISubManager, hRes);
    }

    if (FAILED(hRes = m_Transport->CreateSession(&m_Session)) || m_Session == NULL)
    {
        Unlock();
	    return Error(L"Can't create new transport session", IID_ISubManager, hRes);
    }

    // Advise for events
    if (FAILED(hRes = m_SessionEvents.DispEventAdvise(m_Session)))
    {
        Unlock();
	    return Error(L"Advise to session failed", IID_ISubManager, hRes);
    }

    Unlock();
	return hRes;
}

void CMsgManager::FinalRelease()
{
    Lock();

    if (m_Session != NULL)
	    m_SessionEvents.DispEventUnadvise(m_Session);
    m_Session = NULL;

    Unlock();
}

/**********************************************************************************************/
const wstring CMsgManager::m_subjMtVola(L"V");
const wstring CMsgManager::m_subjMtTradeUpdate(L"Trade");
const wstring CMsgManager::m_subjMtPriceUpdate(L"Price");
const wstring CMsgManager::m_subjMtUnderlyingUpdate(L"Underlying");
const wstring CMsgManager::m_subjMtBroadcastMessage(L"BRMSG");
const wstring CMsgManager::m_subjMtRequestNewOrder(L"ORDN");
const wstring CMsgManager::m_subjMtAlterOrder(L"ORDR");
const wstring CMsgManager::m_subjMtCancelOrder(L"ORDC");
const wstring CMsgManager::m_subjMtRequestOrderStatus(L"ORDS");
const wstring CMsgManager::m_subjMtOrder(L"NTFORDO");
const wstring CMsgManager::m_subjMtExecutionReport(L"NTFORDE");
const wstring CMsgManager::m_subjMtFixGatewayStatusRequest(L"FIXGSR");
const wstring CMsgManager::m_subjMtFixGatewayStatus(L"FIXGS");
const wstring CMsgManager::m_subjMtDynamicMessage(L"DYNMSG");
const wstring CMsgManager::m_subjMtActiveFuturesChange(L"AFTCHG");
const wstring CMsgManager::m_subjMtFlexOption(L"NFORTD");
const wstring CMsgManager::m_subjMtManualPriceUpdate(L"MPUPDATE");
const wstring CMsgManager::m_subjMtSettingsUpdate(L"SETTINGSUPDATE");
/**********************************************************************************************/

_bstr_t CMsgManager::GenerateSubject(MESSAGE_TYPE Type, ISymbolObject* pSymbolObject)
{
	HRESULT		hRes = S_OK;
	_bstr_t	RetVal;

	if(g_bstrSubjectPrefix.length() > 0)
	{
		RetVal = g_bstrSubjectPrefix;
		RetVal += L"/";
	}

	switch (Type)
	{
		case enMtVola:
			RetVal += m_subjMtVola.c_str();
			break;
		
		case enMtTradeUpdate:
			RetVal += m_subjMtTradeUpdate.c_str();
			return RetVal;

		case enMtPriceUpdate:
			RetVal += m_subjMtPriceUpdate.c_str();
			return RetVal;

		case enMtUnderlyingUpdate:
			RetVal += m_subjMtUnderlyingUpdate.c_str();
			return RetVal;

		case enMtManualPriceUpdate:
			RetVal += m_subjMtManualPriceUpdate.c_str();
			return RetVal;

		case enMtSettingsUpdate:
			RetVal += m_subjMtSettingsUpdate.c_str();
			return RetVal;

		case enMtBroadcastMessage:
			RetVal += m_subjMtBroadcastMessage.c_str();
			return RetVal;

		case enMtRequestNewOrder:
			RetVal += m_subjMtRequestNewOrder.c_str();
			break;
		
		case enMtAlterOrder:
			RetVal += m_subjMtAlterOrder.c_str();
			break;
		
		case enMtCancelOrder:
			RetVal += m_subjMtCancelOrder.c_str();
			break;
		
		case enMtRequestOrderStatus:
			RetVal += m_subjMtRequestOrderStatus.c_str();
			break;
		
		case enMtOrder:
			RetVal += m_subjMtOrder.c_str();
			break;
		
		case enMtExecutionReport:
			RetVal += m_subjMtExecutionReport.c_str();
			break;

		case enMtFixGatewayStatusRequest:
			RetVal += m_subjMtFixGatewayStatusRequest.c_str();
			break;

		case enMtFixGatewayStatus:
			RetVal += m_subjMtFixGatewayStatus.c_str();
			break;

		case enMtDynamicMessage:
			RetVal += m_subjMtDynamicMessage.c_str();
			break;
		case enMtActiveFuturesChange:
			RetVal += m_subjMtActiveFuturesChange.c_str();
			break;
		case enMtFlexOption:
			RetVal += m_subjMtFlexOption.c_str();
			break;
		default:
			return L"";
	}

	if (pSymbolObject == NULL)
		return RetVal;

	CComBSTR	Symbol;

	hRes = pSymbolObject->GetString(&Symbol);

	if (FAILED(hRes))
		return L"";

	RetVal += L"/";
	RetVal += (BSTR)Symbol;

	return RetVal;
}

bool CMsgManager::DecodeSubject(BSTR Subject, MESSAGE_TYPE& Type, ISymbolObject* pSymbolObject)
{
	_bstr_t bsSubject(Subject);

	wstring	sSubject = bsSubject;
	if (sSubject.size() == 0)
		return false;

	if(g_bstrSubjectPrefix.length() > 0)
	{
		wstring sPrefix; 
		long nDelimPos = sSubject.find(L'/');

		if(nDelimPos != wstring::npos)
		{
			sPrefix   = sSubject.substr(0, nDelimPos);
			sSubject  = sSubject.substr(nDelimPos + 1, wstring::npos);
		}
		else
		{
			sPrefix = sSubject;
		}

		if(sPrefix != (BSTR)g_bstrSubjectPrefix)
			return false;
	}

	if (sSubject == m_subjMtTradeUpdate)
	{
		Type = enMtTradeUpdate;
		return true;
	}
	else if (sSubject == m_subjMtPriceUpdate)		
	{
		Type = enMtPriceUpdate;
		return true;
	}
	else if (sSubject == m_subjMtUnderlyingUpdate)		
	{
		Type = enMtUnderlyingUpdate;
		return true;
	}
	else if (sSubject == m_subjMtManualPriceUpdate)		
	{
		Type = enMtManualPriceUpdate;
		return true;
	}
	else if (sSubject == m_subjMtSettingsUpdate)		
	{
		Type = enMtSettingsUpdate;
		return true;
	}
	else if (sSubject == m_subjMtRequestNewOrder)		
	{
		Type = enMtRequestNewOrder;
		return true;
	}
	else if (sSubject == m_subjMtAlterOrder)		
	{
		Type = enMtAlterOrder;
		return true;
	}
	else if (sSubject == m_subjMtCancelOrder)		
	{
		Type = enMtCancelOrder;
		return true;
	}
	else if (sSubject == m_subjMtRequestOrderStatus)		
	{
		Type = enMtRequestOrderStatus;
		return true;
	}
	else if (sSubject == m_subjMtOrder)		
	{
		Type = enMtOrder;
		return true;
	}
	else if (sSubject == m_subjMtExecutionReport)		
	{
		Type = enMtExecutionReport;
		return true;
	}
    else if (sSubject == m_subjMtBroadcastMessage)
    {
		Type = enMtBroadcastMessage;
		return true;
    }
	else if (sSubject == m_subjMtFixGatewayStatusRequest)
	{
		Type = enMtFixGatewayStatusRequest;
		return true;
	}
	else if (sSubject == m_subjMtFixGatewayStatus)
	{
		Type = enMtFixGatewayStatus;
		return true;
	}
	else if (sSubject == m_subjMtDynamicMessage)
	{
		Type = enMtDynamicMessage;
		return true;
	}
	else if (sSubject == m_subjMtActiveFuturesChange)
	{
		Type = enMtActiveFuturesChange;
		return true;
	}
	else if (sSubject == m_subjMtFlexOption)
	{
		Type = enMtFlexOption;
		return true;
	}

	long nDelimPos = sSubject.find(L'/');

	wstring sType; 
	if (nDelimPos != wstring::npos)
		sType = sSubject.substr(0, nDelimPos);
	else
		sType = sSubject;

	if (m_subjMtVola == sType)
		Type = enMtVola;
	else
		return false;

	if (pSymbolObject == NULL)
		return false;

	_bstr_t Symbol = sSubject.substr(nDelimPos+1).c_str();

	if (FAILED(pSymbolObject->SetFromString(Symbol)))
		return false;

	return true;
}

HRESULT	CMsgManager::SendMessageToTransport(BSTR Subject, MESSAGE_TYPE enType, INetPacking* pPacker, HRESULT& Result)
{
	HRESULT hRes = S_OK;

	if (pPacker == NULL) 
		return E_FAIL;

	BSTR Message = NULL;
	
	hRes = pPacker->Pack(&Message);

	if (FAILED(hRes))
		return hRes;

	MsgPropertiesEnum enMsgProp;
	switch (enType)
	{
		case enMtFixGatewayStatusRequest:
		case enMtFixGatewayStatus:
			{
				enMsgProp = enMpNotBuffered;
			}
			break;

		case enMtVola:
		case enMtTradeUpdate:
		case enMtPriceUpdate:
		case enMtUnderlyingUpdate:
		case enMtManualPriceUpdate:
		case enMtSettingsUpdate:
			{
				enMsgProp = enMpGuarantee;
			}
			break;

		case enMtDynamicMessage:
		case enMtBroadcastMessage:
		case enMtRequestNewOrder:
		case enMtAlterOrder:
		case enMtCancelOrder:
		case enMtRequestOrderStatus:
		case enMtOrder:
		case enMtExecutionReport:
		case enMtActiveFuturesChange:
		case enMtFlexOption:
			{
				enMsgProp = enMpGuarantee;
				break;
			}


		default:
			enMsgProp = enMpUnspecified;
			break;
	}

    Lock();
	hRes = m_Session->SendTransportMessage(Subject, enMsgProp,Message);
    Unlock();

	if (hRes == EVENT_E_ALL_SUBSCRIBERS_FAILED)
	{
		Result = EVENT_E_ALL_SUBSCRIBERS_FAILED;
		hRes = S_OK;
	}
	else
		Result = hRes;

	SysFreeString(Message);

	return hRes;
}

HRESULT __stdcall CMsgManager::OnMessage(BSTR Subject, BSTR Message)
{
	CComBSTR	bstrSubject = Subject;

	MESSAGE_TYPE Type;
	
	ISymbolObjectPtr	spSymbolObject;
	if (FAILED(spSymbolObject.CreateInstance(L"MsgStruct.SymbolObject")))
		return S_OK;

	if (!DecodeSubject(bstrSubject, Type, spSymbolObject))
		return S_OK;

	INetPackingPtr spUnPacker = NULL;

	switch (Type)
	{
		case enMtVola:
			{
				IVMESurfacePtr		spVolaMessage;
				if (FAILED(spVolaMessage.CreateInstance(__uuidof(VMESurface))))
					return S_OK;

				spUnPacker = spVolaMessage;
			
				if (spUnPacker == NULL)
					return S_OK;

				if (FAILED(spUnPacker->Unpack(Message)))
					return S_OK;

				Fire_OnVola(spSymbolObject, spVolaMessage);
			
				return S_OK;
			}

		case enMtActiveFuturesChange:
			{
				IActiveFuturesChangePtr spActiveFutures;
				if (FAILED(spActiveFutures.CreateInstance(__uuidof(ActiveFuturesChange))))
					return S_OK;

				spUnPacker = spActiveFutures;
				if (spUnPacker == NULL)
					return S_OK;

				if (FAILED(spUnPacker->Unpack(Message)))
					return S_OK;

				Fire_OnActiveFuturesChange(spActiveFutures);
				return S_OK;

			}break;
		case enMtFlexOption:
			{
				IFlexOptionPtr spFlexOption;
				if (FAILED(spFlexOption.CreateInstance(__uuidof(FlexOption))))
					return S_OK;

				spUnPacker = spFlexOption;
				if (spUnPacker == NULL)
					return S_OK;

				if (FAILED(spUnPacker->Unpack(Message)))
					return S_OK;

				Fire_OnFlexOption(spFlexOption);
				return S_OK;
				
			}break;
		case enMtTradeUpdate:
			{
				ITradeUpdatePtr		spTradeUpdateMessage;
				if (FAILED(spTradeUpdateMessage.CreateInstance(__uuidof(TradeUpdate))))
					return S_OK;
			
				spUnPacker = spTradeUpdateMessage;
			
				if (spUnPacker == NULL)
					return S_OK;

				if (FAILED(spUnPacker->Unpack(Message)))
					return S_OK;

				Fire_OnTradeUpdate(spTradeUpdateMessage);

				return S_OK;
			}

		case enMtPriceUpdate:
			{
				IPriceUpdatePtr		spPriceUpdateMessage;
				if (FAILED(spPriceUpdateMessage.CreateInstance(__uuidof(PriceUpdate))))
					return S_OK;

				spUnPacker = spPriceUpdateMessage;

				if (spUnPacker == NULL)
					return S_OK;

				if (FAILED(spUnPacker->Unpack(Message)))
					return S_OK;

				Fire_OnPriceUpdate(spPriceUpdateMessage);

				return S_OK;
			}

		case enMtManualPriceUpdate:
			{
				IManualPriceUpdatePtr		spManualPriceUpdateMessage;
				if (FAILED(spManualPriceUpdateMessage.CreateInstance(__uuidof(ManualPriceUpdate))))
					return S_OK;

				spUnPacker = spManualPriceUpdateMessage;

				if (spUnPacker == NULL)
					return S_OK;

				if (FAILED(spUnPacker->Unpack(Message)))
					return S_OK;

				Fire_OnManualPriceUpdate(spManualPriceUpdateMessage);

				return S_OK;
			}

		case enMtSettingsUpdate:
			{
				ISettingsUpdatePtr		spSettingsUpdateMessage;
				if (FAILED(spSettingsUpdateMessage.CreateInstance(__uuidof(SettingsUpdate))))
					return S_OK;

				spUnPacker = spSettingsUpdateMessage;

				if (spUnPacker == NULL)
					return S_OK;

				if (FAILED(spUnPacker->Unpack(Message)))
					return S_OK;

				Fire_OnSettingsUpdate(spSettingsUpdateMessage);

				return S_OK;
			}

		case enMtUnderlyingUpdate:
			{
				IUnderlyingUpdatePtr		spUnderlyingUpdateMessage;
				if (FAILED(spUnderlyingUpdateMessage.CreateInstance(__uuidof(UnderlyingUpdate))))
					return S_OK;

				spUnPacker = spUnderlyingUpdateMessage;

				if (spUnPacker == NULL)
					return S_OK;

				if (FAILED(spUnPacker->Unpack(Message)))
					return S_OK;

				Fire_OnUnderlyingUpdate(spUnderlyingUpdateMessage);

				return S_OK;
			}

		CASE_IMPL1(DynamicMessage)

		CASE_IMPL1(BroadcastMessage)

		CASE_IMPL2(RequestNewOrder,		Order)
		CASE_IMPL2(AlterOrder,			Order)
		CASE_IMPL2(CancelOrder,			Order)
		CASE_IMPL2(RequestOrderStatus,	Order)
		CASE_IMPL1(Order)
		CASE_IMPL1(ExecutionReport)

		CASE_IMPL1(FixGatewayStatus)
		CASE_IMPL1(FixGatewayStatusRequest)
		default:
			return S_OK;
	}

	return S_OK;
}

HRESULT __stdcall CMsgManager::OnLogon(BSTR Router)
{
	return Fire_OnLogon(Router);
}

HRESULT __stdcall CMsgManager::OnLogoff(BSTR Router)
{
	return Fire_OnLogoff(Router);
}

HRESULT __stdcall CMsgManager::OnSubjectStatus(BSTR Subject, unsigned long ulNumSubscribers)
{
	CComBSTR	bstrSubject = Subject;

	MESSAGE_TYPE Type;
	
	ISymbolObjectPtr	spSymbolObject;
	if (FAILED(spSymbolObject.CreateInstance(L"MsgStruct.SymbolObject")))
		return S_OK;

	if (!DecodeSubject(bstrSubject, Type, spSymbolObject))
		return S_OK;

	return S_OK;
}

/**********************************************************************************************/

STDMETHODIMP CMsgManager::RegPublisher(MESSAGE_TYPE Type, ISymbolObject* Symbol)
{
    return S_OK;
}

STDMETHODIMP CMsgManager::UnregPublisher(MESSAGE_TYPE Type, ISymbolObject* Symbol)
{
    return S_OK;
}

/**********************************************************************************************

	VOLATILTY MESSAGE 

**********************************************************************************************/
STDMETHODIMP CMsgManager::SubVola(ISymbolObject* Symbol)
{
    if (Symbol == NULL)
        return E_INVALIDARG;

	return Subscribe(enMtVola, Symbol);
}

STDMETHODIMP CMsgManager::UnsubVola(ISymbolObject* Symbol)
{
    if (Symbol == NULL)
        return E_INVALIDARG;

	return Unsubscribe(enMtVola, Symbol);
}

STDMETHODIMP CMsgManager::PubVola(ISymbolObject* Symbol, IVMESurface* Data, long* Result)
{
	if (Symbol == NULL || Data == NULL)
		return E_INVALIDARG;

	_bstr_t bstrSubject = GenerateSubject(enMtVola, Symbol);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

	INetPackingPtr Packer = Data;

	return SendMessageToTransport(bstrSubject,enMtVola, Packer, *Result);
}

/**********************************************************************************************

	TRADEUPDATE MESSAGE 

**********************************************************************************************/

STDMETHODIMP CMsgManager::SubTradeUpdate()
{
	return Subscribe(enMtTradeUpdate, NULL);
}

STDMETHODIMP CMsgManager::UnsubTradeUpdate()
{
	return Unsubscribe(enMtTradeUpdate, NULL);
}

STDMETHODIMP CMsgManager::PubTradeUpdate(ITradeUpdate* Data, long* Result)
{
	if (Data == NULL)
		return E_INVALIDARG;

	_bstr_t bstrSubject = GenerateSubject(enMtTradeUpdate, NULL);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

	INetPackingPtr Packer = Data;

	return SendMessageToTransport(bstrSubject,enMtTradeUpdate, Packer, *Result);
}
/***********************************************************************************************/

STDMETHODIMP CMsgManager::SubActiveFuturesChange()
{
	return Subscribe(enMtActiveFuturesChange, NULL);
}

STDMETHODIMP CMsgManager::UnsubActiveFuturesChange()
{
	return Unsubscribe(enMtActiveFuturesChange, NULL);
}

STDMETHODIMP CMsgManager::PubActiveFuturesChange(IActiveFuturesChange* Data, long* Result)
{
	if (Data == NULL)
		return E_INVALIDARG;

	_bstr_t bstrSubject = GenerateSubject(enMtActiveFuturesChange, NULL);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

	INetPackingPtr Packer = Data;

	return SendMessageToTransport(bstrSubject, enMtActiveFuturesChange, Packer, *Result);
}

/************************************************************************
	FLEX OTION UPDATE MESSAGE
************************************************************************/

STDMETHODIMP CMsgManager::SubFlexOption()
{
	return Subscribe(enMtFlexOption, NULL);
}

STDMETHODIMP CMsgManager::UnsubFlexOption()
{
	return Unsubscribe(enMtFlexOption, NULL);
}

STDMETHODIMP CMsgManager::PubFlexOption(IFlexOption* Data, long* Result)
{
	if (Data == NULL)
		return E_INVALIDARG;

	_bstr_t bstrSubject = GenerateSubject(enMtFlexOption, NULL);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

	INetPackingPtr Packer = Data;

	return SendMessageToTransport(bstrSubject, enMtFlexOption, Packer, *Result);
}

/**********************************************************************************************

	PRICEUPDATE MESSAGE 

**********************************************************************************************/

STDMETHODIMP CMsgManager::SubPriceUpdate()
{
	return Subscribe(enMtPriceUpdate, NULL);
}

STDMETHODIMP CMsgManager::UnsubPriceUpdate()
{
	return Unsubscribe(enMtPriceUpdate, NULL);
}

STDMETHODIMP CMsgManager::SubUnderlyingUpdate()
{
	return Subscribe(enMtUnderlyingUpdate, NULL);
}

STDMETHODIMP CMsgManager::UnsubUnderlyingUpdate()
{
	return Unsubscribe(enMtUnderlyingUpdate, NULL);
}

STDMETHODIMP CMsgManager::SubExecutionReport()
{
	return Subscribe(enMtExecutionReport, NULL);
}

STDMETHODIMP CMsgManager::UnsubExecutionReport()
{
	return Unsubscribe(enMtExecutionReport, NULL);
}

STDMETHODIMP CMsgManager::PubPriceUpdate(IPriceUpdate* Data, long* Result)
{
	if (Data == NULL)
		return E_INVALIDARG;

	_bstr_t bstrSubject = GenerateSubject(enMtPriceUpdate, NULL);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

	INetPackingPtr Packer = Data;

	return SendMessageToTransport(bstrSubject, enMtPriceUpdate, Packer, *Result);
}

STDMETHODIMP CMsgManager::PubUnderlyingUpdate(IUnderlyingUpdate* Data, long* Result)
{
	if (Data == NULL)
		return E_INVALIDARG;

	_bstr_t bstrSubject = GenerateSubject(enMtUnderlyingUpdate, NULL);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

	INetPackingPtr Packer = Data;

	return SendMessageToTransport(bstrSubject, enMtUnderlyingUpdate, Packer, *Result);
}

STDMETHODIMP CMsgManager::PubManualPriceUpdate(IManualPriceUpdate* Data, long* Result)
{
	if (Data == NULL)
		return E_INVALIDARG;

	_bstr_t bstrSubject = GenerateSubject(enMtManualPriceUpdate, NULL);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

	INetPackingPtr Packer = Data;

	return SendMessageToTransport(bstrSubject, enMtManualPriceUpdate, Packer, *Result);
}

STDMETHODIMP CMsgManager::SubManualPriceUpdate()
{
	return Subscribe(enMtManualPriceUpdate, NULL);
}

STDMETHODIMP CMsgManager::UnsubManualPriceUpdate()
{
	return Unsubscribe(enMtManualPriceUpdate, NULL);
}

STDMETHODIMP CMsgManager::PubSettingsUpdate(ISettingsUpdate* Data, long* Result)
{
	if (Data == NULL)
		return E_INVALIDARG;

	_bstr_t bstrSubject = GenerateSubject(enMtSettingsUpdate, NULL);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

	INetPackingPtr Packer = Data;

	return SendMessageToTransport(bstrSubject, enMtSettingsUpdate, Packer, *Result);
}

STDMETHODIMP CMsgManager::SubSettingsUpdate()
{
	return Subscribe(enMtSettingsUpdate, NULL);
}

STDMETHODIMP CMsgManager::UnsubSettingsUpdate()
{
	return Unsubscribe(enMtSettingsUpdate, NULL);
}

REQUEST_METHODS_IMPL_EX(DynamicMessage)
REQUEST_METHODS_IMPL(BroadcastMessage)

PUBLISH_METHOD_IMPL(RequestNewOrder, Order)
PUBLISH_METHOD_IMPL(AlterOrder, Order)
PUBLISH_METHOD_IMPL(CancelOrder, Order)
PUBLISH_METHOD_IMPL(RequestOrderStatus, Order)
PUBLISH_METHOD_IMPL(Order, Order)
PUBLISH_METHOD_IMPL(ExecutionReport, ExecutionReport)

REQUEST_METHODS_IMPL(FixGatewayStatusRequest)
REQUEST_METHODS_IMPL(FixGatewayStatus)

HRESULT CMsgManager::Subscribe(	MESSAGE_TYPE enType, 
								ISymbolObject* pSymbol )
{
	HRESULT hRes = S_OK;
	
	_bstr_t bstrSubject = GenerateSubject(enType, pSymbol);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

    Lock();
	hRes = m_Session->Subscribe(bstrSubject);
    Unlock();

	return hRes;
}

// ---------------------------------------------------------------------
HRESULT CMsgManager::Unsubscribe(MESSAGE_TYPE	enType, ISymbolObject* pSymbol)
{
	HRESULT hRes = S_OK;
	_bstr_t bstrSubject = GenerateSubject(enType, pSymbol);

	if (!bstrSubject.length())
		return Error(ERR_INVALID_SUBJECT, IID_ISubManager, E_FAIL);

    Lock();
	hRes = m_Session->Unsubscribe(bstrSubject);
    Unlock();

	return hRes;
}


STDMETHODIMP CMsgManager::get_IsLogoned(VARIANT_BOOL *pVal)
{
    Lock();
	HRESULT hr = m_Session->get_IsLogoned(pVal);
    Unlock();

    return hr;
}

STDMETHODIMP CMsgManager::put_UserGroup(BSTR bsVal)
{
	Lock();
	g_bstrSubjectPrefix = _bstr_t(bsVal);
	Unlock();

	return S_OK;
}

STDMETHODIMP CMsgManager::SubOrderMessages()
{
    Lock();

	_bstr_t RetVal;
	if(g_bstrSubjectPrefix.length() > 0)
	{
		RetVal = g_bstrSubjectPrefix;
		RetVal += L"/";
	}
	RetVal += L"ORD?";

	HRESULT hRes = m_Session->Subscribe(RetVal);
    Unlock();

	return hRes;
}
STDMETHODIMP CMsgManager::UnsubOrderMessages()
{
	Lock();
	_bstr_t RetVal;
	if(g_bstrSubjectPrefix.length() > 0)
	{
		RetVal = g_bstrSubjectPrefix;
		RetVal += L"/";
	}
	RetVal += L"ORD?";

	HRESULT hRes = m_Session->Unsubscribe(RetVal);
	Unlock();

	return hRes;
}

STDMETHODIMP CMsgManager::SubOrdersNotificationMessages()
{
	Lock();
	_bstr_t RetVal;
	if(g_bstrSubjectPrefix.length() > 0)
	{
		RetVal = g_bstrSubjectPrefix;
		RetVal += L"/";
	}
	RetVal += L"NTFORD?";
	HRESULT hRes = m_Session->Subscribe(RetVal);
	Unlock();

	return hRes;
}
STDMETHODIMP CMsgManager::UnsubOrdersNotificationMessages()
{
	Lock();
	_bstr_t RetVal;
	if(g_bstrSubjectPrefix.length() > 0)
	{
		RetVal = g_bstrSubjectPrefix;
		RetVal += L"/";
	}
	RetVal += L"NTFORD?";

	HRESULT hRes = m_Session->Unsubscribe(RetVal);
	Unlock();

	return hRes;
}