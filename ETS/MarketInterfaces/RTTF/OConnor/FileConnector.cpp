#include "stdafx.h"

#include "OConst.h"
#include "FileConnector.h"
#include "Tracer.h"

#import "libid:FF8DA174-3574-11D4-8068-0060082AE372" version("1.1") rename_namespace("XFTP") rename("DeleteFile", "FTPDeleteFile")
using namespace XFTP;

class CSimpleXceedFtpEventsImpl : public IXceedFtpEvents
{
public:
	STDMETHODIMP QueryInterface( 
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void** ppvObject)
	{
		if( riid == IID_IUnknown )
			*ppvObject = static_cast< IUnknown* >( this );

		else if( riid == __uuidof(IXceedFtpEvents) )
			*ppvObject = static_cast<IXceedFtpEvents* >( this );
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}

		reinterpret_cast< IUnknown* >( *ppvObject )->AddRef();
		return S_OK;
	}

	STDMETHODIMP_(ULONG) AddRef(void)
	{
		return 2;
	}

	STDMETHODIMP_(ULONG) Release(void)
	{
		return 1;
	}

	virtual HRESULT __stdcall raw_Disconnected ( )
	{
		return S_OK;
	}

	virtual HRESULT __stdcall raw_FileMatched (
		BSTR sFilename,
		DATE dtDate,
		long lFileSize,
		enum EXFFolderItemType eItemType,
		BSTR sUserData,
		VARIANT_BOOL * bSkipFile )
	{
		return S_OK;
	}

	virtual HRESULT __stdcall raw_FileTransferStatus (
		BSTR sLocalFilename,
		BSTR sRemoteFilename,
		long lFileSize,
		long lBytesTransferred,
		short nBytesPercent,
		long lTotalSize,
		long lTotalBytesTransferred,
		short nTotalBytesPercent,
		long lTotalFiles,
		long lTotalFilesTransferred,
		short nTotalFilesPercent,
		long lBytesPerSecond,
		long lTotalBytesPerSecond )
	{ 
		if(CTracer::GetWindowHandle())
		{
			CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Loaded %d%% of %s"), nTotalBytesPercent, (LPCSTR)_bstr_t(sRemoteFilename));

			TCHAR   lpszErrorMsg[512] = {0};						
			_sntprintf(lpszErrorMsg, 512, _T("OConnor Connector: Download %s %d%%"), (LPCSTR)_bstr_t(sRemoteFilename), nTotalBytesPercent);
			::SetWindowText(CTracer::GetWindowHandle(), lpszErrorMsg);
		}

		return S_OK;
	}

	/*
	* This is the event we want to handle. We simply display the file information.
	*/

	virtual HRESULT __stdcall raw_ListingFolderItem (
		BSTR sName,
		DATE dtDate,
		long lFileSize,
		enum EXFFolderItemType eItemType,
		BSTR sUserData )
	{
		switch( eItemType )
		{
		case 0: //File
			CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("File %s is found. Size: %dK  Created %s"),(LPCSTR)_bstr_t(sName), (long)lFileSize/1024, (LPCSTR)_bstr_t(vt_date(dtDate).GetVtDate()));
			m_pFileConnector->FileFound(true);
			m_pFileConnector->FileFoundDateTime(dtDate);
			break;
		}
		return S_OK;
	}

	virtual HRESULT __stdcall raw_LoggingCommandLine (
		BSTR sLine,
		enum EXFCommandType eCommandType )
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("FTP Command: %s"), (LPCSTR)_bstr_t(sLine));
		return S_OK;
	}

	virtual HRESULT __stdcall raw_ParsingListLine (
		BSTR sLineToParse,
		BSTR * sName,
		DATE * dtDate,
		long * lFileSize,
		enum EXFFolderItemType * eItemType,
		BSTR * sUserData,
		VARIANT_BOOL * bParsed )
	{
		return S_OK;
	}

	virtual HRESULT __stdcall raw_ProcessCompleted (
		enum EXFState eCompletedState,
		EXFError eResult )
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("FTP Process Completed: %d with state %d"), (long) eCompletedState, (long)eResult);
		return S_OK;
	}

	virtual HRESULT __stdcall raw_ReceivingFile (
		BSTR sRemoteFilename,
		BSTR * sLocalFilename,
		long lFileSize )
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("FTP Receiving File: %d with state %d with size %dK"), (LPCSTR)_bstr_t(sRemoteFilename), (long)lFileSize/1024);

		return S_OK;
	}

	virtual HRESULT __stdcall raw_ReceivingMemoryFileData (
		BSTR sRemoteFilename,
		long lFileSize,
		VARIANT * vaData,
		VARIANT_BOOL bEndOfData )
	{
		return S_OK;
	}

	virtual HRESULT __stdcall raw_ReplacingFile (
		BSTR * sFilename,
		DATE dtDate,
		long lFileSize,
		BSTR sRemoteFilename,
		enum EXFReplaceAction * eAction )
	{
		return S_OK;
	}

	virtual HRESULT __stdcall raw_ScanningFolder (
		BSTR sFolder,
		VARIANT_BOOL * bSkipFolder )
	{
		return S_OK;
	}

	virtual HRESULT __stdcall raw_SendingFile (
		BSTR sLocalFilename,
		BSTR * sRemoteFilename,
		long lFileSize )
	{
		return S_OK;
	}

	virtual HRESULT __stdcall raw_SkippingFile (
		BSTR sLocalFilename,
		BSTR sRemoteFilename,
		EXFError eSkippingReason )
	{
		return S_OK;
	}

	virtual HRESULT __stdcall raw_AccountRequired (
		BSTR * sAccountName )
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("FTP Account Required"));
		return S_OK;
	}

	virtual HRESULT __stdcall raw_PasswordRequired (
		BSTR * sPassword )
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("FTP Password Required"));
		return S_OK;
	}
public:
	CFileConnector* m_pFileConnector;
};


///////////////////////////////////////////////////////////////////////////////////////
CFileConnector::CFileConnector(std::string strSessionID) 
	:m_Session(_T("O'Connor connector"))
	,m_strSessionID(strSessionID)
    ,m_nReadDataPortion(4096)
    ,m_nReadTradesPeriod(0)
    ,m_dtPositionsDownload(0)
	,m_dwLastTradeDate(0)
	,m_dwLastTradeTime(0)
	,m_dwLastTradeLine(0)
	,m_bAlreadyDoneToday(false)
	,m_bFileFound(false)
	,m_bUseDecryption(false)

{
}

///////////////////////////////////////////////////////////////////////////////////////
CFileConnector::~CFileConnector()
{
    m_Session.Close();
}

///////////////////////////////////////////////////////////////////////////////////////
DWORD CFileConnector::ReadSettings()
{
	long lResult = ERROR_SUCCESS;

	_bstr_t sbsKey = SETTINGS_XML_KEY;
	sbsKey += "\\";
	sbsKey += m_strSessionID.c_str();

	CXMLParamsHelper XMLParams;
	HRESULT hr = XMLParams.LoadXMLParams();
	if(hr == S_OK)
	{
		try
		{
			hr = XMLParams.GetMainXMLString(sbsKey, SETTINGS_SERVER_ADDRESS, &m_bsServerAddress);
			if(FAILED(hr) || m_bsServerAddress.length() == 0) throw _T("Failed to read 'server address' setting from the registry");

			hr = XMLParams.GetMainXMLString(sbsKey, SETTINGS_USER, &m_bsUser);
			hr = XMLParams.GetMainXMLString(sbsKey, SETTINGS_PASSWORD, &m_bsPassword);

			long nPassive = 0;
			hr = XMLParams.GetMainXMLLong(sbsKey, SETTINGS_PASSIVEFTP, &nPassive);
			m_bIsPassiveMode = nPassive != 0;

			hr = XMLParams.GetMainXMLString(sbsKey, SETTINGS_TRADES_FILENAME, &m_bsTradesFile);
			if(FAILED(hr) || m_bsTradesFile.length() == 0) throw _T("Failed to read 'trades file' setting from the registry");

			hr = XMLParams.GetMainXMLString(sbsKey, SETTINGS_POSITIONS_FILENAME, &m_bsPositionsFile);
			if(FAILED(hr) || m_bsPositionsFile.length() == 0) throw _T("Failed to read 'positions file' setting from the registry");

			hr = XMLParams.GetMainXMLLong(sbsKey, SETTINGS_READ_DATA_PORTION, (long*)&m_nReadDataPortion);

			hr = XMLParams.GetMainXMLLong(sbsKey, SETTINGS_READ_TRADES_PERIOD, (long*)&m_nReadTradesPeriod);
			if(FAILED(hr) || m_nReadTradesPeriod == 0) throw _T("Failed to read 'read trades period' setting from the registry");

			hr = XMLParams.GetMainXMLString(sbsKey, SETTINGS_READ_PASSPHRASE, &m_bsPassPhrase);
			if(FAILED(hr)) throw _T("Failed to read 'read Pass Phrase' setting from the registry");

			long nRead = 0;
			hr = XMLParams.GetMainXMLLong(sbsKey, SETTINGS_READ_ENCRYPTED, &nRead);
			if(FAILED(hr)) throw _T("Failed to read 'read Encrypted Source' setting from the registry");
			m_bUseDecryption = nRead != 0;

			_bstr_t bsTime;
			hr = XMLParams.GetMainXMLString(sbsKey, SETTINGS_POSITIONS_DOWNLOAD_TIME, &bsTime);
			if(FAILED(hr) || bsTime.length() == 0) throw _T("Failed to read 'positions download time' setting from the registry");

			COleDateTime dtDownloadPositionTime;
			if(!dtDownloadPositionTime.ParseDateTime(bsTime, VAR_TIMEVALUEONLY))
			{
				lResult = ERROR_INVALID_DATA;
				throw _T("Failed to parse 'positions download time' setting from the registry");
			}

			COleDateTime dtCurrentTime(COleDateTime::GetCurrentTime());

			dtDownloadPositionTime.SetDateTime(dtCurrentTime.GetYear(), dtCurrentTime.GetMonth(), dtCurrentTime.GetDay(),
				dtDownloadPositionTime.GetHour(), dtDownloadPositionTime.GetMinute(), dtDownloadPositionTime.GetSecond());

			COleDateTimeSpan dtSpanOneDay(1L,0,0,0);
			COleDateTimeSpan dtSpanOneMinute(0L,0,1,0);
			if(dtCurrentTime + dtSpanOneMinute > dtDownloadPositionTime)
				dtDownloadPositionTime += dtSpanOneDay;

			m_dtPositionsDownload = dtDownloadPositionTime;

			hr = XMLParams.GetMainXMLLong(sbsKey, SETTINGS_TRADES_LAST_LINE, (long*)&m_dwLastTradeLine);

			hr = XMLParams.GetMainXMLLong(sbsKey, SETTINGS_TRADES_LAST_DATE, (long*)&m_dwLastTradeDate, (DWORD)(DATE)vt_date::GetCurrentDate() - 1);
			hr = XMLParams.GetMainXMLLong(sbsKey, SETTINGS_TRADES_LAST_TIME, (long*)&m_dwLastTradeTime);

			m_strGnuPgPath  = L"C:\\GnuPg\\";
			CEgRegKey regGpg;
			if(ERROR_SUCCESS == regGpg.Open(HKEY_LOCAL_MACHINE, _T("Software\\GNU\\GnuPG"), KEY_READ))
				regGpg.QueryValue(m_strGnuPgPath, _T("HomeDir"));

			if(!m_strGnuPgPath.length() && m_bUseDecryption)
			{
				lResult = ERROR_INVALID_DATA;
				throw _T("Failed to init GnuPg.exe");
			}

			lResult = ERROR_SUCCESS;
		}
		catch(const _TCHAR* szError)
		{
			CTracer::TraceWin32Error(lResult, szError);
			lResult = ERROR_INVALID_DATA;
		}
	}
	else
	{
		CTracer::TraceWin32Error(lResult, _T("Failed to read OConnor settings from the registry"));
		lResult = ERROR_INVALID_DATA;
	}

 	return lResult;
}

///////////////////////////////////////////////////////////////////////////////////////
DWORD CFileConnector::SaveSettings()
{
	long lResult = ERROR_SUCCESS;

	_bstr_t sbsKey = SETTINGS_XML_KEY;
	sbsKey += "\\";
	sbsKey += m_strSessionID.c_str();

	CXMLParamsHelper XMLParams;
	HRESULT hr = XMLParams.LoadXMLParams();
	if(hr == S_OK)
	{
		hr = XMLParams.SetMainXMLLong(sbsKey, SETTINGS_TRADES_LAST_TIME, m_dwLastTradeTime);
		hr = XMLParams.SetMainXMLLong(sbsKey, SETTINGS_TRADES_LAST_DATE, m_dwLastTradeDate);
		hr = XMLParams.SetMainXMLLong(sbsKey, SETTINGS_TRADES_LAST_LINE, m_dwLastTradeLine);
	}
	else
		lResult = ERROR_INVALID_DATA;

	hr = XMLParams.SaveMainXMLParams();

	return hr == S_OK ? ERROR_SUCCESS : ERROR_INVALID_DATA;
}

///////////////////////////////////////////////////////////////////////////////////////
DWORD __stdcall CFileConnector::Start(void* pParam)
{
    DWORD dwError = ReadSettings();
	if (dwError)
		return dwError;

	dwError = CThreadWrapper::Start(this);
	if (dwError)
		return CTracer::TraceWin32Error(dwError, _T("ThreadWrapper error."));

	return dwError;
}

///////////////////////////////////////////////////////////////////////////////////////
DWORD __stdcall CFileConnector::Stop()
{
    DWORD dwError = 0;
	if (dwError = CThreadWrapper::Stop() != 0)
		CTracer::TraceWin32Error(dwError, _T("ThreadWrapper error."));

	if (dwError = SaveSettings() != 0)
		CTracer::TraceWin32Error(dwError, _T("Failed to save settings."));

	return dwError;
}

///////////////////////////////////////////////////////////////////////////////////////
UINT  __stdcall CFileConnector::Thread(void* pParam)
{
    CFileConnector*	pFC = reinterpret_cast<CFileConnector*>(pParam);
    const LONG	nTimeout = pFC->m_nReadTradesPeriod * 1000;
    bool	bSucceeded = false;

	try
	{
		//bsPathBuffer +=pFC->m_bsTradesFile;


    //CInternetException  ie(ERROR_INTERNET_INTERNAL_ERROR);
		DWORD nTicks = 0; 	        

		//do 
		{
			nTicks = GetTickCount(); 
			try
			{
				if(!pFC->DownloadTradesFile() || !pFC->DownloadPositionsFile() )
					//break;
					return 0;
			}
			catch (_com_error &e)
			{
				TCHAR   lpszErrorMsg[512] = {0};						
				_sntprintf(lpszErrorMsg, 512, _T("Can not open URL. Error: %s %d"), e.Description().length()?(LPCSTR)e.Description():e.ErrorMessage(), e.Error());
				throw std::exception(lpszErrorMsg);
			}
			catch(const std::exception& e)
			{
				CTracer::TraceMessage(CTracer::enMtError, NULL, e.what());
			}

			nTicks += nTimeout - GetTickCount();
			if (nTicks < 0) 
				nTicks = 0;

		}
		//while (  WaitForSingleObject(GetStopEventHandle(), nTicks) != WAIT_OBJECT_0);
	}
	catch (_com_error &e)
	{
		TCHAR   lpszErrorMsg[512] = {0};						
		_sntprintf(lpszErrorMsg, 512, _T("Can not open URL. Error: %s %d"), e.Description().length()?(LPCSTR)e.Description():e.ErrorMessage(), e.Error());
		throw std::exception(lpszErrorMsg);
	}
	catch(const std::exception& e)
	{
		CTracer::TraceMessage(CTracer::enMtError, NULL, e.what());
	}
	return 0;
}

#define IS_FIELD_EXIST(field, line_len) ((offsetof(INPUTLINE, field) + sizeof(((INPUTLINE *)0)->field)) <= line_len)
#define GET_FIELD_VALUE(field, line)    CString(&line[offsetof(INPUTLINE, field)], sizeof(((INPUTLINE *)0)->field))
#define CHECK_FIELD_EXISTENCE(field, line_len, len) if (!IS_FIELD_EXIST(field, line_len))   \
    { CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field '%s' doesn't exist"), #field );  \
    return FALSE; }

//--------------------------------------------------------------------------------------------------------------//
int CFileConnector::GetElement(const CString  *ptrLine, int  nElement, CString  &strElemnt)
{
	if (ptrLine)
	{
		CString	str(*ptrLine);
		int cur_pos_s, cur_pos_e, iLen, cur_Elem, iDlt;

		cur_pos_s = cur_pos_e = cur_Elem = iDlt = 0;
		iLen = str.GetLength();

		str = _TEXT("|") + str + +_TEXT("|");

		while ((cur_Elem++ < nElement) && (cur_pos_e <= iLen))
		{
			cur_pos_s = str.Find(_TEXT("|"), cur_pos_e); 
			cur_pos_e = str.Find(_TEXT("|"), cur_pos_s+1);
		}

		iDlt = cur_pos_e - cur_pos_s - 1;
		if (iDlt)
			strElemnt =	str.Mid(cur_pos_s+1, iDlt).TrimLeft().TrimRight();

		return iDlt;
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------------------//
int CFileConnector::CheckFileFormat(CString& strTrade)
{
	//because of OCONNOR format not contain symbol "|" in trade line
	return (strTrade.Find(_T("|")) < 0 ? OCONNOR : EZECASTLE);
}
//--------------------------------------------------------------------------------------------------------------//
int CFileConnector::GetLineTypeEze(CString& strLine)
{
	int iRet	=	EZE_LINE_ERROR;
	int nCount	=	0;
	int nLen	=	strLine.GetLength();

	LPCSTR  lpszTrade = strLine.GetString();
	for(int j=0; j<nLen; j++)
	{
		if(lpszTrade[j] == '|') ++nCount;
	}

	switch(nCount)
	{
		case 3:
			iRet =  EZE_HEADER;
			break;
		case 22:
			iRet =	EZE_MAIN;
			break;
		case 2:
			iRet =  EZE_FOOTER;
			break;
		default:
			iRet =  EZE_LINE_ERROR;
			break;
	}

	return iRet;
}
//--------------------------------------------------------------------------------------------------------------//
int CFileConnector::ParseTradeLineEzeCastle(CString& strTrade)
{
	USES_CONVERSION;

	CTradePtr    pTrade(new CTrade());

	CString     sTmp;

	LPCSTR  lpszTrade = strTrade.GetString();

	//Security Type
	if (GetElement(&strTrade, enTfSecurityType, sTmp))
	{
		if (sTmp=="STOCK")
			pTrade->security_type = 'S';
		else if (sTmp=="OPTION")
			pTrade->security_type = 'O';
		else {
			CTracer::TraceMessage(CTracer::enMtInformation, lpszTrade, _T("Only Stocks and Options supported.") );
			return 0;
		}
	}
	else{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field Security Type not defined.") );
		return 0;
	}

	//trade id
	if (GetElement(&strTrade, enTfTradeID, sTmp))
		pTrade->trade_id = sTmp;
	else{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field tradeID not defined") );
		return 0;
	}

	//buy_sell
	if (GetElement(&strTrade, enTfTransaction, sTmp))
		pTrade->buy_sell = ((sTmp=="SSEL" || sTmp=="SELL") ? 'S': 'L');
	else{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field transactionID not defined") );
		return 0;
	}

	//product id
	if (GetElement(&strTrade, enTfProductID, sTmp)){
		pTrade->base_sym = sTmp;
		if (pTrade->security_type == "O")
			pTrade->trading_sym = sTmp.Left(sTmp.GetLength()-2);
		else
			pTrade->trading_sym = sTmp;
	}
	else{
		 CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field productID not defined") );
		 return 0;
	}

	//Order Quantity
	if (GetElement(&strTrade, enTfOrderQuantity, sTmp))
		pTrade->quantity = _ttol(sTmp);
	else{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field Order Quantity not defined") );
		return 0;
	}

	//Broker [not used]
	//Pricing Currency   [only USD]
	if (GetElement(&strTrade, enTfPricingCurrency, sTmp))
	{
		if (sTmp!="USD")
		{
			CTracer::TraceMessage(CTracer::enMtInformation, lpszTrade, _T("Field Pricing Currency incorrect. Note: USD only") );
			return 0;
		}
	}
	
	//Settlement Currency [not used]

	//Order Price
	if (GetElement(&strTrade, enTfOrderPrice, sTmp))
		pTrade->price = _tstof(sTmp);
	else{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field Order Currency not defined.") );
		return 0;
	}
	
	//Account Number
	if (GetElement(&strTrade, enTfAccountNumber, sTmp))
		pTrade->account = sTmp;
	else{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field Account Number not defined.") );
		return 0;
	}
	
	//Trade Date
	if(GetElement(&strTrade, enTfTradeDate, sTmp))
	{
		SYSTEMTIME stNow = {0};						
		::GetSystemTime(&stNow);
		pTrade->execution_date = (DATE)vt_date(stNow);
	}
	else{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field Trade Date not defined. It set to default.") );
	}

	if(GetElement(&strTrade, enTfUserDefinedField1, sTmp))
		pTrade->strategy = sTmp;
	else{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field Strategy not defined. It set to default.") );
	}

	//update flag
	if (0 == pTrade->quantity)
	{
		pTrade->ins_upd_del_flag = DEL_FLAG;
	}
	else
	{
		pTrade->ins_upd_del_flag = NEW_FLAG;
	}

	//Settle Date   [not used]
	//Custodian		[not used]

	//service parametr
	pTrade->exec_broker_id	= EZECASTLE;

	//Total Sec Fees		[not used]
	//Total Other Fees		[not used]
	//Total Commission		[not used]
	//Net Amount			[not used]
	//Tax Lot ID			[not used]

	return (int)OnTrade(pTrade);
}
//--------------------------------------------------------------------------------------------------------------//
int CFileConnector::ParseTradeLine(CString& strTrade)
{
	USES_CONVERSION;

    CTradePtr    pTrade(new CTrade());

    CString     sTmp;
    CString     sTmp2;
    double      dblTmp;

	//expiry date for options contracts
    long        lYear = 0;
    long        lMonth = 0;
    long        lDay = 0;
	
	//maturity date for future contract
	long		lmDay = 1;
	long		lmMonth = 0;
	long		lmYear = 0;
	
    LPCSTR  lpszTrade = strTrade.GetString();
    DWORD   dwLen     = strTrade.GetLength();

    const INPUTLINE*  pIL = reinterpret_cast<const INPUTLINE*>(lpszTrade);

    // trans_id
    CHECK_FIELD_EXISTENCE(trans_id, dwLen, lpszTrade)
    if ( GET_FIELD_VALUE(trans_id, lpszTrade) != _T("346") )
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'trans_id' not equal '346'") );
        return 0;
    }

    // record_id - ignore this field (from Joey Payumo)
    CHECK_FIELD_EXISTENCE(record_id, dwLen, lpszTrade)
    if ( !IsValidInsUpdDelFlag(pIL->record_id) )
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'record_id' is not valid") );
        return 0;
    }
    // They don't use column 4 at all, only 80 //pTrade->ins_upd_del_flag = /*NEW_FLAG*/pIL->record_id;

    // clearing_firm
    CHECK_FIELD_EXISTENCE(clearing_firm, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(clearing_firm, lpszTrade);
    sTmp.TrimLeft(_T(' '));
    pTrade->clearing_firm = sTmp;

    // account
    CHECK_FIELD_EXISTENCE(account, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(account, lpszTrade);
    sTmp.TrimRight(_T(' '));
    pTrade->account = sTmp;

// check account
//    const CPublisher::TAccounts& Accounts = GetAccounts();
//    if ( Accounts.find((LPCTSTR)sTmp) == Accounts.end() )
//	{
//		//CTracer::TraceMessage(CTracer::enMtWarning, NULL, _T("Unknown Account %s"), pTrade->account.c_str() );
//		return -1;
//	}
    
    // put_call
    CHECK_FIELD_EXISTENCE(account, dwLen, lpszTrade)
    if ( !IsValidPutCall(pIL->put_call) )
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'put_call' is not valid") );
        return 0;
    }
    pTrade->put_call = pIL->put_call;

    // symbol
    CHECK_FIELD_EXISTENCE(symbol, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(symbol, lpszTrade);
    sTmp.TrimRight(_T(' '));
    if (sTmp == _T(""))
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'symbol' is not valid") );
        return 0;
    }
    pTrade->base_sym = sTmp;

    // expire year
    CHECK_FIELD_EXISTENCE(expire_year, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(expire_year, lpszTrade);
    sTmp.TrimLeft(_T(' '));
    if (!sTmp.IsEmpty())
        lYear = _ttol(sTmp);

    // expire_month
    CHECK_FIELD_EXISTENCE(expire_month, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(expire_month, lpszTrade);
    sTmp.TrimLeft(_T(' '));
    if (!sTmp.IsEmpty())
        lMonth = _ttol(sTmp);

	// future contract maturity year
	CHECK_FIELD_EXISTENCE(contract_year, dwLen, lpszTrade)
		sTmp = GET_FIELD_VALUE(contract_year, lpszTrade);
	sTmp.TrimLeft(_T(' '));
	if (!sTmp.IsEmpty())
		lmYear = _ttol(sTmp);

	// future contract maturity month
	CHECK_FIELD_EXISTENCE(contract_month, dwLen, lpszTrade)
		sTmp = GET_FIELD_VALUE(contract_month, lpszTrade);
	sTmp.TrimLeft(_T(' '));
	if (!sTmp.IsEmpty())
		lmMonth = _ttol(sTmp);


    // strike dollar
    CHECK_FIELD_EXISTENCE(strike_dollar, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(strike_dollar, lpszTrade);
    sTmp.TrimLeft(_T(' '));

    // strike frac
    CHECK_FIELD_EXISTENCE(strike_frac, dwLen, lpszTrade)
    sTmp2 = GET_FIELD_VALUE(strike_frac, lpszTrade);
    sTmp2.TrimLeft(_T(' '));

    if ( (pTrade->put_call == PUT_FLAG || pTrade->put_call == CALL_FLAG) &&
        !sTmp.IsEmpty() && !sTmp2.IsEmpty() )
    {
        long dollar = _ttol(sTmp);
        double frac = double(_ttol(sTmp2))/8;
        pTrade->strike = dollar + frac;
    }

    // long/short
    CHECK_FIELD_EXISTENCE(long_short, dwLen, lpszTrade)
    if ( !IsValidBuySell(pIL->long_short) )
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'long_short' is not valid") );
        return 0;
    }
    pTrade->buy_sell = pIL->long_short;

    // security_type
    CHECK_FIELD_EXISTENCE(security_type, dwLen, lpszTrade)
    if ( !IsValidSecurityType(pIL->security_type) )
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'security_type' is not valid "));
        return 0;
    }

    if (pIL->security_type == OPTION_FLAG && 
        !(pTrade->put_call == PUT_FLAG || pTrade->put_call == CALL_FLAG) )
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Fields 'security_type' or 'put_call' are not valid") );
        return 0;
    }
    pTrade->security_type = pIL->security_type;

    // trade_price
    CHECK_FIELD_EXISTENCE(trade_price, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(trade_price, lpszTrade);
	if ( (dblTmp = boost::lexical_cast<double>(sTmp)/1000000) < 0. )
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'trade_price' is not valid"));
        return 0;
    }
    pTrade->price = dblTmp;

    // net position
    CHECK_FIELD_EXISTENCE(net_position, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(net_position, lpszTrade);

	try
	{	
		_variant_t vtVar(sTmp);
		pTrade->quantity = (LONG)vtVar;
	}
	catch(_com_error&) 
	{
		CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'net_position' is not valid") );
        return 0;
	}
    
	// Ravi Jain 
	// "If there is a correction, they just send a new record with the same tag number as the original, 
	// and we should just replace the original
	// If there is a deletion they just send a new record with the same tag number but with 0 amount, 
	// so it basically replaces the original with a 0 amount, thus effectively a deletion."
	if (0 == pTrade->quantity)
	{
		pTrade->ins_upd_del_flag = DEL_FLAG;
	}
	else
	{
		pTrade->ins_upd_del_flag = NEW_FLAG;
	}
	
    // record type
    CHECK_FIELD_EXISTENCE(record_type, dwLen, lpszTrade)
    if (pIL->record_type != '1')
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'record_type' is not valid") );
        return 0;
    }
	
    // trade_symbol
    CHECK_FIELD_EXISTENCE(trade_symbol, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(trade_symbol, lpszTrade);
    sTmp.TrimRight(_T(' '));
    if (pIL->security_type == OPTION_FLAG || pIL->security_type == FUTURE_FLAG)
    {
        if (sTmp.IsEmpty())
        {
            CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'trade_symbol' is not valid") );
            return 0;
        }
    }
    else
    {
        sTmp = pTrade->base_sym.c_str();
    }
    pTrade->trading_sym = sTmp;

    // expire date
    CHECK_FIELD_EXISTENCE(expiration_day, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(expiration_day, lpszTrade);
    sTmp.TrimLeft(_T(' '));
    if (!sTmp.IsEmpty())
        lDay = _ttol(sTmp);

    if (pIL->security_type == OPTION_FLAG)
    {
        SYSTEMTIME  systime = {(WORD)lYear, (WORD)lMonth, 0, (WORD)lDay, 0, 0, 0, 0};
        if ( FAILED(SystemTimeToVariantTime(&systime, &pTrade->expiration_date)) )
        {
            CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Fields 'expire year' or 'expire month' or 'expire day' are not valid"));
            return 0;
        }
    }
	else if(pIL->security_type == FUTURE_FLAG)
	{
		SYSTEMTIME  systime = {(WORD)lmYear, (WORD)lmMonth, 0, (WORD)lmDay, 0, 0, 0, 0};
		if ( FAILED(SystemTimeToVariantTime(&systime, &pTrade->maturity_date)) )
		{
			CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Fields 'expire year' or 'expire month' or 'expire day' are not valid"));
			return 0;
		}
	}

    // decimal_strike
    CHECK_FIELD_EXISTENCE(decimal_strike, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(decimal_strike, lpszTrade);
    if (pIL->security_type == OPTION_FLAG && pTrade->strike == 0)
    {
        double strike = double(_ttol(sTmp))/10000;
        if (strike == 0)
        {
            CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'decimal_strike' is not valid"));
            return 0;
        }
        pTrade->strike = strike;
    }

    // SPC
    CHECK_FIELD_EXISTENCE(SPC_Shares, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(SPC_Shares, lpszTrade);
    sTmp.TrimLeft(_T(' '));

    // execution date
	SYSTEMTIME stNow = {0};
	::GetSystemTime(&stNow);
	
    pTrade->execution_date = (DATE)vt_date(stNow);

    // tag_number
    CHECK_FIELD_EXISTENCE(tag_number, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(tag_number, lpszTrade);
    sTmp.TrimLeft(_T(' '));
    sTmp.TrimRight(_T(' '));
    if (sTmp.IsEmpty())
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'tag number' is not valid"));
        return 0;
    }
	if(IS_FIELD_EXIST(filler4, dwLen))
	{
		CString sTmpEx(GET_FIELD_VALUE(filler4, lpszTrade));
		sTmpEx.TrimLeft(_T(' '));
		sTmpEx.TrimRight(_T(' '));
		if (!sTmpEx.IsEmpty())
			sTmp+=sTmpEx;
	}

	pTrade->trade_id = sTmp;
	
	CHECK_FIELD_EXISTENCE(decimal_strike, dwLen, lpszTrade)
		sTmp = GET_FIELD_VALUE(decimal_strike, lpszTrade);
	if (pIL->security_type == OPTION_FLAG && pTrade->strike == 0)
	{
		double strike = double(_ttol(sTmp))/10000;
		if (strike == 0)
		{
			CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'decimal_strike' is not valid"));
			return 0;
		}
		pTrade->strike = strike;
	}
	
	size_t offset = offsetof(INPUTLINE, strategy);

	if(offset < dwLen ) // Strategy
		pTrade->strategy = CString(&lpszTrade[offsetof(INPUTLINE, strategy)], dwLen - offset );

	
    return (int)OnTrade(pTrade);
}

//--------------------------------------------------------------------------------------------------------------//
BOOL CFileConnector::DownloadTradesFile()
{
	DWORD dwFoundHour = static_cast<DWORD>(m_dwLastTradeTime/100);
	dwFoundHour = dwFoundHour>23?0:dwFoundHour;
	
	DWORD dwFoundMinute = m_dwLastTradeTime - (dwFoundHour*100);
	dwFoundMinute = dwFoundMinute>59?0:dwFoundMinute;


	vt_date vtLastDate((DATE)m_dwLastTradeDate);
	vtLastDate += vt_date_span(0, dwFoundHour, dwFoundMinute);
	if(vtLastDate <1)
	{
		vtLastDate = vt_date::GetCurrentDate(true);
		vtLastDate -= vt_date_span(5L,0L);
	}

  	_bstr_t bsDownloadedFile = DownloadFile(m_bsTradesFile, vtLastDate);

	if(bsDownloadedFile.length())
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL,_T("Processing Trases %s file"),(LPCSTR)m_bsTradesFile);
		CStdioFile cFile(bsDownloadedFile, CFile::modeRead|CFile::shareDenyNone);

		DWORD	dwTrades = 0L;
		DWORD	dwAllTrades = 0L;
		DWORD	dwNoAccount = 0L;
		//LPTSTR	lpRead = NULL;
		//vector<TCHAR>   vecRead(2*4096, 0);

		//ResetEvent(CPublisher::m_hFinishHandle);
		//while( (lpRead = cFile.ReadString((LPTSTR)&(*vecRead.begin()), vecRead.size() - 1))!=0 )
		while(true)
		{
			CString strRead;
			if(!cFile.ReadString(strRead))
				break;

			if( CheckFileFormat(strRead) == OCONNOR )
			{
				dwAllTrades++;
				if (dwAllTrades > m_dwLastTradeLine)
				{
					int nParseRes = ParseTradeLine(strRead);
					if (nParseRes == -1)
						dwNoAccount++;

					if (strRead.GetLength() > 1 && nParseRes > 0 )
						dwTrades++;
				}
			}
			else if( CheckFileFormat(strRead) == EZECASTLE )  //EZECASTLE
			{
				if (!m_bAlreadyDoneToday)   //is this file already loaded ?
				{
					int iLineType = GetLineTypeEze(strRead);
					if( iLineType == EZE_MAIN )  //is this main trade line
					{
						dwAllTrades++;

						int nParseRes = ParseTradeLineEzeCastle(strRead);	//parse it store to DB and publish
						if (nParseRes == -1)
							dwNoAccount++;

						if (strRead.GetLength() > 1 && nParseRes > 0 )
							dwTrades++;
					}
					else if (iLineType == EZE_LINE_ERROR)
					{
						CTracer::TraceMessage(CTracer::enMtError, (LPCSTR)strRead.GetString(), _T("Wrong count of separators."));	
					}
				}
			}
			//if(WaitForSingleObject(GetStopEventHandle(),0)!=WAIT_TIMEOUT)
			//break;
		}
		m_dwLastTradeLine = dwAllTrades;


		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Read %d of %d trades."), dwTrades, dwAllTrades);
		if (!dwTrades && dwAllTrades && dwNoAccount == dwAllTrades)
			CTracer::TraceMessage(CTracer::enMtWarning, NULL, _T("No new trades for current traders. Please, check traders in database."));

		if(dwNoAccount)
			CTracer::TraceMessage(CTracer::enMtWarning, NULL, _T("File contains %d records for unknown account."),dwNoAccount);
		

	}
	if(CTracer::GetWindowHandle())
		::SetWindowText(CTracer::GetWindowHandle(), "File Format - Real-Time Trade Feed");

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------//
_bstr_t CFileConnector::DownloadFile(_bstr_t fileName, DATE vtLastDate)
{
	_bstr_t bsFileLoaded = L"";
	HRESULT hr = S_OK;
	_bstr_t	bsURL(L"ftp://");
	bsURL += m_bsServerAddress + L"/" + fileName;

	try
	{
		IXceedFtpPtr piXceedFtp( __uuidof(XceedFtp));
		piXceedFtp->License( _bstr_t(L"FTP11-CU1NZ-BZ1YH-BKCA") );
		
		piXceedFtp->ServerAddress = m_bsServerAddress;  // Host address
		if(m_bsUser.length())
		{
			piXceedFtp->UserName = m_bsUser;           
			piXceedFtp->Password = m_bsPassword;       
		}

		piXceedFtp->PassiveMode = false;
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Opening URL %s"),(LPCSTR)m_bsServerAddress);
		piXceedFtp->Connect();

		TCHAR pathBuffer[ MAX_PATH *2 ];
		SHGetSpecialFolderPath(::AfxGetMainWnd()->GetSafeHwnd(), pathBuffer	, CSIDL_PROFILE, false);
		_bstr_t bsPathBuffer = pathBuffer;
		bsPathBuffer +=L"\\Local Settings\\Temp\\";

		m_bAlreadyDoneToday = false;

		CSimpleXceedFtpEventsImpl xEventHandler; 
		xEventHandler.m_pFileConnector = this;
		long lCookie = 0; 

		IXceedFtpAdviseEventsPtr  piAdvise( piXceedFtp );
		piAdvise->Advise( dynamic_cast< IXceedFtpEvents* >( &xEventHandler ), &lCookie );
		bool bCheckFileDate = (vtLastDate > 1.0);

		try
		{
			piXceedFtp->ListParsingFlags = flpAutomaticParsing ;
			m_bFileFound = false;
			piXceedFtp->ListFolderContents(fileName);

			bool bIsOldFile = false;
			if(m_dtFoundFileDate < vt_date::GetCurrentDate(true))
				bIsOldFile = true;

			if(m_bFileFound && (!bCheckFileDate || m_dtFoundFileDate > vt_date(vtLastDate)) && !bIsOldFile)
			{
				if(bCheckFileDate)
				{
					if(m_dtFoundFileDate.get_day() != vt_date(vtLastDate).get_day() || m_dtFoundFileDate.get_month() != vt_date(vtLastDate).get_month())
						m_dwLastTradeLine = 0;

					m_dwLastTradeDate = static_cast<DWORD>((DATE)m_dtFoundFileDate);
					m_dwLastTradeTime = m_dtFoundFileDate.get_hour()*100+m_dtFoundFileDate.get_minute(); 
				}

				hr = piXceedFtp->ReceiveMultipleFiles(fileName, bsPathBuffer, VARIANT_FALSE);
				bsFileLoaded = bsPathBuffer + fileName;
			}
			else
				if(!m_bFileFound)
					CTracer::TraceMessage(CTracer::enMtWarning, NULL,_T("Can not find URL file %s. No such file exists"),(LPCSTR)bsURL);	
				else
				{
					if(bIsOldFile)
						CTracer::TraceMessage(CTracer::enMtInformation, NULL,_T("Ignored old file %s"),(LPCSTR)bsURL);
					else
					{
						CTracer::TraceMessage(CTracer::enMtInformation, NULL,_T("The file %s allready been downloaded"),(LPCSTR)bsURL);

						m_bAlreadyDoneToday = true;

						if(bCheckFileDate)
							bsFileLoaded = bsPathBuffer + fileName;
					}
				}

			piAdvise->Unadvise(lCookie);
		}
		catch (_com_error &e)
		{
			piAdvise->Unadvise(lCookie);
//			DeleteFile(_bstr_t(bsPathBuffer + m_bsTradesFile));

			TCHAR   lpszErrorMsg[512] = {0};						
			_sntprintf(lpszErrorMsg, 512, _T("Can not open URL %s. Error: %s %d"),(LPCSTR)bsURL, (LPCSTR)piXceedFtp->GetResultDescription((EXFError)e.Error()), e.Error());
			throw std::exception(lpszErrorMsg);
		}
	}
	catch (_com_error &e)
	{
		TCHAR   lpszErrorMsg[512] = {0};						
		_sntprintf(lpszErrorMsg, 512, _T("Can not open FTP connection URL %s. Error: %s %d"), (LPCSTR)bsURL,e.Description().length()?(LPCSTR)e.Description():e.ErrorMessage(), e.Error());
		throw std::exception(lpszErrorMsg);
	}
	if(bsFileLoaded.length() && m_bUseDecryption)
	{
		// Decrypt Source
		bsFileLoaded = DoDecode(bsFileLoaded);

//		for(int i = 0; i<10000; ++i)
//		{
//			try{
//				_bstr_t bsFileLoaded1 = DoDecode(bsFileLoaded);
//			}   catch(...){
//			}
//		}
	}
	return bsFileLoaded;

}

DWORD CFileConnector::DeleteDestinationFile(_bstr_t strAscFilePath)
{
	DWORD dwDeleteFileREs = ERROR_SUCCESS;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(strAscFilePath, &fd);
	if(INVALID_HANDLE_VALUE != hFind && !::DeleteFile(strAscFilePath))
		dwDeleteFileREs = E_FAIL;

	return dwDeleteFileREs;
}

HANDLE CFileConnector::GeneratePasswordFile()
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	SECURITY_ATTRIBUTES saAttrFile;
	ZeroMemory( &saAttrFile, sizeof(saAttrFile) );
	DWORD dw = 0;

	saAttrFile.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttrFile.bInheritHandle = TRUE;
	saAttrFile.lpSecurityDescriptor = NULL;
	std::string sFilePath = "ocpas.dat";
	hFile =  ::CreateFile(
		sFilePath.c_str(), 
		GENERIC_WRITE|GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, 
		&saAttrFile, 
		CREATE_ALWAYS, 
		/*FILE_ATTRIBUTE_NORMAL,//*/FILE_ATTRIBUTE_NOT_CONTENT_INDEXED|FILE_FLAG_DELETE_ON_CLOSE|FILE_ATTRIBUTE_TEMPORARY, 
		NULL);

	if(INVALID_HANDLE_VALUE != hFile)
	{
		DWORD dwWritten = 0;
		::WriteFile(hFile, (LPCSTR)m_bsPassPhrase, m_bsPassPhrase.length(), &dwWritten, NULL);

		DWORD dw = SetFilePointer(hFile,0, NULL,FILE_BEGIN);
		char bf[2000];
		::ReadFile(hFile, bf, 2000, &dw, NULL);
		dw = SetFilePointer(hFile,0, NULL,FILE_BEGIN);
	}
	else
	{
		dw = GetLastError()	;
	}
	return hFile;
}

_bstr_t CFileConnector::DoDecode(_bstr_t bsSource)
{
	DWORD dwResult = ERROR_SUCCESS;
	_bstr_t bsDestination =  bsSource+ L".decoded";
	_bstr_t bsRetFile;

	DeleteDestinationFile(bsDestination);

	CHandle hFile(GeneratePasswordFile());

	if(INVALID_HANDLE_VALUE != hFile)
	{
		CString strBuffer;
		strBuffer.Format("--yes  --output \"%s\" --passphrase-fd %d --decrypt \"%s\"",(LPCSTR)bsDestination, long((HANDLE)hFile), (LPCSTR)bsSource);
		TCHAR* pBuffer = strBuffer.GetBuffer();

		SECURITY_ATTRIBUTES saAttr;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );
		ZeroMemory( &saAttr, sizeof(saAttr) );

		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;
		CString strGnuPath;
		strGnuPath.Format("%s\\gpg.exe", (LPCSTR)m_strGnuPgPath);
		// Start the child process. 
		if( CreateProcess( strGnuPath, // No module name (use command line). 
			pBuffer, // Command line. 
			&saAttr,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			TRUE,             // Set handle inheritance to TRUE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi )             // Pointer to PROCESS_INFORMATION structure.
			) 
		{

			// Wait until child process exits.
			if(WAIT_TIMEOUT == WaitForSingleObject( pi.hProcess, 300000 ))
			{
				::TerminateProcess(pi.hProcess,1);
				throw std::exception("File Encoding timeout is expired");
				dwResult = E_FAIL;
			}
			else
			{
				DWORD dwReturnStatus = 0;
				if(::GetExitCodeProcess(pi.hProcess, &dwReturnStatus) && 0 == dwReturnStatus)
				{
					//Encoding Succeeded
					dwResult = ERROR_SUCCESS;
					bsRetFile = bsDestination;
				}
				else
				{
					//Encoding is failed
					CString strResult;
					strResult.Format("The decoding procedure exit with error status %d ", dwReturnStatus);
					throw std::exception(strResult.GetString());
				}
			}

			// Close process and thread handles. 

			::CloseHandle( pi.hProcess );
			::CloseHandle( pi.hThread );
		}
		else
		{
			CString strResult;
			strResult.Format("Unable to create encoding process: %s", strGnuPath.GetString());
			throw std::exception(strResult.GetString());
		}

		hFile.Close();
		strBuffer.ReleaseBuffer();
	}
	else
	{
		CString strResult;
		dwResult = GetLastError();
		strResult.Format("Unable to create pass file: %d", dwResult);
		throw std::exception(strResult.GetString());

	}
	return bsRetFile;
}

BOOL CFileConnector::DownloadPositionsFile()
{

	USES_CONVERSION;
	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	if ((DATE)dtNow < m_dtPositionsDownload) 
		return TRUE;


	_bstr_t bsDownloadedPosition = DownloadFile(m_bsPositionsFile);
	if(bsDownloadedPosition.length())
	{
		CTracer::TraceMessage(CTracer::enMtInformation, NULL,_T("Processing Positions %s file"),(LPCSTR)m_bsPositionsFile);
		CStdioFile cFile(bsDownloadedPosition, CFile::modeRead|CFile::shareDenyNone);

		DWORD	dwTrades = 0L;
		DWORD	dwAllTrades = 0L;
		DWORD	dwNoAccount = 0L;
		LPTSTR	lpRead = NULL;
		vector<TCHAR>   vecRead(4096, 0);

		CString strLogFileDate = dtNow.Format(_T("%d_%m_%y"));

		COleDateTime dtDayBegin((DATE)(LONG)(dtNow.m_dt));
		COleDateTimeSpan dtSpan = dtNow - dtDayBegin;

		CString strLogFile;
		strLogFile.Format( _T("Logs/OPositions_%s_%lu.dat"), 
			strLogFileDate.GetBuffer(strLogFileDate.GetLength()), 
			(LONG)dtSpan.GetTotalSeconds() );

		fstream fPositions( strLogFile.GetBuffer(strLogFile.GetLength()), ios::app | ios::out);

		long nPositions = 0;
		long nAllPositions = 0;		

		CString strHeader = dtNow.Format(_T("BSC219  MICROHEDGE POS      %m/%d/%y"));
		fPositions << strHeader.GetBuffer(strHeader.GetLength());
		fPositions << endl;

		//LPTSTR lpszRead = NULL;

		while( true )
		{
			CString strTrade;
			if(!cFile.ReadString(strTrade))
				break;

			if ( WAIT_OBJECT_0 == WaitForSingleObject(GetStopEventHandle(), 0) )
			{
				CTracer::TraceMessage(CTracer::enMtWarning, NULL, "Breaks downloading a positions file.");
				break;
			}

			if (strTrade.GetLength() > 1)
			{
				if (ParsePositionLine(strTrade))
				{
					fPositions << strTrade;
					nPositions++;
				}
				nAllPositions++;
			}
		}
		fPositions.flush();
		fPositions.close();
		cFile.Close();
		CTracer::TraceMessage(CTracer::enMtInformation, NULL, _T("Saved %d of %d open positions."), nPositions, nAllPositions);
		m_dtPositionsDownload += 1; // tomorrow

		DeleteFile(bsDownloadedPosition);
	}
	return TRUE;

}

//--------------------------------------------------------------------------------------------------------------//
BOOL CFileConnector::ParsePositionLine(CString& strTrade)
{
    CString     sTmp;
    
    USES_CONVERSION;
    LPCSTR  lpszTrade = strTrade.GetString();
    DWORD   dwLen     = strTrade.GetLength();

    const INPUTLINE*  pIL = reinterpret_cast<const INPUTLINE*>(lpszTrade);

    // trans_id
    CHECK_FIELD_EXISTENCE(trans_id, dwLen, lpszTrade)
    if ( GET_FIELD_VALUE(trans_id, lpszTrade) != _T("346") )
    {
        CTracer::TraceMessage(CTracer::enMtError, lpszTrade, _T("Field 'trans_id' not equal '346'") );
        return FALSE;
    }

    // account
    CHECK_FIELD_EXISTENCE(account, dwLen, lpszTrade)
    sTmp = GET_FIELD_VALUE(account, lpszTrade);
    sTmp.TrimRight(_T(' '));

    // check account
    const CPublisher::TAccounts  Accounts = GetAccounts();
    if (Accounts.find((LPCTSTR)sTmp) == Accounts.end())
        return FALSE;
    
    return TRUE;
}
