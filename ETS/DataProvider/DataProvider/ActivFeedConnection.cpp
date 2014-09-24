#include "StdAfx.h"
#include "ActivFeedConnection.h"

//----------------------------------------------------------------------//
CActivFeedConnection::pointer CProviderSingleton::m_providerPtr = NULL;
CSettings::pointer CProviderSingleton::m_settingsPtr = NULL;
//----------------------------------------------------------------------//

bool CActivFeedConnection::Connect()
{
	if (m_Gateway.IsConnected()) 
		return true;

	GetProcess()->SetState( IProcess::STATE_RUNNING );
	
	StartThread();
	
	return m_Gateway.Activate();
};
//----------------------------------------------------------------------//
bool CActivFeedConnection::Disconnect()
{
	return m_Gateway.Deactivate();
}
//----------------------------------------------------------------------//
bool CActivFeedConnection::Request(QuoteRequestPtr spRequest, QuoteAtomPtr& spQuote)
{
	clock_t start = clock();	
	StatusCode statusCode = m_Gateway.Request(spRequest, spQuote);
	clock_t finish = clock();

	bool result = (statusCode == STATUS_CODE_SUCCESS);

	if (result)
	{
		LOG4CPLUS_INFO(ProviderLog, "Subscribe (single) done (" << finish - start << " ms.)");
	}
	else
	{
		LOG4CPLUS_ERROR(ProviderLog, "Subscribe (single) failed. StatusCode = " << statusCode << " (" << StatusCodeToString(statusCode) << ")");
	}

	return result;
}
//----------------------------------------------------------------------//
bool CActivFeedConnection::Subscribe(QuoteRequestPtr spRequest)
{
	clock_t start = clock();	
	StatusCode statusCode = m_Gateway.Subscribe(spRequest);
	clock_t finish = clock();

	bool result = (statusCode == STATUS_CODE_SUCCESS);

	if (result)
	{
		LOG4CPLUS_INFO(ProviderLog, "Subscribe (single) done (" << finish - start << " ms.)");
	}
	else
	{
		LOG4CPLUS_ERROR(ProviderLog, "Subscribe (single) failed. StatusCode = " << statusCode << " (" << StatusCodeToString(statusCode) << ")");
	}

	return result;
}
//----------------------------------------------------------------------//
bool CActivFeedConnection::RequestMultiple(CRequestVector& request, CQuoteVector& response)
{
	clock_t start = clock();	
	StatusCode statusCode =	m_Gateway.RequestMultiple(request, response);
	clock_t finish = clock();

	bool result = (statusCode == STATUS_CODE_SUCCESS);
	
	if (result)
	{
		LOG4CPLUS_INFO(ProviderLog, "Get multiple done (" << finish - start << " ms.)");
	}
	else
	{
		LOG4CPLUS_ERROR(ProviderLog, "Get multiple failed. StatusCode = " << statusCode << " (" << StatusCodeToString(statusCode) << ")");
	}
	
	return result;	
}
//----------------------------------------------------------------------//
bool CActivFeedConnection::SubscribeMultiple(CRequestVector& request)
{
	clock_t start = clock();	
	StatusCode statusCode =	m_Gateway.SubscribeMultiple(request);
	clock_t finish = clock();
	
	bool result = (statusCode == STATUS_CODE_SUCCESS);

	if (result)
	{
		LOG4CPLUS_INFO(ProviderLog, "Subscribe multiple done (" << finish - start << " ms.)");
	}
	else
	{
		LOG4CPLUS_ERROR(ProviderLog, "Subscribe multiple failed. StatusCode = " << statusCode << " (" << StatusCodeToString(statusCode) << ")");
	}

	return result;	
}
//----------------------------------------------------------------------//
bool CActivFeedConnection::GetUpdates(CQuoteVector &quotes)
{	
	clock_t start = clock();	
	bool result = m_Gateway.GetUpdates(quotes);
	clock_t finish = clock();

	LOG4CPLUS_INFO(ProviderLog, "Get updates on subscribed records (" << finish - start << " ms.)");

	return result;
}
