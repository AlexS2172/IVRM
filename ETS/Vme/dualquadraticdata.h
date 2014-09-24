// DualQuadraticData.h : Declaration of the CDualQuadraticData

#ifndef __DUALQUADRATICDATA_H_
#define __DUALQUADRATICDATA_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDualQuadraticData
class ATL_NO_VTABLE CDualQuadraticData : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDualQuadraticData, &CLSID_DualQuadraticData>,
	public IDispatchImpl<IDualQuadraticData, &IID_IDualQuadraticData, &LIBID_VMELib>
{
	double		m_dATMVola;
	double		m_dCoefA1;
	double		m_dCoefB1;
	double		m_dCoefA2;
	double		m_dCoefB2;

// Construction/destruction
public:
	CDualQuadraticData(); 
	HRESULT FinalConstruct();
	void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_DUALQUADRATICDATA)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDualQuadraticData)
	COM_INTERFACE_ENTRY(IDualQuadraticData)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDualQuadraticData)
END_CONNECTION_POINT_MAP()


// IDualQuadraticData
public:
	STDMETHOD(get_ATMVola)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_ATMVola)(/*[in]*/ double newVal);
	STDMETHOD(get_CoefA1)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_CoefA1)(/*[in]*/ double newVal);
	STDMETHOD(get_CoefB1)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_CoefB1)(/*[in]*/ double newVal);
	STDMETHOD(get_CoefA2)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_CoefA2)(/*[in]*/ double newVal);
	STDMETHOD(get_CoefB2)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_CoefB2)(/*[in]*/ double newVal);
};


#endif //__DUALQUADRATICDATA_H_
