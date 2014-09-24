#ifndef _SAGE_CONNECTOR_H
#define _SAGE_CONNECTOR_H

#include "FixConnector.h"
#include "Publisher.h"

class CSageConnector : public CFixConnector
{
public:

	DWORD __stdcall Start(void* pParam);

	DWORD __stdcall Stop();

	CPublisher	m_Publisher;

private:

	virtual bool OnApplicationMessage(CFixMessage& Msg);

};

#endif //_SAGE_CONNECTOR_H