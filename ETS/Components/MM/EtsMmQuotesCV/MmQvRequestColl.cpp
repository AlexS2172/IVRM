// MmQvRequestColl.cpp : Implementation of CMmQvRequestColl

#include "stdafx.h"
#include "MmQvRequestColl.h"


// CMmQvRequestColl
IMmQvRequestAtomPtr CMmQvRequestColl::AddNew(_bstr_t bsKey, CComObject<CMmQvRequestAtom>** ppAtom)
{
	ObjectLock (this);

	IMmQvRequestAtomPtr	spEntityComp;
	CComObject<CMmQvRequestAtom>* pEntityComp = NULL;

	_CHK(CComObject<CMmQvRequestAtom>::CreateInstance(&pEntityComp), _T("Fail to add Quote Request."));
	spEntityComp.Attach(pEntityComp, TRUE);

	if(FAILED(IMmQvRequestCollImpl::Add(bsKey, pEntityComp)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add Quote Request."));

	if(ppAtom)
		*ppAtom = pEntityComp;
	return 	spEntityComp;
}


STDMETHODIMP CMmQvRequestColl::Add(BSTR Key, IMmQvRequestAtom* Value, IMmQvRequestAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);
	_bstr_t bsKey(Key);

	try
	{
		if(m_coll.find(bsKey) != m_coll.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Quote Request component with the same key is already exists."));


		if(!Value)
			*pRetVal = AddNew(bsKey).Detach();
		else
		{
			_CHK(IMmQvRequestCollImpl::Add(bsKey, Value), _T("Fail to add Quote Request."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmQvRequestColl, e.Error());
	}

	return S_OK;
}

HRESULT CMmQvRequestColl::RequestLastQuotes(IBatchPriceProviderPtr spProvider)
{
	typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >  _QuoteUpdateParams;
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

						IMmQvRequestAtomPtr spRequestAtom = itr->second;
						spRequestAtom->get_QuoteUpdateParam(request);
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
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), IID_IMmQvRequestColl, err.Error());
	}
	/*catch (...)
	{
	}*/
	return S_OK;
}

HRESULT CMmQvRequestColl::StartRealtime(IBatchPriceProviderPtr  spProvider)
{
	typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >  _QuoteUpdateParams;
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

						IMmQvRequestAtomPtr spRequestAtom = itr->second;
						spRequestAtom->get_QuoteUpdateParam(request);
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
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), IID_IMmQvRequestColl, err.Error());
	}
	/*catch (...)
	{
	}*/
	return S_OK;
}
