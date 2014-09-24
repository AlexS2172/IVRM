// DualQuadraticData.cpp : Implementation of CDualQuadraticData
#include "stdafx.h"
#include "VME.h"

#include "DualQuadraticData.h"

/////////////////////////////////////////////////////////////////////////////
// Construction/destruction
/////////////////////////////////////////////////////////////////////////////
CDualQuadraticData::CDualQuadraticData() :
m_dATMVola(0),m_dCoefA1(0),m_dCoefB1(0),m_dCoefA2(0),m_dCoefB2(0)
{
}


HRESULT CDualQuadraticData::FinalConstruct()
{
	return S_OK;
}


void CDualQuadraticData::FinalRelease()
{
}


/////////////////////////////////////////////////////////////////////////////
// IDualQuadraticData implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDualQuadraticData::get_ATMVola(double *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_dATMVola;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::put_ATMVola(double newVal)
{
	ObjectLock lock(this);
	m_dATMVola = newVal;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::get_CoefA1(double *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_dCoefA1;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::put_CoefA1(double newVal)
{
	ObjectLock lock(this);
	m_dCoefA1 = newVal;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::get_CoefB1(double *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_dCoefB1;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::put_CoefB1(double newVal)
{
	ObjectLock lock(this);
	m_dCoefB1 = newVal;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::get_CoefA2(double *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_dCoefA2;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::put_CoefA2(double newVal)
{
	ObjectLock lock(this);
	m_dCoefA2 = newVal;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::get_CoefB2(double *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_dCoefB2;

	return S_OK;
}

STDMETHODIMP CDualQuadraticData::put_CoefB2(double newVal)
{
	ObjectLock lock(this);
	m_dCoefB2 = newVal;

	return S_OK;
}
