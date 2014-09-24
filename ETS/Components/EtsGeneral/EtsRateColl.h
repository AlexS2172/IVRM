// EtsRateColl.h : Declaration of the CEtsRateColl

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"
#include "EtsRateAtom.h"
#include <boost\scoped_array.hpp>

typedef IDispatchImpl<IEtsRateColl, &IID_IEtsRateColl, &LIBID_EtsGeneralLib>					IEtsRateCollDispImpl;
typedef ICollectionOnSTLMapOfInterfacePtrImpl<IEtsRateCollDispImpl, LONG, IEtsRateAtom, LONG>	IEtsRateCollImpl;

class CBorrowingRatesCurve
{
public:
	long						m_nID;
	CComBSTR					m_bstrName;
	InterestRateRuleTypeEnum	m_enRule;
	double						m_dPosThreshold;

public:
	CBorrowingRatesCurve(): m_nID(0),
							m_bstrName(L""),
							m_enRule(enRtNeutralRate),
							m_dPosThreshold(0.)
	{
	};
};

// CEtsRateColl
class ATL_NO_VTABLE CEtsRateColl :
	public CComObjectRootEx<CComMultiThreadModel/*CComMultiThreadModel*/>,
	public CComCoClass<CEtsRateColl, &CLSID_EtsRateColl>,
	public ISupportErrorInfoImpl<&IID_IEtsRateColl>,
	public IEtsRateCollImpl,
	public CBorrowingRatesCurve
{
public:
	CEtsRateColl()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_ETSRATECOLL)


	BEGIN_COM_MAP(CEtsRateColl)
		COM_INTERFACE_ENTRY(IEtsRateColl)
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

	STDMETHOD(Add)(LONG Key, IEtsRateAtom* Value, IEtsRateAtom** pRetVal);
	STDMETHOD(CopyTo)(IEtsRateColl** ppRetVal);

	STDMETHOD(GetShortRate)	 (DATE dtToday, DATE dtExpiry, DOUBLE* pdRate);
	STDMETHOD(GetLongRate)	 (DATE dtToday, DATE dtExpiry, DOUBLE* pdRate);
	STDMETHOD(GetNeutralRate)(DATE dtToday, DATE dtExpiry, DOUBLE* pdRate);

	STDMETHOD(GetPeriodInDays)(DATE dtToday, 
							   EtsPeriodTypeEnum enPeriodType, 
							   LONG nPeriodAmount, 
							   LONG* pnPeriod);

public:

	IEtsRateAtomPtr AddNew(LONG Key, CComObject<CEtsRateAtom>** pAtom = NULL);

public:
	//CBorrowingRatesCurve
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Name, m_bstrName)
	IMPLEMENT_SIMPLE_PROPERTY(InterestRateRuleTypeEnum,	Rule, m_enRule)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,	PosThreshold, m_dPosThreshold)

};

OBJECT_ENTRY_AUTO(__uuidof(EtsRateColl), CEtsRateColl)
