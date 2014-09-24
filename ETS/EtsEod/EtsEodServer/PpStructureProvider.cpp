#include "StdAfx.h"
#include "PpStructureProvider.h"

namespace EODPP
{
typedef TRecord<PP::OptionParams, &PP::LIBID_PP>		_OptionParams;
typedef TRecord<PP::StockParams, &PP::LIBID_PP>			_StockParams;
typedef TRecord<PP::FutureRootParams, &PP::LIBID_PP>	_FutureRootParams;
typedef TRecord<PP::FuturesOptionParams, &PP::LIBID_PP>	_FuturesOptionParams;

_ATL_FUNC_INFO CPpStructureProvider::m_fiOnStock = 
{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD, VT_RECORD} };

_ATL_FUNC_INFO CPpStructureProvider::m_fiOnOption = 
{CC_STDCALL, VT_EMPTY, 3, {VT_RECORD, VT_RECORD, VT_BOOL} };

_ATL_FUNC_INFO CPpStructureProvider::m_fiOnError = 
{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };

_ATL_FUNC_INFO CPpStructureProvider::m_fiOnFutureByRoot = 
{CC_STDCALL, VT_EMPTY, 3, {VT_RECORD, VT_RECORD, VT_BOOL} };

_ATL_FUNC_INFO CPpStructureProvider::m_fiOnFuturesOption = 
{CC_STDCALL, VT_EMPTY, 3, {VT_RECORD, VT_RECORD, VT_BOOL} };

/////////////////////////////////////////////////////////////////////////////
//
CPpStructureProvider::CPpStructureProvider(void)
	: m_enCurrentRequest(enSpRequestNone)
{
}

/////////////////////////////////////////////////////////////////////////////
//
CPpStructureProvider::~CPpStructureProvider(void)
{
	Disconnect();
}

/////////////////////////////////////////////////////////////////////////////
//
void CPpStructureProvider::Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	EODSM::CSmSettingsMgrPtr spSettingsMgr(pSettingsMgr);
	if(!pSettingsMgr)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Invalid settings manager."));
		throw exception(_T("Invalid settings manager."));
	}

	try
	{
		if(_GetStructureProvider())
			Disconnect();

		PP::IStructureProvider2Ptr	spStructureProvider;
		_com_util::CheckError(spStructureProvider.CreateInstance(PP::CLSID_StructureProviderEx));

		PP::IProviderPtr spProvider = spStructureProvider;
		spProvider->Type = spSettingsMgr->PriceProviderType();
		spStructureProvider->Connect();

		_com_util::CheckError(DispEventAdvise(spStructureProvider));
		_SetStructureProvider(spStructureProvider);
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception %s %s"), typeid(e).name(), e.what());
		Disconnect();
		throw e;
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
		Disconnect();
		CString strDescr;
		strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
		throw exception(strDescr);
	}
	catch(...)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Exception %s %s"));
		Disconnect();
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CPpStructureProvider::Disconnect()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	try
	{
		PP::IStructureProvider2Ptr	spStructureProvider = _GetStructureProvider();
		if(spStructureProvider)
		{
			DispEventUnadvise(spStructureProvider);
			
			Sleep(1000L);

			CancelRequests();

			spStructureProvider->Disconnect();

			_SetStructureProvider(NULL);
		}
	}
	catch(_com_error& e)
	{
		CString strDescr;
		strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
		Trace(EODCO::enTrLogError,__FUNCTION__, (LPCTSTR)strDescr);
	}
	catch(exception& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("%s, %s"), typeid(e).name(), e.what());
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CPpStructureProvider::RequestUnderlying(const CString& strUnderlyingSymbol)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	
	PP::IStructureProvider2Ptr	spStructureProvider = _GetStructureProvider();

	if(spStructureProvider == NULL)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Not connected to structure provider."));
		throw logic_error(_T("Not connected to structure provider."));
	}
	if(strUnderlyingSymbol.IsEmpty())
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Empty underlying name."));
		throw logic_error(_T("Empty underlying name."));
	}

	if(enSpRequestNone == m_enCurrentRequest)
	{
		try
		{
			m_enCurrentRequest = enSpRequestStock;

			struct PP::StockParams params;
			_bstr_t bsSymbol = strUnderlyingSymbol;
			params.Stock = bsSymbol;
			params.Exchange = NULL;

			_com_util::CheckError(spStructureProvider->raw_RequestStock(&params));

		}
		catch(_com_error& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
			m_enCurrentRequest = enSpRequestNone;

			spStructureProvider->raw_CancelStock();

			CString strDescr;
			strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
			throw exception(strDescr);
		}
	}
	else
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Structure provider already requests price."));
		throw logic_error(_T("Structure provider already requests price."));
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}


/////////////////////////////////////////////////////////////////////////////
//
void CPpStructureProvider::CancelRequests()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	PP::IStructureProvider2Ptr	spStructureProvider = _GetStructureProvider();
	if(spStructureProvider == NULL)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Not connected to structure provider."));
		throw logic_error(_T("Not connected to structure provider."));
	}

	if(enSpRequestStock == m_enCurrentRequest)
	{

		try
		{
			_com_util::CheckError(spStructureProvider->raw_CancelStock());
		}
		catch(_com_error& e)
		{
			CString strDescr;
			strDescr.Format(_T("CancelStock failed. Error: %08x %s"), e.Error(), (LPCTSTR)e.Description());
			throw exception(strDescr);
		}
	}
	else if(enSpRequestOptions == m_enCurrentRequest)
	{

		try
		{
			_com_util::CheckError(spStructureProvider->raw_CancelOptions());
		}
		catch(_com_error& e)
		{
			CString strDescr;
			strDescr.Format(_T("CancelOptions failed. Error: %08x %s"), e.Error(), (LPCTSTR)e.Description());
			throw exception(strDescr);
		}
	}
	else if(enSpRequestFutures == m_enCurrentRequest)
	{

		try
		{
			_com_util::CheckError(spStructureProvider->raw_CancelFutures());
		}
		catch(_com_error& e)
		{
			CString strDescr;
			strDescr.Format(_T("CancelFutures failed. Error: %08x %s"), e.Error(), (LPCTSTR)e.Description());
			throw exception(strDescr);
		}
	}
	else if(enSpRequestFuturesOptions == m_enCurrentRequest)
	{

		try
		{
			_com_util::CheckError(spStructureProvider->raw_CancelFuturesOptions());
		}
		catch(_com_error& e)
		{
			CString strDescr;
			strDescr.Format(_T("CancelFuturesOptions failed. Error: %08x %s"), e.Error(), (LPCTSTR)e.Description());
			throw exception(strDescr);
		}
	}

	m_enCurrentRequest = enSpRequestNone;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
STDMETHODIMP CPpStructureProvider::_OnStock(struct PP::StockParams* pReq,  struct PP::StockResultsEx* pResult)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(enSpRequestStock == m_enCurrentRequest)
	{
		m_enCurrentRequest = enSpRequestNone;
		OnStock(pReq, pResult);
	}
	else if(enSpRequestNone != m_enCurrentRequest)
		ATLASSERT(false);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return S_OK;
}

STDMETHODIMP CPpStructureProvider::_OnOption(struct PP::OptionParams* pReq,  struct PP::OptionResultsEx* pResult, VARIANT_BOOL IsLastOption)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(enSpRequestOptions == m_enCurrentRequest)
	{
		if(IsLastOption)
			m_enCurrentRequest = enSpRequestNone;

		OnOption(pReq, pResult,IsLastOption);
	}
	else if(enSpRequestNone != m_enCurrentRequest)
		ATLASSERT(false);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return S_OK;
}

STDMETHODIMP CPpStructureProvider::_OnError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
	PP::RequestsTypeEnum enReqType, VARIANT varRequest)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if(enErrorNumber == PP::enNotConnected || enErrorNumber == PP::enConnectionWasDisconnected)
	{
		OnDisconnect(enErrorNumber, bsDescription);
		return S_OK;
	}

	if(enReqType == PP::enRequestOptions)
	{
		_OptionParams Params;
		Params.Copy(varRequest);
			
		m_enCurrentRequest = enSpRequestNone;

		OnGetOptionError(enErrorNumber,bsDescription,&Params);
	}	
	else if(enReqType == PP::enRequestStock)
	{
		_StockParams Params;
		Params.Copy(varRequest);

		if(enErrorNumber == PP::enNoDataAvailableForSymbol)
		{
			struct PP::OptionParams OptParams;
			_bstr_t bsUndSymbol = _bstr_t(Params.Stock);
			OptParams.UnderlyingSymbol = bsUndSymbol;
			OptParams.Exchange = NULL;

			m_enCurrentRequest = enSpRequestNone;

			OnGetOptionError(enErrorNumber,bsDescription,&OptParams);
		}
		else 
		{
			m_enCurrentRequest = enSpRequestNone;

			OnGetStockError(enErrorNumber,bsDescription,&Params);
		}
	}
	else if(enReqType == PP::enRequestFuturesOptions)
	{
		_FuturesOptionParams Params;
		Params.Copy(varRequest);

		m_enCurrentRequest = enSpRequestNone;

		OnGetFuturesOptionError(enErrorNumber,bsDescription,&Params);
	}	
	else if(enReqType == PP::enRequestFutures)
	{
		_FutureRootParams Params;
		Params.Copy(varRequest);

		m_enCurrentRequest = enSpRequestNone;

		OnGetFutureByRootError(enErrorNumber,bsDescription,&Params);
	}
	else
	{
		OnError(enErrorNumber,bsDescription);
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return S_OK;
}

STDMETHODIMP CPpStructureProvider::_OnFutureByRoot(struct PP::FutureRootParams* pReq,  struct PP::FutureResultsEx* pResult, VARIANT_BOOL IsLastFuture)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(enSpRequestFutures == m_enCurrentRequest)
	{
		if(IsLastFuture)
			m_enCurrentRequest = enSpRequestNone;

		OnFutureByRoot(pReq, pResult,IsLastFuture);
	}
	else if(enSpRequestNone != m_enCurrentRequest)
		ATLASSERT(false);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return S_OK;
}

STDMETHODIMP CPpStructureProvider::_OnFuturesOption(struct PP::FuturesOptionParams* pReq,  struct PP::FuturesOptionResultsEx* pResult, VARIANT_BOOL IsLastOption)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(enSpRequestFuturesOptions == m_enCurrentRequest)
	{
		if(IsLastOption)
			m_enCurrentRequest = enSpRequestNone;

		OnFuturesOption(pReq, pResult,IsLastOption);
	}
	else if(enSpRequestNone != m_enCurrentRequest)
		ATLASSERT(false);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return S_OK;
}

void CPpStructureProvider::RequestOptions(const CString& strUnderlyingSymbol)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if(enSpRequestNone == m_enCurrentRequest)
	{
		PP::IStructureProvider2Ptr	spStructureProvider = _GetStructureProvider();
		if(spStructureProvider)
		{
			try
			{
				m_enCurrentRequest = enSpRequestOptions;
				struct PP::OptionParams Params;
				_bstr_t bsUndSymbol = strUnderlyingSymbol;
				Params.UnderlyingSymbol = bsUndSymbol;
				Params.Exchange = NULL;

				_com_util::CheckError(spStructureProvider->raw_RequestOptions(&Params));
			}
			catch(_com_error& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
				m_enCurrentRequest = enSpRequestNone;

				spStructureProvider->raw_CancelOptions();

				CString strDescr;
				strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
				throw exception(strDescr);
			}
		}
	}
	else
		ATLASSERT(false);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CPpStructureProvider::RequestFutures(const CString& strFutureRootSymbol)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if(enSpRequestNone == m_enCurrentRequest)
	{
		PP::IStructureProvider2Ptr	spStructureProvider = _GetStructureProvider();
		if(spStructureProvider)
		{
			try
			{
				m_enCurrentRequest = enSpRequestFutures;
				struct PP::FutureRootParams Params;
				_bstr_t bsRootSymbol = strFutureRootSymbol;
				Params.FuturesRootSymbol = bsRootSymbol;

				_com_util::CheckError(spStructureProvider->raw_RequestFutures(&Params));
			}
			catch(_com_error& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
				m_enCurrentRequest = enSpRequestNone;

				spStructureProvider->raw_CancelFutures();

				CString strDescr;
				strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
				throw exception(strDescr);
			}
		}
	}
	else
		ATLASSERT(false);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CPpStructureProvider::RequestFuturesOptions(const CString& strFutureSymbol)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	if(enSpRequestNone == m_enCurrentRequest)
	{
		PP::IStructureProvider2Ptr	spStructureProvider = _GetStructureProvider();
		if(spStructureProvider)
		{
			try
			{
				m_enCurrentRequest = enSpRequestFuturesOptions;
				struct PP::FuturesOptionParams Params;
				_bstr_t bsFutSymbol = strFutureSymbol;
				Params.FutureSymbol = bsFutSymbol;
				Params.Exchange = NULL;

				_com_util::CheckError(spStructureProvider->raw_RequestFuturesOptions(&Params));
			}
			catch(_com_error& e)
			{
				Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
				m_enCurrentRequest = enSpRequestNone;

				spStructureProvider->raw_CancelFuturesOptions();

				CString strDescr;
				strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
				throw exception(strDescr);
			}
		}
	}
	else
		ATLASSERT(false);

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

};