#if !defined(AFX_COMMON_H_)
#define AFX_COMMON_H_

#define DEF_PATH _T("SOFTWARE\\eGAR\\{279F8CEA-29DE-4b2a-B868-75BE88C7C41E}")
#define TIMEOUT_LOGON			60 //1 minute wating timeout

#ifdef _DEBUG
	#define TIMEOUT_TRACK		1*1000 
#else
	#define TIMEOUT_TRACK		2*1000 //2 seconds waiting while send next symbols
#endif

//----------------------------------------------------------------------------------------//
typedef CComRecPtr<QuoteUpdateParams, &LIBID_TRACKDATAPROVIDERSLib>	_QuoteUpdateParams;
typedef CComRecPtr<QuoteUpdateResult, &LIBID_TRACKDATAPROVIDERSLib>	_QuoteUpdateResult;
typedef CComRecPtr<StockResults, &LIBID_TRACKDATAPROVIDERSLib>		_StockResults;
typedef CComRecPtr<StockInfo, &LIBID_TRACKDATAPROVIDERSLib>			_StockInfo;
typedef CComRecPtr<OptionResults, &LIBID_TRACKDATAPROVIDERSLib>		_OptionResults;
typedef CComRecPtr<OptionInfo, &LIBID_TRACKDATAPROVIDERSLib>		_OptionInfo;
typedef CComRecPtr<StockParams, &LIBID_TRACKDATAPROVIDERSLib>		_StockParams;
typedef CComRecPtr<OptionParams, &LIBID_TRACKDATAPROVIDERSLib>		_OptionParams;
//typedef CComRecPtr<DescriptionResults, &LIBID_MYTRACKPROVIDERSLib>	_DescriptionResults;

class CLoginInfo
{
public:
	_bstr_t m_strUserName;
	_bstr_t m_strUserPass;
	_bstr_t	m_strAppName;
	int	m_iRevHi;
	int	m_iRevLow;
	int	m_iLogonTimeout;
	int	m_iRequestTimeout;
	
	CLoginInfo()
	{
		m_strUserName = "";
		m_strUserPass = "";
		m_strAppName = "";
		m_iRevHi = 1;
		m_iRevLow = 0;
		m_iLogonTimeout = 0;
		m_iRequestTimeout = 0;
	}

	bool Load()
	{
		eg_reg::CEgRegKey regUser;
		eg_reg::CEgRegKey regLocal;
		DWORD	  dwValue;

		regUser.Open(HKEY_CURRENT_USER,DEF_PATH,KEY_READ);
		regLocal.Open(HKEY_LOCAL_MACHINE,DEF_PATH,KEY_READ);
		if(regUser.m_hKey == NULL && regLocal.m_hKey == NULL)
			return false;

		if(!GetRegValue(_T("RevHi"),dwValue,regUser,regLocal))
			return false;
		
		m_iRevHi = dwValue;
		if(!GetRegValue(_T("RevLo"),dwValue,regUser,regLocal))
			return false;
		
		m_iRevLow = dwValue;
	
		if(GetRegValue(_T("LogonTimeout"),dwValue,regUser,regLocal))
			m_iLogonTimeout = dwValue;
		else
			m_iLogonTimeout = TIMEOUT_LOGON;
	
		if(GetRegValue(_T("RequestTimeout"),dwValue,regUser,regLocal))
			m_iRequestTimeout = dwValue;
		else
			m_iRequestTimeout = TIMEOUT_TRACK;
	
		if(!GetRegValue(_T("Account"),m_strUserName,regUser,regLocal))
			return false;

		if(!GetRegValue(_T("Password"),m_strUserPass,regUser,regLocal))
			return false;
		
		if(!GetRegValue(_T("ProductName"),m_strAppName,regUser,regLocal))
			return false;

	return true; 		
	}

private:
	bool GetRegValue(LPCTSTR szKeyName, _bstr_t& bsValue, eg_reg::CEgRegKey& key1, eg_reg::CEgRegKey& key2)
	{
		if(key1.m_hKey!=NULL)
			if(ERROR_SUCCESS == key1.QueryValue(bsValue, szKeyName))
				return true;

		if(key2.m_hKey!=NULL)
			if(ERROR_SUCCESS == key2.QueryValue(bsValue, szKeyName))
				return true;
		return false;

	}

	bool GetRegValue(LPCTSTR szKeyName, DWORD& dwValue, eg_reg::CEgRegKey& key1, eg_reg::CEgRegKey& key2)
	{
		if(key1.m_hKey!=NULL)
			if(ERROR_SUCCESS == key1.QueryValue(dwValue, szKeyName))
				return true;

		if(key2.m_hKey!=NULL)
			if(ERROR_SUCCESS == key2.QueryValue(dwValue, szKeyName))
				return true;
		return false;
	}
	
};

class CCommonException
{
public:

	static void ThrowError (HRESULT hRes, LPCOLESTR szError )
	{
		if (szError && wcslen (szError) > 0)
		{
			CComPtr <ICreateErrorInfo> spCreateErrorInfo;
			HRESULT hr = ::CreateErrorInfo (&spCreateErrorInfo);
			if (SUCCEEDED (hr))
			{
				spCreateErrorInfo->SetDescription (const_cast <LPOLESTR> (szError));
				CComQIPtr <IErrorInfo> spErrInfo = spCreateErrorInfo;
				ATLASSERT (spErrInfo);
				SetErrorInfo (0, spErrInfo);
				throw _com_error(hRes, spErrInfo);
			}
		}
		throw  _com_error(hRes);
	}
	
	static void ThrowError (HRESULT hRes, UINT nId )
	{
		CComBSTR spBuffer;
		spBuffer.LoadString (nId);
		ThrowError(hRes,(LPCOLESTR)spBuffer.m_str); 
	}
};


#endif