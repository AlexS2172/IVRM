// ProvidersCollection.cpp: implementation of the CProvidersCollection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProvidersCollection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProvidersCollection::CProvidersCollection()
{
	m_bInialized = false;

}
bool CProvidersCollection::Initialize()
{
	if(m_bInialized)
		return true;

	HRESULT hr = m_spProviders.CoCreateInstance(CLSID_Providers);
	if(SUCCEEDED(hr))
	{
		if(SUCCEEDED(m_spProviders->Initialize()))
		{
			m_bInialized = true;
			return true;
		}
	}
	
	return false;		
}

CProvidersCollection::~CProvidersCollection()
{
	m_spProviders = NULL;
}

bool CProvidersCollection::GetProgID(long lProviderID, ProviderRole role, _bstr_t &bsProgID)
{

	CAutoLock sw(m_csLock);
	if(!Initialize())
		return false;
	bool bRet = false;
	long lCount=-1;
	HRESULT hr = m_spProviders->get_Count(&lCount);
	if(SUCCEEDED(hr))
	{
		for(long l=1;l<=lCount;l++)
		{
			CComPtr<IProviderData> spData;
			hr = m_spProviders->get_Item(l, &spData);
			if(SUCCEEDED(hr))
			{
				long lProvID;
				spData->get_ProviderID(&lProvID);
				if(lProvID == lProviderID)
				{
					CComBSTR bsProg;
					hr = spData->get_ProgID(role, &bsProg);
					if(SUCCEEDED(hr))
					{
						bRet =true;
						bsProgID = (BSTR)bsProg;
					}
					else
						bRet =false;
					break;
				}
			} else
				break;
		}
	}
	m_spProviders = NULL;
	m_bInialized = false;

	return bRet;
}
