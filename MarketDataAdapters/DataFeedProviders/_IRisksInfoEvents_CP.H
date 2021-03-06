// Wizard-generated connection point proxy class
// WARNING: This file may be regenerated by the wizard


#pragma once

template<class T>
class CProxy_IRisksInfoEvents :
	public IConnectionPointImpl<T, &__uuidof(_IRisksInfoEvents)>
{
public:
	HRESULT Fire_OnRisksUpdate()
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant varResult;

				DISPPARAMS params = { NULL, NULL, 0, 0 };
				hr = pConnection->Invoke(0x6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}

	HRESULT Fire_OnRisksSubscribed(ITicker * Params)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[1];

				_Ticker rp;
				rp.Init();
				rp->Exchange = _bstr_t("").copy();
				rp->Symbol = _bstr_t(Params->Symbol).copy();
				rp->Type = Params->Type;
				rp.CopyTo( avarParams[0] );

				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 1, 0 };

				hr = pConnection->Invoke(0x7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
};