// VolatilitySource.h : Declaration of the CVolatilitySource

#ifndef __VOLATILITYSOURCE_H_
#define __VOLATILITYSOURCE_H_

#include "resource.h"       // main symbols
#include "VMEDCP.h"
#include "SymbolVolatility.h"

typedef std::map<_bstr_t, CComObject<CSymbolVolatility>*> CSymbolsMap;

/////////////////////////////////////////////////////////////////////////////
// CVolatilitySource
class ATL_NO_VTABLE CVolatilitySource : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVolatilitySource, &CLSID_VolatilitySource>,
	public ISupportErrorInfoImpl<&IID_IVolatilitySource>,
	public IConnectionPointContainerImpl<CVolatilitySource>,
	public IDispatchImpl<IVolatilitySource, &IID_IVolatilitySource, &LIBID_VMELib>,
	public CProxy_IVolatilitySourceEvents< CVolatilitySource >,
	public IDispEventImpl <1, CVolatilitySource, &DIID__IOnMessages>
{
	IVAManagementPtr	m_spDS;
	CSymbolsMap			m_surfaces;

	ISymbolObjectPtr	m_spSymbolObject;
	IPubManagerPtr		m_spPubManager;
	ISubManagerPtr		m_spSubManager;

	VARIANT_BOOL		m_bEnableEvents;
	VARIANT_BOOL		m_bEnableCache;
	VARIANT_BOOL		m_bEnableEditing;
	double				m_dDefaultVola;
																					
// Construction/destruction
public:
	CVolatilitySource();
	HRESULT FinalConstruct();
	void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_VOLATILITYSOURCE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVolatilitySource)
	COM_INTERFACE_ENTRY(IVolatilitySource)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CVolatilitySource)
CONNECTION_POINT_ENTRY(DIID__IVolatilitySourceEvents)
END_CONNECTION_POINT_MAP()

BEGIN_SINK_MAP(CVolatilitySource)
	SINK_ENTRY_INFO(1, DIID__IOnMessages, DISPID_OnVola, OnVMESurfaceSource, &m_OnVMESurfaceInfo)
END_SINK_MAP()

// Notification entries
private:
	void OnSurfaceChanged( const _bstr_t& SymbolName );
	void OnSimulatedSave(const _bstr_t& Symbol);
// IVolatilitySource
public:
	STDMETHOD(get_DefaultVolatility)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_DefaultVolatility)(/*[in]*/ double newVal);
	STDMETHOD(get_EnableEditing)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_EnableEditing)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(Reload)();
	STDMETHOD(get_EnableCache)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_EnableCache)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_EnableEvents)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_EnableEvents)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_SymbolVolatility)(/*[in]*/BSTR Symbol, /*[in]*/SYMBOL_TYPE SymbolType, /*[out, retval]*/IVSSymbolVolatility* *pVal);
	STDMETHOD(get_DataSource)(/*[out, retval]*/IVAManagement **pVal);
	STDMETHOD(put_DataSource)(/*[in]*/IVAManagement *newVal);
	STDMETHOD(putref_DataSource)(/*[in]*/IVAManagement *newVal);
	STDMETHOD(RegisterPublisher)();
	STDMETHOD(UnregisterPublisher)();

	STDMETHOD(get_SimulatedSymbolVol)(BSTR Symbol, SYMBOL_TYPE SymbolType, IVSSymbolVolatility **pVal);

	static _ATL_FUNC_INFO m_OnVMESurfaceInfo;
	STDMETHOD (OnVMESurfaceSource)(/*[in]*/IDispatch* Symbol, /*[in]*/IDispatch* Data);
	void PublishChanges( _RecordsetPtr& spRS, IVMESurfacePtr&  spVMESurface ){
		m_spPubManager->PubVola( m_spSymbolObject, spVMESurface );
	}
	friend CSymbolVolatility;

// Helpers
protected:
	STDMETHOD(ClearMap)();
};

typedef _com_ptr_t<IVolatilitySource> IVolatilitySourcePtr;


#endif //__VOLATILITYSOURCE_H_
