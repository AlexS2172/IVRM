#include "stdafx.h"

#include "MmRvRowData.h"
//#include "MmRvUndAtom.h"
//#include "MmRvAggregationData.h"
#include "MmRiskView.h"
#include "MmRvPosColl.h"
#include "MmRvUndColl.h"

#include <algorithm>
#include <boost/bind.hpp>

using namespace boost;

void accumulate_if_not_bad( double& sumValue, const double addValue){
	if ( !IsBadValue(addValue)) {
		if (IsBadValue(sumValue)) 
			sumValue = 0.;
		sumValue += addValue;
	}
}

void accumulate_if_not_bad( double& sumValue, const double addValue, const double K){
	if ( !IsBadValue(addValue)) {
		if (IsBadValue(sumValue)) 
			sumValue = 0.;
		sumValue += K * addValue;
	}
}

void accumulate_if_not_bad( LONG& sumValue, const LONG addValue, const double K){
	double tmpAdd = static_cast<double>(addValue);
	if ( !IsBadValue(addValue)) {
		if (IsBadValue(sumValue)) 
			sumValue = 0;
		sumValue += static_cast<LONG>(K * tmpAdd );
	}
}

void CMmRvAggData::AddAggregatedValues(const CMmRvRowData* pRow)
{
	if (pRow->m_pUnd->m_spHeadComponent || pRow->m_pUnd->m_bIsHead)
	{
		double	dCoeff = 1.0;
		if (pRow->m_pUnd->m_spHeadComponent || pRow->m_pUnd->m_bIsHead == VARIANT_TRUE) 
			dCoeff = pRow->m_pUnd->m_dCoeff;

		accumulate_if_not_bad( UndPos_,			pRow->m_pAgg->UndPos_,	dCoeff );
		//accumulate_if_not_bad( OptQty_,			pRow->m_pAgg->OptQty_,	1.0 );
		//accumulate_if_not_bad( FutQty_,			pRow->m_pAgg->FutQty_,	/*1.0*/dCoeff );

		accumulate_if_not_bad( OptDlt_,			pRow->m_pAgg->OptDlt_,	dCoeff );
		accumulate_if_not_bad( OptDlt$_,		pRow->m_pAgg->OptDlt$_, /*dCoeff*/ 1.0 );
		accumulate_if_not_bad( Gma1P_,			pRow->m_pAgg->Gma1P_,	dCoeff /** dCoeff*/ );
		accumulate_if_not_bad( Gma1$_,			pRow->m_pAgg->Gma1$_,	dCoeff * dCoeff );
		accumulate_if_not_bad( Vga$_,			pRow->m_pAgg->Vga$_,	1.0 );
		accumulate_if_not_bad( Tht$_,			pRow->m_pAgg->Tht$_,	1.0 );
		accumulate_if_not_bad( WtdVga$_,		pRow->m_pAgg->WtdVga$_, 1.0 );
		accumulate_if_not_bad( Rho$_,			pRow->m_pAgg->Rho$_,	1.0 );
		accumulate_if_not_bad( ThtDlt_,			pRow->m_pAgg->ThtDlt_,	dCoeff );
		accumulate_if_not_bad( ThtGma_,			pRow->m_pAgg->ThtGma_,	dCoeff /** dCoeff*/ );
		accumulate_if_not_bad( VgaDlt_,			pRow->m_pAgg->VgaDlt_,	dCoeff );
		accumulate_if_not_bad( VgaGma_,			pRow->m_pAgg->VgaGma_,	dCoeff /** dCoeff*/ );
		accumulate_if_not_bad( NetDlt_,			pRow->m_pAgg->NetDlt_,	/*1.0*/ dCoeff);
		accumulate_if_not_bad( NetDlt$_,		pRow->m_pAgg->NetDlt$_, 1.0 /*dCoeff*/);
		accumulate_if_not_bad( BetaWtdDelta$_,	pRow->m_pAgg->BetaWtdDelta$_, 1.0 );
		accumulate_if_not_bad( NetGma$_,		pRow->m_pAgg->NetGma$_, /*dCoeff * dCoeff*/ 1.0 );

		accumulate_if_not_bad( PnlEdge_,		pRow->m_pAgg->PnlEdge_, 1.0 );
		accumulate_if_not_bad( PnlMTM_,			pRow->m_pAgg->PnlMTM_,	1.0 );
		accumulate_if_not_bad( PnLTheo_,		pRow->m_pAgg->PnLTheo_, 1.0 );
		accumulate_if_not_bad( TmVal_,			pRow->m_pAgg->TmVal_,	1.0 );


		//-------------------------------------------------------------------------------//
		if(pRow->m_pUnd->m_spSynthGreeks != NULL)
		{
			IMmRvSynthGreeksAtomPtr spUndSynthGreeks;
			LONG nSynthUndID = 0L;
			DOUBLE dSelfValue	= BAD_DOUBLE_VALUE;
			DOUBLE dUndValue	= BAD_DOUBLE_VALUE;

			IUnknownPtr spUnk;
			_variant_t varItem;
			ULONG nFetched = 0L;
			HRESULT hr;

			_CHK(pRow->m_pUnd->m_spSynthGreeks->get__NewEnum(&spUnk));
			IEnumVARIANTPtr spUndSynthGreekEnum(spUnk);
			_CHK(spUndSynthGreekEnum->Reset());
			while((hr = spUndSynthGreekEnum->Next(1L, &varItem, &nFetched)) == S_OK)
			{
				ATLASSERT(varItem.vt == VT_DISPATCH);
				spUndSynthGreeks = varItem;
				if(spUndSynthGreeks != NULL)
				{
					_CHK(spUndSynthGreeks->get_NetDelta(&dUndValue));
					accumulate_if_not_bad(NetDlt_, dUndValue, dCoeff);

					_CHK(spUndSynthGreeks->get_DeltaInMoney(&dUndValue));
					accumulate_if_not_bad(NetDlt$_, dUndValue, 1.0 /*dCoeff*/);
					accumulate_if_not_bad(OptDlt$_, dUndValue, 1.0 /*dCoeff*/);

					_CHK(spUndSynthGreeks->get_DeltaInShares(&dUndValue));
					accumulate_if_not_bad(OptDlt_, dUndValue, dCoeff);

					_CHK(spUndSynthGreeks->get_GammaInShares(&dUndValue));
					accumulate_if_not_bad(Gma1$_, dUndValue, dCoeff * dCoeff);

					_CHK(spUndSynthGreeks->get_GammaInSharesPerc(&dUndValue));
					accumulate_if_not_bad(Gma1P_, dUndValue, dCoeff /** dCoeff*/);

					_CHK(spUndSynthGreeks->get_NetGamma(&dUndValue));
					accumulate_if_not_bad(NetGma$_, dUndValue, /*dCoeff * dCoeff*/ 1.0);

					_CHK(spUndSynthGreeks->get_BetaWtdDeltaInMoney(&dUndValue));
					accumulate_if_not_bad( BetaWtdDelta$_,	dUndValue, 1.0 );
				}
			}
		}
	}	
}
void CMmRvAggData::CalcNetDeltas(EtsContractTypeEnum underlyingContractType)
{

	for_each(	
					Aggregations_.begin(), 
					Aggregations_.end(), 
					bind(&CMmRvAggData::CalcNetDeltas, _1, underlyingContractType)
				);

	// calculate NetDeltas only for aggregations by underlying and future 
	if ( Type_ == enFutAgg) 
	{

		// for future aggregation use FutureQuantity or UnderlyingPosition
		// depending on Underlying Contract Type
		double Qty, optDlt;
		if (underlyingContractType ==	enCtFutUnd)
			Qty = IsBadValue(FutQty_) ? BAD_DOUBLE_VALUE : ( double )FutQty_;
		else
			Qty = IsBadValue(UndPos_) ? BAD_DOUBLE_VALUE : ( double )UndPos_;
		if (! (IsBadValue(Qty) && IsBadValue(OptDlt_) ) ) {
			if (IsBadValue(Qty))
				Qty = 0.;
			optDlt = IsBadValue(OptDlt_) ? 0. : OptDlt_;

			double dNetDlt_CSzMult = 1.0;
			if (underlyingContractType == enCtFutUnd && pFut_ && !pFut_->m_bMultOptDltEq 
				&& !IsBadValue(pFut_->m_nFutLotSize) && pFut_->m_nFutLotSize)
				dNetDlt_CSzMult = ( (double)pFut_->m_nFutLotSize );
			// for future aggregation NetDelta = Qty + ( Sum(OptionDelta) in aggregation )
			NetDlt_ = Qty * dNetDlt_CSzMult + optDlt;//optDlt has been already multiplied by 
														//Contract Size, if this is needed.

			// calculate NetDelta$
			NetDlt$_ = BAD_DOUBLE_VALUE;
			if (underlyingContractType ==	enCtFutUnd ) 
			{
				if (pFut_ && !IsBadValue(pFut_->m_pPrice->m_dActivePrice) && pFut_->m_pPrice->m_dActivePrice) 
					NetDlt$_ = (NetDlt_ / dNetDlt_CSzMult) * pFut_->m_pPrice->m_dActivePrice * pFut_->m_dKEq;
			}
			else 
				if (pUnd_ && !IsBadValue(pUnd_->m_pPrice->m_dActivePrice) && pUnd_->m_pPrice->m_dActivePrice) 
					NetDlt$_ = NetDlt_ * pUnd_->m_pPrice->m_dActivePrice;
		}
		else
			NetDlt_ = NetDlt$_ = BAD_DOUBLE_VALUE;
	}
	else if ( Type_ == enUndAgg)
	{
		double sumNetDeltas = BAD_DOUBLE_VALUE;

		// for underlying aggregation there are 2 cases:
		// 1. underlying aggregation for future underlying: NetDelta = Sum of NetDelta of aggregations residing this aggregation
		// 2. underlying aggregation for stock/index: NetDelta = OptionDelta of aggregation + Underlying position of this aggregation
		if (underlyingContractType == enCtFutUnd ) {
			for_each(	
				Aggregations_.begin(), 
				Aggregations_.end(), 
				bind( accumulate_if_not_bad, ref(NetDlt_), bind(&CMmRvAggData::NetDlt_, _1) )
				);
			for_each(	
				Aggregations_.begin(), 
				Aggregations_.end(), 
				bind( accumulate_if_not_bad, ref(NetDlt$_), bind(&CMmRvAggData::NetDlt$_, _1) )
				);
		}
		else{
			if ( !(IsBadValue(OptDlt_) && IsBadValue(UndPos_) ) ) {
				NetDlt_ = IsBadValue(OptDlt_) ? 0. : OptDlt_;
				if ( !IsBadValue(UndPos_) ) 
					NetDlt_ += ( static_cast<double>(UndPos_) );

				NetDlt$_ = BAD_DOUBLE_VALUE;
				if (pUnd_ && !IsBadValue(NetDlt_)) 
				{
					if (!IsBadValue(pUnd_->m_pPrice->m_dActivePrice) && pUnd_->m_pPrice->m_dActivePrice!=0.0) 
						NetDlt$_ = NetDlt_ * pUnd_->m_pPrice->m_dActivePrice;
				}
			}
		}
	}
	if(Type_ != enExpAgg)
	{
		DOUBLE dAUM = BAD_DOUBLE_VALUE;
		if(pUnd_)
			dAUM = pUnd_->GetNetExposureAUM();

		if( dAUM >BAD_DOUBLE_VALUE && dAUM != 0.0 && !IsBadValue(NetDlt$_))
		{

			if(NetExposure_ == BAD_DOUBLE_VALUE) NetExposure_ = 0.0;
			double dExposure = (NetDlt$_ / dAUM * 100);

			if(dExposure < 0.0)
			{
				if(NetExposureSort_ == BAD_DOUBLE_VALUE) NetExposureSort_ = 0;
				NetExposureSort_+= dExposure;
			}
			if(dExposure > 0.0)
			{
				if(NetExposureLong_ == BAD_DOUBLE_VALUE) NetExposureLong_ = 0;
				NetExposureLong_+= dExposure;
			}
			BadNetExposure_ = VARIANT_TRUE;
			NetExposure_ = (NetExposureSort_ == BAD_DOUBLE_VALUE?0:NetExposureSort_)+(NetExposureLong_ == BAD_DOUBLE_VALUE?0:NetExposureLong_);

		}
		else
			NetExposure_ = BAD_DOUBLE_VALUE;
	}
}

bool CMmRvAggData::IsAggregationRowVisible( const CMmRvAggData* pAgg) 
{
	if (pAgg->m_bIsHeadAggregation == VARIANT_TRUE)
		return true;

	if( find_if(pAgg->Positions_.begin(),pAgg->Positions_.end(), bind( &CMmRvPosAtom::IsVisible, _1) ) != pAgg->Positions_.end() )
		return true;
	CMmRvAggData::Aggregations::const_iterator itrCurAgg = pAgg->Aggregations_.begin();
	CMmRvAggData::Aggregations::const_iterator itrEndAgg = pAgg->Aggregations_.end();
	while(itrCurAgg != itrEndAgg ){
		if( IsAggregationRowVisible( itrCurAgg->get() ) )
			return true;
		++itrCurAgg;
	}
	return false;
}

bool CMmRvAggData::CreateRow(CMmRvUndAtom* pUndAtom, unsigned outlnLvl, std::vector<IMmRvRowDataPtr>& aggRows){

	IMmRvRowDataPtr spAggRowData;
	CComObject<CMmRvRowData>* pRow = NULL;

	if( IsAggregationRowVisible(this) && SUCCEEDED(CComObject<CMmRvRowData>::CreateInstance(&pRow))) {
		spAggRowData.Attach(pRow, TRUE);
		pRow->m_pUnd = pUndAtom;
		pRow->m_spUnd.Attach(pUndAtom, TRUE);
		pRow->m_Type = Type_ == enUndAgg ? RDT_UNDAGG : RDT_AGG;
		pRow->m_nLevel = outlnLvl;
		pRow->m_pAgg = MmRvAggDataPtr(this, null_deleter());
		aggRows.push_back(spAggRowData);
	}
	else
		return false;
	return true;
}

size_t CMmRvAggData::CreateRowsForPositions(CMmRvUndAtom* pUndAtom, long lExpiryFilter, unsigned outlineLevel, 
														const CRiskViewFieldSort *sortCmp, std::vector<IMmRvRowDataPtr>& positionsRows, 
														long& optPos,long& undPos, CComObject<CMmRvUndColl>* pUndColl ){
	Positions::iterator itrCurPos = Positions_.begin();
	Positions::iterator itrEndPos = Positions_.end();

	// underlyings aggregations in synthetic 
	CMmRiskView::CRowsData synthUndRows;

	if (pUndAtom->m_spSynthGreeks) 
	{
		IMmRvUndAtomPtr spUndAtom;
		CMmRvSynthGreeksColl* pSynthColl = dynamic_cast<CMmRvSynthGreeksColl*>(pUndAtom->m_spSynthGreeks.GetInterfacePtr());
		CMmRvSynthGreeksColl::EnumCollType::iterator itrSynth    =  pSynthColl->m_coll.begin();
		CMmRvSynthGreeksColl::EnumCollType::iterator itrSynthEnd =  pSynthColl->m_coll.end();
		for(; itrSynth != itrSynthEnd; ++itrSynth) {
			CMmRvSynthGreeksAtom* pSynthAtom = dynamic_cast<CMmRvSynthGreeksAtom*>(itrSynth->second);
			if(pSynthAtom)	{
				pUndColl->get_Item(pSynthAtom->m_nSynthUndID, &spUndAtom );
				if ( spUndAtom ) {
					CComObject<CMmRvRowData>* pSynthUndRow = NULL;
					if(SUCCEEDED(CComObject<CMmRvRowData>::CreateInstance(&pSynthUndRow))) 	{
						IMmRvRowDataPtr spSynthRowData;
						spSynthRowData.Attach(pSynthUndRow, TRUE);
						//pSynthUndRow->m_spPos.Attach(pPos.get(), TRUE);
						//pSynthUndRow->m_pPos = pPos.get();
						pSynthUndRow->m_Type = RDT_UNDAGG;
						pSynthUndRow->m_spUnd = spUndAtom; //pRowData->m_spUnd;
						pSynthUndRow->m_pUnd = static_cast<CMmRvUndAtom*>(spUndAtom.GetInterfacePtr());
						pSynthUndRow->m_pAgg = MmRvAggDataPtr(pSynthUndRow->m_pUnd, null_deleter());
						pSynthUndRow->m_spSynthGreeks.Attach(pSynthAtom, TRUE);
						pSynthUndRow->m_pSynthGreeks = pSynthAtom;
						pSynthUndRow->m_nLevel = outlineLevel + 1;
						synthUndRows.push_back( spSynthRowData );
					}
				}
			}
		}
	}

	sort(synthUndRows.begin(), synthUndRows.end(), *sortCmp );
	positionsRows.reserve(positionsRows.size() + synthUndRows.size() );
	copy(synthUndRows.begin(), synthUndRows.end(), back_inserter(positionsRows));

	itrCurPos = Positions_.begin();
	itrEndPos = Positions_.end();

	// positions
	for (;itrCurPos != itrEndPos; ++itrCurPos) {
		MmRvPosAtomPtr pPos = *itrCurPos;
		if (!pPos) continue;
		if	( !lExpiryFilter || ( ( enCtOption == pPos->m_enContractType ||  enCtFutOption == pPos->m_enContractType ) && static_cast<long>(pPos->m_dtExpiry) == lExpiryFilter ) ) {
			pPos->m_bVisible = VARIANT_TRUE;
			CComObject<CMmRvRowData>* pRow = NULL;
			if(SUCCEEDED(CComObject<CMmRvRowData>::CreateInstance(&pRow)))	{
				IMmRvRowDataPtr spRowData;
				spRowData.Attach(pRow, TRUE);
				pRow->m_spPos.Attach(pPos.get(), TRUE);
				pRow->m_pPos = pPos.get();
				pRow->m_Type = RDT_POS;
				pRow->m_spUnd.Attach(pUndAtom, TRUE);
				pRow->m_pUnd = pUndAtom;
				pRow->m_nLevel = outlineLevel + 1;

				if(pPos->m_enContractType == enCtOption || pPos->m_enContractType == enCtFutOption)
					++optPos;
				else
					++undPos;
				positionsRows.push_back( spRowData);
			}
		}
		else
			pPos->m_bVisible = VARIANT_FALSE;
	}


	return synthUndRows.size();
}

bool CMmRvAggData::CheckPositions(long lExpiryFilter)
{
	Positions::iterator itrCurPos = Positions_.begin();
	Positions::iterator itrEndPos = Positions_.end();
	if(!lExpiryFilter)
		return true;

	for (;itrCurPos != itrEndPos; ++itrCurPos) 
	{
		MmRvPosAtomPtr pPos = *itrCurPos;
		if (!pPos || pPos->IsVisible()) continue;
		switch(pPos->m_enContractType)
		{
		case enCtOption:
		case enCtFutOption:
			if(static_cast<long>(pPos->m_dtExpiry) == lExpiryFilter) 
				return true;
			break;
		default:
			break;
		}
	}
	return false;
}

void CMmRvAggData::CreateRows(CMmRvUndAtom* pUndAtom, long lExpiryFilter, unsigned outlineLevel, 
										const CRiskViewFieldSort *sortCmp, std::vector<IMmRvRowDataPtr>& Rows,
										long& nOptPositions,long& nUndPositions,CComObject<CMmRvUndColl>* pUndColl ){
	CMmRiskView::CRowsData aggregationRows;

	// first, create row for this aggregation
	if(!CreateRow(pUndAtom, outlineLevel, aggregationRows))
		return;
	
	long nOptPosSave = nOptPositions;
	long nUndPosSave = nUndPositions;
	// second, create rows for positions reside in this aggregation
	size_t synthUndCnt = CreateRowsForPositions(pUndAtom, lExpiryFilter, outlineLevel, sortCmp, aggregationRows, nOptPositions,nUndPositions, pUndColl  );

	// sort positions
	std::sort(aggregationRows.begin() + 1 + synthUndCnt, aggregationRows.end(), *sortCmp );

	// make rows for children aggregation
	CMmRiskView::CRowsData childrenAggRows;

	// expiry child aggregations first
	Aggregations::iterator itrCurAgg = Aggregations_.begin();
	Aggregations::iterator itrEndAgg = Aggregations_.end();
	for (;itrCurAgg != itrEndAgg; ++itrCurAgg) {
		MmRvAggDataPtr pAgg = *itrCurAgg;
		if (!pAgg || pAgg->Type_ != enExpAgg) continue;
		IMmRvRowDataPtr spAggRowData;
		CComObject<CMmRvRowData>* pRow = NULL;
		if(SUCCEEDED(CComObject<CMmRvRowData>::CreateInstance(&pRow))) {
			spAggRowData.Attach(pRow, TRUE);
			pRow->m_pUnd = pUndAtom;
			pRow->m_spUnd.Attach(pUndAtom, TRUE);
			pRow->m_Type = pAgg->Type_ == enUndAgg ? RDT_UNDAGG : RDT_AGG;
			pRow->m_nLevel = outlineLevel;
			pRow->m_pAgg = pAgg;
			childrenAggRows.push_back(spAggRowData);
		}
	}

	// sort expiry child aggregations
	std::sort(childrenAggRows.begin(), childrenAggRows.end(), *sortCmp );
	CMmRiskView::CRowsData::size_type expAggCount = childrenAggRows.size();

	// future aggregation next
	itrCurAgg = Aggregations_.begin();
	for (;itrCurAgg != itrEndAgg; ++itrCurAgg) {
		MmRvAggDataPtr pAgg = *itrCurAgg;
		if (!pAgg || pAgg->Type_ != enFutAgg) continue;
		IMmRvRowDataPtr spAggRowData;
		CComObject<CMmRvRowData>* pRow = NULL;
		if(SUCCEEDED(CComObject<CMmRvRowData>::CreateInstance(&pRow))) {
			spAggRowData.Attach(pRow, TRUE);
			pRow->m_pUnd = pUndAtom;
			pRow->m_spUnd.Attach(pUndAtom, TRUE);
			pRow->m_Type = pAgg->Type_ == enUndAgg ? RDT_UNDAGG : RDT_AGG;
			pRow->m_nLevel = outlineLevel;
			pRow->m_pAgg = pAgg;
			childrenAggRows.push_back(spAggRowData);
		}
	}
	std::sort(childrenAggRows.begin()+expAggCount, childrenAggRows.end(), *sortCmp );
	
	// create rows for data that reside in every child aggregation
	CMmRiskView::CRowsData::iterator itrCurChildAgg = childrenAggRows.begin();
	CMmRiskView::CRowsData::iterator itrEndChildAgg = childrenAggRows.end();
	for ( ;itrCurChildAgg != itrEndChildAgg; ++itrCurChildAgg) {
		CMmRvRowData *pRow = static_cast<CMmRvRowData *>( (*itrCurChildAgg).GetInterfacePtr() );
		MmRvAggDataPtr p(pRow->m_pAgg);
		if (!pRow || !p) continue;
		p->CreateRows(pUndAtom, lExpiryFilter, outlineLevel + 1, sortCmp, aggregationRows, nOptPositions, nUndPositions, pUndColl);
	}

	
	if(!CMmRvAggData::IsAggregationRowVisible(this))
		return;
	

	// add rows of this aggregation into final rows vector
	CMmRiskView::CRowsData::iterator itrCurRow = aggregationRows.begin();
	CMmRiskView::CRowsData::iterator itrEndRow = aggregationRows.end();
	Rows.reserve(Rows.size() + aggregationRows.size());
	for (; itrCurRow != itrEndRow; ++itrCurRow ) {
		IMmRvRowDataPtr spRow;
		spRow = (*itrCurRow);
		Rows.push_back(spRow);
		//if (Type_ != enExpAgg ) continue;
		CMmRvRowData *pRowData = dynamic_cast<CMmRvRowData*>((*itrCurRow).GetInterfacePtr() );
		if ( pRowData && pRowData->m_Type == RDT_POS && pRowData->m_pPos ) {
			CMmRvPosAtom* pPos = pRowData->m_pPos;
			if(VARIANT_TRUE == pPos->m_bIsSynthetic && pPos->m_spSynthGreeks!=NULL)	{
				CMmRvSynthGreeksColl* pSynthColl = dynamic_cast<CMmRvSynthGreeksColl*>(pPos->m_spSynthGreeks.GetInterfacePtr());
				ATLASSERT(pSynthColl);
				if(pSynthColl) {
					CMmRvSynthGreeksColl::EnumCollType::iterator itrSynth    =  pSynthColl->m_coll.begin();
					CMmRvSynthGreeksColl::EnumCollType::iterator itrSynthEnd =  pSynthColl->m_coll.end();
					for(; itrSynth != itrSynthEnd; ++itrSynth) {
						CMmRvSynthGreeksAtom* pSynthAtom = dynamic_cast<CMmRvSynthGreeksAtom*>(itrSynth->second);
						if(pSynthAtom)	{
							CComObject<CMmRvRowData>* pSynthRow = NULL;
							if(SUCCEEDED(CComObject<CMmRvRowData>::CreateInstance(&pSynthRow))) 	{
								IMmRvRowDataPtr spSynthRowData;
								spSynthRowData.Attach(pSynthRow, TRUE);
								pSynthRow->m_spPos.Attach(pPos, TRUE);
								pSynthRow->m_pPos = pPos;
								pSynthRow->m_Type = RDT_POS;
								pSynthRow->m_spUnd = pRowData->m_spUnd;
								pSynthRow->m_pUnd = pRowData->m_pUnd;
								pSynthRow->m_spSynthGreeks.Attach(pSynthAtom, TRUE);
								pSynthRow->m_pSynthGreeks = pSynthAtom;
								pSynthRow->m_nLevel = pRowData->m_nLevel+1;
								pSynthAtom->m_bVisible = VARIANT_TRUE;
								Rows.push_back( spSynthRowData );		// add row for synthetic position
							}
						}
					}
				}
			}
		}
	}
}
