#pragma once
#include "SettingsManager.h"
#include "EgTracingClass.h"

namespace ActivFeedSettings
{
	
	class CCustomerInfo
	{
	public:							
		static void Initialize();
	private:
		static CLock m_csInitializeLock;

	public:							

		CCustomerInfo()
			:m_spSettingsManager(new CSettingsManager)
		{
			TCHAR szBufferName[MAX_PATH];
			DWORD dwSize = MAX_PATH;
			::GetUserName(szBufferName, &dwSize);

			m_strUserName = szBufferName;

			m_strSessionId = m_strUserName;
			m_spSettingsManager->Initialize(m_strSessionId);
			
			if(!EgLib::CEgLibTraceManager::g_spTrace)
				EgLib::CEgLibTraceManager::g_spTrace = EgLib::CEgLibTraceManagerPtr(static_cast<EgLib::CEgLibTraceManager*>(new Trace::CEgHyprFeedTrace));
						
			CDictionaryValue val_log_directory = m_spSettingsManager->GetSettings(_T("LogDirectory"));
			if (val_log_directory.length()) {
				if (EgLib::CEgLibTraceManager::g_spTrace != NULL) {

					std::string log_directory = val_log_directory + "\\" + m_strUserName;

					EgLib::CEgLibTraceManager::g_spTrace->SetPathType(EgLib::CEgLibTraceManager::enAbsolute);
					EgLib::CEgLibTraceManager::g_spTrace->SetFilePath(_bstr_t(log_directory.c_str()));						
				};
			};
			
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ ,_T("Using session ID: %s"), m_strSessionId.c_str() );
		}

		virtual ~CCustomerInfo(void)
		{

		}
	public:
		__declspec (property(get = GetSessionId)) const std::string&        SessionId;
		__declspec (property(get = GetUserName)) const std::string&			UserName;
		__declspec (property(get = GetSettingsManager))CSettingsManagerPtr  Settings;

		const std::string& GetSessionId() const {return m_strSessionId;}
		const std::string& GetUserName() const {return m_strUserName;}

		CSettingsManagerPtr GetSettingsManager(){ return m_spSettingsManager; }
	private:


		CSettingsManagerPtr	m_spSettingsManager;
		std::string			m_strSessionId;
		std::string			m_strUserName;

	};

	typedef boost::shared_ptr<CCustomerInfo> UserSettingsPtr;
	__declspec(selectany)
		UserSettingsPtr g_spUserSettings;

	__declspec(selectany)
	CLock CCustomerInfo::m_csInitializeLock;

	inline
	void CCustomerInfo::Initialize()
	{
		CAutoLock lock(m_csInitializeLock);

		if(!ActivFeedSettings::g_spUserSettings)
		{
			ActivFeedSettings::g_spUserSettings = ActivFeedSettings::UserSettingsPtr (new ActivFeedSettings::CCustomerInfo);
			if(ActivFeedSettings::g_spUserSettings != NULL)
			{					 
				CDictionaryValue valLogLevel = g_spUserSettings->Settings->GetSettings(_T("LogLevel"));
				if(valLogLevel.length()) {
					long lLogLevel = atol(valLogLevel.c_str());
					if(lLogLevel && EgLib::CEgLibTraceManager::g_spTrace!=NULL) {
						EgLib::CEgLibTraceManager::g_spTrace->MinLogLevel = lLogLevel;
					}
				};
			}
		}
	}

};
