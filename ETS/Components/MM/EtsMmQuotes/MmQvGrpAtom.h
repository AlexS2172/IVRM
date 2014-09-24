// MmQvGrpAtom.h : Declaration of the CMmQvGrpAtom
#ifndef __MMQVGRPATOM_H__
#define __MMQVGRPATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsMmQuotes.h"
#include "MmQvExpColl.h"
#include "MmQvOptColl.h"
#include "MmQvUndAtom.h"
//#include "MmQvFutAtom.h"
//#include "MmQvFutRootAtom.h"
#include "MmQvStrikeColl.h"
#include "MmQvFutRootCollection.h"

_COM_SMARTPTR_TYPEDEF(IMmQvGrpAtom, IID_IMmQvGrpAtom);
_COM_SMARTPTR_TYPEDEF(IMmQvSpreadColl, IID_IMmQvSpreadColl);
class CMmQvSpreadColl;

struct __MmQvGrpAtom
{
	LONG						m_nID;
	EtsContractTypeEnum			m_enContractType;
	CComBSTR					m_bstrSymbol;

	IMmQvUndAtomPtr				m_spUnd;
//	IMmQvFutAtomPtr				m_spFut;
//IMmQvFutRootAtomPtr			m_spFutRoot;
	IMmQvFutRootCollectionPtr   m_spFutRootColl;
	IMmQvExpCollPtr				m_spExpAll;
	IMmQvExpCollPtr				m_spFutExpAll;
	IMmQvStrikeCollPtr			m_spStrikeAll;
	IExchCollPtr				m_spUndExchAll;
	IExchCollPtr				m_spOptExchAll;
	IMmQvSpreadCollPtr		    m_spSpreadColl;

	VARIANT_BOOL				m_bUseCustRates;
	VARIANT_BOOL				m_bShowIndexFutureOptions;

	__MmQvGrpAtom()
		: m_nID(0L)
		,m_enContractType(enCtNone) 
		,m_bUseCustRates(VARIANT_FALSE)
		,m_bShowIndexFutureOptions(VARIANT_FALSE)


	{
	}
};

// CMmQvGrpAtom

_COM_SMARTPTR_TYPEDEF(IMmQvGrpAtom, IID_IMmQvGrpAtom);

class ATL_NO_VTABLE CMmQvGrpAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMmQvGrpAtom, &CLSID_MmQvGrpAtom>,
	public ISupportErrorInfoImpl<&IID_IMmQvGrpAtom>,
	public IDispatchImpl<IMmQvGrpAtom, &IID_IMmQvGrpAtom, &LIBID_EtsMmQuotesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __MmQvGrpAtom
{
public:
	CMmQvGrpAtom()
	  :m_pStrikeAll(NULL)
	  ,m_pExpAll(NULL)
	  ,m_pFutExpAll(NULL)
	  ,m_pFutRootColl(NULL)
//	  ,m_pFut(NULL)
	  ,m_pUnd(NULL)
	  ,m_pSpreadColl(NULL)
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MMQVGRPATOM)


BEGIN_COM_MAP(CMmQvGrpAtom)
	COM_INTERFACE_ENTRY(IMmQvGrpAtom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		HRESULT hr = S_OK;
		try
		{
			_CHK(m_spUndExchAll.CreateInstance(CLSID_ExchColl), _T("Fail to create underlying exchanges."));
			_CHK(m_spOptExchAll.CreateInstance(CLSID_ExchColl), _T("Fail to create option exchanges."));


			_CHK(CComObject<CMmQvUndAtom>::CreateInstance(&m_pUnd), _T("Fail to create underlying."));
			m_spUnd.Attach(m_pUnd, TRUE);

			//_CHK(CComObject<CMmQvFutAtom>::CreateInstance(&m_pFut), _T("Fail to create futures."));
			//m_spFut.Attach(m_pFut, TRUE);

			_CHK(CComObject<CMmQvFutRootCollection>::CreateInstance(&m_pFutRootColl), _T("Fail to create futures root collection."));
			m_spFutRootColl.Attach(m_pFutRootColl, TRUE);

			_CHK(CComObject<CMmQvExpColl>::CreateInstance(&m_pExpAll), _T("Fail to create expiries."));
			m_spExpAll.Attach(m_pExpAll, TRUE);

			_CHK(CComObject<CMmQvExpColl>::CreateInstance(&m_pFutExpAll), _T("Fail to create futures expiries collection."));
			m_spFutExpAll.Attach(m_pFutExpAll, TRUE);

			_CHK(CComObject<CMmQvStrikeColl>::CreateInstance(&m_pStrikeAll), _T("Fail to create strikes."));
			m_spStrikeAll.Attach(m_pStrikeAll, TRUE);

			_CHK(CreateSpreadCollection(), _T("Fail to create Spread Collection."));

			hr = CoCreateFreeThreadedMarshaler(	GetControllingUnknown(), &m_pUnkMarshaler.p);
		}
		catch(const _com_error& e)
		{
			hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvGrpAtom, e.Error());
		}
		return hr;
	}
	
	void FinalRelease()
	{
		m_spUnd = NULL;
//		m_spFut = NULL;
		m_spFutRootColl = NULL;
		m_spExpAll = NULL;
		m_spFutExpAll = NULL;
		m_spStrikeAll = NULL;
		m_spUndExchAll = NULL;
		m_spOptExchAll = NULL;
		m_pUnkMarshaler.Release();

	}

private:
	HRESULT CreateSpreadCollection();
	CComPtr<IUnknown> m_pUnkMarshaler;


public:
	CComObject<CMmQvUndAtom>*      m_pUnd;
//	CComObject<CMmQvFutAtom>*	   m_pFut;
	CComObject<CMmQvFutRootCollection>*  m_pFutRootColl;
	CComObject<CMmQvExpColl>*	   m_pExpAll;
	CComObject<CMmQvExpColl>*	   m_pFutExpAll;
	CComObject<CMmQvStrikeColl>*   m_pStrikeAll;
	CComObject<CMmQvSpreadColl>*   m_pSpreadColl;

public:
	CComObject<CMmQvUndAtom>* GetUndAtom() {return m_pUnd;}
public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_SIMPLE_PROPERTY(enum EtsContractTypeEnum,	ContractType, m_enContractType)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvUndAtom*, Und, m_spUnd)
//	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvFutAtom*, Fut, m_spFut)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvFutRootCollection*, FutRootColl, m_spFutRootColl)
//	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvExpColl*, ExpAll, m_spExpAll)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvStrikeColl*, StrikeAll, m_spStrikeAll)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IExchColl*, UndExchAll, m_spUndExchAll)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IExchColl*, OptExchAll, m_spOptExchAll)

	IMPLEMENT_OBJECTREADONLY_PROPERTY(IMmQvSpreadColl*, Spread, m_spSpreadColl)

	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, UseCustRates, m_bUseCustRates)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, ShowIndexFutureOptions, m_bShowIndexFutureOptions)
	STDMETHOD(get_IsStockOrIndex)(VARIANT_BOOL* pRetVal);
	STDMETHOD(get_ExpAll)(IMmQvExpColl **pVal);

	STDMETHOD(CleanUp)(VARIANT_BOOL vbAll);
};

OBJECT_ENTRY_AUTO(__uuidof(MmQvGrpAtom), CMmQvGrpAtom)

#endif //__MMQVGRPATOM_H__
