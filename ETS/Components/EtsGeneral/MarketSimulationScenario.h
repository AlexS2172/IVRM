// MarketSimulationScenario.h : Declaration of the CMarketSimulationScenario

#pragma once
#include "resource.h"       // main symbols

#include "EtsGeneral.h"
#include "MarketSimulator.h"
#include "CommonSPtr.h"


typedef CComRecPtr<KeyValue, &LIBID_EtsGeneralLib>	__KeyValue;

// CMarketSimulationScenario
struct __MarketSimulatioScenario{
	CComBSTR	m_ShiftDriverAsset;

	long		m_TimeShiftUnit;
	double		m_TimeShiftValue;
	long		m_TimeShiftScenario;

	long		m_PriceShiftUnit;
	double		m_PriceShiftValue;
	long		m_PriceShiftScenario;

	long		m_VolatilityShiftUnit;
	double		m_VolatilityShiftValue;
	long		m_VolatilityShiftScenario;
	long		m_MaturityWeighted;

	IAssetPtr	m_PriceShiftDriver;

	CMarketSimulator::ptr	m_Simulator;

	typedef std::map<std::string, double>	container;
	typedef std::map<long, container>	scenario_specific_container;

	scenario_specific_container m_container;

	//ctor
	__MarketSimulatioScenario():
	m_ShiftDriverAsset(""),
	m_TimeShiftScenario(0),
	m_TimeShiftUnit(0),
	m_TimeShiftValue(0.),
	m_PriceShiftScenario(0),
	m_PriceShiftUnit(0),
	m_PriceShiftValue(0.),
	m_VolatilityShiftScenario(0),
	m_VolatilityShiftUnit(0),
	m_VolatilityShiftValue(0),
	m_Simulator(CMarketSimulator::ptr()),
	m_PriceShiftDriver(0),
	m_MaturityWeighted(0){
		m_container.clear();
	};
};

class ATL_NO_VTABLE CMarketSimulationScenario :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMarketSimulationScenario, &CLSID_MarketSimulationScenario>,
	public IDispatchImpl<IMarketSimulationScenario, &IID_IMarketSimulationScenario, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MarketSimulatioScenario
{
public:


	CMarketSimulationScenario():
	__MarketSimulatioScenario(){
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_MARKETSIMULATIONSCENARIO)

	BEGIN_COM_MAP(CMarketSimulationScenario)
		COM_INTERFACE_ENTRY(IMarketSimulationScenario)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct(){
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease(){
		m_PriceShiftDriver = NULL;
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;
public:

	IMPLEMENT_BSTR_PROPERTY(ShiftDriverAsset, m_ShiftDriverAsset)

	IMPLEMENT_SIMPLE_PROPERTY(long,		TimeShiftUnit,			m_TimeShiftUnit)
	IMPLEMENT_SIMPLE_PROPERTY(long,		PriceShiftUnit,			m_PriceShiftUnit)
	IMPLEMENT_SIMPLE_PROPERTY(long,		VolatilityShiftUnit,	m_VolatilityShiftUnit)
	
	IMPLEMENT_SIMPLE_PROPERTY(double,	TimeShiftValue,			m_TimeShiftValue)
	IMPLEMENT_SIMPLE_PROPERTY(double,	PriceShiftValue,		m_PriceShiftValue)
	IMPLEMENT_SIMPLE_PROPERTY(double,	VolatilityShiftValue,	m_VolatilityShiftValue)

	IMPLEMENT_SIMPLE_PROPERTY(long,		TimeShiftScenario,		m_TimeShiftScenario)
	IMPLEMENT_SIMPLE_PROPERTY(long,		PriceShiftScenario,		m_PriceShiftScenario)
	IMPLEMENT_SIMPLE_PROPERTY(long,		VolatilityShiftScenario,m_VolatilityShiftScenario)
	IMPLEMENT_SIMPLE_PROPERTY(long,		MaturityWeghted,		m_MaturityWeighted)

	IMPLEMENT_OBJECT_PROPERTY(IAsset*,	PriceShiftDriver,		m_PriceShiftDriver)

	STDMETHOD(RebuildScenario)();

	CMarketSimulator::ptr getMarketSimulator();

	STDMETHOD(GetSpecificValues)(/*[in]*/long Scenario, 
								 /*[in, out]*/SAFEARRAY** SpecificValues);

	STDMETHOD(SetSpecificValues)(/*[in]*/long Scenario, 
								 /*[in, out]*/SAFEARRAY** SpecificValues);
};

OBJECT_ENTRY_AUTO(__uuidof(MarketSimulationScenario), CMarketSimulationScenario)
