// GroupPrice.h : Declaration of the CGroupPrice

#ifndef __GROUPPRICE_H__
#define __GROUPPRICE_H__

#include "Price.h"

/////////////////////////////////////////////////////////////////////////////
// CGroupPrice

class CGroupPrice : public CPrice
{

public:
	CGroupPrice();
	virtual ~CGroupPrice();

protected:

	void GetGroupPrice(const _QuoteUpdateParams& Params);
	void CancelGroupPrice(const CComVariant& Params);
	void SubscribeGroup(const _QuoteUpdateParams& Params, GroupRequestType enRequest = enGroupRequestAllNotify);
	void UnsubscribeGroup(const CComVariant& Params);
	virtual void UnsubscribeAll();

private:
	void _SetDbKey(const _QuoteUpdateParams& Params);
	long _GetOptions(const _QuoteUpdateParams& params, GroupRequestType enRequest = enGroupRequestLastQuote);
	void _CancelOptions();
	void _ParseOptionsExchange(BSTR Exchange);
	bool _IsOurOptionExchange(char cExchange);

	static long CALLBACK _OptionsProcStatic(VOID *pblk, ULONG ulFunction, DBA_KEY *pDbaKey, int nKeyLen, DBA_OPTIONS_FILTER_RECORD *pDbaRec, int nRecLen, ULONG ulKey, DWORD dwStatus, void *p);
	long _OptionsProc(ULONG ulFunction, DBA_KEY *pDbaKey, DBA_OPTIONS_FILTER_RECORD *pDbaRec, DWORD dwStatus);

protected:
	bool m_bRegionals;
	bool m_bComposites;
	typedef std::set<CHahedKeyStringPtr> tickerset;
	typedef std::map<_bstr_t, tickerset> grouptickermap;
	typedef std::set<char> exchangeset;

	OPTIONS_FILTER m_optionsFilter;	

	grouptickermap		  m_mapGroupSubscript;
	static grouptickermap m_mapGroupSubscriptGlobal;

private:
	_bstr_t					m_szUndSymbol;
	_bstr_t					m_szUndSymbolGlobal;
	CComAutoCriticalSection m_csExchanges;
	exchangeset				m_setExchanges;
	bool					m_bIsGotOptions;
	bool					m_bIsGotError;

	GroupRequestType		m_enRequestType;
	CComVariant				m_vtRequest;

	volatile bool			m_bTerminate;

	CStrategy<boost::shared_ptr<CVectorKeys>, CGroupPrice> m_UnsubscribeStrategy;

	//derived from CPrice
public:

	HRESULT OnStrategyInitialize();
	void    OnStrategyUninitialize();
	virtual void OnData(boost::shared_ptr<CVectorKeys>& Data);

	long Attach();
	long Detach();

	virtual void CopyExtendedToInfo(InstrumentTypeEnum enType, const CSubscriptionInfoPtr& pRec, _QuoteUpdateInfo& Result)
	{
		if(pRec != NULL)
		{
			CAutoLock lock(pRec->GetSubscriptionLock());
			CPrice::CopyExtendedToInfo(enType, *pRec->GetData(), Result);
		}
	}
	virtual void CopyExtendedToInfo(InstrumentTypeEnum enType, const CSubscriptionInfoPtr& pRec, QuoteUpdateInfo* Result)
	{
		if(pRec != NULL)
		{
			CAutoLock lock(pRec->GetSubscriptionLock());
			CPrice::CopyExtendedToInfo(enType, *pRec->GetData(), Result);
		}
	}

	virtual void CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3& rec, _QuoteUpdateInfo& Result)
	{
		CPrice::CopyExtendedToInfo(enType, rec, Result);
	}
	virtual void CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3& rec, QuoteUpdateInfo* Result)
	{
		CPrice::CopyExtendedToInfo(enType, rec, Result);
	}

	virtual void CopyExtendedToResult(const DBA_RECORD_3& rec, _QuoteUpdateResult& Result)
	{
		CPrice::CopyExtendedToResult(rec, Result) ;
	}
	virtual void CopyExtendedToResult(const CSubscriptionInfoPtr& pRec, _QuoteUpdateResult& Result)
	{
		if(pRec!=NULL)
		{
			CAutoLock lock(pRec->GetSubscriptionLock());
			CPrice::CopyExtendedToResult(*pRec->GetData(), Result) ;
		}
	}

	void OnRemoveAll()
	{
		m_bTerminate             = true;
		CPrice::OnRemoveAll();
	}

};
__declspec(selectany)
	CGroupPrice::grouptickermap CGroupPrice::m_mapGroupSubscriptGlobal;

#endif //__GROUPPRICE_H__
