#include "stdafx.h"
#include "CommohFunctions.h"

void MakeCopy(_QuoteUpdateInfo & ri, QuoteUpdateInfo & vi)
{
	//std::ostringstream oss;
	ri->AskExchange = (_bstr_t(vi.AskExchange)).copy();
	ri->BidExchange = (_bstr_t(vi.BidExchange)).copy();
	ri->Currency = (_bstr_t(vi.Currency)).copy();
	ri->Exchange = (_bstr_t(vi.Exchange)).copy();
	ri->AskSize = vi.AskSize;
	//oss << "; AskSize=" ;oss << std::fixed <<ri->AskSize;
	ri->AskPrice = vi.AskPrice;
	//oss << "; AskPrice=" ;oss << std::fixed << ri->AskPrice;
	ri->BidPrice = vi.BidPrice;
	//oss << "; BidPrice=" ;oss << std::fixed << ri->BidPrice;
	ri->BidSize = vi.BidSize;
	//oss << "; BidSize=" ;oss << ri->BidSize;
	ri->ClosePrice = vi.ClosePrice;
	//oss << "; ClosePrice=" ;oss << std::fixed << ri->ClosePrice;
	ri->HighPrice = vi.HighPrice;
	//oss << "; HighPrice=" ;oss << std::fixed << ri->HighPrice;
	ri->LastPrice = vi.LastPrice;
	//oss << "; LastPrice=" ;oss << std::fixed << ri->LastPrice;
	ri->LotSize = vi.LotSize;
	//oss << "; LotSize=" ;oss << ri->LotSize;
	ri->LowPrice = vi.LowPrice;
	//oss << "; LowPrice=" ;oss << std::fixed << ri->LowPrice;
	ri->NetChange = vi.NetChange;
	//oss << "; NetChange=" ;oss << std::fixed << ri->NetChange;
	ri->OpenInterest = vi.OpenInterest;
	//oss << "; OpenInterest=" ;oss << std::fixed << ri->OpenInterest;
	ri->OpenPrice = vi.OpenPrice;
	//oss << "; OpenPrice=" ;oss << std::fixed << ri->OpenPrice;
	ri->PriceTick = vi.PriceTick;
	//oss << "; PriceTick=" ;oss << ri->PriceTick;
	ri->SeqNum = vi.SeqNum;
	//oss << "; SeqNum=" ;oss << ri->SeqNum;
	ri->Skipped = vi.Skipped;
	//oss << "; Skipped=" ;oss << ri->Skipped;
	ri->Status = vi.Status;
	//oss << "; Status=" ;oss << std::fixed << ri->Status;
	ri->TotalRequests = vi.TotalRequests;
	//oss << "; TotalRequests=" ;oss << ri->TotalRequests;
	ri->UpdateDateTime = vi.UpdateDateTime;
	//oss << "; UpdateDateTime=" ;oss << std::fixed << ri->UpdateDateTime;
	ri->Volume = vi.Volume;
	//oss << "; Volume=" ;oss << ri->Volume;
	ri->WaitTime = vi.WaitTime;
	//oss << "; WaitTime=" ;oss << ri->WaitTime;
	//std::string str(oss.str());
	//TRACEINFO(__FUNCTION__,str.c_str());
}

void MakeCopy(IRisks & ri, IRisks& vi)
{
	//std::ostringstream oss;
	ri.TheoPrice = vi.TheoPrice;
	//oss << "; TheoPrice=" ;oss << std::fixed << ri->TheoPrice;
	ri.Alpha = vi.Alpha;
	//oss << "; Alpha=" ;oss << std::fixed << ri->Alpha;
	ri.Delta = vi.Delta;
	//oss << "; Delta=" ;oss << ri->Delta;
	ri.Gamma = vi.Gamma;
	//oss << "; Gamma=" ;oss << std::fixed << ri->Gamma;
	ri.Vega = vi.Vega;
	//oss << "; Vega=" ;oss << std::fixed << ri->Vega;
	ri.Theta = vi.Theta;
	//oss << "; Theta=" ;oss << std::fixed << ri->Theta;

	ri.Rho = vi.Rho;
	//oss << "; Rho=" ;oss << std::fixed << ri->Rho;
	ri.Volga = vi.Volga;
	//oss << "; Volga=" ;oss << std::fixed << ri->Volga;
	ri.DeltaVega = vi.DeltaVega;
	//oss << "; DeltaVega=" ;oss << std::fixed << ri->DeltaVega;
	ri.DeltaTheta = vi.DeltaTheta;
	//oss << "; DeltaTheta=" ;oss << std::fixed << ri->DeltaTheta;
	ri.GammaVega = vi.GammaVega;
	//oss << "; GammaVega=" ;oss << std::fixed << ri->GammaVega;
	ri.GammaTheta = vi.GammaTheta;
	//oss << "; GammaTheta=" ;oss << std::fixed << ri->GammaTheta;
	ri.IVola = vi.IVola;
	//oss << "; IVola=" ;oss << std::fixed << ri->IVola;
	ri.IVolaBid = vi.IVolaBid;
	//oss << "; IVolaBid=" ;oss << std::fixed << ri->IVolaBid;
	ri.IVolaAsk = vi.IVolaAsk;
	//oss << "; IVolaAsk=" ;oss << std::fixed << ri->IVolaAsk;
	ri.TheoVola = vi.TheoVola;
	//oss << "; TheoVola=" ;oss << std::fixed << ri->TheoVola;
	ri.BaseContractPrice = vi.BaseContractPrice;
	//oss << "; BaseContractPrice=" ;oss << std::fixed << ri->BaseContractPrice;
	ri.ContractPrice = vi.ContractPrice;
	//oss << "; ContractPrice=" ;oss << std::fixed << ri->ContractPrice;
	
	//std::string str(oss.str());
	//TRACEINFO(__FUNCTION__,str.c_str());
}

void MakeCopy(_QuoteUpdateParams & rp, QuoteUpdateParams & vp)
{
	//std::string str = "Symbol = ";
	//str+= (char*) _bstr_t(vp.Symbol);
	//TRACEINFO(__FUNCTION__, str.c_str());
	rp->Exchange = _bstr_t("").copy();
	rp->Type = vp.Type;
	rp->Symbol = (_bstr_t(vp.Symbol).copy());
}

void MakeCopy(ITicker & rp, ITicker & vp)
{
	//std::string str = "Symbol = ";
	//str+= (char*) _bstr_t(vp.Symbol);
	//TRACEINFO(__FUNCTION__, str.c_str());
	rp.Exchange = _bstr_t("").copy();
	rp.Type = vp.Type;
	rp.Symbol = (_bstr_t(vp.Symbol).copy());
}

void MakeCopy(QuoteUpdateParams & rp, QuoteUpdateParams & vp)
{
	//std::string str = "Symbol = ";
	//str+= (char*) _bstr_t(vp.Symbol);
	//TRACEINFO(__FUNCTION__, str.c_str());
	rp.Exchange = _bstr_t("").copy();
	rp.Type = vp.Type;
	rp.Symbol = (_bstr_t(vp.Symbol).copy());
}

void MakeCopy(QuoteUpdateInfo & ri, QuoteUpdateInfo & vi)
{
	//std::ostringstream oss;
	ri.AskExchange = (_bstr_t(vi.AskExchange)).copy();
	ri.BidExchange = (_bstr_t(vi.BidExchange)).copy();
	ri.Currency = (_bstr_t(vi.Currency)).copy();
	ri.Exchange = (_bstr_t(vi.Exchange)).copy();
	ri.AskSize = vi.AskSize;
	//oss << "; AskSize=" ;oss << std::fixed <<ri.AskSize;
	ri.AskPrice = vi.AskPrice;
	//oss << "; AskPrice=" ;oss << std::fixed << ri.AskPrice;
	ri.BidPrice = vi.BidPrice;
	//oss << "; BidPrice=" ;oss << std::fixed << ri.BidPrice;
	ri.BidSize = vi.BidSize;
	//oss << "; BidSize=" ;oss << ri.BidSize;
	ri.ClosePrice = vi.ClosePrice;
	//oss << "; ClosePrice=" ;oss << std::fixed << ri.ClosePrice;
	ri.HighPrice = vi.HighPrice;
	//oss << "; HighPrice=" ;oss << std::fixed << ri.HighPrice;
	ri.LastPrice = vi.LastPrice;
	//oss << "; LastPrice=" ;oss << std::fixed << ri.LastPrice;
	ri.LotSize = vi.LotSize;
	//oss << "; LotSize=" ;oss << ri.LotSize;
	ri.LowPrice = vi.LowPrice;
	//oss << "; LowPrice=" ;oss << std::fixed << ri.LowPrice;
	ri.NetChange = vi.NetChange;
	//oss << "; NetChange=" ;oss << std::fixed << ri.NetChange;
	ri.OpenInterest = vi.OpenInterest;
	//oss << "; OpenInterest=" ;oss << std::fixed << ri.OpenInterest;
	ri.OpenPrice = vi.OpenPrice;
	//oss << "; OpenPrice=" ;oss << std::fixed << ri.OpenPrice;
	ri.PriceTick = vi.PriceTick;
	//oss << "; PriceTick=" ;oss << ri.PriceTick;
	ri.SeqNum = vi.SeqNum;
	//oss << "; SeqNum=" ;oss << ri.SeqNum;
	ri.Skipped = vi.Skipped;
	//oss << "; Skipped=" ;oss << ri.Skipped;
	ri.Status = vi.Status;
	//oss << "; Status=" ;oss << std::fixed << ri.Status;
	ri.TotalRequests = vi.TotalRequests;
	//oss << "; TotalRequests=" ;oss << ri.TotalRequests;
	ri.UpdateDateTime = vi.UpdateDateTime;
	//oss << "; UpdateDateTime=" ;oss << std::fixed << ri.UpdateDateTime;
	ri.Volume = vi.Volume;
	//oss << "; Volume=" ;oss << ri.Volume;
	ri.WaitTime = vi.WaitTime;
	//oss << "; WaitTime=" ;oss << ri.WaitTime;
	//std::string str(oss.str());
	//TRACEINFO(__FUNCTION__,str.c_str());
};

void MakeCopy(_OptionUpdateInfo & ri, OptionResultsEx & vp)
{
	ri->YVolume = vp.YVolume;
	ri->Volume = vp.Volume;
	ri->StrikePrice = vp.StrikePrice;
	ri->SettlementType = vp.SettlementType;
	ri->Series = _bstr_t(vp.Series).copy(); //
	ri->OptionType = vp.OptionType;
	ri->OpenInterest = vp.OpenInterest;
	ri->LotSize = vp.LotSize;
	ri->Last = vp.Last;
	ri->ExpirationType = vp.ExpirationType;
	ri->ExpirationDate = vp.ExpirationDate;
	ri->Exchange = _bstr_t(vp.Exchange).copy(); //
	ri->Currency = _bstr_t(vp.Currency).copy(); //
	ri->Ask = vp.Ask;
	ri->Bid = vp.Bid;
	ri->CloseAsk = vp.CloseAsk;
	ri->CloseBid = vp.CloseBid;
	ri->ClosePrice = vp.ClosePrice;
};

void MakeCopy(_DividendsUpdateInfo & ri, DividendsResults & vp)
{
	ri->Market.Amount = vp.Market.Amount;
	ri->Market.Date = vp.Market.Date;
	ri->Market.Freq = vp.Market.Freq;
	ri->Custom.Amount = vp.Custom.Amount;
	ri->Custom.Date = vp.Custom.Date;
	ri->Custom.Freq = vp.Custom.Freq;
};

void MakeCopy(_StockUpdateInfo & ri, StockResultsEx & vp)
{
	ri->Volume = vp.Volume;
	ri->StockType = vp.StockType;
	ri->OpenPrice = vp.OpenPrice;
	ri->Low = vp.Low;
	ri->LotSize = vp.LotSize;
	ri->LastTradeVolume = vp.LastTradeVolume;
	ri->High = vp.High;
	ri->Exchange = _bstr_t(vp.Exchange).copy(); //
	ri->DivLastDate = vp.DivLastDate;
	ri->DivFreq = vp.DivFreq;
	ri->DivAmount = vp.DivAmount;
	ri->Description = _bstr_t(vp.Description).copy(); //
	ri->CurrentVolume = vp.CurrentVolume;
	ri->Currency = _bstr_t(vp.Currency).copy();//
	ri->CompositeVolume = vp.CompositeVolume;
	ri->ClosePrice = vp.ClosePrice;
	ri->CloseBid = vp.CloseBid;
	ri->CloseAsk = vp.CloseAsk;
	ri->Bid = vp.Bid;
	ri->Ask = vp.Ask;
}


void MakeCopy(_OptionUpdateParams & rp, OptionParams & vp)
{
	rp->Exchange = _bstr_t("").copy();
	rp->UnderlyingSymbol = (_bstr_t(vp.UnderlyingSymbol).copy());
}

void MakeCopy(_StockUpdateParams & rp, StockParams & vp)
{
	rp->Exchange = _bstr_t("").copy();
	rp->Stock = (_bstr_t(vp.Stock).copy());
}


void MakeCopy(_FutureUpdateInfo & ri, FutureResultsEx & vp)
{
	ri->Ask = vp.Ask;
	ri->Bid = vp.Bid;
	ri->ClosePrice = vp.ClosePrice;
	ri->ContractSize = vp.ContractSize;
	ri->Description = (_bstr_t(vp.Description).copy()); //BSTR
	ri->ExpirationDate = vp.ExpirationDate;
	ri->LastTradingDate = vp.LastTradingDate;
	ri->MinimumTick = vp.MinimumTick;
	ri->OptionContractSize = vp.OptionContractSize;
	ri->Symbol = (_bstr_t(vp.Symbol).copy()); //BSTR
	ri->Trade = vp.Trade;
	ri->UnitOfMeasure = vp.UnitOfMeasure;
	ri->UnitOfMeasureCurrency = vp.UnitOfMeasureCurrency; //BSTR
}

void MakeCopy(_FutureUpdateParams & ri, FutureParams & vp)
{
	ri->FutureSymbol = (_bstr_t(vp.FutureSymbol).copy());
}

void MakeCopy(_FutureRootUpdateParams & ri, FutureRootParams & vp)
{
	ri->FuturesRootSymbol = (_bstr_t(vp.FuturesRootSymbol).copy());
}

void MakeCopy(_FutureOptionUpdateInfo & ri, FuturesOptionResultsEx & vp)
{
	ri->Ask = vp.Ask;
	ri->Bid = vp.Bid;
	ri->CloseAsk = vp.CloseAsk;
	ri->CloseBid = vp.CloseBid;
	ri->ClosePrice = vp.ClosePrice;
	ri->ExpirationDate = vp.ExpirationDate;
	ri->Last = vp.Last;
	ri->LastTradingDate = vp.LastTradingDate;
	ri->LotSize = vp.LotSize;
	ri->OptionType = vp.OptionType;
	ri->Series = (_bstr_t(vp.Series).copy());
	ri->StrikePrice = vp.StrikePrice;
}

void MakeCopy(_FutureOptionUpdateParams & ri, FuturesOptionParams & vp)
{
	ri->Exchange = (_bstr_t(vp.Exchange).copy());
	ri->FutureSymbol = (_bstr_t(vp.FutureSymbol).copy());
}