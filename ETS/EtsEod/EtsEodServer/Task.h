// Task.h : Declaration of the CTask

#pragma once
#include "resource.h"       // main symbols

#include "EtsEodServer.h"


// CTask

class ATL_NO_VTABLE CTask : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTask, &CLSID_Task>,
	public IDispatchImpl<ITask, &IID_ITask, &LIBID_EtsEodServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTask() : m_dtStartTime(0.)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TASK)


BEGIN_COM_MAP(CTask)
	COM_INTERFACE_ENTRY(ITask)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(get_StartTime)(DATE* pVal);
	STDMETHOD(put_StartTime)(DATE newVal);
private:
	DATE	m_dtStartTime;
};

OBJECT_ENTRY_AUTO(__uuidof(Task), CTask)
