#pragma once
#include "..\..\..\..\ivservice\ivserver\volatilityserver_3_0_0_8\idl\ivdataservice410.h"
#include "IVDataManagerImpl.h"

class CIVDataServiceImpl :
	public POA_DDS::IVDataService
{
public:
	CIVDataServiceImpl(PortableServer::POA_ptr	poa, CORBA::ORB_ptr	orb, CSettings::pointer	settings);
	virtual ~CIVDataServiceImpl(void);

protected:
	CIVDataServiceImpl(void);
public:
	//IVDataService interface
	DDS::IVDataManager_ptr		GetIVDataManager();
	DDS::IVDataManager_ptr		ResolveObject(const char* username, const char* password);

	CORBA::Short				AddQuoteFilter(DDS::CustomFilter_ptr cmFilter);
	CORBA::Short				RemoveQuoteFilter();

	DDS::IVSessionManager_ptr	ResolveSessionManager(const char* username, const char* password);
	DDS::IVSessionManager_ptr	GetSessionManager();

public:
	std::string					GenerateUniqueObjectID();
	CIVDataManagerImpl*			CreateDataManager();
	DDS::IVDataManager_ptr		ActivateObject(CIVDataManagerImpl* dataManager) const;

	PortableServer::POA_var		m_poa;
	CORBA::ORB_var				m_orb;
	CSettings::pointer			m_Settings;
};
