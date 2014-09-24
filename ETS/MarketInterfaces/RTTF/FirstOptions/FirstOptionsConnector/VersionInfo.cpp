// VersionInfo.cpp: implementation of the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VersionInfo.h"
#include <winver.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVersionInfo::CVersionInfo(const string& FileToQuery)
{
	memset(&m_VersionInfo, 0, sizeof(m_VersionInfo));

	CHAR szFileName[MAX_PATH + 1];
	memset(szFileName, 0, (MAX_PATH + 1) * sizeof(CHAR));
	
	if (FileToQuery.size() == 0)
		GetModuleFileName(0, szFileName, MAX_PATH);
	else
		strcpy(szFileName, FileToQuery.c_str());

	DWORD VerInfoSize = GetFileVersionInfoSize(szFileName, 0);
	PVOID pVerInfoBuffer = 0;

	if (0 != VerInfoSize)
	{
		pVerInfoBuffer = new BYTE[VerInfoSize];
		if (FALSE != GetFileVersionInfo(szFileName, 0, VerInfoSize, pVerInfoBuffer))
		{
			VS_FIXEDFILEINFO * pVerInfo = 0;		
			UINT uiSize = 0;
	
			if (FALSE != VerQueryValue(pVerInfoBuffer, "\\", (PVOID*)&pVerInfo, &uiSize) && 
				uiSize >= sizeof(VS_FIXEDFILEINFO))
				memcpy(&m_VersionInfo, pVerInfo, sizeof(m_VersionInfo));

			delete [] pVerInfoBuffer;
		}
	}
}


CVersionInfo::~CVersionInfo()
{
}

string CVersionInfo::GetVersionAsString()
{
	string sVerNum("");
	CHAR szVerBuff[256];

	sprintf(szVerBuff, "[%d.%d.%d.%d]", 
			HIWORD(m_VersionInfo.dwFileVersionMS), 
			LOWORD(m_VersionInfo.dwFileVersionMS), 
			HIWORD(m_VersionInfo.dwFileVersionLS), 
			LOWORD(m_VersionInfo.dwFileVersionLS));

	return sVerNum = szVerBuff;
}