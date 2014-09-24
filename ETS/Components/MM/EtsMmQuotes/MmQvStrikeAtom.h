// MmQvStrikeAtom.h : Declaration of the CMmQvStrikeAtom
#ifndef __MMQVSTRIKEATOM_H__
#define __MMQVSTRIKEATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvOptAtom.h"
#include "MmQvOptPairColl.h"

_COM_SMARTPTR_TYPEDEF(IMmQvStrikeAtom, IID_IMmQvStrikeAtom);

struct __MmQvStrikeAtom
{
	DOUBLE				m_dStrike;
	IMmQvOptPairCollPtr	m_spOptPair;
	VARIANT_BOOL		m_bVisible;

	__MmQvStrikeAtom()
		: m_dStrike(0.), m_bVisible(VARIANT_FALSE)
	{
	}
};

// CMmQvStrikeAtom

class ATL_NO_VTABLE CMmQvStrikeAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvStrikeAtom, &CLSID_MmQvStrikeAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvStrikeAtom>,
	public IDispatchImpl<IMmQvStrikeAtom, &IID_IMmQvStrikeAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvStrikeAtom
{
public:
	CMmQvStrikeAtom()
		:m_pOptPair(NULL)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVSTRIKEATOM)


BEGIN_COM_MAP(CMmQvStrikeAtom)
	COM_INTERFACE_ENTRY(IMmQvStrikeAtom)
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
			
			_CHK(CComObject<CMmQvOptPairColl>::CreateInstance(&m_pOptPair),_T("Fail to create option pairs collection."));
			m_spOptPair.Attach(m_pOptPair, TRUE);

			_CHK(CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p), _T("Unable to create custom Marshaller"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvStrikeAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spOptPair = NULL;
		m_pUnkMarshaler.Release();
	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;


public:
	 CComObject<CMmQvOptPairColl>*	 m_pOptPair;
public:

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvOptPairColl*, OptPair, m_spOptPair)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Visible, m_bVisible)
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvStrikeAtom), CMmQvStrikeAtom)

#endif //__MMQVSTRIKEATOM_H__