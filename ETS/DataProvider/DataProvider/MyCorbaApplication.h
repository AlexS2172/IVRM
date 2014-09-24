#pragma once
#include "CorbaApplication.h"
#include "IVDataServiceImpl.h"

class CMyCorbaApplication: public CCorbaApplication
{
public:
	CMyCorbaApplication(CSettings::pointer settings);
	virtual ~CMyCorbaApplication(void);

public:
	virtual void PreORBInit(const std::string& objectName);
	virtual void PostORBInit(const std::string& objectName);
	virtual CIVDataServiceImpl*	CreateDataServiceObject(PortableServer::POA_ptr poa, CORBA::ORB_ptr	orb);

	PortableServer::POA_var		m_thePoa;
	PortableServer::POA_var		m_theInsPoa;
};
