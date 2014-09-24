// MmQvOptAtom.cpp : Implementation of CMmQvOptAtom

#include "stdafx.h"
#include "MmQvOptAtom.h"


STDMETHODIMP CMmQvOptAtom::put_Qty(long nQty)
{
	HRESULT hr = S_OK;
	try
	{
		if(m_nQty != nQty)
		{
			m_nQty = nQty;
			if(m_pQuote)
			{
				IMmQvQuoteAtomPtr spQuote = m_pQuote->GetQuote(0L);
				CMmQvQuoteAtom* pQuote = dynamic_cast<CMmQvQuoteAtom*>(spQuote.GetInterfacePtr());
				if(pQuote!=NULL)
					pQuote->SetDirty();
			}
		}
	}
	catch (_com_error& e)
	{
		hr =  Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvOptAtom, e.Error());
	}
        catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvOptAtom, _hr);
	}
	return hr;
}

STDMETHODIMP CMmQvOptAtom::CalcMarketDepth(SAFEARRAY** BidMarketDepth, SAFEARRAY** AskMarketDepth/*, LONG* lBidCount, LONG* lAskCount, LONG* retDiffPrcCount*/)
{
	HRESULT hr = S_OK;
	map<DOUBLE, LONG> gmBid, gmAsk;
	map<DOUBLE, LONG>::iterator itGroup;

	try
	{
		if( !BidMarketDepth )return E_POINTER;
		if( *BidMarketDepth )
		{
			::SafeArrayDestroy(*BidMarketDepth);
			*BidMarketDepth = NULL;
		}

		if( !AskMarketDepth )return E_POINTER;
		if( *AskMarketDepth )
		{
			::SafeArrayDestroy(*AskMarketDepth);
			*AskMarketDepth = NULL;
		}

		if( m_pQuote != NULL )
		{

			CMmQvQuoteColl::EnumCollType::iterator itQuote    =  m_pQuote->m_coll.begin();
			CMmQvQuoteColl::EnumCollType::iterator itQuoteEnd =  m_pQuote->m_coll.end();

			MmQvMarketDepthAtom* pvBidData = NULL;
			MmQvMarketDepthAtom* pvAskData = NULL;

			long	lQuoteCount;

			//m_pQuote->get_Count(&lQuoteCount);
			lQuoteCount = static_cast<long>(m_pQuote->m_coll.size());

			for( ;itQuote!=itQuoteEnd; ++itQuote )
			{
				CMmQvQuoteAtom * pQuote = dynamic_cast<CMmQvQuoteAtom*>(itQuote->second);

				if(pQuote && (pQuote->GetExchangeID() != 0 || lQuoteCount== 1))
				{
					if((itGroup = gmBid.find(pQuote->m_dPriceBid)) != gmBid.end() && !gmBid.empty())
					{
						itGroup->second += (pQuote->m_nSizeBid != BAD_LONG_VALUE ? pQuote->m_nSizeBid : 0);
					}
					else
					{
						gmBid.insert(map<DOUBLE, LONG>::value_type(pQuote->m_dPriceBid,(pQuote->m_nSizeBid != BAD_LONG_VALUE ? pQuote->m_nSizeBid : 0)));
					}
					if((itGroup = gmAsk.find(pQuote->m_dPriceAsk)) != gmAsk.end() && !gmAsk.empty())
					{
						itGroup->second += (pQuote->m_nSizeAsk != BAD_LONG_VALUE ? pQuote->m_nSizeAsk : 0);
					}
					else
					{
						gmAsk.insert(map<DOUBLE, LONG>::value_type(pQuote->m_dPriceAsk,(pQuote->m_nSizeAsk != BAD_LONG_VALUE ? pQuote->m_nSizeAsk : 0)));
					}
				}
			}
			
			__MmQvMarketDepthAtom	marketDepthInit;
			marketDepthInit.Init();
			
			SAFEARRAYBOUND			saBound = { (long)gmBid.size(), 0};
			LPSAFEARRAY psaBid	= ::SafeArrayCreateEx( VT_RECORD, 1, &saBound, (PVOID) marketDepthInit.GetRecordInfo() );
			saBound.cElements	=	(long)gmAsk.size();
			LPSAFEARRAY psaAsk	= ::SafeArrayCreateEx( VT_RECORD, 1, &saBound, (PVOID) marketDepthInit.GetRecordInfo() );

			if ( psaBid==NULL || psaAsk==NULL ) return E_FAIL;
		
			if( SUCCEEDED( hr = ::SafeArrayAccessData(psaBid, (void**)&pvBidData) ) && static_cast<long>(gmBid.size())!=0)
			{
				LONG j = 0 ;
				for(itGroup = gmBid.begin();itGroup != gmBid.end(); ++itGroup)
				{
					pvBidData[j].Depth = itGroup->second; 	
					pvBidData[j].Price = itGroup->first;
					j++;
				}
				::SafeArrayUnaccessData(psaBid);
			}

			if( SUCCEEDED( hr = ::SafeArrayAccessData(psaAsk, (void**)&pvAskData) ) && gmAsk.size()!=0) 
			{
				LONG j = 0 ;
				for(itGroup = gmAsk.begin();itGroup != gmAsk.end();++itGroup)
				{
					pvAskData[j].Depth = itGroup->second; 	
					pvAskData[j].Price = itGroup->first;
					j++;
				}
				::SafeArrayUnaccessData(psaAsk);
			}
	
			*BidMarketDepth = psaBid;
			*AskMarketDepth = psaAsk;

// 			*retDiffPrcCount	=	(gmAsk.size() > gmBid.size() ? (long)gmAsk.size() : (long)gmBid.size());
// 
// 			*lBidCount = (long)gmBid.size(); 
// 			*lAskCount = (long)gmAsk.size();

			hr					=	S_OK;

		}
		else	
			hr	=	E_FAIL;
	}
	catch(...)
	{
		HRESULT _hr = HRESULT_FROM_WIN32(::GetLastError());
		if(SUCCEEDED(_hr)) _hr = E_FAIL;
		hr = Error((PTCHAR)"System Error", IID_IMmQvOptAtom, _hr);
	}
	return hr;
}