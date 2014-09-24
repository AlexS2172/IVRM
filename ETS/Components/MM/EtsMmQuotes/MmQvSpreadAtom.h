// MmQvSpreadAtom.h : Declaration of the CMmQvSpreadAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmQuotes.h"
#include "MmQvUndAtom.h"
#include "MmQvRowData.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

_COM_SMARTPTR_TYPEDEF(IMmQvSpreadAtom, IID_IMmQvSpreadAtom);


// CMmQvSpreadAtom

class ATL_NO_VTABLE CMmQvSpreadAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvSpreadAtom, &CLSID_MmQvSpreadAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvSpreadAtom>,
	public IDispatchImpl<IMmQvSpreadAtom, &IID_IMmQvSpreadAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMmQvSpreadAtom()
		:m_enSpreadType(SPT_NONE)
		,m_enOptionType(enOtPut)
		,m_lExchangeId(BAD_LONG_VALUE)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVSPREADATOM)


BEGIN_COM_MAP(CMmQvSpreadAtom)
	COM_INTERFACE_ENTRY(IMmQvSpreadAtom)
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
	IMmQvUndAtomPtr				m_spUndAtom;
	IMmQvFutAtomPtr				m_spFutAtom;
	IMmQvRowDataPtr				m_spRowData;
	MmQvSpreadItemTypeEnum		m_enSpreadType;
	EtsOptionTypeEnum			m_enOptionType;
	long						m_lExchangeId;
public:

	IMPLEMENT_SIMPLE_PROPERTY(MmQvSpreadItemTypeEnum,	SpreadType,	m_enSpreadType);
	IMPLEMENT_OBJECT_PROPERTY(IMmQvUndAtom*,			Und,		m_spUndAtom);
	IMPLEMENT_OBJECT_PROPERTY(IMmQvFutAtom*,			Fut,		m_spFutAtom);

	STDMETHOD(get_SpreadData)(IMmQvSpreadData** ppVal);
	STDMETHOD(get_OptionItemPositiion)(LONG* pVal);
	STDMETHOD(get_Quote)(IMmQvQuoteAtom** ppVal);
	STDMETHOD(get_Opt)(IMmQvOptAtom** ppVal);

	STDMETHOD(putref_RowData)( enum EtsOptionTypeEnum enType, IMmQvRowData* newVal);

};

OBJECT_ENTRY_AUTO(__uuidof(MmQvSpreadAtom), CMmQvSpreadAtom)
