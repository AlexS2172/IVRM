// AssetColl.cpp : Implementation of CAssetColl

#include "stdafx.h"
#include "AssetColl.h"

// CAssetColl
//-------------------------------------------------------------------------------------------------------------------//
STDMETHODIMP CAssetColl::Add(LONG Key, BSTR SortKey, IAsset* Value, IAsset** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) == m_collRef.end())
		{
			_bstr_t bsSortKey(SortKey);
			if (Value != NULL)
			{
				_CHK(IAssetCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add Asset."));
				Value->AddRef();
				*pRetVal = Value;
			}
			else
				return E_POINTER;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IAssetColl, e.Error());
	}
	return S_OK;
}
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//

