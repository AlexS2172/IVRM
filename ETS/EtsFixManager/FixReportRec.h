#pragma once

/////////////////////////////////////////////////////////////////////////////
//
class CFixReportRec
{
public:
	CFixReportRec(void);
	virtual ~CFixReportRec(void);

	bool ReadFromString(const CString& strRecord, long& nStep);
	long GetTypeIconResID() const;
	COLORREF GetTypeBackColor() const
	{
		return CFixReportRec::GetTypeBackColor(m_nType);
	}

	COLORREF GetTypeForeColor() const
	{
		return CFixReportRec::GetTypeForeColor(m_nType);
	}

	static COLORREF GetTypeBackColor(long nType);
	static COLORREF GetTypeForeColor(long nType);

public:
	COleDateTime			m_dtTime;
	FgLogMessageTypeEnum	m_nType;
	CString					m_strMessage;
};

typedef vector<CFixReportRec>				CFixReportRecVec;
