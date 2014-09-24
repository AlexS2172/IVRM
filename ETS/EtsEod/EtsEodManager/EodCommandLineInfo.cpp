#include "StdAfx.h"
#include "EodCommandLineInfo.h"

void CEodCommandLineInfo::ParseParam(const TCHAR* pszParam,BOOL bFlag,BOOL bLast)
{
	if(bFlag)
	{
		USES_CONVERSION;
		if (lstrcmpA(T2CA(pszParam), "background") == 0)
			m_bIsBackground = true;		
	}

	__super::ParseParam(pszParam, bFlag, bLast);
}

#ifdef UNICODE
void CEodCommandLineInfo::ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
{
	if(bFlag)
	{
		if (lstrcmpA(pszParam, "background") == 0)
			m_bIsBackground = true;		
	}
	__super::ParseParam(pszParam, bFlag, bLast);
}
#endif // UNICODE
