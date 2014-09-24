#include "StdAfx.h"
#include "MyCorbaApplication.h"
//----------------------------------------------------------------------------------//
CMyCorbaApplication::CMyCorbaApplication(CSettings::pointer settings):
										CCorbaApplication(settings)
{

}
//----------------------------------------------------------------------------------//
CMyCorbaApplication::~CMyCorbaApplication(void)
{

}
//----------------------------------------------------------------------------------//
void CMyCorbaApplication::PreORBInit(const std::string& objectName)
{

}
//----------------------------------------------------------------------------------//
void CMyCorbaApplication::PostORBInit(const std::string& objectName)
{
	CORBA::ORB_var orb = CORBA::ORB::_duplicate(GetORB());

	CORBA::Object_var poaRef				= orb->resolve_initial_references("RootPOA");
	PortableServer::POA_var			poa		= PortableServer::POA::_narrow(poaRef);
	PortableServer::POAManager_var	pman	= poa->the_POAManager();

	CORBA::PolicyList pl;
	pl.length(3);
	pl[0] = poa->create_lifespan_policy(PortableServer::PERSISTENT);
	pl[1] = poa->create_id_assignment_policy(PortableServer::USER_ID);
	pl[2] = poa->create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
	
	m_thePoa = poa->create_POA("lifespan poa", pman, pl);
	pman->activate();

	// Get the "magic" interoperable naming service POA
	poaRef				= orb->resolve_initial_references("omniINSPOA");
	m_theInsPoa			= PortableServer::POA::_narrow(poaRef);
	pman				= m_theInsPoa->the_POAManager();
	pman->activate();

	CIVDataServiceImpl* servant				= CreateDataServiceObject(m_thePoa, orb);
	PortableServer::ObjectId_var serviceIID = PortableServer::string_to_ObjectId(objectName.c_str());
	m_theInsPoa->activate_object_with_id(serviceIID, servant);
};
//----------------------------------------------------------------------------------//
CIVDataServiceImpl*	
CMyCorbaApplication::CreateDataServiceObject(PortableServer::POA_ptr poa, CORBA::ORB_ptr orb)
{
	return new CIVDataServiceImpl(poa, orb, m_Settings);
}
//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------//
