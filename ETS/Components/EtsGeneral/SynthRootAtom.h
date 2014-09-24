// SynthRootAtom.h : Declaration of the CSynthRootAtom
#ifndef __SYNTHROOTATOM_H_
#define __SYNTHROOTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "SynthRootCompColl.h"
#include "IndexCompColl.h"

#include "EtsIndexDivAtom.h"
#include "EtsIndexDivColl.h"

_COM_SMARTPTR_TYPEDEF(ISynthRootAtom, IID_ISynthRootAtom);

struct __SynthRootAtom
{
	LONG								m_nOptRootID;
	ISynthRootCompCollPtr				m_spSynthRootComponents;
	DOUBLE								m_dCashValue;
	DOUBLE								m_dSkew;
	DOUBLE								m_dKurt;
	VARIANT_BOOL						m_bBasket;
	DOUBLE								m_dYield;
	IEtsIndexDivCollPtr					m_spDivs;

	std::vector<EtsRegularDividend>		m_Divs;

	__SynthRootAtom() : m_nOptRootID(0L),m_dCashValue(0.),m_dSkew(0.),
		m_dKurt(0.),m_bBasket(VARIANT_FALSE),m_dYield(0.)
	{
	}
};

// CSynthRootAtom

class ATL_NO_VTABLE CSynthRootAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSynthRootAtom, &CLSID_SynthRootAtom>,
	public ISupportErrorInfoImpl<&IID_ISynthRootAtom>,
	public IDispatchImpl<ISynthRootAtom, &IID_ISynthRootAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __SynthRootAtom
{
public:
	CSynthRootAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SYNTHROOTATOM)


BEGIN_COM_MAP(CSynthRootAtom)
	COM_INTERFACE_ENTRY(ISynthRootAtom)
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
			CComObject<CSynthRootCompColl>* pObject;
			_CHK(CComObject<CSynthRootCompColl>::CreateInstance(&pObject), _T("Fail to create synthetic root components."));
			m_spSynthRootComponents.Attach(pObject, TRUE);

			CComObject<CEtsIndexDivColl>* pIndObject;
			_CHK(CComObject<CEtsIndexDivColl>::CreateInstance(&pIndObject), _T("Fail to create synthetic root index object collection."));
			m_spDivs.Attach(pIndObject, TRUE);

		}
		catch(const _com_error& e)
		{
			hr = Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_ISynthRootAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_spSynthRootComponents = NULL;
		m_Divs.clear();
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptRootID, m_nOptRootID)
	IMPLEMENT_OBJECTREADONLY_PROPERTY(ISynthRootCompColl*, SynthRootComponents, m_spSynthRootComponents)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, CashValue, m_dCashValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Skew, m_dSkew)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Kurt, m_dKurt)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, Basket, m_bBasket)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE, Yield, m_dYield)
	STDMETHOD(get_BasketDivsCount)(LONG* pVal);
	STDMETHOD(get_BasketDiv)(LONG Index, EtsRegularDividend* pVal);
	STDMETHOD(InitBasketDivs)(IUndColl* pUndColl, IIndexColl* pIndexColl);
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsIndexDivColl*, BasketDivs,		m_spDivs)

};

OBJECT_ENTRY_AUTO(__uuidof(SynthRootAtom), CSynthRootAtom)

#endif //__SYNTHROOTATOM_H_