// MmQvStrikeColl.cpp : Implementation of CMmQvStrikeColl

#include "stdafx.h"
#include "MmQvStrikeColl.h"
#include "MmQvOptRootAtom.h"
#include "MmQvUndAtom.h"

IMmQvStrikeAtomPtr CMmQvStrikeColl::GetStrike(DOUBLE dStrike)
{
	IMmQvStrikeAtomPtr spStrike;
	CollType::iterator itrStrike = m_coll.find(dStrike);
	if(itrStrike != m_coll.end())
		spStrike = itrStrike->second;

	return spStrike;
}

IMmQvStrikeAtomPtr CMmQvStrikeColl::FindATMStrike(double dSpotPrice)
{
	IMmQvStrikeAtomPtr spStrike;
	if(!m_coll.empty())
	{
		CollType::iterator itr = m_coll.lower_bound(dSpotPrice);
		if(itr == m_coll.end())
			spStrike = m_coll.rbegin()->second;
		else 
			if(itr == m_coll.begin())
				spStrike = itr->second;
			else
			{
				CollType::iterator greaterIt =   itr;
				CollType::iterator lowerIt   = --itr;
				double dgreaterDiff = fabs(dSpotPrice - greaterIt->first);
				double lowerDiff    = fabs(dSpotPrice - lowerIt->first);
				if(dgreaterDiff > lowerDiff)
					spStrike = lowerIt->second;
				else
					spStrike = greaterIt->second;
			}
	}

	return spStrike;
}

IMmQvStrikeAtomPtr CMmQvStrikeColl::AddNew(DOUBLE dStrike, CComObject<CMmQvStrikeAtom>** ppStrike)
{
	IMmQvStrikeAtomPtr spStrike;
	CComObject<CMmQvStrikeAtom>* pStrike = NULL;

	_CHK(CComObject<CMmQvStrikeAtom>::CreateInstance(&pStrike), _T("Fail to add strike."));
	
	spStrike.Attach(pStrike, TRUE);
	if(FAILED(IMmQvStrikeCollImpl::Add(dStrike, pStrike)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add strike."));

	pStrike->m_dStrike  = dStrike;
	pStrike->m_bVisible = VARIANT_TRUE;

	if(ppStrike)
		*ppStrike = pStrike;

	return spStrike;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvStrikeColl::Add(DOUBLE Key, IMmQvStrikeAtom* Value, IMmQvStrikeAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Strike with the same key is already exists."));


		if(!Value)
			*pRetVal = AddNew(Key).Detach();
		else
		{
			_CHK(IMmQvStrikeCollImpl::Add(Key, Value), _T("Fail to add strike."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvStrikeColl, e.Error());
	}

	return S_OK;
}
