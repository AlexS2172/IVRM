// MmHsCalc.cpp : Implementation of CMmHsCalc

#include "stdafx.h"
#include "MmHsCalc.h"


// CMmHsCalc

STDMETHODIMP CMmHsCalc::CalcYs(EtsHedgeTypeEnum enHedgeType, DOUBLE dUndPriceTolerance, 
			EtsPriceRoundingRuleEnum enPriceRoundingRule)
{
	HRESULT	hr;
	IUnknownPtr	spUnk;		
	_variant_t	varItem;
	ULONG	nFetched = 0L;	
	DOUBLE	dSpotPrice = 0.;
	LONG	nID = 0L;
	DOUBLE	dHV = 0., dDelta = 0.;
	DOUBLE	dNetVega = 0., dIVIndex = 0., dVolOfVol = 0.;	
	IMmHsUndAtomPtr	spStockAtom;
	

	m_StocksYs.clear();
	m_InstrumentsYs.clear();

	try
	{
		__CHECK_HRESULT(m_spStocks->get__NewEnum(&spUnk), _T("Fail to get stocks collection"));
		
		IEnumVARIANTPtr spUndCollEnum(spUnk);
		__CHECK_HRESULT(spUndCollEnum->Reset(), _T("Fail to reset stocks collection"));
		while( (hr = spUndCollEnum->Next(1L, &varItem, &nFetched)) == S_OK )
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			spStockAtom = V_DISPATCH(&varItem);
			__CHECK_HRESULT3(spStockAtom->get_ID(&nID));
		
			if (enHedgeType == enEtsHedgeTypeDelta) //if Delta case
			{	
				DOUBLE	dSpotBid = 0., dSpotAsk = 0., dSpotLast = 0.;
				IEtsPriceProfileAtomPtr	spUndPriceProfile;
			
				__CHECK_HRESULT3(spStockAtom->get_HV(&dHV) );
				__CHECK_HRESULT3(spStockAtom->get_Delta(&dDelta) );
				__CHECK_HRESULT3(spStockAtom->get_PriceAsk(&dSpotAsk) );
				__CHECK_HRESULT3(spStockAtom->get_PriceBid(&dSpotBid) );
				__CHECK_HRESULT3(spStockAtom->get_PriceLast(&dSpotLast) );
				__CHECK_HRESULT3(spStockAtom->get_UndPriceProfile(&spUndPriceProfile) );

				dSpotPrice = spUndPriceProfile->GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, dUndPriceTolerance, enPriceRoundingRule, NULL, FALSE);

				if ( dDelta && dDelta!=BAD_DOUBLE_VALUE 
					&& dSpotPrice && dSpotPrice!=BAD_DOUBLE_VALUE 
					&& dHV && dHV!=BAD_DOUBLE_VALUE )
					m_StocksYs[nID] = dDelta * dSpotPrice * dHV;
			}
			else if (enHedgeType == enEtsHedgeTypeVega) //if Vega case
			{
				__CHECK_HRESULT3(spStockAtom->get_Vega(&dNetVega) );
				__CHECK_HRESULT3(spStockAtom->get_IV(&dIVIndex) );
				__CHECK_HRESULT3(spStockAtom->get_VolOfVol(&dVolOfVol) );
			
				if (dNetVega && dNetVega!=BAD_DOUBLE_VALUE 
					&& dIVIndex && dIVIndex!=BAD_DOUBLE_VALUE 
					&& dVolOfVol && dVolOfVol!=BAD_DOUBLE_VALUE )
					m_StocksYs[nID] = dNetVega * dIVIndex * dVolOfVol;
			}
		}	

		if (m_StocksYs.size() == 0)
			CComErrorWrapper::ThrowError( E_FAIL, _T("No symbols with non-zero data in portfolio") );
		

		spUnk = 0;
		spUndCollEnum = 0;
		varItem.Clear();

		__CHECK_HRESULT(m_spInstruments->get__NewEnum(&spUnk), _T("Fail to get instruments collection"));
		
		spUndCollEnum = spUnk;
		__CHECK_HRESULT(spUndCollEnum->Reset(), _T("Fail to reset instruments collection"));
		while( (hr = spUndCollEnum->Next(1L, &varItem, &nFetched)) == S_OK )
		{
			ATLASSERT(varItem.vt == VT_DISPATCH);
			IMmHsUndAtomPtr	spInstrumentAtom = V_DISPATCH(&varItem);
			VARIANT_BOOL	vbInstrumentEnabled;
			__CHECK_HRESULT3(spInstrumentAtom->get_ID(&nID) );
			__CHECK_HRESULT3(spInstrumentAtom->get_Enabled(&vbInstrumentEnabled) );

			if (vbInstrumentEnabled == VARIANT_TRUE)
			{
				EtsContractTypeEnum	enUndType;
				__CHECK_HRESULT3(spInstrumentAtom->get_UndType(&enUndType) );

				if (enHedgeType == enEtsHedgeTypeDelta) //if Delta case
				{
					if (enUndType == enCtIndex || enUndType == enCtStock)
					{
						DOUBLE	dSpotBid = 0., dSpotAsk = 0., dSpotLast = 0.;
						IEtsPriceProfileAtomPtr	spUndPriceProfile;
			
						__CHECK_HRESULT3(spInstrumentAtom->get_HV(&dHV));
						__CHECK_HRESULT3(spInstrumentAtom->get_PriceAsk(&dSpotAsk));
						__CHECK_HRESULT3(spInstrumentAtom->get_PriceBid(&dSpotBid));
						__CHECK_HRESULT3(spInstrumentAtom->get_PriceLast(&dSpotLast));
						__CHECK_HRESULT3(spInstrumentAtom->get_UndPriceProfile(&spUndPriceProfile));

						dSpotPrice = spUndPriceProfile->GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, dUndPriceTolerance, enPriceRoundingRule, NULL, FALSE);

						if (dSpotPrice && dSpotPrice!=BAD_DOUBLE_VALUE 
							&& dHV && dHV!=BAD_DOUBLE_VALUE )
							m_InstrumentsYs[nID] = dSpotPrice * dHV;
					}
				}
				else if (enHedgeType == enEtsHedgeTypeVega) //if Vega case
				{
					if (enUndType == enCtIndex || enUndType == enCtStock)
					{
						__CHECK_HRESULT3(spInstrumentAtom->get_IV(&dIVIndex) );
						__CHECK_HRESULT3(spInstrumentAtom->get_VolOfVol(&dVolOfVol) );			
				
						if (dIVIndex && dIVIndex!=BAD_DOUBLE_VALUE 
							&& dVolOfVol && dVolOfVol!=BAD_DOUBLE_VALUE )
							m_InstrumentsYs[nID] = dIVIndex * dVolOfVol;
					}
				}
			}
		}

		if (m_InstrumentsYs.size() == 0)
			CComErrorWrapper::ThrowError(E_FAIL, _T("No symbols with non-zero data in hedging instruments"));
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCalc, e.Error());
	}
	
	return S_OK;
}

STDMETHODIMP CMmHsCalc::CreateEqs()
{
	try
	{
		_CalcAs();
		_CalcBs();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCalc, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmHsCalc::DecomposeMatrix()
{
	CMmHsCalc::TMapInstrumentsYs::const_iterator _iterIYs1 = m_InstrumentsYs.begin();
	CMmHsCalc::TMapInstrumentsYs::const_iterator _iterIYs2;
	CMmHsCalc::TMapInstrumentsYs::const_iterator _iterIYs3;
	DOUBLE	dA = 0.;
	DOUBLE	dNewValue21 = 0.;
	DOUBLE	dNewValue23 = 0.;
	LONG	nID1 = 0L;
	LONG	nID2 = 0L;
	LONG	nID3 = 0L;
	DOUBLE	dY1 = 0.;
	CMmHsKey	Key1;
	CMmHsKey	Key2;	

	try
	{
		while (_iterIYs1 != m_InstrumentsYs.end() ) 
		{
			nID1 = _iterIYs1->first;
			dY1 = _iterIYs1->second;

			Key1.Set(nID1, nID1);
			dA = m_InstrumentsAs.Get(Key1);
			if (!_finite(dA) || DoubleEQZero(dA) )
				CComErrorWrapper::ThrowError(E_FAIL, _T("Correlation values for hedging instruments are incorrect - try to reload"));

			_iterIYs2 = _iterIYs1;
			_iterIYs2++;
			while(_iterIYs2 != m_InstrumentsYs.end())
			{
				nID2 = _iterIYs2->first;

				//a(i,k)=a(i,k)/a(k,k)
				Key1.Set(nID2, nID1);
				dNewValue21 = m_InstrumentsAs.Get(Key1) /dA;
				m_InstrumentsAs.Set(Key1, dNewValue21);

				_iterIYs3 = _iterIYs1;
				_iterIYs3++;
				while(_iterIYs3 != m_InstrumentsYs.end())
				{
					nID3 = _iterIYs3->first;

					//a(i,j)=a(i,j)-a(i,k)*a(k,j)
					Key1.Set(nID2, nID3);
					Key2.Set(nID1, nID3);
					dNewValue23 = m_InstrumentsAs.Get(Key1) - dNewValue21 * m_InstrumentsAs.Get(Key2);
					m_InstrumentsAs.Set(Key1, dNewValue23);
					_iterIYs3++;
				}
				_iterIYs2++;
			}
			_iterIYs1++;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCalc, e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmHsCalc::SolveEqs(EtsHedgeTypeEnum enHedgeType)
{
	CMmHsCalc::TMapInstrumentsBs::const_iterator	_iterIBs1 = m_InstrumentsBs.begin();
	CMmHsCalc::TMapInstrumentsBs::const_iterator	_iterIBs2;	
	DOUBLE	dA = 0.;
	LONG	nID1 = 0L;
	LONG	nID2 = 0L;	
	CMmHsKey	Key;

	m_Results.clear();	

	try
	{
		m_Results[_iterIBs1->first] = _iterIBs1->second;	
		_iterIBs1++;

		while(_iterIBs1 != m_InstrumentsBs.end())
		{
			nID1 = _iterIBs1->first;

			m_Results[nID1] = _iterIBs1->second;
			_iterIBs2 = m_InstrumentsBs.begin();
			while(_iterIBs2 != _iterIBs1)
			{
				nID2 = _iterIBs2->first;

				Key.Set(nID1, nID2);
				m_Results[_iterIBs1->first] = m_Results.find(nID1)->second -
					m_InstrumentsAs.Get(Key)* m_Results.find(nID2)->second;
				_iterIBs2++;
			}
			_iterIBs1++;
		}

		ATLASSERT(m_Results.size()==m_InstrumentsYs.size());

		CMmHsCalc::TMapOfResults::iterator			_iterResults1 = m_Results.end();
		CMmHsCalc::TMapOfResults::const_iterator	_iterResults2;

		_iterResults1--;
		nID1 = _iterResults1->first;

		Key.Set(nID1, nID1);
		dA = m_InstrumentsAs.Get(Key);
		if (!_finite(dA) || DoubleEQZero(dA) )
			CComErrorWrapper::ThrowError(E_FAIL, _T("Correlation values for hedging instruments are incorrect - try to reload"));

		_iterResults1->second = _iterResults1->second /dA;

		while (_iterResults1 != m_Results.begin() )
		{	
			_iterResults2 = _iterResults1;
			_iterResults1--;
			nID1 = _iterResults1->first;

			while (_iterResults2 != m_Results.end() )
			{
				nID2 = _iterResults2->first;

				Key.Set(nID1, nID2);
				_iterResults1->second = _iterResults1->second - m_InstrumentsAs.Get(Key)*_iterResults2->second;
				_iterResults2++;
			}

			Key.Set(nID1, nID1);
			dA = m_InstrumentsAs.Get(Key);
			if (!_finite(dA) || DoubleEQZero(dA) )
				CComErrorWrapper::ThrowError(E_FAIL, _T("Correlation values for hedging instruments are incorrect - try to reload"));

			_iterResults1->second = _iterResults1->second /dA;
		}

		_iterResults1 = m_Results.begin();
//		HRESULT	hr;
		IMmHsUndAtomPtr spInstrumentAtom;
		DOUBLE dContracts = 0.;		
		DOUBLE dEquivVega = 0.;

		while(_iterResults1 != m_Results.end() )
		{
			__CHECK_HRESULT3(m_spInstruments->get_Item(_iterResults1->first, &spInstrumentAtom) );

			if (enHedgeType == enEtsHedgeTypeDelta)
			{	
				dContracts = _iterResults1->second;
				__CHECK_HRESULT3(spInstrumentAtom->put_Delta(dContracts));
			}
			else if (enHedgeType == enEtsHedgeTypeVega)
			{
				dEquivVega = _iterResults1->second;
				__CHECK_HRESULT3(spInstrumentAtom->put_Vega(dEquivVega) );
			}

			spInstrumentAtom = 0;
			_iterResults1++;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCalc, e.Error());
	}

	return S_OK;
}

HRESULT	CMmHsCalc::_GetStockNameById(LONG nId, CComBSTR& bstrName)
{
	try
	{
//		HRESULT			hr;
		IMmHsUndAtomPtr	spStock;
		CComBSTR		bstrSymbol;

		__CHECK_HRESULT3(m_spStocks->get_Item(nId, &spStock));
		if (spStock==0)
			_com_issue_error(E_FAIL);

		__CHECK_HRESULT3(spStock->get_Symbol(&bstrSymbol));
		if (!bstrSymbol.Length())
			_com_issue_error(E_FAIL);

		bstrName = bstrSymbol;
	}
	catch(const _com_error& e)
	{
		return e.Error();
	}

	return S_OK;
}

HRESULT	CMmHsCalc::_GetInstrumentNameById(LONG nId, CComBSTR& bstrName)
{
	try
	{
		//HRESULT			hr;
		IMmHsUndAtomPtr	spInstrument;
		CComBSTR		bstrSymbol;

		__CHECK_HRESULT3(m_spInstruments->get_Item(nId, &spInstrument));
		if (spInstrument==0)
			_com_issue_error(E_FAIL);

		__CHECK_HRESULT3(spInstrument->get_Symbol(&bstrSymbol));
		if (!bstrSymbol.Length())
			_com_issue_error(E_FAIL);

		bstrName = bstrSymbol;
	}
	catch(const _com_error& e)
	{
		return e.Error();
	}

	return S_OK;
}

void CMmHsCalc::_CalcPortfolioVariations(DOUBLE* pdVar, DOUBLE* pdVarHedged) throw(const _com_error&)
{	
	if (!pdVar || !pdVarHedged)
		CComErrorWrapper::ThrowError(E_FAIL, _T("Invalid argument"));

	//HRESULT	hr;
	DOUBLE	dPortfolioVariation = 0.;
	//TCHAR	pErrDescr[256] = {0};

	CMmHsCalc::TMapStocksYs::const_iterator _iterSYs1 = m_StocksYs.begin();
	CMmHsCalc::TMapStocksYs::const_iterator _iterSYs2;
	while(_iterSYs1 != m_StocksYs.end() )
	{
		DOUBLE dSum = 0.;
		_iterSYs2 = _iterSYs1; 
		_iterSYs2++;
		while(_iterSYs2 != m_StocksYs.end() )
		{
			DOUBLE	dCorr = 0.;
			__CHECK_HRESULT3(m_spCorrelations->get_Item(_iterSYs1->first, _iterSYs2->first, &dCorr) );

			if (dCorr == BAD_DOUBLE_VALUE)
			{	
				CComBSTR		bstrStockName1;
				CComBSTR		bstrStockName2;

				CComBSTR bstrError(L"Invalid correlation value");
				if (_GetStockNameById(_iterSYs1->first,bstrStockName1)==S_OK && _GetStockNameById(_iterSYs2->first,bstrStockName2)==S_OK)				
				{
					bstrError.Append(L" (");
					bstrError.Append(bstrStockName1);
					bstrError.Append(L":");
					bstrError.Append(bstrStockName2);
					bstrError.Append(L")");
				}				
				
				USES_CONVERSION;
				CComErrorWrapper::ThrowError(E_FAIL, W2T(bstrError));
			}

			dSum += dCorr * _iterSYs2->second;
			_iterSYs2++;
		}

		dPortfolioVariation += _iterSYs1->second * (_iterSYs1->second + dSum * 2.);
		_iterSYs1++;
	}	

	DOUBLE dSum = 0.;
	CMmHsCalc::TMapInstrumentsBs::const_iterator _iterIBs = m_InstrumentsBs.begin();
	CMmHsCalc::TMapOfResults::const_iterator _iterResults;
	while (_iterIBs != m_InstrumentsBs.end() )
	{
		_iterResults = m_Results.find(_iterIBs->first);
		ATLASSERT(_iterResults!=m_Results.end() );
        dSum += _iterIBs->second * _iterResults->second;
		_iterIBs++;
	}

	*pdVar = dPortfolioVariation;
	*pdVarHedged = dPortfolioVariation - dSum;
}

void CMmHsCalc::_CalcAs() throw(const _com_error&)
{
//	HRESULT	hr;

	CMmHsCalc::TMapInstrumentsYs::const_iterator _iterIYs1 = m_InstrumentsYs.begin();
	CMmHsCalc::TMapInstrumentsYs::const_iterator _iterIYs2;

	DOUBLE	dCorr = 0.;
	DOUBLE	dA = 0.;
	DOUBLE	dY1 = 0.;
	LONG	nID1 = 0L;
	LONG	nID2 = 0L;
	CMmHsKey	Key;

	m_InstrumentsAs.Clear();
	
	while (_iterIYs1 != m_InstrumentsYs.end() )
	{
		nID1 = _iterIYs1->first;
		dY1 = _iterIYs1->second;
		_iterIYs2 = m_InstrumentsYs.begin();		
		while (_iterIYs2 != m_InstrumentsYs.end() )
		{
			nID2 = _iterIYs2->first;
			__CHECK_HRESULT3(m_spCorrelations->get_Item(nID1, nID2, &dCorr) );
			
			if (dCorr == BAD_DOUBLE_VALUE)
			{
				CComBSTR		bstrInstrumentName1;
				CComBSTR		bstrInstrumentName2;

				CComBSTR bstrError(L"Invalid correlation value");
				if (_GetInstrumentNameById(nID1,bstrInstrumentName1)==S_OK && _GetInstrumentNameById(nID2,bstrInstrumentName2)==S_OK)
				{
					bstrError.Append(L" (");
					bstrError.Append(bstrInstrumentName1);
					bstrError.Append(L":");
					bstrError.Append(bstrInstrumentName2);
					bstrError.Append(L")");
				}				
				
				USES_CONVERSION;
				CComErrorWrapper::ThrowError(E_FAIL, W2T(bstrError));
			}

			dA = dCorr * dY1 * _iterIYs2->second;
			if (!_finite(dA) || DoubleEQZero(dA) )
			{
				CComErrorWrapper::ThrowError(E_FAIL, _T("Correlation values for hedging instruments are incorrect - try to reload"));
			}

			Key.Set(nID1, nID2);
			m_InstrumentsAs.Add(Key, dA);
			_iterIYs2++;
		}
		_iterIYs1++;
	}
}

void CMmHsCalc::_CalcBs() throw(const _com_error&)
{	
	//HRESULT	hr;

	m_InstrumentsBs.clear();

	// calculating of Ks
	CMmHsCalc::TMapInstrumentsKs InstrumentsKs;
	CMmHsCalc::TMapStocksYs::const_iterator _iterSYs;
	CMmHsCalc::TMapInstrumentsYs::const_iterator _iterIYs = m_InstrumentsYs.begin(); 

	while (_iterIYs != m_InstrumentsYs.end() )
	{
		DOUBLE dSum = 0.;

		_iterSYs = m_StocksYs.begin();
		while (_iterSYs != m_StocksYs.end() )
		{	
			DOUBLE dCorr = 0.;
			__CHECK_HRESULT3(m_spCorrelations->get_Item(_iterSYs->first, _iterIYs->first, &dCorr) );

			if (dCorr==BAD_DOUBLE_VALUE)
			{
				CComBSTR		bstrStockName;
				CComBSTR		bstrInstrumentName;

				CComBSTR bstrError(L"Invalid correlation value");
				if (_GetStockNameById(_iterSYs->first,bstrStockName)==S_OK && _GetInstrumentNameById(_iterIYs->first,bstrInstrumentName)==S_OK)
				{
					bstrError.Append(L" (");
					bstrError.Append(bstrStockName);
					bstrError.Append(L":");
					bstrError.Append(bstrInstrumentName);
					bstrError.Append(L")");
				}				
				
				USES_CONVERSION;
				CComErrorWrapper::ThrowError(E_FAIL, W2T(bstrError));
			}

			dSum += dCorr * _iterSYs->second;
			_iterSYs++;		
		}
        
		InstrumentsKs[_iterIYs->first] = dSum;
		_iterIYs++;
	}

	ATLASSERT(InstrumentsKs.size() == m_InstrumentsYs.size() );

	// calculating of Bs
	CMmHsCalc::TMapInstrumentsKs::const_iterator _iterIKs;
	
	_iterIYs = m_InstrumentsYs.begin(); 
	while(_iterIYs != m_InstrumentsYs.end() )
	{
        _iterIKs = InstrumentsKs.find(_iterIYs->first);
		ATLASSERT(_iterIKs != InstrumentsKs.end());
		
		DOUBLE dB = - _iterIKs->second * _iterIYs->second;
		m_InstrumentsBs[_iterIYs->first] = dB;
		_iterIYs++;
	}

	ATLASSERT(m_InstrumentsBs.size()==m_InstrumentsYs.size());
}

STDMETHODIMP CMmHsCalc::CalcPositions(DOUBLE dUndPriceTolerance, 
						EtsPriceRoundingRuleEnum enPriceRoundingRule)
{
	CMmHsCalc::TMapInstrumentsDeltas::const_iterator _iterIDeltas = m_Results.begin();
	
	IMmHsUndAtomPtr spInstrumentAtom;
	
	//	..ÐÊÓÛÃÄÅ	ðêæ
	//HRESULT	hr;
	DOUBLE	dSpotBid = 0., dSpotAsk = 0., dSpotLast = 0.;
	DOUBLE	dSpotPrice = 0.;
	DOUBLE	dPos = 0.;	

	IEtsPriceProfileAtomPtr	spUndPriceProfile;

	try
	{
		while(_iterIDeltas != m_Results.end() )
		{	
			__CHECK_HRESULT3(m_spInstruments->get_Item(_iterIDeltas->first, &spInstrumentAtom));
			__CHECK_HRESULT3(spInstrumentAtom->get_PriceAsk(&dSpotAsk));
			__CHECK_HRESULT3(spInstrumentAtom->get_PriceBid(&dSpotBid));
			__CHECK_HRESULT3(spInstrumentAtom->get_PriceLast(&dSpotLast));
			__CHECK_HRESULT3(spInstrumentAtom->get_UndPriceProfile(&spUndPriceProfile));

			dSpotPrice = spUndPriceProfile->GetUndPriceMid(dSpotBid, dSpotAsk, dSpotLast, dUndPriceTolerance, enPriceRoundingRule, NULL, FALSE);			
			
			dPos = dSpotPrice * _iterIDeltas->second;
			__CHECK_HRESULT3(spInstrumentAtom->put_Pos(dPos));
		
			spInstrumentAtom = 0;
			_iterIDeltas++;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCalc, e.Error());
	}

	return S_OK;	
}

STDMETHODIMP CMmHsCalc::CalcGain(DOUBLE* dblGain)
{
	try
	{	
		if (!dblGain)
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		*dblGain = BAD_DOUBLE_VALUE;

		DOUBLE dPortVariation = 0.;
		DOUBLE dPortVariationHedged = 0.;
		_CalcPortfolioVariations(&dPortVariation, &dPortVariationHedged);
		
		if (dPortVariation < 0. || DoubleEQZero(dPortVariation))
			CComErrorWrapper::ThrowError(E_FAIL, _T("Zero portfolio variation. Correlation values are incorrect - try to reload"));

		if (dPortVariationHedged > dPortVariation)
			CComErrorWrapper::ThrowError(E_FAIL, _T("Correlation values are incorrect - try to reload"));

		if (dPortVariationHedged < 0.)
			dPortVariationHedged = 0.;

		DOUBLE dGain = 1.0 - sqrt(dPortVariationHedged/dPortVariation);
		if (_finite(dGain))
			*dblGain = dGain;
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmHsCalc, e.Error());
	}
    
	return S_OK;
}

