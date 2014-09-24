// MmQvSpreadColl.cpp : Implementation of CMmQvSpreadColl

#include "stdafx.h"
#include "MmQvSpreadColl.h"
#include "MmQvSpreadAtom.h"
#include "MmQvSpreadData.h"


// CMmQvSpreadColl
IMmQvSpreadAtomPtr CMmQvSpreadColl::AddNew(_bstr_t bsKey,  CComObject<CMmQvSpreadAtom>** ppRetVal)
{
	CComObject<CMmQvSpreadAtom>* pValue = NULL;

	IMmQvSpreadAtomPtr spSpreadData; 
	_CHK(CComObject<CMmQvSpreadAtom>::CreateInstance(&pValue), _T("Fail to add Spread Data."));
	spSpreadData.Attach(pValue, TRUE);

	if(FAILED(IMmQvSpreadCollImpl::Add(bsKey, pValue)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add Spreead."));
	if(ppRetVal!=NULL)
		*ppRetVal = pValue;

	return spSpreadData;

}
/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvSpreadColl::Add(BSTR Key, IMmQvSpreadAtom* Value, IMmQvSpreadAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	_bstr_t bsKey(Key);
	try
	{
		if(m_coll.find(bsKey) != m_coll.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Spread Item with the same key is already exists."));


		if(!Value)
			*pRetVal = AddNew(bsKey).Detach();
		else
		{
			_CHK(IMmQvSpreadCollImpl::Add(bsKey, Value), _T("Fail to add Spread."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadColl, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmQvSpreadColl::Calculate(BYTE bQuoteBidColBuy)
{
	try
	{
		DOUBLE m_dSign = (bQuoteBidColBuy == 0 ? 1.0 : -1.0);

		_CMmQvSpreadData::Clear();
		CollType::iterator itr = m_coll.begin();
		for (;itr!=m_coll.end(); ++itr)
		{
			CMmQvSpreadAtom* pSpreadAtom = dynamic_cast<CMmQvSpreadAtom*>(itr->second);
			if(pSpreadAtom)
			{
				IMmQvSpreadDataPtr spSpreadData; 
				pSpreadAtom->get_SpreadData(&spSpreadData);
				CMmQvSpreadData* pSpreadData = dynamic_cast<CMmQvSpreadData*>(spSpreadData.GetInterfacePtr());

				if(pSpreadData->m_nQty > 0)
				{
					// Buy
					if(m_dPrice == BAD_DOUBLE_VALUE)m_dPrice = 0;

					if(pSpreadData->m_dAsk != BAD_DOUBLE_VALUE)
					{
						m_dPrice += m_dSign * pSpreadData->m_dAsk * pSpreadData->m_nQty; 
					}
					else
					{
						m_dPrice += m_dSign * pSpreadData->m_nQty * (pSpreadData->m_dTheoPrice != BAD_DOUBLE_VALUE ? pSpreadData->m_dTheoPrice : 0.0 );
					}
				}
				if(pSpreadData->m_nQty < 0)
				{
					// Sell
					if(m_dPrice == BAD_DOUBLE_VALUE)m_dPrice = 0;

					if(pSpreadData->m_dBid != BAD_DOUBLE_VALUE)
					{
						m_dPrice += m_dSign * pSpreadData->m_dBid * pSpreadData->m_nQty; 
					}
					else
					{
						m_dPrice += m_dSign * pSpreadData->m_nQty * (pSpreadData->m_dTheoPrice != BAD_DOUBLE_VALUE ? pSpreadData->m_dTheoPrice : 0.0 );	
					}
				}
				if(pSpreadAtom->m_spRowData!=NULL)
				{
					if(pSpreadData->m_dTheoPrice != BAD_DOUBLE_VALUE)
					{
						if(m_dTheoPrice == BAD_DOUBLE_VALUE)m_dTheoPrice = 0;
						m_dTheoPrice += m_dSign * pSpreadData->m_dTheoPrice * pSpreadData->m_nQty; 

					}
					if(pSpreadData->m_dDelta != BAD_DOUBLE_VALUE)
					{
						if(m_dDelta == BAD_DOUBLE_VALUE) m_dDelta = 0;
						m_dDelta += m_dSign * pSpreadData->m_dDelta * pSpreadData->m_nQty; 

					}
					if(pSpreadData->m_dGamma != BAD_DOUBLE_VALUE)
					{
						if(m_dGamma == BAD_DOUBLE_VALUE) m_dGamma = 0;
						m_dGamma += m_dSign * pSpreadData->m_dGamma * pSpreadData->m_nQty; 

					}
					if(pSpreadData->m_dVega != BAD_DOUBLE_VALUE)
					{
						if(m_dVega == BAD_DOUBLE_VALUE) m_dVega = 0;
						m_dVega += m_dSign * pSpreadData->m_dVega * pSpreadData->m_nQty; 

					}
					if(pSpreadData->m_dTheta != BAD_DOUBLE_VALUE)
					{
						if(m_dTheta == BAD_DOUBLE_VALUE) m_dTheta = 0;
						m_dTheta += m_dSign * pSpreadData->m_dTheta * pSpreadData->m_nQty; 
					}
					if(pSpreadData->m_dRho != BAD_DOUBLE_VALUE)
					{
						if(m_dRho == BAD_DOUBLE_VALUE) m_dRho = 0;
						m_dRho += m_dSign * pSpreadData->m_dRho * pSpreadData->m_nQty; 
					}
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadColl, e.Error());
	}
	return S_OK;
}
STDMETHODIMP CMmQvSpreadColl::UpdateUnderlying(LONG nQty, LONG nExchID, IMmQvUndAtom* pUndAtom)
{
	try
	{
		BSTR BSTRSymbol = NULL;
		pUndAtom->get_Symbol(&BSTRSymbol);

		_bstr_t bsSymbol;
		bsSymbol.Attach(BSTRSymbol);

		IMmQvSpreadAtomPtr spItem = GetSpreadItem(bsSymbol);
		if(spItem!=NULL)
		{
			// TODO: Fire notification about Erase
			Remove(bsSymbol);
			Fire_OnSpreadItemRemoved(spItem);
		}
		if(nQty!=0)
		{
			CComObject<CMmQvSpreadAtom>* pAtom = NULL;
			spItem = AddNew(bsSymbol, &pAtom);
			if(pAtom)
			{
				pAtom->m_spUndAtom		= pUndAtom;
				pAtom->m_lExchangeId	= nExchID;
				pAtom->m_enSpreadType	= nQty > 0 ? SPT_BUY : SPT_SELL;
				Fire_OnSpreadItemAdded(spItem);
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadColl, e.Error());
	}
	return S_OK;
}
STDMETHODIMP CMmQvSpreadColl::UpdateFuture(LONG nQty, LONG nExchID, IMmQvFutAtom* pFutAtom)
{
	try
	{
		BSTR BSTRSymbol = NULL;
		pFutAtom->get_Symbol(&BSTRSymbol);

		_bstr_t bsSymbol;
		bsSymbol.Attach(BSTRSymbol);

		IMmQvSpreadAtomPtr spItem = GetSpreadItem(bsSymbol);
		if(spItem!=NULL)
		{
			// TODO: Fire notification about Erase
			Remove(bsSymbol);
			Fire_OnSpreadItemRemoved(spItem);
		}
		if(nQty!=0)
		{
			CComObject<CMmQvSpreadAtom>* pAtom = NULL;
			spItem = AddNew(bsSymbol, &pAtom);
			if(pAtom)
			{
				pAtom->m_spFutAtom		= pFutAtom;
				pAtom->m_lExchangeId	= nExchID;
				pAtom->m_enSpreadType	= nQty > 0 ? SPT_BUY : SPT_SELL;
				Fire_OnSpreadItemAdded(spItem);
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadColl, e.Error());
	}
	return S_OK;
}
STDMETHODIMP CMmQvSpreadColl::UpdateOption( LONG nQty, EtsOptionTypeEnum enOptType, IMmQvRowData* pRowData)
{
	try
	{
		BSTR BSTRSymbol = NULL;
		IMmQvOptAtomPtr spOpt;
		pRowData->get_Opt(enOptType, &spOpt);
		if(spOpt)
		{
			spOpt->get_Symbol(&BSTRSymbol);
			_bstr_t bsSymbol;
			bsSymbol.Attach(BSTRSymbol);
			IMmQvSpreadAtomPtr spItem = GetSpreadItem(bsSymbol);

			if(spItem!=NULL)
			{
				Remove(bsSymbol);
				Fire_OnSpreadItemRemoved(spItem);
			}
			if( nQty!=0 )
			{
				CComObject<CMmQvSpreadAtom>* pAtom = NULL;
				spItem = AddNew(bsSymbol, &pAtom);
				if(pAtom)
				{
					pAtom->m_spRowData		= pRowData;
					pAtom->m_enOptionType	= enOptType;
					pAtom->m_enSpreadType	= nQty > 0 ? SPT_BUY : SPT_SELL;
					Fire_OnSpreadItemAdded(spItem);
				}
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadColl, e.Error());
	}
	return S_OK;
}

STDMETHODIMP CMmQvSpreadColl::Clear()
{
	try
	{
		_CMmQvSpreadData::Clear();
		CollType::iterator itr = m_coll.begin();
		while(itr!=m_coll.end())
		{
			IMmQvSpreadAtomPtr spAtom = itr->second;
			Remove(itr->first);
			Fire_OnSpreadItemRemoved(spAtom);
			itr = m_coll.begin();
		}		
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvSpreadColl, e.Error());
	}
	return S_OK;
}


