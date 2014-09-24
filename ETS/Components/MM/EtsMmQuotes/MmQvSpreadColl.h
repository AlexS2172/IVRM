// MmQvSpreadColl.h : Declaration of the CMmQvSpreadColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmQuotes.h"
#include "MmQvSpreadAtom.h"
#include "MmQvSpreadData.h"
#include "_IMmQvSpreadCollEvents_CP.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

_COM_SMARTPTR_TYPEDEF(IMmQvSpreadColl, IID_IMmQvSpreadColl);


typedef IDispatchImpl<IMmQvSpreadColl, &IID_IMmQvSpreadColl, &LIBID_EtsMmQuotesLib> IMmQvSpreadCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IMmQvSpreadCollDispImpl, BSTR, IMmQvSpreadAtom, _bstr_t >	IMmQvSpreadCollImpl;

// CMmQvSpreadColl

class ATL_NO_VTABLE CMmQvSpreadColl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvSpreadColl, &CLSID_MmQvSpreadColl>,
	public ISupportErrorInfoImpl<&IID_IMmQvOptColl>,
	public IConnectionPointContainerImpl<CMmQvSpreadColl>,
	public IMmQvSpreadCollImpl,
	public CProxy_IMmQvSpreadCollEvents<CMmQvSpreadColl>,
	public _CMmQvSpreadData
{
public:
	CMmQvSpreadColl()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVSPREADCOLL)


BEGIN_COM_MAP(CMmQvSpreadColl)
	COM_INTERFACE_ENTRY(IMmQvSpreadColl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMmQvSpreadColl)
	CONNECTION_POINT_ENTRY(__uuidof(_IMmQvSpreadCollEvents))
END_CONNECTION_POINT_MAP()


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

	IMmQvSpreadAtomPtr GetSpreadItem(_bstr_t bsKey)
	{
		IMmQvSpreadAtomPtr spResult;
		CollType::iterator itrFind = m_coll.find(bsKey);
		if(itrFind != m_coll.end())
			spResult = itrFind->second;
		return spResult;
	}

	IMmQvSpreadAtomPtr AddNew(_bstr_t bsKey, CComObject<CMmQvSpreadAtom>** ppRetVal = NULL);

public:
	STDMETHOD(Add)(BSTR Key, IMmQvSpreadAtom* Value, IMmQvSpreadAtom** pRetVal);

	STDMETHOD(Clear)();
	STDMETHOD(Calculate)(BYTE bQuoteBidColBuy);
	STDMETHOD(UpdateUnderlying)(LONG nQty, LONG nExchID, IMmQvUndAtom* pUndAtom);
	STDMETHOD(UpdateFuture)(LONG nQty, LONG nExchID, IMmQvFutAtom* pFutAtom);
	STDMETHOD(UpdateOption)( LONG nQty, EtsOptionTypeEnum enOptType, IMmQvRowData* pRowData);

	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG,		TotalQty,		m_nQty);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DOUBLE,	TotalPrice,		m_dPrice);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DOUBLE,	TotalTheoPrice,	m_dTheoPrice);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DOUBLE,	TotalDelta,		m_dDelta);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DOUBLE,	TotalGamma,		m_dGamma);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DOUBLE,	TotalVega,		m_dVega);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DOUBLE,	TotalTheta,		m_dTheta);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(DOUBLE,	TotalRho,		m_dRho);


};

OBJECT_ENTRY_AUTO(__uuidof(MmQvSpreadColl), CMmQvSpreadColl)
