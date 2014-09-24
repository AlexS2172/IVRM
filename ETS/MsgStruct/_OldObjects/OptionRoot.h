// OptionRoot.h : Declaration of the COptionRoot

#ifndef __OPTIONROOT_H_
#define __OPTIONROOT_H_

#include "resource.h"       // main symbols
#include "hhstruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// COptionRoot
class ATL_NO_VTABLE COptionRoot : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<COptionRoot, &CLSID_OptionRoot>,
	public IDispatchImpl<IOptionRoot, &IID_IOptionRoot, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<COptionRoot, __OptionRootData>
{
public:
	COptionRoot()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_OPTIONROOT)
DECLARE_NOT_AGGREGATABLE(COptionRoot)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(COptionRoot)
	COM_INTERFACE_ENTRY(IOptionRoot)
	COM_INTERFACE_ENTRY2(IDispatch, IOptionRoot)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IOptionRoot
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(OptionRootID, m_nOptionRootID)
	IMPLEMENT_LONG_PROP(UnderlyingContractID, m_nUnderlyingContractID)
	IMPLEMENT_VARCHAR_PROP(Symbol, m_sSymbol)
	IMPLEMENT_LONG_PROP(LotSize, m_nLotSize)
	IMPLEMENT_LONG_PROP(IsSynthetic, m_nIsSynthetic)
};

#endif //__OPTIONROOT_H_
