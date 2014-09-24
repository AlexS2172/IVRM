#ifndef _MMW_CP_H_
#define _MMW_CP_H_

#define REQUEST_CP_IMPL(_object)										\
	HRESULT Fire_On##_object(I##_object* Object)						\
	{																	\
		CComVariant varResult;											\
		T* pT = static_cast<T*>(this);									\
		int nConnectionIndex;											\
		CComVariant* pvars = new CComVariant[1];						\
		int nConnections = m_vec.GetSize();								\
																		\
		for (nConnectionIndex = 0; nConnectionIndex < nConnections;		\
			nConnectionIndex++)											\
		{																\
			pT->Lock();													\
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);		\
			pT->Unlock();												\
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);	\
			if (pDispatch != NULL)										\
			{															\
				VariantClear(&varResult);								\
				pvars[0] = Object;										\
				DISPPARAMS disp = { pvars, NULL, 1, 0 };				\
				pDispatch->Invoke(DISPID_On##_object, IID_NULL,			\
					LOCALE_USER_DEFAULT, DISPATCH_METHOD,				\
					&disp, &varResult, NULL, NULL);						\
			}															\
		}																\
		delete[] pvars;													\
		return varResult.scode;											\
	}

#define REQUEST_CP_IMPL2(_method, _object)								\
	HRESULT Fire_On##_method(I##_object* Object)						\
	{																	\
		CComVariant varResult;											\
		T* pT = static_cast<T*>(this);									\
		int nConnectionIndex;											\
		CComVariant* pvars = new CComVariant[1];						\
		int nConnections = m_vec.GetSize();								\
																		\
		for (nConnectionIndex = 0; nConnectionIndex < nConnections;		\
			nConnectionIndex++)											\
		{																\
			pT->Lock();													\
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);		\
			pT->Unlock();												\
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);	\
			if (pDispatch != NULL)										\
			{															\
				VariantClear(&varResult);								\
				pvars[0] = Object;										\
				DISPPARAMS disp = { pvars, NULL, 1, 0 };				\
				pDispatch->Invoke(DISPID_On##_method, IID_NULL,			\
					LOCALE_USER_DEFAULT, DISPATCH_METHOD,				\
					&disp, &varResult, NULL, NULL);						\
			}															\
		}																\
		delete[] pvars;													\
		return varResult.scode;											\
	}

template <class T>
class CProxy_IOnMessages : public IConnectionPointImpl<T, &DIID__IOnMessages, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnVola(ISymbolObject* Symbol, IVMESurface* Data)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
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
				pvars[1] = Symbol;
				pvars[0] = Data;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(DISPID_OnVola, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}

	HRESULT Fire_OnActiveFuturesChange(IActiveFuturesChange* Data)
	{
		CComVariant varResult;
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
				pvars[0] = Data;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_OnActiveFuturesChange, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	}

	HRESULT Fire_OnFlexOption(IFlexOption* Data)
	{
		CComVariant varResult;
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
				pvars[0] = Data;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_OnFlexOption, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	}

	HRESULT Fire_OnTradeUpdate(ITradeUpdate* Data)
	{
		CComVariant varResult;
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
				pvars[0] = Data;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_OnTradeUpdate, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}
	HRESULT Fire_OnPriceUpdate(IPriceUpdate* Data)
	{
		CComVariant varResult;
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
				pvars[0] = Data;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_OnPriceUpdate, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;

	}
	HRESULT Fire_OnUnderlyingUpdate(IUnderlyingUpdate* Data)
	{
		CComVariant varResult;
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
				pvars[0] = Data;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_OnUnderlyingUpdate, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;

	}

	HRESULT Fire_OnManualPriceUpdate(IManualPriceUpdate* Data)
	{
		CComVariant varResult;
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
				pvars[0] = Data;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_OnManualPriceUpdate, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;

	}

	HRESULT Fire_OnSettingsUpdate(ISettingsUpdate* Data)
	{
		CComVariant varResult;
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
				pvars[0] = Data;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(DISPID_OnSettingsUpdate, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;

	}
	// events 
	HRESULT Fire_OnLogon(BSTR bsRouterName)
	{
		CComVariant varResult;
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
				pDispatch->Invoke(DISPID_OnLogon, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}
	HRESULT Fire_OnLogoff(BSTR bsRouterName)
	{
		CComVariant varResult;
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
				pDispatch->Invoke(DISPID_OnLogoff, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;
	
	}
	REQUEST_CP_IMPL(DynamicMessage)

	REQUEST_CP_IMPL(BroadcastMessage)

	REQUEST_CP_IMPL2(RequestNewOrder,	Order)
	REQUEST_CP_IMPL2(AlterOrder,		Order)
	REQUEST_CP_IMPL2(CancelOrder,		Order)
	REQUEST_CP_IMPL2(RequestOrderStatus,Order)
	REQUEST_CP_IMPL(Order)
	REQUEST_CP_IMPL(ExecutionReport)

	REQUEST_CP_IMPL(FixGatewayStatusRequest)
	REQUEST_CP_IMPL(FixGatewayStatus)
};
#endif