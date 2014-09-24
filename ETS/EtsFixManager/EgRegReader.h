#pragma once

class CEgRegReader
{
public:
	CEgRegReader(LPCTSTR lpszKeyName);

	virtual ~CEgRegReader(void);

	long Read(LPCTSTR lpszValueName, long nDefault);
	CString Read(LPCTSTR lpszValueName, LPCTSTR lpszDefault);
	
private:
	bool	m_bIsLMOpen;
	bool	m_bIsCUOpen;
	CRegKey	m_kLM;
	CRegKey	m_kCU;
};
