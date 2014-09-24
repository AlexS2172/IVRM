// MmQvSpreadData.h : Declaration of the CMmQvSpreadData

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmQuotes.h"
#include "MmQvOptAtom.h"
#include "MmQvFutAtom.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

_COM_SMARTPTR_TYPEDEF(IMmQvSpreadData, IID_IMmQvSpreadData);

class _CMmQvSpreadData
{
public:
	LONG		m_nQty;
	LONG        m_nUnderlyingID;
	DOUBLE		m_dPrice;
	DOUBLE		m_dTheoPrice;
	DOUBLE		m_dBid;
	DOUBLE		m_dAsk;
	DOUBLE		m_dDelta;
	DOUBLE		m_dGamma;
	DOUBLE		m_dVega;
	DOUBLE		m_dTheta;
	DOUBLE		m_dRho;
	_bstr_t				m_bsSeries;
	EtsContractTypeEnum m_enType;
	long				m_nContractID;
	IMmQvOptAtomPtr     m_spOpt;
	IMmQvFutAtomPtr     m_spFut;
	LONG		m_nFutRootID;
	LONG		m_nFutID;



public:
	_CMmQvSpreadData()
		:m_nQty(BAD_LONG_VALUE)
		,m_dPrice(BAD_DOUBLE_VALUE)
		,m_dTheoPrice(BAD_DOUBLE_VALUE)
		,m_dBid(BAD_DOUBLE_VALUE)
		,m_dAsk(BAD_DOUBLE_VALUE)
		,m_dDelta(BAD_DOUBLE_VALUE)
		,m_dGamma(BAD_DOUBLE_VALUE)
		,m_dVega(BAD_DOUBLE_VALUE)
		,m_dTheta(BAD_DOUBLE_VALUE)
		,m_dRho(BAD_DOUBLE_VALUE)
		,m_nContractID(BAD_LONG_VALUE)
		,m_nUnderlyingID(BAD_LONG_VALUE)
		,m_enType(enCtNone)
		,m_nFutRootID(BAD_LONG_VALUE)
		,m_nFutID(BAD_LONG_VALUE)
	{
	}
	void Clear()
	{
		m_nQty			= BAD_LONG_VALUE;
		m_dPrice		= BAD_DOUBLE_VALUE;
		m_dTheoPrice	= BAD_DOUBLE_VALUE;
		m_dBid			= BAD_DOUBLE_VALUE;
		m_dAsk			= BAD_DOUBLE_VALUE;
		m_dDelta		= BAD_DOUBLE_VALUE;
		m_dGamma		= BAD_DOUBLE_VALUE;
		m_dVega			= BAD_DOUBLE_VALUE;
		m_dTheta		= BAD_DOUBLE_VALUE;
		m_dRho			= BAD_DOUBLE_VALUE;
		m_enType		= enCtNone;
		m_nContractID	= BAD_LONG_VALUE;
		m_nUnderlyingID = BAD_LONG_VALUE;
		m_bsSeries		= L"";
		m_spOpt         = NULL;
		m_spFut			= NULL;
		m_nFutRootID	= BAD_LONG_VALUE;
		m_nFutID		= BAD_LONG_VALUE;
	}
};


// CMmQvSpreadData

class ATL_NO_VTABLE CMmQvSpreadData :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvSpreadData, &CLSID_MmQvSpreadData>,
	public ISupportErrorInfoImpl<&IID_IMmQvSpreadData>,
	public IDispatchImpl<IMmQvSpreadData, &IID_IMmQvSpreadData, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public _CMmQvSpreadData

{
public:
	CMmQvSpreadData()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVSPREADDATA)


BEGIN_COM_MAP(CMmQvSpreadData)
	COM_INTERFACE_ENTRY(IMmQvSpreadData)
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

	IMPLEMENT_SIMPLE_PROPERTY(LONG,		Qty, m_nQty);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	Bid, m_dBid);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	Ask, m_dAsk);
	IMPLEMENT_SIMPLE_PROPERTY(LONG,		FutRootID, m_nFutRootID);
	IMPLEMENT_SIMPLE_PROPERTY(LONG,		FutID, m_nFutID);

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	Price,		m_dPrice);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	TheoPrice,	m_dTheoPrice);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	Delta,		m_dDelta);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	Gamma,		m_dGamma);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	Vega,		m_dVega);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	Theta,		m_dTheta);
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	Rho,		m_dRho);
	IMPLEMENT_SIMPLE_PROPERTY(LONG,		ContractID,	m_nContractID);
	IMPLEMENT_SIMPLE_PROPERTY(LONG,		UnderlyingID,	m_nUnderlyingID);
	IMPLEMENT_BSTRT_PROPERTY (			Series,		m_bsSeries);
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum,	ContractType,	m_enType);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvOptAtom*, Opt, m_spOpt);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvFutAtom*, Fut, m_spFut);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvSpreadData), CMmQvSpreadData)
