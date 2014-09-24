// HyperFeedBatchPriceInfo.h : Declaration of the CHyperFeedBatchPriceInfo
#ifndef __HYPERFEEDBATCHPRICEINFO_H__
#define __HYPERFEEDBATCHPRICEINFO_H__

#pragma once
#include "resource.h"       // main symbols
#include "HyperFeedBase.h"
#include "GroupPrice.h"

/////////////////////////////////////////////////////////////////////////////
/*class __QuoteUpdateParams : public QuoteUpdateParams
{
public:
	bool operator < (const __QuoteUpdateParams& Val) const
	{
		if (Type < Val.Type) return true;

		_bstr_t cmp1 = Symbol;
		_bstr_t cmp2 = Val.Symbol;

		long nCmp = _bstr_cmp(cmp1, cmp2);
		if (nCmp == 0)
		{
			cmp1 = Exchange;
			cmp2 = Val.Exchange;
			nCmp = _bstr_cmp(cmp1, cmp2);
		}

		return nCmp < 0;
	}
};

template<class _Ty, class _Cy>
struct _less : public binary_function<_Ty, _Ty, bool>
{
	bool operator()(const _Ty& _Left, const _Ty& _Right) const
	{
		return (*(_Cy*)&_Left < *(_Cy*)&_Right);
	}
};*/

//typedef map<_bstr_t, _variant_t> CFullInfoDataCollection;

typedef map<_bstr_t, _QuoteUpdateFullInfo> CFullInfoDataCollection;

//typedef map<_QuoteUpdateParams, CComVariant, 
//	_less<QuoteUpdateParams, __QuoteUpdateParams> > CFullInfoDataCollection;

// CHyperFeedBatchPriceInfo
class ATL_NO_VTABLE CHyperFeedBatchPriceInfo : 
	public CHyperFeedBase<CHyperFeedBatchPriceInfo>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHyperFeedBatchPriceInfo, &CLSID_HyperFeedBatchPriceInfo>,
	public ISupportErrorInfoImpl<&IID_IPriceProvider>,
	public IConnectionPointContainerImpl<CHyperFeedBatchPriceInfo>,
	public IDispatchImpl<IPriceProvider, &IID_IPriceProvider, &LIBID_HYPERFEEDPROVIDERSLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IBatchPriceProvider, &IID_IBatchPriceProvider, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IDispatchImpl<IGroupPriceWithNotify, &IID_IGroupPriceWithNotify, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IConnectionPointImpl<CHyperFeedBatchPriceInfo, &DIID__IBatchPriceInfoEvents, CComDynamicUnkArray>,
	public CGroupPrice
	// We can use trace here as the CGroupPrice are inherited from CPrice
{
public:
	CHyperFeedBatchPriceInfo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HYPERFEEDBATCHPRICEINFO)

BEGIN_COM_MAP(CHyperFeedBatchPriceInfo)
	COM_INTERFACE_ENTRY(IPriceProvider)
	COM_INTERFACE_ENTRY(IBatchPriceProvider)
	COM_INTERFACE_ENTRY2(IDispatch, IBatchPriceProvider)
	COM_INTERFACE_ENTRY(IGroupPriceWithNotify)
	COM_INTERFACE_ENTRY2(IDispatch, IGroupPriceWithNotify)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHyperFeedBatchPriceInfo)
	CONNECTION_POINT_ENTRY(DIID__IBatchPriceInfoEvents)
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	// IPriceProvider
	STDMETHOD(RequestLastQuote)(QuoteUpdateParams *Params);
	STDMETHOD(CancelLastQuote)(VARIANT Params);
	STDMETHOD(SubscribeQuote)(QuoteUpdateParams *Params);
	STDMETHOD(UnSubscribeQuote)(VARIANT Params);
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	// IBatchPriceProvider
	STDMETHOD(get_IsQuotesUpdated)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(GetQuotesUpdates)(/*[out, retval]*/ SAFEARRAY** pRetVal);
	
	// IGroupPriceWithNotify
	STDMETHOD(RequestLastGroupQuotes)(QuoteUpdateParams * Params);
	STDMETHOD(CancelLastGroupQuotes)(VARIANT Params);
	STDMETHOD(SubscribeGroupQuotes)(QuoteUpdateParams * Params);
	STDMETHOD(UnSubscribeGroupQuotes)(VARIANT Params);

	long Attach();
	long Detach();

	void OnResponse(CResponseBasePtr pResponse);
	void OnRequest(CRequestBasePtr pResponse);

private:
	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults);
	HRESULT OnQuoteUpdate();
	HRESULT OnSubscribed(const CComVariant &varParams);

	virtual void PublicResponse(CResponseBasePtr pResponse){ AddResponse(pResponse);}
	virtual void PublicLastQuote(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec);
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec);

private:
	CFullInfoDataCollection			m_FullInfoData;
	EgLib::CLock					m_csData;

	_bstr_t _GetKey(const _QuoteUpdateParams& Params)
	{
		/*
		_bstr_t bs = Params->Symbol;
		bs += L"_";
		bs += Params->Exchange;
		bs += L"_";
		
		//bs += _bstr_t(_variant_t(Params->Type));
		wchar_t szType[20] = { 0 };
		swprintf(szType, L"%d", Params->Type);
		bs += szType;
		return bs;
		*/
		wchar_t szType[64] = { 0 };
		try
		{
			if(!Params->Exchange)
				_snwprintf(szType, 63, L"%s__%d", Params->Symbol, Params->Type);
			else
				_snwprintf(szType, 63, L"%s_%s_%d", Params->Symbol, Params->Exchange, Params->Type);

		}
		_CATCH_UNHANDLED_EXCEPTION;
		return szType;

	}
};

OBJECT_ENTRY_AUTO(__uuidof(HyperFeedBatchPriceInfo), CHyperFeedBatchPriceInfo)

#endif //__HYPERFEEDBATCHPRICEINFO_H__