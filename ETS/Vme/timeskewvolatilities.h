// TimeSkewVolatilities.h : Declaration of the CTimeSkewVolatilities

#ifndef __TIMESKEWVOLATILITIES_H_
#define __TIMESKEWVOLATILITIES_H_

#include "resource.h"       // main symbols

#include "DataHolder.h"
#include "SymbolVolatilitySurface.h"

/////////////////////////////////////////////////////////////////////////////
// CTimeSkewVolatilities
class ATL_NO_VTABLE CTimeSkewVolatilities : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTimeSkewVolatilities, &CLSID_TimeSkewVolatilities>,
	public ISupportErrorInfo,
	public IDispatchImpl<IVMTimeSkewVolatilities, &IID_IVMTimeSkewVolatilities, &LIBID_VMELib>
{
	CSymbolVolatilitySurface*	  m_pSurface;	// To call notifications
	IVMSymbolVolatilitySurfacePtr m_spSurface;	// To protect common data buffer

	long	  m_nSurfaceID;
	EOptType  m_enOptType;

	CDataHolder* m_pData;

// Construction/initialization/destruction
public:
	CTimeSkewVolatilities()	{}
	STDMETHOD(Init)( CSymbolVolatilitySurface* pSurface, 
					 long nSurfaceID, EOptType enOptType );

DECLARE_REGISTRY_RESOURCEID(IDR_TIMESKEWVOLATILITIES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTimeSkewVolatilities)
	COM_INTERFACE_ENTRY(IVMTimeSkewVolatilities)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVMTimeSkewVolatilities
public:
	STDMETHOD(Reload)();
	STDMETHOD(get_Strike)(/*[in]*/DATE ExpDate, /*[out, retval]*/ double *pVal);
	STDMETHOD(Save)();
	STDMETHOD(put_VolatilityDelta)(/*[in]*/DATE ExpDate, /*[in]*/ double newVal);
	STDMETHOD(get_Volatility)(/*[in]*/DATE ExpDate, /*[out, retval]*/ double *pVal);
	STDMETHOD(get_ExpDate)(/*[in]*/long index, /*[out, retval]*/ DATE *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

#endif //__TIMESKEWVOLATILITIES_H_
