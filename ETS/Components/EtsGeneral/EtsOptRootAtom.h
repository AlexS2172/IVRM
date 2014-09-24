// EtsOptRootAtom.h : Declaration of the CEtsOptRootAtom
#ifndef __ETSOPTROOTATOM_H_
#define __ETSOPTROOTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "ContractRoot.h"

_COM_SMARTPTR_TYPEDEF(IEtsOptRootAtom, IID_IEtsOptRootAtom);


struct __EtsOptRootAtom
{

	VARIANT_BOOL			m_bIsSynth;
	VARIANT_BOOL            m_bIsFittingEnabled;

	__EtsOptRootAtom():
	m_bIsSynth(VARIANT_FALSE),
	m_bIsFittingEnabled(VARIANT_TRUE){
	}
};

// CEtsOptRootAtom

class ATL_NO_VTABLE CEtsOptRootAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsOptRootAtom, &CLSID_EtsOptRootAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsOptRootAtom>,
	public IDispatchImpl<IEtsOptRootAtom, &IID_IEtsOptRootAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsOptRootAtom,
	public CContractRoot
{
public:
	CEtsOptRootAtom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSOPTROOTATOM)


BEGIN_COM_MAP(CEtsOptRootAtom)
	COM_INTERFACE_ENTRY(IEtsOptRootAtom)
	COM_INTERFACE_ENTRY(IContractRoot)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease() 
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:
	//IContractRoot
	IMPLEMENT_SIMPLE_PROPERTY(LONG, ID, m_ID)

	IMPLEMENT_BSTR_PROPERTY(Name,				m_Name)
	IMPLEMENT_BSTR_PROPERTY(Symbol,				m_Symbol)
	IMPLEMENT_BSTR_PROPERTY(QuotationUnitName,	m_QuotationUnitName)

	IMPLEMENT_SIMPLE_PROPERTY(ExerciseStyleEnum,	ExerciseStyle,			m_ExerciseStyle)
	IMPLEMENT_SIMPLE_PROPERTY(DATE,					SettlementValueTime,	m_SettlementValueTime)

	IMPLEMENT_SIMPLE_PROPERTY(LONG,			LotSize,			m_LotSize)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,			QuotationUnitID,	m_QuotationUnitID)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		TickSize,			m_TickSize)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		TickValue,			m_TickValue)
	IMPLEMENT_SIMPLE_PROPERTY(DOUBLE,		PriceQuotationUnit,	m_PriceQuotationUnit)
	IMPLEMENT_SIMPLE_PROPERTY(LONG,			QuoteFormat,		m_QuoteFormat)

	IMPLEMENT_SIMPLE_PROPERTY(LONG,			UndID,				m_AssetID)
	
	//IEtsOptRootAtom
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsSynth, m_bIsSynth)
	IMPLEMENT_SIMPLE_PROPERTY(VARIANT_BOOL, IsFit, m_bIsFittingEnabled)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsOptRootAtom), CEtsOptRootAtom)

#endif //__ETSOPTROOTATOM_H_