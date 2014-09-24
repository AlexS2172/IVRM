// ETSLog.h : Declaration of the CETSLog

#pragma once
#include "resource.h"       // main symbols
#include <shlobj.h>


[
	export,
	uuid("C572C1BA-E445-43f7-BB34-E16D65A91B01")
]
enum eOperType
{
	OPER_OPENLAYOUT          =1,
	OPER_LOADINITDATA        =2,
	OPER_LOADTRADESDATA      =3,
	OPER_OPENWINDOW	         =4,
	OPER_ACTIVATEWINDOW      =5,
	OPER_LOADDATA	         =6,
	OPER_REQUESTQUOTE        =7,
	OPER_SUBSCRIBEQUOTE      =8,
	OPER_CALCULATION         =9,
	OPER_REFRESHSCREEN       =10,
	OPER_CALCREFRESHSCREEN   =11
};

[
	export,
	uuid("191B524D-54D6-4142-8416-400FB485D169")
]
enum  LogLevelEnum {
			enLogNone           = 0,
			enLogFaults         = 1,
			enLogSystem         = 2,
			enLogWarning        = 3,
			enLogUserAction     = 4,
			enLogUserActionExt  = 5,
			enLogInformation    = 6,
			enLogInformationExt = 7,
			enLogDebug			= 8,
			enLogEnhDebug		= 9,
			enLogAll = enLogEnhDebug
};


[
	export,
	uuid(005CBE4E-53C2-4312-9834-B5BFCFD23220),
	helpstring("FilderPathEnum Type")
]

typedef enum eFilderPathEnum
{
	enAbsolute,
	enRelativeCurrent,
	enRelativeMyDocuments

} eFilderPathEnum;

// IETSLog
[
	object,
	uuid("AE4DB7AF-31C7-4060-BDB0-400C2C939278"),
	dual,	helpstring("IETSLog Interface"),
	pointer_default(unique)
]
__interface IETSLog : IDispatch
{
	[propget, id(1), helpstring("property FileName")]    HRESULT FileName([out, retval] BSTR* pVal);
	[propput, id(1), helpstring("property FileName")]    HRESULT FileName([in] BSTR newVal);
	[propget, id(2), helpstring("property FilePath")]    HRESULT FilePath([out, retval] BSTR* pVal);
	[propput, id(2), helpstring("property FilePath")]    HRESULT FilePath([in] BSTR newVal);
	[id(3), helpstring("method BeginLogMmOperation")]    HRESULT BeginLogMmOperation([out, retval] LONG* pVal);
	[id(4),helpstring("method ContinueLogMmOperation")] HRESULT  ContinueLogMmOperation( [in] LONG nOperation, 
																						 [in] LogLevelEnum enLogLevel,
																						 [in] eOperType enOperation, 
																						 [in] BSTR sWindowName,
																						 [in, defaultvalue(0)] long nUnds,
																						 [in, defaultvalue(0)] long nOpts,
																						 [in, defaultvalue(0)] long nFuts,
																						 [in, defaultvalue(-1)] long nTime);

	[id(5),helpstring("method FinishLogMmOperation")] HRESULT  FinishLogMmOperation([in] LONG nOperation, 
																					[in] LogLevelEnum enLogLevel,
																					[in] eOperType enOperation, 
																					[in] BSTR sWindowName,
																					[in, defaultvalue(0)] long nUnds,
																					[in, defaultvalue(0)] long nOpts,
																					[in, defaultvalue(0)] long nFuts,
																					[in, defaultvalue(-1)] long nTime);

	[id(6), helpstring("method CheckLogMmOperation")] HRESULT CheckLogMmOperation([in] LONG nOperation, [out,retval] LONG* nResult);
	[propget, id(7), helpstring("property MinLogLevel")] HRESULT MinLogLevel([out, retval] LogLevelEnum* pVal);
	[propput, id(7), helpstring("property MinLogLevel")] HRESULT MinLogLevel([in] LogLevelEnum newVal);
	[id(8), helpstring("method LogMmInfo")] HRESULT LogMmInfo([in] LogLevelEnum LogLevel, [in] BSTR bsInformation, [in] BSTR sWindowName);
	[id(9), helpstring("method LogPerformanceCounters")] HRESULT LogPerformanceCounters(LogLevelEnum LogLevel);

	[propget, id(10), helpstring("property LogLifetime")]    HRESULT LogLifetime([out, retval] LONG* pVal);
	[propput, id(10), helpstring("property LogLifetime")]    HRESULT LogLifetime([in] LONG newVal);
	[id(11), helpstring("method RemoveOldFiles")] HRESULT RemoveOldFiles(void);
	[propget, id(12), helpstring("property MaxSize")] HRESULT MaxSize([out, retval] LONG* pVal);
	[propput, id(12), helpstring("property MaxSize")] HRESULT MaxSize([in] LONG newVal);
	[id(33), helpstring("method SetFilePathType")] HRESULT SetFilePathType([in] eFilderPathEnum lType);
};


struct SLogLine
{
public:

	SLogLine() : m_bEasternTimeInitalized(false)
	{ 
		GetLocalTime(&m_st);
		::ZeroMemory(&m_stEastern, sizeof(m_stEastern));
	}

	_bstr_t			m_bsInform;
	_bstr_t			m_bsWindow;

	LogLevelEnum	m_enLogLevel;
private:
	SYSTEMTIME		m_st;
	SYSTEMTIME      m_stEastern;
	bool            m_bEasternTimeInitalized;


	CString           m_strBuffer;
	void InitializeTime();
public:
	const SYSTEMTIME& GetTime()
	{
		InitializeTime();
		return     m_stEastern;
	}

	CString& GetLogLine()
	{
		InitializeTime();
		m_strBuffer.Format(_T("%-2.2i:%-2.2i:%-2.2i.%03i %d %s\t%s"), m_stEastern.wHour, m_stEastern.wMinute, m_stEastern.wSecond, m_stEastern.wMilliseconds, static_cast<long>(m_enLogLevel),(LPCSTR)m_bsWindow,(LPCSTR)m_bsInform);
		return m_strBuffer;
	}
};

typedef boost::shared_ptr<SLogLine> SLogLinePtr;

// CETSLog

[
	coclass,
	threading(apartment),
	support_error_info("IETSLog"),
	vi_progid("ETSLogsManager.ETSLog"),
	progid("ETSLogsManager.ETSLog.1"),
	version(1.0),
	uuid("421B31E0-8439-45BD-AE7F-64391030605F"),
	helpstring("ETSLog Class")
]
class ATL_NO_VTABLE CETSLog : 
	public IETSLog,
	public EgLib::CEgLibTraceManager
{
	typedef std::map<LONG, DWORD> LogsCollection;
public:
	CETSLog()
		:EgLib::CEgLibTraceManager(EgLib::CEgLibTraceManager::enRelativeMyDocuments , _T("ETS\\Logs"))
		,m_hPDHQuery(0)
		,m_hPDHCntCm(0)
		,m_hPDHCntAm(0)
		,m_hPDHCntPt(0)
		,m_hPDHCntMmPm(0)
		,m_hPDHCntMmPt(0)
		,m_nOperation(0)
		,m_enMinLogLevel(enLogAll)

	{
    }


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		PDH_STATUS				pdhStatus;
		m_Operations.clear();

		EgLib::CEgLibTraceManager::MinLogLevel = static_cast<long>(m_enMinLogLevel);

		pdhStatus = PdhOpenQuery (0, 0, &m_hPDHQuery);
		if(ERROR_SUCCESS == pdhStatus) 
		{
			PdhAddCounter(m_hPDHQuery, _T("\\Memory\\Committed Bytes"), 0, &m_hPDHCntCm);
			PdhAddCounter(m_hPDHQuery, _T("\\Memory\\Available Bytes"), 0, &m_hPDHCntAm);
			PdhAddCounter(m_hPDHQuery, _T("\\Processor(_Total)\\% Processor Time"), 0, &m_hPDHCntPt);
			PdhAddCounter(m_hPDHQuery, _T("\\Process(EtsMM)\\Private Bytes"), 0, &m_hPDHCntMmPm);
			PdhAddCounter(m_hPDHQuery, _T("\\Process(EtsMM)\\% Processor Time"), 0, &m_hPDHCntMmPt);
		} 
		return S_OK;		  
	}
	
	void FinalRelease() 
	{
		//Stop();
		if(m_hPDHQuery)
			PdhCloseQuery(m_hPDHQuery);
	}

public:

	//void OnData(SLogLinePtr& data);
private:

	HQUERY		 m_hPDHQuery;
	PDH_HCOUNTER m_hPDHCntCm;
	PDH_HCOUNTER m_hPDHCntAm;
	PDH_HCOUNTER m_hPDHCntPt;
	PDH_HCOUNTER m_hPDHCntMmPm;
	PDH_HCOUNTER m_hPDHCntMmPt;


	LogsCollection		m_Operations; 
	unsigned long       m_nOperation;          
	LogLevelEnum	    m_enMinLogLevel;

private:
//	void TraceToFile(LogLevelEnum LogLevel, _bstr_t bsWindowName, _bstr_t bsInformation);
//	void RemoveOldLogFiles(const _bstr_t rsDirName, const _bstr_t rsFileName, 	const _bstr_t rsFileNameMask);
//	void CreateNewFileName(const SYSTEMTIME& st);
//
//	void AddLogInfoQueue(LogLevelEnum LogLevel, _bstr_t bsWindowName, _bstr_t bsInformation);

public:
	STDMETHOD(get_LogLifetime)(LONG* pVal);
	STDMETHOD(put_LogLifetime)(LONG newpVal);

	STDMETHOD(get_FileName)(BSTR* pVal);
	STDMETHOD(put_FileName)(BSTR newpVal);
	STDMETHOD(get_FilePath)(BSTR* pVal);
	STDMETHOD(put_FilePath)(BSTR pVal);
	STDMETHOD(BeginLogMmOperation)(LONG* pVal);
	STDMETHOD(ContinueLogMmOperation)(LONG nOperation, LogLevelEnum enLogLevel, eOperType enOperation, BSTR sWindowName, long nUnds, long nOpts, long nFuts, long nTime);
	STDMETHOD(FinishLogMmOperation)(LONG nOperation, LogLevelEnum enLogLevel, eOperType enOperation, BSTR sWindowName, long nUnds, long nOpts, long nFuts, long nTime);
	STDMETHOD(CheckLogMmOperation)(LONG nOperation, LONG* nResult);
	STDMETHOD(get_MinLogLevel)(LogLevelEnum* pVal);
	STDMETHOD(put_MinLogLevel)(LogLevelEnum newVal);
	STDMETHOD(LogMmInfo)(LogLevelEnum LogLevel, BSTR bsInformation, BSTR sWindowName);
	STDMETHOD(LogPerformanceCounters)(LogLevelEnum LogLevel);
	STDMETHOD(RemoveOldFiles)(void);
	STDMETHOD(get_MaxSize)(LONG* pVal);
	STDMETHOD(put_MaxSize)(LONG newVal);
	STDMETHOD(SetFilePathType)(eFilderPathEnum newVal);
};

