#ifndef __IVDATALOADER_H__
#define __IVDATALOADER_H__

#include "EodGeneral.h"

namespace EODIV
{

/////////////////////////////////////////////////////////////////////////////
//
const long nMaxStocksInRequest = 100L;

/////////////////////////////////////////////////////////////////////////////
//
template <class _RequestData, class _ResultData>
class CIvDataLoader	: protected CEgIvProvider, virtual public EODCO::CEgTraceable, 	virtual public EODCO::CEgReportable
{
public:
	CIvDataLoader(void) : m_strSID(_T("")), m_bIsCancel(false), m_nWaitInterval(0L), 
		m_nReconnectAttempt(0L) {}
	virtual ~CIvDataLoader(void){ _Disconnect(); }

	void RequestIvData(_RequestData* pRequest) throw (exception);
	void CancelRequest() throw (exception);

	virtual void OnIvData(_ResultData pResult) = 0;

protected:
	virtual const HANDLE _StopEventHandle() = 0;
	virtual EodStepsMaskEnum _StepMask() = 0;
	bool	_IsCancel();
private:
	void _Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr) throw (CEgHttpError, exception);
	void _Disconnect() throw ();
	void _ReadAllData(CEgString& rstrData);

	void _CheckResultFormat(CEgString& strData) throw(exception);
	CString _GetSidStr() throw();
	void	_SetCancel();
private:
	CString			m_strSID;
	bool			m_bIsCancel;
	long			m_nReconnectAttempt;
	long			m_nWaitInterval;

	CLock	m_Lock;
};

/////////////////////////////////////////////////////////////////////////////
//
class CIVRequestBase : virtual public EODCO::CEgTraceable
{
public:
	CIVRequestBase() :
	  m_nUndSend(0L){}
	virtual ~CIVRequestBase() {}

	virtual CString GetURL() = 0;
	virtual void Init(EODSM::CSmSettingsMgrPtr	spSettingsMgr, EODMS::CMsUndCollPtr pcollUnd, EODMS::CMsIndexCollPtr pcollIndex = EODMS::CMsIndexCollPtr());

	EODMS::CMsUndCollPtr Und()
	{
		return m_collUnd;
	}

	EODMS::CMsIndexCollPtr Indexes()
	{
		return m_collIndex;
	}

	long UndSend()
	{
		return m_nUndSend;
	}
	EODSM::CSmSettingsMgrPtr SettingsMgr()
	{
		return m_spSettingsMgr;
	}
private:
	CString	_GetUndSymbol(EODMS::CMsUndPtr pUnd);
protected:
	CString GetContracts(long nCount = nMaxStocksInRequest);

protected:
	EODMS::CMsUndCollPtr		m_collUnd;
	EODMS::CMsIndexCollPtr		m_collIndex;

	EODMS::CMsUndColl::iterator m_iterNextUnd;
	long						m_nUndSend;

	EODSM::CSmSettingsMgrPtr	m_spSettingsMgr;
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvSkewKurtosisRequest : public CIVRequestBase
{
public:
	CIvSkewKurtosisRequest() {}

	virtual CString GetURL();
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvUndIndexBetaRequest : public CIVRequestBase
{
public:
	CIvUndIndexBetaRequest() {}

	virtual CString GetURL();
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvAvrgUndVolumesRequest : public CIVRequestBase
{
public:
	CIvAvrgUndVolumesRequest() {}

	virtual CString GetURL();
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvIvBandsRequest : public CIVRequestBase
{
public:
	CIvIvBandsRequest() {}

	virtual CString GetURL();
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvDefaultIvRequest : public CIVRequestBase
{
public:
	CIvDefaultIvRequest() {}

	virtual CString GetURL();
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvMarketStructureUpdatesRequest : public CIVRequestBase
{
public:
	CIvMarketStructureUpdatesRequest() : m_dtLastUpdate(DATE(BAD_DOUBLE_VALUE)) {}

	void Init(EODSM::CSmSettingsMgrPtr	pSettingsMgr, EODMS::CMsUndCollPtr pcollUnd, DATE dtLastUpdate);

	virtual CString GetURL();
private:
	DATE m_dtLastUpdate;
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvUndClosePriceRequest : public CIVRequestBase
{
public:
	CIvUndClosePriceRequest() {}

	virtual CString GetURL();
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvOptClosePriceRequest : public CIVRequestBase
{
public:
	CIvOptClosePriceRequest() {}

	virtual CString GetURL();
private:
	CString _GetOptionsRangesString(EODMS::CMsUndPtr pUnd);
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvResultBase :  virtual public EODCO::CEgTraceable
{
protected:
	CIvResultBase(){};
public:
	virtual ~CIvResultBase(){};
	
	/////////////////////////////////////////////////////////////////////////////
	// custom Tokenize.
	static CEgString Tokenize(CEgString strSource, LPCTSTR pszTokens, long& iStart)
	{
		ATLASSERT (iStart >= 0L);

		if (pszTokens == NULL)
		{
			return strSource;
		}

		LPCTSTR pszPlace = strSource.GetString() + iStart;
		LPCTSTR pszEnd = strSource.GetString() + strSource.GetLength();

		if (pszPlace < pszEnd)
		{
			long nExcluding = (long)_tcscspn(pszPlace, pszTokens);

			long iFrom = iStart;
			long nUntil = nExcluding;
			iStart = iFrom + nUntil + 1L;

			return (strSource.Mid(iFrom, nUntil));
		}

		// return empty CString, done tokenizing
		iStart = -1L;

		return CEgString();
	}
};

/////////////////////////////////////////////////////////////////////////////
//
class CIvSkewKurtosisResult;
typedef boost::shared_ptr<CIvSkewKurtosisResult> CIvSkewKurtosisResultPtr;

class CIvSkewKurtosisResult : public CIvResultBase
{
public:
	CIvSkewKurtosisResult() : m_strUndSymbol(_T("")), m_dSkew(BAD_DOUBLE_VALUE), m_dKurtosis(BAD_DOUBLE_VALUE) {};
	virtual ~CIvSkewKurtosisResult(){};

	static CIvSkewKurtosisResultPtr Parse(const CEgString& strResult, long& nPos) throw (exception);
public:
	CString m_strUndSymbol;
	double m_dSkew;
	double m_dKurtosis;
};



/////////////////////////////////////////////////////////////////////////////
//
class CIvUndIndexBetaResult;
typedef boost::shared_ptr<CIvUndIndexBetaResult> CIvUndIndexBetaResultPtr;

class CIvUndIndexBetaResult : public CIvResultBase
{
public:
	CIvUndIndexBetaResult() : m_strUndSymbol(_T("")), m_strIndexSymbol(_T("")), m_dBeta(BAD_DOUBLE_VALUE) {};
	virtual ~CIvUndIndexBetaResult(){};

	static CIvUndIndexBetaResultPtr Parse(const CEgString& strResult, long& nPos) throw (exception);
public:
	CString m_strUndSymbol;
	CString m_strIndexSymbol;
	double m_dBeta;
};


/////////////////////////////////////////////////////////////////////////////
//
class CIvAvrgUndVolumesResult;
typedef boost::shared_ptr<CIvAvrgUndVolumesResult> CIvAvrgUndVolumesResultPtr;

class CIvAvrgUndVolumesResult : public CIvResultBase
{
public:
	CIvAvrgUndVolumesResult() : m_strUndSymbol(_T("")), m_nVolume10Day(BAD_LONG_VALUE), m_nVolume5Expiration(BAD_LONG_VALUE) {};
	virtual ~CIvAvrgUndVolumesResult(){};

	static CIvAvrgUndVolumesResultPtr Parse(const CEgString& strResult, long& nPos) throw (exception);
public:
	CString	m_strUndSymbol;
	long	m_nVolume10Day;
	long	m_nVolume5Expiration;
};


/////////////////////////////////////////////////////////////////////////////
//
class CIvIvBandsResult;
typedef boost::shared_ptr<CIvIvBandsResult> CIvIvBandsResultPtr;

class CIvIvBandsResult : public CIvResultBase
{
public:
	CIvIvBandsResult() : m_strUndSymbol(_T("")), m_nStrikeMoneyness(BAD_LONG_VALUE), m_nExpPeriod(BAD_LONG_VALUE),
						m_dIVBandLow(BAD_DOUBLE_VALUE), m_dIVBandHigh(BAD_DOUBLE_VALUE) {};
	virtual ~CIvIvBandsResult(){};

	static CIvIvBandsResultPtr Parse(const CEgString& strResult, long& nPos) throw (exception);
public:
	CString	m_strUndSymbol;
	long	m_nStrikeMoneyness;
	long	m_nExpPeriod;
	double	m_dIVBandLow;
	double	m_dIVBandHigh;
};


/////////////////////////////////////////////////////////////////////////////
//
class CIvDefaultIvResult;
typedef boost::shared_ptr<CIvDefaultIvResult> CIvDefaultIvResultPtr;
class CIvDefaultIvResult : public CIvResultBase
{
public:
	CIvDefaultIvResult() : m_strUndSymbol(_T("")), m_dStrike(BAD_DOUBLE_VALUE), m_dtExpiration(DATE(BAD_DOUBLE_VALUE)),m_dVolatility(BAD_DOUBLE_VALUE) {};
	virtual ~CIvDefaultIvResult(){};

	static CIvDefaultIvResultPtr Parse(const CEgString& strResult, long& nPos) throw (exception);
public:
	CString	m_strUndSymbol;
	double	m_dStrike;
	DATE	m_dtExpiration;
	double	m_dVolatility;
};


/////////////////////////////////////////////////////////////////////////////
//
class CIvMarketStructureUpdatesResult;
typedef boost::shared_ptr<CIvMarketStructureUpdatesResult> CIvMarketStructureUpdatesResultPtr;

class CIvMarketStructureUpdatesResult : public CIvResultBase
{
public:
	CIvMarketStructureUpdatesResult() : m_strUndSymbol(_T("")), m_dtLastUpdate(DATE(BAD_DOUBLE_VALUE)) {};
	virtual ~CIvMarketStructureUpdatesResult(){};

	static CIvMarketStructureUpdatesResultPtr Parse(const CEgString& strResult, long& nPos) throw (exception);
public:
	CString	m_strUndSymbol;
	DATE	m_dtLastUpdate;
};


/////////////////////////////////////////////////////////////////////////////
//
class CIvUndClosePriceResult;
typedef boost::shared_ptr<CIvUndClosePriceResult> CIvUndClosePriceResultPtr;

class CIvUndClosePriceResult : public CIvResultBase
{
public:
	CIvUndClosePriceResult() : m_strUndSymbol(_T("")), m_dClosePrice(BAD_DOUBLE_VALUE) {};
	virtual ~CIvUndClosePriceResult(){};

	static CIvUndClosePriceResultPtr Parse(const CEgString& strResult, long& nPos) throw (exception);
public:
	CString	m_strUndSymbol;
	double	m_dClosePrice;
};


/////////////////////////////////////////////////////////////////////////////
//
class CIvOptClosePriceResult;
typedef boost::shared_ptr<CIvOptClosePriceResult> CIvOptClosePriceResultPtr;
class CIvOptClosePriceResult : public CIvResultBase
{
public:
	CIvOptClosePriceResult() : m_strUndSymbol(_T("")), m_dClosePrice(BAD_DOUBLE_VALUE) {};
	virtual ~CIvOptClosePriceResult(){};

	static CIvOptClosePriceResultPtr Parse(const CEgString& strResult, long& nPos) throw (exception);
public:
	CString	m_strUndSymbol;
	EODMS::CMsOptionKey	m_aKey;
	double	m_dClosePrice;
};


};

#include "IvDataLoader.inl"

#endif// __IVDATALOADER_H__
