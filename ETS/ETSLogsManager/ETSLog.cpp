// ETSLog.cpp : Implementation of CETSLog

#include "stdafx.h"
#include "ETSLog.h"
#include ".\etslog.h"

// CETSLog
STDMETHODIMP CETSLog::put_LogLifetime(LONG newVal)
{
	EgLib::CEgLibTraceManager::LogLifetime = newVal;
	return S_OK;
}

STDMETHODIMP CETSLog::SetFilePathType(eFilderPathEnum newVal)
{
	switch(newVal)
	{
		case enAbsolute:
			EgLib::CEgLibTraceManager::SetPathType(EgLib::CEgLibTraceManager::enAbsolute);
			break;
		case enRelativeCurrent:
			EgLib::CEgLibTraceManager::SetPathType(EgLib::CEgLibTraceManager::enRelativeCurrent);
			break;
		case enRelativeMyDocuments:
			EgLib::CEgLibTraceManager::SetPathType(EgLib::CEgLibTraceManager::enRelativeMyDocuments);
			break;
	}
	return S_OK;
}

STDMETHODIMP CETSLog::get_LogLifetime(LONG* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal =  EgLib::CEgLibTraceManager::LogLifetime;
	return S_OK;
}

STDMETHODIMP CETSLog::put_FileName(BSTR newVal)
{
	_bstr_t bsName(newVal);
	if(EgLib::CEgLibTraceManager::FileName != bsName)
		EgLib::CEgLibTraceManager::FileName = bsName;

	return S_OK;
}
              
STDMETHODIMP CETSLog::get_FileName(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = EgLib::CEgLibTraceManager::FileName.copy();
	return S_OK;
}

STDMETHODIMP CETSLog::get_FilePath(BSTR* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = EgLib::CEgLibTraceManager::FilePath.copy();
	return S_OK;
}

STDMETHODIMP CETSLog::put_FilePath(BSTR pnewVal)
{

	EgLib::CEgLibTraceManager::FilePath  = pnewVal ;
	return S_OK;
}
      
STDMETHODIMP CETSLog::BeginLogMmOperation(LONG* pVal)
{
	DWORD dwTime = GetTickCount();  
	m_Operations.insert(std::pair<LONG, DWORD>(++m_nOperation, dwTime));
	*pVal = m_nOperation;
	return S_OK;
}

STDMETHODIMP CETSLog::FinishLogMmOperation(LONG nOperation, LogLevelEnum enLogLevel, eOperType enOperation, BSTR sWindowName, long nUnds, long nOpts, long nFuts, long nTime)
{
	HRESULT hr = ContinueLogMmOperation(nOperation, enLogLevel, enOperation, sWindowName, nUnds, nOpts, nFuts, nTime);
	m_Operations.erase(nOperation);
	return hr;
}

STDMETHODIMP CETSLog::ContinueLogMmOperation(LONG nOperation, LogLevelEnum enLogLevel, eOperType enOperation, BSTR sWindowName, long nUnds, long nOpts, long nFuts, long nTime)
{
	HRESULT hr = S_OK;
    if(enLogLevel > m_enMinLogLevel)
		return hr;

	std::string sInfo;//(sWindowName);

		switch (enOperation)
		{
		case OPER_OPENLAYOUT:
				sInfo += _T("opening layout file\t");
				break;
		case OPER_LOADINITDATA:
				sInfo += _T("loading init data\t");
				break;
		case OPER_LOADTRADESDATA:
				sInfo += _T("loading trades data\t");
				break;
		case OPER_OPENWINDOW:
				sInfo += _T("opening window\t");
				break;
		case OPER_ACTIVATEWINDOW:
				sInfo += _T("active window\t");
				break;
		case OPER_LOADDATA:
				sInfo += _T("\tloading data\t");
				break;
		case OPER_REQUESTQUOTE:
				sInfo += _T("queering quotes\t");
				break;
		case OPER_SUBSCRIBEQUOTE:
				sInfo += _T("subscribing quotes\t");
				break;
		case OPER_CALCULATION:
				sInfo += _T("calc quotes\t");
				break;
		case OPER_REFRESHSCREEN:
				sInfo += _T("updating screen\t");
				break;
		case OPER_CALCREFRESHSCREEN:
				sInfo += _T("calc & updating\t");
				break;                         
		default:
			return E_FAIL;
		}
		TCHAR buffer[1024];
		LONG lOpTime = nTime;
		if(nTime < 0)
			CheckLogMmOperation(nOperation, &lOpTime);

		_sntprintf_s(buffer, sizeof(buffer), 1024,"%d\t%d\t%d\t%d", nUnds, nOpts, nFuts, lOpTime);
		sInfo += buffer;

		EgLib::CEgLibTraceManager::LogInfo(static_cast<long>(enLogLevel), _bstr_t(sWindowName), sInfo.c_str());
		//TraceToFile(sInfo);
	return hr;
}

STDMETHODIMP CETSLog::CheckLogMmOperation(LONG nOperation, LONG* nResult)
{
	DWORD dwOp = 0;
	LogsCollection::iterator itr = m_Operations.find(nOperation);
	if(itr !=m_Operations.end())
		*nResult = GetTickCount() -	 itr->second;
	else
		*nResult = 0;

	return S_OK;
}

STDMETHODIMP CETSLog::get_MinLogLevel(LogLevelEnum* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = static_cast<LogLevelEnum>(EgLib::CEgLibTraceManager::MinLogLevel);
	return S_OK;
}

STDMETHODIMP CETSLog::put_MinLogLevel(LogLevelEnum newVal)
{
	EgLib::CEgLibTraceManager::MinLogLevel = static_cast<long>(newVal);      
	return S_OK;
}

STDMETHODIMP CETSLog::LogMmInfo(LogLevelEnum LogLevel, BSTR sInformation, BSTR sWindowName)
{

	HRESULT hr = S_OK;
	EgLib::CEgLibTraceManager::LogInfoText(static_cast<long>(LogLevel), CW2A(sWindowName), CW2A(sInformation));
	return hr;
}

STDMETHODIMP CETSLog::LogPerformanceCounters(LogLevelEnum LogLevel)
{
	HRESULT		hr				= S_OK;
	double		fVal			= 0.0;
	PDH_STATUS  ngCounterStatus = 0;

	std::string sInfo;
	if(!m_Operations.empty() && LogLevel<=m_enMinLogLevel)
	{
		if(ERROR_SUCCESS == PdhCollectQueryData(m_hPDHQuery))
		{
			{
				PDH_FMT_COUNTERVALUE counterValue;   
				DWORD dwType = 0 ;
				if(ERROR_SUCCESS == PdhGetFormattedCounterValue(m_hPDHCntCm,PDH_FMT_DOUBLE, &dwType, &counterValue))
				{
					sInfo += "RAM: (";
					sInfo += (LPCSTR)_bstr_t((double)counterValue.doubleValue/0x100000);
					sInfo += ")\t";
				}
			}
			{
				PDH_FMT_COUNTERVALUE counterValue;   
				DWORD dwType = 0 ;
				if(ERROR_SUCCESS == PdhGetFormattedCounterValue(m_hPDHCntAm, PDH_FMT_DOUBLE, &dwType, &counterValue))
				{
					sInfo += "Committed RAM: (";
					sInfo += (LPCSTR)_bstr_t((double)counterValue.doubleValue/0x100000);
					sInfo += ")\t";
				}
			}
			{
				PDH_FMT_COUNTERVALUE counterValue;   
				DWORD dwType = 0 ;
				if(ERROR_SUCCESS == PdhGetFormattedCounterValue(m_hPDHCntMmPm, PDH_FMT_DOUBLE, &dwType, &counterValue))
				{
					sInfo += "ETSMM PrivateMem: (";
					sInfo += (LPCSTR)_bstr_t((double)counterValue.doubleValue/0x100000);
					sInfo += ")\t";
				}
			}
			{
				PDH_FMT_COUNTERVALUE counterValue;   
				DWORD dwType = 0 ;
				if(ERROR_SUCCESS == PdhGetFormattedCounterValue(m_hPDHCntPt,PDH_FMT_DOUBLE, &dwType, &counterValue))
				{
					sInfo += "CPU Total: (";
					sInfo += (LPCSTR)_bstr_t((double)counterValue.doubleValue);
					sInfo += ")\t";
				}
			}
			{
				PDH_FMT_COUNTERVALUE counterValue;   
				DWORD dwType = 0 ;
				if(ERROR_SUCCESS == PdhGetFormattedCounterValue(m_hPDHCntMmPt, PDH_FMT_DOUBLE, &dwType, &counterValue))
				{
					sInfo += "CPU ETSMM: (";
					sInfo += (LPCSTR)_bstr_t((double)counterValue.doubleValue);
					sInfo += ")\t";
				}
			}

		}
		EgLib::CEgLibTraceManager::LogInfo(static_cast<long>(LogLevel), _bstr_t(L""), sInfo.c_str());
	}
	return S_OK;
}

STDMETHODIMP CETSLog::RemoveOldFiles(void)
{
	
	_bstr_t bsFileNameMask = EgLib::CEgLibTraceManager::FilePath ;
	bsFileNameMask += _T("*");
	bsFileNameMask += _T(".log");

	EgLib::CEgLibTraceManager::RemoveOldLogFiles ( EgLib::CEgLibTraceManager::FilePath, EgLib::CEgLibTraceManager::FileName, bsFileNameMask ) ;

	return S_OK;
}

STDMETHODIMP CETSLog::get_MaxSize(LONG* pVal)
{
	if(!pVal)
		return E_POINTER;

	*pVal = EgLib::CEgLibTraceManager::FileMaxSize ;

	return S_OK;
}

STDMETHODIMP CETSLog::put_MaxSize(LONG newVal)
{
	EgLib::CEgLibTraceManager::FileMaxSize = newVal ;

	return S_OK;
}
