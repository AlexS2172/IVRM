// CEtsPerformanceObject.cpp : Implementation of CCEtsPerformanceObject

#include "stdafx.h"
#include "CEtsPerformanceObject.h"
#include ".\cetsperformanceobject.h"


// CCEtsPerformanceObject
STDMETHODIMP CCEtsPerformanceObject::CalcOptionGreeks(void)
{
	if(m_pCounters)
	{
		m_pCounters->Calculations++;
		m_pCounters->CalcGreeks++;
		m_pCounters->CalcOptionGreeks++;
	}
	return S_OK;
}
STDMETHODIMP CCEtsPerformanceObject::CalcFutureOptionGreeks(void)
{
	if(m_pCounters)
	{
		m_pCounters->Calculations++;
		m_pCounters->CalcGreeks++;
		m_pCounters->CalcFutureOptionGreeks++;
	}
	return S_OK;
}
STDMETHODIMP CCEtsPerformanceObject::CalcOptionVolatility(void)
{
	if(m_pCounters)
	{
		m_pCounters->Calculations++;
		m_pCounters->CalcVolatility++;
		m_pCounters->CalcOptionVolatility++;
	}
	return S_OK;
}
STDMETHODIMP CCEtsPerformanceObject::CalcFutureOptionVolatility(void)
{
	if(m_pCounters)
	{
		m_pCounters->Calculations++;
		m_pCounters->CalcVolatility++;
		m_pCounters->CalcFutureOptionVolatility++;
	}
	return S_OK;
}

