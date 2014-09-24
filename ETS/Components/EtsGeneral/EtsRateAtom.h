// EtsRateAtom.h : Declaration of the CEtsRateAtom

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"


struct __EtsRateAtom
{
	LONG		m_nID;
	LONG		m_nPeriod;
	DOUBLE		m_dLongRate;
	DOUBLE		m_dShortRate;
	DOUBLE		m_dNeutralRate;

	EtsPeriodTypeEnum	m_enPeriodType;
	LONG				m_nPeriodAmount;

	__EtsRateAtom(): m_nID(BAD_LONG_VALUE), m_nPeriod(0),
					 m_dLongRate(0), m_dShortRate(0), m_dNeutralRate(0),
					 m_enPeriodType(enPeriodDays), m_nPeriodAmount(0)
	{
	}
};


_COM_SMARTPTR_TYPEDEF(IEtsRateAtom, IID_IEtsRateAtom);

// CEtsRateAtom
class ATL_NO_VTABLE CEtsRateAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsRateAtom, &CLSID_EtsRateAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsRateAtom>,
	public IDispatchImpl<IEtsRateAtom, &IID_IEtsRateAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsRateAtom
{
public:
	CEtsRateAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSRATEATOM)

BEGIN_COM_MAP(CEtsRateAtom)
	COM_INTERFACE_ENTRY(IEtsRateAtom)
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

	IMPLEMENT_SIMPLE_PROPERTY		 (LONG,		ID,				m_nID);
	IMPLEMENT_SIMPLEREADONLY_PROPERTY(LONG,		Period,			m_nPeriod);
	IMPLEMENT_SIMPLE_PROPERTY		 (DOUBLE,	LongRate,		m_dLongRate);
	IMPLEMENT_SIMPLE_PROPERTY		 (DOUBLE,	ShortRate,		m_dShortRate);
	IMPLEMENT_SIMPLE_PROPERTY		 (DOUBLE,	NeutralRate,	m_dNeutralRate);

	IMPLEMENT_SIMPLE_PROPERTY(EtsPeriodTypeEnum, PeriodType,	m_enPeriodType);
	IMPLEMENT_SIMPLE_PROPERTY(LONG,				 PeriodAmount,	m_nPeriodAmount);

public:

	STDMETHOD(CalcPeriodInDays)(DATE dToday)
	{
		vt_date dtStrtDate(dToday);
		vt_date dtDestDate(dToday);

		switch(m_enPeriodType)
		{
		case enPeriodDays:
			dtDestDate += m_nPeriodAmount;
			break;

		case enPeriodWeeks:
			dtDestDate += (m_nPeriodAmount * 7);
			break;

		case enPeriodMonths:
			{
				WORD nYear		= dtStrtDate.get_year();
				WORD nMonth		= dtStrtDate.get_month() + static_cast<WORD>(m_nPeriodAmount);

				while(nMonth > 12)
				{
					nYear++;
					nMonth -= 12;
				}

				dtDestDate = vt_date(nYear, nMonth, dtStrtDate.get_day());
			}
			break;

		case enPeriodYears:
			dtDestDate = vt_date(dtStrtDate.get_year() + static_cast<WORD>(m_nPeriodAmount), 
				dtStrtDate.get_month(), 
				dtStrtDate.get_day());
			break;

		default:
			break;
		}

		m_nPeriod = (dtDestDate - dtStrtDate).get_days();

		return S_OK;
	}

};

OBJECT_ENTRY_AUTO(__uuidof(EtsRateAtom), CEtsRateAtom)
