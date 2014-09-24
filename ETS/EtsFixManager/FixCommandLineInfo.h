#pragma once
#include "afxwin.h"

class CFixCommandLineInfo :
	public CCommandLineInfo
{
public:
	CFixCommandLineInfo(void) : m_bIsBackground(false) {};
	~CFixCommandLineInfo(void) {};

	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
#ifdef _UNICODE
	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);
#endif

	bool m_bIsBackground;
};
