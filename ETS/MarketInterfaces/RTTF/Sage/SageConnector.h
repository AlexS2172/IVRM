#ifndef _SAGE_CONNECTOR_H
#define _SAGE_CONNECTOR_H

#include "stdafx.h"
#include "Trace.h"
#include "Publisher.h"


class CSageConnector : 
	private FIX::Application,
	private FIX::MessageCracker
{
public:

	CPublisher	*m_Publisher;
	DWORD Start();
	DWORD Stop();
	DWORD m_nGroupsCount;
	HRESULT SetGroups(UserGroups& clParams)
	{
		m_nGroupsCount = static_cast<DWORD>(clParams.size());
		if (m_nGroupsCount <= 0) return E_FAIL;
		
		m_Publisher = new CPublisher[m_nGroupsCount];
		if (m_Publisher == NULL) return E_FAIL;

		DWORD dwCounter = 0;
		for(UGIterator it = clParams.begin(); it != clParams.end(); it++)		
		{
			m_Publisher[dwCounter++].SetUserGroup(it->first);
		}
		return S_OK;
	};


	// FIX::Application overloads

	virtual void onCreate(const FIX::SessionID& sessionID)
	{
		CString sSession;
		sSession.Format("FIX Session is created: %s", sessionID.toString().c_str());
		CTracer::Trace(sSession);
	}

	virtual void onLogon(const FIX::SessionID& sessionID)
	{
		CString sSession;
		sSession.Format("OnLogOn: %s", sessionID.toString().c_str());
		CTracer::Trace(sSession);
	}
	virtual void onLogout(const FIX::SessionID& sessionID)
	{
		CString sSession;
		sSession.Format("OnLogOut: %s", sessionID.toString().c_str());

		CTracer::Trace(sSession);
	}

	virtual void toAdmin(FIX::Message& message, const FIX::SessionID& sessionID)
		throw(FIX::DoNotSend&){}

	virtual void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID)
	{
		crack(message, sessionID);
	}
	virtual void fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID)
		throw(FIX::FieldNotFound&, FIX::IncorrectDataFormat&, FIX::IncorrectTagValue&, FIX::RejectLogon&)
	{
	}
	virtual void toApp(FIX::Message& message, const FIX::SessionID& sessionID)
		throw(FIX::DoNotSend&)
	{
	}

	//virtual void onRun();

	// FIX::MessageCracker overloads
	virtual void onMessage(const FIX41::ExecutionReport& message, const FIX::SessionID& sessionID);
	virtual void onMessage(const FIX42::ExecutionReport& message, const FIX::SessionID& sessionID);

private:

	void checkAndCorrectEquityAssetSymbol(std::string &symbol);
	
	boost::shared_ptr<FIX::SocketInitiator>		m_pInitiator;
	boost::shared_ptr<FIX::SocketAcceptor>		m_pAcceptor;
	boost::shared_ptr<FIX::SessionSettings>		m_pSettings;
	boost::shared_ptr<FIX::FileStoreFactory>	m_pStoreFactory;
	boost::shared_ptr<FIX::FileLogFactory>		m_pLogFactory;
	
	bool	m_bFilterOutFutureTrades;
};

#endif //_SAGE_CONNECTOR_H