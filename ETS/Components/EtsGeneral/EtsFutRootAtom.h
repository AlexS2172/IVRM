// EtsFutRootAtom.h : Declaration of the CEtsFutRootAtom
#ifndef __ETSFUTROOTATOM_H_
#define __ETSFUTROOTATOM_H_
#pragma once

#include "resource.h"       // main symbols
#include "EtsGeneral.h"
#include "EtsFutColl.h"
#include "ContractRoot.h"

//_COM_SMARTPTR_TYPEDEF(IEtsFutRootAtom, IID_IEtsFutRootAtom);

struct __EtsFutRootAtom{

	LONG					m_nOptLotSize;
	IEtsFutCollPtr			m_spFutures;

	__EtsFutRootAtom():
	m_nOptLotSize(0L){
	};
};

// CEtsFutRootAtom

class ATL_NO_VTABLE CEtsFutRootAtom : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEtsFutRootAtom, &CLSID_EtsFutRootAtom>,
	public ISupportErrorInfoImpl<&IID_IEtsFutRootAtom>,
	public IDispatchImpl<IEtsFutRootAtom, &IID_IEtsFutRootAtom, &LIBID_EtsGeneralLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public __EtsFutRootAtom,
	public CContractRoot
{
public:
	CEtsFutRootAtom(){
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ETSFUTROOTATOM)


BEGIN_COM_MAP(CEtsFutRootAtom)
	COM_INTERFACE_ENTRY(IEtsFutRootAtom)
	COM_INTERFACE_ENTRY(IContractRoot)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct(){
		try{
			_CHK(m_spFutures.CreateInstance(CLSID_EtsFutColl), _T("Fail to create futures."));
		}
		catch(const _com_error& e){
			return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsFutRootAtom, e.Error());
		}
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}
	
	void FinalRelease(){
		m_spFutures = NULL;
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
	
	//IEtsFutRootAtom
	IMPLEMENT_SIMPLE_PROPERTY(LONG, FutLotSize, m_LotSize)
	IMPLEMENT_SIMPLE_PROPERTY(LONG, OptLotSize, m_nOptLotSize)	
	IMPLEMENT_OBJECTREADONLY_PROPERTY(IEtsFutColl*, Futures, m_spFutures)
};

OBJECT_ENTRY_AUTO(__uuidof(EtsFutRootAtom), CEtsFutRootAtom)

#endif //__ETSFUTROOTATOM_H_
