#ifndef _PROVIDERSCP_H_
#define _PROVIDERSCP_H_


template <class T>
class CProxy_FixProviderEvents : public IConnectionPointImpl<T, &DIID__OrderProviderEvents, CComDynamicUnkGITArray>
{
	//Warning this class may be recreated by the wizard.
public:
	VOID Fire_OnCancelReject(BSTR Reason, FoOrderCancelReject* Report)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr<IDispatch> pDispatch(sp);
			if (pDispatch != NULL)
			{
				pvars[1] = Reason;

				CComVariant vtRec;
				vtRec.vt = VT_RECORD | VT_BYREF;
				::GetRecordInfoFromGuids(LIBID_FixProvidersLibrary, 1, 0, ::GetUserDefaultLCID(), __uuidof(FoOrderCancelReject), &vtRec.pRecInfo);
				vtRec.pvRecord = (void*)Report;
				pvars[0] = vtRec;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(DISPID_FpOnCancelReject, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;

	}
	VOID Fire_OnExecutionReport(FoExecutionReport* Report)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr<IDispatch> pDispatch(sp);
			if (pDispatch != NULL)
			{
				CComVariant vtRec;
				vtRec.vt = VT_RECORD | VT_BYREF;
				::GetRecordInfoFromGuids(LIBID_FixProvidersLibrary, 1, 0, ::GetUserDefaultLCID(), __uuidof(FoExecutionReport), &vtRec.pRecInfo);
				vtRec.pvRecord = (void*)Report;
				pvars[0] = vtRec;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_FpOnExecReport, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;

	}
	VOID Fire_OnOrderError(BSTR OrderID, BSTR Description)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				pvars[1] = OrderID;
				pvars[0] = Description;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(DISPID_FpOnOrderError, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;

	}
	VOID Fire_OnDisconnect(long ProviderType)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				pvars[0] = ProviderType;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_FpOnDisconnect, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;
	}

	VOID Fire_OnReLogon(long ProviderType)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				pvars[0] = ProviderType;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_FpOnReLogon, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;
	}
	VOID Fire_OnLogonFailed(long ProviderType, BSTR Description)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			CComQIPtr< IDispatch > pDispatch( sp );
			if (pDispatch != NULL)
			{
				pvars[1] = ProviderType;
				pvars[0] = Description;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(DISPID_FpOnLogonFailed, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;

	}

};
#endif