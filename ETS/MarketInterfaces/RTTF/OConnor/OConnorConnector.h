#ifndef __OCONNOR_CONNECTOR_H__
#define __OCONNOR_CONNECTOR_H__

#include "FileConnector.h"
#include "Publisher.h"
#include <string>

class COConnorConnector : public CFileConnector
{
public:
	COConnorConnector(string strSession)
		: CFileConnector(strSession){};

    typedef set<string>    TTrades;

	DWORD __stdcall Start(void* pParam);
	DWORD __stdcall Stop();

	CPublisher	m_Publisher;

protected:
    virtual const CPublisher::TAccounts& GetAccounts();
    virtual bool OnTrade(CTradePtr& trade);

private:
    ///REMOVED/// TTrades     m_Trades;
};

inline const CPublisher::TAccounts& COConnorConnector::GetAccounts()
{ return m_Publisher.GetAccounts(); }

inline bool COConnorConnector::OnTrade(CTradePtr& pTrade)
{
    m_Publisher.OnData(pTrade);
    return true; 
}

#endif //__OCONNOR_CONNECTOR_H__