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
	catch (...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		return Error((PTCHAR)"System Error", IID_IMmQvRequestColl, _hr);
	}


	return S_OK;
}

HRESULT CMmQvRequestColl::RequestQuotes(CRequests& requests, IBatchPriceProviderPtr spProvider, bool bSubscribe)
{
	typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >  _QuoteUpdateParams;
	HRESULT hr = S_OK;
	try
	{
		long lSize = static_cast<long>(requests.size());
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
					memset(pvData, 0,  requests.size() * sizeof(QuoteUpdateParams));
					int i = 0;
					for(CRequests::iterator itr = requests.begin(); itr!=requests.end(); ++itr, ++i)
					{

						IMmQvRequestAtomPtr spRequestAtom = *itr;
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
					if(bSubscribe)
						spProvider->SubscribeMultipleQuotes(&psa);
					else
						spProvider->RequestMultipleQuotes(&psa);
				}
				SafeArrayDestroy(psa);
			}
		}
	}catch(_com_error& err)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), IID_IMmQvRequestColl, err.Error());
	}
	catch (...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		return Error((PTCHAR)"System Error", IID_IMmQvRequestColl, _hr);
	}
	return S_OK;

}

HRESULT CMmQvRequestColl::Request(bool bStockOrIndex, IBatchPriceProviderPtr spProvider, bool bSubscribe, long lActiveFuturesID)
{
	HRESULT hr = S_OK;
	LONG	lUndPrimExchID = 0;
	m_lVisibleRequests   = 0;
	m_lInvisibleRequests = 0;
	try
	{
		std::vector<IMmQvRequestAtomPtr> vecVisible;
		std::vector<IMmQvRequestAtomPtr> vecInvisible;
		vecVisible.reserve(m_coll.size());
		vecInvisible.reserve(m_coll.size());
		for(CollType::iterator itr = m_coll.begin(); itr!=m_coll.end(); ++itr)
		{
			IMmQvRequestAtomPtr spRequestAtom = itr->second;
			CMmQvRequestAtom* pRequestAtom    =  dynamic_cast<CMmQvRequestAtom*>(spRequestAtom.GetInterfacePtr());
			if(pRequestAtom)
			{
				bool bVisible = false;
				if (pRequestAtom->m_spUnd)
					pRequestAtom->m_spUnd->get_PrimaryExchangeID(&lUndPrimExchID);

				if(lUndPrimExchID == pRequestAtom->m_spExch->ID ||
					0 == pRequestAtom->m_spExch->ID ||
					VARIANT_FALSE!=pRequestAtom->m_spExch->Visible)
				{	
					if(bStockOrIndex && pRequestAtom->m_spFut==NULL) 
						bVisible = true;
					else
					if(bStockOrIndex && pRequestAtom->m_spFut!=NULL && lActiveFuturesID != 0L) 
					{
						long lFutID = 0;
						pRequestAtom->m_spFut->get_ID(&lFutID);
						if(lFutID == lActiveFuturesID)
							bVisible = true;
					}
					else
					if(!bStockOrIndex && pRequestAtom->m_spFut!=NULL) 
						bVisible = true;
					
					if (pRequestAtom->m_spUnd){
						VARIANT_BOOL	bIsHead  = VARIANT_FALSE;
						pRequestAtom->m_spUnd->get_IsHead(&bIsHead);
						if (bIsHead == VARIANT_TRUE) bVisible = true;
					}
				}
				pRequestAtom->m_bVisible = (bVisible?VARIANT_TRUE:VARIANT_FALSE);

				if(bVisible)
					vecVisible.push_back(spRequestAtom);
				else
					vecInvisible.push_back(spRequestAtom);
			}
		}
		m_lVisibleRequests   = static_cast<long>(vecVisible.size());
		m_lInvisibleRequests = static_cast<long>(vecInvisible.size());

		if(SUCCEEDED(hr) && m_lVisibleRequests!=0)
			hr = RequestQuotes(vecVisible, spProvider, bSubscribe);
		if(SUCCEEDED(hr) && m_lInvisibleRequests!=0)
			hr = RequestQuotes(vecInvisible, spProvider, bSubscribe);

	}catch(_com_error& err)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(err), IID_IMmQvRequestColl, err.Error());
	}
	catch (...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		return Error((PTCHAR)"System Error", IID_IMmQvRequestColl, _hr);
	}
	return S_OK;
}




