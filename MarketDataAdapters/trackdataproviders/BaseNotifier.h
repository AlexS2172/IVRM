// BaseNotifier.h: interface for the CBaseNotifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASENOTIFIER_H__E8254BA0_E0A2_4B36_9D30_77B8084B03A0__INCLUDED_)
#define AFX_BASENOTIFIER_H__E8254BA0_E0A2_4B36_9D30_77B8084B03A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrackDataProviders.h"
#include "Common.h"

class CBaseNotifier
{
	public:		
		typedef enum ResponseType
		{
			enResponseRegular,
            enResponseLazy

		};
		typedef enum ClientType 
		{
			enClientUnknown,
			enClientPriceProvider,
			enClientPriceInfo,
            enClientStructureProvider,
			enClientStructureInfo,
			enLastQuote,
			enQuoteUpdate
		};

	virtual HRESULT OnStock(CComVariant &varParams, CComVariant &varResults) {return S_OK;};
	virtual HRESULT OnOption(CComVariant &varParams, CComVariant &varResults, CComVariant &varIsLastOption) {return S_OK;};
	virtual HRESULT OnLastQuote(CComVariant &varParams, CComVariant &varResults) {return S_OK;};
	virtual HRESULT OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults) {return S_OK;};
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, CComVariant &varRequest) {return S_OK;};

	virtual ClientType GetClientType(){return 	enClientUnknown;};
	virtual ResponseType GetResponseType(){return enResponseRegular;	}

	static void WaitObject(HANDLE &hObject)
	{
		if (hObject==INVALID_HANDLE_VALUE)
			return;

		::WaitForSingleObject (hObject,INFINITE);
	};
	static DWORD WaitWithEvents(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE, BOOL bMode = FALSE)
	{
		DWORD dwStatus = -1;
		ATLASSERT(nCount > 0);
		for(;;)
		{
			dwStatus = ::MsgWaitForMultipleObjects(nCount,pHandles,bMode,dwMilliseconds,QS_ALLEVENTS);
			
			if(dwStatus != WAIT_OBJECT_0+nCount) break;
			
			MSG msg;
			while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return dwStatus;		
	}
	static void WaitWithEvents(HANDLE &hObject)
	{
		if (hObject==INVALID_HANDLE_VALUE)
			return;
		MSG msg;	
		while(1)
		{
			DWORD dwRet = MsgWaitForMultipleObjects( 1,&hObject,FALSE,INFINITE,QS_ALLEVENTS);
			if(dwRet == WAIT_OBJECT_0)
				return;
			else 
			if(dwRet == WAIT_OBJECT_0 + 1)
			{
				while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			} 
			else
				return;
		}
	};

};

//*************************************************************************************
typedef set<CBaseNotifier*> NOTIFIERS;
typedef list<CBaseNotifier*> CLIENTS;
//*************************************************************************************

class CNotifierFind
{
public:
	CNotifierFind(CBaseNotifier* pNotifier)
	{
		m_pNotifier = pNotifier;
	}
	bool operator()(CBaseNotifier* pNotifier)
	{
		return memcmp(pNotifier,m_pNotifier,sizeof(CBaseNotifier))==0;
	}	

protected:
	CBaseNotifier*		m_pNotifier;
	
};

struct SResponse
{
	SResponse()
	{
		m_pResponseBuffer = NULL;
		m_lenResponse = 0;
		m_reqID = 0;

		m_Notify.clear();
		m_bValue = false;
		m_enType = enNone;
		m_CreationTick = GetTickCount();
		m_extError = enUnknown;
		m_requestType = enNoRequest;

	}
	~SResponse()
	{
		if(m_pResponseBuffer)
			delete m_pResponseBuffer;
	}

	SResponse& operator=(const SResponse& cn)
	{
		m_vtRequest = cn.m_vtRequest;

		CopyNotifiers(cn.m_Notify);
		m_bValue = cn.m_bValue;
		m_enType = cn.m_enType;
		m_CreationTick = cn.m_CreationTick;
		m_reqID = cn.m_reqID;
		m_extError = cn.m_extError;
		m_bsMessage = cn.m_bsMessage;
		m_requestType = cn.m_requestType;

		AssignResponseBuffer(cn.m_lenResponse, cn.m_pResponseBuffer);
		return *this;
	}

	void AssignResponseBuffer(long len, RT_RAW_FIELD_BUF* pBuf)
	{
		if(m_pResponseBuffer)
		{
			delete m_pResponseBuffer;
			m_pResponseBuffer = NULL;
			m_lenResponse = 0;
		}
		if(len)
		{
			m_pResponseBuffer = new RT_RAW_FIELD_BUF[len];
			m_lenResponse = len;
			memcpy(m_pResponseBuffer, pBuf, len);
		}
	}

	SResponse(const SResponse& cn)
	{
		m_vtRequest = cn.m_vtRequest;

		CopyNotifiers(cn.m_Notify);
		m_bValue = cn.m_bValue;
		m_enType = cn.m_enType;
		m_CreationTick = cn.m_CreationTick;
		m_pResponseBuffer = NULL;
		m_lenResponse = 0;
		m_reqID = cn.m_reqID;

		m_extError = cn.m_extError;
		m_bsMessage = cn.m_bsMessage;
		m_requestType = cn.m_requestType;


		AssignResponseBuffer(cn.m_lenResponse, cn.m_pResponseBuffer);
	}

	void CopyNotifiers(const NOTIFIERS& notify)
	{
		m_Notify.clear();
		for(NOTIFIERS::const_iterator iter = notify.begin(); iter!=notify.end(); iter++)
		{
			m_Notify.insert(*iter);
		}
	}

	CComVariant m_vtRequest;

	long			  m_lenResponse;
	RT_REQ_ID		  m_reqID;
	RT_RAW_FIELD_BUF* m_pResponseBuffer;

	NOTIFIERS	   m_Notify;
	bool           m_bValue;
	DWORD		   m_CreationTick;

	ErrorNumberEnum  m_extError;
	_bstr_t		     m_bsMessage;
	RequestsTypeEnum m_requestType;

	enum  enType {enNone,
				  enStockStructure, 
				  enOptionStructure, 
				  enStockInfo, 
				  enOptionInfo, 
				  enLastQuote, 
				  enRealtime,
				  enError} m_enType;

};

typedef vector<SResponse>  RESPONCE;
typedef map<_bstr_t, RT_REQ_ID>  REQUEST;
typedef map<RT_REQ_ID, long> REQIDs;

#endif // !defined(AFX_BASENOTIFIER_H__E8254BA0_E0A2_4B36_9D30_77B8084B03A0__INCLUDED_)
