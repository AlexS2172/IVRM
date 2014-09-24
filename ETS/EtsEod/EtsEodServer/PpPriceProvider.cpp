#include "StdAfx.h"
#include "PpPriceProvider.h"

namespace EODPP
{

typedef TRecord<PP::QuoteUpdateParams, &PP::LIBID_PP>	_QuoteUpdateParams;

_ATL_FUNC_INFO CPpPriceProvider::m_fiOnLastQuoteInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD, VT_RECORD} };

_ATL_FUNC_INFO CPpPriceProvider::m_fiOnErrorInfo = 
	{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };

/////////////////////////////////////////////////////////////////////////////
//
CPpPriceProvider::CPpPriceProvider(void)
	: m_bLastQuoteReqNow(false),
	m_bIsGroupRequest(false),
	m_nLastQuoteReqCount(0L),
	m_nLastQuoteReqDone(0L),
	m_nLastQuoteGroupReqCount(0L),
	m_nLastQuoteGroupReqDone(0L)
{
}

/////////////////////////////////////////////////////////////////////////////
//
CPpPriceProvider::~CPpPriceProvider(void)
{	
	if(_GetPriceProvider())
		Disconnect();
}

/////////////////////////////////////////////////////////////////////////////
//
void CPpPriceProvider::Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
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
		if(_GetPriceProvider())
			Disconnect();

		PP::IPriceProviderPtr		spPriceProvider;
		_com_util::CheckError(spPriceProvider.CreateInstance(__uuidof(PP::BatchPriceInfo)));

		PP::IProviderPtr spProvider = spPriceProvider;
		spProvider->Type = spSettingsMgr->PriceProviderType();
		spPriceProvider->Connect();

		PP::IProvidersPtr spProvidersInfo; 
		_com_util::CheckError(spProvidersInfo.CreateInstance(PP::CLSID_Providers));
		_com_util::CheckError(spProvidersInfo->Initialize());
		PP::IProviderDataPtr spProviderInfo;

		_com_util::CheckError(spProvidersInfo->GetProvider(spProvider->Type,&spProviderInfo));
		
		VARIANT_BOOL bIsGroup;
		_com_util::CheckError(spProviderInfo->get_IsGroupRequestSupported(&bIsGroup));

		_com_util::CheckError(DispEventAdvise(spPriceProvider)); // can't connect error occure

		_SetPriceProvider(spPriceProvider);

		if(bIsGroup)
		{
			PP::IGroupPricePtr	spGroupPriceProvider;
			spGroupPriceProvider = spPriceProvider;
			_SetGroupPriceProvider(spGroupPriceProvider);
		}
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
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CPpPriceProvider::Disconnect()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		if(_GetGroupPriceProvider())
		{
			_SetGroupPriceProvider(NULL);
		}
		PP::IPriceProviderPtr spPriceProvider = _GetPriceProvider();
		if(spPriceProvider)
		{
			DispEventUnadvise(spPriceProvider);

			Sleep(1000L);

			CancelLastQuote();

			m_bLastQuoteReqNow = false;
			spPriceProvider->Disconnect();

			_SetPriceProvider(NULL);
		}
	}
	catch(_com_error& e)
	{
		CString strDescr;
		strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
		Trace(EODCO::enTrLogError,__FUNCTION__, (LPCTSTR)strDescr);
	}

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CPpPriceProvider::SetRequestData(CPpRequestDataPtr pReqData)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(pReqData == NULL)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Empty request data."));
		throw exception( _T("Empty request data."));
	}

	CancelLastQuote();

	m_spReqData = pReqData;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CPpPriceProvider::RequestLastQuote()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	PP::IPriceProviderPtr spPriceProvider = _GetPriceProvider();
	if(spPriceProvider == NULL)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Not connected to price provider."));
		throw logic_error(_T("Not connected to price provider."));
	}
	if(!m_spReqData || m_spReqData->Reqs()->size() <= 0)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Empty requests collection."));
		throw logic_error(_T("Empty requests collection."));
	}

	if(!m_bLastQuoteReqNow)
	{
		PP::IGroupPricePtr			spGroupPriceProvider = _GetGroupPriceProvider();
		try
		{
			m_bLastQuoteReqNow = true;

			CPpPriceRequestKeySetPtr spUndGroupReqs(m_spReqData->UndGroupReqs());

			if(spUndGroupReqs->size() > 0 && spGroupPriceProvider != NULL)
			{
				CPpPriceRequestKeySetPtr spUndReqs(m_spReqData->UndReqs());

				m_bIsGroupRequest = true;
				m_nLastQuoteReqDone = 0L;
				m_nLastQuoteReqCount = spUndReqs->size();
				m_nLastQuoteGroupReqDone = 0L;
				m_nLastQuoteGroupReqCount = m_nLastQuoteReqCount + spUndGroupReqs->size();

				for(CPpPriceRequestKeySet::iterator itUndReq = spUndReqs->begin(); itUndReq != spUndReqs->end(); itUndReq++)
				{
					CPpPriceRequestKey aKey(*itUndReq);
					PP::QuoteUpdateParams aParam;
					aKey.CopyTo(aParam);
					_com_util::CheckError(spPriceProvider->raw_RequestLastQuote(&aParam));
				}

				for(CPpPriceRequestKeySet::iterator itUndGroupReq = spUndGroupReqs->begin(); itUndGroupReq != spUndGroupReqs->end(); itUndGroupReq++)
				{
					CPpPriceRequestKey aKey(*itUndGroupReq);

					if(aKey.m_enType != PP::enGrSTK && aKey.m_enType != PP::enGrIDX)
					{
						Trace(EODCO::enTrLogError,__FUNCTION__,_T("Invalid group requests collection."));
						throw logic_error( _T("Invalid group requests collection."));
					}

					PP::QuoteUpdateParams aParam;
					aKey.CopyTo(aParam);

					_com_util::CheckError(spGroupPriceProvider->raw_RequestGroup(&aParam,PP::enGroupRequestLastQuote));
				}
			}
			else
			{
				CPpPriceRequestKeySetPtr spReqs(m_spReqData->Reqs());

				m_bIsGroupRequest = false;
				m_nLastQuoteReqDone = 0L;
				m_nLastQuoteReqCount = spReqs->size();
				m_nLastQuoteGroupReqDone = 0L;
				m_nLastQuoteGroupReqCount = 0L;

				for(CPpPriceRequestKeySet::iterator itReq = spReqs->begin(); itReq != spReqs->end(); itReq++)
				{
					CPpPriceRequestKey aKey(*itReq);
					PP::QuoteUpdateParams aParam;
					aKey.CopyTo(aParam);
					_com_util::CheckError(spPriceProvider->raw_RequestLastQuote(&aParam));
				}
			}

		}
		catch(_com_error& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
			m_bLastQuoteReqNow = false;

			spPriceProvider->raw_CancelLastQuote();
			if(spGroupPriceProvider)
					spGroupPriceProvider->raw_CancelGroup();

			CString strDescr;
			strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
			throw exception(strDescr);
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CPpPriceProvider::CancelLastQuote()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));

	PP::IPriceProviderPtr	spPriceProvider = _GetPriceProvider();
	PP::IGroupPricePtr		spGroupPriceProvider = _GetGroupPriceProvider();
	if(spPriceProvider == NULL)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Not connected to price provider."));
		throw logic_error(_T("Not connected to price provider."));
	}

	if(m_bLastQuoteReqNow)
	{
		m_bLastQuoteReqNow = false;

		if(m_spReqData)
		{
			spPriceProvider->raw_CancelLastQuote();
			if(spGroupPriceProvider)
					spGroupPriceProvider->raw_CancelGroup();
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}
STDMETHODIMP CPpPriceProvider::_OnLastQuote(struct PP::QuoteUpdateParams* pParams, struct PP::QuoteUpdateInfo* pResults)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(m_spReqData && m_bLastQuoteReqNow)
	{
		CPpPriceRequestKey aReq(*pParams);

		bool bLastQuoteDone = false;

		if(aReq.m_enType != PP::enGrSTK && aReq.m_enType != PP::enGrIDX)
		{
			if(IsExists(m_spReqData->Reqs(),aReq))
			{
				if(m_bIsGroupRequest && aReq.m_enType != PP::enOPT)
					m_nLastQuoteGroupReqDone++;

				m_nLastQuoteReqDone++;

				OnLastQuote(&aReq, pResults);

				bLastQuoteDone = (m_nLastQuoteReqCount == m_nLastQuoteReqDone
					|| m_bIsGroupRequest && m_nLastQuoteGroupReqCount == m_nLastQuoteGroupReqDone);
			}
		}
		else
		{
			m_nLastQuoteGroupReqDone++;
			bLastQuoteDone = (m_nLastQuoteGroupReqCount == m_nLastQuoteGroupReqDone);
		}

		if(bLastQuoteDone)
		{
			m_bLastQuoteReqNow = false;
			
			OnLastQuoteDone();
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return S_OK;
}

STDMETHODIMP CPpPriceProvider::_OnError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
										   PP::RequestsTypeEnum enReqType, VARIANT varRequest)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	CString sDesc = (LPCTSTR)_bstr_t(bsDescription);
	
	if(enErrorNumber == PP::enNotConnected || enErrorNumber == PP::enConnectionWasDisconnected)
	{
		OnDisconnect(enErrorNumber, bsDescription);
		return S_OK;
	}

	switch(enReqType)
	{
	case PP::enRequestLastQuote:
		{
			if(m_spReqData && m_bLastQuoteReqNow)
			{
				_QuoteUpdateParams qup;
				qup.Copy(varRequest);
				
				CPpPriceRequestKey aReq(qup);

				bool bLastQuoteDone = false;

				if(IsExists(m_spReqData->Reqs(), aReq) || IsExists(m_spReqData->UndGroupReqs(), aReq))
				{
					if(enReqType != PP::enGrSTK && enReqType != PP::enGrIDX)
					{
						if(m_bIsGroupRequest)
							m_nLastQuoteGroupReqDone++;

						m_nLastQuoteReqDone++;
					}
					else
						m_nLastQuoteGroupReqDone++;

					OnLastQuoteError(enErrorNumber, bsDescription, &aReq);

					if(enReqType != PP::enGrSTK && enReqType != PP::enGrIDX)
					{
						bLastQuoteDone = (m_nLastQuoteReqCount == m_nLastQuoteReqDone
							|| m_bIsGroupRequest && m_nLastQuoteGroupReqCount == m_nLastQuoteGroupReqDone);
					}
					else
						bLastQuoteDone = (m_nLastQuoteGroupReqCount == m_nLastQuoteGroupReqDone);

				}
				else
					ATLASSERT(false);

				if(bLastQuoteDone)
				{
					m_bLastQuoteReqNow = false;

					OnLastQuoteDone();
					break;
				}
			}

			OnError(enErrorNumber, bsDescription);
			break;
		}

	default:
		{
			OnError(enErrorNumber, bsDescription);
		}
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
	return S_OK;
}

};