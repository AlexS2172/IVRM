// TrackDataCore.cpp: implementation of the TrackDataCore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <process.h>
#include "TrackDataCore.h"

CRTCore  CTrackDataCore::m_Core;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CTrackDataCore::CTrackDataCore()
{
	m_hEventStop = CreateEvent(NULL,TRUE,FALSE,NULL);

	m_bConnected = false;
}

CTrackDataCore::~CTrackDataCore()
{

}

void CTrackDataCore::Connect(CBaseNotifier *pNotify)
{
	if(!pNotify) return;
	m_Core.Connect(pNotify);
	if(!m_bConnected)
	{
		ResetEvent(m_hEventStop);
	}

	m_bConnected = true;
}

void CTrackDataCore::Disconnect(CBaseNotifier* pNotify)
{
	if(!pNotify) return ;

	CAutoLock(m_Core.m_csNotify);
	if(!m_bConnected) return ;
	// Stop All Notifications
	// ...
	SetEvent(m_hEventStop);
	SetEvent(m_Core.m_hEventCancelOptions);
	SetEvent(m_Core.m_hEventCancelStocks);
	UnsubscribeQuote(pNotify, NULL);		
	m_Core.Disconnect(pNotify);
/*
	REQUEST::iterator iter = m_Core.m_Request.begin();
	while( iter!=m_Core.m_Request.end() )
	{
		if(m_Core.UnregisterRequest(iter->second,pNotify))
		{
			m_Core.m_Request.erase(iter);
			iter = m_Core.m_Request.begin();
			continue;
		}
		iter++;
	}

	m_Core.m_Responce.clear();
*/
	m_bConnected = false;
}


RT_ERR CTrackDataCore::RequestSymbol(CBaseNotifier *pNotifier, RT_SYMBOL* pSymbol, CComVariant& vtRequest)
{
	RT_ERR ret = RT_ERR_OK;
	try
	{
		USES_CONVERSION;
		ResetEvent(m_Core.m_hEventCancelStocks);

		if(!m_Core.ISConnected())
			throw(m_Core.GetTrackDataError(RT_ERR_CONNECTION_NOT_OPENED));

		SStructureRequest* pData = new SStructureRequest;
		pData->m_vtRequest = vtRequest;
		pData->AddNotifyer(pNotifier);
		
		CComBSTR bs = pSymbol->name;
		bs.ToUpper();
		pData->m_bsName = _bstr_t(bs);

		RT_REQ_ID ID;
		if(pNotifier->GetClientType() == CBaseNotifier::enClientStructureProvider)
		{
			RT_FIELD  fields[] = 
			{
					RT_SYMBOL_HANDLE,
					RT_PRIMARY_MKT_FLAG,
					RT_INSTRUMENT_TYPE,
					RT_DESCRIPTION,
					RT_DIV_PER_YEAR,
					RT_1ST_DIVID_DATE,
					RT_1ST_DIVID,
					RT_MARKET_CODE
			};


			ret = m_Core.RequestFields(&ID, RT_REQ_TYPE_GET, pSymbol, 9, fields, CRTCore::rtGetSymbol);
			if(RT_ERR_OK == ret)
				m_Core.RegisterRequest(ID, pData);
			else				delete pData;
		}
		else
		{
			RT_FIELD  fields[] = 
			{
					RT_SYMBOL_NAME,
					RT_SYMBOL_HANDLE,
					RT_PRIMARY_MKT_FLAG,
					RT_INSTRUMENT_TYPE,
					RT_DESCRIPTION,
					RT_DIV_PER_YEAR,
					RT_1ST_DIVID_DATE,
					RT_1ST_DIVID,
					RT_MARKET_CODE,
					RT_ASK,
					RT_BID,
					RT_OPEN,
					RT_Y_CLOSE, 
					RT_HIGH,
					RT_LOW,
					RT_TOT_VOLUME,
					RT_Y_VOLUME,
					RT_LAST_VOLUME,
			};
			ret = m_Core.RequestFields(&ID, RT_REQ_TYPE_GET, pSymbol, 18, fields, CRTCore::rtGetSymbol);			

			if(RT_ERR_OK == ret)
				m_Core.RegisterRequest(ID, pData);
			else
				delete pData;

		}
	}
	catch(_com_error &e)
	{
		throw e;
	}
	catch (CTrackDataError &err)
	{
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
	}
	return ret;
}

RT_ERR CTrackDataCore::RequestOptions(CBaseNotifier *pNotifier, 
									RT_SYMBOL* pUnderlyingSymbol, CComVariant& vtRequest)
{

	RT_ERR ret = RT_ERR_OK;
	SStructureRequest* pData = new SStructureRequest;
	RT_REQ_ID ID;
	::ResetEvent(m_Core.m_hEventCancelOptions);

	pData->AddNotifyer(pNotifier);
	pData->m_bsName = pUnderlyingSymbol->name;
	pData->m_vtRequest = vtRequest;
	_bstr_t bsName = CreateSymbolKey(pUnderlyingSymbol);

	SYMBOLCASHE::iterator iter =  m_Core.m_SymbolsHandleCashe.find(bsName);
	if(iter!=m_Core.m_SymbolsHandleCashe.end())
	{
		ret = m_Core.RequestGroup(&ID, RT_GRP_REQ_TY_STKOPTS, iter->second, CRTCore::rtGetOptionsIDs);
		if(RT_ERR_OK == ret)
			m_Core.RegisterRequest(ID, pData);
		else
			delete pData;

	}
	else
	{
		RT_FIELD field = RT_SYMBOL_HANDLE;
		ret = m_Core.RequestFields(&ID, RT_REQ_TYPE_GET, pUnderlyingSymbol, 1,&field, CRTCore::rtGetSymbol_Options);
		if(RT_ERR_OK == ret)
			m_Core.RegisterRequest(ID, pData);
		else
			delete pData;
	}

	if(RT_ERR_OK!=ret)
		delete pData;

	return ret;
}


RT_ERR CTrackDataCore::RequestQuote(CBaseNotifier* pNotify, RT_SYMBOL* pSymbol, CComVariant& vtRequest, bool bSubscribe)
{
	RT_ERR ret = RT_ERR_OK;
	try
	{
		USES_CONVERSION;

		if(!m_bConnected)
			throw(m_Core.GetTrackDataError(RT_ERR_CONNECTION_NOT_OPENED));

		bool bHandle = false;
		_bstr_t bsName = CreateSymbolKey(pSymbol);

		if(bSubscribe)
		{
			if(m_Subscribed.find(bsName)!=m_Subscribed.end())
				return ret;
		}

		
		SYMBOLCASHE::iterator iter = m_Core.m_SymbolsHandleCashe.find(bsName);
		if(iter != m_Core.m_SymbolsHandleCashe.end())
		{
			bHandle = true;
		} 

		RT_REQ_ID ID;
		RT_FIELD  fields[15] = 
		{
					RT_ASK,
					RT_BID,
					RT_ASK_SIZE,
					RT_BID_SIZE,
					RT_LAST_TIME,
					RT_LAST_PRICE,
					RT_OPEN_INTEREST,
					RT_TOT_VOLUME,
					RT_MARKET_CODE,
					RT_HIGH,
					RT_LOW,
					RT_OPEN,
					RT_Y_CLOSE,
					RT_SYMBOL_HANDLE,
					RT_PREV_DIFF_PRICE
		};

		SStructureRequest* pData = new SStructureRequest;
		pData->m_vtRequest = vtRequest;
		pData->AddNotifyer(pNotify);
		pData->m_bsName = bsName;// _bstr_t(pSymbol->name);


		if(bHandle)
		{
			if(!bSubscribe)
			{
				ret = m_Core.RequestFieldsViaHandle(&ID, RT_REQ_TYPE_GET, iter->second, 15, fields, CRTCore::rtGetQuote);
				if(ret==RT_ERR_OK)
				{
					m_Core.RegisterRequest(ID, pData);
					m_Core.m_LtRequest[bsName] = ID;
				}
			}
			else
			{
				REQUEST::iterator reqIter = m_Core.m_RtRequest.find(bsName);
				if(reqIter != m_Core.m_RtRequest.end())
					m_Core.RegisterRequest(reqIter->second, pData);
				else
				{
					ret = m_Core.RequestFieldsViaHandle(&ID, RT_REQ_TYPE_MIND, iter->second, 15, fields, CRTCore::rtGetQuote);
					if(ret==RT_ERR_OK)
					{
						m_Core.RegisterRequest(ID, pData);
						m_Core.m_RtRequest[bsName] = ID;
					}

				}
			}
		}
		else
		{
			if(!bSubscribe)
			{
				ret = m_Core.RequestFields(&ID, RT_REQ_TYPE_GET, pSymbol, 15, fields, CRTCore::rtGetQuote);
				if(ret==RT_ERR_OK)
				{
					m_Core.RegisterRequest(ID, pData);
					m_Core.m_LtRequest[bsName] = ID;
				}

			}
			else
			{
				REQUEST::iterator reqIter = m_Core.m_RtRequest.find(bsName);
				if(reqIter != m_Core.m_RtRequest.end())
					m_Core.RegisterRequest(reqIter->second, pData);
				else
				{
					ret = m_Core.RequestFields(&ID, RT_REQ_TYPE_MIND, pSymbol, 15, fields, CRTCore::rtGetQuote);
					if(ret==RT_ERR_OK)
					{
						m_Core.RegisterRequest(ID, pData);
						m_Core.m_RtRequest[bsName] = ID;
					}
				}
			}
		}

		if(ret!=RT_ERR_OK)	
			delete pData;
		else
		{
			if(bSubscribe)
				m_Subscribed.insert(bsName);
		}
	}
	catch(_com_error &e)
	{
		throw e;
	}
	catch (CTrackDataError &err)
	{
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
	}
	return ret;
}

void CTrackDataCore::UnsubscribeQuote(CBaseNotifier *pNotifier, RT_SYMBOL* pSymbol)
{
	if(!pSymbol || (pSymbol && !*pSymbol->name))
	{
		{
			CAutoLock ss(m_Core.m_csRequest);
			REQUEST::iterator iter = m_Core.m_RtRequest.begin(); 
			while(iter!=m_Core.m_RtRequest.end())
			{
				m_Subscribed.erase(iter->first);
				if(m_Core.UnregisterRequest(iter->second,pNotifier))
				{
					m_Core.RemoveFields(iter->second,0,NULL);
					m_Core.m_RtRequest.erase(iter);
					iter = m_Core.m_RtRequest.begin();
					continue;
				}
				iter++;
			}
		}
		{
			CAutoLock ss(m_Core.m_cSResponse);
			for(RESPONCE::iterator It =m_Core.m_Responce.begin(); It!=m_Core.m_Responce.end();It++)
			{
				It->m_Notify.erase(pNotifier);
			}
		}		
	}
	else
	{
		_bstr_t bsName = CreateSymbolKey(pSymbol);
		{
			CAutoLock ss(m_Core.m_csRequest);
			REQUEST::iterator iter = m_Core.m_RtRequest.find(bsName);
			if(iter != m_Core.m_RtRequest.end())
			{
				if(m_Core.UnregisterRequest(iter->second, pNotifier))
				{
					m_Core.RemoveFields(iter->second,0,NULL);
					m_Core.m_RtRequest.erase(iter);
				}
			}	
		}
		m_Subscribed.erase(bsName);
	}	
}

void CTrackDataCore::CancelOptions()
{
	SetEvent(m_Core.m_hEventCancelOptions);

}

void CTrackDataCore::CancelStocks()
{
	SetEvent(m_Core.m_hEventCancelStocks);
}

