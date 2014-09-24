// SymbolVolatilitySurface.h : Declaration of the CSymbolVolatilitySurface

#ifndef __SYMBOLVOLATILITYSURFACE_H_
#define __SYMBOLVOLATILITYSURFACE_H_

#include "resource.h"       // main symbols

#include "DataHolder.h"

class CVolatilityManagement;
class CStrikeSkewPoints;
class CStrikeSkewVolatilities;
class CTimeSkewVolatilities;

/////////////////////////////////////////////////////////////////////////////
// CSymbolVolatilitySurface
class ATL_NO_VTABLE CSymbolVolatilitySurface : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSymbolVolatilitySurface, &CLSID_SymbolVolatilitySurface>,
	public ISupportErrorInfo,
	public IDispatchImpl<IVMSymbolVolatilitySurface, &IID_IVMSymbolVolatilitySurface, &LIBID_VMELib>,
	public IDispEventImpl <1, CSymbolVolatilitySurface, &DIID__IOnMessages>
{
	CVolatilityManagement*  m_pManager; // To call notifications
	IVAManagementPtr		m_spDS;

	// Messaging
	IPubManagerPtr	 m_spPubManager;
	ISubManagerPtr	 m_spSubManager;
	ISymbolObjectPtr m_spSymbolObject;
	IVMESurfacePtr	 m_spVMESurface;
	_bstr_t			 m_bsSymbolName;
	SYMBOL_TYPE		 m_enSymbolType;
	bool			 m_bIsDefault;
	VARIANT_BOOL	 m_bEnableEvents;

	long	  m_nSurfaceID;
	EOptType  m_enOptType;

	CDataHolder m_data;

// Construction/destruction/initialization
public:
	CSymbolVolatilitySurface();
	HRESULT FinalConstruct();
	void	FinalRelease();

	STDMETHOD(Init)( CVolatilityManagement* pManager );
	STDMETHOD(Deinit)();
	STDMETHOD(LoadData)( long SurfaceID, EOptType OptType, _bstr_t& Symbol );
	STDMETHOD(SetDefault)( bool bDefault );

	//STDMETHOD(RegisterPublisher)();
	//STDMETHOD(UnregisterPublisher)();
	STDMETHOD(PublishChanges)( _RecordsetPtr& spRS );

	STDMETHOD(SetSurfaceData) ( _RecordsetPtr& spData  );
	STDMETHOD(SetSurfaceProps)( _RecordsetPtr& spProps );
	STDMETHOD(GetSurfaceData) ( _RecordsetPtr& spData, bool bAllRecords, bool bClone );
	STDMETHOD(GetSurfaceProps)( _RecordsetPtr& spProps );

	//STDMETHOD(SetSubManager)(ISubManagerPtr& spSubManger);
	//STDMETHOD(SetPubManager)(IPubManagerPtr& spPubManger);

public:
DECLARE_REGISTRY_RESOURCEID(IDR_SYMBOLVOLATILITYSURFACE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSymbolVolatilitySurface)
	COM_INTERFACE_ENTRY(IVMSymbolVolatilitySurface)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

BEGIN_SINK_MAP(CSymbolVolatilitySurface)
	SINK_ENTRY_INFO(1, DIID__IOnMessages, DISPID_OnVola, OnVMESurface, &m_OnVMESurfaceInfo)
END_SINK_MAP()

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVMSymbolVolatilitySurface
public:
	STDMETHOD(get_Volatility)(/*[in]*/double UnderlinePrice, /*[in]*/double Strike, /*[in]*/DATE ExpDate, /*[out, retval]*/ double *pVal);
	STDMETHOD(Reload)();
	STDMETHOD(Save)();
	STDMETHOD(get_StrikeSkew)(/*[in]*/DATE ExpDate, /*[out, retval]*/ IVMStrikeSkewVolatilities* *pVal);
	STDMETHOD(get_TimeSkew)(/*[out, retval]*/ IVMTimeSkewVolatilities* *pVal);
	STDMETHOD(get_DataSource)(/*[out, retval]*/ IVAManagement* *pVal);
	STDMETHOD(get_InterpolationFactor)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_InterpolationFactor)(/*[in]*/ double newVal);
	STDMETHOD(get_DiscreteAcceleration)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DiscreteAcceleration)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_PriceOverride)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_PriceOverride)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_UnderlinePrice)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_UnderlinePrice)(/*[in]*/ double newVal);
	STDMETHOD(get_SmileAccelerator)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_SmileAccelerator)(/*[in]*/ double newVal);
	STDMETHOD(get_EnableEvents)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_EnableEvents)(/*[in]*/ VARIANT_BOOL newVal);

// _IOnMessages
public:
	STDMETHOD(SaveAs)(/*[in]*/long SurfaceID, /*[in]*/VARIANT_BOOL NewDefault);
	static _ATL_FUNC_INFO m_OnVMESurfaceInfo;

	STDMETHOD (OnVMESurface)(/*[in]*/IDispatch* Symbol, /*[in]*/IDispatch* Data);

	friend CStrikeSkewPoints;
	friend CStrikeSkewVolatilities;
	friend CTimeSkewVolatilities;
};

_COM_SMARTPTR_TYPEDEF( IVMSymbolVolatilitySurface, __uuidof(IVMSymbolVolatilitySurface) );

#endif //__SYMBOLVOLATILITYSURFACE_H_
