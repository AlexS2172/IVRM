// MmQvRequestAtom.h : Declaration of the CMmQvRequestAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmQuotes.h"
#include "MmQvExpAtom.h"
#include "MmQvUndAtom.h"
#include "MmQvOptAtom.h"
#include "MmQvFutAtom.h"
#include "MmQvFutRootAtom.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


struct _CMmQvRequestAtom
{

	IExchAtomPtr			m_spExch;
	IMmQvUndAtomPtr			m_spUnd;
	IMmQvOptAtomPtr			m_spOpt;
	IMmQvExpAtomPtr			m_spExp;
	IMmQvFutAtomPtr			m_spFut;
	IMmQvFutRootAtomPtr		m_spFutRoot;
	VARIANT_BOOL			m_bVisible;
};

// CMmQvRequestAtom
_COM_SMARTPTR_TYPEDEF(IMmQvRequestAtom, IID_IMmQvRequestAtom);


class ATL_NO_VTABLE CMmQvRequestAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvRequestAtom, &CLSID_MmQvRequestAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvRequestAtom>,
	public IDispatchImpl<IMmQvRequestAtom, &IID_IMmQvRequestAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public _CMmQvRequestAtom
{
public:
	CMmQvRequestAtom()
	{
		m_bVisible	=	VARIANT_FALSE;
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVREQUESTATOM)


BEGIN_COM_MAP(CMmQvRequestAtom)
	COM_INTERFACE_ENTRY(IMmQvRequestAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}
private:
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_OBJECT_PROPERTY(IExchAtom*, Exch, m_spExch)
	IMPLEMENT_OBJECT_PROPERTY(IMmQvUndAtom*, Und, m_spUnd)
	IMPLEMENT_OBJECT_PROPERTY(IMmQvOptAtom*, Opt, m_spOpt)
	IMPLEMENT_OBJECT_PROPERTY(IMmQvExpAtom*, Exp, m_spExp)
	IMPLEMENT_OBJECT_PROPERTY(IMmQvFutAtom*, Fut, m_spFut)
	IMPLEMENT_OBJECT_PROPERTY(IMmQvFutRootAtom*, FutRoot, m_spFutRoot)
	IMPLEMENT_SIMPLE_PROPERTY(BOOL, Visible, m_bVisible)

	STDMETHOD(get_QuoteUpdateParam)(struct QuoteUpdateParams* pQuoteUpdate);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvRequestAtom), CMmQvRequestAtom)
