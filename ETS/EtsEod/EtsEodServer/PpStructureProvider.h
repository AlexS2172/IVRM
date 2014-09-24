#ifndef __PPSTRUCTUREPROVIDER_H__
#define __PPSTRUCTUREPROVIDER_H__

#include "PpPriceRequest.h"
#include "PpRequestData.h"

namespace EODPP
{

const unsigned int SOURCEID_StructureProviderEx	= 0x1;
const DISPID DISPID_OnStock						= 0x1;
const DISPID DISPID_OnOption					= 0x2;
const DISPID DISPID_OnErrorInfo					= 0x3;
const DISPID DISPID_OnFutureByRoot				= 0x5;
const DISPID DISPID_OnFuturesOption				= 0x6;

/////////////////////////////////////////////////////////////////////////////
//
enum SpRequestTypeEnum
{
	enSpRequestNone = 0,
	enSpRequestStock,
	enSpRequestOptions,
	enSpRequestFutures,
	enSpRequestFuturesOptions
};
 
/////////////////////////////////////////////////////////////////////////////
//
class CPpStructureProvider : 
	public IDispEventSimpleImpl<SOURCEID_StructureProviderEx, CPpStructureProvider, &PP::DIID__IStructureProviderExEvents>, 
	virtual public EODCO::CEgTraceable
{
public:
	CPpStructureProvider(void) throw (exception);
	virtual ~CPpStructureProvider(void);

	void Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr) throw (exception);
	void Disconnect() throw ();

	void RequestUnderlying(const CString& strUnderlyingSymbol) throw (exception);
	void RequestOptions(const CString& strUnderlyingSymbol) throw (exception);
	void RequestFutures(const CString& strFutureRootSymbol) throw (exception);
	void RequestFuturesOptions(const CString& strFutureSymbol) throw (exception);
	void CancelRequests() throw (exception);

	virtual void OnStock(struct PP::StockParams* pReq,  struct PP::StockResultsEx* pResult) = 0;
	virtual void OnOption(struct PP::OptionParams* pReq,  struct PP::OptionResultsEx* pResult, VARIANT_BOOL IsLastOption) = 0;
	virtual void OnGetStockError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
		struct PP::StockParams* pReq ) = 0;
	virtual void OnGetOptionError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
		struct PP::OptionParams* pReq ) = 0;
	virtual void OnFutureByRoot(struct PP::FutureRootParams* pReq,  struct PP::FutureResultsEx* pResult, VARIANT_BOOL IsLastFuture) = 0;
	virtual void OnFuturesOption(struct PP::FuturesOptionParams* pReq,  struct PP::FuturesOptionResultsEx* pResult, VARIANT_BOOL IsLastOption) = 0;
	virtual void OnGetFutureByRootError(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
	struct PP::FutureRootParams* pReq ) = 0;
	virtual void OnGetFuturesOptionError(	PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
	struct PP::FuturesOptionParams* pReq ) = 0;

	virtual void OnError( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription ) = 0;

	virtual void OnDisconnect( PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription ) = 0;

BEGIN_SINK_MAP(CPpStructureProvider)
	SINK_ENTRY_INFO(SOURCEID_StructureProviderEx, PP::DIID__IStructureProviderExEvents, DISPID_OnStock, _OnStock, &m_fiOnStock)
	SINK_ENTRY_INFO(SOURCEID_StructureProviderEx, PP::DIID__IStructureProviderExEvents, DISPID_OnOption, _OnOption, &m_fiOnOption)
	SINK_ENTRY_INFO(SOURCEID_StructureProviderEx, PP::DIID__IStructureProviderExEvents, DISPID_OnErrorInfo, _OnError, &m_fiOnError)
	SINK_ENTRY_INFO(SOURCEID_StructureProviderEx, PP::DIID__IStructureProviderExEvents, DISPID_OnFutureByRoot, _OnFutureByRoot, &m_fiOnFutureByRoot)
	SINK_ENTRY_INFO(SOURCEID_StructureProviderEx, PP::DIID__IStructureProviderExEvents, DISPID_OnFuturesOption, _OnFuturesOption, &m_fiOnFuturesOption)
END_SINK_MAP()

	static _ATL_FUNC_INFO		m_fiOnStock;
	static _ATL_FUNC_INFO		m_fiOnOption;
	static _ATL_FUNC_INFO		m_fiOnError;
	static _ATL_FUNC_INFO		m_fiOnFutureByRoot;
	static _ATL_FUNC_INFO		m_fiOnFuturesOption;

private:

	//_IPriceInfoEvents
	STDMETHOD(_OnStock)(struct PP::StockParams* pReq,  struct PP::StockResultsEx* pResult);
	STDMETHOD(_OnOption)(struct PP::OptionParams* pReq,  struct PP::OptionResultsEx* pResult, VARIANT_BOOL IsLastOption);
	STDMETHOD(_OnError)(PP::ErrorNumberEnum enErrorNumber, BSTR bsDescription,
		PP::RequestsTypeEnum enReqType, VARIANT varRequest);
	STDMETHOD(_OnFutureByRoot)(struct PP::FutureRootParams* pReq,  struct PP::FutureResultsEx* pResult, VARIANT_BOOL IsLastFuture);
	STDMETHOD(_OnFuturesOption)(struct PP::FuturesOptionParams* pReq,  struct PP::FuturesOptionResultsEx* pResult, VARIANT_BOOL IsLastOption);

	PP::IStructureProvider2Ptr		_GetStructureProvider()
	{
		CAutoLock lock(m_lockPp);

		return m_spStructureProvider;
	}

	void	_SetStructureProvider(PP::IStructureProvider2* pStructureProvider)
	{
		CAutoLock lock(m_lockPp);

		m_spStructureProvider = pStructureProvider;
	}
private:
	PP::IStructureProvider2Ptr	m_spStructureProvider;

	SpRequestTypeEnum			m_enCurrentRequest;

	CLock				m_lockPp;
};

};

#endif //__PPSTRUCTUREPROVIDER_H__