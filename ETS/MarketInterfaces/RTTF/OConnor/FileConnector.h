#ifndef __FILE_CONNECTOR_H__
#define __FILE_CONNECTOR_H__

#include "ThreadWrapper.h"
#include "OConst.h"
#include "Publisher.h"

class CMyInternetSession : public CInternetSession
{
public:
	CMyInternetSession(LPCTSTR pstrAgent = NULL,
		DWORD dwContext = 1,
		DWORD dwAccessType = PRE_CONFIG_INTERNET_ACCESS,
		LPCTSTR pstrProxyName = NULL,
		LPCTSTR pstrProxyBypass = NULL,
		DWORD dwFlags = 0)
	{
		CInternetSession(pstrAgent, dwContext, dwAccessType,
							pstrProxyName, pstrProxyBypass, dwFlags);
	}

	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus,
		LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
	{	
		LPSOCKADDR lpSAddr = reinterpret_cast<LPSOCKADDR>(lpvStatusInformation);
	}

};

/////////////////////////////////////////////////////////////////////////////////////
class CFileConnector : public CThreadWrapper
{
// ThreadWrapper methods
private:
	UINT  virtual __stdcall Thread(void* pParam);

// Own public methods
	CFileConnector(){};
public:
	CFileConnector(std::string strSessionID);
    virtual ~CFileConnector();

	DWORD __stdcall Start(void* pParam);
	DWORD __stdcall Stop();

// settings
private:
	DWORD SaveSettings();
	DWORD ReadSettings();
    int ParseTradeLine(CString& strTrade);
	int ParseTradeLineEzeCastle(CString& strTrade);
	int GetLineTypeEze(CString& strLine);
	int CheckFileFormat(CString& strTrade);
	int GetElement(const CString  *ptrLine, int  nElement, CString  &strElemnt);
    BOOL ParsePositionLine(CString& strTrade);
    BOOL DownloadPositionsFile();
	BOOL DownloadTradesFile();
	_bstr_t DownloadFile(_bstr_t fileName, DATE vtLastDate=(DATE)0.);

public:
	void FileFound(bool bStatus)		{m_bFileFound      = bStatus;}
	void FileFoundDateTime(DATE dtFile) {m_dtFoundFileDate = dtFile;}

protected:
	
    virtual const CPublisher::TAccounts& GetAccounts() = 0;
    virtual bool OnTrade(CTradePtr& trade) = 0;

private:
	DWORD DeleteDestinationFile(_bstr_t strAscFilePath);
	_bstr_t DoDecode(_bstr_t bsSource);
	HANDLE GeneratePasswordFile();

private:

	std::string			  m_strSessionID;
	bool                  m_bFileFound;
	vt_date               m_dtFoundFileDate;
    CMyInternetSession    m_Session;
    _bstr_t             m_bsServerAddress;
    _bstr_t             m_bsUser;
    _bstr_t             m_bsPassword;
    _bstr_t             m_bsTradesFile;
    _bstr_t             m_bsPositionsFile;
	_bstr_t             m_bsPassPhrase;
	bool                m_bUseDecryption;


    DWORD               m_nReadDataPortion;
    DWORD               m_nReadTradesPeriod;
    DATE                m_dtPositionsDownload;
	DWORD				m_dwLastTradeDate;
	DWORD               m_dwLastTradeTime;
	DWORD				m_dwLastTradeLine;
	bool				m_bAlreadyDoneToday;
	bool                m_bIsPassiveMode;

	_bstr_t             m_strGnuPgPath;

};

#endif // __FILE_CONNECTOR_H__