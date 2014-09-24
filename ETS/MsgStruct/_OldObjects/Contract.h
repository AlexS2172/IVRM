// Contract.h : Declaration of the CContract

#ifndef __CONTRACT_H_
#define __CONTRACT_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CContract
class ATL_NO_VTABLE CContract : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CContract, &CLSID_Contract>,
	public IDispatchImpl<IContract, &IID_IContract, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CContract, __ContractData>
{
public:
	CContract()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONTRACT)
DECLARE_NOT_AGGREGATABLE(CContract)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CContract)
	COM_INTERFACE_ENTRY(IContract)
	COM_INTERFACE_ENTRY2(IDispatch, IContract)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IContract
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
	IMPLEMENT_LONG_PROP(ContractTypeID, m_nContractTypeID)
	IMPLEMENT_VARCHAR_PROP(Symbol, m_Symbol)
	IMPLEMENT_VARCHAR_PROP(ContractName, m_ContractName)
	IMPLEMENT_LONG_PROP(ExpCalendarID, m_nExpCalendarID)
	IMPLEMENT_DOUBLE_PROP(PriceClose, m_dPriceClose)
	IMPLEMENT_DOUBLE_PROP(PriceTheoClose, m_dPriceTheoClose)
};

#endif //__CONTRACT_H_
