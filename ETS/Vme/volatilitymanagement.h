// VolatilityManagement.h : Declaration of the CVolatilityManagement

#ifndef __VOLATILITYMANAGEMENT_H_
#define __VOLATILITYMANAGEMENT_H_

#include "resource.h"       // main symbols
#include "VMEDCP.h"

#include "SymbolVolatilitySurface.h"
#include "VMEDCP.h"

/////////////////////////////////////////////////////////////////////////////
// Type definitions for surfaces map
/////////////////////////////////////////////////////////////////////////////
class CSurfaceKey
{
public:
	long		m_nSurfaceID; 
	EOptType	m_enOptType;
	_bstr_t		m_bsSymbol;		// Auxilary field - doesn't included in find

	CSurfaceKey( long nSurfaceID, EOptType enOptType ): 
	m_nSurfaceID(nSurfaceID), m_enOptType(enOptType){}
};

inline bool operator<( const CSurfaceKey& k1, const CSurfaceKey& k2 )
{
	if( k1.m_nSurfaceID < k2.m_nSurfaceID )
		return true;
	else if( k1.m_nSurfaceID > k2.m_nSurfaceID )
		return false;

	if( k1.m_enOptType < k2.m_enOptType )
		return true;
	else if( k1.m_enOptType > k2.m_enOptType )
		return false;

	return false;
}

typedef std::map<CSurfaceKey, CComObject<CSymbolVolatilitySurface>*> CSurfaceMap;
typedef std::map<_bstr_t, long>	CActiveSurfaceIDMap;


/////////////////////////////////////////////////////////////////////////////
// CVolatilityManagement class
/////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE CVolatilityManagement : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVolatilityManagement, &CLSID_VolatilityManagement>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CVolatilityManagement>,
	public IDispatchImpl<IVolatilityManagement, &IID_IVolatilityManagement, &LIBID_VMELib>,
	public CProxy_IVolatilityManagementEvents< CVolatilityManagement >,
	public IDispEventImpl <1, CVolatilityManagement, &DIID__IOnMessages>
{
// Properties
	IVAManagementPtr	m_spDS;
	CSurfaceMap			m_surfaces;
	CActiveSurfaceIDMap	m_activeSurfaces;

	ISymbolObjectPtr	m_spSymbolObject;
	IPubManagerPtr		m_spPubManager;
	ISubManagerPtr		m_spSubManager;

// Construction/destruction
public:
	CVolatilityManagement();
	HRESULT FinalConstruct();
	void FinalRelease();

// Notification entries
private:
	void OnDefaultSurfaceChanged( const _bstr_t& SymbolName, long SurfaceID, EOptType OptType, bool bNewDefault );

public:
DECLARE_AGGREGATABLE( CVolatilityManagement )

DECLARE_REGISTRY_RESOURCEID(IDR_VOLATILITYMANAGEMENT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVolatilityManagement)
	COM_INTERFACE_ENTRY(IVolatilityManagement)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CVolatilityManagement)
	CONNECTION_POINT_ENTRY(DIID__IVolatilityManagementEvents)
END_CONNECTION_POINT_MAP()

BEGIN_SINK_MAP(CVolatilityManagement)
	SINK_ENTRY_INFO(1, DIID__IOnMessages, DISPID_OnVola, OnVMESurface, &m_OnVMESurfaceInfo)
END_SINK_MAP()

// ISupportsErrorInfo
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVolatilityManagement
public:
	STDMETHOD(get_DefaultSurfaceID)(/*[in]*/BSTR Symbol, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_DefaultSurfaceID)(/*[in]*/BSTR Symbol, /*[in]*/ long newVal);
	STDMETHOD(put_UnderlinePrice)(/*[in]*/BSTR Symbol, /*[in]*/ double newVal);
	STDMETHOD(get_Surface)(/*[in]*/long SurfaceID, /*[in]*/EOptType OptType, /*[out, retval]*/ IVMSymbolVolatilitySurface **pVal);
	STDMETHOD(get_ActiveSurfaceID)(/*[in]*/BSTR Symbol, /*[out, retval]*/ long *pVal);
	STDMETHOD(put_ActiveSurfaceID)(/*[in]*/BSTR Symbol, /*[in]*/ long newVal);
	STDMETHOD(putref_DataSource)(/*[in]*/ IVAManagement *newVal);
	STDMETHOD(put_DataSource)(/*[in]*/ IVAManagement *newVal);
	STDMETHOD(get_DataSource)(/*out, retval*/ IVAManagement **pVal);

	STDMETHOD(RegisterPublisher)();
	STDMETHOD(UnregisterPublisher)();

	static _ATL_FUNC_INFO m_OnVMESurfaceInfo;
	STDMETHOD (OnVMESurface)(/*[in]*/IDispatch* Symbol, /*[in]*/IDispatch* Data);
	void PublishChanges( _RecordsetPtr& spRS, IVMESurfacePtr&  spVMESurface ){
		m_spPubManager->PubVola( m_spSymbolObject, spVMESurface );
	}

	friend CSymbolVolatilitySurface;

// Helpers
protected:
	STDMETHOD(ClearMaps)();
};

#endif //__VOLATILITYMANAGEMENT_H_
