// EtsPriceProfileAtom.cpp : Implementation of CEtsPriceProfileAtom
#include "stdafx.h"
#include "EtsPriceProfileAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetUndPriceMid(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound, EtsReplacePriceStatusEnum* penPriceStatus,VARIANT_BOOL ManualEdit, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if ( ManualEdit == VARIANT_TRUE ||
			 m_bAlwaysUseLast == VARIANT_TRUE ||	
			 ( DoubleEQZero(dPriceBid) && DoubleEQZero(dPriceAsk) ) ||
			 ( m_enBadUndPriceRule == enUbprCheckTolerance && !CheckTolerance(dPriceBid, dPriceAsk, dPriceLast, dTolerance) ))
		{
			dRetVal = dPriceLast;
			nPriceStatus = enRpsBoth;
		}
		else
		{
			dRetVal = (dPriceBid + dPriceAsk) / 2;
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, .01, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetUndPriceBidForPnL(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if ( (m_bAlwaysUseLast == VARIANT_TRUE) ||
			(DoubleEQZero(dPriceBid) && DoubleEQZero(dPriceAsk)) ||
			(m_enBadUndPriceRule == enUbprCheckTolerance && !CheckTolerance(dPriceBid, dPriceAsk, dPriceLast, dTolerance)) )
		{
			dRetVal = dPriceLast;
			nPriceStatus = enRpsBoth;
		}
		else if (m_bUseMidMarketForPnL != VARIANT_FALSE)
		{
			dRetVal = (dPriceBid + dPriceAsk) / 2;
		}
		else
		{
			dRetVal = dPriceBid;
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, .01, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetUndPriceAskForPnL(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if ( (m_bAlwaysUseLast == VARIANT_TRUE) ||
			(DoubleEQZero(dPriceBid) && DoubleEQZero(dPriceAsk)) ||
			(m_enBadUndPriceRule == enUbprCheckTolerance && !CheckTolerance(dPriceBid, dPriceAsk, dPriceLast, dTolerance)) )
		{
			dRetVal = dPriceLast;
			nPriceStatus = enRpsBoth;
		}
		else if (m_bUseMidMarketForPnL != VARIANT_FALSE)
		{
			dRetVal = (dPriceBid + dPriceAsk) / 2;
		}
		else
		{
			dRetVal = dPriceAsk;
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, .01, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetUndPriceBid(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if ( (m_bAlwaysUseLast == VARIANT_TRUE) ||
			(DoubleEQZero(dPriceBid) && DoubleEQZero(dPriceAsk)) ||
			(m_enBadUndPriceRule == enUbprCheckTolerance && !CheckTolerance(dPriceBid, dPriceAsk, dPriceLast, dTolerance)) )
		{
			dRetVal = dPriceLast;
			nPriceStatus = enRpsBoth;
		}
		else
		{
			dRetVal = dPriceBid;
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, .01, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetUndPriceAsk(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	DOUBLE dTolerance, EtsPriceRoundingRuleEnum enPriceRound, EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if ( (m_bAlwaysUseLast == VARIANT_TRUE) ||
			(DoubleEQZero(dPriceBid) && DoubleEQZero(dPriceAsk)) ||
			(m_enBadUndPriceRule == enUbprCheckTolerance && !CheckTolerance(dPriceBid, dPriceAsk, dPriceLast, dTolerance)) )
		{
			dRetVal = dPriceLast;
			nPriceStatus = enRpsBoth;
		}
		else
		{
			dRetVal = dPriceAsk;
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, .01, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetOptPriceMid(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola, DOUBLE dPriceTheo,
	EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;
	if (dPriceTheo < 0) dPriceTheo = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (!m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is not option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		bool bZeroBid = DoubleEQZero(dPriceBid);
		bool bZeroAsk = DoubleEQZero(dPriceAsk);

		if (bZeroBid && bZeroAsk)
		{
			if (m_enBadOptBothPriceRule == enObbprUseLast || bUseTheoVola == VARIANT_FALSE )
				dRetVal = dPriceLast;
			else
				dRetVal = dPriceTheo;

			nPriceStatus = enRpsBoth;
		}
		else
		{
			if (bZeroBid)
			{
				dPriceBid = ReplacePrice(dPriceTheo, dPriceLast, dPriceAsk, bUseTheoVola);
				nPriceStatus = enRpsBid;
			}

			if (bZeroAsk)
			{
				dPriceAsk = ReplacePrice(dPriceTheo, dPriceLast, dPriceBid, bUseTheoVola);
				nPriceStatus |= enRpsAsk;
			}

			dRetVal = (dPriceBid + dPriceAsk) / 2;
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, dRetVal <= 3. ? .05 : .1, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetOptPriceBid(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola, DOUBLE dPriceTheo, 
	EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;
	if (dPriceTheo < 0) dPriceTheo = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (!m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is not option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		bool bZeroBid = DoubleEQZero(dPriceBid);
		bool bZeroAsk = DoubleEQZero(dPriceAsk);

		if (bZeroBid && bZeroAsk)
		{
			if (m_enBadOptBothPriceRule == enObbprUseLast || bUseTheoVola == VARIANT_FALSE )
				dRetVal = dPriceLast;
			else
				dRetVal = dPriceTheo;

			nPriceStatus = enRpsBid;
		}
		else
		{
			if (bZeroBid)
			{
				dPriceBid = ReplacePrice(dPriceTheo, dPriceLast, dPriceAsk, bUseTheoVola);
				nPriceStatus = enRpsBid;
			}

			dRetVal = dPriceBid;
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, dRetVal <= 3. ? .05 : .1, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetOptPriceAsk(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola, DOUBLE dPriceTheo,
	EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;
	if (dPriceTheo < 0) dPriceTheo = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (!m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is not option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		bool bZeroBid = DoubleEQZero(dPriceBid);
		bool bZeroAsk = DoubleEQZero(dPriceAsk);

		if (bZeroBid && bZeroAsk)
		{
			if (m_enBadOptBothPriceRule == enObbprUseLast || bUseTheoVola == VARIANT_FALSE )
				dRetVal = dPriceLast;
			else
				dRetVal = dPriceTheo;

			nPriceStatus = enRpsAsk;
		}
		else
		{
			if (bZeroAsk)
			{
				dPriceAsk = ReplacePrice(dPriceTheo, dPriceLast, dPriceBid, bUseTheoVola);
				nPriceStatus = enRpsAsk;
			}

			dRetVal = dPriceAsk;
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, dRetVal <= 3. ? .05 : .1, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetOptPriceBidForPnL(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola, DOUBLE dPriceTheo, 
	EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;
	if (dPriceTheo < 0) dPriceTheo = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (!m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is not option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		bool bZeroBid = DoubleEQZero(dPriceBid);
		bool bZeroAsk = DoubleEQZero(dPriceAsk);
		bool bZeroLast = DoubleEQZero(dPriceLast);

		if (bZeroBid && bZeroAsk)
		{
			if (/*!bZeroLast &&*/ (m_enBadOptBothPriceRule == enObbprUseLast || bUseTheoVola == VARIANT_FALSE) )
				dRetVal = dPriceLast;
			else
				dRetVal = dPriceTheo;
	
			nPriceStatus = enRpsBid;
		}
///		else
//		{
//			if (bZeroBid)
//			{
//				dPriceBid = ReplacePrice(dPriceTheo, dPriceLast, dPriceAsk, bUseTheoVola);
//				nPriceStatus = enRpsBid;
//			}
//
//			if (m_bUseMidMarketForPnL != VARIANT_FALSE)
//			{
//				if (bZeroAsk)
//				{
//					dPriceAsk = ReplacePrice(dPriceTheo, dPriceLast, dPriceBid, bUseTheoVola);
//					nPriceStatus |= enRpsAsk;
//				}
//
//				dRetVal = (dPriceBid + dPriceAsk) / 2;
//			}
//			else
//			{
//				dRetVal = dPriceBid;
//			}
//		}
		else
		{
			if (!bZeroBid)
			{
				if (m_bUseMidMarketForPnL != VARIANT_FALSE)
				{
					if (bZeroAsk)
					{
						dPriceAsk = ReplacePrice(dPriceTheo, dPriceLast, dPriceBid, bUseTheoVola);
						nPriceStatus |= enRpsAsk;
					}

					dRetVal = (dPriceBid + dPriceAsk) / 2;
				}
				else
				{
					dRetVal = dPriceBid;
				}
			}
			else
			{
				if (m_bUseMidMarketForPnL != VARIANT_TRUE)
				{

					dPriceBid = ReplacePrice(dPriceTheo, dPriceLast, dPriceAsk, bUseTheoVola);
					nPriceStatus |= enRpsBid;
					dRetVal = dPriceBid;
				}
				else
				{
					if(m_bUseZeroBidRule)
						dRetVal = dPriceBid;
					else
					{
						if(bZeroAsk)
						{
							if(m_enBadOptBothPriceRule == enObbprUseTheo)
								dRetVal = dPriceTheo;
							else
								dRetVal = dPriceLast;
							nPriceStatus |= enRpsBoth;
						}
						else
						{
							dPriceBid = ReplacePrice(dPriceTheo, dPriceLast, dPriceAsk, bUseTheoVola);
							nPriceStatus |= enRpsBid;
							dRetVal = (dPriceBid + dPriceAsk) / 2;
						}
					}
				}
			}
		}


		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, dRetVal <= 3. ? .05 : .1, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileAtom::GetOptPriceAskForPnL(DOUBLE dPriceBid, DOUBLE dPriceAsk, DOUBLE dPriceLast,
	EtsPriceRoundingRuleEnum enPriceRound, VARIANT_BOOL bUseTheoVola, DOUBLE dPriceTheo,
	EtsReplacePriceStatusEnum* penPriceStatus, DOUBLE* pdRetVal)
{
	if (dPriceBid < 0) dPriceBid = 0;
	if (dPriceAsk < 0) dPriceAsk = 0;
	if (dPriceLast < 0) dPriceLast = 0;
	if (dPriceTheo < 0) dPriceTheo = 0;

	double dRetVal = 0;
	long nPriceStatus = enRpsNone;

	try
	{
		if (!m_bIsOptionProfile)
			EgLib::CComErrorWrapper::ThrowError(E_FAIL, _T("It is not option profile"));

		if (!pdRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		bool bZeroBid = DoubleEQZero(dPriceBid);
		bool bZeroAsk = DoubleEQZero(dPriceAsk);
		bool bZeroLast = DoubleEQZero(dPriceLast);

		if (bZeroBid && bZeroAsk)
		{
			if (/*!bZeroLast &&*/ (m_enBadOptBothPriceRule == enObbprUseLast || bUseTheoVola == VARIANT_FALSE) )
				dRetVal = dPriceLast;
			else
				dRetVal = dPriceTheo;

			nPriceStatus = enRpsAsk;
		}
//		else
//		{
//			if (bZeroAsk)
//			{
//				dPriceAsk = ReplacePrice(dPriceTheo, dPriceLast, dPriceBid, bUseTheoVola);
//				nPriceStatus = enRpsAsk;
//			}
//
//			if (m_bUseMidMarketForPnL != VARIANT_FALSE)
//			{
//				if (bZeroBid)
//				{
//					dPriceBid = ReplacePrice(dPriceTheo, dPriceLast, dPriceAsk, bUseTheoVola);
//					nPriceStatus |= enRpsBid;
//				}
//
//				dRetVal = (dPriceBid + dPriceAsk) / 2;
//			}
//			else
//			{
//				dRetVal = dPriceAsk;
//			}
//		}
//
		else
		{
			if(!bZeroAsk)
			{
				if(m_bUseMidMarketForPnL)
				{
					if(bZeroBid && !m_bUseZeroBidRule)
					{
						dPriceBid = ReplacePrice(dPriceTheo, dPriceLast, dPriceAsk, bUseTheoVola);
						nPriceStatus |= enRpsBid;
					}
					dRetVal = (dPriceBid + dPriceAsk) / 2;
				}
				else dRetVal = dPriceAsk;
			}
			else
			{
				dPriceAsk = ReplacePrice(dPriceTheo, dPriceLast, dPriceBid, bUseTheoVola);
				nPriceStatus = enRpsAsk;

				if(!m_bUseMidMarketForPnL)
					dRetVal = dPriceAsk;
				else
				{
					if(!m_bUseZeroBidRule)
					{
						if(bZeroBid)
						{
							if(m_enBadOptBothPriceRule == enObbprUseTheo)
								dRetVal = dPriceTheo;
							else
								dRetVal = dPriceLast;
							nPriceStatus |= enRpsBoth;
						}
						else
						dRetVal = (dPriceBid + dPriceAsk) / 2;
					}
					else
						dRetVal = (dPriceBid + dPriceAsk) / 2;
				}
			}
		}

		if (dRetVal > 0)
		{
			dRetVal = RoundPrice(dRetVal, dRetVal <= 3. ? .05 : .1, enPriceRound);
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileAtom, e.Error());
	}

	*pdRetVal = dRetVal;

	if (penPriceStatus)
		*penPriceStatus = (EtsReplacePriceStatusEnum)nPriceStatus;

	return S_OK;
}

