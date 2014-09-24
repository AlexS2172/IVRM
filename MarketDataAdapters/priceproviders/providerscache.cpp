// ProvidersCache.cpp : Implementation of CProvidersCache

#include "stdafx.h"
#include "ProvidersCache.h"


// CProvidersCache

STDMETHODIMP CProvidersCache::Attach(void)
{
	InterlockedIncrement(&m_lClientsCount);
	if(m_bInitialized)
		return S_OK;
	Clear();
	CCatInformation catInfo;							// CatInformation
	HRESULT hr = catInfo.Initialize();
	if(FAILED(hr))
		return hr;
	
	if(SUCCEEDED(hr))
	{
		ULONG nRet=0;
		PROVIDERS::iterator iter;
		for(iter= catInfo.begin(); iter!=catInfo.end(); iter++)
		{
			EgLib::CEgRegKey reg;
			_bstr_t str(_T("SOFTWARE\\Egar\\Providers\\"));
			str += *iter;
			bool bRead = false;
			if(ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE,str,KEY_READ))
			{
				DWORD dwID;
				_bstr_t bsName;
				if(ERROR_SUCCESS == reg.QueryValue(dwID,  _T("")) &&
				   ERROR_SUCCESS == reg.QueryValue(bsName,_T("Name")))
				{
					CLSID clsid;
					HRESULT _hr = CLSIDFromString(*iter, &clsid);
					if(SUCCEEDED(_hr))
					{
						CProviderData* pData =new CComObject<CProviderData>;
						pData->AddRef();
						pData->m_lID = dwID;
						pData->m_bsDescription = (BSTR)bsName;
						pData->m_clsidProvider = clsid;

						pData->m_bInitialized = false;
						m_coll.push_back(pData);
						bRead = true;
					}

				}
			}
			if(!bRead)
			{
				CLSID clsid;
				HRESULT _hr = CLSIDFromString(*iter, &clsid);
				if(SUCCEEDED(_hr))
				{
					IProviderInfoPtr spInfo;
					hr = spInfo.CreateInstance(clsid);
					if(SUCCEEDED(hr))
					{
						CProviderData* pData =new CComObject<CProviderData>;
						pData->AddRef();

						BSTR  bsData;
						try
						{
							__CHECK_HRESULT3(spInfo->get_ProviderID(&pData->m_lID));
							__CHECK_HRESULT3(spInfo->get_Description(&pData->m_bsDescription));
							__CHECK_HRESULT3(spInfo->get_IsGroup(&pData->m_bGroup));
							__CHECK_HRESULT3(spInfo->get_NeedLogin(&pData->m_bLogin));

//							__CHECK_HRESULT3(spInfo->get_Provider(enStructureProvider,&bsData));
//							pData->m_Progs[enStructureProvider] = bsData;
//							SysFreeString(bsData);			
//							bsData = NULL;

//							__CHECK_HRESULT3(spInfo->get_Provider(enStructureInfo, &bsData));
//							pData->m_Progs[enStructureInfo] = bsData;
//							SysFreeString(bsData);			
//							bsData = NULL;

//							__CHECK_HRESULT3(spInfo->get_Provider(enPriceProvider, &bsData));
//							pData->m_Progs[enPriceProvider] = bsData;
//							SysFreeString(bsData);			
//							bsData = NULL;
//
//							__CHECK_HRESULT3(spInfo->get_Provider(enPriceInfo, &bsData));
//							pData->m_Progs[enPriceInfo] = bsData;
//							SysFreeString(bsData);	
//							bsData = NULL;
//
//							__CHECK_HRESULT3(spInfo->get_Provider(enPriceInfoWithNotify, &bsData));
//							pData->m_Progs[enPriceInfoWithNotify] = bsData;
//							SysFreeString(bsData);	
//							bsData = NULL;

							__CHECK_HRESULT3(spInfo->get_Provider(enBatchPriceInfo, &bsData));
							pData->m_Progs[enBatchPriceInfo] = bsData;
							SysFreeString(bsData);	
							bsData = NULL;

							__CHECK_HRESULT3(spInfo->get_Provider(enStructureProviderEx,&bsData));
							pData->m_Progs[enStructureProviderEx] = bsData;
							SysFreeString(bsData);			
							bsData = NULL;
						}
						catch(_com_error& e)
						{
							if(bsData)
							{
								SysFreeString(bsData);
								bsData = NULL;
							}
							hr = e.Error();
							return hr;
						}

						pData->m_clsidProvider = clsid;
						pData->m_bInitialized = true;

						m_coll.push_back(pData);
					}
				}
			}
		}
	}

	if(SUCCEEDED(hr))
		m_bInitialized = true;
	return hr;
}

STDMETHODIMP CProvidersCache::Detach(void)
{
	InterlockedDecrement(&m_lClientsCount);
	if(!m_lClientsCount)
		Clear();

	return S_OK;
}
