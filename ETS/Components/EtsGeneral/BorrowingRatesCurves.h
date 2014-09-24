// BorrowingRatesCurves.h : Declaration of the CBorrowingRatesCurves

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"
#include "EtsRateColl.h"
#include "CommonSPtr.h"

typedef IDispatchImpl<IBorrowingRatesCurves, &IID_IBorrowingRatesCurves, &LIBID_EtsGeneralLib>	IBorrowingRatesCurvesDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IBorrowingRatesCurvesDispImpl, LONG, IEtsRateColl, LONG>	IBorrowingRatesCurvesImpl;
// CBorrowingRatesCurves
class ATL_NO_VTABLE CBorrowingRatesCurves :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CBorrowingRatesCurves, &CLSID_BorrowingRatesCurves>,
	public IBorrowingRatesCurvesImpl
{
public:
	CBorrowingRatesCurves()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BORROWINGRATESCURVES)


BEGIN_COM_MAP(CBorrowingRatesCurves)
	COM_INTERFACE_ENTRY(IBorrowingRatesCurves)
	COM_INTERFACE_ENTRY(IDispatch)
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
	STDMETHOD(Add)(LONG Key, IEtsRateColl* Value, IEtsRateColl** pRetVal);

private:
	IEtsRateCollPtr AddNew(LONG Key, CComObject<CEtsRateColl>** pAtom = NULL);
};

OBJECT_ENTRY_AUTO(__uuidof(BorrowingRatesCurves), CBorrowingRatesCurves)
