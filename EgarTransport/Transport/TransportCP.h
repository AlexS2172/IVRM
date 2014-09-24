#ifndef _TRANSPORTCP_H_
#define _TRANSPORTCP_H_


template <class T>
class CProxy_ISessionEvents : public IConnectionPointImpl<T, &DIID__ISessionEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	static bool IsRPCError(HRESULT hr) {
		return FAILED(hr) && (0x800706BF == hr || 0x800706BE == hr || 0x800706BA == hr);
	}
private:
	void ClearBrokenLinks(DWORD dwCookie)
	{
		try
		{
			if(dwCookie>0 && dwCookie<=(DWORD)m_vec.GetSize())
				m_vec.Remove(dwCookie);
		}
		catch (...){}
	}
public:
/*	HRESULT Force_UnAdvise()
	{
		HRESULT hr = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				hr = CoDisconnectObject(pDispatch,0);
			}
		}		
		return hr;
	}*/

	HRESULT Fire_OnMessage(BSTR Subject, BSTR Message)
	{
		T* pT = static_cast<T*>(this);

		CComVariant varResult;
		CComVariant varSubject = Subject;
		VARIANT		varParams[2];
		DWORD       dwRPCErrorDeleteCookie = 0; 
		
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				
				varParams[1].vt = VT_BSTR;
				varParams[1].bstrVal = ::SysAllocStringLen(Subject, SysStringLen(Subject));

				varParams[0].vt = VT_BSTR;
				varParams[0].bstrVal = ::SysAllocStringLen(Message, SysStringLen(Message));
				
				DISPPARAMS disp = { varParams, NULL, 2, 0 };
				UINT iArgErr;
				HRESULT hRes = pDispatch->Invoke(DISPID_ISessionEventsOnMessage, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, &iArgErr);
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);

				SysFreeString(varParams[0].bstrVal);
				SysFreeString(varParams[1].bstrVal);
			}
		}
		ClearBrokenLinks(dwRPCErrorDeleteCookie);

		return varResult.scode;
	}
	HRESULT Fire_OnLogon(BSTR bsRouterName)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		
		int			nConnectionIndex;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsRouterName;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_ISessionEventsOnLogon, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		ClearBrokenLinks(dwRPCErrorDeleteCookie);

		delete[] pvars;
		return varResult.scode;
	
	}
	HRESULT Fire_OnLogoff(BSTR bsRouterName)
	{
		CComVariant varResult;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsRouterName;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_ISessionEventsOnLogoff, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		ClearBrokenLinks(dwRPCErrorDeleteCookie);

		delete[] pvars;
		return varResult.scode;
	
	}
	HRESULT Fire_OnSubjectStatus(BSTR Subject, unsigned long ulNumSubscribers)
	{
		T* pT = static_cast<T*>(this);

		CComVariant varResult;
		CComVariant varSubject = Subject;
		VARIANT		varParams[2];
		DWORD       dwRPCErrorDeleteCookie = 0; 
		
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
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
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);

				SysFreeString(varParams[1].bstrVal);
			}
		}

		ClearBrokenLinks(dwRPCErrorDeleteCookie);
		return varResult.scode;
	}
};

template <class T>
class CProxy_IRouterEvents : public IConnectionPointImpl<T, &DIID__IRouterEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	static bool IsRPCError(HRESULT hr) {
		return FAILED(hr) && (0x800706BF == hr || 0x800706BE == hr || 0x800706BA == hr);
	}
private:
	void ClearBrokenLinks(DWORD dwCookie)
	{
		try
		{
			if(dwCookie>0 && dwCookie<=(DWORD)m_vec.GetSize())
				m_vec.Remove(dwCookie);
		}
		catch (...){}
	}
public:
	HRESULT Fire_OnClientConnected(BSTR bsHostIP)
	{
		CComVariant varResult;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsHostIP;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_IRouterEventsOnClientConnected, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		delete[] pvars;
		ClearBrokenLinks(dwRPCErrorDeleteCookie);

		return varResult.scode;
	
	}
	HRESULT Fire_OnClientDisconnected(BSTR bsHostIP)
	{
		CComVariant varResult;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsHostIP;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_IRouterEventsOnClientDisconnected, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		delete[] pvars;
		ClearBrokenLinks(dwRPCErrorDeleteCookie);
		return varResult.scode;
	
	}
	HRESULT Fire_OnTransportStopped()
	{
		CComVariant varResult;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_IRouterEventsOnTransportStopped, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		ClearBrokenLinks(dwRPCErrorDeleteCookie);
		return varResult.scode;
	
	}
};
template <class T>
class CProxy_IHostEvents : public IConnectionPointImpl<T, &DIID__IHostEvents, CComDynamicUnkArray>
{
public:
	static bool IsRPCError(HRESULT hr) {
		return FAILED(hr) && (0x800706BF == hr || 0x800706BE == hr || 0x800706BA == hr);
	}
private:
	void ClearBrokenLinks(DWORD dwCookie)
	{
		try
		{
			if(dwCookie>0 && dwCookie<=(DWORD)m_vec.GetSize())
				m_vec.Remove(dwCookie);
		}
		catch (...){}
	}
public:
	//Warning this class may be recreated by the wizard.
	HRESULT Fire_OnLogoned(BSTR bsRouterName)
	{
		CComVariant varResult;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsRouterName;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_IHostEventsOnLogoned, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		delete[] pvars;
		ClearBrokenLinks(dwRPCErrorDeleteCookie);
		return varResult.scode;
	
	}
	HRESULT Fire_OnLogout(BSTR bsRouterName)
	{
		CComVariant varResult;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = bsRouterName;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_IHostEventsOnLogout, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);

				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		delete[] pvars;

		ClearBrokenLinks(dwRPCErrorDeleteCookie);
		return varResult.scode;
	
	}

	HRESULT Fire_OnDisabled()
	{
		CComVariant varResult;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_IHostEventsOnDisabled, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);

				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		ClearBrokenLinks(dwRPCErrorDeleteCookie);
		return varResult.scode;
	
	}
	HRESULT Fire_OnTransportStopped()
	{
		CComVariant varResult;
		DWORD       dwRPCErrorDeleteCookie = 0; 

		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				HRESULT hRes = pDispatch->Invoke(DISPID_IHostEventsOnTransportStopped, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				if(IsRPCError(hRes))
					dwRPCErrorDeleteCookie = m_vec.GetCookie(&sp.p);
			}
		}
		ClearBrokenLinks(dwRPCErrorDeleteCookie);
		return varResult.scode;
	
	}
	

};
#endif