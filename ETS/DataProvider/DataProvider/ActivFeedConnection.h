#pragma once

#include "ActivFeedContentGateway.h"

using namespace Activ;
using namespace std;
//--------------------------------------------------------------------------//
class CActivFeedConnection: public Activ::AgentApplication
{
public:
	CActivFeedConnection(	AgentApplication::Settings& settings, CSettings::pointer	userSettings):
							AgentApplication(settings),
							m_Gateway(*this, this, userSettings)
	{
	};

	virtual ~CActivFeedConnection(void)
	{
		PostDiesToThreads();
		Shutdown();
	};
	
public:
	typedef	CActivFeedConnection*	pointer;
public:
	CActivFeedContentGateway	m_Gateway;
	bool	Connect();
	bool	Disconnect();
	bool	Request(QuoteRequestPtr	spRequest, QuoteAtomPtr& spQuote);
	bool	Subscribe(QuoteRequestPtr spRequest);
	bool	RequestMultiple(CRequestVector& request, CQuoteVector& response);
	bool	SubscribeMultiple(CRequestVector& request);
	bool	GetUpdates(CQuoteVector& quotes);
};
//--------------------------------------------------------------------------//
class CProviderSingleton
{
private:
	static	CActivFeedConnection::pointer m_providerPtr;
	static	CSettings::pointer m_settingsPtr;
public:
	static	void Initialize(CSettings::pointer settings)
	{
		m_settingsPtr = settings;
	};

	static CActivFeedConnection::pointer GetProvider()
	{
		if (m_providerPtr == NULL && m_settingsPtr != NULL){
			m_providerPtr = new CActivFeedConnection(m_settingsPtr->GetACTIVSettings(), m_settingsPtr);
			LOG4CPLUS_INFO(ProviderLog, _T("Initialize ActivFeedConnection...."))
		}
		
		return	m_providerPtr;
	};
};
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
