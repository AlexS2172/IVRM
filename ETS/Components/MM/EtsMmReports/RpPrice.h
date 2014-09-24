// RpPrice.h : Declaration of the CRpPrice

#pragma once
#include "resource.h"       // main symbols

#include "EtsMmReports.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif
namespace
{


class __CMmRpPrice
{
public:
	DOUBLE							m_dPriceBid;
	DOUBLE							m_dPriceAsk;
	DOUBLE							m_dPriceLast;
	DOUBLE							m_dPriceClose;
	DOUBLE							m_dNetChange;
	DOUBLE							m_dPriceTheoClose;


	__CMmRpPrice()
	{
		Clear();
	}

	void Clear()
	{

		m_dPriceBid = BAD_DOUBLE_VALUE;
		m_dPriceAsk = BAD_DOUBLE_VALUE;
		m_dPriceLast = BAD_DOUBLE_VALUE;
		m_dPriceClose = BAD_DOUBLE_VALUE;
		m_dNetChange = BAD_DOUBLE_VALUE;
		m_dPriceTheoClose = BAD_DOUBLE_VALUE;
	}
};
} ;

// CRpPrice

_COM_SMARTPTR_TYPEDEF(IMMRpPrice, IID_IMMRpPrice);

class ATL_NO_VTABLE CMMRpPrice :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMMRpPrice, &CLSID_MMRpPrice>,
	public IDispatchImpl<IMMRpPrice, &IID_IMMRpPrice, &LIBID_EtsMmReportsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	__CMmRpPrice
{
public:
	CMMRpPrice()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_RPPRICE)


BEGIN_COM_MAP(CMMRpPrice)
	COM_INTERFACE_ENTRY(IMMRpPrice)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Bid,       m_dPriceBid)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Ask,       m_dPriceAsk)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Last,      m_dPriceLast)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Close,     m_dPriceClose)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, NetChange, m_dNetChange)
		IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TheoClose, m_dPriceTheoClose)

};

OBJECT_ENTRY_AUTO(__uuidof(MMRpPrice), CMMRpPrice)
