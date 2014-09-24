// MyTrackStructureInfo.h : Declaration of the CMyTrackStructureInfo

#ifndef __MYTRACKSTRUCTUREINFO_H_
#define __MYTRACKSTRUCTUREINFO_H_

#include "resource.h"       // main symbols
#include "MyTrackProviders.h"
#include "BaseNotifier.h"
#include "common.h"
#include "MyTrackCore.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTrackStructureInfo
class ATL_NO_VTABLE CMyTrackStructureInfo : 
		public CComObjectRootEx<CComMultiThreadModel>,
		public CComCoClass<CMyTrackStructureInfo, &CLSID_MyTrackStructureInfo>,
		public ISupportErrorInfoImpl<&IID_IStructureProvider>,
		public IDispatchImpl<IStructureProvider, &IID_IStructureProvider, &LIBID_MYTRACKPROVIDERSLib>,
  		public IDispatchImpl<IMyTrackInfo, &IID_IMyTrackInfo, &LIBID_MYTRACKPROVIDERSLib>,
		public IConnectionPointContainerImpl<CMyTrackStructureInfo>,
		public IConnectionPointImpl<CMyTrackStructureInfo, &DIID__IMyTrackStructureInfoEvents, CComDynamicUnkArray>,
		public CBaseNotifier
{
public:
	CMyTrackStructureInfo()
	{
		m_enMode = enPPDefault;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYTRACKSTRUCTUREINFO)

DECLARE_NOT_AGGREGATABLE(CMyTrackStructureInfo)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMyTrackStructureInfo)
	COM_INTERFACE_ENTRY(IStructureProvider)
	COM_INTERFACE_ENTRY(IMyTrackInfo)
	COM_INTERFACE_ENTRY2(IDispatch, IMyTrackInfo)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMyTrackStructureInfo)
	CONNECTION_POINT_ENTRY(DIID__IMyTrackStructureInfoEvents)
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
	
	virtual ClientType GetClientType(){return 	enClientStructureInfo;};

	virtual HRESULT OnStock(CComVariant &varParams, CComVariant &varResults);
	virtual HRESULT OnOption(CComVariant &varParams, CComVariant &varResults, CComVariant &varIsLastOption);
	virtual HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
						RequestsTypeEnum enumRequest, CComVariant &varRequest);
public :
	STDMETHOD(get_PriceProviderMode)(/*[out, retval]*/ PriceProviderTypeEnum *pVal);
	STDMETHOD(put_PriceProviderMode)(/*[in]*/ PriceProviderTypeEnum newVal);

private:
	PriceProviderTypeEnum m_enMode;


};

#endif //__MYTRACKSTRUCTUREINFO_H_
