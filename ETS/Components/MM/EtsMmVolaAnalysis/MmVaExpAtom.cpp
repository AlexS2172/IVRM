// MmVaExpAtom.cpp : Implementation of CMmVaExpAtom

#include "stdafx.h"
#include "MmVaExpAtom.h"
#include <math.h>


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmVaExpAtom::FindAtmStrike(DOUBLE UnderlyingSpot)
{
	m_spAtmStrike = NULL;

	if(UnderlyingSpot > 0.)
	{
		ObjectLock lock(this);
		DOUBLE dDiff = 1E+308;
		try
		{
			IMmVaStrikeAtomPtr spStrike;
			_variant_t varItem;
			DOUBLE dStrike = 0.;
			HRESULT hr = S_OK;
			DOUBLE dCurDiff = 0.;

			CMmVaStrikeColl::CollType::iterator itr    = m_pStrikeColl->m_coll.begin();
			CMmVaStrikeColl::CollType::iterator itrEnd = m_pStrikeColl->m_coll.end();
			for(;itr!=itrEnd;++itr)
			{
				spStrike = itr->second;
				if(spStrike != NULL)
				{
					__CHECK_HRESULT(spStrike->get_Strike(&dStrike), _T("Fail to get strike value."));
					dCurDiff = fabs(UnderlyingSpot - dStrike);
					if(dCurDiff < dDiff)
					{
						dDiff = dCurDiff;
						m_spAtmStrike = spStrike;
					}
					else
						break;
				}
				spStrike = NULL;
			}
		}
		catch(const _com_error& e)
		{
			return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaExpAtom, e.Error());
		}
	}

	return S_OK;
}
