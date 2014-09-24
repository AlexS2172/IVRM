// BaseNotifier.h: interface for the CBaseNotifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASENOTIFIER_H__E8254BA0_E0A2_4B36_9D30_77B8084B03A0__INCLUDED_)
#define AFX_BASENOTIFIER_H__E8254BA0_E0A2_4B36_9D30_77B8084B03A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyTrackProviders.h"
#include "common.h"
#include <vector>

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
			enClientPriceInfoWithNotify
		};

	virtual HRESULT OnStock(CComVariant &varParams, CComVariant &varResults) {return S_OK;};
	virtual HRESULT OnOption(CComVariant &varParams, CComVariant &varResults, CComVariant &varIsLastOption) {return S_OK;};
	virtual HRESULT OnLastQuote(CComVariant &varParams, CComVariant &varResults) {return S_OK;};
	virtual HRESULT OnQuoteUpdate(CComVariant &varParams, CComVariant &varResults) {return S_OK;};
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, CComVariant &varRequest) {return S_OK;};
	virtual HRESULT OnSubscribed(CComVariant &varParams) {return S_OK;};

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

#endif // !defined(AFX_BASENOTIFIER_H__E8254BA0_E0A2_4B36_9D30_77B8084B03A0__INCLUDED_)
