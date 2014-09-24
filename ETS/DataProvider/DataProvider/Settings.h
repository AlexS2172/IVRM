#pragma once

#include "ActivMiddleware/ActivAgentApplication/AgentApplication.h"

typedef std::pair<std::string, std::string> ORBParameter;
typedef std::vector<ORBParameter>			ORBParametersList;

using namespace Activ;

class CSettings
{
public:
	CSettings(void);
	~CSettings(void);

public:
	std::string		m_UserID;
	std::string		m_Password;
	std::string		m_ServiceLocationINI;
	std::string		m_DBConnectionString;
		
public:
	typedef	CSettings*			pointer;

	bool						LoadSettings();
	ORBParametersList&			GetORBSettings();
	AgentApplication::Settings&	GetACTIVSettings();
	
	static 
	std::string	GetConfigFileName();
	
private:
	ORBParametersList			m_ORBSettings;
	AgentApplication::Settings	m_ACTIVSettings;
	std::string GetParameter(std::string file, std::string section, std::string key);
};
