// CalculationParametrs.h : Declaration of the CCalculationParametrs

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"


//#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
//#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
//#endif


_COM_SMARTPTR_TYPEDEF(ICalculationParametrs, IID_ICalculationParametrs);
// CCalculationParametrs Interface
struct __CalculationParametrs 
{
public:
	VARIANT_BOOL	m_bUseTimePrecision;

	__CalculationParametrs(): m_bUseTimePrecision(VARIANT_TRUE){};
};

class ATL_NO_VTABLE CCalculationParametrsAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCalculationParametrsAtom, &CLSID_CalculationParametrs>,
	public IDispatchImpl<ICalculationParametrs, &IID_ICalculationParametrs, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __CalculationParametrs
{
public:
	CCalculationParametrsAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CALCULATIONPARAMETRS)


BEGIN_COM_MAP(CCalculationParametrsAtom)
	COM_INTERFACE_ENTRY(ICalculationParametrs)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseTimePrecision, m_bUseTimePrecision)
};

OBJECT_ENTRY_AUTO(__uuidof(CalculationParametrs), CCalculationParametrsAtom)
