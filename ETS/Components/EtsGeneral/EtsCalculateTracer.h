// EtsCalculateTracer.h : Declaration of the CEtsCalculateTracer

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"

#include "EgStdLib\Trace.h"


// CEtsCalculateTracer

class ATL_NO_VTABLE CEtsCalculateTracer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEtsCalculateTracer, &CLSID_EtsCalculateTracer>,
	public ISupportErrorInfoImpl<&IID_ITracer>,
	public IDispatchImpl<ITracer, &IID_IEtsCalculateTracer, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CEtsCalculateTracer()
	{
	}

DECLARE_CLASSFACTORY_SINGLETON(CEtsCalculateTracer)
DECLARE_REGISTRY_RESOURCEID(IDR_ETSCALCULATETRACER)

BEGIN_COM_MAP(CEtsCalculateTracer)
	COM_INTERFACE_ENTRY(ITracer)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			TCHAR sName[1000], sDrv [32], sPath[1000], sShName [256], sExt [32];
			GetModuleFileName(NULL, sName, 1000);
			_splitpath (sName, sDrv,sPath, sShName,sExt);

			CString strName = "CulcLogOf_";
			strName += sShName;
			m_pTrace = new EgStd::CEgTraceManager (strName);
			m_pTrace->Init ();
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_ITracer, e.Error());
		}
		return S_OK;
	}

	void FinalRelease() 
	{
		if (m_pTrace != NULL)
		{
			m_pTrace->Exit();
			delete m_pTrace;
		}
	}

	STDMETHOD (Trace) (EtsLogLevels lLogLevel, BSTR sCategory, BSTR sMsg);

protected:
	EgStd::CEgTraceManager* m_pTrace;


public:

};

OBJECT_ENTRY_AUTO(__uuidof(EtsCalculateTracer), CEtsCalculateTracer)
