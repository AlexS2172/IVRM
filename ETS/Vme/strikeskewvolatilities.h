// StrikeSkewVolatilities.h : Declaration of the CStrikeSkewVolatilities

#ifndef __STRIKESKEWVOLATILITIES_H_
#define __STRIKESKEWVOLATILITIES_H_

#include "resource.h"       // main symbols

#include "DataHolder.h"
#include "SymbolVolatilitySurface.h"

/////////////////////////////////////////////////////////////////////////////
// CStrikeSkewVolatilities
class ATL_NO_VTABLE CStrikeSkewVolatilities : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CStrikeSkewVolatilities, &CLSID_StrikeSkewVolatilities>,
	public ISupportErrorInfo,
	public IDispatchImpl<IVMStrikeSkewVolatilities, &IID_IVMStrikeSkewVolatilities, &LIBID_VMELib>
{
	CSymbolVolatilitySurface*	  m_pSurface;	// To call notifications
	IVMSymbolVolatilitySurfacePtr m_spSurface;	// To protect common data buffer

	long	  m_nSurfaceID;
	EOptType  m_enOptType;
	DATE	  m_dtExpDate;
	_bstr_t	  m_bsSymbolName;
	SYMBOL_TYPE m_enSymbolType;

	CDataHolder* m_pData;

// Construction/initialization/destruction
public:
	CStrikeSkewVolatilities() {}
	HRESULT FinalConstruct();
	void	FinalRelease();

	STDMETHOD(Init)( CSymbolVolatilitySurface* pSurface, 
					 BSTR bsSymbolName,SYMBOL_TYPE enSymbolType,
					 long nSurfaceID, EOptType enOptType, DATE dtExpDate );

DECLARE_REGISTRY_RESOURCEID(IDR_STRIKESKEWVOLATILITIES)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStrikeSkewVolatilities)
	COM_INTERFACE_ENTRY(IVMStrikeSkewVolatilities)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVMStrikeSkewVolatilities
public:
	STDMETHOD(get_DualQuadraticParams)(/*[out, retval]*/ IDualQuadraticData** pVal);
	STDMETHOD(putref_DualQuadraticParams)(/*[in]*/ IDualQuadraticData* newVal);
	STDMETHOD(get_UpdateMark)(/*[out, retval]*/ long *pVal);
	STDMETHOD(ProportionalShift)(/*[in]*/double Strike, /*[in]*/double Vola);
	STDMETHOD(get_BasePointsInterpolation)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_BasePointsInterpolation)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(Reload)();
	STDMETHOD(get_BasePointsCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_IsBasePoint)(/*[in]*/double Strike, /*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_IsBasePoint)(/*[in]*/double Strike, /*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(RemoveAll)();
	STDMETHOD(Remove)(/*[in]*/double Strike);
	STDMETHOD(Add)(/*[in]*/double Strike, /*[in]*/double Volatility, /*[in]*/VARIANT_BOOL IsBasePoint, /*[out, retval]*/long *pnIndex);
	STDMETHOD(Save)();
	STDMETHOD(get_IsATM)(/*[in]*/double Strike, /*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_Volatility)(/*[in]*/double Strike, /*[out, retval]*/ double *pVal);
	STDMETHOD(put_Volatility)(/*[in]*/double Strike, /*[in]*/ double newVal);
	STDMETHOD(get_Status)(/*[in]*/double Strike, /*[out, retval]*/ EVolaItemStatus *pVal);
	STDMETHOD(get_Strike)(/*[in]*/long index, /*[out, retval]*/ double *pVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

#endif //__STRIKESKEWVOLATILITIES_H_
