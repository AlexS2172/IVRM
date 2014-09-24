// DBLayoutAccess.h : Declaration of the CDBLayout

#ifndef __DBLAYOUTACCESS_H_
#define __DBLAYOUTACCESS_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDBLayout
class ATL_NO_VTABLE CDBLayoutAccess : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDBLayoutAccess, &CLSID_DBLayoutAccess>,
	public IObjectControl,
	public ISupportErrorInfoImpl<&IID_IVAManagement>,
	public IDispatchImpl<IVADBLayout, &IID_IVADBLayout, &LIBID_VADBLAYOUTLib>,
	public IDispatchImpl<IVAManagement, &IID_IVAManagement, &LIBID_VADBLAYOUTLib>
{
	_bstr_t m_bsConnectionString;

public:
	CDBLayoutAccess()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DBLAYOUTACCESS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_NOT_AGGREGATABLE(CDBLayoutAccess)

BEGIN_COM_MAP(CDBLayoutAccess)
	COM_INTERFACE_ENTRY(IVADBLayout)
	COM_INTERFACE_ENTRY(IObjectControl)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY2(IDispatch, IVADBLayout)
	COM_INTERFACE_ENTRY(IVAManagement)
END_COM_MAP()

// IObjectControl
public:
	STDMETHOD(Activate)();
	STDMETHOD_(BOOL, CanBePooled)();
	STDMETHOD_(void, Deactivate)();

	IObjectContextPtr m_spObjectContext;

// IDBLayout
public:
	STDMETHOD(get_ConnectionString)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ConnectionString)(/*[in]*/ BSTR newVal);

// IVAManagement
public:
	STDMETHOD(SaveDualQuadraticProps)(/*[in]*/BSTR Symbol, /*[in]*/long ContractType, /*[in]*/ DATE ExpDate, /*[in]*/ _Recordset* pRS);
	STDMETHOD(GetDualQuadraticProps)(/*[in]*/ BSTR Symbol, /*[in]*/long ContractType,/*[in]*/ DATE ExpDate, /*[out,retval]*/ _Recordset** ppRS);
	//STDMETHOD(GenerateDefaultVolatilities)(/*[in]*/BSTR Symbol, /*[in]*/EOptType OptType, /*[in]*/double dDefVola);
	STDMETHOD(GetDefaultSurfaceID) (/*[in]*/BSTR Symbol, /*[out, retval]*/long *pSurfaceID);
	STDMETHOD(SaveDefaultSurfaceID)(/*[in]*/BSTR Symbol, /*[in]*/long SurfaceID);

	STDMETHOD(SaveVolatilitySurfaceProps)(/*[in]*/long SurfaceID, /*[in]*/_Recordset *pRecordset);
	STDMETHOD(GetVolatilitySurfaceProps) (/*[in]*/long SurfaceID, /*[out, retval]*/_Recordset **ppRecordset);

	STDMETHOD(SaveSurfaceVolatilities)(/*[in]*/long SurfaceID, /*[in]*/EOptType OptType, /*[in]*/_Recordset *pRecordset);
	STDMETHOD(GetSurfaceVolatilities) (/*[in]*/long SurfaceID, /*[in]*/EOptType OptType, /*[out, retval]*/_Recordset **ppRecordset);

	STDMETHOD(GetVolaSurfaceGroup) (/*[in]*/BSTR Symbol, /*[out, retval]*/_Recordset **ppRecordset);
};

#endif //__DBLAYOUTACCESS_H_
