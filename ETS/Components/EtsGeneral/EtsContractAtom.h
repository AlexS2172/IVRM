// EtsContractAtom.h : Declaration of the CEtsContractAtom
#ifndef __ETSCONTRACTATOM_H_
#define __ETSCONTRACTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "AbstractContract.h"
#include "ContractColl.h"

// CEtsContractAtom

class ATL_NO_VTABLE CEtsContractAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsContractAtom, &CLSID_EtsContractAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsContractAtom>,
	public IDispatchImpl<IEtsContractAtom, &IID_IEtsContractAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CAbstractContract
{
public:
	CEtsContractAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSCONTRACTATOM)


BEGIN_COM_MAP(CEtsContractAtom)
	COM_INTERFACE_ENTRY(IEtsContractAtom)
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
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractAtom, e.Error());
		}

		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_spFut = NULL;
		m_spFutRoot = NULL;
		m_spUnd = NULL;
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	//IEtsContractAtom
	IMPLEMENT_OBJECT_PROPERTY(IUndAtom*, Und, m_spUnd)
	IMPLEMENT_OBJECT_PROPERTY(IEtsFutRootAtom*, FutRoot, m_spFutRoot)
	IMPLEMENT_OBJECT_PROPERTY(IEtsFutAtom*, Fut, m_spFut)

	//IContract
	STDMETHOD(get_ID)(LONG* pRetVal);
	STDMETHOD(put_ID)(LONG  Val);
	STDMETHOD(get_ContractType)(enum EtsContractTypeEnum* pRetVal);
	STDMETHOD(put_ContractType)(enum EtsContractTypeEnum  Val);
	STDMETHOD(get_Symbol)(BSTR* pRetVal);
	STDMETHOD(put_Symbol)(BSTR  Val);
	STDMETHOD(get_ContractName)(BSTR* pRetVal);
	STDMETHOD(put_ContractName)(BSTR  Val);

	STDMETHOD(get_ManualPrice)(DOUBLE* pRetVal);
	STDMETHOD(put_ManualPrice)(DOUBLE  Val);
	STDMETHOD(get_ClosePrice)(DOUBLE* pRetVal);
	STDMETHOD(put_ClosePrice)(DOUBLE  Val);
	STDMETHOD(get_TheoClosePrice)(DOUBLE* pRetVal);
	STDMETHOD(put_TheoClosePrice)(DOUBLE  Val);

	IMPLEMENT_BSTR_PROPERTY(ImportID, m_bstrImportID)

	STDMETHOD(Calculate)(/*[in]*/ ICalculationSettings* Settings,
						/*[in]*/ IMarketSimulationScenario* SimulationScenario,
						/*[out, retval]*/ struct IRisks* RetVal);

	STDMETHOD(GetRisks)(/*[out, retval]*/ struct IRisks* RetVal);
	STDMETHOD(GetQuotes)(/*[out, retval]*/ struct QuoteUpdateInfo* RetVal);
	IMPLEMENT_OBJECT_PROPERTY(IContractColl*,	DerivativeContracts,	m_spDerivativeContracts)

	STDMETHOD(putref_BaseContract)(IContract* NewVal);
	STDMETHOD(get_BaseContract)(IContract** pVal);

	STDMETHOD(putref_Asset)(IAsset* NewVal);
	STDMETHOD(get_Asset)(IAsset** pVal);

	STDMETHOD(FitToMarketVolatility)(/*[in]*/DOUBLE Value);

	STDMETHOD(GetPricingUnit)(/*out*/ DOUBLE* Value);
	STDMETHOD(GetContractSizeInAsset)(/*out*/ DOUBLE* Value);
};

OBJECT_ENTRY_AUTO(__uuidof(EtsContractAtom), CEtsContractAtom)

#endif //__ETSCONTRACTATOM_H_
