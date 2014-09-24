// MyTrackStructureProvider.h : Declaration of the CMyTrackStructureProvider

#ifndef __MYTRACKSTRUCTUREPROVIDER_H_
#define __MYTRACKSTRUCTUREPROVIDER_H_

#include "resource.h"       // main symbols
#include "BaseNotifier.h"
#include "common.h"
#include "MyTrackCore.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTrackStructureProvider
class ATL_NO_VTABLE CMyTrackStructureProvider : 
		public CComObjectRootEx<CComMultiThreadModel>,
		public CComCoClass<CMyTrackStructureProvider, &CLSID_MyTrackStructureProvider>,
		public ISupportErrorInfoImpl<&IID_IStructureProvider>,
		public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_MYTRACKPROVIDERSLib>,
		public IDispatchImpl<IMyTrackInfo, &IID_IMyTrackInfo, &LIBID_MYTRACKPROVIDERSLib>,
		public IConnectionPointContainerImpl<CMyTrackStructureProvider>,
		public IConnectionPointImpl<CMyTrackStructureProvider, &DIID__IStructureProviderEvents, CComDynamicUnkArray>,
		public CBaseNotifier
{
public:
	CMyTrackStructureProvider()
	{
		m_enMode = enPPDefault;
	}
	
DECLARE_REGISTRY_RESOURCEID(IDR_MYTRACKSTRUCTUREPROVIDER)
DECLARE_NOT_AGGREGATABLE(CMyTrackStructureProvider)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMyTrackStructureProvider)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IMyTrackInfo)
	COM_INTERFACE_ENTRY2(IDispatch,IMyTrackInfo)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMyTrackStructureProvider)
	CONNECTION_POINT_ENTRY(DIID__IStructureProviderEvents)
END_CONNECTION_POINT_MAP()

	STDMETHOD(RequestStock)(StockParams *Params);
	STDMETHOD(CancelStock)(VARIANT Params);
	STDMETHOD(RequestOptions)(OptionParams *Params);
	STDMETHOD(CancelOptions)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
public:
	void FinalRelease() 
	{
		_Module.GetMyTrackCore ()->Disconnect(static_cast<CBaseNotifier*>(this));
	}
	
	virtual HRESULT OnStock(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnOption(CComVariant &varParams, CComVariant &varResults, CComVariant &varIsLastOption);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, CComVariant &varRequest);

	virtual ClientType GetClientType(){return 	enClientStructureProvider;};

public :
	STDMETHOD(get_PriceProviderMode)(/*[out, retval]*/ PriceProviderTypeEnum *pVal);
	STDMETHOD(put_PriceProviderMode)(/*[in]*/ PriceProviderTypeEnum newVal);

protected:
	PriceProviderTypeEnum m_enMode;

};

#endif //__MYTRACKSTRUCTUREPROVIDER_H_
