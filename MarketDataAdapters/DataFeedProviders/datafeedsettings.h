#pragma once
#include "stdafx.h"

class CDataFeedSettings
{
public:
	static std::string m_userName;
	static std::string m_password;
	static std::string m_remoteObj;
	static std::string m_provider;
	static std::string m_endpoint;
	static std::string m_ORBTraceLevel;
	static int m_attempts;
	static CORBA::Short m_compression;
	static CORBA::ULong m_serverTimeout;
	static bool UseXML;
	static void Initialize();
	static std::string GetProcessPath();
	static std::string GetProcessFilename();
	static long log_lifetime;
	static long log_level;
	static long pull_timeout;
	static std::string log_directory;
	static std::string GetUserName();
};