// EtsTraderAtom.h : Declaration of the CEtsTraderAtom
#ifndef __ETSTRADERATOM_H__
#define __ETSTRADERATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsTraderUndColl.h"
#include "EtsStrategyAtom.h"

_COM_SMARTPTR_TYPEDEF(IEtsTraderAtom, IID_IEtsTraderAtom);

struct __EtsTraderAtom
{
	LONG					m_nID;
	_bstr_t					m_bstrName;
	_bstr_t					m_bstrDesc;
	_bstr_t					m_bstrTntAccount;
	IEtsTraderUndCollPtr	m_spUnd;
	LONG					m_nGroupID;

	__EtsTraderAtom()
		: m_nID(0L), m_nGroupID(0L)
	{
	}
};

// CEtsTraderAtom

class ATL_NO_VTABLE CEtsTraderAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsTraderAtom, &CLSID_EtsTraderAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsTraderAtom>,
	public IDispatchImpl<IEtsTraderAtom, &IID_IEtsTraderAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsTraderAtom
{
public:
	CEtsTraderAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSTRADERATOM)


BEGIN_COM_MAP(CEtsTraderAtom)
	COM_INTERFACE_ENTRY(IEtsTraderAtom)
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
			CComObject<CEtsTraderUndColl>* pCreate = NULL;
			_CHK(CComObject<CEtsTraderUndColl>::CreateInstance(&pCreate), _T("Fail to create trader underlyings."));
			m_spUnd.Attach(pCreate, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsTraderAtom, e.Error());
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
	IMPLEMENT_BSTRT_PROPERTY(Name, m_bstrName)
	IMPLEMENT_BSTRT_PROPERTY(Desc, m_bstrDesc)
	IMPLEMENT_BSTRT_PROPERTY(TntAccount, m_bstrTntAccount)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsTraderUndColl*, Und, m_spUnd)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, GroupID, m_nGroupID)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsTraderAtom), CEtsTraderAtom)

#endif //__ETSTRADERATOM_H__
