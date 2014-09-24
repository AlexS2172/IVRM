// EtsFilterData.cpp : Implementation of CEtsFilterData

#include "stdafx.h"
#include "EtsFilterData.h"


// CEtsFilterData

STDMETHODIMP CEtsFilterData::get_Data(LONG DataId, LONG* pVal)
{
	if(!m_bInitialized)
		return E_FAIL;

	if(m_lLowerBound > DataId || m_lUpperBound < DataId)
		return E_INVALIDARG;
	long lPoint = DataId - m_lLowerBound;
	*pVal = m_spArrayData[lPoint];

	return S_OK;
}

STDMETHODIMP CEtsFilterData::put_Data(LONG DataId, LONG newVal)
{
	if(!m_bInitialized)
		return E_FAIL;

	if(m_lLowerBound > DataId || m_lUpperBound < DataId)
		return E_INVALIDARG;
	long lPoint = DataId - m_lLowerBound;

	m_spArrayData[lPoint] = newVal;
	return S_OK;
}

STDMETHODIMP CEtsFilterData::Initialize(LONG LowBound, LONG HighBound)
{
	if(HighBound < LowBound || m_bInitialized)
		return E_INVALIDARG;

	m_szDataSize = HighBound - LowBound;
	m_spArrayData = boost::shared_array<long>(new long[m_szDataSize]);
	for(size_t i = 0; i< m_szDataSize; ++i)
		m_spArrayData[i] = 0;
	m_lLowerBound = LowBound;
	m_lUpperBound = HighBound;
	m_bInitialized = true;
	return S_OK;
}
