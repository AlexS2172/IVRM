#include "StdAfx.h"
#include "Messenger.h"
#include "ActivFeedConnection.h"
//--------------------------------------------------------------------------//
_ATL_FUNC_INFO CMessenger::m_fiOnTradeUpdate = 
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
		if (Data){

			QuoteRequestPtr spRequest(new QuoteRequest);
			
			_bstr_t bsSymbol = Data->GetSymbol();
			char* __symbol = _com_util::ConvertBSTRToString(bsSymbol.GetBSTR());
			spRequest->Symbol = __symbol;
			delete [] __symbol;			
			
			if (Data->GetSymbolType() == enStStock){
				spRequest->Type = enSTK;
			}
			else if (Data->GetSymbolType() == enStIndex){
				spRequest->Type = enIDX;
			}
			else if (Data->GetSymbolType() == enStOption){
				spRequest->Type = enOPT;

				QuoteRequestPtr spBaseContractRequest(new QuoteRequest);

				_bstr_t bsSymbol = Data->GetUndSymbol();
				char* __symbol = _com_util::ConvertBSTRToString(bsSymbol.GetBSTR());
				spBaseContractRequest->Symbol = __symbol;
				delete [] __symbol;
				
				long lBaseContractType = CDataBase::getInstance()->getContractType(spBaseContractRequest->Symbol);
				
				if (lBaseContractType > 0){

					if (lBaseContractType == 1)
						spBaseContractRequest->Type = enIDX;
					else if (lBaseContractType == 2)
						spBaseContractRequest->Type = enSTK;

					if (CProviderSingleton::GetProvider()){
						if (CProviderSingleton::GetProvider()->Connect())
							CProviderSingleton::GetProvider()->Subscribe(spBaseContractRequest);
					}
				}

			}
			else if (Data->GetSymbolType() == enStFuture){
				spRequest->Type = enFUT;
			}
			else if (Data->GetSymbolType() == enStFutureOption){
				spRequest->Type = enFOP;
			}

			if (CProviderSingleton::GetProvider()){
				if (CProviderSingleton::GetProvider()->Connect())
					CProviderSingleton::GetProvider()->Subscribe(spRequest);
			}
		}
	}
	catch (_com_error& err)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("ComError: OnTradeUpdate() ")<<(char*)(err.Description()));
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("Unknown error occured while raw_OnTradeUpdate()"));
		ATLASSERT(FALSE);
	};
	return S_OK;
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
	catch (_com_error& err)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("ComError: SubscribeTradeUpdate() ")<<(char*)(err.Description()));
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("Unknown error occured while SubscribeTradeUpdate()"));
		ATLASSERT(FALSE);
	};
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
	catch (_com_error& err)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("ComError: UnSubscribeTradeUpdate() ")<<(char*)(err.Description()));
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("Unknown error occured while UnSubscribeTradeUpdate()"));
		ATLASSERT(FALSE);
	};
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
	catch (_com_error& err)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("ComError: GetSubManager() ")<<(char*)(err.Description()));
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("Unknown error occured while GetSubManager()"));
		ATLASSERT(FALSE);
	};
	return NULL;
};
//--------------------------------------------------------------------------//