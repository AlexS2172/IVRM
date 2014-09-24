// Underlyings.cpp : Implementation of CUnderlyings
#include "stdafx.h"
#include "Underlyings.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnderlyings::~CUnderlyings()
{
	Detach();
}

void CUnderlyings::GetStockInfo(const _StockParams& Params)
{
	_GetStock(Params, true, false);
}

void CUnderlyings::GetStockResults(const _StockParams& Params)
{
	_GetStock(Params, false, false);
}

void CUnderlyings::GetStockResultsEx(const _StockParams& Params)
{
	_GetStock(Params, false, true);
}

void CUnderlyings::_GetStock(const _StockParams& Params, bool IsResultInInfo, bool IsResultEx)
{
	_SetDbKey(Params->Stock);

	long nResult = GetEqual();

	if(nResult == DBA_ERR_NO_ERROR)
	{
		if(IsResultInInfo)
		{
			_StockInfo info;
			info.Init();

			char name[MAX_TICKER + 1];
			ZeroMemory(name, sizeof(name));
			CreateSz(name, (const char *)m_dbaRec.securityProfile.item.symbol, MAX_TICKER);
			_bstr_t bsName(name);
			info->Name = bsName;

			info->DivFreq = m_dbaRec.securityProfile.vehicle.divFreq;

			info->DivLastDate = EgLib::vt_date(m_dbaRec.securityProfile.vehicle.exdDivDate.century*100+m_dbaRec.securityProfile.vehicle.exdDivDate.year, m_dbaRec.securityProfile.vehicle.exdDivDate.month, m_dbaRec.securityProfile.vehicle.exdDivDate.day);
			info->DivAmount = m_dbaRec.securityProfile.vehicle.exdDivAmount;
			for(int i = 0; i<m_dbaRec.securityProfile.vehicle.divFormat; i++)
				info->DivAmount  /= 10.;

			info->LotSize = 1;
			_bstr_t bsExch(L"");
			info->Exchange = bsExch;
			info->StockType = (m_dbaRec.securityProfile.item.type == IT_INDEX)?enIndex:enStock;
			_bstr_t bsCurrency(L"USD");
			info->Currency = bsCurrency;

			char buffer[MAX_DESCRIPTION + 1];
			ZeroMemory(buffer, sizeof(buffer));
			TrimRight(buffer, (const char *)m_dbaRec.securityProfile.underlying.name, MAX_DESCRIPTION);
			_bstr_t bsDesc(buffer);
			info->Description = bsDesc;

			CResponseBasePtr pStock = CResponseBasePtr((CResponseBase*)new CStockInfoResponse(Params, info));
			PublicResponse(pStock);
		}
		else if(!IsResultEx)
		{
			_StockResults results;
			results.Init();

			results->DivFreq = m_dbaRec.securityProfile.vehicle.divFreq;
			results->DivLastDate = EgLib::vt_date(m_dbaRec.securityProfile.vehicle.exdDivDate.century*100+m_dbaRec.securityProfile.vehicle.exdDivDate.year, m_dbaRec.securityProfile.vehicle.exdDivDate.month, m_dbaRec.securityProfile.vehicle.exdDivDate.day);
			results->DivAmount = m_dbaRec.securityProfile.vehicle.exdDivAmount;
			for(int i = 0; i<m_dbaRec.securityProfile.vehicle.divFormat; i++)
				results->DivAmount  /= 10.;

			results->LotSize = 1;
			_bstr_t bsExch(L"");
			results->Exchange = bsExch;
			results->StockType = (m_dbaRec.securityProfile.item.type == IT_INDEX)?enIndex:enStock;
			_bstr_t bsCurr(L"USD");
			results->Currency = bsCurr;

			char buffer[MAX_DESCRIPTION + 1];
			ZeroMemory(buffer, sizeof(buffer));
			TrimRight(buffer, (const char *)m_dbaRec.securityProfile.underlying.name, MAX_DESCRIPTION);
			_bstr_t bsDesc(buffer);
			results->Description = bsDesc;

			CResponseBasePtr pStock = CResponseBasePtr((CResponseBase*)new CStockResultResponse(Params, results));
			PublicResponse(pStock);
		}
		else
		{
			_StockResultsEx results;
			results.Init();

			results->DivFreq = m_dbaRec.securityProfile.vehicle.divFreq;
			results->DivLastDate = EgLib::vt_date(m_dbaRec.securityProfile.vehicle.exdDivDate.century*100+m_dbaRec.securityProfile.vehicle.exdDivDate.year, m_dbaRec.securityProfile.vehicle.exdDivDate.month, m_dbaRec.securityProfile.vehicle.exdDivDate.day);
			results->DivAmount = m_dbaRec.securityProfile.vehicle.exdDivAmount;
			for(int i = 0; i<m_dbaRec.securityProfile.vehicle.divFormat; i++)
				results->DivAmount  /= 10.;

			results->LotSize = 1;
			_bstr_t bsExch(L"");
			results->Exchange = bsExch;
			results->StockType = (m_dbaRec.securityProfile.item.type == IT_INDEX)?enIndex:enStock;
			_bstr_t bsCurr(L"USD");
			results->Currency = bsCurr;

			char buffer[MAX_DESCRIPTION + 1];
			ZeroMemory(buffer, sizeof(buffer));
			TrimRight(buffer, (const char *)m_dbaRec.securityProfile.underlying.name, MAX_DESCRIPTION);
			_bstr_t bsDesc(buffer);
			results->Description = bsDesc;

			results->ClosePrice = BAD_DOUBLE_VALUE;
			PublicStock(Params, results);
		}
	}
	else
	{
		CErrorResponse* pError = new CErrorResponse();
		pError->m_enRequestType = enRequestStock;
		const_cast<_StockParams&>(Params).CopyTo(pError->m_vtRequest);
		if(nResult == DBA_ERR_KEY_NOT_FOUND)
		{
			pError->m_bstrDescription = L"No such stock found";
			pError->m_Error = enNoDataAvailableForSymbol;
		}
		else
		{
			_bstr_t bs =  "Couldn't get stock info. Error: ";
			bs += EtGetMessage(DBA_ERROR,nResult);
			TCHAR buffer[0x100] = {0};				
			_ltot(nResult,buffer,10);
			bs += " (";
			bs += buffer;
			bs += ")";
			pError->m_bstrDescription = bs;
			pError->m_Error = enProviderInternalError;
		}
		CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
		PublicResponse(pErrorPtr);
	}
}

void CUnderlyings::_SetDbKey(BSTR bsUnderlying)
{
	m_nKeyNum = SP_KEY_SYMBOL;
	::StringToKey(_bstr_t(bsUnderlying), &m_dbaKey.dbaKey);
}

