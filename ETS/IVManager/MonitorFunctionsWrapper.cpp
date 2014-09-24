#include "stdafx.h"

#include "MonitorFunctionsWrapper.h"


HMODULE CMonitorFunctionsWrapper::m_hUser32Lib = GetModuleHandle( _T("User32.dll") );

MonitorFromWindowPtr CMonitorFunctionsWrapper::m_fpMonitorFromWindow = (MonitorFromWindowPtr)GetProcAddress( CMonitorFunctionsWrapper::m_hUser32Lib, _T("MonitorFromWindow") );
MonitorFromPointPtr  CMonitorFunctionsWrapper::m_fpMonitorFromPoint  = (MonitorFromPointPtr)GetProcAddress( CMonitorFunctionsWrapper::m_hUser32Lib, _T("MonitorFromPoint") );
#ifdef UNICODE
GetMonitorInfoPtr CMonitorFunctionsWrapper::m_fpGetMonitorInfo = (GetMonitorInfoPtr)GetProcAddress( CMonitorFunctionsWrapper::m_hUser32Lib, _T("GetMonitorInfoW") );
#else
GetMonitorInfoPtr CMonitorFunctionsWrapper::m_fpGetMonitorInfo = (GetMonitorInfoPtr)GetProcAddress( CMonitorFunctionsWrapper::m_hUser32Lib, _T("GetMonitorInfoA") );
#endif