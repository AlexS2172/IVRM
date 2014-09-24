#include "StdAfx.h"
#include "StopWatch.h"
#include "Corba.h"
#include "Server.h"

//---------------------------------------------------------//
CServer::CServer() 
{
	m_spMain = NULL;
	m_spVolatilitySource = NULL;
	m_spTradesCache = CTradesCachePtr(new CTradesCache);
	m_spSettings = CSettingsPtr(new CXmlSettings);
	m_spController = CControllerPtr(new CController);
}
//---------------------------------------------------------//
CServer::~CServer(void) 
{
	destroy_corba();
	
	delete dataBrokerReceiver;
	delete dataBrokerTransmitter;
	
	m_spMain = NULL;
	m_spVolatilitySource = NULL;
}
//---------------------------------------------------------//
IVolatilitySourcePtr CServer::GetVolatilitySource() 
{
	try	
	{
		if (m_spVolatilitySource == NULL) 
		{
			BSTR bsTitle, bsFileName, bsVersion, bsProgID;
			BSTR	bsDBConnectionString;
			m_spSettings->GetDBConnection(&bsDBConnectionString);

			IVADBLayoutPtr spDBLayout = NULL;
			HRESULT hr = spDBLayout.CreateInstance(__uuidof(DBLayout));
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create DBLayout object.");
			spDBLayout->put_ConnectionString(bsDBConnectionString);

			IVolatilitySourcesPtr spVolaSources = NULL;
			hr = spVolaSources.CreateInstance(__uuidof(VolatilitySources));
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create VolatilitySources object.");

			hr = spVolaSources->GetVolatilitySourceInfoByID(0, &bsTitle, &bsFileName, &bsVersion, &bsProgID);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to GetVolatilitySourceInfoByID.");

			hr = spVolaSources->get_VolatilitySource(0, &m_spVolatilitySource);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to get_VolatilitySource.");

			IVAManagementPtr spManagement = spDBLayout;
			hr = m_spVolatilitySource->put_DataSource(spManagement);
			if(FAILED(hr))
				utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to put_DataSource.");

			m_spVolatilitySource->put_EnableEditing(VARIANT_TRUE);
			m_spVolatilitySource->put_EnableEvents(VARIANT_TRUE);
		}
	}
	catch (_com_error err) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(),
		"com_error occured while get_volatility_source. " <<
		err.Description());
	}
	catch (...) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(),
		"Unknown error occured while initializing server.");
	};
	return m_spVolatilitySource;
};
//---------------------------------------------------------//
void  CServer::initialize_logger(log4cplus::Logger& my_logger, IniReader::Section& section)
{
	using namespace log4cplus;
	
	my_logger.removeAllAppenders();

	std::string log_pattern;
	section.get_value("LOG_PATTERN", log_pattern, "%D{%c} [%t] [%p]: %m%n");

	std::string log_file_name;
	section.get_value("LOG_FILE_NAME", log_file_name, "Server.log");

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
};
//---------------------------------------------------------//
log4cplus::Logger& CServer::corba_logger() 
{
	static log4cplus::Logger logger_(log4cplus::Logger::getInstance("corba_logger"));
	return logger_;
}
//---------------------------------------------------------//
/*static*/
log4cplus::Logger& CServer::server_logger() 
{
	static log4cplus::Logger logger_(log4cplus::Logger::getInstance("server_logger"));
	return logger_;
}
//---------------------------------------------------------//
/*static*/
void CServer::corba_log_function(const char *msg) 
{
	LOG4CPLUS_TRACE(CServer::corba_logger(), msg);
}
//---------------------------------------------------------//
void CServer::Initialiaze() 
{
	std::cout << "Initializing application ...\n";

	std::string file_name;
	get_configuration_file_name(file_name);	
	std::cout << "\tReading configuration file ... " << file_name.c_str();
	IniReader reader(file_name);
	std::cout << "\t[ OK ]\n";

	std::cout << "\tInitializing log ... ";
	IniReader::Section logger_settings("SERVER_LOG");
	reader.read_section(logger_settings);
	initialize_logger(CServer::server_logger(), logger_settings);
	std::cout << server_logger().getName() << "\t[ OK ]\n";
	
	std::cout << "Application output is in the log file now.\n";
	
	try
	{
		LOG4CPLUS_INFO(server_logger(), "Initializing CORBA");
		
		CorbaSettings corbaSettings("ORB");
		reader.read_section(corbaSettings);

		Corba::initialize(this, corbaSettings);
	}
	catch (std::exception& e)
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), "Application failed initializing CORBA. " << e.what());			
	}
	
	try	
	{

		LOG4CPLUS_INFO(server_logger(), "Initializing financial objects library.");
		
		if (m_spMain != NULL) 
		{
			return;
		}	

		HRESULT hr = m_spMain.CreateInstance(__uuidof(EtsMain));
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to create EtsMain object.");

		// Initializing financial object library (log file)
		LOG4CPLUS_INFO(server_logger(), "\tInitialize log file.");
		LOG4CPLUS_INFO(server_logger(), "\t\t Directory - " << m_spSettings->GetLogDirectory().c_str());

		hr = m_spMain->SetLogDirectory(_bstr_t(m_spSettings->GetLogDirectory().c_str()));
		if (FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to set log directory for core.");	

		LOG4CPLUS_INFO(server_logger(), "\t\t Level - " << m_spSettings->GetLogLevel());
		
		hr = m_spMain->SetLogLevel(m_spSettings->GetLogLevel());
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to read log level from configuration file.");	

		LOG4CPLUS_INFO(server_logger(), "\t\t Life time - " << m_spSettings->GetLogLifetime());
		hr = m_spMain->SetLogLifetime(m_spSettings->GetLogLifetime());
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to read log lifetime from configuration file.");	

		hr = m_spMain->SetWorkingMode(enWmServer);
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to SetWorkingMode.");
			
		// Initializing financial object library (database) 	
		LOG4CPLUS_INFO(server_logger(), "\tInitialize database connection." );
		BSTR bsDBConnectionString;
		m_spSettings->GetDBConnection(&bsDBConnectionString);
		LOG4CPLUS_INFO(server_logger(), "\t\t" << static_cast<char*>(_bstr_t(bsDBConnectionString)));
				
		hr = m_spMain->put_DatabaseString(bsDBConnectionString);
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to put_DatabaseString.");	

		LOG4CPLUS_INFO(server_logger(), "\tInitialize volatility source." );
		hr = m_spMain->putref_VolatilitySource(GetVolatilitySource());
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to putref_VolatilitySource.");	

		LOG4CPLUS_INFO(server_logger(), "\tLoading financial objects." );

		LOG4CPLUS_INFO(server_logger(), "\t\tTrader groups objects..." );
		hr = m_spMain->LoadTraderGroup();
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to LoadTraderGroup.");

		LOG4CPLUS_INFO(server_logger(), "\t\tTrader objects..." );
		hr = m_spMain->LoadTrader(0);
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to LoadTrader.");

		LOG4CPLUS_INFO(server_logger(), "\t\tBroker objects..." );
		hr = m_spMain->LoadBroker();
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to LoadBrokers.");

		LOG4CPLUS_INFO(server_logger(), "\t\tTrade strategy objects..." );
		hr = m_spMain->LoadStrategy();
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to LoadStrategy.");	


		LOG4CPLUS_INFO(server_logger(), "\t\tContract market price adjustment profiles objects..." );
		hr = m_spMain->LoadPriceProfile();
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to LoadPriceProfile.");

		LOG4CPLUS_INFO(server_logger(), "\t\tAssets objects..." );
		hr = m_spMain->LoadUnderlying(0);
		if(FAILED(hr))
			utils::ThrowErrorNoSetErrorInfo(hr, L"Failed to LoadUnderlying.");

		//---------------------//
		//trade channel routine//
		//---------------------//
		
		m_spTradesCache->SetSettingsImpl(m_spSettings);
		m_spTradesCache->SetMain(m_spMain);
		m_spTradesCache->LoadTrades();
		m_spTradesCache->SubscribeTradeUpdate();

		m_spController->SetSettingsImpl(m_spSettings);
		m_spController->SetMain(m_spMain);
		m_spController->LoadSettings();
		m_spController->LoadAssetList();
		m_spController->SubscribeSettingsUpdate();
		m_spController->SubscribeUnderlyingUpdate();


		//-------------------------//
		IniReader::Section main_settings("MAIN");
		reader.read_section(main_settings);
		
		main_settings.get_value("RECALCULATION_PERIOD", calculationCyclePeriod, 5000);

		std::string filter_object_name;
		main_settings.get_value("FILTER_NAME", filter_object_name, "IvRmServer");
		

		CorbaSettings orb_settings("ORB");
		reader.read_section(orb_settings);

		IniReader::Section corba_logger_settings("CORBA_LOG");
		reader.read_section(corba_logger_settings);

		initialize_logger(CServer::corba_logger(), corba_logger_settings);

		//if (initialize_corba(orb_settings))
		//{
			dataBrokerReceiver = new DataBrokerFilter(m_spMain, filter_object_name);
			dataBrokerReceiver->initialize();
			
			IniReader::Section dataBrokerTransmitterSettings("TRANSMITTER");
			reader.read_section(dataBrokerTransmitterSettings);
			dataBrokerTransmitter = new DataBrokerTransmitter(new IvDataPacker);
			dataBrokerTransmitter->initialize(dataBrokerTransmitterSettings);

			//activate_transmiter(dataBrokerTransmitter);
		//}
	}
	catch (_com_error e) 
	{
	
		std::cout<<"ComError: " << e.Description();
	
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"_com_error occured while initialize server: " 
		<< e.Description());
		throw;
	}
	catch (...) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"Unknown error occured while initialize server.");
		throw;
	};
};
//---------------------------------------------------------//
UINT CServer::Run() 
{
	try 
	{	
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		Initialiaze();
	}
	catch(const _com_error& e)
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), "Com error occured initializing application." << e.Description());			
		return -1;
	}
	catch (...) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), "Unknown exception occured while calculation cycle.");
		return -1;					
	}
		
	if (!m_spMain) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), "Core has not been not initialized.");
		return -1;
	}

	static long emptyCycleCounter = 0;
	LPSAFEARRAY contractAnalyticalDataBuffer = 0;
	CORBA::Any* result = new CORBA::Any();
	
	while(WAIT_TIMEOUT == ::WaitForSingleObject(GetStopEventHandle(), calculationCyclePeriod)) 
	{								
		try 
		{
			StopWatch stopWatch;

			_CHK(m_spMain->raw_CalculateUpdatedContracts(&contractAnalyticalDataBuffer), _T("Fail to calculate updated contract."));

			long contractAnalyticalDataBufferSize = contractAnalyticalDataBuffer->rgsabound[0].cElements;

			if (contractAnalyticalDataBufferSize > 0)
			{	
				LOG4CPLUS_INFO(CServer::server_logger(), "Calculated [" << contractAnalyticalDataBufferSize << "] contracts in " << stopWatch.toString());		
				emptyCycleCounter = 0;
			}
			else
			{
				if (++emptyCycleCounter == 10)
				{
					// application will inform that it is alive if no calculations done more than 30 cycle
					LOG4CPLUS_INFO(CServer::server_logger(), "Calculation thread is alive. No output because empty calculation cycles.");											
					emptyCycleCounter = 0;						  
				}

				continue; // skip sending data to DataBroker
			}
			
			dataBrokerTransmitter->transmit(contractAnalyticalDataBuffer);
			contractAnalyticalDataBuffer = 0;
			 
		}
		catch(const _com_error& e)
		{
			LOG4CPLUS_ERROR(CServer::server_logger(), "Com error occured in calculation thread." << e.Description());			
		}
		catch (...) 
		{
			LOG4CPLUS_ERROR(CServer::server_logger(), "Unknown exception occured while calculation cycle");
			continue;					
		}
	}

	LOG4CPLUS_INFO(CServer::server_logger(), "Calculation thread complete.");
	::CoUninitialize();

	return 0;
};
//---------------------------------------------------------//
bool CServer::initialize_corba(CorbaSettings& settings)
{
	try 
	{
		char**	args = 0;
		int		args_count = 0;

		omniORB::setLogFunction(CServer::corba_log_function);

		settings.get_orb_args(args, args_count);
		CORBA::ORB_ptr orb = CORBA::ORB_init(args_count, args);
		
		CORBA::release(orb);
		return (orb != 0);
	}
	catch(CORBA::SystemException& err) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"corba::SystemException: " << ToString(err));
	}
	catch(CORBA::Exception& err) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"corba::Exception: " << ToString(err));
	}
	catch(omniORB::fatalException& err) 
	{		
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"omniORB::FatalException: " << ToString(err));
	}
	catch (std::exception err) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"std::exception occured while initialize_corba() " <<
		err.what());
	}
	catch (...) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(),
		"Unknown exception occured while initializing CORBA.");
		throw;
	}
	
	return false;
};
//---------------------------------------------------------//
bool CServer::destroy_corba()
{
	try 
	{
		int count = 0;
		char** args = 0;
	
		CORBA::ORB_ptr orb = CORBA::ORB_init(count, args);
		CORBA::release(orb);

		orb->shutdown(true);
		orb->destroy();

		omniORB::setLogFunction(0);
	}
	catch(CORBA::SystemException& err) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"corba::SystemException: " << ToString(err));
	}
	catch(CORBA::Exception& err) 
	{
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"corba::Exception: " << ToString(err));
	}
	catch(omniORB::fatalException& err) 
	{		
		LOG4CPLUS_ERROR(CServer::server_logger(), 
		"omniORB::FatalException: " << ToString(err));
	}
	catch (std::exception err) {
		LOG4CPLUS_ERROR(CServer::server_logger(),
		"std::exception occured while destroy_corba()" <<
		err.what());	
	}
	catch (...) {
		LOG4CPLUS_ERROR(CServer::server_logger(),
		"Unknown exception occured while destroying corba.");
	};
	return true;
}
//---------------------------------------------------------//
CorbaSettings::CorbaSettings(const std::string &id) : IniReader::Section(id)
{
}
//---------------------------------------------------------//
void
CorbaSettings::add(const std::string& key, const std::string& value)
{
	parameters_.push_back(key_value("-" + key, value));
}
//---------------------------------------------------------//
void CorbaSettings::get_orb_args(char**& args, int& count){

	int		argc		= __argc;
	TCHAR**	argv		= __targv;

	count = argc + static_cast<int>(2 * parameters_.size());
	args = new char*[count];
	memcpy(args, argv, sizeof(char*)*argc);

	int paramIndex = argc;

	IniReader::Section::key_container::iterator it = parameters_.begin();
	IniReader::Section::key_container::iterator it_end = parameters_.end();

	for( ;it != it_end; it++)
	{
		IniReader::Section::key_value& pmtr = *it;
		args[paramIndex++] = const_cast<char*>(pmtr.first.c_str());
		args[paramIndex++] = const_cast<char*>(pmtr.second.c_str());
	}
}

