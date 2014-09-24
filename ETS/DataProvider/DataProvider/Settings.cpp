#include "StdAfx.h"
#include "Settings.h"
//----------------------------------------------------------------//
CSettings::CSettings(void)
{
}
//----------------------------------------------------------------//
CSettings::~CSettings(void)
{
}
//----------------------------------------------------------------//
/*static*/
std::string 
CSettings::GetConfigFileName() {

	TCHAR szBuf[1024] = {0};
	
	if(GetModuleFileName(NULL, szBuf, sizeof(szBuf))) {
		TCHAR szDrive[8] = {0};
		TCHAR szDir[512] = {0};
		TCHAR szName[128] = {0};
		_tsplitpath( szBuf, szDrive, szDir, szName, NULL);
		std::string strName = szName;
		std::string strDrive = szDrive;
		std::string strDir = szDir;
		return strDrive + strDir + strName + std::string(".ini");
	};
	return std::string("");	
};
//----------------------------------------------------------------//
std::string 
CSettings::GetParameter(std::string file, std::string section, std::string key)
{
	TCHAR strValue[2048] = {0};
	
	GetPrivateProfileString(section.c_str(), 
							key.c_str(),
							_T(""), 
							strValue,
							2048, 
							file.c_str()
							);

	LOG4CPLUS_INFO(ProviderLog, _T("Section=") << section << 
								_T("\tKey=") << key << 
								_T("\tValue=") << strValue);
		
	return strValue;
};
//----------------------------------------------------------------//
bool 
CSettings::LoadSettings() {

	std::string fileName = GetConfigFileName();
	
	LOG4CPLUS_INFO(ProviderLog, _T("ConfigurationFile: ") << fileName.c_str());

	if (fileName.length() > 0) {
		
		IniReader config_reader(fileName);
		
		IniReader::Section activ_settings("ACTIV");
		config_reader.read_section(activ_settings);
		
		activ_settings.get_value("UserID", m_UserID, "");
		activ_settings.get_value("Password", m_Password, "");
		activ_settings.get_value("ServiceLocationINI", m_ServiceLocationINI, "");
		
		IniReader::Section db_settings("DATABASE");
		config_reader.read_section(db_settings);
		db_settings.get_value("ConnectionString", m_DBConnectionString, "");

		m_ACTIVSettings.m_enableUiService = false;
		m_ACTIVSettings.m_createSystemLog = false;
		m_ACTIVSettings.m_systemLogMode = AgentApplication::Settings::SYSTEM_LOG_MODE_OFF;
		m_ACTIVSettings.m_serviceLocationIniFile = m_ServiceLocationINI;

		return true;
	}
	return false;
};
//----------------------------------------------------------------//
ORBParametersList& CSettings::GetORBSettings()
{
	return m_ORBSettings;
};
//----------------------------------------------------------------//
AgentApplication::Settings& CSettings::GetACTIVSettings()
{
	return m_ACTIVSettings;
}
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
