// FixProvidersCache.cpp : Implementation of CFixProvidersCache

#include "stdafx.h"
#include "FixProvidersCache.h"
// CFixProvidersCache

STDMETHODIMP CFixProvidersCache::Attach(void)
{
	InterlockedIncrement(&m_lClientsCount);

	if(m_bInitialized)
		return S_OK;

	Clear();
	CComPtr<IEnumCLSID> spEnumCLSID;					// IEnumCLSID
	CCatInformation spCatInfo;							// ICatInformation
	CATID catInfo = CATID_FixProviders;

	HRESULT hr = spCatInfo->EnumClassesOfCategories(
			1,			// num implemented cats in array
			&catInfo,	// array of cats to look for (implement)
			NULL,			// num required categories in array
			0,	// array of required categories to look for
			&spEnumCLSID);	// IEnum returned

	if(SUCCEEDED(hr))
	{
		ULONG nRet=0;
		CLSID clsid;

		while(spEnumCLSID->Next(1, &clsid, &nRet) == S_OK)
		{
			LPOLESTR pDaya ;
			StringFromCLSID(clsid, &pDaya);
			CEgRegKey reg;

			_bstr_t str(_T("SOFTWARE\\Egar\\Fix Providers\\"));
			str += pDaya;
			bool bRead = false;
			if(ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, str, KEY_READ))
			{
				DWORD dwID;
				_bstr_t bsName,
						bsId;

				LONG lReturn = reg.QueryValue(dwID, _T(""));

				if (ERROR_SUCCESS == lReturn &&
				   ERROR_SUCCESS == reg.QueryValue(bsName, _T("Name")))
				{
					CFixProviderSpecifics* pData = new CComObject<CFixProviderSpecifics>;
					pData->AddRef();
					pData->m_lID = dwID;
					pData->m_bsDescription = (BSTR)bsName;
					pData->m_clsidFixProvider = clsid;

					pData->m_bInitialized = false;
					m_coll.push_back(pData);
					bRead = true;
				}
			}
			::CoTaskMemFree(pDaya);
		}
	}

	if(SUCCEEDED(hr))
		m_bInitialized = true;

	return hr;
}

STDMETHODIMP CFixProvidersCache::Detach(void)
{
	InterlockedDecrement(&m_lClientsCount);
	if(!m_lClientsCount)
		Clear();

	return S_OK;
}
