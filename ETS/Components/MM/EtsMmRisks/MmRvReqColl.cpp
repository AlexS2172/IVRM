// MmRvReqColl.cpp : Implementation of CMmRvReqColl
#include "stdafx.h"
#include "MmRvReqColl.h"

/////////////////////////////////////////////////////////////////////////////
//
IMmRvReqAtomPtr CMmRvReqColl::AddNew(_bstr_t bsKey, CComObject<CMmRvReqAtom>** pObject)
{
	CComObject<CMmRvReqAtom>* pNewVal = NULL;
	IMmRvReqAtomPtr spNewVal;

	_CHK(CComObject<CMmRvReqAtom>::CreateInstance(&pNewVal), _T("Fail to add quote request."));
	spNewVal.Attach(pNewVal, TRUE);
	if(FAILED(IMmRvReqCollImpl::Add(bsKey, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add quote request."));

	if(pObject)
		*pObject = pNewVal;

	return spNewVal;
}

STDMETHODIMP CMmRvReqColl::Add(BSTR Key, IMmRvReqAtom* Value, IMmRvReqAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		_bstr_t bsKey(Key);
		if(m_coll.find(bsKey) != m_coll.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Quote request with the same key is already exists."));

		if(!Value)
			*pRetVal = AddNew(bsKey).Detach();
		else
		{
			_CHK(IMmRvReqCollImpl::Add(bsKey, Value), _T("Fail to add quote request."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvReqColl, e.Error());
	}

	return S_OK;
}

HRESULT CMmRvReqColl::RequestLastQuotes(IBatchPriceProvider* pIProvider)
{
	typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >  _QuoteUpdateParams;
	__CHECK_POINTER(pIProvider);
	IBatchPriceProviderPtr spProvider(pIProvider);
	HRESULT hr = S_OK;
	try
	{
		long lSize = static_cast<long>(m_coll.size());
		if(lSize)
		{
			_QuoteUpdateParams request;
			request.Init();
			SAFEARRAYBOUND sab = { lSize, 0};

			LPSAFEARRAY psa = ::SafeArrayCreateEx(VT_RECORD, 1, &sab, (PVOID)request.GetRecordInfo());
			if(psa)
			{
				QuoteUpdateParams* pvData = NULL;
				if(SUCCEEDED(hr = ::SafeArrayAccessData(psa, (void**)&pvData)))
				{
					memset(pvData, 0,  m_coll.size() * sizeof(QuoteUpdateParams));
					int i = 0;
					for(CollType::iterator itr = m_coll.begin(); itr!=m_coll.end(); ++itr, ++i)
					{

						IMmRvReqAtomPtr spRequestAtom = itr->second;
						spRequestAtom->GetQuoteUpdateParam(request);
						request.CopyTo(&pvData[i]);

						if(request->Symbol)
						{
							SysFreeString(request->Symbol);
							request->Symbol = NULL;
						}
						if(request->Exchange)
						{
							SysFreeString(request->Exchange);
							request->Exchange = NULL;
						}
					}	
					::SafeArrayUnaccessData(psa);
					spProvider->RequestMultipleQuotes(&psa);
				}
				SafeArrayDestroy(psa);
			}
		}
	}catch(_com_error& err)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), IID_IMmRvReqColl, err.Error());
	}
	catch (...)
	{
	}
	return S_OK;
}

HRESULT CMmRvReqColl::StartRealtime(IBatchPriceProvider*  pIProvider)
{
	typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >  _QuoteUpdateParams;
	__CHECK_POINTER(pIProvider);
	IBatchPriceProviderPtr spProvider(pIProvider);
	HRESULT hr = S_OK;
	try
	{
		long lSize = static_cast<long>(m_coll.size());
		if(lSize)
		{
			_QuoteUpdateParams request;
			request.Init();
			SAFEARRAYBOUND sab = { lSize, 0};

			LPSAFEARRAY psa = ::SafeArrayCreateEx(VT_RECORD, 1, &sab, (PVOID)request.GetRecordInfo());
			if(psa)
			{
				QuoteUpdateParams* pvData = NULL;
				if(SUCCEEDED(hr = ::SafeArrayAccessData(psa, (void**)&pvData)))
				{
					memset(pvData, 0,  m_coll.size() * sizeof(QuoteUpdateParams));
					int i = 0;
					for(CollType::iterator itr = m_coll.begin(); itr!=m_coll.end(); ++itr, ++i)
					{

						IMmRvReqAtomPtr spRequestAtom = itr->second;
						spRequestAtom->GetQuoteUpdateParam(request);
						request.CopyTo(&pvData[i]);

						if(request->Symbol)
						{
							SysFreeString(request->Symbol);
							request->Symbol = NULL;
						}
						if(request->Exchange)
						{
							SysFreeString(request->Exchange);
							request->Exchange = NULL;
						}
					}	
					::SafeArrayUnaccessData(psa);
					spProvider->SubscribeMultipleQuotes(&psa);
				}
				SafeArrayDestroy(psa);
			}
		}
	}catch(_com_error& err)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), IID_IMmRvReqColl, err.Error());
	}
	catch (...)
	{
	}
	return S_OK;
}