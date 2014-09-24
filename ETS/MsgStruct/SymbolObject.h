// SymbolObject.h : Declaration of the CSymbolObject

#ifndef __SYMBOLOBJECT_H_
#define __SYMBOLOBJECT_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSymbolObject
class ATL_NO_VTABLE CSymbolObject : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSymbolObject, &CLSID_SymbolObject>,
	public IDispatchImpl<ISymbolObject, &IID_ISymbolObject, &LIBID_MSGSTRUCTLib>
{
public:
	CSymbolObject()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SYMBOLOBJECT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSymbolObject)
	COM_INTERFACE_ENTRY(ISymbolObject)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ISymbolObject
public:

	STDMETHOD(get_Type)(/*[out, retval]*/ SYMBOL_TYPE *pVal);
	STDMETHOD(put_Type)(/*[in]*/ SYMBOL_TYPE newVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Exchange)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Exchange)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Currency)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Currency)(/*[in]*/ BSTR newVal);

	STDMETHOD(Set)(/*[in]*/ SYMBOL_TYPE Type,
				   /*[in]*/ BSTR Name,
				   /*[in]*/ BSTR Exchange,
				   /*[in]*/ BSTR Currency);

	STDMETHOD(SetFromString)(/*[in]*/ BSTR Val);

	STDMETHOD(GetString)(/*[out, retval]*/ BSTR* RetVal);

private:
	SYMBOL_TYPE		m_Type;
	CComBSTR		m_Name;
	CComBSTR		m_Exchange;
	CComBSTR		m_Currency;
};

#endif //__SYMBOLOBJECT_H_
