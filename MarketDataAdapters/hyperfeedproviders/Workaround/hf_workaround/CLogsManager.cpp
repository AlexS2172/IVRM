// CLogsManager.cpp : Implementation of CCLogsManager

#include "stdafx.h"
#include "CLogsManager.h"
#include ".\clogsmanager.h"

// CCLogsManager
STDMETHODIMP CSettingsManager::get_MinLogLevel(long* pVal)
{

	if(pVal )
		*pVal = EgStd::CEgTracingClass::GetMinLogLevel();
	return S_OK;
}

STDMETHODIMP CSettingsManager::put_MinLogLevel(long newVal)
{
	EgStd::CEgTracingClass::SetMinLogLevel(newVal);
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
