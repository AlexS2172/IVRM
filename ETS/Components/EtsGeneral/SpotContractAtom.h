// SpotContractAtom.h : Declaration of the CSpotContractAtom

#pragma once
#include "resource.h"       // main symbols

#include "SpotContract.h"
#include "ContractColl.h"

// CSpotContractAtom

class ATL_NO_VTABLE CSpotContractAtom :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSpotContractAtom, &CLSID_SpotContractAtom>,
	public ISupportErrorInfo,
	public IDispatchImpl<ISpotContractAtom, &IID_ISpotContractAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CSpotContract
{
public:
	CSpotContractAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SPOTCONTRACTATOM)


BEGIN_COM_MAP(CSpotContractAtom)
	COM_INTERFACE_ENTRY(ISpotContractAtom)
	COM_INTERFACE_ENTRY(IContract)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	//ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

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
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_ISpotContractAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		CSpotContract::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	//IContract
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

	//ISpotContract
	IMPLEMENT_SIMPLE_PROPERTY(LONG, TotalQtyInShares, m_nTotalQtyInShares)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, bShouldMultOptDeltaEq, m_bShouldMultOptDeltaEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, dKOptDeltaEq, m_dKOptDeltaEq)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, LotSize, m_nLotSize)

	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Kurt, m_dKurt)

	IMPLEMENT_SIMPLE_PROPERTY(LONG, PrimaryExchangeID, m_nPrimaryExchangeID)
	IMPLEMENT_BSTRT_PROPERTY(PrimaryExchImportID, m_bstrPrimaryExchImportID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, UndPosForRates, m_dUndPosForRates)
};

OBJECT_ENTRY_AUTO(__uuidof(SpotContractAtom), CSpotContractAtom)
