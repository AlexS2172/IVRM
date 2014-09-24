// EtsDivAtom.h : Declaration of the CEtsDivAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"

struct _EtsDivAtom
{
	DATE	m_dDivDate;
	DOUBLE	m_dDivAmount;
	_EtsDivAtom():m_dDivDate(0),m_dDivAmount(0)
	{
	}

};

// CEtsDivAtom

_COM_SMARTPTR_TYPEDEF(IEtsDivAtom, IID_IEtsDivAtom);

class ATL_NO_VTABLE CEtsDivAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsDivAtom, &CLSID_EtsDivAtom>,
	public ISupportErrorInfoImpl<&CLSID_EtsDivAtom>,
	public IDispatchImpl<IEtsDivAtom, &IID_IEtsDivAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public _EtsDivAtom
{
public:
	CEtsDivAtom()
	{
		m_pUnkMarshaler = NULL;
	}

	BEGIN_PROP_MAP(CEtsDivAtom)
	END_PROP_MAP()


DECLARE_REGISTRY_RESOURCEID(IDR_ETSDIVATOM)


BEGIN_COM_MAP(CEtsDivAtom)
	COM_INTERFACE_ENTRY(IEtsDivAtom)
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
	IMPLEMENT_SIMPLE_PROPERTY(DATE, DivDate, m_dDivDate);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, DivAmount, m_dDivAmount);

};

OBJECT_ENTRY_AUTO(__uuidof(EtsDivAtom), CEtsDivAtom)
