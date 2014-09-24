#include "StdAfx.h"
#include "IVDataServiceImpl.h"

//-----------------------------------------------------------------------------//
CIVDataServiceImpl::CIVDataServiceImpl(void)
{
}
//-----------------------------------------------------------------------------//
CIVDataServiceImpl::~CIVDataServiceImpl(void)
{
}
//-----------------------------------------------------------------------------//
CIVDataServiceImpl::CIVDataServiceImpl(PortableServer::POA_ptr	poa, CORBA::ORB_ptr	orb, CSettings::pointer settings):
m_Settings(settings)
{
	m_poa = PortableServer::POA::_duplicate(poa);
	m_orb = CORBA::ORB::_duplicate(orb);
}
//-----------------------------------------------------------------------------//
CIVDataManagerImpl* 
CIVDataServiceImpl::CreateDataManager()
{
	std::string strObjectID = GenerateUniqueObjectID();
	CIVDataManagerImpl* dataManager = new CIVDataManagerImpl(strObjectID, m_Settings);
	return dataManager;
}
//-----------------------------------------------------------------------------//
DDS::IVDataManager_ptr 
CIVDataServiceImpl::ActivateObject(CIVDataManagerImpl* dataManager) const
{
	if (dataManager == NULL){
		DDS::IVDataManager_ptr refDataManager = DDS::IVDataManager::_nil();
		return refDataManager;
	}

	PortableServer::ObjectId_var objectID = PortableServer::string_to_ObjectId(dataManager->m_objectID.c_str());
	m_poa->activate_object_with_id(objectID, dataManager);

	CORBA::Object_var ref = dataManager->_this();
	dataManager->_remove_ref();
	return DDS::IVDataManager::_narrow(ref);
}
//-----------------------------------------------------------------------------//
DDS::IVDataManager_ptr 
CIVDataServiceImpl::GetIVDataManager()
{
	DDS::IVDataManager_ptr refDataManager = DDS::IVDataManager::_nil();
	try
	{
		CIVDataManagerImpl* dataManager = CreateDataManager();
		refDataManager = ActivateObject(dataManager);
	}
	catch(CORBA::TRANSIENT&){
	}
	catch(CORBA::SystemException&) {
	}
	catch(CORBA::Exception&) {
	}
	catch(omniORB::fatalException&) {
	}
	catch(...) {
	}
	return refDataManager;
}
//-----------------------------------------------------------------------------//
DDS::IVSessionManager_ptr 
CIVDataServiceImpl::GetSessionManager()
{
	DDS::IVSessionManager_ptr refSessionManager = DDS::IVSessionManager::_nil();

	return refSessionManager;
};	
//-----------------------------------------------------------------------------//
DDS::IVDataManager_ptr	
CIVDataServiceImpl::ResolveObject(const char* username, const char* password)
{
	DDS::IVDataManager_ptr refDataManager = DDS::IVDataManager::_nil();

	return refDataManager;
}
//-----------------------------------------------------------------------------//
DDS::IVSessionManager_ptr 
CIVDataServiceImpl::ResolveSessionManager(const char* username, const char* password)
{
	DDS::IVSessionManager_ptr refSessionManager = DDS::IVSessionManager::_nil();

	return refSessionManager;
}
//-----------------------------------------------------------------------------//
CORBA::Short	
CIVDataServiceImpl::AddQuoteFilter(DDS::CustomFilter_ptr cmFilter)
{
	return 0;
}
//-----------------------------------------------------------------------------//
CORBA::Short	
CIVDataServiceImpl::RemoveQuoteFilter()
{
	return 0;
}
//-----------------------------------------------------------------------------//
std::string 
CIVDataServiceImpl::GenerateUniqueObjectID()
{

	static CLock lock;	
	CAutoLock csLock(lock);

	GUID guid = GUID_NULL;;
	TCHAR szBuf[256];
	HRESULT result;
	TCHAR* strfmt = "%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X";

	result = CoCreateGuid(&guid);

	_stprintf(szBuf, strfmt,guid.Data1,guid.Data2,guid.Data3,guid.Data4[0],
		guid.Data4[1],guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],
		guid.Data4[6],guid.Data4[7],guid.Data1,guid.Data2,guid.Data3,guid.Data4[0],
		guid.Data4[1],guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],
		guid.Data4[6],guid.Data4[7]);

	return szBuf;	
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//