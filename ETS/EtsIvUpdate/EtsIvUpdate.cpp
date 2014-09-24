// EtsIvUpdate.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "IV_Data_Update_Service.h"

#include <EgDebugUtil/EgDebugUtil.h>
#pragma comment ( lib ,"EgDebugUtil.lib") 

// The module attribute causes WinMain to be automatically implemented for you
[ module(SERVICE, uuid = "{BF161EA5-D640-40DF-B347-22BD5E3CE83A}", 
		 name = "EtsIvUpdate", 
		 helpstring = "EtsIvUpdate 1.0 Type Library", 
		 resource_name="IDS_SERVICENAME") ]
class CEtsIvUpdateModule
{
	CIV_Data_Update_Service m_IVUpdater ;
public:
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
		// your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.

		return S_OK;
	}
	HRESULT PreMessageLoop (int nShowCmd);
	void RunMessageLoop (void) throw();
	void OnContinue (void) throw();
	void OnPause (void) throw();
	void OnShutdown (void) throw();
	void OnStop (void) throw();
	void OnStart () throw() ;

	void RunMainThread ()
	{
		m_IVUpdater.LoadingFunction() ;
	}
};

DWORD __stdcall RunTheMainThread (PVOID vPar)
{
#ifndef _DEBUG
	SetMiniDumpDefaultCrashHandler();
#endif
//	ATLASSERT ( !"void CEtsIvUpdateModule::OnStart () throw()" ) ;
	CEtsIvUpdateModule* pPar = (CEtsIvUpdateModule*)vPar ;
	ATLASSERT ( pPar ) ;
	pPar->RunMainThread () ;
	return 0 ;
}


HRESULT CEtsIvUpdateModule::PreMessageLoop (int nShowCmd) throw()
{
	OnStart() ;
	return CAtlServiceModuleT< CEtsIvUpdateModule, IDS_SERVICENAME >::PreMessageLoop (nShowCmd);
}

void CEtsIvUpdateModule::RunMessageLoop () throw()
{
	CAtlServiceModuleT< CEtsIvUpdateModule, IDS_SERVICENAME >::RunMessageLoop ();
}

void CEtsIvUpdateModule::OnContinue () throw()
{
	CAtlServiceModuleT< CEtsIvUpdateModule, IDS_SERVICENAME >::OnContinue ();
}

void CEtsIvUpdateModule::OnPause () throw()
{
	CAtlServiceModuleT< CEtsIvUpdateModule, IDS_SERVICENAME >::OnPause ();
}

void CEtsIvUpdateModule::OnShutdown () throw()
{
	OnStop ();
}

void CEtsIvUpdateModule::OnStop () throw()
{
	m_IVUpdater.OnStopMainThread() ;
	g_Logs.TraceToFile ( LogInfo , "Shutdown Of ETS IV Update service" ) ;
	CAtlServiceModuleT< CEtsIvUpdateModule, IDS_SERVICENAME >::OnStop ();
}

void CEtsIvUpdateModule::OnStart () throw()
{
	
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	g_Logs.InitializeService() ;
	g_Logs.TraceToFile ( LogInfo , "Start Of ETS IV Update service" ) ;

	DWORD lRet ;
	HANDLE hProc = CreateThread ( NULL , 0 , RunTheMainThread , this , 0, &lRet	) ;
	CloseHandle ( hProc ) ;
}