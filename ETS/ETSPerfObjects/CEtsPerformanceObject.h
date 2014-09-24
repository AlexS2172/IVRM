// CEtsPerformanceObject.h : Declaration of the CCEtsPerformanceObject

#pragma once
#include "resource.h"       // main symbols
#include "EtsPerformance.h"

// ICEtsPerformanceObject
[
	object,
	uuid("44E599CE-3A08-4DA8-BB30-146942E70F0D"),
	dual,	helpstring("ICEtsPerformanceObject Interface"),
	pointer_default(unique)
]
__interface ICEtsPerformanceObject : IDispatch
{
	[id(1), helpstring("method CalcOptionGreeks")]				HRESULT CalcOptionGreeks(void);
	[id(2), helpstring("method CalcFutureOptionGreeks")]		HRESULT CalcFutureOptionGreeks(void);
	[id(3), helpstring("method CalcOptionVolatility")]			HRESULT CalcOptionVolatility(void);
	[id(4), helpstring("method CalcFutureOptionVolatility")]	HRESULT CalcFutureOptionVolatility(void);
};



// CCEtsPerformanceObject

[
	coclass,
	threading("apartment"),
	support_error_info("ICEtsPerformanceObject"),
	vi_progid("ETSPerfObjects.CEtsPerformanceObject"),
	progid("ETSPerfObjects.CEtsPerformanceObject.1"),
	version(1.0),
	uuid("C1EF046B-6C8D-422B-A936-775F3E6791D1"),
	helpstring("CEtsPerformanceObject Class")
]
class ATL_NO_VTABLE CCEtsPerformanceObject : 
	public ICEtsPerformanceObject
{
public:
	CCEtsPerformanceObject()
		:m_pCounters(NULL)
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_CLASSFACTORY_SINGLETON(CCEtsPerformanceObject)

	HRESULT FinalConstruct()
	{
		if(SUCCEEDED(m_PerfObject.Initialize()))
		{
			wchar_t buffer[256];
			DWORD lBuffer = 256;
			GetUserNameW(buffer, &lBuffer);
            m_PerfObject.CreateInstance(0,buffer, &m_pCounters);
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		if(m_pCounters)
			m_PerfObject.ReleaseInstance(m_pCounters);
	}
private:
	CEtsPerformance	m_PerfObject;
	ETSPerfObject*  m_pCounters;
public:

	STDMETHOD(CalcOptionGreeks)(void);
	STDMETHOD(CalcFutureOptionGreeks)(void);
	STDMETHOD(CalcOptionVolatility)(void);
	STDMETHOD(CalcFutureOptionVolatility)(void);
};

