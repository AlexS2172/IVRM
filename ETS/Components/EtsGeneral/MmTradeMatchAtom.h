// MmTradeMatchAtom.h : Declaration of the CMmTradeMatchAtom

#pragma once
#include "resource.h"       // main symbols

#include "MmTradeInfoAtom.h"
#include "MmTradeInfoColl.h"
#include "EtsGeneral.h"

#ifndef __MM_TRADE_MATCH_ATOM_H__
#define __MM_TRADE_MATCH_ATOM_H__

#pragma once

_COM_SMARTPTR_TYPEDEF(IMmTradeMatchAtom, IID_IMmTradeMatchAtom);

struct __MmTradeMatchAtom
{
	IMmTradeInfoAtomPtr			m_spTrdActual;
	IMmTradeInfoCollPtr			m_spTrdManual;
	IMmTradeInfoAtomPtr			m_spTrdMatch;
	VARIANT_BOOL				m_bUpdated;

	__MmTradeMatchAtom():m_bUpdated(VARIANT_FALSE)
	{
	}
};

// CMmTradeMatchAtom

class ATL_NO_VTABLE CMmTradeMatchAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmTradeMatchAtom, &CLSID_MmTradeMatchAtom>,
	public ISupportErrorInfo,
	public IDispatchImpl<IMmTradeMatchAtom, &IID_IMmTradeMatchAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	__MmTradeMatchAtom
{
public:
	CMmTradeMatchAtom()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MMTRADEMATCHATOM)


	BEGIN_COM_MAP(CMmTradeMatchAtom)
		COM_INTERFACE_ENTRY(IMmTradeMatchAtom)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		try
		{
			CComObject<CMmTradeInfoAtom>* pObject = NULL;
			_CHK(CComObject<CMmTradeInfoAtom>::CreateInstance(&pObject), _T("Fail to create trades actual atom"));
			m_spTrdActual.Attach(pObject, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeMatchAtom, e.Error());
		}		

		try
		{
			__CHECK_HRESULT(m_spTrdManual.CreateInstance(CLSID_MmTradeInfoColl), 
				_T("Fail to create trades manual collection"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeMatchAtom, e.Error());
		}		

		try
		{
			__CHECK_HRESULT(m_spTrdMatch.CreateInstance(CLSID_MmTradeInfoAtom), 
				_T("Fail to create trades match atom"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTradeMatchAtom, e.Error());
		}		

		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_spTrdActual = NULL;
		m_spTrdManual = NULL;
		m_spTrdMatch = NULL;
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IMPLEMENT_OBJECT_PROPERTY( IMmTradeInfoAtom* , TrdActual , m_spTrdActual )
	IMPLEMENT_OBJECT_PROPERTY( IMmTradeInfoColl* , TrdManual , m_spTrdManual )
	IMPLEMENT_OBJECT_PROPERTY( IMmTradeInfoAtom* , TrdMatch , m_spTrdMatch )
	IMPLEMENT_SIMPLE_PROPERTY( VARIANT_BOOL , Updated, m_bUpdated )
};

OBJECT_ENTRY_AUTO(__uuidof(MmTradeMatchAtom), CMmTradeMatchAtom)

#endif  //__MM_TRADE_MATCH_ATOM_H__
