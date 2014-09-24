// ATFPriceProvider.h : Declaration of the CATFPriceProvider

#ifndef __ATFPRICEPROVIDER_H_
#define __ATFPRICEPROVIDER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CATFPriceProvider
#include "resource.h"       // main symbols
#include "ATFBaseFeed.h"

/////////////////////////////////////////////////////////////////////////////
// CATFPriceProvider
class ATL_NO_VTABLE CATFPriceProvider : 
	public CATFBaseFeed<CATFPriceProvider>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CATFPriceProvider, &CLSID_ATFPriceProvider>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CATFPriceProvider>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_ATFDATAPROVIDERLib>,
	public IDispatchImpl<IGroupPrice, &IID_IGroupPrice, &LIBID_ATFDATAPROVIDERLib>,
	IConnectionPointImpl<CATFPriceProvider, &DIID__IPriceProviderEvents, CComDynamicUnkArray>
{
public:
	CATFPriceProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATFPRICEPROVIDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CATFPriceProvider)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPrice)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPrice)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CATFPriceProvider)
	CONNECTION_POINT_ENTRY(DIID__IPriceProviderEvents)
END_CONNECTION_POINT_MAP()

	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
				RequestsTypeEnum enumRequest, const CComVariant &varRequest);

	void PublicRealtime(_bstr_t bsKey, PB* pData);
	void PublicGroupData(const CComVariant& request, PB* pData, int lotSize, bool bLast);

// ITrackDataPriceProvider
public:
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();

	void PublicQuoteData(const CComVariant& request, PB* pData); 

private:
//	CTrackDataCore  m_Client;

	void GetParams(CComVariant &varResult, QuoteUpdateParams *Params)
	{
		_QuoteUpdateParams qup;
		qup.Init();
		qup->Symbol = SysAllocString(Params->Symbol);
		qup->Exchange = SysAllocString(Params->Exchange);
		qup->Type = Params->Type;
		qup.CopyTo (varResult);
	}

// IGroupPrice
	STDMETHOD(RequestGroup)(QuoteUpdateParams * Params, GroupRequestType enOperationType)
	{
		try
		{
			if(Params->Type != enSTK && Params->Type != enIDX)
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

#endif //__ATFPRICEPROVIDER_H_
