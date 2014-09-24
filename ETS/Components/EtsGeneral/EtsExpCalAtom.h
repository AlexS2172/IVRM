// EtsExpCalAtom.h : Declaration of the CEtsExpCalAtom
#ifndef __ETSEXPCALATOM_H__
#define __ETSEXPCALATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsExpMonthColl.h"

_COM_SMARTPTR_TYPEDEF(IEtsExpCalAtom, IID_IEtsExpCalAtom);

struct __EtsExpCalAtom
{
	LONG				m_nID;
	CComBSTR			m_bstrName;
	CComBSTR			m_bstrDesc;
	IEtsExpMonthCollPtr	m_spExpByID;
	IEtsExpMonthCollPtr	m_spExpByMonth;
	LONG				m_bHidden;	// true for internal calendars used by flex options

	__EtsExpCalAtom()
		: m_nID(0L), m_bHidden(0)
	{
	}
};

// CEtsExpCalAtom

class ATL_NO_VTABLE CEtsExpCalAtom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEtsExpCalAtom, &CLSID_EtsExpCalAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsExpCalAtom>,
	public IDispatchImpl<IEtsExpCalAtom, &IID_IEtsExpCalAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsExpCalAtom
{
public:
	CEtsExpCalAtom()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSEXPCALATOM)


BEGIN_COM_MAP(CEtsExpCalAtom)
	COM_INTERFACE_ENTRY(IEtsExpCalAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		try
		{
			CComObject<CEtsExpMonthColl>* pEtsExpMonthColl = NULL;
			_CHK(CComObject<CEtsExpMonthColl>::CreateInstance(&pEtsExpMonthColl), _T("Fail to create expiry months."));
			m_spExpByID.Attach(pEtsExpMonthColl,TRUE);

			_CHK(CComObject<CEtsExpMonthColl>::CreateInstance(&pEtsExpMonthColl), _T("Fail to create expiry months."));
			m_spExpByMonth.Attach(pEtsExpMonthColl, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsExpCalAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spExpByID = NULL;
		m_spExpByMonth = NULL;
	}

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)
	IMPLEMENT_BSTR_PROPERTY(Desc, m_bstrDesc)
	IMPLEMENT_OBJECT_PROPERTY(IEtsExpMonthColl*, ExpByID, m_spExpByID)
	IMPLEMENT_OBJECT_PROPERTY(IEtsExpMonthColl*, ExpByMonth, m_spExpByMonth)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Hidden, m_bHidden)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsExpCalAtom), CEtsExpCalAtom)

#endif //__ETSEXPCALATOM_H__
