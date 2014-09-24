#ifndef ISE_STATUS_H
#define ISE_STATUS_H

#define CSTATUS_FAILED(cstatus) ((cstatus) < 0)
#define CSTATUS_SUCCEEDED(cstatus) ((cstatus) >= 0)

class CISESession;

typedef enum
{
	enDisconnect,
	enCommon
} IseErrorClass;

char* GetTextError(CISESession* pSession, const int32 iStatus);
void GetErrorClass(int32 iStatus, IseErrorClass& Class);

class CISEException
{
private:

	// NOTE we cannot call omni_get_message, to translate codes to strings,
    // until we are logged in. Brilliant eh.
	CISESession*		m_pSession;
	string				m_sDescription;
	IseErrorClass		m_ErrorClass;
	int32	m_iCStatus;
	int32	m_iTransactionStatus;

private:

	void GetErrorString(char* szBuf) const;

public:
	CISEException(const int32 iCStatus, const int32 iTransactionStatus, 
		CISESession* pSession, const char* szDescription): 

		m_iCStatus(iCStatus), m_pSession(pSession),
		m_sDescription(szDescription),
		m_iTransactionStatus(iTransactionStatus)
	{
		char szBuf[4096];
		GetErrorString(szBuf);
		m_sDescription = szBuf;
		GetErrorClass(iCStatus, m_ErrorClass);
	}

	void AddDescription(const string& szDescription)
	{
		m_sDescription = szDescription + " @ " + m_sDescription;
	}

	bool IsConnectionError() const {return m_ErrorClass == enDisconnect;}

	const char* const GetDescription() {return m_sDescription.c_str();}

	HRESULT	GetHRESULT();
};

#endif