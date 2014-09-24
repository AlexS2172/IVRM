#ifndef _EGTPPRXCP_H_
#define _EGTPPRXCP_H_

template <class T>
class CProxy_ISessionEvents : public IConnectionPointImpl<T, &DIID__ISessionEvents, CComDynamicUnkGITArray>
{
	CLock m_lck;
	//Warning this class may be recreated by the wizard.
	inline void __lock(){
		//DWORD   threadID = GetCurrentThreadId();
		bool	firstTime = true;
		while(!m_lck.TryToLock() ){
			if ( firstTime ){
				firstTime = false;
				//ATLTRACE("++++ Obj %x waiting in thread %d ++++\n", this, threadID );

			}
			//Sleep(0); 
			MSG msg;
			HACCEL haccel = NULL;
			HWND currWnd = ::GetActiveWindow();

			if (GetMessage(&msg, 0, 0, 0) != 0 ) {
				if (!IsDialogMessage(currWnd, &msg) && !TranslateAccelerator(currWnd, haccel, &msg)) { 
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		//ATLTRACE("++++ Obj %x locked in thread %d ++++\n", this, threadID );
	}
	inline void __unlock(){
		//DWORD   threadID = GetCurrentThreadId();
		m_lck.Unlock();
		//ATLTRACE("++++ Obj %x unlocked in thread %d ++++\n", this, threadID );
	}

public:
	HRESULT Force_UnAdvise()
	{
		HRESULT hr = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		
		__lock();
		int nConnections = m_vec.GetSize();
		__unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			__lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			__unlock();
			
			CComQIPtr< IDispatch > pDispatch( sp );
//			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				hr = CoDisconnectObject(pDispatch,0);
			}
		}		
		return hr;
	}

	HRESULT Fire_OnMessage(BSTR Subject, BSTR Message)
	{

		
		//DWORD threadID = GetCurrentThreadId();
		//ATLTRACE("Fire_OnMessage Enter(%d)\n", threadID);
		//CAutoLock lck(m_lck);

		T* pT = static_cast<T*>(this);

		CComVariant varResult;
		VARIANT		varParams[2];
		
		int nConnectionIndex;

		
		__lock();
		int nConnections = m_vec.GetSize();
		__unlock();
		
		

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			//threadID = GetCurrentThreadId();
			//ATLTRACE("Lock OnMessage Enter(%d) addr(%x)\n", threadID, pT);
			__lock();
			//ATLTRACE("GetAt OnMessage Enter(%d)\n", threadID);
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			//ATLTRACE("GetAt OnMessage Exit(%d)\n", threadID);

			//threadID = GetCurrentThreadId();
			__unlock();
			//ATLTRACE("UnLock OnMessage Exit(%d) addr(%x)\n", threadID, pT);

			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				
				varParams[1].vt = VT_BSTR;
				varParams[1].vt = VT_BSTR;
				varParams[1].bstrVal = ::SysAllocStringLen(Subject, SysStringLen(Subject));

				varParams[0].vt = VT_BSTR;
				varParams[0].bstrVal = ::SysAllocStringLen(Message, SysStringLen(Message));


				DISPPARAMS disp = { varParams, NULL, 2, 0 };
				UINT iArgErr;
				HRESULT hRes = pDispatch->Invoke(DISPID_ISessionEventsOnMessage, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, &iArgErr);

				SysFreeString(varParams[0].bstrVal);
				SysFreeString(varParams[1].bstrVal);
			}
		}
		//ATLTRACE("Fire_OnMessage Exit(%d)\n", threadID);
		return varResult.scode;
	}
	HRESULT Fire_OnLogoff(BSTR bsRouterName)
	{
		//return S_OK;
		
		//DWORD threadID = GetCurrentThreadId();
		//ATLTRACE("Fire_OnLogOff Enter(%d)\n", threadID);
		//CAutoLock lck(m_lck);

		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		VARIANT		varParams[1];
		
		__lock();
		int nConnections = m_vec.GetSize();
		__unlock();
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			//threadID = GetCurrentThreadId();
			//ATLTRACE("Lock OnLogOff Enter(%d) addr(%x)\n", threadID, pT);
			__lock();
			//ATLTRACE("GetAt OnLogOff Enter(%d)\n", threadID);
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			//ATLTRACE("GetAt OnLogOff Exit(%d)\n", threadID);

			//threadID = GetCurrentThreadId();
			__unlock();
			//ATLTRACE("UnLock OnLogOff Exit(%d) addr(%x)\n", threadID, pT);

			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				
				varParams[0].vt = VT_BSTR;
				varParams[0].bstrVal = ::SysAllocStringLen(bsRouterName, SysStringLen(bsRouterName));

				DISPPARAMS disp = { varParams, NULL, 1, 0 };
				UINT iArgErr;
				pDispatch->Invoke(DISPID_ISessionEventsOnLogoff, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, &iArgErr);

				SysFreeString(varParams[0].bstrVal);
			}
		}
		//ATLTRACE("Fire_OnLogOff Exit(%d)\n", threadID);
		return varResult.scode;
	}
	HRESULT Fire_OnLogon(BSTR bsRouterName)
	{

		//DWORD threadID = GetCurrentThreadId();
		//ATLTRACE("Fire_OnLogOn Enter(%d)\n", threadID);
		//CAutoLock lck(m_lck);

		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		VARIANT		varParams[1];
		
		__lock();
		int nConnections = m_vec.GetSize();
		__unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			//threadID = GetCurrentThreadId();
			//ATLTRACE("Lock OnLogOn Enter(%d) addr(%x)\n", threadID, pT);
			__lock();

			//ATLTRACE(_T("GetAt OnLogOn Enter\n"));
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			//ATLTRACE(_T("GetAt OnLogOn Exit\n"));

			//threadID = GetCurrentThreadId();
			__unlock();
			//ATLTRACE("UnLock OnLogOn Exit(%d) addr(%x)\n", threadID, pT);

			CComQIPtr< IDispatch > pDispatch( sp );

			//ATLTRACE("OnLogOn CComQIPtr< IDispatch > pDispatch( sp )\n");

			if (pDispatch != NULL)
			{
				VariantClear(&varResult);

				varParams[0].vt = VT_BSTR;
				varParams[0].bstrVal = ::SysAllocStringLen(bsRouterName, SysStringLen(bsRouterName));

				DISPPARAMS disp = { varParams, NULL, 1, 0 };
				UINT iArgErr;
				pDispatch->Invoke(DISPID_ISessionEventsOnLogon, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, &iArgErr);

				SysFreeString(varParams[0].bstrVal);
			}
		}
		//ATLTRACE(_T("Fire_OnLogOn Exit\n"));
		return varResult.scode;
	
	}
	HRESULT Fire_OnSubjectStatus(BSTR Subject, unsigned long ulNumSubscribers)
	{
		T* pT = static_cast<T*>(this);

		CComVariant varResult;
		VARIANT		varParams[2];
		
		int nConnectionIndex;
		
		__lock();
		int nConnections = m_vec.GetSize();
		__unlock();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			__lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			__unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				
				varParams[1].vt = VT_BSTR;
				varParams[1].bstrVal = ::SysAllocStringLen(Subject, SysStringLen(Subject));

				varParams[0].vt = VT_UI4;
				varParams[0].ulVal = ulNumSubscribers;
				
				DISPPARAMS disp = { varParams, NULL, 2, 0 };
				UINT iArgErr;
				HRESULT hRes = pDispatch->Invoke(DISPID_ISessionEventsOnSubjectStatus, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, &iArgErr);

				SysFreeString(varParams[1].bstrVal);
			}
		}

		return varResult.scode;
	}
};

template <class T>
class CProxy_IRouterEvents : public IConnectionPointImpl<T, &DIID__IRouterProxyEvents/*, CComDynamicUnkGITArray*/>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Force_UnAdvise()
	{
		HRESULT hr = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				hr = CoDisconnectObject(pDispatch,0);
			}
		}		
		return hr;
	}

	HRESULT Fire_OnServiceStatus(long Status)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = Status;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_IRouterProxyEventsOnServiceStatus, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;	
	}

	HRESULT Fire_OnServiceError(long Code, BSTR Description)
	{
		T* pT = static_cast<T*>(this);

		CComVariant varResult;
		VARIANT		varParams[2];
		
		int nConnectionIndex;
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				
				varParams[1].vt = VT_I4;
				varParams[1].lVal = Code;

				varParams[0].vt = VT_BSTR;
				varParams[0].bstrVal = ::SysAllocStringLen(Description, SysStringLen(Description));
				
				DISPPARAMS disp = { varParams, NULL, 2, 0 };
				UINT iArgErr;
				HRESULT hRes = pDispatch->Invoke(DISPID_IRouterProxyEventsOnServiceError, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, &iArgErr);

				SysFreeString(varParams[0].bstrVal);
			}
		}

		return varResult.scode;
	}
	
	HRESULT Fire_OnClientConnected(BSTR bsHostIP)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsHostIP;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_IRouterProxyEventsOnClientConnected, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}

	HRESULT Fire_OnClientDisconnected(BSTR bsHostIP)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsHostIP;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_IRouterProxyEventsOnClientDisconnected, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}
};
template <class T>
class CProxy_IHostEvents : public IConnectionPointImpl<T, &DIID__IHostProxyEvents/*, CComDynamicUnkGITArray*/>
{
public:
	HRESULT Force_UnAdvise()
	{
		HRESULT hr = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				hr = CoDisconnectObject(pDispatch,0);
			}
		}		
		return hr;
	}
	HRESULT Fire_OnServiceStatus(long Status)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = Status;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_IHostProxyEventsOnServiceStatus, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;	
	}

	HRESULT Fire_OnServiceError(long Code, BSTR Description)
	{
		T* pT = static_cast<T*>(this);

		CComVariant varResult;
		VARIANT		varParams[2];
		
		int nConnectionIndex;
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				
				varParams[1].vt = VT_I4;
				varParams[1].lVal = Code;

				varParams[0].vt = VT_BSTR;
				varParams[0].bstrVal = ::SysAllocStringLen(Description, SysStringLen(Description));
				
				DISPPARAMS disp = { varParams, NULL, 2, 0 };
				UINT iArgErr;
				HRESULT hRes = pDispatch->Invoke(DISPID_IHostProxyEventsOnServiceError, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, &iArgErr);

				SysFreeString(varParams[0].bstrVal);
			}
		}

		return varResult.scode;
	}
	//Warning this class may be recreated by the wizard.
	HRESULT Fire_OnLogoned(BSTR bsRouterName)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsRouterName;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_IHostProxyEventsOnLogoned, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}
	HRESULT Fire_OnLogout(BSTR bsRouterName)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsRouterName;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_IHostProxyEventsOnLogout, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}

	HRESULT Fire_OnDisabled()
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		
		//pT->Lock();
		int nConnections = m_vec.GetSize();
		//pT->Unlock();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				pDispatch->Invoke(DISPID_IHostProxyEventsOnDisabled, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		return varResult.scode;
	
	}
};
#endif