#pragma once
#include "afxwin.h"

class CEodCommandLineInfo :
	public CCommandLineInfo
{
public:
	CEodCommandLineInfo(void) : m_bIsBackground(false) {};
	~CEodCommandLineInfo(void) {};

	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
#ifdef _UNICODE
	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);
#endif

	bool m_bIsBackground;
};
