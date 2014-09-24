// ATFPriceInfo.h : Declaration of the CATFPriceInfo

#ifndef __ATFPRICEINFO_H_
#define __ATFPRICEINFO_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CATFPriceInfo
#include "ATFPriceInfoBase.h"
//#include "ATFBaseFeed.h"

/////////////////////////////////////////////////////////////////////////////
// CATFPriceInfo
class ATL_NO_VTABLE CATFPriceInfo : 
	public CATFPriceInfoBaseFeed<CATFPriceInfo>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CATFPriceInfo, &CLSID_ATFPriceInfo>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CATFPriceInfo>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_ATFDATAPROVIDERLib>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_ATFDATAPROVIDERLib>,
	public IConnectionPointImpl<CATFPriceInfo, &DIID__IPriceInfoEvents, CComDynamicUnkArray>
{
public:
	CATFPriceInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATFPRICEINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CATFPriceInfo)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPrice)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CATFPriceInfo)
	CONNECTION_POINT_ENTRY(DIID__IPriceInfoEvents)
END_CONNECTION_POINT_MAP()



// ITrackDataPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

public:
// IGroupPrice
	STDMETHOD(RequestGroup)(QuoteUpdateParams * Params, GroupRequestType enOperationType)
	{
		try
		{
			if(Params->Type != enSTK && Params->Type != enIDX && Params->Type != enGrSTK && Params->Type != enGrIDX)
				return E_INVALIDARG;

			_QuoteUpdateParams param;
			param.Init();

			CComVariant vt;
			param->Symbol = SysAllocString(Params->Symbol);
			param->Exchange = SysAllocString(Params->Exchange);
			param->Type = Params->Type;
			param.CopyTo(vt);
			AddRequest(vt, _enRequestGroup, enOperationType);
		}
		catch (_com_error &err)
		{
			return eg_lib::utils::ComError2ErrInfo (err,this);
		}
		return S_OK;
	}

	STDMETHOD(CancelGroup)(VARIANT Params)
	{
		try
		{
			RemoveRequest(Params, _enRequestGroup);
		}
		catch (_com_error &err)
		{
			return eg_lib::utils::ComError2ErrInfo (err,this);
		}
		return S_OK;
	}
};

#endif //__ATFPRICEINFO_H_
