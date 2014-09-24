// MmTradeByUndColl.cpp : Implementation of CMmTradeByUndColl

#include "stdafx.h"
//#include "MmTradeInfoAtom.h"
#include "MmTradeInfoColl.h"
//#include "MmTradeByUndAtom.h"
#include "MmTradeByUndColl.h"


// CMmTradeByUndColl


STDMETHODIMP CMmTradeByUndColl::Add(BSTR Key, IMmTradeInfoColl* Value, IMmTradeInfoColl** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		_bstr_t bsKey(Key);
		if(m_coll.find( bsKey ) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Trade with the same key already exists."));
		}
		
		if(!Value)
		{
			
			CComObject<CMmTradeInfoColl>* pNewVal;
			__CHECK_HRESULT(CComObject<CMmTradeInfoColl>::CreateInstance(&pNewVal), _T("Failed to add trade coll."));
			pNewVal->AddRef();

			if(FAILED(IMmTradeByUndCollImpl::Add(bsKey,  pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Failed to add trade."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmTradeByUndCollImpl::Add(bsKey, Value), _T("Failed to add trade."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeInfoColl, e.Error());
	}

	return S_OK;
}

/*
HRESULT CMmTradeByUndColl::PersistSave(LPSTREAM pStream, BOOL bClearDirty, ATL_PROPMAP_ENTRY* pMap)
{
	HRESULT hr = S_OK;
	ULONG ret = 0;
	LONG lKey = 0;

	try
	{
		size_t szSize = m_coll.size();
		__CHECK_HRESULT3( pStream->Write(&szSize, sizeof(szSize), &ret));

		for(EnumCollType::iterator Iter = m_coll.begin(); Iter!= m_coll.end(); ++Iter)
		{
			IMmTradeByUndAtomPtr pObj = Iter->second;

			CComQIPtr<IPersistStreamInit> spStream(pObj);
			ATLASSERT(spStream !=NULL);
			__CHECK_HRESULT3(spStream->Save(pStream, bClearDirty));
		}
	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeByUndColl, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhandled exception handled at PersistSave"), IID_IMmTradeByUndColl, E_FAIL);
	}
	return hr;
}
HRESULT CMmTradeByUndColl::PersistLoad(LPSTREAM pStream, ATL_PROPMAP_ENTRY* pMap)
{
	HRESULT hr = S_OK;
	ULONG ret = 0;
	size_t lLoad = 0;
	BSTR bsKey;
	DOUBLE lSortKey = 0;
	try
	{

		__CHECK_HRESULT3(pStream->Read(&lLoad, sizeof(&lLoad), &ret));
		for(size_t i=0; i<lLoad; ++i)
		{
			CComObject<CMmTradeByUndAtom>* pAtom = NULL;
			CComPtr<IPersistStreamInit> spStream;

			__CHECK_HRESULT3(CComObject<CMmTradeByUndAtom>::CreateInstance(&pAtom));
			__CHECK_HRESULT3(pAtom->QueryInterface(IID_IPersistStream, (void**)&spStream));
			__CHECK_HRESULT3(spStream->Load(pStream));

			CComQIPtr<IMmTradeByUndAtom> spAtom(spStream);
			CComPtr<IMmTradeByUndAtom>   spRetAtom;

			__CHECK_HRESULT3(spAtom->get_bsKey(&bsKey));
			__CHECK_HRESULT3(spAtom->get_dSortKey(&lSortKey));

			
			__CHECK_HRESULT3(Add(bsKey , lSortKey, spAtom, &spRetAtom));
		}
	}
	catch (_com_error& e) 
	{
		hr =  Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeByUndColl, e.Error());
	}
	catch(...)
	{
		hr =  Error( _T("Unhandled exception handled at PersistLoad"), IID_IMmTradeByUndColl, E_FAIL);
	}
	return hr;
}
*/

