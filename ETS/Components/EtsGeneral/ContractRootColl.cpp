// ContractRootColl.cpp : Implementation of CContractRootColl

#include "stdafx.h"
#include "ContractRootColl.h"


// CContractRootColl
//----------------------------------------------------------------------------------------------
STDMETHODIMP 
CContractRootColl::Add(LONG Key, BSTR SortKey, IContractRoot* Value, IContractRoot** pRetVal){
	try{
		
		if(m_collRef.find(Key) == m_collRef.end()){
			_bstr_t bsSortKey(SortKey);
			if (Value != NULL){
				_CHK(IContractRootCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add contract root."));
				Value->AddRef();
				*pRetVal = Value;
			}
			else
				return E_POINTER;
		}
	}
	catch(const _com_error& e){
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IContractRootColl, e.Error());
	}
	return S_OK;
}
//----------------------------------------------------------------------------------------------
