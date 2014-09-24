// RTCore.h: interface for the CRTCore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTCORE_H__40074616_03CA_4641_BA7C_BF16AD3F2275__INCLUDED_)
#define AFX_RTCORE_H__40074616_03CA_4641_BA7C_BF16AD3F2275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct CTrackDataError
{
	int				m_iError;
	LPCTSTR			m_szMessage;
	ErrorNumberEnum	m_enExtError;
};
struct SStructureRequest
{
	NOTIFIERS		m_Notify;
	_bstr_t			m_bsName;
	CComVariant     m_vtRequest;
	bool            m_bValue;

	SStructureRequest()
	{
		m_Notify.clear();
		m_bValue = false;
	}
	SStructureRequest& operator=(const SStructureRequest& cm)
	{
		CopyNotifiers(cm.m_Notify);
		m_bsName = cm.m_bsName;
		m_vtRequest = cm.m_vtRequest;
		m_bValue = cm.m_bValue;
		return *this;

	}
	void AddNotifyer(CBaseNotifier* pNotify)
	{
			m_Notify.insert(pNotify);
	}
	void CopyNotifiers(const NOTIFIERS& notify)
	{
		m_Notify.clear();
		for(NOTIFIERS::const_iterator iter = notify.begin(); iter!=notify.end(); iter++)
		{
			m_Notify.insert(*iter);
		}
	}
};

//*****************************************************************************************
typedef map<RT_REQ_ID, SStructureRequest*> REQUESTDATA;
typedef map<_bstr_t, RT_HANDLE> SYMBOLCASHE;
typedef map<RT_REQ_ID, SStructureRequest*> REQUESTDATA;
//*******************************************************************************************
class CRTCore  
{
public:
	bool Connect( CBaseNotifier* pNotify );
	bool Disconnect(CBaseNotifier* pNotify );

	CRTCore();
	virtual ~CRTCore();

	RT_ERR  RequestGroup( RT_REQ_ID* req_id_ptr_to_fill, RT_GRP_REQ_TY group_type, RT_HANDLE  handle, RT_GET_GROUP_CLBK callback)
	{
		return rt_request_group(m_rtConnectID, req_id_ptr_to_fill, this, group_type, handle, callback);
	}

	RT_ERR RequestFieldsViaHandle(RT_REQ_ID* req_id_ptr_to_fill,  RT_REQ_TYPE  req_type, RT_HANDLE handle, int field_count, RT_FIELD* fields, RT_GET_FIELDS_CLBK callback )
	{
		return rt_request_fields_via_handle(m_rtConnectID, req_id_ptr_to_fill, this, req_type, handle, field_count, fields, callback);
	}

	RT_ERR RequestFields(RT_REQ_ID* req_id_ptr_to_fill , RT_REQ_TYPE req_type, RT_SYMBOL* symbol, int field_count,RT_FIELD* fields , RT_GET_FIELDS_CLBK callback  )
	{
		return rt_request_fields(m_rtConnectID, req_id_ptr_to_fill, this, req_type, symbol, field_count,fields, callback );
	}

	RT_ERR  RemoveFields( RT_REQ_ID req_id, int field_count, RT_FIELD* fields)
	{
		return rt_remove_fields(m_rtConnectID, req_id, field_count, fields);
	}

	//getting registry information
	void _Connect()
	{
		if(!m_tagLoginInfo.Load()) 
				CCommonException::ThrowError (E_FAIL,L"Loading registry settings failed");
		if(!m_tagLoginInfo.m_strUserName.length())
			CCommonException::ThrowError (E_FAIL,L"The Price Provider Account could not be empty");
		if(!m_tagLoginInfo.m_strUserPass.length())
			CCommonException::ThrowError (E_FAIL,L"The Price Provider Password could not be empty");

		rt_startup((LPSTR)m_tagLoginInfo.m_strUserName, (LPSTR)m_tagLoginInfo.m_strUserPass, &m_rtConnectID, rtStartUp);
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hEventStartUp,m_tagLoginInfo.m_iLogonTimeout*1000))
		{
			if(RT_ERR_OK == m_rtErrorCode)
			{
				rt_set_shutdown_callback(m_rtConnectID,rtShutDown);
				return;
			}		
			else
				throw(GetTrackDataError(m_rtErrorCode));
		}
		CCommonException::ThrowError (E_FAIL,L"Logon timeout.");
	}

	void FindAndAddNotifier(CBaseNotifier *pNotifier)
	{
		CNotifierFind finder(pNotifier);
		CAutoLock cs(m_csClients);

		CLIENTS::iterator it = std::find_if (m_Clients.begin (),m_Clients.end(),finder);

		if (it==m_Clients.end ())
			m_Clients.push_back(pNotifier);
	}

	void RegisterRequest(RT_REQ_ID ID, SStructureRequest* pRequest)
	{
		CAutoLock cs(m_csRequest);
		REQUESTDATA::iterator iter = m_Request_pData.find(ID);
		if(iter != m_Request_pData.end())
		{
			for(NOTIFIERS::iterator itNotify = pRequest->m_Notify.begin(); itNotify!=pRequest->m_Notify.end();itNotify++)
			{
				iter->second->AddNotifyer(*itNotify);			
			}
			delete pRequest;
		}
		else
			m_Request_pData[ID] = pRequest;
	}
	bool UnregisterRequest(RT_REQ_ID ID, NOTIFIERS& Data)
	{
		CAutoLock cs(m_csRequest);
		REQUESTDATA::iterator iter = m_Request_pData.find(ID);
		if(iter != m_Request_pData.end())
		{
			try
			{
				for(NOTIFIERS::iterator itNotifiers = Data.begin();itNotifiers!=Data.end();itNotifiers++)
				{
					NOTIFIERS::iterator itNotify = iter->second->m_Notify.find(*itNotifiers);
					if(itNotify != iter->second->m_Notify.end())
						iter->second->m_Notify.erase(itNotify);
					if(iter->second->m_Notify.empty())
					{
						delete iter->second;
						m_Request_pData.erase(iter);
						return true;
					}

				}
			}
			catch(...)
			{

			}
		}
		return false;
	}

	bool UnregisterRequest(RT_REQ_ID ID, CBaseNotifier* pData)
	{
		CAutoLock cs(m_csRequest);
		REQUESTDATA::iterator iter = m_Request_pData.find(ID);
		if(iter != m_Request_pData.end())
		{
			if(pData)
			{
				NOTIFIERS::iterator itNotify = iter->second->m_Notify.find(pData);
				if(itNotify != iter->second->m_Notify.end())
					iter->second->m_Notify.erase(itNotify);
				if(iter->second->m_Notify.empty())
				{
					delete iter->second;
					m_Request_pData.erase(iter);
					return true;
				}
			}
			else
			{
				delete iter->second;
				m_Request_pData.erase(iter);
				return true;
			}
		}
		return false;
	}
	SStructureRequest* GetRequest(RT_REQ_ID ID)
	{
		CAutoLock cs(m_csRequest);
		REQUESTDATA::iterator iter = m_Request_pData.find(ID);
		if(iter != m_Request_pData.end())
			return iter->second;
		else
			return NULL;
	}

	bool AddResponse(SResponse& resp)
	{
		bool bIsLazy = false;
		CAutoLock s(m_cSResponse);

		if(resp.m_enType == SResponse::enRealtime)
		{
			for(NOTIFIERS::iterator itNotify = resp.m_Notify.begin();itNotify!=resp.m_Notify.end();itNotify++)
			{
				if(IsBadCodePtr((FARPROC)(*itNotify)))
					continue;

				if((*itNotify)->GetResponseType() == CBaseNotifier::enResponseLazy)
				{
					bIsLazy = true;
					break;
				}
			}
		}
		if(bIsLazy)
		{
			REQIDs::iterator iter = m_ReqIDs.find(resp.m_reqID);
			if(iter!=m_ReqIDs.end())
			{
				RESPONCE::iterator itResp;

				for(itResp = m_Responce.begin(); itResp!= m_Responce.end(); itResp++)
				{
					if(resp.m_reqID == itResp->m_reqID)
					{
						long startTick = itResp->m_CreationTick;
						*itResp = resp;
						itResp->m_CreationTick = startTick;
						break;
					}
				}
				iter->second++;
				return false;
			}
			m_ReqIDs[resp.m_reqID] = 0;
		}

		m_Responce.insert(m_Responce.end(), resp);
		return true;
	}

	bool ISConnected(){return m_bIsConnected; }
	CTrackDataError	GetTrackDataError(RT_ERR err);
	REQUEST							m_LtRequest;
	REQUEST							m_RtRequest;

//protected:
	static UINT WINAPI EventsSender(LPVOID pData);

	static const CTrackDataError    m_TrackDataErrors[];

	CLIENTS					 m_Clients;
	REQUESTDATA              m_Request_pData;
	SYMBOLCASHE				 m_SymbolsHandleCashe;
	RESPONCE				 m_Responce;
	REQIDs                   m_ReqIDs;

	CLock						m_cSResponse;
	CLock						m_csClients;

	bool					 m_bIsConnected;
	
	HANDLE					 m_hEventsSender;
	HANDLE					 m_hEventNotify;

	static HANDLE					m_hEventStartUp;
	static HANDLE					m_hEventDisconnected;
	static RT_ERR					m_rtErrorCode;

	static void rtShutDown(RT_CONNECT_ID rt_id, RT_ERR err_code);
	static void rtStartUp(RT_CONNECT_ID rt_id, RT_ERR err_code);
	static void rtGetSymbol(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_REQ_TYPE req_type, int field_count, RT_FIELD* fields, int data_length, RT_RAW_FIELD_BUF* data_buf, RT_ERR err_code);
	static void rtGetQuote(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_REQ_TYPE req_type, int field_count, RT_FIELD* fields, int data_length, RT_RAW_FIELD_BUF* data_buf, RT_ERR err_code);

	static void rtGetOptionsIDs(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_GRP_REQ_TY group_type, RT_HANDLE underlier, int tot_num, int cur_offset, int num_received, RT_HANDLE* handle_list, RT_ERR err_code); // group
	static void rtGetSymbol_Options(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_REQ_TYPE req_type, int field_count, RT_FIELD* fields, int data_length, RT_RAW_FIELD_BUF* data_buf, RT_ERR err_code);
	static void rtGetOption(RT_CONNECT_ID rt_id, RT_REQ_ID req_id, void* user_data, RT_REQ_TYPE req_type, int field_count, RT_FIELD* fields, int data_length, RT_RAW_FIELD_BUF* data_buf, RT_ERR err_code);


	HANDLE	m_hEventStopThread;

	bool ParseOptionInfo(RT_RAW_FIELD_BUF* pBuffer, CComVariant& vtResp);
	bool ParseOptionStructure(RT_RAW_FIELD_BUF* pBuffer, CComVariant& vtResp);

	bool ParseSymbolInfo(RT_RAW_FIELD_BUF* pBuffer, CComVariant& vtResp);
	bool ParseSymbolStructure(RT_RAW_FIELD_BUF* pBuffer, CComVariant& vtResp);

	bool ParseQuoteInfo(const SResponse& resp, long lSkippesSize, long lSizeQueue, RT_RAW_FIELD_BUF* pBuffer, CComVariant& vtResp);
	bool ParseQuote(const SResponse& resp, RT_RAW_FIELD_BUF* pBuffer, CComVariant& vtResp);
	
public:
	HANDLE       m_hEventCancelOptions;
	HANDLE       m_hEventCancelStocks;

	CLock		 m_csRequest;
	CLock        m_csNotify;

	static RT_CONNECT_ID            m_rtConnectID;
	CLoginInfo				m_tagLoginInfo;

};

#endif // !defined(AFX_RTCORE_H__40074616_03CA_4641_BA7C_BF16AD3F2275__INCLUDED_)
