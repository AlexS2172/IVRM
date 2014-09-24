#pragma once

template<class T>
class CProxy_IMmRiskViewEvents :
	public IConnectionPointImpl<T, &__uuidof(_IMmRiskViewEvents)>
{
public:
	HRESULT Fire_Progress(BSTR  bsDescription, DOUBLE  Precent)
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
				CComVariant avarParams[2];
				avarParams[1] = bsDescription;	avarParams[1].vt = VT_BSTR;
				avarParams[0] = Precent;	avarParams[0].vt = VT_R8;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
};

