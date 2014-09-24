#include "StdAfx.h"
#include "TradesCache.h"
//----------------------------------------------------------------------------------//
CTradesCache::CTradesCache(void)
{
	EgLib::CThread::Start();
	HRESULT hr = m_spTradeChannel.CreateInstance(__uuidof(MmTradeChannel));
	if(FAILED(hr))
		utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create object MmTradeChannel.");
}
//----------------------------------------------------------------------------------//
CTradesCache::~CTradesCache(void)
{
	EgLib::CThread::Stop(true, 5000);
	UnSubscribeTradeUpdate();
	m_spTradeChannel = NULL;
}
//----------------------------------------------------------------------------------//
void CTradesCache::Clear()
{

};
//----------------------------------------------------------------------------------//
void CTradesCache::SetSettingsImpl(CSettingsPtr spSettings)
{
	m_spSettings = spSettings;
};
//----------------------------------------------------------------------------------//
CSettingsPtr CTradesCache::GetSettingsImpl()
{
	return m_spSettings;
};
//----------------------------------------------------------------------------------//
void CTradesCache::LoadTrades()
{
	try
	{
		if (static_cast<bool>(m_spTradeChannel))
		{
			BSTR bstrConnectionString;
			if (GetSettingsImpl().get() == NULL)
				utils::ThrowErrorNoSetErrorInfo(E_POINTER, L"Failed to GetSettings.");

			GetSettingsImpl()->GetDBConnection(&bstrConnectionString);
			SetConnectionString(bstrConnectionString);

			HRESULT hr = m_spTradeChannel->put_DatabaseString(bstrConnectionString);

			hr = m_spTradeChannel->LoadTrades(0, GetMain(), VARIANT_FALSE);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to LoadTrades.");
		}
	}
	catch (_com_error& err){
		std::ostringstream out_stream;
		out_stream << "Exception occured while loading trades.";
		TRACE_COM_ERROR(LOG4CPLUS_ERROR, VS::Log, err, std::string(out_stream.str()));	
	}
	catch (...){
		std::ostringstream out_stream;
		out_stream<<"Unknown C++ exception occured while loading trades.";
		TRACE_UNKNOWN_ERROR(LOG4CPLUS_ERROR, VS::Log, std::string(out_stream.str()));
		ATLASSERT(FALSE);
	}
};
//----------------------------------------------------------------------------------//
BSTR CTradesCache::GetXMLString(IRisks* pRisks, CTradePtr spTrade)
{
	if (pRisks && spTrade)
	{
		IContractPtr spContract = spTrade->m_spContract;
		EtsContractTypeEnum enContractType = enCtNone;
		if (static_cast<bool>(spContract))
			spContract->get_ContractType(&enContractType);

		IContractPtr spBaseContarct = NULL;
		QuoteUpdateInfo retBaseContractQuotes;
		spContract->get_BaseContract(&spBaseContarct);
		if (static_cast<bool>(spBaseContarct))
			retBaseContractQuotes = spBaseContarct->GetQuotes();


		COleDateTime strDate(0.);
		const LPCTSTR dateFormat = _T("%Y-%m-%d %H:%M:%S");
		
		_bstr_t bstrXPath = L"<root>";

			bstrXPath += L"<Risks TradeID=\"" +		_bstr_t(spTrade->m_nTradeID) + "\" ";
			bstrXPath += L"ContractID=\"" +			_bstr_t(spTrade->m_nContractID) + "\" ";
			bstrXPath += L"TheoPrice=\"" +			_bstr_t(pRisks->TheoPrice) + "\" ";
			bstrXPath += L"Alpha=\"" +				_bstr_t(pRisks->Alpha) + "\" ";
			bstrXPath += L"Delta=\"" +				_bstr_t(pRisks->Delta) + "\" ";
			bstrXPath += L"Gamma=\"" +				_bstr_t(pRisks->Gamma) + "\" ";
			bstrXPath += L"Vega=\"" +				_bstr_t(pRisks->Vega) + "\" ";
			bstrXPath += L"Theta=\"" +				_bstr_t(pRisks->Theta) + "\" ";
			bstrXPath += L"Rho=\"" +				_bstr_t(pRisks->Rho) + "\" ";
			bstrXPath += L"Volga=\"" +				_bstr_t(pRisks->Volga) + "\" ";
			bstrXPath += L"DeltaVega=\"" +			_bstr_t(pRisks->DeltaVega) + "\" ";
			bstrXPath += L"DeltaTheta=\"" +			_bstr_t(pRisks->DeltaTheta) + "\" ";
			bstrXPath += L"GammaVega=\"" +			_bstr_t(pRisks->GammaVega) + "\" ";
			bstrXPath += L"GammaTheta=\"" +			_bstr_t(pRisks->GammaTheta) + "\" ";

			if (enContractType == enCtOption){

				bstrXPath += L"IVola=\"" +						_bstr_t(pRisks->IVola) + "\" ";
				bstrXPath += L"IVolaBid=\"" +					_bstr_t(pRisks->IVolaBid) + "\" ";
				bstrXPath += L"IVolaAsk=\"" +					_bstr_t(pRisks->IVolaAsk) + "\" ";
				bstrXPath += L"TheoVola=\"" +					_bstr_t(pRisks->TheoVola) + "\" ";
				bstrXPath += L"BaseContractPrice=\"" +			_bstr_t(pRisks->BaseContractPrice) + "\" ";

				if (retBaseContractQuotes.BidPrice > DBL_EPSILON){
					bstrXPath += L"BaseContractPriceBid=\"" +		_bstr_t(retBaseContractQuotes.BidPrice) + "\" "; 
				}

				if (retBaseContractQuotes.AskPrice > DBL_EPSILON){
					bstrXPath += L"BaseContractPriceAsk=\"" +		_bstr_t(retBaseContractQuotes.AskPrice) + "\" "; 
				}

				if (retBaseContractQuotes.LastPrice > DBL_EPSILON){
					bstrXPath += L"BaseContractPriceLast=\"" +		_bstr_t(retBaseContractQuotes.LastPrice) + "\" ";
				}

				if (retBaseContractQuotes.BidDateTime > DBL_EPSILON){				
					strDate = COleDateTime(retBaseContractQuotes.BidDateTime);
					bstrXPath += L"BaseContractPriceBidDate=\"" +		_bstr_t(strDate.Format(dateFormat)) + "\" ";
				}
				
				if (retBaseContractQuotes.AskDateTime > DBL_EPSILON){
					strDate = COleDateTime(retBaseContractQuotes.AskDateTime);
					bstrXPath += L"BaseContractPriceAskDate=\"" +		_bstr_t(strDate.Format(dateFormat)) + "\" "; 
				}

				if (retBaseContractQuotes.LastDateTime > DBL_EPSILON){
					strDate = COleDateTime(retBaseContractQuotes.AskDateTime);
					bstrXPath += L"BaseContractPriceLastDate=\"" +		_bstr_t(strDate.Format(dateFormat)) + "\" ";
				}
			};

			bstrXPath += L"ContractPrice=\"" +	_bstr_t(pRisks->ContractPrice) + "\" ";

			if (pRisks->BidPrice > DBL_EPSILON){
				bstrXPath += L"ContractPriceBid=\"" + _bstr_t(pRisks->BidPrice) + "\" "; 
			}

			if (pRisks->AskPrice > DBL_EPSILON){
				bstrXPath += L"ContractPriceAsk=\"" + _bstr_t(pRisks->AskPrice) + "\" "; 
			}

			if (pRisks->LastPrice > DBL_EPSILON){
				bstrXPath += L"ContractPriceLast=\"" + _bstr_t(pRisks->LastPrice) + "\" ";
			}

			if (pRisks->BidDate > DBL_EPSILON){				
				strDate = COleDateTime(pRisks->BidDate);
				bstrXPath += L"ContractPriceBidDate=\"" + _bstr_t(strDate.Format(dateFormat)) + "\" ";
			}

			if (pRisks->AskDate > DBL_EPSILON){
				strDate = COleDateTime(pRisks->AskDate);
				bstrXPath += L"ContractPriceAskDate=\"" + _bstr_t(strDate.Format(dateFormat)) + "\" "; 
			}

			if (pRisks->LastDate > DBL_EPSILON){
				strDate = COleDateTime(pRisks->LastDate);
				bstrXPath += L"ContractPriceLastDate=\"" + _bstr_t(strDate.Format(dateFormat)) + "\" ";
			}

			if (pRisks->UpdateDate > DBL_EPSILON){
				strDate = COleDateTime(pRisks->UpdateDate);
				bstrXPath += L"AnalyticsTimestamp=\"" +	_bstr_t(strDate.Format(dateFormat)) + "\" ";
			}

		bstrXPath += L" /></root>";

		return bstrXPath.Detach();
	}
	return L"";
};
//----------------------------------------------------------------------------------//
void CTradesCache::ReprocessTradesForAnalytics()
{
	CAutoLock lock(m_ReprocessTradesForAnalyticsLock);

	TradesColl::iterator it = m_ReprocessTradesForAnalytics.begin();
	TradesColl::iterator itEnd = m_ReprocessTradesForAnalytics.end();

	TradesColl notProcessedTrades;
	for ( ; it != itEnd; it++){
		if ( !ProcessTradeForAnalytics(*it) )
			notProcessedTrades.push_back(*it);
	}

	m_ReprocessTradesForAnalytics.swap(notProcessedTrades);
}
//----------------------------------------------------------------------------------//
bool CTradesCache::ProcessTradeForAnalytics(CTradePtr spTrade){
	if (spTrade.get()){

		long	lStep		= 0;
		IRisks	retRisks;

		try{
			retRisks = spTrade->m_spContract->Calculate(NULL, NULL);
		}
		catch (_com_error& err){
			
			std::ostringstream out_stream;
			out_stream << "Analytical data for trade [";
			out_stream << spTrade->m_nTradeID;
			out_stream<< "] has not been ready at this moment. It will be respocessed one more time by queue.";

			TRACE_COM_ERROR(LOG4CPLUS_WARN,
							VS::Log,
							err,
							std::string(out_stream.str()));

			return false;
		}

		try{

			//format xml to pass to DB
			lStep = 1;
			_bstr_t	bstrXML;
			bstrXML.Attach(GetXMLString(&retRisks, spTrade));

			lStep = 2;
			//save to DB
			InitializeDB();
			CStoredProc<CClientRecordset> rs(GetDBConnection(), L"usp_ContractAnalytics_Save");
			rs << bstrXML;

			lStep = 3;
			rs.Open();
			
			lStep = 4;
			rs.Close();

			return true;
		}
		catch (_com_error& err){
			std::ostringstream out_stream;
			out_stream<<"Exception occured while ProcessTradeForAnalytics().\n\tStep = " << lStep;
			out_stream<<"\n\tTradeID = " << spTrade->m_nTradeID;
			TRACE_COM_ERROR(LOG4CPLUS_ERROR, VS::Log, err, std::string(out_stream.str()));
			return false;
		}
		catch (...){
			std::ostringstream out_stream;
			out_stream<<"Unknown C++ exception occured while ProcessTradeForAnalytics().\n\tStep = " << lStep;
			out_stream<<"\n\tTradeID = " << spTrade->m_nTradeID;
			TRACE_UNKNOWN_ERROR(LOG4CPLUS_ERROR, VS::Log, std::string(out_stream.str()));
			return false;
		}
	}
	return false;
};
//----------------------------------------------------------------------------------//
void CTradesCache::OnTradeUpdate(CTradePtr spTrade)
{
	try{

		if (!m_spSettings->GetProcessTradeForAnalytics()){
			std::ostringstream out_stream;
			out_stream<<"Process trade for analytics is disabled.";
			TRACE_EXECUTION(LOG4CPLUS_INFO, VS::Log, std::string(out_stream.str()))
			return;
		}

		std::ostringstream out_stream;
		out_stream<<"Process trade for analytics. TradeID = " << spTrade->m_nTradeID;
		TRACE_EXECUTION(LOG4CPLUS_INFO, VS::Log, std::string(out_stream.str()))
						
		IContractPtr spContract = GetMain()->GetContract(spTrade->m_nContractID);

		IMmTradeInfoCollPtr spTradesColl = NULL;
		m_spTradeChannel->get_Trd(&spTradesColl);
		if (static_cast<bool>(spTradesColl)){

			IMmTradeInfoAtomPtr spNewTrade = NULL;
			spNewTrade = spTradesColl->Item[spTrade->m_nTradeID];
			if (spNewTrade == NULL)
				spNewTrade = spTradesColl->Add(spTrade->m_nTradeID, spTrade->m_dtTradeDate, NULL);

			spNewTrade->putref_Contract (spContract);
			spTrade->m_spContract = spContract;

			if ( !ProcessTradeForAnalytics(spTrade) ){
				CAutoLock lock(m_ReprocessTradesForAnalyticsLock);
				m_ReprocessTradesForAnalytics.push_back(spTrade);
			}	
		}
	}
	catch (_com_error& err){
		std::ostringstream out_stream;
		out_stream << "Exception occured while processing trade.\t\nTradeID = " << spTrade->m_nTradeID;
		TRACE_COM_ERROR(LOG4CPLUS_ERROR, VS::Log, err, std::string(out_stream.str()));
		ATLASSERT(FALSE);
	}
	catch (...){
		std::ostringstream out_stream;
		out_stream<<"Unknown C++ exception occured OnTradeUpdate().\n\tTradeID = " << spTrade->m_nTradeID;
		TRACE_UNKNOWN_ERROR(LOG4CPLUS_ERROR, VS::Log, std::string(out_stream.str()));
		ATLASSERT(FALSE);
	}
};
//----------------------------------------------------------------------------------//
UINT	CTradesCache::Run()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	try{		
		while(WAIT_TIMEOUT == WaitForSingleObject(GetStopEventHandle(), 5000)){
			ReprocessTradesForAnalytics();
		}
	}
	catch (_com_error& err){
		std::ostringstream out_stream;
		out_stream << "Exception occured in Trade processing thread.";
		TRACE_COM_ERROR(LOG4CPLUS_ERROR, VS::Log, err, std::string(out_stream.str()));
	}
	catch (...){
		std::ostringstream out_stream;
		out_stream<<"Unknown C++ exception occured in Trade processing thread.";
		TRACE_UNKNOWN_ERROR(LOG4CPLUS_ERROR, VS::Log, std::string(out_stream.str()));
	}
	CoUninitialize();
	return 0;
};
//----------------------------------------------------------------------------------//
