// MmQvOptPairAtom.h : Declaration of the CMmQvOptPairAtom
#ifndef __MMQVOPTPAIRATOM_H__
#define __MMQVOPTPAIRATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvOptAtom.h"

_COM_SMARTPTR_TYPEDEF(IMmQvOptPairAtom, IID_IMmQvOptPairAtom);

struct __MmQvOptPairAtom
{
	LONG				m_nRootID;
	IMmQvOptAtomPtr		m_spOpt[2];

	__MmQvOptPairAtom()
		: m_nRootID(0L)
	{
	}
};

// CMmQvOptPairAtom
class ATL_NO_VTABLE CMmQvOptPairAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvOptPairAtom, &CLSID_MmQvOptPairAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvOptPairAtom>,
	public IDispatchImpl<IMmQvOptPairAtom, &IID_IMmQvOptPairAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvOptPairAtom
{
public:
	CMmQvOptPairAtom()
		:m_dVolaCache(BAD_DOUBLE_VALUE)
		,m_bSetVolaCache(false)
	{
		m_pOpt[0] = NULL;
		m_pOpt[1] = NULL;
		m_pUnkMarshaler = NULL;

	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVOPTPAIRATOM)


BEGIN_COM_MAP(CMmQvOptPairAtom)
	COM_INTERFACE_ENTRY(IMmQvOptPairAtom)
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
				_CHK(CComObject<CMmQvOptAtom>::CreateInstance(&m_pOpt[0]), _T("Fail to create option."));
				m_spOpt[0].Attach(m_pOpt[0], TRUE);

				_CHK(CComObject<CMmQvOptAtom>::CreateInstance(&m_pOpt[1]), _T("Fail to create option."));
				m_spOpt[1].Attach(m_pOpt[1], TRUE);
		
				_CHK(CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p), _T("Unable to create custom Marshaller"));
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvOptPairAtom, e.Error());
		}
		return S_OK;
	}
	
	void FinalRelease() 
	{
		m_spOpt[0] = NULL;
		m_spOpt[1] = NULL;
		m_pUnkMarshaler.Release();

	}

private:
	CComPtr<IUnknown> m_pUnkMarshaler;

	double                      m_dVolaCache;
	bool						m_bSetVolaCache;

public:
	CComObject<CMmQvOptAtom>*   m_pOpt[2];

public:
	
	// This vola cache is for data loading only
	bool	IsSetVolaCache(){ return m_bSetVolaCache;}
	void	SetVola(double dVola){m_dVolaCache = dVola; m_bSetVolaCache = true;}
	double	GetVola(){return m_dVolaCache; }


	CComObject<CMmQvOptAtom>* GetOpt(EtsOptionTypeEnum enOptType)
	{
		return m_pOpt[enOptType != enOtPut ? 1L : 0L];
	}
public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, RootID, m_nRootID)
	STDMETHOD(get_Opt)(EtsOptionTypeEnum enOptType, IMmQvOptAtom** pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvOptPairAtom), CMmQvOptPairAtom)

#endif //__MMQVOPTPAIRATOM_H__