// FixProvider.cpp : Implementation of CFixProvider
#include "stdafx.h"
#include "FixProviders.h"
#include "FixProvider.h"

_ATL_FUNC_INFO CFixProviderNotify::m_OnCancelReject =
	{CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_RECORD} };

_ATL_FUNC_INFO CFixProviderNotify::m_OnExecutionReport =
	{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };

_ATL_FUNC_INFO CFixProviderNotify::m_OnOrderError =
{CC_STDCALL, VT_EMPTY, 2, {VT_BSTR, VT_BSTR} };

_ATL_FUNC_INFO CFixProviderNotify::m_OnDisconnect =
{CC_STDCALL, VT_EMPTY, 1, {VT_I4} };

_ATL_FUNC_INFO CFixProviderNotify::m_OnReLogon =
{CC_STDCALL, VT_EMPTY, 1, {VT_I4} };

_ATL_FUNC_INFO CFixProviderNotify::m_OnLogonFailed =
{CC_STDCALL, VT_EMPTY, 2, {VT_I4, VT_BSTR} };

STDMETHODIMP CFixProviderNotify::OnCancelReject(BSTR Reason, FoOrderCancelReject* Report)
{
	if (m_pClient)
	{
		m_pClient->Fire_OnCancelReject(Reason, Report);
	}
	return S_OK;
}

STDMETHODIMP CFixProviderNotify::OnExecutionReport(FoExecutionReport* Report)
{
	if (m_pClient)
	{
		m_pClient->Fire_OnExecutionReport(Report);
	}
	return S_OK;
}

STDMETHODIMP CFixProviderNotify::OnOrderError(BSTR ClOrdID, BSTR Description)
{
	if (m_pClient)
	{
		m_pClient->Fire_OnOrderError(ClOrdID, Description);
	}
	return S_OK;
}

STDMETHODIMP CFixProviderNotify::OnDisconnect(long ProviderType)
{
	if (m_pClient)
	{
		m_pClient->Fire_OnDisconnect(ProviderType);
	}
	return S_OK;
}

STDMETHODIMP CFixProviderNotify::OnReLogon(long ProviderType)
{
	if (m_pClient)
	{
		m_pClient->Fire_OnReLogon(ProviderType);
	}
	return S_OK;
}

STDMETHODIMP CFixProviderNotify::OnLogonFailed(long ProviderType, BSTR Description)
{
	if (m_pClient)
	{
		m_pClient->Fire_OnLogonFailed(ProviderType, Description);
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CFixProvider

STDMETHODIMP CFixProvider::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] =
	{
		&IID_IOrderProvider
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CFixProvider::SetType(long NewId)
{
	HRESULT hr = S_OK;

	if (m_FixProviderId != NewId)
	{
		if (m_bConnected)
			hr = Error("FixProvider already active. Unable to activate it twice.", IID_IOrderProvider, E_ABORT);
		else
		{
			m_Notify.Destroy();
			m_pOrderProvider = 0;
			m_FixProviderId = NewId;

			if (m_FixProviderId < 0)
				m_pOrderProvider = 0;
			else
			{
				_bstr_t bsProgId;
				if(g_FixProviders.GetProgID(m_FixProviderId, bsProgId))
				{
					CLSID cls;
					IID iid = __uuidof(IOrderProvider);	// FOR DEBUGGING
					CLSIDFromProgID(bsProgId, &cls);
					hr = CoCreateInstance(cls, 0, CLSCTX_ALL, iid, (PVOID*)&m_pOrderProvider);

					if FAILED(hr)
						return Error("Error occurred during CoCreateInstance call", IID_IOrderProvider, hr);
				}
				else
				{
					hr = Error(_T("Unknown FixProvider type"), IID_IOrderProvider, E_INVALIDARG);
				}
			}

			if(SUCCEEDED(hr) && m_pOrderProvider != NULL)
			{
				m_Notify.Initialize(m_pOrderProvider, this);
				m_bConnected = true;
			}
		}
	}

	return hr;
}

STDMETHODIMP CFixProvider::GetType(long * CurrType)
{
	return E_NOTIMPL;
}

STDMETHODIMP CFixProvider::Connect()
{
	if (!m_bConnected)
		return Error("FixProvider is in \"Disconnected\" mode", IID_IOrderProvider, E_FAIL);

	if (!m_pOrderProvider)
		return Error("Flag \"Connection\" has been set, but underlying interface pointer is invalid (NULL)",
					IID_IOrderProvider, E_UNEXPECTED);

	HRESULT hr = m_pOrderProvider->Connect();

	if SUCCEEDED(hr)
		m_bSessionEstablished = true;

	return hr;
}

STDMETHODIMP CFixProvider::Disconnect()
{
	if (!m_bConnected)
		return Error("FixProvider is in \"Disconnected\" mode", IID_IOrderProvider, E_FAIL);

	if (!m_pOrderProvider)
		return Error("Flag \"Connection\" has been set, but underlying interface pointer is invalid (NULL)",
					IID_IOrderProvider, E_UNEXPECTED);

	HRESULT hr = S_OK;
	if (m_bSessionEstablished)
		hr = m_pOrderProvider->Disconnect();

	m_bSessionEstablished = false;
	return hr;
}

STDMETHODIMP CFixProvider::SubmitNewOrder(FoOrder* Order, BSTR* OrderID)
{
	if (!m_bConnected)
		return Error("FixProvider is in \"Disconnected\" mode", IID_IOrderProvider, E_FAIL);

	if (!m_pOrderProvider)
		return Error("Flag \"Connection\" has been set, but underlying interface pointer is invalid (NULL)",
					IID_IOrderProvider, E_UNEXPECTED);

	return m_pOrderProvider->SubmitNewOrder(Order, OrderID);
}

STDMETHODIMP CFixProvider::AlterOrder(BSTR OrderID, FoOrderCancelReplaceRequest* Order, BSTR* AlteredOrderID)
{
	if (!m_bConnected)
		return Error("FixProvider is in \"Disconnected\" mode", IID_IOrderProvider, E_FAIL);

	if (!m_pOrderProvider)
		return Error("Flag \"Connection\" has been set, but underlying interface pointer is invalid (NULL)",
					IID_IOrderProvider, E_UNEXPECTED);

	return m_pOrderProvider->AlterOrder(OrderID, Order, AlteredOrderID);
}

STDMETHODIMP CFixProvider::CancelOrder(BSTR OrderID, FoOrderCancelRequest* Order, BSTR* CanceledOrderID)
{
	if (!m_bConnected)
		return Error("FixProvider is in \"Disconnected\" mode", IID_IOrderProvider, E_FAIL);

	if (!m_pOrderProvider)
		return Error("Flag \"Connection\" has been set, but underlying interface pointer is invalid (NULL)",
					IID_IOrderProvider, E_UNEXPECTED);

	return m_pOrderProvider->CancelOrder(OrderID, Order, CanceledOrderID);
}

STDMETHODIMP CFixProvider::RequestOrderStatus(BSTR OrderID, FoOrderStatusRequest* Order)
{
	if (!m_bConnected)
		return Error("FixProvider is in \"Disconnected\" mode", IID_IOrderProvider, E_FAIL);

	if (!m_pOrderProvider)
		return Error("Flag \"Connection\" has been set, but underlying interface pointer is invalid (NULL)",
					IID_IOrderProvider, E_UNEXPECTED);

	return m_pOrderProvider->RequestOrderStatus(OrderID, Order);
}

HRESULT CFixProvider::FinalConstruct()
{
	m_pOrderProvider = 0;
	m_bConnected = false;
	m_bRecursive = false;
	m_bSessionEstablished = false;
	m_FixProviderId = - 777;

	return S_OK;
}

void CFixProvider::FinalRelease()
{
	Disconnect();

	if (m_pOrderProvider)
		m_pOrderProvider->Release();

	m_pOrderProvider = 0;
}
