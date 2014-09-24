// VolatilitySourcesImpl.cpp : Implementation of CVolatilitySources

#include "stdafx.h"
#include "VolatilitySourcesImpl.h"
#include "vsdatastorage.h"
#include "Source.h"

STDMETHODIMP CVolatilitySources::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVolatilitySources
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}//
//STDMETHODIMP CVolatilitySources::get_DefaultSourceID(long* pVal)
//{
//	if(!g_VSDataStorage.IsInitialized())
//		return Error(L"Failed to initialize VolatilitySources storage", IID_IVolatilitySources,E_FAIL);
//
//	__CHECK_POINTER( pVal );
//	return g_VSDataStorage.GetCurrentSource(pVal);
//}
//
//STDMETHODIMP CVolatilitySources::put_DefaultSourceID(long newVal)
//{
//	if(!g_VSDataStorage.IsInitialized())
//		return Error(L"Failed to initialize VolatilitySources storage", IID_IVolatilitySources,E_FAIL);
//	return g_VSDataStorage.SetCurrentSource(newVal);
//}
//
//STDMETHODIMP CVolatilitySources::Reload()
//{
//	g_VSDataStorage.Reload();
//	return S_OK;


STDMETHODIMP CVolatilitySources::get_Count( long *  pVal)
{
	if(!g_VSDataStorage.IsInitialized())
		return Error(L"Failed to initialize VolatilitySources storage", IID_IVolatilitySources,E_FAIL);
	__CHECK_POINTER( pVal );
	*pVal = g_VSDataStorage.GetSize();
	return S_OK;
}


//
//STDMETHODIMP CVolatilitySources::GetSourceInfo( long  SourceID,   BSTR *  Title,  BSTR *  FileName,  BSTR *  Version)
//{
//	if(!g_VSDataStorage.IsInitialized())
//		return Error(L"Failed to initialize VolatilitySources storage", IID_IVolatilitySources,E_FAIL);
//
//	__CHECK_POINTER( Title );
//	__CHECK_POINTER( FileName );
//	__CHECK_POINTER( Version );
//	
//	VSData data;
//	if(g_VSDataStorage.GetItembyID(SourceID, &data))
//	{
//		*Title		= data.m_Title;
//		*FileName	= data.m_FileName;
//		*Version	= data.m_Version;
//		return S_OK;
//	}
//	return E_INVALIDARG;
//}

STDMETHODIMP CVolatilitySources::GetVolatilitySourceInfo(long Index, long* ID, BSTR* Title, BSTR* FileName, BSTR* Version, BSTR* ProgID)
{
	if(!g_VSDataStorage.IsInitialized())
		return Error(L"Failed to initialize VolatilitySources storage", IID_IVolatilitySources, E_FAIL);

	__CHECK_POINTER(ID);
	__CHECK_POINTER(Title);
	__CHECK_POINTER(FileName);
	__CHECK_POINTER(Version);
	__CHECK_POINTER(ProgID);
	
	if(*Title) ::SysFreeString(*Title);
	if(*FileName) ::SysFreeString(*FileName);
	if(*Version) ::SysFreeString(*Version);
	if(*ProgID) ::SysFreeString(*ProgID);

	VSData data;
	long nID = 0;
	if(g_VSDataStorage.GetItembyIndex(Index, &nID, &data))
	{
		*ID	= nID;
		*Title = data.m_Title.copy();
		*FileName = data.m_FileName.copy();
		*Version = data.m_Version.copy();
		*ProgID = data.m_ProgID.copy();
		return S_OK;
	}
	
	return E_INVALIDARG;
}

STDMETHODIMP CVolatilitySources::GetVolatilitySourceInfoByID(long ID, BSTR* Title, BSTR* FileName, BSTR* Version, BSTR* ProgID)
{
	if(!g_VSDataStorage.IsInitialized())
		return Error(L"Failed to initialize VolatilitySources storage", IID_IVolatilitySources, E_FAIL);

	__CHECK_POINTER(Title);
	__CHECK_POINTER(FileName);
	__CHECK_POINTER(Version);
	__CHECK_POINTER(ProgID);
	
	if(*Title) ::SysFreeString(*Title);
	if(*FileName) ::SysFreeString(*FileName);
	if(*Version) ::SysFreeString(*Version);
	if(*ProgID) ::SysFreeString(*ProgID);

	VSData data;
	long nID = 0;
	if(g_VSDataStorage.GetItembyID(ID, &data))
	{
		*Title = data.m_Title.copy();
		*FileName = data.m_FileName.copy();
		*Version = data.m_Version.copy();
		*ProgID = data.m_ProgID.copy();
		return S_OK;
	}
	
	return E_INVALIDARG;
}

STDMETHODIMP CVolatilitySources::get_VolatilitySource(long ID, IVolatilitySource** Source)
{
	__CHECK_POINTER(Source);
	HRESULT hr = S_OK;
	try
	{
		if(!g_VSDataStorage.IsInitialized())
			return Error(L"Failed to initialize VolatilitySources storage", IID_IVolatilitySources,E_FAIL);

		CLSID clsid = CLSID_NULL;
		VSData data;
		if(g_VSDataStorage.GetItembyID(ID, &data))
		{
			CComObject<CSource> *pObject = NULL;
			__CHECK_HRESULT( CComObject<CSource>::CreateInstance(&pObject), _T("Error creating volatility source object instance.") );
			__CHECK_HRESULT( pObject->Initialize(data.m_clsid), _T("Error initializing volatility source object.") );
			__CHECK_HRESULT( pObject->QueryInterface(__uuidof(IVolatilitySource), (void**)Source), _T("Error initializing volatility source object."));
		}
	}catch(_com_error& e)
	{
		hr = utils::ComError2ErrInfo(e, this, __uuidof(IVolatilitySource));
	}
	return hr;
}