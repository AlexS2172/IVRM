#include "StdAfx.h"
#include "Messenger.h"

//--------------------------------------------------------------------------//
_ATL_FUNC_INFO CMessenger::m_fiOnTradeUpdate = 
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };
_ATL_FUNC_INFO CMessenger::m_fiOnSettingsUpdate = 
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };
_ATL_FUNC_INFO CMessenger::m_fiOnUnderlyingUpdate = 
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };
//--------------------------------------------------------------------------//
CMessenger::CMessenger(void)
{
	m_spSubManager = NULL;
};
//--------------------------------------------------------------------------//
CMessenger::~CMessenger(void)
{
	
};
//--------------------------------------------------------------------------//
STDMETHODIMP CMessenger::raw_OnTradeUpdate(struct ITradeUpdate * Data)
{
	try
	{
		CTradePtr spTrade = CTradePtr(new CTrade);
		spTrade->Copy(Data);
		OnTradeUpdate(spTrade);
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
	return S_OK;
};
//--------------------------------------------------------------------------//
STDMETHODIMP CMessenger::raw_OnSettingsUpdate(struct ISettingsUpdate * Data)
{
	try
	{
		CCalculationSettingsPtr spSettings = CCalculationSettingsPtr(new CCalculationSettings);
		
		if (spSettings)
		{
			if (Data)
			{
				spSettings->SetGreeksCalculationMode(static_cast<IvRmCore::GreeksCalculationModeEnum>(Data->GetGreeksCalculationMode()));
				spSettings->SetGreeksCalculationModel(Data->GetGreeksCalculationModel());
				spSettings->SetUseTimePrecision(Data->GetGreeksCalculationPrecision() > 0 ? true : false);
				spSettings->SetUseTheoVolaIfNoBid(Data->GetUseTheoVolaIfNoBid() > 0 ? true : false);
				spSettings->SetUseTheoVolaIfBadMarketVola(Data->GetUseTheoVolaIfBadMarketVola() > 0 ? true : false);
				spSettings->SetPriceTolerance(Data->GetPriceTolerance());
				spSettings->SetPriceRoundingRule(Data->GetPriceRoundingRule());
				OnSettingsUpdate(spSettings);
			}
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
	return S_OK;
};
//--------------------------------------------------------------------------//
STDMETHODIMP CMessenger::raw_OnUnderlyingUpdate(struct IUnderlyingUpdate* Data)
{
	try
	{
		if (Data)
		{
			OnUnderlyingUpdate(Data);
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
	return S_OK;
};
//--------------------------------------------------------------------------//
void CMessenger::SubscribeUnderlyingUpdate()
{
	try
	{	
		ISubManagerPtr spSubManager = GetSubManager();

		if (static_cast<bool>(spSubManager))
		{
			HRESULT hr = S_OK; //= DispEventAdvise(spSubManager);
			//if(FAILED(hr))
			//	utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to DispEventAdvise.");

			hr = spSubManager->SubUnderlyingUpdate();
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to Subscribe TradeUpdate.");
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
};
//--------------------------------------------------------------------------//
void CMessenger::UnSubscribeUnderlyingUpdate()
{
	try
	{	
		ISubManagerPtr spSubManager = GetSubManager();

		if (static_cast<bool>(spSubManager))
		{
			HRESULT hr = spSubManager->UnsubUnderlyingUpdate();;
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to UnSubscribe TradeUpdate.");

			hr = DispEventUnadvise(spSubManager);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to DispEventUnadvise.");

			m_spSubManager = NULL;
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
};
//--------------------------------------------------------------------------//
void CMessenger::SubscribeTradeUpdate()
{
	try
	{	
		ISubManagerPtr spSubManager = GetSubManager();

		if (static_cast<bool>(spSubManager))
		{
			HRESULT hr = DispEventAdvise(spSubManager);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to DispEventAdvise.");

			hr = spSubManager->SubTradeUpdate();
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to Subscribe TradeUpdate.");
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
};
//--------------------------------------------------------------------------//
void CMessenger::UnSubscribeTradeUpdate()
{
	try
	{	
		ISubManagerPtr spSubManager = GetSubManager();

		if (static_cast<bool>(spSubManager))
		{
			HRESULT hr = spSubManager->UnsubTradeUpdate();;
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to UnSubscribe TradeUpdate.");

			hr = DispEventUnadvise(spSubManager);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to DispEventUnadvise.");

			m_spSubManager = NULL;
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
};
//--------------------------------------------------------------------------//
void CMessenger::SubscribeSettingsUpdate()
{
	try
	{	
		ISubManagerPtr spSubManager = GetSubManager();

		if (static_cast<bool>(spSubManager))
		{
			HRESULT hr = DispEventAdvise(spSubManager);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to DispEventAdvise.");

			hr = spSubManager->SubSettingsUpdate();
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to Subscribe TradeUpdate.");
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
};
//--------------------------------------------------------------------------//
void CMessenger::UnSubscribeSettingsUpdate()
{
	try
	{	
		ISubManagerPtr spSubManager = GetSubManager();

		if (static_cast<bool>(spSubManager))
		{
			HRESULT hr = spSubManager->UnsubSettingsUpdate();;
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to UnSubscribe TradeUpdate.");

			hr = DispEventUnadvise(spSubManager);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to DispEventUnadvise.");

			m_spSubManager = NULL;
		}
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
};
//--------------------------------------------------------------------------//
ISubManagerPtr CMessenger::GetSubManager()
{
	try
	{
		if (static_cast<bool>(m_spSubManager) == false)
		{
			HRESULT hr = m_spSubManager.CreateInstance(__uuidof (MsgManager));
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create MsgManager object.");
		}
		return m_spSubManager;
	}
	catch (_com_error&)
	{
		ATLASSERT(false);
	}
	return NULL;
};
//--------------------------------------------------------------------------//