// Book.h : Declaration of the CBook

#ifndef __BOOK_H_
#define __BOOK_H_

#include "resource.h"       // main symbols
#include "HHStruct.h"
#include "HHCommonProps.h"
#include "MsgPropImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CBook
class ATL_NO_VTABLE CBook : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CBook, &CLSID_Book>,
	public IDispatchImpl<IBook, &IID_IBook, &LIBID_MSGSTRUCTLib>,
	public INetPackingConstSizeDataImpl<CBook, __BookData>
{
public:
	CBook()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BOOK)
DECLARE_NOT_AGGREGATABLE(CBook)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBook)
	COM_INTERFACE_ENTRY(IBook)
	COM_INTERFACE_ENTRY2(IDispatch, IBook)
	COM_INTERFACE_ENTRY(IHHCommon)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IBook
public:
	IMPLEMENT_HH_COMMON_PROP

	IMPLEMENT_LONG_PROP(BookID, m_nID)
	IMPLEMENT_VARCHAR_PROP(BookName, m_Name)
	IMPLEMENT_VARCHAR_PROP(Description, m_Description)
	IMPLEMENT_LONG_PROP(Priority, m_nPriority)
};

#endif //__BOOK_H_
