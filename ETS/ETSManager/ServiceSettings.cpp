// ServiceSettings.cpp : Implementation of CServiceSettings
#include "stdafx.h"
#include "ETSManager.h"
#include "ServiceSettings.h"

/////////////////////////////////////////////////////////////////////////////
// CServiceSettings

STDMETHODIMP CServiceSettings::get_DatabaseConnection(BSTR *pVal)
{
	if(!pVal)
		return E_POINTER;

	ObjectLock lock(this);

	if(*pVal)
	{
		::SysFreeString(*pVal);
	}

	*pVal = m_bsConnectionString.copy();
	return S_OK;
}

STDMETHODIMP CServiceSettings::put_DatabaseConnection(BSTR newVal)
{
	ObjectLock lock(this);
	_bstr_t bsNew = newVal;
	if(bsNew.length() && bsNew != m_bsConnectionString)
	{
		m_bsConnectionString = newVal;
		if(SaveDatabase())
			Fire_DatabaseConnectionChanged(m_bsConnectionString);
		m_bRequiresSave = true;
	}
	return S_OK;
}

STDMETHODIMP CServiceSettings::get_PriceProvider(long *pVal)
{
	if(!pVal)
		return E_POINTER;
	ObjectLock lock(this);
	*pVal = m_lProviderType;
	return S_OK;
}

STDMETHODIMP CServiceSettings::put_PriceProvider(long newVal)
{
	ObjectLock lock(this);
	if(newVal>=-1 && newVal!=m_lProviderType)
	{
		m_lProviderType = newVal;
		if(SaveProvider())
			Fire_PriceProviderChanged(m_lProviderType);		
		m_bRequiresSave = true;
	}
	return S_OK;
}

STDMETHODIMP CServiceSettings::get_VolatilitySource(long *pVal)
{
	if(!pVal)
		return E_POINTER;
	ObjectLock lock(this);
	*pVal = m_nVolatilitySourceType;
	return S_OK;
}

STDMETHODIMP CServiceSettings::put_VolatilitySource(long newVal)
{
	ObjectLock lock(this);
	if(newVal >= -1 && newVal != m_nVolatilitySourceType)
	{
		m_nVolatilitySourceType = newVal;
		if(SaveVolatilitySource())
			Fire_VolatilitySourceChanged(m_nVolatilitySourceType);		
		m_bRequiresSave = true;
	}
	return S_OK;
}

STDMETHODIMP CServiceSettings::GetDBConnectionString(LONG lType, BSTR bsName, BSTR *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
