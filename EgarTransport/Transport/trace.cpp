#include "stdafx.h"
#include "trace.h"
#include "Router.h"

DWORD WINAPI CEgTpTrace::ReadTraceSettings()
{
	bool IsDir =false, IsFile = false;

	if(!m_strLogDirectory.empty())
		IsDir = true;
	
//	DWORD ret = CEgTraceManager::ReadTraceSettings();

	CAutoLock	Lock(m_LockMembers);

	if(!IsDir)
	{
		CRouter::GetTraceSettingsFromReg(SETTINGS_LOGS_DIRECTORY, m_strLogDirectory.c_str(), m_strLogDirectory);
		CEgLibTraceManager::SetPathType(CEgLibTraceManager::enAbsolute);
		CEgLibTraceManager::SetFilePath(m_strLogDirectory.c_str());
	}
		
	m_ulDaysInHistory = CRouter::GetTraceSettingsFromReg(SETTINGS_LOGS_HISTORY,  m_ulDaysInHistory);
	m_ulMinLogLevel   = CRouter::GetTraceSettingsFromReg(SETTINGS_LOGS_MIN_LEVEL,m_ulMinLogLevel);

	CEgLibTraceManager::SetLogLifeTime(m_ulDaysInHistory);
	CEgLibTraceManager::SetMinLogLevel(m_ulMinLogLevel);

	return S_OK;
}

