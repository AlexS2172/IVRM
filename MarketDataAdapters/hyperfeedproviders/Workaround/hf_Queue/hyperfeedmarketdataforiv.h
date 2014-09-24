// HyperFeedMarketDataForIv.h : Declaration of the CHyperFeedMarketDataForIv

#pragma once

#ifndef __HYPERFEEMARKETDATAFORIV_H_
#define __HYPERFEEMARKETDATAFORIV_H_

#include "resource.h"       // main symbols
#include "HyperFeedBase.h"
#include "MasterOptions.h"
#include "Options.h"
#include "Price.h"
#include "SecurityProfile.h"

#define MAX_OPTIONS_IN_ROOT (UINT) 2*12*26
#define THEO_ROOT_COUNT		(UINT) 5

// CHyperFeedMarketDataForIv
class ATL_NO_VTABLE CHyperFeedMarketDataForIv :
	public CHyperFeedBase<CHyperFeedMarketDataForIv>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHyperFeedMarketDataForIv, &CLSID_HyperFeedMarketDataForIv>,
	public ISupportErrorInfoImpl<&IID_IMarketDataForIv>,
	public IConnectionPointContainerImpl<CHyperFeedMarketDataForIv>,
	public IDispatchImpl<IMarketDataForIv, &IID_IMarketDataForIv, &LIBID_HYPERFEEDPROVIDERSLib>,
	public IConnectionPointImpl<CHyperFeedMarketDataForIv, &DIID__IMarketDataForIvEvents, CComDynamicUnkArray>,
	public CMasterOptions,
	//public COptions,
	public CPrice,
	public CSecurityProfile
{
public:
	CHyperFeedMarketDataForIv()
	{
		m_OptList.reserve(MAX_OPTIONS_IN_ROOT);
		m_OptionResults.reserve(MAX_OPTIONS_IN_ROOT * THEO_ROOT_COUNT);
	}
	virtual ~CHyperFeedMarketDataForIv()
	{
		m_OptList.clear();
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_HYPERFEEDMARKETDATAFORIV)


	BEGIN_COM_MAP(CHyperFeedMarketDataForIv)
		COM_INTERFACE_ENTRY(IMarketDataForIv)
		COM_INTERFACE_ENTRY2(IDispatch, IMarketDataForIv)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CHyperFeedMarketDataForIv)
		CONNECTION_POINT_ENTRY(DIID__IMarketDataForIvEvents)
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	// IMarketDataForIv
	STDMETHOD(Connect)();
	STDMETHOD(Disconnect)();
	STDMETHOD(RequestData)(/*[in]*/ short RequestDataTypes);
	STDMETHOD(CancelRequest)();

	long Attach();
	long Detach();

	void OnResponse(CResponseBasePtr pResponse);
	void OnRequest(CRequestBasePtr pResponse);

	void OnRemoveAll()
	{
		CPrice::OnRemoveAll();
		//COptions::OnRemoveAll();
	}

protected:
	virtual void OnSecurityInfo(const _QuoteUpdateParams& Params, const DBA_RECORD_52& rec);

private:
	HRESULT OnOption(const CComVariant& varResults, const CComVariant& varIsLastOption);
	HRESULT OnStock(const CComVariant& varResults);

	virtual void PublicResponse(CResponseBasePtr pResponse){ AddResponse(pResponse);}
	
	//virtual void PublicOption(DBA_OPTIONS_FILTER_RECORD* pData);
	virtual void PublicGetOptionError(ErrorNumberEnum nError, BSTR bsDescription);
	virtual void PublicGetMasterOptionError(ErrorNumberEnum nError, BSTR bsDescription);
	virtual void PublicLastMasterOption();

	virtual void PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData);
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pRec){};
	virtual void _PublicLastQuoteSymbolOnly(const QuoteUpdateParams* Params);
	void _CopyOptionExtendedToIvResult(const DBA_RECORD_3& rec, _OptionForIvResult& Result);
	void _RetrieveSecuritiesInfo();
	void _CopySecurityInfoToIvResult(_OptionForIvResult& Result);

	virtual void PublishUnderlyingLoaded(BSTR bsUnderlying);
private:
	CComVariant m_vtCurrentRequest;
	short m_RequestDataTypes;

	typedef map<string, sec_info_t> CSecurityRequestMap;
	CSecurityRequestMap	m_SecurityRequestMap;

	vector<_OptionForIvResult> m_OptionResults;
	CMasterOptions::optionlist m_OptList;
};

#endif //__HYPERFEEMARKETDATAFORIV_H_
