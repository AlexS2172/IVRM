// ActivFeedStructureInfo.cpp : Implementation of CActivFeedStructureInfo

#include "stdafx.h"
#include "ActivFeedStructureInfo.h"

STDMETHODIMP	CActivFeedStructureInfo::CancelFuture(VARIANT Params)
{
	if( m_spActiv )
		m_spActiv->CancelFuture(Params);
	return S_OK;
}
STDMETHODIMP	CActivFeedStructureInfo::CancelFutures(VARIANT Params)
{
	if( m_spActiv )
		m_spActiv->CancelFutures(Params);
	return S_OK;
}
STDMETHODIMP	CActivFeedStructureInfo::CancelFuturesOptions(VARIANT Params)
{
	if( m_spActiv )
		m_spActiv->CancelFuturesOptions(Params);
	return S_OK;
}


STDMETHODIMP	CActivFeedStructureInfo::CancelOptions(VARIANT Params)
{
	if( m_spActiv )
		m_spActiv->CancelOptions(Params);
	return S_OK;
}
STDMETHODIMP	CActivFeedStructureInfo::CancelStock(VARIANT Params)
{
	if( m_spActiv )
		m_spActiv->CancelStock(Params);
	return S_OK;
}
STDMETHODIMP	CActivFeedStructureInfo::Connect()
{
	if(m_spActiv == NULL)
	{
		m_spActiv =  CActivFeedAppicationPtr(new CActivFeedAppication(this, m_Settings));
		try{
			if( m_spActiv!=NULL && m_spActiv->Connect() )
				return S_OK;
		}
		catch( _com_error& e) {
			return e.Error();
		}
	}
	return S_OK;
}

STDMETHODIMP	CActivFeedStructureInfo::Disconnect()
{
	if(m_spActiv != NULL)
		m_spActiv.reset();
	return S_OK;
}

HRESULT			CActivFeedStructureInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
	EgLib::CEgLibTraceManager::TraceText(LogDebug, __FUNCTION__ , _T("Enter"));
	HRESULT hr = S_OK;
	CComVariant varResult;

	try
	{

		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[4];
		int nConnections = m_vec.GetSize();


		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[3] = enumError;
				pvars[2] = (BSTR)bstrDescription;
				pvars[1] = enumRequest;
				pvars[0] = varRequest;
				DISPPARAMS disp = { pvars, NULL, 4, 0 };
				hr = pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;

		if(FAILED(hr))
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Exit with error hr = %d, file: %s line: %d"),  hr, __FILE__, __LINE__);
			hr = E_FAIL;
		}

	}
	_CATCH_COM_EXCEPTION_RETURN_COM(L"On Error Failed")
	_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgLib::CEgLibTraceManager::TraceText(LogDebug, __FUNCTION__ , _T("Exit"));
	if (hr == E_FAIL)
		return E_FAIL;
	else
		return varResult.scode;
}


HRESULT			CActivFeedStructureInfo::OnFuture(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CActivFeedStructureInfo::OnFuture\n"));

	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[1] = varParams;
			pvars[0] = varResults;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT			CActivFeedStructureInfo::OnFutureByRoot(const CComVariant &varParams,const CComVariant &varResults, const CComVariant &varIsLastFutures)
{
	ATLTRACE(_T("CActivFeedStructureInfo::OnFutureByRoot\n"));

	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[2] = varParams;
			pvars[1] = varResults;
			pvars[0] = varIsLastFutures;			
			DISPPARAMS disp = { pvars, NULL, 3, 0 };
			hr = pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT			CActivFeedStructureInfo::OnFuturesOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CStructureProviderEx::OnFuturesOption\n"));
	HRESULT  hr = S_OK ;
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
	int nConnections = m_vec.GetSize();

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[2] = varParams;
			pvars[1] = varResults;
			pvars[0] = varIsLastOption;
			DISPPARAMS disp = { pvars, NULL, 3, 0 };
			hr = pDispatch->Invoke(0x6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
HRESULT			CActivFeedStructureInfo::OnStock(const CComVariant &varParams,const CComVariant &varResults)
{
	ATLTRACE(_T("CActivFeedStructureInfo::OnStock\n"));

	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[1] = varParams;
			pvars[0] = varResults;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT			CActivFeedStructureInfo::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CActivFeedStructureInfo::OnOption\n"));
	HRESULT  hr = S_OK ;
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
	int nConnections = m_vec.GetSize();

	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[2] = varParams;
			pvars[1] = varResults;
			pvars[0] = varIsLastOption;
			DISPPARAMS disp = { pvars, NULL, 3, 0 };
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}

STDMETHODIMP	CActivFeedStructureInfo::RequestFuture(FutureParams *Params)
{
	HRESULT hr = S_OK;
	if( Params && m_spActiv ){
		try						{ m_spActiv->RequestFuture( Params ); }
		catch ( _com_error& e)	{ hr = e.Error(); }
		catch (... )			{ EgLib::CEgLibTraceManager::TraceText( LogSystem, __FUNCTION__ , _T(" Unhandled exception ") ); }
	}
	return m_spActiv ? hr : E_INVALIDARG;
}
STDMETHODIMP	CActivFeedStructureInfo::RequestFutures(FutureRootParams *Params)
{
	HRESULT hr = S_OK;
	if( Params && m_spActiv ){
		try						{ m_spActiv->RequestFutures(Params); }
		catch ( _com_error& e)	{ hr = e.Error(); }
		catch (... )			{ EgLib::CEgLibTraceManager::TraceText( LogSystem, __FUNCTION__ , _T(" Unhandled exception ") ); }
	}
	return m_spActiv ? hr : E_INVALIDARG;
}
STDMETHODIMP	CActivFeedStructureInfo::RequestFuturesOptions(FuturesOptionParams *Params)
{
	HRESULT hr = S_OK;
	if( Params && m_spActiv ){
		try						{ m_spActiv->RequestFuturesOptions(Params); }
		catch ( _com_error& e)	{ hr = e.Error(); }
		catch (... )			{ EgLib::CEgLibTraceManager::TraceText( LogSystem, __FUNCTION__ , _T(" Unhandled exception ") ); }
	}
	return m_spActiv ? hr : E_INVALIDARG;
}
STDMETHODIMP	CActivFeedStructureInfo::RequestOptions(OptionParams *Params)
{
	HRESULT hr = S_OK;
	if( Params && m_spActiv ){
		try						{ m_spActiv->RequestOptions(Params); }
		catch ( _com_error& e)	{ hr = e.Error(); }
		catch (... )			{ EgLib::CEgLibTraceManager::TraceText( LogSystem, __FUNCTION__ , _T(" Unhandled exception ") ); }
	}
	return m_spActiv ? hr : E_INVALIDARG;
}
STDMETHODIMP	CActivFeedStructureInfo::RequestStock(StockParams *Params)
{   
	HRESULT hr = S_OK;
	if( Params && m_spActiv ){
		try						{ m_spActiv->RequestStock(Params); }
		catch ( _com_error& e)	{ hr = e.Error(); }
		catch (... )			{ EgLib::CEgLibTraceManager::TraceText( LogSystem, __FUNCTION__ , _T(" Unhandled exception ") ); }
	}
	return m_spActiv ? hr : E_INVALIDARG;
}
