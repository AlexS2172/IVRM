#ifndef __Pequote_CONNECTOR_H__
#define __Pequote_CONNECTOR_H__

#include "FileConnector.h"
#include "Publisher.h"

class CPequoteConnector : public CFileConnector
{
public:
    typedef set<string>    TTrades;

	CPequoteConnector(std::string strSessionID):CFileConnector(strSessionID){};

	DWORD __stdcall Start(void* pParam);
	DWORD __stdcall Stop();

	CPublisher	m_Publisher;

protected:
    virtual const CPublisher::TAccounts& GetAccounts();
    virtual bool OnTrade(CTradePtr& trade);

	virtual bool OnSnapShotBegin();
	virtual bool OnSnapShotEnd();

private:
    ///REMOVED/// TTrades     m_Trades;
};

inline const CPublisher::TAccounts& CPequoteConnector::GetAccounts()
{ return m_Publisher.GetAccounts(); }

inline bool CPequoteConnector::OnTrade(CTradePtr& pTrade)
{
    m_Publisher.OnData(pTrade);
    return true; 
}

inline bool CPequoteConnector::OnSnapShotBegin()
{
	return (m_Publisher.ClearOldSnapShot() == ERROR_SUCCESS);
}

inline bool CPequoteConnector::OnSnapShotEnd()
{
	return (m_Publisher.UpdateETS() == ERROR_SUCCESS);
}

#endif //__Pequote_CONNECTOR_H__
