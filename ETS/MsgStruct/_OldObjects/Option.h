// Option.h : Declaration of the COption

#ifndef __OPTION_H_
#define __OPTION_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"


/////////////////////////////////////////////////////////////////////////////
// COption
class ATL_NO_VTABLE COption : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<COption, &CLSID_Option>,
	public IDispatchImpl<IOption, &IID_IOption, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<COption, __OptionData>
{
public:
	COption()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_OPTION)
DECLARE_NOT_AGGREGATABLE(COption)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(COption)
	COM_INTERFACE_ENTRY(IOption)
	COM_INTERFACE_ENTRY2(IDispatch, IOption)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IOption
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(ContractID, m_nContractID)
	IMPLEMENT_LONG_PROP(OptionRootID, m_nOptionRootID)
	IMPLEMENT_DOUBLE_PROP(Strike, m_dStrike)	
	IMPLEMENT_LONG_PROP(ExpiryID, m_nExpiryID)
	IMPLEMENT_LONG_PROP(IsCall, m_IsCall)
};

#endif //__OPTION_H_
