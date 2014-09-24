#ifndef TRACE_H
#define TRACE_H

class  CEgTpTrace : public EgLib::CEgLibTraceManager
{
public:
	CEgTpTrace(void)
		:EgLib::CEgLibTraceManager(EgLib::CEgLibTraceManager::enRelativeCurrent, _T("Logs"))
		,m_ulDaysInHistory(0L)
		,m_ulMinLogLevel(0L)
	{
		EgLib::CEgLibTraceManager::FileName = L"EgTransport";
		ReadTraceSettings();
	}

	virtual ~CEgTpTrace(void)
	{
	}
public:
	DWORD WINAPI CEgTpTrace::ReadTraceSettings();

private:
	unsigned long    m_ulDaysInHistory;
	unsigned long    m_ulMinLogLevel;
	tstring m_strLogDirectory;
	CLock   m_LockMembers;

};

typedef boost::shared_ptr<CEgTpTrace> CEgTpTracePtr;
#endif // TRACE_H