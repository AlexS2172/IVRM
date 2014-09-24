// RTCore.cpp: implementation of the CRTCore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTCore.h"
#include <process.h>
#include <math.h>
#include <float.h>
_bstr_t bsError;
static inline bool fEQZero (double dVal)
{
	return fabs(dVal) < DBL_EPSILON;		
}

static inline bool fEQ (double dVal1 , double dVal2 )
{
	return fEQZero (dVal1 - dVal2);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const CTrackDataError  CRTCore::m_TrackDataErrors[]=
{
	{RT_ERR_BAD_USERNAME, _T("Invalid User Name"), enInvalidAccount},
	{RT_ERR_BAD_USER_ID, _T("Invalid User ID"), enInvalidAccount},
	{RT_ERR_CONNECTION_NOT_OPENED, _T("Connection is not opened"), enNotConnected},
	{RT_ERR_CONNECTION_NOT_OPEN, _T("Unable to open connection"), enCouldNotConnectToProvider},
	{RT_ERR_CONNECTION_INVALID, _T("Invalid connection ID"), enNotConnected },
	{RT_ERR_SYMBOL_NOT_FOUND, _T("No data available for the ticker requested"), enSymbolNotSupported},
	{RT_ERR_INVALID_REQ_ID, _T("Invalid request ID"), enInternalError},
	{RT_ERR_INVALID_FIELD, _T("Invalid Field Requested"), enInternalError},
	{RT_ERR_INVALID_CONVERSION, _T("Invalid conversion"), enInternalError},
	{RT_ERR_NOT_ENTITLED, _T("Error Not Entitled"), enInternalError},
	{RT_ERR_OUT_OF_STORAGE, _T("Out of storage"), enInternalError},
	{RT_ERR_BAD_CALLBACK, _T("Bad callback function"), enInternalError},
	{RT_ERR_NOT_OK, _T("Request complited with Not OK status"), enInternalError},
	{RT_ERR_INVALID_HANDLE, _T("Invalid handle value"), enInternalError},
	{RT_ERR_UNDEFINED_ERR, _T("Undefended error"), enInternalError},
	{RT_ERR_INVALID_RET_CODE, _T("Invalid Return Code"), enInternalError}
};

CTrackDataError	CRTCore::GetTrackDataError(RT_ERR err)
{
	int i=0;
	while (m_TrackDataErrors[i].m_szMessage)
	{
		if (m_TrackDataErrors[i].m_iError == err)
			return m_TrackDataErrors[i];
		i++;
	}
	bsError = "Unknown provider error code ";
	char buffer[100] ;
	itoa((int)err,buffer,10);
	bsError +=buffer;


	CTrackDataError tagErr = {-1,(LPCTSTR)bsError,enUnknown};

	return tagErr;
}


void CRTCore::rtStartUp(RT_CONNECT_ID rt_id, RT_ERR err_code)
{
	if(rt_id == m_rtConnectID)
	{
		m_rtErrorCode = err_code;
		SetEvent(m_hEventStartUp);
	}
}

void CRTCore::rtShutDown(RT_CONNECT_ID rt_id,  RT_ERR err_code)
{
	if(rt_id == m_rtConnectID)
	{
		m_rtErrorCode = err_code;
		SetEvent(m_hEventDisconnected);
	}
}

CRTCore::CRTCore()
{
	m_bIsConnected = false;
	unsigned id = 0;
	m_hEventNotify = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEventStopThread  = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hEventCancelOptions = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventCancelStocks = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_hEventsSender = (HANDLE)_beginthreadex(NULL,0,EventsSender,this,0,&id); 
}

RT_CONNECT_ID          CRTCore::m_rtConnectID = NULL;

HANDLE				   CRTCore::m_hEventStartUp = CreateEvent(NULL,FALSE,FALSE,NULL);
HANDLE				   CRTCore::m_hEventDisconnected = CreateEvent(NULL,FALSE,FALSE,NULL);
RT_ERR				   CRTCore::m_rtErrorCode = RT_ERR_OK;

CRTCore::~CRTCore()
{
	SetEvent(m_hEventStopThread);
	Sleep(100);

	CloseHandle(m_hEventStartUp);
	CloseHandle(m_hEventDisconnected);
	CloseHandle(m_hEventNotify);

	CloseHandle(m_hEventCancelOptions);
	CloseHandle(m_hEventCancelStocks);


}

bool CRTCore::Connect(CBaseNotifier *pNotify)
{
	try
	{
		if(!m_bIsConnected)
		{
			_Connect();
			FindAndAddNotifier(pNotify);
			m_bIsConnected = true;
		}
	}
	catch (CTrackDataError &err)
	{
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		ATLTRACE(_T("Error occured.Connect ->%s\n"),szErr);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
		return false;
	}
	return true;
}

bool CRTCore::Disconnect(CBaseNotifier* pNotify )
{
	{
		{
			CAutoLock ss(m_csRequest);
			REQUEST::iterator iter = m_RtRequest.begin(); 
			while(iter!=m_RtRequest.end())
			{
				if(UnregisterRequest(iter->second,pNotify))
				{
					RemoveFields(iter->second,0,NULL);
					m_RtRequest.erase(iter);
					iter = m_RtRequest.begin();
					continue;
				}
				iter++;
			}
		}

		{
			CAutoLock ss(m_csRequest);
			REQUEST::iterator iter = m_LtRequest.begin(); 
			while(iter!=m_LtRequest.end())
			{
				if(UnregisterRequest(iter->second,pNotify))
				{
					RemoveFields(iter->second,0,NULL);
					m_LtRequest.erase(iter);
					iter = m_LtRequest.begin();
					continue;
				}
				iter++;
			}
		}

		{
			CAutoLock ss(m_cSResponse);
			for(RESPONCE::iterator It =m_Responce.begin(); It!=m_Responce.end();It++)
			{
				It->m_Notify.erase(pNotify);
			}
		}

	}

	CAutoLock sc(m_csClients);
	m_Clients.remove(pNotify);
	
	if(!m_Clients.size())
	{
		rt_shutdown(m_rtConnectID);
		m_bIsConnected = false;
	}

	return true;
}


bool CRTCore::ParseQuoteInfo(const SResponse& resp, long lSkippesSize, long lSizeQueue, RT_RAW_FIELD_BUF* data_buf, CComVariant& vtResponse)
{
		CComRecPtr<QuoteUpdateInfo> osi;
		osi.Init();

		int iHandle;
		char buf[100];
		int len = 0;

		osi->Skipped =	lSkippesSize;

//		vt_date dt1 = vt_date::GetCurrentDate();
//		vt_date dt2 = resp.m_CreationDate;
	
		osi->WaitTime = GetTickCount() - resp.m_CreationTick;
		osi->TotalRequests = lSizeQueue;

		rt_get_field_int32(data_buf, RT_SYMBOL_HANDLE, &iHandle);

		rt_get_field_char(data_buf, RT_MARKET_CODE,10, buf, &len);
		osi->Exchange = _bstr_t(buf).copy();

		rt_get_field_real(data_buf, RT_ASK, &osi->AskPrice);
		rt_get_field_real(data_buf, RT_BID, &osi->BidPrice);
		rt_get_field_int32(data_buf, RT_ASK_SIZE, (int*)&osi->AskSize);
		rt_get_field_int32(data_buf, RT_BID_SIZE, (int*)&osi->BidSize);
		rt_get_field_int32(data_buf, RT_OPEN_INTEREST, (int*)&osi->OpenInterest);
		rt_get_field_int32(data_buf, RT_TOT_VOLUME, (int*)&osi->Volume);
		rt_get_field_real(data_buf, RT_LAST_PRICE, &osi->LastPrice);


		rt_get_field_real(data_buf, RT_HIGH, &osi->HighPrice);
		rt_get_field_real(data_buf, RT_LOW, &osi->LowPrice);
		rt_get_field_real(data_buf, RT_OPEN, &osi->OpenPrice);
		rt_get_field_real(data_buf, RT_Y_CLOSE, &osi->ClosePrice);
		rt_get_field_real(data_buf, RT_PREV_DIFF_PRICE, &osi->NetChange);
		
		int lastTime;
		rt_get_field_int32(data_buf, RT_LAST_TIME, &lastTime);
		int hours = lastTime/3600;
		int minutes = (lastTime%3600)/60;
		int secs = lastTime -(hours*3600+minutes*60); 
		vt_date dt = vt_date::GetCurrentDate();
		osi->UpdateDateTime = vt_date(dt.get_year(),dt.get_month(), dt.get_day(), hours, minutes, secs);
		
		_QuoteUpdateParams param(resp.m_vtRequest);
		if(param->Type == enOPT )
			osi->LotSize = 100;
		else
			osi->LotSize = 1;

		osi->Currency = ::SysAllocString(L"USD");

		osi.CopyTo(vtResponse);

		if(WAIT_TIMEOUT != WaitForSingleObject(m_hEventStopThread,0))
			return false;

		return true;
}

bool CRTCore::ParseQuote(const SResponse& resp, RT_RAW_FIELD_BUF* data_buf, CComVariant& vtResponse)
{
		CComRecPtr<QuoteUpdateResult> osi;
		osi.Init();
		int iHandle;
		char buf[100];
		int len = 0;

		rt_get_field_int32(data_buf, RT_SYMBOL_HANDLE, &iHandle);

		rt_get_field_char(data_buf, RT_MARKET_CODE,10, buf, &len);
		osi->Exchange = _bstr_t(buf).copy();

		rt_get_field_real(data_buf, RT_ASK, &osi->AskPrice);
		rt_get_field_real(data_buf, RT_BID, &osi->BidPrice);
		rt_get_field_int32(data_buf, RT_ASK_SIZE, (int*)&osi->AskSize);
		rt_get_field_int32(data_buf, RT_BID_SIZE, (int*)&osi->BidSize);
		rt_get_field_int32(data_buf, RT_OPEN_INTEREST, (int*)&osi->OpenInterest);
		rt_get_field_int32(data_buf, RT_TOT_VOLUME, (int*)&osi->Volume);
		rt_get_field_real(data_buf, RT_LAST_PRICE, &osi->LastPrice);

		int lastTime;
		rt_get_field_int32(data_buf, RT_LAST_TIME, &lastTime);
		int hours = lastTime/3600;
		int minutes = (lastTime%3600)/60;
		int secs = lastTime -(hours*3600+minutes*60); 
		vt_date dt = vt_date::GetCurrentDate();
		osi->UpdateDateTime = vt_date(dt.get_year(),dt.get_month(), dt.get_day(), hours, minutes, secs);

		_QuoteUpdateParams param(resp.m_vtRequest);
		if(param->Type == enOPT )
			osi->LotSize = 100;
		else
			osi->LotSize = 1;

		osi->Currency = ::SysAllocString(L"USD");

		osi.CopyTo(vtResponse);

		if(WAIT_TIMEOUT != WaitForSingleObject(m_hEventStopThread,0))
			return false;

		return true;
}


bool CRTCore::ParseOptionInfo(RT_RAW_FIELD_BUF* data_buf, CComVariant& vtResp)
{
	char buf[100];
	int len;
	CComRecPtr<OptionInfo> ois;
	ois.Init();

	rt_get_field_real(data_buf, RT_STRIKE_PRICE, &ois->StrikePrice);
	
	int dte = 0;
	rt_get_field_int32(data_buf, RT_OPTION_DAY, &dte);
	ois->ExpirationDate = long(vt_date::GetCurrentDate())+dte;			
				
	rt_get_field_char(data_buf, RT_SYMBOL_NAME, 100,buf,&len);
	ois->Series = _bstr_t(buf).copy();
	rt_get_field_int32(data_buf, RT_INSTRUMENT_TYPE, &len);
	if(len == 3)
		ois->OptionType = enCall;
	else
		ois->OptionType = enPut;

	rt_get_field_char(data_buf, RT_MARKET_CODE,100, buf, &len);
	ois->Exchange = _bstr_t(buf).copy();

	rt_get_field_real(data_buf, RT_ASK, &ois->Ask);
	rt_get_field_real(data_buf, RT_BID, &ois->Bid);
	rt_get_field_real(data_buf, RT_LAST_PRICE,&ois->Last);
	rt_get_field_int32(data_buf, RT_OPEN_INTEREST, &len);
	ois->OpenInterest = len;

	rt_get_field_int32(data_buf, RT_TOT_VOLUME, &len);
	ois->Volume =len;

	rt_get_field_int32(data_buf, RT_Y_VOLUME, &len);
	ois->YVolume =len;
	ois->LotSize = 1;
	ois->Currency = _bstr_t(L"USD").copy();

	if( fEQZero(ois->Last) && fEQZero(ois->Bid) && fEQZero(ois->Ask) )
	{
		ois->Last =0.001;
	}

	ois.CopyTo(vtResp);
	if(WAIT_TIMEOUT != WaitForSingleObject(m_hEventStopThread,0))
		return false;

	
	return true;
}

bool CRTCore::ParseOptionStructure(RT_RAW_FIELD_BUF* data_buf, CComVariant& vtResp)
{
	char buf[100];
	int len;
	CComRecPtr<OptionResults> oi;
	oi.Init();
	rt_get_field_real(data_buf, RT_STRIKE_PRICE, &oi->StrikePrice);
	
	int dte = 0;
	rt_get_field_int32(data_buf, RT_OPTION_DAY, &dte);
	oi->ExpirationDate = long(vt_date::GetCurrentDate())+dte;

	rt_get_field_char(data_buf, RT_SYMBOL_NAME, 10,buf,&len);
	oi->Series = _bstr_t(buf).copy();

	rt_get_field_int32(data_buf, RT_INSTRUMENT_TYPE, &len);
	if(len == 3)
		oi->OptionType = enCall;
	else
		oi->OptionType = enPut;

	rt_get_field_char(data_buf, RT_MARKET_CODE,10, buf, &len);
	oi->Exchange = _bstr_t(buf).copy();
	oi->LotSize = 1;
	oi->Currency = _bstr_t(L"USD").copy();


	oi.CopyTo(vtResp);
	if(WAIT_TIMEOUT != WaitForSingleObject(m_hEventStopThread,0))
		return false;

	return true;
}

bool CRTCore::ParseSymbolInfo(RT_RAW_FIELD_BUF* data_buf, CComVariant& vtResp)
{
	char buf[100];
	int len = 0;

	CComRecPtr<StockInfo> osi;
	osi.Init();
	rt_get_field_int32(data_buf, RT_INSTRUMENT_TYPE, (int*)&osi->StockType);
	rt_get_field_char(data_buf, RT_SYMBOL_NAME, 100, buf, &len);
	osi->Name = _bstr_t(buf).copy();

	rt_get_field_char(data_buf, RT_DESCRIPTION, 100, buf, &len);
	osi->Description = _bstr_t(buf).copy();

	rt_get_field_char(data_buf, RT_MARKET_CODE,10, buf, &len);
	osi->Exchange = _bstr_t(buf).copy();

	rt_get_field_real(data_buf, RT_ASK, &osi->Ask);
	rt_get_field_real(data_buf, RT_BID, &osi->Bid);
	rt_get_field_real(data_buf, RT_OPEN,&osi->OpenPrice);
	rt_get_field_real(data_buf, RT_LOW, &osi->Low);
	rt_get_field_real(data_buf, RT_HIGH, &osi->High);
	rt_get_field_real(data_buf, RT_LAST_VOLUME, &osi->LastTradeVolume);
	rt_get_field_real(data_buf, RT_TOT_VOLUME, &osi->CurrentVolume);
	rt_get_field_real(data_buf, RT_Y_VOLUME, &osi->Volume);

	rt_get_field_int32(data_buf, RT_DIV_PER_YEAR, &len);
	osi->DivFreq = len;
	rt_get_field_char(data_buf, RT_1ST_DIVID_DATE,100,buf,&len);
	rt_get_field_real(data_buf, RT_1ST_DIVID, &osi->DivAmount);
	osi->DivLastDate = 0;
	osi->LotSize = 1;

	osi.CopyTo(vtResp);

	if(WAIT_TIMEOUT != WaitForSingleObject(m_hEventStopThread,0))
		return false;

	return true;

}

bool CRTCore::ParseSymbolStructure(RT_RAW_FIELD_BUF* data_buf, CComVariant& vtResp)
{
	char buf[100];
	int len = 0;
	CComRecPtr<StockResults> os;
	os.Init();

	rt_get_field_int32(data_buf, RT_INSTRUMENT_TYPE, (int*)&os->StockType);
	rt_get_field_char(data_buf, RT_DESCRIPTION, 100, buf, &len);
	os->Description = _bstr_t(buf).copy();

	rt_get_field_char(data_buf, RT_MARKET_CODE,10, buf, &len);
	os->Exchange = _bstr_t(buf).copy();

	rt_get_field_int32(data_buf, RT_DIV_PER_YEAR, &len);
	os->DivFreq = len;

	rt_get_field_char(data_buf, RT_1ST_DIVID_DATE,100,buf,&len);
	rt_get_field_real(data_buf, RT_1ST_DIVID, &os->DivAmount);
	os->DivLastDate = 0;
	os->LotSize = 1;

	os.CopyTo(vtResp);

	if(WAIT_TIMEOUT != WaitForSingleObject(m_hEventStopThread,0))
		return false;

	return true;

}

UINT CRTCore::EventsSender(LPVOID pData)
{
	CRTCore* pThis = reinterpret_cast<CRTCore*>(pData);
	if(pThis)
	{
		CoInitializeEx(NULL,COINIT_MULTITHREADED);  
		HANDLE hEvents[] = 
		{
			pThis->m_hEventStopThread,
			pThis->m_hEventNotify
		};
		while(1)
		{
			if((WAIT_OBJECT_0+1) != CBaseNotifier::WaitWithEvents(2,hEvents))
				break;
			int len = 0;
			{
				CAutoLock s(pThis->m_cSResponse);
				len = pThis->m_Responce.size();
			}

			SResponse resp;
			while(len)
			{
				if(WAIT_TIMEOUT != WaitForSingleObject(pThis->m_hEventStopThread,0))
					break;
				
				long lQueueSize = 0;
				long lSkippedSize = 0;

//				if(pThis->m_Request.empty())
//					break;
//				else
				{
					if(pThis->m_Responce.empty() || WAIT_TIMEOUT != WaitForSingleObject(pThis->m_hEventStopThread, 0))
						break;

					CAutoLock s(pThis->m_cSResponse);
					RESPONCE::iterator itResp = pThis->m_Responce.begin();
					if(itResp == pThis->m_Responce.end())
						break;

					try
					{
						resp = *itResp;
						pThis->m_Responce.erase(itResp);
						lQueueSize = pThis->m_Responce.size();

					}
					catch(...)
					{
						lQueueSize = 0;
					}
					REQIDs::iterator itIDs = pThis->m_ReqIDs.find(resp.m_reqID); 
					if(itIDs != pThis->m_ReqIDs.end())
					{
						lSkippedSize = itIDs->second;
						pThis->m_ReqIDs.erase(itIDs); 
					}

				}


				try
				{
					CAutoLock(pThis->m_csNotify);
					for(NOTIFIERS::iterator itNotify = resp.m_Notify.begin(); itNotify!=resp.m_Notify.end(); itNotify++)
					{

						if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStopThread,0))
							break;

						switch(resp.m_enType)
						{
						case SResponse::enError:
							{
								(*itNotify)->OnError(resp.m_extError, resp.m_bsMessage, resp.m_requestType, resp.m_vtRequest);

							} break;
						case SResponse::enLastQuote:
							{
								CComVariant vtResponse;
								pThis->UnregisterRequest(resp.m_reqID, *itNotify);
								bool bRes = false;
								if((*itNotify)->GetResponseType() == CBaseNotifier::enResponseLazy)
									bRes = pThis->ParseQuoteInfo(resp, lSkippedSize, lQueueSize, resp.m_pResponseBuffer, vtResponse);
								else
									bRes = pThis->ParseQuote(resp, resp.m_pResponseBuffer, vtResponse);
								if(bRes)
									(*itNotify)->OnLastQuote(resp.m_vtRequest, vtResponse);
							}
							break;
						case SResponse::enRealtime:
							{
								CComVariant vtResponse;
								bool bRes = false;
								if(IsBadCodePtr((FARPROC)*itNotify))
									break;
								if((*itNotify)->GetResponseType() == CBaseNotifier::enResponseLazy)
									bRes = pThis->ParseQuoteInfo(resp, lSkippedSize, lQueueSize, resp.m_pResponseBuffer, vtResponse);
								else
									bRes = pThis->ParseQuote(resp, resp.m_pResponseBuffer, vtResponse);
								if(bRes)
									(*itNotify)->OnQuoteUpdate(resp.m_vtRequest, vtResponse);
							}
							break;
						case SResponse::enOptionInfo:
							{
								pThis->UnregisterRequest(resp.m_reqID, *itNotify);
								CComVariant vtResponse;
								if(pThis->ParseOptionInfo(resp.m_pResponseBuffer, vtResponse))
									(*itNotify)->OnOption(resp.m_vtRequest, vtResponse, CComVariant(resp.m_bValue));
							}break;
						case SResponse::enOptionStructure:
							{
								pThis->UnregisterRequest(resp.m_reqID, *itNotify);
								CComVariant vtResponse;
								if(pThis->ParseOptionStructure(resp.m_pResponseBuffer, vtResponse))
									(*itNotify)->OnOption(resp.m_vtRequest, vtResponse, CComVariant(resp.m_bValue));
							}
							break;
						case SResponse::enStockInfo:
							{
								pThis->UnregisterRequest(resp.m_reqID, *itNotify);
								CComVariant vtResponse;
								if(pThis->ParseSymbolInfo(resp.m_pResponseBuffer, vtResponse))
									(*itNotify)->OnStock(resp.m_vtRequest, vtResponse);
							}
							break;
						case SResponse::enStockStructure:
							{
								pThis->UnregisterRequest(resp.m_reqID, *itNotify);
								CComVariant vtResponse;
								if(pThis->ParseSymbolStructure(resp.m_pResponseBuffer, vtResponse))
									(*itNotify)->OnStock(resp.m_vtRequest, vtResponse);
							}
							break;
						}
					}

				}
				catch(...){	}

				{
					CAutoLock s(pThis->m_cSResponse);
					len = pThis->m_Responce.size();
				}

			}
		}
		CoUninitialize();
	}
	return 0;
}



void CRTCore::rtGetQuote(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_REQ_TYPE req_type, int field_count, RT_FIELD* fields, int data_length, RT_RAW_FIELD_BUF* data_buf, RT_ERR err_code)
{
	try
	{
		if(user_data)
		{
			CRTCore* pThis = reinterpret_cast<CRTCore*>(user_data);
			CAutoLock ss(pThis->m_csRequest);

			if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStopThread,0))
			{
				pThis->UnregisterRequest(req_id, NULL);
				return;
			}

			SStructureRequest* pRequest = pThis->GetRequest(req_id);
			if(!pRequest)
				err_code = RT_ERR_INVALID_REQ_ID; 

			if(RT_ERR_OK == err_code )
			{
				int iHandle;
				rt_get_field_int32(data_buf, RT_SYMBOL_HANDLE, &iHandle);

				SYMBOLCASHE::iterator iter = pThis->m_SymbolsHandleCashe.find(pRequest->m_bsName);

				if(iter == pThis->m_SymbolsHandleCashe.end())
					pThis->m_SymbolsHandleCashe[pRequest->m_bsName] = iHandle;

				SResponse resp;
				if(req_type == RT_REQ_TYPE_GET)
					resp.m_enType = SResponse::enLastQuote;
				else
					resp.m_enType = SResponse::enRealtime;

				resp.m_bValue = false;
				resp.CopyNotifiers(pRequest->m_Notify);
				resp.m_vtRequest = pRequest->m_vtRequest;
				resp.m_reqID = req_id;

				resp.AssignResponseBuffer(data_length, data_buf);
				

				if(req_type == RT_REQ_TYPE_GET)
				{
					CAutoLock ds(pThis->m_csRequest);
					_bstr_t bsGet = pRequest->m_bsName;
					 if(pThis->UnregisterRequest(req_id,pRequest->m_Notify))
						 pThis->m_LtRequest.erase(bsGet);

				}
				if(pThis->AddResponse(resp))
					SetEvent(pThis->m_hEventNotify);

			}
			else
			{
				SResponse resp;
				CTrackDataError error = pThis->GetTrackDataError(err_code);
				resp.m_enType = SResponse::enError;
				resp.CopyNotifiers(pRequest->m_Notify);

				resp.m_vtRequest = pRequest->m_vtRequest;
				resp.m_reqID = req_id;
				resp.m_extError = error.m_enExtError;
				resp.m_bsMessage = error.m_szMessage;
				resp.m_requestType = req_type==RT_REQ_TYPE_GET?enRequestLastQuote:enSubscribeQuote;
				if(pThis->AddResponse(resp))
					SetEvent(pThis->m_hEventNotify);
				
				pThis->UnregisterRequest(req_id, NULL);
			}
		}
	}
	catch(...)
	{}
	return;
}

void CRTCore::rtGetOption(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_REQ_TYPE req_type, int field_count, RT_FIELD* fields, int data_length, RT_RAW_FIELD_BUF* data_buf, RT_ERR err_code)
{
	if(user_data)
	{
		CRTCore* pThis = reinterpret_cast<CRTCore*>(user_data);
		CAutoLock ss(pThis->m_csRequest);

		SStructureRequest* pRequest = pThis->GetRequest(req_id);
		if(!pRequest)
			err_code = RT_ERR_INVALID_REQ_ID; 

		HANDLE hEvents[] =
		{
			pThis->m_hEventCancelOptions,
			pThis->m_hEventStopThread
		};

		if(WAIT_TIMEOUT != WaitForMultipleObjects(2, hEvents, FALSE, 0))
		{
			pThis->UnregisterRequest(req_id, NULL);
			return;
		}
		
		if(RT_ERR_OK == err_code)
		{
			SResponse resp;
			resp.m_enType = SResponse::enNone;
			resp.AssignResponseBuffer(data_length,data_buf);

			if((*pRequest->m_Notify.begin())->GetClientType() == CBaseNotifier::enClientStructureProvider)
				resp.m_enType = SResponse::enOptionStructure;
			if((*pRequest->m_Notify.begin())->GetClientType() == CBaseNotifier::enClientStructureInfo)
				resp.m_enType = SResponse::enOptionInfo;

			resp.m_bValue = pRequest->m_bValue;
			resp.CopyNotifiers(pRequest->m_Notify);
			resp.m_vtRequest = pRequest->m_vtRequest;
			resp.m_reqID = req_id;

			if(pThis->AddResponse(resp))
				SetEvent(pThis->m_hEventNotify);
		}
		else
		{
			SResponse resp;
			CTrackDataError error = pThis->GetTrackDataError(err_code);
			resp.m_enType = SResponse::enError;
			resp.CopyNotifiers(pRequest->m_Notify);

			resp.m_vtRequest = pRequest->m_vtRequest;
			resp.m_reqID = req_id;
			resp.m_extError = error.m_enExtError;
			resp.m_bsMessage = error.m_szMessage;
			resp.m_requestType = ((req_type==RT_REQ_TYPE_GET)?enRequestLastQuote:enSubscribeQuote);
			if(pThis->AddResponse(resp))
				SetEvent(pThis->m_hEventNotify);
		}
		pThis->UnregisterRequest(req_id, NULL);
	}
}


void CRTCore::rtGetOptionsIDs(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_GRP_REQ_TY group_type, RT_HANDLE underlier, int tot_num, int cur_offset, int num_received, RT_HANDLE* handle_list, RT_ERR err_code)
{
	if(user_data)
	{
		CRTCore* pThis = reinterpret_cast<CRTCore*>(user_data);
		CAutoLock ss(pThis->m_csRequest);

		SStructureRequest* pRequest = pThis->GetRequest(req_id);
		if(!pRequest)
			err_code = RT_ERR_INVALID_REQ_ID; 

		bool bLast = false;
		HANDLE hEvents[] =
		{
			pThis->m_hEventCancelOptions,
			pThis->m_hEventStopThread
		};

		if(WAIT_TIMEOUT != WaitForMultipleObjects(2, hEvents, FALSE, 0))
		{
			pThis->UnregisterRequest(req_id, NULL);
			return;
		}

		if(RT_ERR_OK == err_code)
		{
			try
			{
				RT_REQ_ID ID;
				for(int i = 0; i<num_received; i++)
				{
					if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventCancelOptions,0))
						break;

					bLast = (tot_num == (num_received+cur_offset));

					if((*pRequest->m_Notify.begin())->GetClientType()==CBaseNotifier::enClientStructureProvider)
					{
						SStructureRequest* pReq = new SStructureRequest;
						*pReq = *pRequest;
						pReq->m_bValue = bLast;

						RT_FIELD field[] =
						{
							RT_STRIKE_PRICE,
							RT_OPTION_DAY,
							RT_SYMBOL_NAME,
							RT_INSTRUMENT_TYPE,
							RT_MARKET_CODE
						};
						RT_ERR err =pThis->RequestFieldsViaHandle(&ID, RT_REQ_TYPE_GET, handle_list[i],5,field,rtGetOption);

						if(RT_ERR_OK == err)
							pThis->RegisterRequest(ID, pReq);
						else
						{

							SResponse resp;
							CTrackDataError error = pThis->GetTrackDataError(err_code);
							resp.m_enType = SResponse::enError;
							resp.CopyNotifiers(pRequest->m_Notify);

							resp.m_vtRequest = pRequest->m_vtRequest;
							resp.m_reqID = req_id;
							resp.m_extError = error.m_enExtError;
							resp.m_bsMessage = error.m_szMessage;
							resp.m_requestType = enRequestOptions;
							if(pThis->AddResponse(resp))
								SetEvent(pThis->m_hEventNotify);

							delete pReq;
						}
					}
					if((*pRequest->m_Notify.begin())->GetClientType()==CBaseNotifier::enClientStructureInfo)
					{
						SStructureRequest* pReq = new SStructureRequest;
						*pReq = *pRequest;
						pReq->m_bValue = bLast;

						RT_FIELD field[] =
						{
							RT_STRIKE_PRICE,
							RT_OPTION_DAY,
							RT_SYMBOL_NAME,
							RT_INSTRUMENT_TYPE,
							RT_MARKET_CODE,
							RT_ASK,
							RT_BID,
							RT_LAST_PRICE,
							RT_OPEN_INTEREST,
							RT_TOT_VOLUME,
							RT_Y_VOLUME
						};

						RT_ERR err = pThis->RequestFieldsViaHandle(&ID, RT_REQ_TYPE_GET, handle_list[i],11,field,rtGetOption);
						if(RT_ERR_OK == err)
							pThis->RegisterRequest(ID, pReq);
						else
						{
							SResponse resp;
							CTrackDataError error = pThis->GetTrackDataError(err_code);
							resp.m_enType = SResponse::enError;
							resp.CopyNotifiers(pRequest->m_Notify);

							resp.m_vtRequest = pRequest->m_vtRequest;
							resp.m_reqID = req_id;
							resp.m_extError = error.m_enExtError;
							resp.m_bsMessage = error.m_szMessage;
							resp.m_requestType = enRequestOptions;
							if(pThis->AddResponse(resp))
								SetEvent(pThis->m_hEventNotify);

							delete pReq;
						}
					}
				}
				if(bLast)
				{
					pThis->UnregisterRequest(req_id, NULL);
				}

			}
			catch(...)
			{
				CComVariant vt;
				CTrackDataError error = pThis->GetTrackDataError(RT_ERR_UNDEFINED_ERR);

				SResponse resp;
				resp.m_enType = SResponse::enError;
				resp.CopyNotifiers(pRequest->m_Notify);

				resp.m_vtRequest = vt;
				resp.m_reqID = req_id;
				resp.m_extError = error.m_enExtError;
				resp.m_bsMessage = error.m_szMessage;
				resp.m_requestType = enRequestOptions;
				if(pThis->AddResponse(resp))
					SetEvent(pThis->m_hEventNotify);

				pThis->UnregisterRequest(req_id, NULL);
			}

		}
		else
		{
			if(pRequest)
			{

				SResponse resp;
				CTrackDataError error = pThis->GetTrackDataError(err_code);
				resp.m_enType = SResponse::enError;
				resp.CopyNotifiers(pRequest->m_Notify);

				resp.m_vtRequest = pRequest->m_vtRequest;
				resp.m_reqID = req_id;
				resp.m_extError = error.m_enExtError;
				resp.m_bsMessage = error.m_szMessage;
				resp.m_requestType = enRequestOptions;
				if(pThis->AddResponse(resp))
					SetEvent(pThis->m_hEventNotify);

				pThis->UnregisterRequest(req_id, NULL);
			}
		}
	}
}

void CRTCore::rtGetSymbol_Options(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_REQ_TYPE req_type, int field_count, RT_FIELD* fields, int data_length, RT_RAW_FIELD_BUF* data_buf, RT_ERR err_code)
{
	if(!user_data) return;

	CRTCore* pThis = reinterpret_cast<CRTCore*>(user_data);
	CAutoLock ss(pThis->m_csRequest);
	SStructureRequest* pRequest =pThis->GetRequest(req_id);
	if(!pRequest)
		err_code = RT_ERR_INVALID_REQ_ID; 
	
	HANDLE hEvents[] =
	{
		pThis->m_hEventCancelOptions,
		pThis->m_hEventStopThread
	};

	if(WAIT_TIMEOUT != WaitForMultipleObjects(2, hEvents, FALSE, 0))
	{
		pThis->UnregisterRequest(req_id, NULL);
		return;
	}

	if(RT_ERR_OK == err_code)
	{
		int iHandle;
		if(RT_ERR_OK == rt_get_field_int32(data_buf, RT_SYMBOL_HANDLE, &iHandle))
		{
			RT_REQ_ID ID;
			SStructureRequest* pReq = new SStructureRequest;
			*pReq = *pRequest;
			pThis->m_SymbolsHandleCashe[pRequest->m_bsName]=(RT_HANDLE)iHandle;
			RT_ERR err = rt_request_group(pThis->m_rtConnectID, &ID, pThis, RT_GRP_REQ_TY_STKOPTS, iHandle, rtGetOptionsIDs);
			if(RT_ERR_OK == err)
			{
				pThis->RegisterRequest(ID, pReq);				
			}
			else
			{
				SResponse resp;
				CTrackDataError error = pThis->GetTrackDataError(err_code);
				resp.m_enType = SResponse::enError;
				resp.CopyNotifiers(pRequest->m_Notify);

				resp.m_vtRequest = pRequest->m_vtRequest;
				resp.m_reqID = req_id;
				resp.m_extError = error.m_enExtError;
				resp.m_bsMessage = error.m_szMessage;
				resp.m_requestType = enRequestOptions;
				if(pThis->AddResponse(resp))
					SetEvent(pThis->m_hEventNotify);

				delete pReq;
			}
		}
	}
	else
	{
		if(pRequest)
		{
			SResponse resp;
			CTrackDataError error = pThis->GetTrackDataError(err_code);
			resp.m_enType = SResponse::enError;
			resp.CopyNotifiers(pRequest->m_Notify);

			resp.m_vtRequest = pRequest->m_vtRequest;
			resp.m_reqID = req_id;
			resp.m_extError = error.m_enExtError;
			resp.m_bsMessage = error.m_szMessage;
			resp.m_requestType = enRequestOptions;
			if(pThis->AddResponse(resp))
				SetEvent(pThis->m_hEventNotify);

		}
	}
	pThis->UnregisterRequest(req_id, NULL);
}


void CRTCore::rtGetSymbol    // Callback to get data from RT server  
(
   RT_CONNECT_ID              rt_id , // Identifies RT connection              
   RT_REQ_ID                 req_id , // Identifies original request          
                                      //  generating this callback            
   void *                 user_data , // User data pointer from original user 
                                      //  request                             
   RT_REQ_TYPE             req_type , // Type of response                     
   int                  field_count , // Number of fields in "fields" below.  
   RT_FIELD *                fields , // Pointer to list of values indicating 
                                      //  the fields returned.                
   int                  data_length , // Length, in bytes, of data_buf.       
   RT_RAW_FIELD_BUF *      data_buf , // Pointer to buffer holding returned   
                                      //  data.  This data may never be       
                                      //  accessed directly, only through the 
                                      //  rt_get_field_... calls.  Using the  
                                      //  data_length field above, this buffer
                                      //  may be moved by the user wherever   
                                      //  desired, since it is self-          
                                      //  describing and does not depend on   
                                      //  any other external data.            
   RT_ERR                    err_code // Error code:                          
                                      //        RT_ERR_OK                     
                                      //        RT_ERR_SYMBOL_NOT_FOUND       
                                      //        RT_ERR_NOT_ENTITLED           
  )
{
	if(user_data)
	{
		CRTCore* pThis = reinterpret_cast<CRTCore*>(user_data);
		CAutoLock ss(pThis->m_csRequest);

		SStructureRequest* pRequest = pThis->GetRequest(req_id);
		if(!pRequest)
			err_code = RT_ERR_INVALID_REQ_ID; 

		if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hEventStopThread,0))
		{
			pThis->UnregisterRequest(req_id, NULL);
			return;
		}

		if(RT_ERR_OK == err_code)
		{
			int iHandle;
			int len = 0;
			rt_get_field_int32(data_buf, RT_SYMBOL_HANDLE, &iHandle);
			SYMBOLCASHE::iterator iter = pThis->m_SymbolsHandleCashe.find(pRequest->m_bsName);
			if(iter == pThis->m_SymbolsHandleCashe.end())
				pThis->m_SymbolsHandleCashe[pRequest->m_bsName] = iHandle;

			SResponse resp;

			if((*pRequest->m_Notify.begin())->GetClientType() == CBaseNotifier::enClientStructureProvider)
				resp.m_enType = SResponse::enStockStructure;

			if((*pRequest->m_Notify.begin())->GetClientType() == CBaseNotifier::enClientStructureInfo)
				resp.m_enType = SResponse::enStockInfo;

			resp.AssignResponseBuffer(data_length, data_buf);
			resp.m_bValue = false;
			resp.CopyNotifiers(pRequest->m_Notify);
			resp.m_vtRequest = pRequest->m_vtRequest;
			resp.m_reqID = req_id;

			if(pThis->AddResponse(resp))
				SetEvent(pThis->m_hEventNotify);

		}
		else
		{

			SResponse resp;
			CTrackDataError error = pThis->GetTrackDataError(err_code);
			resp.m_enType = SResponse::enError;
			resp.CopyNotifiers(pRequest->m_Notify);

			resp.m_vtRequest = pRequest->m_vtRequest;
			resp.m_reqID = req_id;
			resp.m_extError = error.m_enExtError;
			resp.m_bsMessage = error.m_szMessage;
			resp.m_requestType = enRequestStock;
			if(pThis->AddResponse(resp))
				SetEvent(pThis->m_hEventNotify);

		}
		pThis->UnregisterRequest(req_id, NULL);
	}
	return;
}
