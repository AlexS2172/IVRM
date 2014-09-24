// EtsProcessDelay.h : Declaration of the CEtsProcessDelay
#ifndef __ETSPROCESSDELAY_H__
#define __ETSPROCESSDELAY_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"

_COM_SMARTPTR_TYPEDEF(IEtsProcessDelay, IID_IEtsProcessDelay);

// CEtsProcessDelay

class ATL_NO_VTABLE CEtsProcessDelay : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsProcessDelay, &CLSID_EtsProcessDelay>,
	public ISupportErrorInfoImpl<&IID_IEtsProcessDelay>,
	public IDispatchImpl<IEtsProcessDelay, &IID_IEtsProcessDelay, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CEtsProcessDelay()
		: m_nFrequency(0L), m_nDuration(0L), m_bIsInterrupted(VARIANT_FALSE),
		m_hDelayTimer(NULL), m_nDelayStep(0L)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSPROCESSDELAY)


BEGIN_COM_MAP(CEtsProcessDelay)
	COM_INTERFACE_ENTRY(IEtsProcessDelay)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		try
		{
			m_hDelayTimer = ::CreateWaitableTimer(NULL, TRUE, NULL);
			if(!m_hDelayTimer)
				EgLib::CComErrorWrapper::ThrowError(HRESULT_FROM_WIN32(::GetLastError()), _T("Fail to create timer"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsProcessDelay, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		if(m_hDelayTimer)
		{	
			::CloseHandle(m_hDelayTimer);
			m_hDelayTimer = NULL;
		}
		m_pUnkMarshaler.Release();
	}

private:
	LONG			m_nFrequency;
	LONG			m_nDuration;
	VARIANT_BOOL	m_bIsInterrupted;

	HANDLE			m_hDelayTimer;
	LONG			m_nDelayStep;

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, Frequency, m_nFrequency)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Duration, m_nDuration)
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(VARIANT_BOOL, IsInterrupted, m_bIsInterrupted)

	STDMETHODIMP Sleep()
	{
		if(m_bIsInterrupted)
			return HRESULT_FROM_WIN32(ERROR_CANCELLED);

		if(m_nDuration > 0L && m_nFrequency > 0L)
		{
			if((m_nDelayStep % m_nFrequency) == 0L)
			{
				if(m_hDelayTimer)
				{
					LARGE_INTEGER liDueTime;
					liDueTime.LowPart = -10000L * m_nDuration;
					liDueTime.HighPart = -1L;
					bool bBreak = false;

					if(::SetWaitableTimer(m_hDelayTimer, &liDueTime, 0L, NULL, NULL, FALSE))
					{
						MSG msg;

						while(WAIT_OBJECT_0 != ::MsgWaitForMultipleObjects(1, &m_hDelayTimer, FALSE, INFINITE, QS_ALLINPUT)) // QS_ALLEVENTS
						{
							while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
							{
								::TranslateMessage(&msg);
								::DispatchMessage(&msg);

								if(m_bIsInterrupted)
								{
									::CancelWaitableTimer(m_hDelayTimer);
									return HRESULT_FROM_WIN32(ERROR_CANCELLED);
								}

								if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hDelayTimer, 0L))
								{
									bBreak = true;
									break;
								}
							}
							::Sleep(0L);

							if(bBreak)
								break;
						}
						::CancelWaitableTimer(m_hDelayTimer);
					}
					else
						::Sleep(m_nDuration);
				}

				m_nDelayStep = 0L;
			}
			m_nDelayStep++;
		}
		return S_OK;
	}

	STDMETHODIMP Interrupt()
	{
		m_bIsInterrupted = VARIANT_TRUE;
		return S_OK;
	}

	STDMETHODIMP Reset()
	{
		m_bIsInterrupted = VARIANT_FALSE;
		m_nDelayStep = 0L;
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(EtsProcessDelay), CEtsProcessDelay)

#endif //__ETSPROCESSDELAY_H__
