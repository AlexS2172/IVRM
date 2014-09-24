#ifndef ISE_MANAGER_H
#define ISE_MANAGER_H

#include "ISEManagerBase.h"

typedef CComRecPtr<StockParams, &LIBID_ISEPROVIDERLib>		_StockParams;
typedef CComRecPtr<ISEStockResults, &LIBID_ISEPROVIDERLib>	_ISEStockResults;
typedef CComRecPtr<StockResults, &LIBID_ISEPROVIDERLib>		_StockResults;
typedef CComRecPtr<StockInfo, &LIBID_ISEPROVIDERLib>		_StockInfo;
typedef CComRecPtr<OptionParams, &LIBID_ISEPROVIDERLib>		_OptionParams;
typedef CComRecPtr<OptionResults, &LIBID_ISEPROVIDERLib>	_OptionResults;
typedef CComRecPtr<OptionInfo, &LIBID_ISEPROVIDERLib>	    _OptionInfo;
typedef CComRecPtr<QuoteUpdateParams, &LIBID_ISEPROVIDERLib> _QuoteUpdateParams;
typedef CComRecPtr<ISEQuoteUpdateResult, &LIBID_ISEPROVIDERLib> _ISEQuoteUpdateResult;
typedef CComRecPtr<QuoteUpdateResult, &LIBID_ISEPROVIDERLib> _QuoteUpdateResult;
typedef CComRecPtr<QuoteUpdateInfo, &LIBID_ISEPROVIDERLib>  _QuoteUpdateInfo;
typedef CComRecPtr<ISEQuote, &LIBID_ISEPROVIDERLib>			_ISEQuote;
typedef CComRecPtr<ISEDeal, &LIBID_ISEPROVIDERLib>			_ISEDeal;
typedef CComRecPtr<ISETrade, &LIBID_ISEPROVIDERLib>			_ISETrade;
typedef CComRecPtr<ISEOrder, &LIBID_ISEPROVIDERLib>			_ISEOrder;
typedef CComRecPtr<ISEMMQuoteParams, &LIBID_ISEPROVIDERLib>	_ISEMMQuoteParams;

template<class _O, class _R>
class CISEManagerStructureObject : public CISEManagerObject<_O, _R>
{
private:
	void AddStockResponse(_O pObject, const _R& Request, const _EventTypeEnum Type, 
		const CUnderlyingInfo& Info, const dwObjectID)
	{
		USES_CONVERSION;

		_ISEStockResults Result;
		Result.Init();

		if(Info.m_bLast == false)
		{
			switch(Info.m_uiType)
			{
				case UT_STOCK_INDEX:
				case UT_CURRENCY_INDEX:
				case UT_INTEREST_RATE_INDEX:
				case UT_ENERGY_INDEX:
				case UT_SOFT_AND_AGRICS_INDEX:
				case UT_METAL_INDEX:
					Result->StockType = enIndex;
					break;

				default:
					Result->StockType = enStock;
					break;
			}

			Result->Currency = SysAllocString(A2W(Info.m_sCurrency.c_str()));
			Result->Description = SysAllocString(A2W(Info.m_sDescription.c_str()));
			Result->LotSize = Info.m_lLotSize;
			Result->Exchange = SysAllocString(A2W(Info.m_sExchange.c_str()));
			Result->TraderRole = Info.m_TraderRole;
			Result->BinNum = Info.m_lBinNum;
		}
		else
		{
			Result->StockType = enStock;
			Result->Currency = SysAllocString(L"");
			Result->Description = SysAllocString(L"");
			Result->LotSize = 0;
			Result->Exchange = SysAllocString(L"");
			Result->TraderRole = enTrEAM;
			Result->BinNum = 0;
		}

		CResponseData Event;
		Result.CopyTo(Event.m_vtData);
		Event.m_vtData2 = Info.m_bLast ? VARIANT_TRUE : VARIANT_FALSE;
		Event.m_bsSymbol = Info.m_sSymbol.c_str();

		AddResponse(pObject, Type, Request, Event, dwObjectID);
	}

	void AddOptionResponse(_O pObject, const _R& Request, const COptionInfo& Info, const dwObjectID)
	{
		USES_CONVERSION;

		_OptionResults Result;
		Result.Init();
		if(Info.m_bLast == false)
		{
			switch(Info.m_uiGroup)
			{
				case IG_EUROPEAN_CALL_OPTION:
				case IG_AMERICAN_CALL_OPTION:
					Result->OptionType = enCall;
					break;

				case IG_EUROPEAN_PUT_OPTION:
				case IG_AMERICAN_PUT_OPTION:
					Result->OptionType = enPut;
					break;

				default:
					EgAssert(false);
					return;
			}

			Result->StrikePrice = Info.m_dStrikePrice;
			Result->Series = SysAllocString(A2W(Info.m_sSeries.c_str()));
			DATE dtOleDate;
			IntDateToOleDate(Info.m_uiExpirationDate, dtOleDate);
			Result->ExpirationDate = dtOleDate;
			Result->Currency = SysAllocString(A2W(Info.m_sCurrency.c_str()));;
			Result->LotSize = Info.m_lLotSize;
			Result->Exchange = SysAllocString(A2W(Info.m_sExchange.c_str()));
		}
		else
		{
			Result->OptionType = enPut;
			Result->StrikePrice = 0;
			Result->Series = SysAllocString(L"");
			Result->ExpirationDate = 0;
			Result->Currency = SysAllocString(L"");;
			Result->LotSize = 0;
			Result->Exchange = SysAllocString(L"");
		}

		CResponseData Event;
		Result.CopyTo(Event.m_vtData);
		Event.m_vtData2 = Info.m_bLast ? VARIANT_TRUE : VARIANT_FALSE;

		AddResponse(pObject, _enRequestOption, Request, Event, dwObjectID);
	}

	void AddErrorResponse(_O pObject, const _R& Request, const _EventTypeEnum Type, 
						ErrorNumberEnum Error, const _bstr_t& bsText, const dwObjectID)
	{
		CResponseData Event;
		Event.m_Error = Error;
		Event.m_bsText = bsText;

		AddResponse(pObject, (_EventTypeEnum)(Type | _enRequestError), Request, Event, dwObjectID);
	}

	void	RequestStockToVariant(const _R& Request, CComVariant& vtRequest)
	{
		//USES_CONVERSION;

		_StockParams Req;
		Req.Init();
		Req->Stock = SysAllocString(_bstr_t(Request.m_sSymbol.c_str()));
		Req->Exchange = SysAllocString(_bstr_t(Request.m_sExchange.c_str()));

		Req.CopyTo(vtRequest);
	}

	void	RequestOptionToVariant(const _R& Request, CComVariant& vtRequest)
	{
		//USES_CONVERSION;

		_OptionParams Req;
		Req.Init();
		Req->UnderlyingSymbol = SysAllocString(_bstr_t(Request.m_sSymbol.c_str()));
		Req->Exchange = SysAllocString(_bstr_t(Request.m_sExchange.c_str()));

		Req.CopyTo(vtRequest);
	}

private:

	virtual void  OnResponse(_O pObject, const CResponseContext<_R>& ResponseCtx)
	{
		CComVariant vtRequest;

		switch(ResponseCtx.m_Type)
		{
			case _enRequestAllStocks:
				{
					_R Request;
					Request.m_sSymbol = (char*)ResponseCtx.m_Data.m_bsSymbol;
					RequestStockToVariant(Request, vtRequest);
				}
				pObject->OnStock(vtRequest, ResponseCtx.m_Data.m_vtData, ResponseCtx.m_Data.m_vtData2);
				break;

			case _enRequestStock:
				RequestStockToVariant(ResponseCtx.m_Request, vtRequest);
				pObject->OnStock(vtRequest, ResponseCtx.m_Data.m_vtData, ResponseCtx.m_Data.m_vtData2);
				break;

			case _enRequestOption:
				RequestOptionToVariant(ResponseCtx.m_Request, vtRequest);
				pObject->OnOption(vtRequest, ResponseCtx.m_Data.m_vtData, ResponseCtx.m_Data.m_vtData2);
				break;

			case _enRequestStock | _enRequestError:
				RequestStockToVariant(ResponseCtx.m_Request, vtRequest);
				pObject->OnError(ResponseCtx.m_Data.m_Error, ResponseCtx.m_Data.m_bsText,
					enRequestStock, vtRequest);
				break;

			case _enRequestOption | _enRequestError:
				RequestOptionToVariant(ResponseCtx.m_Request, vtRequest);
				pObject->OnError(ResponseCtx.m_Data.m_Error, ResponseCtx.m_Data.m_bsText,
					enRequestOptions, vtRequest);
				break;
		}
	}

	virtual bool OnRequest(_O pObject, CRequestContext<_R>& Request)
	{
		bool bOk = true;

		try
		{
			switch(Request.m_Type)
			{
				case _enRequestAllStocks:
					{
						vector<CUnderlyingInfo> Unds;
						bOk = QueryAllUnderlyingsInfo(Unds);
						if(bOk)
						{
							for(vector<CUnderlyingInfo>::iterator it = Unds.begin(); it != Unds.end(); it++)
							{
								AddStockResponse(pObject, Request.m_Data, Request.m_Type, *it, Request.m_dwObjectID);
							}

							CUnderlyingInfo Und;
							Und.m_bLast = true;
							AddStockResponse(pObject, Request.m_Data, Request.m_Type, Und, Request.m_dwObjectID);
						}
					}
					break;

				case _enRequestStock:
					{
						CUnderlyingInfo Und;
						bOk = QueryUnderlyingInfo(Request.m_Data, Und);
						if(bOk)
						{
							AddStockResponse(pObject, Request.m_Data, Request.m_Type, Und, Request.m_dwObjectID);
						}
					}
					break;

				case _enRequestOption:
					{
						vector<COptionInfo> Opts;
						bOk = QueryUnderlyingOptionsInfo(Request.m_Data, Opts);
						if(bOk)
						{
							for(vector<COptionInfo>::iterator it = Opts.begin(); it != Opts.end(); it++)
							{
								AddOptionResponse(pObject, Request.m_Data, *it, Request.m_dwObjectID);
							}

							COptionInfo Info;
							Info.m_bLast = true;
							AddOptionResponse(pObject, Request.m_Data, Info, Request.m_dwObjectID);
						}
					}
					break;
			}
		}
		catch(CISEException& e)
		{					
			AddErrorResponse(pObject, Request.m_Data, Request.m_Type, 
						enSymbolNotSupported, e.GetDescription(), Request.m_dwObjectID);
		}
/*
		if(!bOk)
		{
			AddErrorResponse(pObject, Request.m_Data, Request.m_Type, 
						enNotConnected, "Not connected.", Request.m_dwObjectID);
		}
*/

		return bOk;
	}

public:

	virtual void OnConnected(const DWORD dwSession)
	{
		CISEManagerObject<_O, _R>::OnConnected(dwSession);

		map<_O, CObjectContext<_R> >::iterator itObj;
		CAutoLock Lock(&m_RefsLock);

		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			
			pObject->AddRef();
			m_RefsLock.Unlock();

			pObject->OnEvent(enEvConnected, "Connected.");

			m_RefsLock.Lock();
			pObject->Release();
		}
	}
};

template<class _O, class _R>
class CISEManagerPriceObject : public CISEManagerObject<_O, _R>
{
private:

	map<_O, set<CSubscribedSymbol> >		m_Subscriptions;

private:

	bool AddQuoteEvent(_O pObject, const _EventTypeEnum Type, 
		const _R& Request, const CQuoteBBO& BBO, const dwObjectID, 
		bool bCache = false)  ///!!!///???
	{
		CResponseData	Event;

		USES_CONVERSION;

		_ISEQuoteUpdateResult Result;
		Result.Init();
		Result->BidPrice = BBO.m_dBid;
		Result->AskPrice = BBO.m_dAsk;
		Result->BidSize = BBO.m_lBidSize;
		Result->AskSize = BBO.m_lAskSize;
		Result->TradingStatus = BBO.m_lStatus;
		Result->LastPrice = BBO.m_dLast;
		Result->ClosePrice = BBO.m_dClose;
		Result->Currency = SysAllocString(_bstr_t(BBO.m_sCurrency.c_str()));

		Result.CopyTo(Event.m_vtData);

		AddResponse(pObject, Type, Request, Event, dwObjectID, bCache);
		return true;
	}

	void AddErrorEvent(_O pObject, _EventTypeEnum Type, const _R& Request, 
					   ErrorNumberEnum Error, const _bstr_t& bsText, const dwObjectID)
	{
		//USES_CONVERSION;

		CResponseData Event;
		Event.m_Error = Error;
		Event.m_bsText = bsText;

		AddResponse(pObject, _EventTypeEnum(Type | _enRequestError), Request, Event, dwObjectID);
	}

	void RequestKeyToVariant(const _R& Request, const _EventTypeEnum Type, CComVariant& vtRequest)
	{

		//USES_CONVERSION;

		_QuoteUpdateParams Req;
		Req.Init();
		Req->Symbol = SysAllocString(_bstr_t(Request.m_sSymbol.c_str()));
		Req->Exchange = SysAllocString(_bstr_t(Request.m_sExchange.c_str()));

		switch(Type & ~_enRequestError)
		{
			case _enRequestLastStock:
			case _enSubscribeStock:
				Req->Type = enSTK;
				break;

			case _enSubscribeGroupStock:
			case _enRequestGroupStock:
				Req->Type = Request.m_InstrumentType;
				break;

			case _enRequestLastOption:
			case _enSubscribeOption:
				Req->Type = enOPT;
				break;

			case _enRequestLastIndex:
			case _enSubscribeIndex:
				Req->Type = enIDX;
				break;

			case _enSubscribeGroupIndex:
			case _enRequestGroupIndex:
				Req->Type = Request.m_InstrumentType;
				break;

			default:
				EgAssert(false);
				return;
		}

		Req.CopyTo(vtRequest);
	}

	virtual void  OnResponse(_O pObject, const CResponseContext<_R>& ResponseCtx)
	{
		CComVariant vtRequest;

		switch(ResponseCtx.m_Type)
		{
			case _enRequestGroupStock:
			case _enRequestGroupIndex:
				RequestKeyToVariant(ResponseCtx.m_Request, ResponseCtx.m_Type, vtRequest);
				pObject->OnLastQuote(vtRequest, ResponseCtx.m_Data.m_vtData);
				break;

			case _enRequestLastStock:
			case _enRequestLastIndex:
			case _enRequestLastOption:
				RequestKeyToVariant(ResponseCtx.m_Request, ResponseCtx.m_Type, vtRequest);
				pObject->OnLastQuote(vtRequest, ResponseCtx.m_Data.m_vtData);
				break;

			case _enSubscribeOption:
			case _enSubscribeStock:
			case _enSubscribeIndex:
				RequestKeyToVariant(ResponseCtx.m_Request, ResponseCtx.m_Type, vtRequest);
				pObject->OnQuoteUpdate(vtRequest, ResponseCtx.m_Data.m_vtData);
				break;

			case _enRequestLastStock | _enRequestError:
			case _enRequestLastIndex | _enRequestError:
			case _enRequestLastOption | _enRequestError:
			case _enRequestGroupStock | _enRequestError:
			case _enRequestGroupIndex | _enRequestError:
				RequestKeyToVariant(ResponseCtx.m_Request, ResponseCtx.m_Type, vtRequest);
				pObject->OnError(ResponseCtx.m_Data.m_Error, ResponseCtx.m_Data.m_bsText,
					enRequestLastQuote, vtRequest);
				break;

			case _enSubscribeOption | _enRequestError:
			case _enSubscribeGroupStock | _enRequestError:
			case _enSubscribeGroupIndex | _enRequestError:
			case _enSubscribeStock | _enRequestError:
			case _enSubscribeIndex | _enRequestError:
				RequestKeyToVariant(ResponseCtx.m_Request, ResponseCtx.m_Type, vtRequest);
				pObject->OnError(ResponseCtx.m_Data.m_Error, ResponseCtx.m_Data.m_bsText,
					enSubscribeQuote, vtRequest);
				break;
		}
	}

	virtual bool OnRequest(_O pObject, CRequestContext<_R>& Request)
	{
		bool	bOk = true;

		try
		{
			switch(Request.m_Type)
			{ 
				case _enRequestLastStock:
				case _enRequestLastIndex:
					{
						CQuoteBBO BBO;
						bOk = QueryUnderlyingBBO(Request.m_Data, BBO);
						if(bOk)
							AddQuoteEvent(pObject, Request.m_Type, Request.m_Data, BBO, Request.m_dwObjectID);
					}
					break;

				case _enRequestLastOption:
					{
						CQuoteBBO BBO;
						bOk = QueryOptionBBO(Request.m_Data, BBO);
						if(bOk)
							AddQuoteEvent(pObject, Request.m_Type, Request.m_Data, BBO, Request.m_dwObjectID);
					}
					break;

				case _enRequestGroupStock:
				case _enRequestGroupIndex:
					{
						map<CSymbolParams, CQuoteBBO>	mapSeriesBBO;
						bOk = QueryGroupOptionsBBO(Request.m_Data, mapSeriesBBO);
						if(bOk)
						{
							map<CSymbolParams, CQuoteBBO>::iterator it;
							for(it = mapSeriesBBO.begin(); it != mapSeriesBBO.end(); it++)
							{
								CSymbolParams NewRequest = it->first;
								NewRequest.m_sExchange = "";
								AddQuoteEvent(pObject, _enRequestLastOption, NewRequest, it->second, Request.m_dwObjectID);
								NewRequest.m_sExchange = ISE_EXCHANGE_CODE;
								AddQuoteEvent(pObject, _enRequestLastOption, NewRequest, it->second, Request.m_dwObjectID);
							}

							CQuoteBBO	EmptyBBO;
							EmptyBBO.m_dBid = EmptyBBO.m_dAsk = 0.0;
							EmptyBBO.m_lAskSize = EmptyBBO.m_lBidSize = 0;
							EmptyBBO.m_lStatus = 0;
							EmptyBBO.m_dLast = EmptyBBO.m_dClose = 0;
							AddQuoteEvent(pObject, Request.m_Type, Request.m_Data, EmptyBBO, Request.m_dwObjectID);
						}
					}
					break;

				case _enSubscribeStock:
				case _enSubscribeIndex:
					{
						CSubscribedSymbol	Symbol;
						Symbol.m_SymbolParams = Request.m_Data;
						Symbol.m_Type = enSTK;
						Symbol.m_bGroup = false;

						if(Request.m_bRemove)
						{
							map<_O, set<CSubscribedSymbol> >::iterator itObj = m_Subscriptions.find(pObject);
							if(itObj != m_Subscriptions.end())
							{
								set<CSubscribedSymbol>::iterator itSymb = itObj->second.find(Symbol);
								if(itSymb != itObj->second.end())
								{
									itObj->second.erase(itSymb);
									UnsubscribeUnderlying(Request.m_Data);
								}
							}
						}
						else
						{
							set<CSubscribedSymbol>&	setSymbols = m_Subscriptions[pObject];
							if(setSymbols.find(Symbol) == setSymbols.end())
							{
								bOk = SubscribeUnderlying(Request.m_Data);
								if(bOk)
								{
									setSymbols.insert(Symbol);
								}
							}
						}
					}
					break;

				case _enSubscribeOption:
				case _enSubscribeGroupStock:
				case _enSubscribeGroupIndex:
					{
						CSubscribedSymbol	Symbol;
						Symbol.m_SymbolParams = Request.m_Data;
						Symbol.m_Type = enOPT;
						Symbol.m_bGroup = (Request.m_Type != _enSubscribeOption);

						if(Request.m_bRemove)
						{
							map<_O, set<CSubscribedSymbol> >::iterator	itObj = m_Subscriptions.find(pObject);
							if(itObj != m_Subscriptions.end())
							{
								set<CSubscribedSymbol>::iterator itSymb = itObj->second.find(Symbol);
								if(itSymb != itObj->second.end())
								{
									itObj->second.erase(itSymb);
									UnsubscribeOption(Request.m_Data, Symbol.m_bGroup);
								}
							}
						}
						else
						{
							set<CSubscribedSymbol>&	setSymbols = m_Subscriptions[pObject];
							if(setSymbols.find(Symbol) == setSymbols.end())
							{
								bOk = SubscribeOption(Request.m_Data, Symbol.m_bGroup);
								if(bOk)
								{
									setSymbols.insert(Symbol);
								}
							}
						}
					}
					break;

				default:
					break;
			}
		}
		catch(CISEException& e)
		{
			AddErrorEvent(pObject, Request.m_Type, Request.m_Data, 
						enSymbolNotSupported, e.GetDescription(), Request.m_dwObjectID);
		}

/*		if(!bOk)
		{
			AddErrorEvent(pObject, Request.m_Type, Request.m_Data, 
						enNotConnected, "Not connected.", Request.m_dwObjectID);
		}
*/
		
		return bOk;
	}

	virtual void  OnRemoveObject(_O pObject)
	{
		map<_O, set<CSubscribedSymbol> >::iterator itObj = m_Subscriptions.find(pObject);
		if(itObj != m_Subscriptions.end())
		{
			set<CSubscribedSymbol>::iterator itSymb;
			for(itSymb = itObj->second.begin(); itSymb != itObj->second.end();)
			{
				try
				{
					if(itSymb->m_Type == enOPT)
					{
						UnsubscribeOption(itSymb->m_SymbolParams, itSymb->m_bGroup);
					}
					else
					{
						UnsubscribeUnderlying(itSymb->m_SymbolParams);
					}
				}
				catch(CISEException&)
				{
				}
				itSymb = itObj->second.erase(itSymb);
			}
		}
	}

protected:

	virtual void OnQuoteBBO(const CSymbolParams& Request, const _EventTypeEnum Type, const CQuoteBBO& BBO)
	{
		map<_O, CObjectContext<_R> >::iterator itObj;
		CAutoLock Lock(&m_RefsLock);

		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			CObjectContext<_R>& Ctx = itObj->second;
			CSymbolParams NewRequest = Request;
			NewRequest.m_sExchange = "";
			if(CISEManagerObject<_O, _R>::IsRequestPresent(pObject, Type, NewRequest))
			{
				AddQuoteEvent(pObject, Type, NewRequest, BBO, pObject->GetID(), true);
			}
			NewRequest.m_sExchange = ISE_EXCHANGE_CODE;
			if(CISEManagerObject<_O, _R>::IsRequestPresent(pObject, Type, NewRequest))
			{
				AddQuoteEvent(pObject, Type, NewRequest, BBO, pObject->GetID(), true);
			}
		}
	}


// overriden request filter
	virtual bool	IsRequestPresent(const _O pObject, const CRequestContext<_R>& Request) const
	{
		CAutoLock	Lock(&m_RefsLock);

		if(CISEManagerObject<_O, _R>::IsRequestPresent(pObject, Request))
			return true;

		map<_O, CObjectContext<_R> >::const_iterator itObj = m_mapObjCtx.find(pObject);

		if(itObj != m_mapObjCtx.end())
		{
			// if request_type == _enRequestOption then check for group
			set<CRequestContext<_R> >::const_iterator itReq;

			if(Request.m_Data.m_sParentSymbol.length() && 
				(Request.m_Type == _enSubscribeOption || Request.m_Type == _enRequestLastOption))
			{
				for(itReq = itObj->second.m_setRequests.begin(); itReq != itObj->second.m_setRequests.end(); itReq++)
				{
					const CRequestContext<_R>& ExistingRequest = *itReq;
					if(Request.m_Type == _enSubscribeOption && 
					   (ExistingRequest.m_Type != _enSubscribeGroupStock &&
						ExistingRequest.m_Type != _enSubscribeGroupIndex))
						continue;
					
					if(Request.m_Type == _enRequestLastOption && 
					   (ExistingRequest.m_Type != _enRequestGroupStock &&
						ExistingRequest.m_Type != _enRequestGroupIndex))
						continue;

					if(Request.m_Data.m_sParentSymbol != ExistingRequest.m_Data.m_sSymbol)
						continue;

					set<string> setExchanges;
					ExistingRequest.m_Data.SplitExchanges(setExchanges);
					if(setExchanges.find(Request.m_Data.m_sExchange) != setExchanges.end())
						return true;
				}
			}
		}

		return false;
	}

public:

	virtual void OnConnected(const DWORD dwSession)
	{
		CISEManagerObject<_O, _R>::OnConnected(dwSession);

		map<_O, CObjectContext<_R> >::iterator itObj;
		CAutoLock Lock(&m_RefsLock);

		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			
			pObject->AddRef();
			m_RefsLock.Unlock();

			pObject->OnEvent(enEvConnected, "Connected.");

			m_RefsLock.Lock();
			pObject->Release();
		}
	}
};

/*********************************************************************************************************************/
template<class _O, class _R>
class CISEManagerMMPriceObject : public CISEManagerObject<_O, _R>
{
private:

	void AddQuoteEvent(_O pObject, _EventTypeEnum Type, const _R& Request, 
		const CQuoteParams& Params, const dwObjectID)
	{
		USES_CONVERSION;

		CResponseData	Event;

		_ISEQuote Res;
		Res.Init();
		Res->Series = SysAllocString(A2W(Params.m_sSeries.c_str()));
		Res->BidOrAsk = Params.m_BidOrAsk;
		Res->Premium = Params.m_dPremium;
    	Res->Quantity = Params.m_lQuantity;
		Res->OpenQuantity = Params.m_lOpenQuantity;
		Res->StepUpBuffer = Params.m_lStepUpBuffer;

		Res.CopyTo(Event.m_vtData);

		Event.m_lTickLevel = Params.m_lTickLevel;

		AddResponse(pObject, Type, Request, Event, dwObjectID);

	}

	void AddErrorEvent(_O pObject, _EventTypeEnum Type, const _R& Request, 
					   ErrorNumberEnum Error, const _bstr_t& bsText, const dwObjectID)
	{
		//USES_CONVERSION;

		CResponseData Event;
		Event.m_Error = Error;
		Event.m_bsText = bsText;

		AddResponse(pObject, _EventTypeEnum(Type | _enRequestError), Request, Event, dwObjectID);
	}

	void RequestKeyToVariant(const _R& Request, CComVariant& vtRequest)
	{
		//USES_CONVERSION;

		CComVariant varParams;
		_ISEMMQuoteParams Req;
		Req.Init();
		Req->Symbol = SysAllocString(_bstr_t(Request.m_sSymbol.c_str()));
		Req->Type = Request.m_Type;
		Req->BidOrAsk = Request.m_BidOrAsk;
		Req.CopyTo(vtRequest);
	}


	virtual void  OnResponse(_O pObject, const CResponseContext<_R>& ResponseCtx)
	{
		CComVariant vtRequest;
		RequestKeyToVariant(ResponseCtx.m_Request, vtRequest);

		switch(ResponseCtx.m_Type)
		{
			case _enRequestLastOption:
				pObject->OnLastMmQuote(vtRequest, ResponseCtx.m_Data.m_vtData, ResponseCtx.m_Data.m_lTickLevel);
				break;

			case _enRequestLastOption | _enRequestError:
				pObject->OnError(ResponseCtx.m_Data.m_Error, ResponseCtx.m_Data.m_bsText,
					enRequestLastQuote, vtRequest);
				break;
		}
	}

	virtual bool OnRequest(_O pObject, CRequestContext<_R>& Request)
	{
		bool	bOk = true;

		try
		{
			switch(Request.m_Type)
			{ 
				case _enRequestLastOption:
					set<CQuoteParams>	setQuotes;
					bOk = QueryMmQuotes(Request.m_Data, setQuotes);
					if(bOk)
					{
						for(set<CQuoteParams>::iterator it = setQuotes.begin(); it != setQuotes.end(); it++)
						{
							AddQuoteEvent(pObject, Request.m_Type, Request.m_Data, *it, Request.m_dwObjectID);
						}
					}
					break;
			}
		}
		catch(CISEException& e)
		{
			AddErrorEvent(pObject, Request.m_Type, Request.m_Data, 
						enSymbolNotSupported, e.GetDescription(), Request.m_dwObjectID);
		}
/*
		if(!bOk)
		{
			AddErrorEvent(pObject, Request.m_Type, Request.m_Data, 
						enNotConnected, "Not connected.", Request.m_dwObjectID);
		}
*/
		
		return bOk;
	}

public:

	virtual void OnConnected(const DWORD dwSession)
	{
		CISEManagerObject<_O, _R>::OnConnected(dwSession);

		map<_O, CObjectContext<_R> >::iterator itObj;
		CAutoLock Lock(&m_RefsLock);

		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			
			pObject->AddRef();
			m_RefsLock.Unlock();

			pObject->OnEvent(enEvConnected, "Connected.");

			m_RefsLock.Lock();
			pObject->Release();
		}
	}
};

/*********************************************************************************************************************/

template<class _O, class _R>
class CISEManagerMMManagerObject : public CISEManagerObject<_O, _R>
{
	vector<CQuoteParams> m_vecQuotes;

private:

	virtual void  OnResponse(_O pObject, const CResponseContext<_R>& ResponseCtx)
	{
		switch(ResponseCtx.m_Type)
		{
			case _enQuoteRejected:
				pObject->OnQuoteRejection(ResponseCtx.m_Data.m_vtData, 
					ResponseCtx.m_Data.m_bsText);
				break;

			case _enQuoteSteppedUp:
				pObject->OnQuoteStepUp(ResponseCtx.m_Data.m_vtData);
				break;

			case _enQuoteTickWorsed:
				pObject->OnQuoteTickWorse(ResponseCtx.m_Data.m_vtData, 
					ResponseCtx.m_Data.m_lTickLevel, 
					ResponseCtx.m_Data.m_bsText);
				break;

			case _enQuoteSent:
				pObject->OnQuotesSent(ResponseCtx.m_Data.m_vtData, 
					ResponseCtx.m_Data.m_Error, ResponseCtx.m_Data.m_bsOrderID); 
				break;

			case _enDeal:
				pObject->OnDeal(ResponseCtx.m_Data.m_vtData);
				break;

			case _enTrade:
				pObject->OnTrade(ResponseCtx.m_Data.m_vtData);
				break;

		}
	}

	virtual UINT RequestProc()
	{
		const HANDLE hEvents[] = {m_hRequestEvent, m_hRequestStop};

		while(true)
		{
			DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), hEvents, FALSE, INFINITE);
			switch(dwRes)
			{
				case WAIT_OBJECT_0:
				{
					SwitchToThread();

					CAutoLock RefsLock(&m_RefsLock);
					
					vector<CQuoteParams> vecQuotes;
					map<_O, CObjectContext<_R> >::iterator itObj;
					deque<CRequestContext<_R> >::iterator itReq;

					for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end();)
					{
						_O pObject = itObj->first;
						CObjectContext<_R>& Ctx = itObj->second;

						if(!IsConnected(pObject))
						{
							itObj = m_mapObjCtx.erase(itObj);
							continue;
						}

						set<CRequestContext<_R> >::iterator itReq;

						vecQuotes.clear();

						for(itReq = Ctx.m_setRequests.begin(); itReq != Ctx.m_setRequests.end(); itReq++)
							vecQuotes.push_back(itReq->m_Data);

						Ctx.m_setRequests.clear();

						bool bOk = true;

						if(vecQuotes.size())
						{
							m_RefsLock.Unlock();

							bOk = OnRequest(pObject, vecQuotes);

							m_RefsLock.Lock();
						}

						if(!bOk)
							break;

						itObj++;
					}
				}
				break;

				default:
					return 0;
			}
		}

		return 0;
	}

	void ConvertQuote(const CQuoteParams& Params, CComVariant& vtQuote)
	{
		USES_CONVERSION;

		_ISEQuote Req;
		Req.Init();
		Req->Series = SysAllocString(A2W(Params.m_sSeries.c_str()));
		Req->BidOrAsk = Params.m_BidOrAsk;
		Req->Premium = Params.m_dPremium;
    	Req->Quantity = Params.m_lQuantity;
		Req->OpenQuantity = Params.m_lOpenQuantity;
		Req->StepUpBuffer = Params.m_lStepUpBuffer;

		Req.CopyTo(vtQuote);
	}

	bool ConvertQuotes(vector<CQuoteParams>& vecQuotes, CComVariant& vtQuotes)
	{
		_ISEQuote recQuote;
		recQuote.Init();

		ISEQuote* pQuote;

		USES_CONVERSION;

		SAFEARRAYBOUND rgsabound;
		rgsabound.lLbound = 0;
		rgsabound.cElements = vecQuotes.size();

		SAFEARRAY* psa = SafeArrayCreateEx(VT_RECORD, 1, &rgsabound, recQuote.GetIRecordInfo());
		if(psa)
		{
			vector<CQuoteParams>::iterator itQuote;
			long lIndex = 0;

			HRESULT hr = SafeArrayAccessData(psa, reinterpret_cast<PVOID*>(&pQuote));
			if(SUCCEEDED(hr))
			{
				for(itQuote = vecQuotes.begin(); itQuote != vecQuotes.end(); itQuote++)
				{
					CComVariant vtQuote;
					pQuote[lIndex].Series = SysAllocString(A2W(itQuote->m_sSeries.c_str()));
					pQuote[lIndex].BidOrAsk = itQuote->m_BidOrAsk;
					pQuote[lIndex].Premium = itQuote->m_dPremium;
    				pQuote[lIndex].Quantity = itQuote->m_lQuantity;
					pQuote[lIndex].OpenQuantity = itQuote->m_lOpenQuantity;
					pQuote[lIndex].StepUpBuffer = itQuote->m_lStepUpBuffer;
					
					lIndex++;
				}

				SafeArrayUnaccessData(psa);
				vtQuotes.vt = VT_ARRAY|VT_RECORD;
				vtQuotes.parray = psa;

				return true;
			}
		}

		return false;
	}

protected:

	virtual void OnQuoteRejection(const CQuoteParams& Quote, 
								  const string& sReason)
	{
		CResponseData  Event;
		ConvertQuote(Quote, Event.m_vtData);
		Event.m_bsText = sReason.c_str();

		CAutoLock Lock(&m_RefsLock);

		map<_O, CObjectContext<_R> >::iterator itObj;
		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			AddResponse(pObject, _enQuoteRejected, Quote, Event, 0);
		}
	}

	virtual void OnQuoteStepUp(const CQuoteParams& Quote)
	{
		CResponseData  Event;
		ConvertQuote(Quote, Event.m_vtData);

		CAutoLock Lock(&m_RefsLock);

		map<_O, CObjectContext<_R> >::iterator itObj;
		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			AddResponse(pObject, _enQuoteSteppedUp, Quote, Event, 0);
		}
	}
	
	virtual void OnQuoteTickWorse(const CQuoteParams& Quote,
								  const unsigned short uiTickWorseLevel,
								  const string&  sReason)
	{
		CResponseData  Event;

		ConvertQuote(Quote, Event.m_vtData);
		Event.m_bsText = sReason.c_str();
		Event.m_lTickLevel = uiTickWorseLevel;

		CAutoLock Lock(&m_RefsLock);

		map<_O, CObjectContext<_R> >::iterator itObj;
		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			AddResponse(pObject, _enQuoteTickWorsed, Quote, Event, 0);
		}
	}

	virtual void OnDeal(const CISEDeal& Deal)
	{
		CQuoteParams Request;
		CResponseData  Event;

		USES_CONVERSION;

		_ISEDeal recDeal;
		recDeal.Init();
		recDeal->Series = SysAllocString(A2W(Deal.m_sSeries.c_str()));
		recDeal->Quantity = Deal.m_lQuantity;
		recDeal->Price = Deal.m_dPrice;
		recDeal->OrderID = SysAllocString(A2W(Deal.m_sOrderID.c_str()));
		recDeal->BidOrAsk = Deal.m_BidOrAsk;

		recDeal.CopyTo(Event.m_vtData);

		CAutoLock Lock(&m_RefsLock);
		map<_O, CObjectContext<_R> >::iterator itObj;
		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			AddResponse(pObject, _enDeal, Request, Event, 0);
		}
	}

	virtual void OnTrade(const CISETrade& Trade)
	{
		CQuoteParams   Request;
		CResponseData  Event;

		USES_CONVERSION;

		_ISETrade recTrade;
		recTrade.Init();

		recTrade->Series = SysAllocString(A2W(Trade.m_sSeries.c_str()));
		recTrade->TradeNumber = Trade.m_lTradeNumber;
		recTrade->Type = Trade.m_Type;
		recTrade->OldSeries = SysAllocString(A2W(Trade.m_sOldSeries.c_str()));
		recTrade->OldTradeNumber = Trade.m_lOldTradeNumber;
		recTrade->OldType = Trade.m_OldType;
		recTrade->Price = Trade.m_dPrice;
		recTrade->Quantity = Trade.m_lQuantity;
		recTrade->OrderID = SysAllocString(A2W(Trade.m_sOrderID.c_str()));
		recTrade->TradeDate = Trade.m_dtDateTime;
		recTrade->IsBought = Trade.m_bIsBought ? VARIANT_TRUE : VARIANT_FALSE;
		recTrade->IsNew = Trade.m_bIsNew ? VARIANT_TRUE : VARIANT_FALSE;
		recTrade.CopyTo(Event.m_vtData);

		CAutoLock Lock(&m_RefsLock);
		map<_O, CObjectContext<_R> >::iterator itObj;
		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			AddResponse(pObject, _enTrade, Request, Event, 0);
		}
	}

	bool OnRequest(_O pObject, vector<CQuoteParams>& vecQuotes)
	{
		bool bRes = true;
		
		SendQuotes(vecQuotes);

		map<CSendQuoteParams, vector<CQuoteParams> >	mapErrorToQuotes;
		
		vector<CQuoteParams>::iterator itQuote;
		for(itQuote = vecQuotes.begin(); itQuote != vecQuotes.end(); itQuote++)
			mapErrorToQuotes[itQuote->m_SendResult].push_back(*itQuote);

		map<CSendQuoteParams, vector<CQuoteParams> >::iterator itResp;
		for(itResp = mapErrorToQuotes.begin(); itResp != mapErrorToQuotes.end(); itResp++)
		{
			CQuoteParams   Request;
			CResponseData  Event;

			if(ConvertQuotes(itResp->second, Event.m_vtData))
			{
				Event.m_Error = itResp->first.m_Error;
				Event.m_bsOrderID = itResp->first.m_sOrderID.c_str();

				AddResponse(pObject, _enQuoteSent, Request, Event, 0);

				if(itResp->first.m_Error == enNotConnected)
				{
					for(itQuote = itResp->second.begin(); itQuote != itResp->second.end(); itQuote++)
						AddRequest(pObject, *itQuote, _enSubmitQuote);

					bRes = false;
				}
			}
		}

		return bRes;
	}

public:

	HRESULT	SetMMParameters(IMMParameters* pParameters, bool bPMM);

	HRESULT	StartTrading();

	HRESULT EnterOrder( 
		const string& sSeries, 
		const unsigned int uiQuantity, 
		const double dPremium,
		const BidAskEnum BidOrAsk,
		string& sOrderID);

	HRESULT	AddRequest(
		const _O pObject, 
		const _R& Request, 
		const _EventTypeEnum Type, 
		const bool bOverride = false)
	{
		CAutoLock	Lock(&m_RefsLock); 

		if(IsConnected(pObject) == false)
			return ComError(E_FAIL, L"Object is not connected");

		map<_O, CObjectContext<_R> >::iterator itObj = m_mapObjCtx.find(pObject);

		CObjectContext<_R>& Ctx = itObj->second;
		CRequestContext<_R> Req;
		Req.m_Data = Request;
		Req.m_Type = Type;

		set<CRequestContext<_R> >::iterator itReq;
		pair<set<CRequestContext<_R> >::iterator, bool> itInsReq;

		if(bOverride)
		{
			itInsReq = Ctx.m_setRequests.insert(Req);
			itInsReq.first->m_dwObjectID = pObject->GetID();
			if(!itInsReq.second)
			{
				itInsReq.first->m_Data.m_dPremium = Request.m_dPremium;
				itInsReq.first->m_Data.m_lQuantity = Request.m_lQuantity;
				itInsReq.first->m_Data.m_lOpenQuantity = Request.m_lOpenQuantity;
				itInsReq.first->m_Data.m_lStepUpBuffer = Request.m_lStepUpBuffer;
			}

			switch(Request.m_BidOrAsk)
			{
				case enBoth:
					// if need to update both sides then delete each
					Req.m_Data.m_BidOrAsk = enBid;
					itReq = Ctx.m_setRequests.find(Req);
					if(itReq != Ctx.m_setRequests.end())
						Ctx.m_setRequests.erase(itReq);

					Req.m_Data.m_BidOrAsk = enAsk;
					itReq = Ctx.m_setRequests.find(Req);
					if(itReq != Ctx.m_setRequests.end())
						Ctx.m_setRequests.erase(itReq);
					break;

				default:
					// if need to update one side then delete both and add another one
					Req.m_Data.m_BidOrAsk = enBoth;
					itReq = Ctx.m_setRequests.find(Req);
					if(itReq != Ctx.m_setRequests.end())
					{
						CRequestContext<_R> ChangedReq = *itReq;
						Ctx.m_setRequests.erase(itReq);

						ChangedReq.m_Data.m_BidOrAsk = (Request.m_BidOrAsk == enBid ? enAsk : enBid);
						itInsReq = Ctx.m_setRequests.insert(ChangedReq);
						itInsReq.first->m_dwObjectID = pObject->GetID();
						if(!itInsReq.second)
						{
							itInsReq.first->m_Data.m_dPremium = ChangedReq.m_Data.m_dPremium;
							itInsReq.first->m_Data.m_lQuantity = ChangedReq.m_Data.m_lQuantity;
							itInsReq.first->m_Data.m_lOpenQuantity = ChangedReq.m_Data.m_lOpenQuantity;
							itInsReq.first->m_Data.m_lStepUpBuffer = ChangedReq.m_Data.m_lStepUpBuffer;
						}
					}
					break;
			}

			SetEvent(m_hRequestEvent);
		}
		else
		{
			BidAskEnum BidOrAsk = Req.m_Data.m_BidOrAsk;
			Req.m_Data.m_BidOrAsk = enBid;
			bool bBid = (Ctx.m_setRequests.find(Req) != Ctx.m_setRequests.end());
			Req.m_Data.m_BidOrAsk = enAsk;
			bool bAsk = (Ctx.m_setRequests.find(Req) != Ctx.m_setRequests.end());
			Req.m_Data.m_BidOrAsk = enBoth;
			bool bBoth = (Ctx.m_setRequests.find(Req) != Ctx.m_setRequests.end());
			Req.m_Data.m_BidOrAsk = BidOrAsk;

			switch(BidOrAsk)
			{
				case enBid:
					if(!bBoth && !bBid)
						Ctx.m_setRequests.insert(Req);
					break;
				case enAsk:
					if(!bBoth && !bAsk)
						Ctx.m_setRequests.insert(Req);
					break;
				case enBoth:
					if(!bBoth && !(bAsk && bBid))
					{
						if(bAsk)
							Req.m_Data.m_BidOrAsk = enBid;
						else if(bBid)
							Req.m_Data.m_BidOrAsk = enAsk;
						else
							Req.m_Data.m_BidOrAsk = enBoth;

						Ctx.m_setRequests.insert(Req);
					}
					break;
			}

		}

		return S_OK;
	}
};

template<class _O, class _R>
class CISEManagerTracerObject : public CISEManagerObject<_O, _R>
{
private:

	virtual void OnResponse(_O pObject, const CResponseContext<_R>& ResponseCtx)
	{
		switch (ResponseCtx.m_Type)
		{
			case _enHeartbeat:
				pObject->OnHeartbeat(ResponseCtx.m_Data.m_lReason);
				break;

			default:
				pObject->OnMessage(ResponseCtx.m_Data.m_InfoType, ResponseCtx.m_Data.m_bsText);
		}
	}

protected:

	virtual void OnMessage(const ISEInfoTypeEnum Type, const string& sText)
	{
		int Request = 0;
		CResponseData	Event;

		Event.m_bsText = sText.c_str();
		Event.m_InfoType = Type;

		CAutoLock	Lock(&m_RefsLock);

		map<_O, CObjectContext<_R> >::iterator itObj;
		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			AddResponse(pObject, _enNoRequest, Request, Event, 0);
		}
	}

	virtual void OnHeartbeat(const long lInstance)
	{
		int Request = 0;
		CResponseData	Event;

		CAutoLock	Lock(&m_RefsLock);

		Event.m_lReason = lInstance;

		map<_O, CObjectContext<_R> >::iterator itObj;
		for(itObj = m_mapObjCtx.begin(); itObj != m_mapObjCtx.end(); itObj++)
		{
			_O pObject = itObj->first;
			AddResponse(pObject, _enHeartbeat, Request, Event, 0, true);
		}
	}

public:

	HRESULT	Bind(const _O pObject)
	{
		CObjectContext<_R>	Ctx;

		CAutoLock	Lock(&m_RefsLock);

		if(m_mapObjCtx.empty())
			if(Start(false, true))
				return ComError(E_FAIL, L"Failed to start object's thread.");
	
		m_mapObjCtx[pObject] = Ctx;

		return S_OK;
	}

	HRESULT	Unbind(const _O pObject)
	{
		CAutoLock	Lock(&m_RefsLock);

		map<_O, CObjectContext<_R> >::iterator it;

		it = m_mapObjCtx.find(pObject);
		
		if(it != m_mapObjCtx.end())
		{
			m_mapObjCtx.erase(it);
		}

		if(m_mapObjCtx.empty())
			Stop();

		return S_OK;
	}
};

typedef CISEManagerStructureObject<CISEStructureBase*, CSymbolParams>	CStructureProviderManagerObject;
typedef CISEManagerPriceObject<CISEPriceBase*, CSymbolParams>			CPriceProviderManagerObject;
typedef CISEManagerMMManagerObject<CISEMMManager*, CQuoteParams>		CMMManagerObject;
typedef CISEManagerMMPriceObject<CISEMMManager*, CMMQuoteParams>		CMMManagerPriceObject;
typedef CISEManagerTracerObject<CISETracer*, int>						CISETracerManagerObject;

class CISEManager : 
    public	CStructureProviderManagerObject,
	public  CPriceProviderManagerObject,
	public  CMMManagerObject,
	public  CISETracerManagerObject,
	public  CMMManagerPriceObject
{
private:

	DWORD	m_dwPrevSessionID;
	CLock	m_DisconnectLock;

private:
// overrides
	virtual void OnQuoteBBO(const CSymbolParams& RequestParams, 
							const _EventTypeEnum RequestType,
							const CQuoteBBO&	 BBO);	

	virtual void OnQuoteRejection(const CQuoteParams& Params, 
								  const string& sReason);

	virtual void OnQuoteStepUp(const CQuoteParams& Params);
	
	virtual void OnQuoteTickWorse(const CQuoteParams& Params,
								  const unsigned short uiTickWorseLevel,
								  const string& sReason);

	virtual void OnMessage(const ISEInfoTypeEnum Type, 
						   const string& sText);

	void OnHeartbeat(const long lInstance);

	virtual void OnDeal(const CISEDeal& Deal);

	virtual void OnTrade(const CISETrade& Trade);

	virtual void OnConnected(const DWORD dwSession);

	virtual void OnDisconnected(const DWORD dwSession);

public:

	CISEManager();
	~CISEManager();
};

extern CISEManager*	theISEManager;

#endif