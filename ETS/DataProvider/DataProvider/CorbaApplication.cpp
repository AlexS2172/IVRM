#include "StdAfx.h"
#include "CorbaApplication.h"
#include "CorbaErrorHelper.h"
#include "TableInitializer.h"
//--------------------------------------------------------------------------------//
CCorbaApplication::CApplicationImpl*	CCorbaApplication::m_pImpl = NULL;
size_t									CCorbaApplication::m_refCount = 0;
//--------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////
//// CCorbaApplication::CApplicationImpl implementation
///////////////////////////////////////////////////////////////
class CCorbaApplication::CApplicationImpl
{
public:
	CApplicationImpl(CSettings::pointer settings);
	~CApplicationImpl();

	void Run();
	bool IsStarted();
	bool IsStopping();
	void Stop();

	CORBA::ORB_var						m_orb;
private:
	void clear();
	CApplicationImpl(const CApplicationImpl& rhs);
	CApplicationImpl& operator = (const CApplicationImpl& rhs);
	
	//private functions
	static void CORBA_LogFunc(const char* msg);
	static UINT WINAPI orb_thread(LPVOID pData);

	UINT orb_thread();

	//private members
	HANDLE						m_stopEvent;	
	HANDLE						m_orbThread;
	CComAutoCriticalSection		m_csObjAccess;

	static const size_t			MAX_ORB_THREAD_WAIT = 10000;

	CSettings::pointer			m_Settings;
};

/////////////////////////////////////////////////////////////////////
//// CCorbaApplication class implementation
/////////////////////////////////////////////////////////////////////
CCorbaApplication::CCorbaApplication(CSettings::pointer settings):
m_Settings(settings)
{

	if (!m_pImpl){
		m_pImpl = new CApplicationImpl(settings);
	}
	m_refCount++;

	CTableInitializer::InitializeWriter();

}
//--------------------------------------------------------------------------------//
CCorbaApplication::~CCorbaApplication()
{

	if(--m_refCount == 0){
		omniORB::setLogFunction(NULL);

		delete m_pImpl;
		m_pImpl = NULL;
	}
}
//--------------------------------------------------------------------------------//
void 
CCorbaApplication::Run(const ORBParametersList& orbParameters, const std::string& objectName) {

	try {
	
		if (IsStarted()) {
			throw std::runtime_error("Server already started");
		};

		PreORBInit(objectName);
		
		int		ORBrefCount = 0;
		char**	args = 0;
		int		args_count = 0;
		
		IniReader config_reader(CSettings::GetConfigFileName());
		
		CorbaSettings orb_settings("ORB");
		config_reader.read_section(orb_settings);
		
		orb_settings.get_orb_args(args, args_count);
		
		m_pImpl->m_orb = CORBA::ORB_init(args_count, args);

		PostORBInit(objectName);

		m_pImpl->Run();

	}
	catch(CORBA::SystemException& e) {
		LOG4CPLUS_ERROR(ProviderLog, CCorbaErrorHelper::ToString(e))
	}
	catch(CORBA::Exception& e) {
		LOG4CPLUS_ERROR(ProviderLog, CCorbaErrorHelper::ToString(e))
	}
	catch(omniORB::fatalException& e) {
		LOG4CPLUS_ERROR(ProviderLog, CCorbaErrorHelper::ToString(e))
	}
	catch(...) {
		LOG4CPLUS_ERROR(ProviderLog, "Unknown exception occured while CorbaApplication::Run()");
	}
};
//--------------------------------------------------------------------------------//
bool CCorbaApplication::IsStopping()
{
	return m_pImpl->IsStopping();	
}
//--------------------------------------------------------------------------------//
bool CCorbaApplication::IsStarted()
{
	return m_pImpl->IsStarted();
}
//--------------------------------------------------------------------------------//
void CCorbaApplication::Stop()
{
	if (m_pImpl){
		delete m_pImpl;		
		m_pImpl = 0;
	}

	omniORB::setLogFunction(NULL);

}
//--------------------------------------------------------------------------------//
void CCorbaApplication::PreORBInit(const std::string& objectName)
{

}
//--------------------------------------------------------------------------------//
void CCorbaApplication::PostORBInit(const std::string& objectName)
{

}
//--------------------------------------------------------------------------------//
void CCorbaApplication::SetLogLevel(const int logLevel)
{
	//Log.setLogLevel(logLevel);
}
//--------------------------------------------------------------------------------//
long CCorbaApplication::GetLogLevel() const
{
	//return Log.getLogLevel();
	return long(15);
}
//--------------------------------------------------------------------------------//
CORBA::ORB_ptr CCorbaApplication::GetORB()
{
	return m_pImpl->m_orb;	
}
//--------------------------------------------------------------------------------//
BOOL CCorbaApplication::ConsoleCtrlHandler(DWORD ctrlType)
{
	if (m_pImpl){
		delete m_pImpl;
		m_pImpl = 0;
	}

	omniORB::setLogFunction(NULL);

	return FALSE;
}

/////////////////////////////////////////////////////////////////////
//// CCORBAServerApplication::CApplicationImpl class implementation
/////////////////////////////////////////////////////////////////////
CCorbaApplication::CApplicationImpl::CApplicationImpl(CSettings::pointer settings):
m_orbThread(NULL),
m_Settings(settings)
{

	m_stopEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CCorbaApplication::ConsoleCtrlHandler, TRUE );

	omniORB::setLogFunction(CApplicationImpl::CORBA_LogFunc);

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if FAILED(hr){
		throw std::runtime_error("Can't initialize COM library");
	}
}
//--------------------------------------------------------------------------------//
CCorbaApplication::CApplicationImpl::~CApplicationImpl()
{
	if (IsStarted()){
		Stop();
	}
}
//--------------------------------------------------------------------------------//
void CCorbaApplication::CApplicationImpl::clear()
{
	try{
		if (IsStarted()){
			Stop();
		}
	}
	catch(...){
		ATLASSERT(FALSE);
	}
}
//--------------------------------------------------------------------------------//
void CCorbaApplication::CApplicationImpl::Run()
{
	if (IsStarted()){
		throw std::runtime_error("Server already started");
	}

	{
		CCriticalSectionWrapper autoLock(m_csObjAccess);
		unsigned ID = 0;
		m_orbThread  = (HANDLE)_beginthreadex(NULL, 0, orb_thread, this, 0, &ID);	
	}
	::WaitForSingleObject(m_stopEvent, INFINITE);

	CCriticalSectionWrapper orbShutDownLock(m_csObjAccess);
}
//--------------------------------------------------------------------------------//
bool CCorbaApplication::CApplicationImpl::IsStarted()
{
	return m_orbThread && ::WaitForSingleObject(m_orbThread, 0) == WAIT_TIMEOUT;
}
//--------------------------------------------------------------------------------//
bool CCorbaApplication::CApplicationImpl::IsStopping()
{
	return WaitForSingleObject(m_stopEvent, 0) == WAIT_OBJECT_0;
}
//--------------------------------------------------------------------------------//
void CCorbaApplication::CApplicationImpl::Stop()
{
	if (!IsStarted()){
		return;
	}

	omniORB::setLogFunction(NULL);

	CCriticalSectionWrapper autoLock(m_csObjAccess);
	SetEvent(m_stopEvent);

	if (::WaitForSingleObject(m_orbThread, MAX_ORB_THREAD_WAIT) == WAIT_TIMEOUT){
		::TerminateThread(m_orbThread, -1);
	}
	::CloseHandle(m_orbThread);
	m_orbThread = NULL;
}
//--------------------------------------------------------------------------------//
void CCorbaApplication::CApplicationImpl::CORBA_LogFunc(const char* msg)
{
	LOG4CPLUS_INFO(CorbaLog, msg);
}
//--------------------------------------------------------------------------------//
UINT WINAPI CCorbaApplication::CApplicationImpl::orb_thread(LPVOID pData)
{
	CApplicationImpl* pThis = reinterpret_cast<CApplicationImpl*>(pData);
	if (!pThis){
		ATLASSERT(FALSE && "Can't cast pointer to application object");
		return -1;
	}
	return pThis->orb_thread();

}
//--------------------------------------------------------------------------------//
UINT CCorbaApplication::CApplicationImpl::orb_thread()
{
	try{
		::WaitForSingleObject(m_stopEvent, INFINITE);

		int RefCount = 0;

		ORBInit::ORBShutdown(&RefCount);

	}catch(CORBA::SystemException&) {
		LOG_EXCEPTION;
	}catch(CORBA::Exception&){
		LOG_EXCEPTION;
	}catch(omniORB::fatalException&) {
		LOG_EXCEPTION;
	}catch(...){
		LOG_EXCEPTION;
	}
	return 0;
}
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//