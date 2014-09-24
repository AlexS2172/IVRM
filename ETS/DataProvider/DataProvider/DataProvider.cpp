// DataProvider.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyCorbaApplication.h"
#include "ActivFeedConnection.h"

#include <log4cplus/fileappender.h>
#include <log4cplus/tstring.h>
#include <log4cplus/layout.h>
#include "Messenger.h"


log4cplus::Logger ProviderLog	= log4cplus::Logger::getInstance("log4cplusDataProvider");
log4cplus::Logger CorbaLog		= log4cplus::Logger::getInstance("log4cplusDataProviderCorba");

void initilize_logger(log4cplus::Logger& my_logger, IniReader::Section& settings);

CDatabaseImpl::pointer CDataBase::m_dbImpl = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		HRESULT hrStatus = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if(FAILED(hrStatus))
			utils::ThrowErrorNoSetErrorInfo(hrStatus, L"Failed to DispEventAdvise.");

		IniReader config_reader(CSettings::GetConfigFileName());
		
		IniReader::Section provider_log_settings("PROVIDER_LOG");
		config_reader.read_section(provider_log_settings);		
		initilize_logger(ProviderLog, provider_log_settings);
			
		IniReader::Section corba_log_settings("CORBA_LOG");
		config_reader.read_section(corba_log_settings);
		initilize_logger(CorbaLog, corba_log_settings);
				
		CSettings::pointer userSettings = CSettings::pointer(new CSettings());
		if (!userSettings->LoadSettings()){
			std::cout<<"Fail to read settings.";
			return 0;
		}

		CProviderSingleton::Initialize(userSettings);

		CMyCorbaApplication	corbaApp(userSettings);	

		CMessengerPtr spMessenger(new CMessenger());

		CDataBase::getInstance()->setConnectionString(userSettings->m_DBConnectionString);
		
		spMessenger->SubscribeTradeUpdate();
		
		corbaApp.Run(userSettings->GetORBSettings(), std::string("IVDataService"));
		
		spMessenger->UnSubscribeTradeUpdate();

		CoUninitialize();
	}
	catch (_com_error& err)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("ComError: Main() ")<<(char*)(err.Description()));
		ATLASSERT(FALSE);
	}
	catch (...)
	{
		LOG4CPLUS_ERROR(ProviderLog, _T("Unknown error occured while Main()"));
		ATLASSERT(FALSE);
	};
	return 0;
}

void 
initilize_logger(log4cplus::Logger& my_logger, IniReader::Section& section) {

	using namespace log4cplus;
	
	my_logger.removeAllAppenders();

	std::string log_pattern;
	section.get_value("LOG_PATTERN", log_pattern, "%D{%c} [%t] [%p]: %m%n");

	std::string log_file_name;
	section.get_value("LOG_FILE_NAME", log_file_name, "provider.log");

	long log_parts;
	section.get_value("LOG_PARTS", log_parts, 3);

	long log_file_size;
	section.get_value("LOG_FILE_SIZE", log_file_size, 100);

	long log_level;
	section.get_value("LOG_LEVEL", log_level, long(log4cplus::FATAL_LOG_LEVEL));

	SharedAppenderPtr appender(new RollingFileAppender(	log_file_name, 
														log_file_size * 1024 * 1024, 
														log_parts));
	appender->setName(_T("log_appender"));
	appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(log_pattern)));
	my_logger.setLogLevel(LogLevel(log_level));
	my_logger.addAppender(appender);

	LOG4CPLUS_INFO(my_logger, _T("logger initialized"));
};