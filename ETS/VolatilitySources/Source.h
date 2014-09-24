// Source.h : Declaration of the CSource

#pragma once
#include "resource.h"       // main symbols

#include "VolatilitySources.h"
#include "_IVolatilitySourceEvents_CP.H"

// CSource
class CSource;

class CVSNotify:
public IDispEventImpl<1, CVSNotify, &__uuidof(_IVolatilitySourceEvents), &CAtlModule::m_libid, 1, 0>
{
public:

	BEGIN_SINK_MAP (CVSNotify)
		SINK_ENTRY_INFO(1, __uuidof(_IVolatilitySourceEvents), 1, OnVolatilityChanged, &m_ArrivedVolatilityChanged)
	END_SINK_MAP()

	STDMETHOD(OnVolatilityChanged)(BSTR Symbol);

	CVSNotify(CSource* pClient):m_pClient(pClient)	{};
	virtual ~CVSNotify()	{Destroy();};

	HRESULT Initialize(IVolatilitySource* pClient);
	void Destroy();

private:
	CSource*	         m_pClient;	
	CComPtr<IVolatilitySource>  m_spEvent;
	static _ATL_FUNC_INFO m_ArrivedVolatilityChanged;

};



#define CSOURCE_IMPLEMENT0(method) {HRESULT hr = S_OK;\
	try{if(m_spVolatilitySource!=NULL)\
		hr = m_spVolatilitySource->method();\
			else \
		hr = Error(_T("Volatility Source is not initialized"), __uuidof(IVolatilitySource), E_FAIL);\
	}\
	catch(_com_error& err){ return CComErrorWrapper::SetError(err, L ## #method, L"", __FILE__,__FUNCDNAME__,__LINE__);}return hr;};


#define CSOURCE_IMPLEMENT1(method, parameter) {HRESULT hr = S_OK;\
	try{if(m_spVolatilitySource!=NULL)\
		hr = m_spVolatilitySource->method(parameter);\
			else \
		hr = Error(_T("Volatility Source is not initialized"), __uuidof(IVolatilitySource), E_FAIL);\
	}\
	catch(_com_error& err){ return CComErrorWrapper::SetError(err, L ## #method, L"", __FILE__,__FUNCDNAME__,__LINE__);}return hr;};


#define CSOURCE_IMPLEMENT3(method, P1, P2, P3) {HRESULT hr = S_OK;\
	try{if(m_spVolatilitySource!=NULL)\
		hr = m_spVolatilitySource->method(P1, P2, P3);\
			else \
		hr = Error(_T("Volatility Source is not initialized"), __uuidof(IVolatilitySource), E_FAIL);\
	}\
	catch(_com_error& err){ return CComErrorWrapper::SetError(err, L ## #method, L"", __FILE__,__FUNCDNAME__,__LINE__); }return hr;};




class ATL_NO_VTABLE CSource : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSource, &CLSID_VolatilitySource>,
	public ISupportErrorInfoImpl<&__uuidof(IVolatilitySource)>,
	public IDispatchImpl<IVolatilitySource, &__uuidof(IVolatilitySource), &CAtlModule::m_libid, /* wMajor = */ 1, /* wMinor = */ 0>,
	public IConnectionPointContainerImpl<CSource>,
	public CProxy_IVolatilitySourceEvents<CSource>
//	public IDispEventImpl<1, CSource, &__uuidof(_IVolatilitySourceEvents), &LIBID_VMELib, 1, 0>
{
public:
	CSource():
		m_spVolatilitySource(NULL),
		m_Notify(this)
	{
	}
	DECLARE_REGISTRY_RESOURCEID(IDR_SOURCE)


	BEGIN_COM_MAP(CSource)
//		COM_INTERFACE_ENTRY(ISource)
		COM_INTERFACE_ENTRY2(IDispatch, IVolatilitySource)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IVolatilitySource)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()


	// ISupportsErrorInfo
	//STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
	/*
	
			HRESULT hr = CreateSource();
	
			
			if(SUCCEEDED(hr))
				hr = Advise();
	
			if(FAILED(hr))
				m_spVolatilitySource = NULL;
			return hr;
	*/
		return S_OK;
	
	}

	void FinalRelease() 
	{		
		Unadvise();
		m_spVolatilitySource = NULL;
		
	}

public:
	HRESULT Initialize(CLSID clsid);
	/////////////////////////////////////////////////////////////////////
	// IVolatilitySource Methods
	STDMETHOD(putref_DataSource)(IVAManagement * pVal)	CSOURCE_IMPLEMENT1(putref_DataSource,pVal)
	STDMETHOD(put_DataSource)(IVAManagement * pVal)		CSOURCE_IMPLEMENT1(put_DataSource,pVal)
	STDMETHOD(get_DataSource)(IVAManagement * * pVal)	CSOURCE_IMPLEMENT1(get_DataSource,pVal)
	STDMETHOD(get_EnableEvents)(VARIANT_BOOL * pVal)	CSOURCE_IMPLEMENT1(get_EnableEvents,pVal)
	STDMETHOD(put_EnableEvents)(VARIANT_BOOL pVal)		CSOURCE_IMPLEMENT1(put_EnableEvents,pVal)
	STDMETHOD(get_EnableCache)(VARIANT_BOOL * pVal)		CSOURCE_IMPLEMENT1(get_EnableCache,pVal)
	STDMETHOD(put_EnableCache)(VARIANT_BOOL pVal)		CSOURCE_IMPLEMENT1(put_EnableCache,pVal)
	STDMETHOD(get_EnableEditing)(VARIANT_BOOL * pVal)	CSOURCE_IMPLEMENT1(get_EnableEditing,pVal)
	STDMETHOD(put_EnableEditing)(VARIANT_BOOL pVal)		CSOURCE_IMPLEMENT1(put_EnableEditing,pVal)
	STDMETHOD(get_DefaultVolatility)(double * pVal)		CSOURCE_IMPLEMENT1(get_DefaultVolatility,pVal)
	STDMETHOD(put_DefaultVolatility)(double pVal)		CSOURCE_IMPLEMENT1(put_DefaultVolatility,pVal)
	STDMETHOD(Reload)()									CSOURCE_IMPLEMENT0(Reload);
	STDMETHOD(get_SymbolVolatility)(BSTR Symbol,SYMBOL_TYPE SymbolType, IVSSymbolVolatility * * pVal)
														CSOURCE_IMPLEMENT3(get_SymbolVolatility,Symbol,SymbolType,pVal)
	STDMETHOD(get_SimulatedSymbolVol)(BSTR Symbol,SYMBOL_TYPE SymbolType, IVSSymbolVolatility * * pVal)
														CSOURCE_IMPLEMENT3(get_SimulatedSymbolVol,Symbol,SymbolType,pVal)
	/////////////////////////////////////////////////////////////////////

	STDMETHOD(UnregisterPublisher)()CSOURCE_IMPLEMENT0(UnregisterPublisher);
	BEGIN_CONNECTION_POINT_MAP(CSource)
		CONNECTION_POINT_ENTRY(__uuidof(_IVSourceEvents))
	END_CONNECTION_POINT_MAP()
private:
//	DWORD GetCurrentSource(); 
	HRESULT Advise();
	HRESULT Unadvise();
//	HRESULT CreateSource();
	CComPtr<IVolatilitySource> m_spVolatilitySource;
	CVSNotify m_Notify;
	
};
OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(VolatilitySource), CSource)
