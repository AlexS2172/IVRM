// EtsFutAtom.h : Declaration of the CEtsFutAtom
#ifndef __ETSFUTATOM_H_
#define __ETSFUTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "FutureContract.h"
#include "ContractColl.h"

// CEtsFutAtom
class ATL_NO_VTABLE CEtsFutAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsFutAtom, &CLSID_EtsFutAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsFutAtom>,
	public IDispatchImpl<IEtsFutAtom, &IID_IEtsFutAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CFutureContract
{
public:
	CEtsFutAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSFUTATOM)


BEGIN_COM_MAP(CEtsFutAtom)
	COM_INTERFACE_ENTRY(IEtsFutAtom)
	COM_INTERFACE_ENTRY(IContract)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
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
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsFutAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		CFutureContract::Clear();
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
	
	STDMETHOD(putref_Asset)(IAsset* NewVal);
	STDMETHOD(get_Asset)(IAsset** pVal);

	STDMETHOD(putref_BaseContract)(IContract* NewVal);
	STDMETHOD(get_BaseContract)(IContract** pVal);

	IMPLEMENT_OBJECT_PROPERTY(IContractColl*,	DerivativeContracts,	m_spDerivativeContracts)

	STDMETHOD(GetPricingUnit)(/*out*/DOUBLE* Value);
	STDMETHOD(GetContractSizeInAsset)(/*out*/DOUBLE* Value);

	//IEtsFutAtom
	IMPLEMENT_SIMPLE_PROPERTY(LONG, FutRootID, m_nFutRootID)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, MaturityDate, m_dtMaturityDate)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsAmerican, m_bIsAmerican)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsActive, m_bIsActive)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HaveOptions, m_bHaveOptions)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL,	ShowInFutureContractEq, m_bShowInFutureContractEq)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		TickSize,				m_dTickSize)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		TickValue,			m_dTickValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PriceQuotationUnit,		m_dPriceQuotationUnit)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		QuoteFormat,			m_dQuoteFormat)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, FutureBasis, m_dBasis)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseInCalc, m_bUseInCalc)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ActiveFutureRatio, m_dActiveFutureRatio)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, K, m_dK)
    IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, KEq, m_dKEq)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, MultOptDltEq, m_bMultOptDltEq)

	//IvRm compatibility  [to remove]
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ManualActivePrice, m_dManualPrice)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceClose, m_dClosePrice)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceTheoClose, m_dTheoClosePrice)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsFutAtom), CEtsFutAtom)

#endif //__ETSFUTATOM_H_
