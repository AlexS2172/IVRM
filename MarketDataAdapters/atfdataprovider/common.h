#if !defined(AFX_COMMON_H_)
#define AFX_COMMON_H_
#include "ATFDataProvider.h"
#include <recPtr.h>

#define TIMEOUT_LOGON			60 //1 minute wating timeout

#ifdef _DEBUG
	#define TIMEOUT_TRACK		1*1000 
#else
	#define TIMEOUT_TRACK		2*1000 //2 seconds waiting while send next symbols
#endif

//----------------------------------------------------------------------------------------//
typedef CComRecPtr<QuoteUpdateInfo, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateInfo;
typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateParams;
typedef CComRecPtr<QuoteUpdateResult, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateResult;
typedef CComRecPtr<StockResults, &LIBID_PRICEPROVIDERSLib>		_StockResults;
typedef CComRecPtr<StockInfo, &LIBID_PRICEPROVIDERSLib>			_StockInfo;
typedef CComRecPtr<OptionResults, &LIBID_PRICEPROVIDERSLib>		_OptionResults;
typedef CComRecPtr<OptionInfo, &LIBID_PRICEPROVIDERSLib>		_OptionInfo;
typedef CComRecPtr<StockParams, &LIBID_PRICEPROVIDERSLib>		_StockParams;
typedef CComRecPtr<OptionParams, &LIBID_PRICEPROVIDERSLib>		_OptionParams;
typedef CComRecPtr<ATOption, &LIBID_ATFDATAPROVIDERLib>			_Options;
typedef CComRecPtr<ATStock, &LIBID_ATFDATAPROVIDERLib>			_Stocks;

//typedef CComRecPtr<DescriptionResults, &LIBID_MYTRACKPROVIDERSLib>	_DescriptionResults;

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