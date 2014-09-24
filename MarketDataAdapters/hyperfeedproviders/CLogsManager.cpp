// CLogsManager.cpp : Implementation of CCLogsManager

#include "stdafx.h"
#include "CLogsManager.h"
#include ".\clogsmanager.h"

// CCLogsManager
STDMETHODIMP CSettingsManager::get_MinLogLevel(long* pVal)
{

	if(pVal )
		*pVal = EgLib::CEgLibTraceManager::GetMinLoggingLevel();
	return S_OK;
}

STDMETHODIMP CSettingsManager::put_MinLogLevel(long newVal)
{
	EgLib::CEgLibTraceManager::SetMinLoggingLevel(newVal);
	return S_OK;
}

STDMETHODIMP CSettingsManager::get_SymbolFilter(BSTR* pVal)
{
	if(pVal)
	{
		_bstr_t bsFilter(CPrice::m_hfServer.GetSymbolFilter().c_str());
		*pVal = bsFilter.copy();
	}
	return S_OK;
}
STDMETHODIMP CSettingsManager::put_SymbolFilter(BSTR newVal)
{
	_bstr_t bsFilter(newVal!=NULL?newVal:L"");
	CPrice::m_hfServer.SetSymbolFilter((LPCSTR)bsFilter);
	return S_OK;
}

STDMETHODIMP CSettingsManager::get_ThreadPoolCapacity(long* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = CPrice::m_hfServer.GetThreadPoolCapacity();

	return S_OK;
}
STDMETHODIMP CSettingsManager::put_ThreadPoolCapacity(long newVal)
{
	if(newVal > 10)
		newVal = 10;
	if(newVal < -5)
		newVal = -15;

	CPrice::m_hfServer.SetThreadPoolCapacity(newVal);
    return S_OK;
}
