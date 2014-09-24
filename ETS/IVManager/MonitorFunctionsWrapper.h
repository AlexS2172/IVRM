#ifndef __MONITOR_FUNCTIONS_WRAPPER
#define __MONITOR_FUNCTIONS_WRAPPER


typedef HMONITOR (WINAPI *MonitorFromWindowPtr)( HWND hwnd, DWORD dwFlags);
typedef HMONITOR (WINAPI *MonitorFromPointPtr)( POINT pt, DWORD dwFlags);
typedef BOOL (WINAPI *GetMonitorInfoPtr)( HMONITOR hMonitor, LPMONITORINFO lpmi);


class CMonitorFunctionsWrapper
{
public:
	static HMONITOR MonitorFromWindow( HWND hWnd )
	{
		if( m_fpMonitorFromWindow )
		{
			return m_fpMonitorFromWindow( hWnd, MONITOR_DEFAULTTONEAREST );
		}

		return NULL;
	}

	static HMONITOR MonitorFromPoint( POINT pt )
	{
		if( m_fpMonitorFromPoint )
		{
			return m_fpMonitorFromPoint( pt, MONITOR_DEFAULTTONEAREST );
		}

		return NULL;
	}

	static BOOL GetMonitorInfo( HMONITOR hMonitor, LPMONITORINFO lpmi )
	{
		if( m_fpGetMonitorInfo )
		{
			return m_fpGetMonitorInfo( hMonitor, lpmi );
		}

		return FALSE;
	}

private:
	static HMODULE				m_hUser32Lib;
	static MonitorFromWindowPtr m_fpMonitorFromWindow;
	static MonitorFromPointPtr  m_fpMonitorFromPoint;
	static GetMonitorInfoPtr	m_fpGetMonitorInfo;
};

#endif//__MONITOR_FUNCTIONS_WRAPPER