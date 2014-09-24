#pragma once

/////////////////////////////////////////////////////////////////////////////
//
class CEodReportRec
{
public:
	CEodReportRec(void);
	virtual ~CEodReportRec(void);

	bool ReadFromString(const CString& strRecord, long& nStep);
	long GetTypeIconResID() const;
	COLORREF GetTypeBackColor() const
	{
		return CEodReportRec::GetTypeBackColor(m_nType);
	}

	COLORREF GetTypeForeColor() const
	{
		return CEodReportRec::GetTypeForeColor(m_nType);
	}

	static COLORREF GetTypeBackColor(long nType);
	static COLORREF GetTypeForeColor(long nType);

public:
	COleDateTime	m_dtTime;
	long			m_nType;
	CString			m_strMessage;
};

typedef vector<CEodReportRec>				CEodReportRecVec;

/////////////////////////////////////////////////////////////////////////////
//
class CEodReportStep
{
public:
	CEodReportStep()
		: m_nProgress(0L), m_bSelected(false)
	{
	}

	~CEodReportStep()
	{
	}

	void AddRecord(CEodReportRec& aRec)
	{
		m_bSelected = true;
		if(aRec.m_nType != enRpMtProgress)
			m_vecRecs.push_back(aRec);
		else
		{
			m_nProgress = _ttol(aRec.m_strMessage);
			if(m_nProgress < 0L)
				m_nProgress = 0L;
			else if(m_nProgress > 100L)
				m_nProgress = 100L;
		}
	}

	const CEodReportRecVec& GetRecs() const
	{
		return m_vecRecs;
	}

	long GetProgress() const
	{
		return m_nProgress;
	}

	__declspec(property(get = GetIsSelected, put = SetIsSelected)) bool IsSelected;
		void SetIsSelected(bool bSelected)
		{
			m_bSelected = bSelected;
		}
		bool GetIsSelected() const
		{
			return m_bSelected;
		}

	static CString StepDescription(long nStep);

protected:
	static long _StepDescriptionID(long nStep);

protected:
	CEodReportRecVec		m_vecRecs;
	long					m_nProgress;
	bool					m_bSelected;
};

typedef map<long, CEodReportStep>			CEodReportStepMap;