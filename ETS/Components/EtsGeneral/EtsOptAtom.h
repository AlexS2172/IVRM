// EtsOptAtom.h : Declaration of the CEtsOptAtom
#ifndef __ETSOPTATOM_H__
#define __ETSOPTATOM_H__
#pragma once

#include "resource.h"       // main symbols

#include "SpotOptionContract.h"
#include "EtsOptRootAtom.h"
#include "ContractColl.h"

// CEtsOptAtom

class ATL_NO_VTABLE CEtsOptAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsOptAtom, &CLSID_EtsOptAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsOptAtom>,
	public IDispatchImpl<IEtsOptAtom, &IID_IEtsOptAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CSpotOptionContract
{
public:
	CEtsOptAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSOPTATOM)


BEGIN_COM_MAP(CEtsOptAtom)
	COM_INTERFACE_ENTRY(IEtsOptAtom)
	COM_INTERFACE_ENTRY(IContract)
	COM_INTERFACE_ENTRY(IOptionContract)
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
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsOptAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		CSpotOptionContract::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	
	//IContract
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_BSTR_PROPERTY(ImportID, m_bstrImportID)
	IMPLEMENT_SIMPLE_PROPERTY(EtsContractTypeEnum, ContractType, m_enType)
	IMPLEMENT_BSTR_PROPERTY(ContractName, m_bstrContractName)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ClosePrice, m_dClosePrice)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, TheoClosePrice, m_dTheoClosePrice)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, ManualPrice, m_dManualPrice)
	
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

	//IOptionContract
	IMPLEMENT_SIMPLE_PROPERTY(EtsOptionTypeEnum, OptType, m_enOptType)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, Expiry, m_dtExpiry)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Strike, m_dStrike)
	IMPLEMENT_BSTR_PROPERTY(Series, m_bstrSeries)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Flex, m_IsFlex)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradingClose, m_dtTradingClose)

	STDMETHOD(get_Root)(IContractRoot** pVal);

	//IvRmCompatibility [to remove]
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceClose, m_dClosePrice)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceTheoClose, m_dTheoClosePrice)
	
	//IEtsOptAtom
	STDMETHOD(CopyTo)(IEtsOptAtom* pDest);
	IMPLEMENT_SIMPLE_PROPERTY(LONG, RootID, m_nRootID)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsOptAtom), CEtsOptAtom)

#endif //__ETSOPTATOM_H__
