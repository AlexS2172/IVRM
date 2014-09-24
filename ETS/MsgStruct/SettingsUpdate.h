// SettingsUpdate.h : Declaration of the CSettingsUpdate

#pragma once
#include "resource.h"       // main symbols

#include "MsgPacking.h"
#include "MsgPropImpl.h"

struct __SettingsUpdate
{
	BEGIN_PACKING
		FIELD_ENTRY(m_enGreeksCalculationMode)
		FIELD_ENTRY(m_lGreeksCalculationModel)
		FIELD_ENTRY(m_lGreeksCalculationPrecision)
		FIELD_ENTRY(m_lUseTheoVolaIfNoBid)
		FIELD_ENTRY(m_lUseTheoVolaIfBadMarketVola)
		FIELD_ENTRY(m_lPriceRoundingRule)
		FIELD_ENTRY(m_dPriceTolerance)
	END_PACKING

	TStaticField<GREEKS_CALCULATION_MODE>		m_enGreeksCalculationMode;
	LongF										m_lGreeksCalculationModel;
	LongF										m_lGreeksCalculationPrecision;
	LongF										m_lUseTheoVolaIfNoBid;
	LongF										m_lUseTheoVolaIfBadMarketVola;
	LongF										m_lPriceRoundingRule;
	DoubleF										m_dPriceTolerance;
};
// CSettingsUpdate
class ATL_NO_VTABLE CSettingsUpdate :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSettingsUpdate, &CLSID_SettingsUpdate>,
	public IDispatchImpl<ISettingsUpdate, &IID_ISettingsUpdate, &LIBID_MSGSTRUCTLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public INetPackingConstSizeDataImpl<CSettingsUpdate, __SettingsUpdate>
{
public:
	CSettingsUpdate()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SETTINGSUPDATE)


BEGIN_COM_MAP(CSettingsUpdate)
	COM_INTERFACE_ENTRY(ISettingsUpdate)
	COM_INTERFACE_ENTRY2(IDispatch, ISettingsUpdate)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	IMPLEMENT_SIMPLE_PROP(GREEKS_CALCULATION_MODE, GreeksCalculationMode, m_enGreeksCalculationMode)
	IMPLEMENT_LONG_PROP(GreeksCalculationModel, m_lGreeksCalculationModel)
	IMPLEMENT_LONG_PROP(GreeksCalculationPrecision, m_lGreeksCalculationPrecision)
	IMPLEMENT_LONG_PROP(UseTheoVolaIfNoBid, m_lUseTheoVolaIfNoBid)
	IMPLEMENT_LONG_PROP(UseTheoVolaIfBadMarketVola, m_lUseTheoVolaIfBadMarketVola)
	IMPLEMENT_LONG_PROP(PriceRoundingRule, m_lPriceRoundingRule)
	IMPLEMENT_DOUBLE_PROP(PriceTolerance, m_dPriceTolerance)

};

OBJECT_ENTRY_AUTO(__uuidof(SettingsUpdate), CSettingsUpdate)
