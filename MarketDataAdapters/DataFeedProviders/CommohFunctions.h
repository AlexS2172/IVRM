#include "stdafx.h"


void MakeCopy(_QuoteUpdateInfo & ri, QuoteUpdateInfo & vi);

void MakeCopy(_QuoteUpdateParams & rp, QuoteUpdateParams & vp);

void MakeCopy(IRisks & rp, IRisks & vp);

void MakeCopy(QuoteUpdateParams & rp, QuoteUpdateParams & vp);

void MakeCopy(QuoteUpdateInfo & ri, QuoteUpdateInfo & vi);

void MakeCopy(_OptionUpdateInfo & ri, OptionResultsEx & vp);

void MakeCopy(_StockUpdateInfo & ri, StockResultsEx & vp);

void MakeCopy(_OptionUpdateParams & rp, OptionParams & vp);

void MakeCopy(_StockUpdateParams & rp, StockParams & vp);

void MakeCopy(_DividendsUpdateInfo & ri, DividendsResults & vp);

void MakeCopy(ITicker & rp, ITicker & vp);

void MakeCopy(_FutureUpdateInfo & ri, FutureResultsEx & vp);

void MakeCopy(_FutureUpdateParams & ri, FutureParams & vp);

void MakeCopy(_FutureRootUpdateParams & ri, FutureRootParams & vp);

void MakeCopy(_FutureOptionUpdateInfo & ri, FuturesOptionResultsEx & vp);

void MakeCopy(_FutureOptionUpdateParams & ri, FuturesOptionParams & vp);