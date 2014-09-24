// SyntheticSpotContractAtom.h : Declaration of the CSyntheticSpotContractAtom

#pragma once
#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "SyntheticSpotContract.h"
#include "ContractColl.h"
// CSyntheticSpotContractAtom

class ATL_NO_VTABLE CSyntheticSpotContractAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSyntheticSpotContractAtom, &CLSID_SyntheticSpotContractAtom>,
	public IDispatchImpl<ISyntheticSpotContractAtom, &IID_ISyntheticSpotContractAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CSyntheticSpotContract
{
public:
	CSyntheticSpotContractAtom()
	{
		m_pUnkMarshaler = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SYNTHETICSPOTCONTRACTATOM)


	BEGIN_COM_MAP(CSyntheticSpotContractAtom)
		COM_INTERFACE_ENTRY(IContract)
		COM_INTERFACE_ENTRY(ISyntheticSpotContract)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		try
		{
			CComObject<CContractColl>* pContractColl; 
			_CHK(CComObject<CContractColl>::CreateInstance(&pContractColl),  _T("Fail to create ContractColl object."));
			m_spDerivativeContracts.Attach(pContractColl, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_ISyntheticSpotContractAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_BSTR_PROPERTY(ContractName, m_bstrContractName)
	IMPLEMENT_BSTR_PROPERTY(ImportID, m_bstrImportID)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, ContractType, m_enType)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ManualPrice, m_dManualPrice)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ClosePrice, m_dClosePrice)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TheoClosePrice, m_dTheoClosePrice)
	
	STDMETHOD(Calculate)(/*[in]*/	ICalculationSettings* Settings,
						/*[in]*/	IMarketSimulationScenario* SimulationScenario,
						/*[out, retval]*/ struct IRisks* RetVal);

	STDMETHOD(GetRisks)(/*[out, retval]*/ struct IRisks* RetVal);
	STDMETHOD(GetQuotes)(/*[out, retval]*/ struct QuoteUpdateInfo* RetVal);
	STDMETHOD(FitToMarketVolatility)(DOUBLE Value);

	IMPLEMENT_OBJECT_PROPERTY(IContractColl*,	DerivativeContracts,	m_spDerivativeContracts)

	STDMETHOD(putref_Asset)(IAsset* NewVal);
	STDMETHOD(get_Asset)(IAsset** pVal);

	STDMETHOD(putref_BaseContract)(IContract* NewVal);
	STDMETHOD(get_BaseContract)(IContract** pVal);

	STDMETHOD(GetPricingUnit)(/*out*/DOUBLE* Value);
	STDMETHOD(GetContractSizeInAsset)(/*out*/DOUBLE* Value);
};

OBJECT_ENTRY_AUTO(__uuidof(SyntheticSpotContractAtom), CSyntheticSpotContractAtom)
