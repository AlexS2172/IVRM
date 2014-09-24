#ifndef __PPPRICEPROVIDER_H__
#define __PPPRICEPROVIDER_H__

#include "PpPriceRequest.h"
#include "PpRequestData.h"

namespace EODPP
{

const unsigned int SOURCEID_PriceInfo	= 0x1;
const DISPID DISPID_OnLastQuote			= 0x1;
const DISPID DISPID_OnError				= 0x3;

/////////////////////////////////////////////////////////////////////////////
//
class CPpPriceProvider : 
	public IDispEventSimpleImpl<SOURCEID_PriceInfo, CPpPriceProvider, &PP::DIID__IPriceInfoEvents>, 
	virtual public EODCO::CEgTraceable
{
public:
	CPpPriceProvider(void) throw (exception);
	virtual ~CPpPriceProvider(void);

	void Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr) throw (exception);
	void Disconnect() throw ();

	void SetRequestData(CPpRequestDataPtr pReqData) throw (exception);

	void RequestLastQuote() throw (exception);
	void CancelLastQuote() throw (exception);

	virtual void OnLastQuote(CPpPriceRequestKey* pReq,  struct PP::QuoteUpdateInfo* pResult) = 0;
	virtual void OnLastQuoteDone() = 0;
	virtual void OnLastQuoteError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
									CPpPriceRequestKey* pReq  ) = 0;

	virtual void OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription ) = 0;
	virtual void OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription ) = 0;
	
BEGIN_SINK_MAP(CPpPriceProvider)
	SINK_ENTRY_INFO(SOURCEID_PriceInfo, PP::DIID__IPriceInfoEvents, DISPID_OnLastQuote, _OnLastQuote, &m_fiOnLastQuoteInfo)
	SINK_ENTRY_INFO(SOURCEID_PriceInfo, PP::DIID__IPriceInfoEvents, DISPID_OnError, _OnError, &m_fiOnErrorInfo)
END_SINK_MAP()

	static _ATL_FUNC_INFO		m_fiOnErrorInfo;
	static _ATL_FUNC_INFO		m_fiOnLastQuoteInfo;

private:
	//_IPriceInfoEvents
	STDMETHOD(_OnLastQuote)(struct PP::QuoteUpdateParams* pParams, struct PP::QuoteUpdateInfo* pResults);
	STDMETHOD(_OnError)(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
		PP::RequestsTypeEnum enReqType, VARIANT varRequest);

	PP::IPriceProviderPtr		_GetPriceProvider()
	{
		CAutoLock lock(m_lockPp);
		return m_spPriceProvider;
	}

	void	_SetPriceProvider(PP::IPriceProviderPtr pPriceProvider)
	{
		CAutoLock lock(m_lockPp);
		m_spPriceProvider = pPriceProvider;
	}
	
	PP::IGroupPricePtr		_GetGroupPriceProvider()
	{
		CAutoLock lock(m_lockPp);
		return m_spGroupPriceProvider;
	}

	void	_SetGroupPriceProvider(PP::IGroupPrice* pGroupPriceProvider)
	{
		CAutoLock lock(m_lockPp);

		m_spGroupPriceProvider = pGroupPriceProvider;
	}
private:
	PP::IPriceProviderPtr		m_spPriceProvider;
	PP::IGroupPricePtr			m_spGroupPriceProvider;

	CPpRequestDataPtr			m_spReqData;

	bool						m_bLastQuoteReqNow;
	bool						m_bIsGroupRequest;
	size_t						m_nLastQuoteReqCount;
	size_t						m_nLastQuoteReqDone;
	size_t						m_nLastQuoteGroupReqCount;
	size_t						m_nLastQuoteGroupReqDone;

	CLock						m_lockPp;
};

};

#endif //__PPPRICEPROVIDER_H__