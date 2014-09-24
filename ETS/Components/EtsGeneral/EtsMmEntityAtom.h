// EtsMmEntityAtom.h : Declaration of the CEtsMmEntityAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"
#include "EtsMmFilterAtomColl.h"

_COM_SMARTPTR_TYPEDEF(IEtsMmEntityAtom, IID_IEtsMmEntityAtom);

// CEtsMmEntityAtom
class _CEtsMmEntityAtom
{
public:
	_CEtsMmEntityAtom()
		:m_lID(0L)
		,m_lData(0L)
		,m_lData2(0L)
		,m_dData3(0.0)
		,m_dValue1(0.0)
		,m_dValue2(0.0)
		,m_dValue3(0.0)
		,m_dValue4(0.0)
	{}

public:
	long		m_lID;
	_bstr_t     m_bsName;
	long		m_lData;
	long		m_lData2;
	double		m_dData3;

	double		m_dValue1;
	double		m_dValue2;
	double		m_dValue3;
	double		m_dValue4;

	_bstr_t     m_bsValue1;
	_bstr_t     m_bsValue2;
	_variant_t  m_vtValue;

	IEtsMmFilterAtomCollPtr m_spExtData;

};

class ATL_NO_VTABLE CEtsMmEntityAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsMmEntityAtom, &CLSID_EtsMmEntityAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsMmEntityAtom>,
	public IDispatchImpl<IEtsMmEntityAtom, &IID_IEtsMmEntityAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public _CEtsMmEntityAtom
{
public:
	CEtsMmEntityAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSMMENTITYATOM)


BEGIN_COM_MAP(CEtsMmEntityAtom)
	COM_INTERFACE_ENTRY(IEtsMmEntityAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_lID);
	IMPLEMENT_BSTRT_PROPERTY( Name,    m_bsName);
	IMPLEMENT_BSTRT_PROPERTY( sValue1, m_bsValue1);
	IMPLEMENT_BSTRT_PROPERTY( sValue2, m_bsValue2);
	IMPLEMENT_VARIANT_PROPERTY(vValue, m_vtValue);

	IMPLEMENT_SIMPLE_PROPERTY(LONG, Data, m_lData);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Data2, m_lData2);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Data3, m_dData3);

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, dValue1, m_dValue1);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, dValue2, m_dValue2);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, dValue3, m_dValue3);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, dValue4, m_dValue4);
	IMPLEMENT_OBJECT_PROPERTY(IEtsMmFilterAtomColl*, ExtData, m_spExtData)


};

OBJECT_ENTRY_AUTO(__uuidof(EtsMmEntityAtom), CEtsMmEntityAtom)
