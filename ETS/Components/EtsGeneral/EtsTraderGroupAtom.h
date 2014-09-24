// EtsTraderGroupAtom.h : Declaration of the CEtsTraderGroupAtom
#ifndef __ETSTRADERGROUPATOM_H__
#define __ETSTRADERGROUPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsTraderColl.h"

_COM_SMARTPTR_TYPEDEF(IEtsTraderGroupAtom, IID_IEtsTraderGroupAtom);

struct __EtsTraderGroupAtom
{
	LONG			m_nID;
	_bstr_t			m_bstrName;
	_bstr_t			m_bstrDesc;
	IEtsTraderCollPtr	m_spTrader;

	__EtsTraderGroupAtom()
		: m_nID(0L)
	{
	}
};

// CEtsTraderGroupAtom

class ATL_NO_VTABLE CEtsTraderGroupAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsTraderGroupAtom, &CLSID_EtsTraderGroupAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsTraderGroupAtom>,
	public IDispatchImpl<IEtsTraderGroupAtom, &IID_IEtsTraderGroupAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsTraderGroupAtom
{
public:
	CEtsTraderGroupAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSTRADERGROUPATOM)


BEGIN_COM_MAP(CEtsTraderGroupAtom)
	COM_INTERFACE_ENTRY(IEtsTraderGroupAtom)
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
			CComObject<CEtsTraderColl>* pColl = NULL;
			_CHK(CComObject<CEtsTraderColl>::CreateInstance(&pColl), _T("Fail to create traders."));
			m_spTrader.Attach(pColl, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsTraderGroupAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_spTrader = NULL;
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTRT_PROPERTY(Name, m_bstrName)
	IMPLEMENT_BSTRT_PROPERTY(Desc, m_bstrDesc)
	IMPLEMENT_OBJECT_PROPERTY(IEtsTraderColl*, Trader, m_spTrader)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsTraderGroupAtom), CEtsTraderGroupAtom)

#endif //__ETSTRADERGROUPATOM_H__
