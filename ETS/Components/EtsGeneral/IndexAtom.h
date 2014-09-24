// IndexAtom.h : Declaration of the CIndexAtom
#ifndef __INDEXATOM_H__
#define __INDEXATOM_H__
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "IndexCompColl.h"
#include "EtsPriceProfileAtom.h"

#include "EtsIndexDivAtom.h"
#include "EtsIndexDivColl.h"

//_COM_SMARTPTR_TYPEDEF(IIndexAtom, IID_IIndexAtom);

struct __IndexAtom
{
	LONG								m_nID;
	CComBSTR							m_bstrSymbol;
	VARIANT_BOOL						m_bIsBasket;
	VARIANT_BOOL						m_bHaveComponentBetas;
	IIndexCompCollPtr					m_spComponents;
	std::vector<EtsRegularDividend>		m_Divs;
	//IIndexCompCollPtr					m_spSyntheticRootBetas;
	IEtsPriceProfileAtomPtr				m_spUndPriceProfile;
	IEtsPriceProfileAtomPtr				m_spOptPriceProfile;
	IEtsIndexDivCollPtr					m_spDivs;

	__IndexAtom()
		: m_nID(0L), m_bIsBasket(VARIANT_FALSE), m_bHaveComponentBetas(VARIANT_FALSE)
	{
	}
};

// CIndexAtom

class ATL_NO_VTABLE CIndexAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CIndexAtom, &CLSID_IndexAtom>,
	public ISupportErrorInfoImpl<&IID_IIndexAtom>,
	public IDispatchImpl<IIndexAtom, &IID_IIndexAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __IndexAtom
{
public:
	CIndexAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INDEXATOM)


BEGIN_COM_MAP(CIndexAtom)
	COM_INTERFACE_ENTRY(IIndexAtom)
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
			CComObject<CIndexCompColl>* pCreator = NULL;
			_CHK(CComObject<CIndexCompColl>::CreateInstance(&pCreator), _T("Fail to create index components."));
			m_spComponents.Attach(pCreator, TRUE);

			//_CHK(CComObject<CIndexCompColl>::CreateInstance(&pCreator), _T("Fail to create synthetic root betas."));
			//m_spSyntheticRootBetas.Attach(pCreator, TRUE);

			CComObject<CEtsIndexDivColl>* pIndDivCreator = NULL;
			_CHK(CComObject<CEtsIndexDivColl>::CreateInstance(&pIndDivCreator), _T("Fail to create index div coll component."));
			m_spDivs.Attach(pIndDivCreator, TRUE);
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IIndexAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_spComponents = NULL;
		m_Divs.clear();
		m_pUnkMarshaler.Release();
	}
	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_nID)
	IMPLEMENT_BSTR_PROPERTY(Symbol, m_bstrSymbol)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsBasket, m_bIsBasket)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IIndexCompColl*, Components, m_spComponents)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, HaveComponentBetas, m_bHaveComponentBetas)
	STDMETHOD(get_BasketDivsCount)(LONG* pVal);
	STDMETHOD(InitBasketDivs)(IUndColl* pUndColl);
	STDMETHOD(get_BasketDiv)(LONG Index, EtsRegularDividend* pVal);
	//IMPLEMENT_OBJECTREADONLY_PROPERTY(IIndexCompColl*, SyntheticRootBetas, m_spSyntheticRootBetas)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, UndPriceProfile, m_spUndPriceProfile)
	IMPLEMENT_OBJECT_PROPERTY(IEtsPriceProfileAtom*, OptPriceProfile, m_spOptPriceProfile)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsIndexDivColl*, BasketDivs, m_spDivs)

};

OBJECT_ENTRY_AUTO(__uuidof(IndexAtom), CIndexAtom)

#endif //__INDEXATOM_H__
