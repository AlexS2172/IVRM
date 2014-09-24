#pragma once

#include "ApplicationConfiguration.h"
#include "TradesCache.h"
#include "Controller.h"
#include "DataBrokerFilter.h"
#include "DataBrokerTransmitter.h"

class CorbaSettings: public IniReader::Section
{
public:

	CorbaSettings(const std::string& id);
	
	/*virtual*/
	void add(const std::string& key, const std::string& value);

	void get_orb_args(char**& args, int& count);
};

class CServer:	public EgLib::CThread
{
public:
	CServer(void);
	~CServer(void);

public:
	UINT					Run();
	void					Initialiaze();

	static	log4cplus::Logger& server_logger();
	
private:
	long					calculationCyclePeriod;


	IEtsMainPtr				m_spMain;
	IVolatilitySourcePtr	m_spVolatilitySource;

	CTradesCachePtr			m_spTradesCache;
	CSettingsPtr			m_spSettings;	
	CControllerPtr			m_spController;
										  
	DataBrokerFilter*		dataBrokerReceiver;
	DataBrokerTransmitter*	dataBrokerTransmitter;

	bool initialize_corba(CorbaSettings& settings);
	bool destroy_corba();
	
	static	log4cplus::Logger& corba_logger();

	static
	void corba_log_function(const char* msg);

	void initialize_logger(log4cplus::Logger& my_logger, IniReader::Section& section);

	IVolatilitySourcePtr GetVolatilitySource();
};