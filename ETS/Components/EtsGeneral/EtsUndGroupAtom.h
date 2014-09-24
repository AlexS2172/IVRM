// EtsUndGroupAtom.h : Declaration of the CEtsUndGroupAtom
#ifndef __ETSUNDGROUPATOM_H__
#define __ETSUNDGROUPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "UndColl.h"

struct __EtsUndGroupAtom
{
	LONG				m_nID;
	CComBSTR			m_bstrName;
	CComBSTR			m_bstrDesc;
	IUndCollPtr			m_spUnd;

	DOUBLE				m_dRatio[2];

	__EtsUndGroupAtom()
		: m_nID(0L)
	{
		m_dRatio[0] = 0.;
		m_dRatio[1] = 0.;
	}

	DOUBLE& operator[](long idx){
		if (idx >= 0 && idx < enGrEnumSize){
			return m_dRatio[idx];
		}
		throw std::exception("Invalid idx");
	};

	void clear(){
		m_dRatio[0] = m_dRatio[1] = 0.;
	};
};

// CEtsUndGroupAtom

class ATL_NO_VTABLE CEtsUndGroupAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsUndGroupAtom, &CLSID_EtsUndGroupAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsUndGroupAtom>,
	public IDispatchImpl<IEtsUndGroupAtom, &IID_IEtsUndGroupAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsUndGroupAtom
{
public:
	CEtsUndGroupAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSUNDGROUPATOM)


BEGIN_COM_MAP(CEtsUndGroupAtom)
	COM_INTERFACE_ENTRY(IEtsUndGroupAtom)
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
			CComObject<CUndColl>* pColl = NULL;
			_CHK(CComObject<CUndColl>::CreateInstance(&pColl), _T("Fail to create underlyings."));

			m_spUnd.Attach(pColl, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsUndGroupAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_spUnd = NULL;
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)
	IMPLEMENT_BSTR_PROPERTY(Desc, m_bstrDesc)
	IMPLEMENT_OBJECT_PROPERTY(IUndColl*, Und, m_spUnd)

	STDMETHOD(get_Ratio)(GroupRatioEnum grIdx, DOUBLE* pVal);
	STDMETHOD(put_Ratio)(GroupRatioEnum grIdx, DOUBLE newVal);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsUndGroupAtom), CEtsUndGroupAtom)

#endif //__ETSUNDGROUPATOM_H__
