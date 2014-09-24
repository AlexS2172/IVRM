// VMESurface.h : Declaration of the CVMESurface

#ifndef __VMESURFACE_H_
#define __VMESURFACE_H_

#include "resource.h"       // main symbols

#define SIZEOFHEADER 
#define SIZEOFROW 

struct VolaPointData
{	
	long	__nPointID;
	double	__ExpDate;
	double	__fStrike;
	double	__fVola;
	BYTE	__bStatus;
	long	__IsBasePoint;
};

struct VolaHeaderData
{
	long			__nSurfaceID;
	long			__nOptType;
	
	double			__fUnderlinePrice;
	double			__fSmileAccelerator;
	double			__fInterpolationFactor;
	
	VARIANT_BOOL	__bDiscreteAcceleration;
	VARIANT_BOOL	__bPriceOverride;
	ULONG			__nPointsCount;
	char			__Symbol[20];
};

/////////////////////////////////////////////////////////////////////////////
// CVMESurface
class ATL_NO_VTABLE CVMESurface : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVMESurface, &CLSID_VMESurface>,
	public IDispatchImpl<IVMESurface, &IID_IVMESurface, &LIBID_MSGSTRUCTLib>,
	public IDispatchImpl<INetPacking, &IID_INetPacking, &LIBID_MSGSTRUCTLib>
{
	/*long m_nSurfaceID;
	long m_nOptType;

	float m_fUnderlinePrice;
	float m_fSmileAccelerator;
	float m_fInterpolationFactor;

	VARIANT_BOOL m_bDiscreteAcceleration;
	VARIANT_BOOL m_bPriceOverride;*/

	_RecordsetPtr m_spPoints;

public:
	CVMESurface()
	{
		m_HeaderData.__nSurfaceID = 0;
		m_HeaderData.__nOptType = 0;
		m_HeaderData.__fUnderlinePrice = 0;
		m_HeaderData.__fSmileAccelerator = 0;
		m_HeaderData.__fInterpolationFactor = 0;
		m_HeaderData.__bDiscreteAcceleration = VARIANT_FALSE;
		m_HeaderData.__bPriceOverride = VARIANT_FALSE;
		memset( m_HeaderData.__Symbol, 0, sizeof(m_HeaderData.__Symbol));
	}

private:
	VolaHeaderData	m_HeaderData;

public:

DECLARE_REGISTRY_RESOURCEID(IDR_VMESURFACE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVMESurface)
	COM_INTERFACE_ENTRY(IVMESurface)
	COM_INTERFACE_ENTRY2(IDispatch, IVMESurface)
	COM_INTERFACE_ENTRY(INetPacking)
END_COM_MAP()

// IVMESurface
public:
	STDMETHOD(get_Points)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(put_Points)(/*[in]*/ IUnknown* newVal);
	STDMETHOD(get_PriceOverride)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_PriceOverride)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_DiscreteAcceleration)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DiscreteAcceleration)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_InterpolationFactor)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_InterpolationFactor)(/*[in]*/ double newVal);
	STDMETHOD(get_SmileAccelerator)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_SmileAccelerator)(/*[in]*/ double newVal);
	STDMETHOD(get_UnderlinePrice)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_UnderlinePrice)(/*[in]*/ double newVal);
	STDMETHOD(get_OptType)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_OptType)(/*[in]*/ long newVal);
	STDMETHOD(get_SurfaceID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_SurfaceID)(/*[in]*/ long newVal);
	STDMETHOD(put_Symbol)(BSTR NewVal);								
	STDMETHOD(get_Symbol)(BSTR* pRetVal);	

// INetPacking
public:
	STDMETHOD(Pack)(/*[out, retval]*/ BSTR *pRetVal);
	STDMETHOD(Unpack)(/*[in]*/ BSTR Val);
};

#endif //__VMESURFACE_H_
