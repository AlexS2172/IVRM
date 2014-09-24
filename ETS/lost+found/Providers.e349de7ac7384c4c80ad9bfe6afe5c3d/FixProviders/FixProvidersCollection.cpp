// FixProvidersCollection.cpp: implementation of the CFixProvidersCollection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FixProvidersCollection.h"

#include "cswrapper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFixProvidersCollection::CFixProvidersCollection()
{
	m_bInitialized = false;

}
bool CFixProvidersCollection::Initialize()
{
	if(m_bInitialized)
		return true;

	HRESULT hr = m_spFixProviders.CoCreateInstance(CLSID_FixProviders);
	if(SUCCEEDED(hr))
	{
		if(SUCCEEDED(m_spFixProviders->Initialize()))
		{
			m_bInitialized = true;
			return true;
		}
	}

	return false;
}

CFixProvidersCollection::~CFixProvidersCollection()
{
	m_spFixProviders = NULL;
}

bool CFixProvidersCollection::GetProgID(long lFixProviderID, _bstr_t &bsProgID)
{
	CCriticalSectionWrapper sw(m_csLock);

	if(!Initialize())
		return false;

	bool bRet	= false;
	long lCount = -1L;

	HRESULT hr = m_spFixProviders->get_Count(&lCount);

	if(SUCCEEDED(hr))
		for (long l = 1; l <= lCount; l++)
		{
			CComPtr<IFixProviderSpecifics> spData;
			hr = m_spFixProviders->get_Item(_variant_t(l), &spData);

			if(SUCCEEDED(hr))
			{
				long lProvID = 0;
				BSTR bsProgId = 0;
				BSTR bsDescr = 0;
				VARIANT_BOOL bNeedLogin = VARIANT_FALSE;
				BSTR bsAccount = 0;
				BSTR bsPassword = 0;

				spData->GetSpecifics(&lProvID, &bsProgId, &bsDescr, &bNeedLogin, &bsAccount, &bsPassword);
				if(lProvID == lFixProviderID)
				{
					bsProgID = bstr_t(bsProgId).copy();
					bRet = true;
				}

				SysFreeString(bsProgId);
				SysFreeString(bsDescr);
				SysFreeString(bsAccount);
				SysFreeString(bsPassword);

				if (bRet)
					break;
			}
			else
				break;
		}

	m_spFixProviders = NULL;
	m_bInitialized = false;

	return bRet;
}
