// EtsFutOptAtom.h : Declaration of the CEtsFutOptAtom
#ifndef __ETSFUTOPTATOM_H__
#define __ETSFUTOPTATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "FutureOptionContract.h"
#include "ContractColl.h"
// CEtsFutOptAtom

class ATL_NO_VTABLE CEtsFutOptAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsFutOptAtom, &CLSID_EtsFutOptAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsFutOptAtom>,
	public IDispatchImpl<IEtsFutOptAtom, &IID_IEtsFutOptAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CFutureOptionContract
{
public:
	CEtsFutOptAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSFUTOPTATOM)


BEGIN_COM_MAP(CEtsFutOptAtom)
	COM_INTERFACE_ENTRY(IEtsFutOptAtom)
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
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsFutOptAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		CFutureOptionContract::Clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	//IEtsFutOptAtom
	IMPLEMENT_SIMPLE_PROPERTY(LONG, FutID, m_nBaseContractID)
	STDMETHOD(CopyTo)(IEtsFutOptAtom* pDest);
	
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
	IMPLEMENT_SIMPLE_PROPERTY(DATE, ExpiryOV, m_dtExpiryOV)
	IMPLEMENT_SIMPLE_PROPERTY(DATE, TradingClose, m_dtTradingClose)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, Flex, m_IsFlex)

	STDMETHOD(get_Root)(IContractRoot** pVal);

	//IvRmCompatibility [to remove]
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceClose, m_dClosePrice)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, PriceTheoClose, m_dTheoClosePrice)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsFutOptAtom), CEtsFutOptAtom)

#endif //__ETSFUTOPTATOM_H__
