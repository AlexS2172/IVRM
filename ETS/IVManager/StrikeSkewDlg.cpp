// StrikeSkewDlg.cpp: implementation of the CStrikeSkewDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IVAnalyzer.h"
#include "StrikeSkewDlg.h"

#include <WinUser.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CStrikeSkewDlg, CChartDlg)
	//{{AFX_MSG_MAP(CStrikeSkewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CStrikeSkewDlg::CStrikeSkewDlg(DATE dtMonth): 
	m_dtMonth (dtMonth)
#ifdef _VOLA_MANAGER
	,m_nUpdateMark(0)
#endif//_VOLA_MANAGER
{
}


void CStrikeSkewDlg::InitializeChart()
{
#ifdef _VOLA_MANAGER
	EnableVolaManagement( false, false );
	SetIsDirty( false );
#endif //_VOLA_MANAGER
}


void CStrikeSkewDlg::UpdateChart( bool bReinitialize )
{
	CString strStock;
		
	const sUndPrice& priceData = g_DataProcessor.GetUndPrice();

	if(!fEQZero(priceData.m_fAsk) || !fEQZero(priceData.m_fBid))
	{
		if(!fEQZero(priceData.m_fLast))
		{
			strStock.Format (_T(" - [%s %.2f/%.2f %.2f]"), (LPCTSTR) g_DataProcessor.GetContractSymbol(), priceData.m_fBid, priceData.m_fAsk, priceData.m_fLast);
		}
		else
			strStock.Format (_T(" - [%s %.2f/%.2f]"), (LPCTSTR) g_DataProcessor.GetContractSymbol(), priceData.m_fBid, priceData.m_fAsk);
	}
	else
		if(!fEQZero(priceData.m_fLast))
		{
			strStock.Format (_T(" - [%s %.2f]"), (LPCTSTR) g_DataProcessor.GetContractSymbol(), priceData.m_fLast);		
		}
		else
			strStock.Format (_T(" - [%s]"), (LPCTSTR) g_DataProcessor.GetContractSymbol());		

	
	CString strTitle = m_strTitle;
	strTitle += _T(' ');
	strTitle += GetCurveModeTitle() + _T(' ');
	strTitle += strStock;

	SetTitle(strTitle);

	bool bVolaInPercent = true; //m_rMgr.GetShowVolatilityInPercents();
	m_lVolaMultiplier = bVolaInPercent ? 100L : 1L;

	SetChartIsBatched(TRUE);
	m_ctlChart.GetLegend().SetIsShowing(FALSE);

	m_AxisX.GetLabelFormat().SetFormat(_T("0;-0;[<5]0.0#;-0.0#"));
	m_AxisX.GetValueLabels().RemoveAll();
	
	ClearSeries( m_lMovingPtIdx != 0 );
	m_vecTypes.clear();

	CString strInfo;
	int nCntVola = 0;
	double dMinX = DBL_MAX, dMaxX = -DBL_MAX;
	double dMinY = DBL_MAX, dMaxY = -DBL_MAX;
	
	typedef std::map <double, const CVolaBandsData*> ivbandsref_data_map;
	typedef std::map <double, CVolaBandsData> ivbands_data_map;  

	double dStartStrike = 0.;
	double dEndStrike   = 0.;
	
	bool bRange = g_DataProcessor.GetRangeInfo( dStartStrike, dEndStrike );

	try
	{
		strikes_map strikesData;
		CDataProcessor::CLockVola lv;
		
		nCntVola = g_DataProcessor.GetOptionsDataByEpiration( m_dtMonth, m_dtExp, strikesData, true );

		if( nCntVola > 0 )
		{
			m_cGroup2.GetData().SetNumPoints(g_lVola, nCntVola);
			m_vecDates.reserve (nCntVola);
			m_vecPrices.reserve (nCntVola);
			m_vecSymbols.reserve(nCntVola);
			m_vecTypes.reserve (nCntVola);

			int n = 1;
			for (strikes_map::iterator it = strikesData.begin(); it != strikesData.end(); it++  )
			{
				double dStrike = it->first;
				
				const COptionData * pOpt = it->second;
				double dV = pOpt->m_dVola;

				if ( IsValidVola ( dV ) )
				{
					m_cGroup2.GetData().SetY(g_lVola, n, dV * m_lVolaMultiplier);
					m_cGroup2.GetData().SetX(g_lVola, n, dStrike);

					if( dStrike < dMinX ) dMinX = dStrike;
					if( dStrike > dMaxX ) dMaxX = dStrike;

					if( dV < dMinY ) dMinY = dV;
					if( dV > dMaxY ) dMaxY = dV;
					
					m_vecSymbols.push_back(pOpt->m_strName);

					m_vecDates.push_back (pOpt->m_dtPriceDate);
					m_vecTypes.push_back (pOpt->m_bIsCall);
					
					m_vecPrices.push_back (pOpt->m_dPrice);
	
					n++;
				}
			}
			m_cGroup2.GetData().SetSeriesDisplay(g_lVola, oc2dDisplayShow);
		}
		else
		{
			if ( strInfo.GetLength() > 0)
				strInfo += _T("\n");

			strInfo += _T("Not enough implied volatility data to display!");

			m_cGroup2.GetData().SetSeriesDisplay(g_lVola, oc2dDisplayHide);
		}
	}
	catch( const _com_error& e )
	{
		strInfo += _T("Error drawing implied volatility data. ");
		strInfo += (LPCSTR)e.Description();
	}
	catch( ... )
	{
		strInfo += _T("Error drawing implied volatility data. ");
	}

// Custom vola stuff goes here
#ifdef _VOLA_MANAGER
	try
	{
		// Get surfaces references
		VME::IVolatilityManagementPtr spVM = g_DataProcessor.GetVolaManager();
		long nSurfaceID = spVM->ActiveSurfaceID[_bstr_t(g_DataProcessor.GetContractSymbol())];

		if( m_nSurfaceID != nSurfaceID )
		{
			m_nSurfaceID = nSurfaceID;

			VME::IVMSymbolVolatilitySurfacePtr spSurface = spVM->Surface[m_nSurfaceID][OTM];
			if( spSurface->PriceOverride != VARIANT_TRUE )
				spSurface->UnderlinePrice = priceData.GetMidMarket();
			m_spSkew  = spSurface->StrikeSkew[m_dtMonth];
			spSurface = NULL;
		}			
		spVM = NULL;

		double dStrike, dVola;

		// Show skews
		if( !m_lMovingPtIdx || bReinitialize )
		{
			// Fill points data buffers
			long nSkewUpdateMark = m_spSkew->UpdateMark;
			if( m_nUpdateMark != nSkewUpdateMark || bReinitialize )
			{
				ConstructSkew( false );
				m_nUpdateMark = nSkewUpdateMark;
			}
			else
				ConstructSkew( true );

			long nPoints = m_ssv.size();
			if( nPoints != 0 )
			{
				EnableVolaManagement( true, nCntVola > 1 );

				m_cGroup2.GetData().SetNumPoints( g_lCustomVola, nPoints );

				std::vector<CCustomStrikePoint>::iterator it = m_ssv.begin();

				for( int i = 1; i <= nPoints; i++ )				// For each point in main skew
				{
					dStrike   = it->m_dStrike;
					dVola     = it->m_dVola;

					// Add main points to chart
					m_cGroup2.GetData().SetX(g_lCustomVola, i, dStrike );
					m_cGroup2.GetData().SetY(g_lCustomVola, i, dVola * m_lVolaMultiplier );

					if( it->m_bATM )
						{
						m_cGroup2.GetData().SetNumPoints( g_lCustomVolaATM, 1 );
						m_cGroup2.GetData().SetX(g_lCustomVolaATM, 1, dStrike );
						m_cGroup2.GetData().SetY(g_lCustomVolaATM, 1, dVola * m_lVolaMultiplier );
						m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaATM, oc2dDisplayShow );
						}
					if( it->m_bIsBasePoint )
						{
						long nBasePoints = m_cGroup2.GetData().GetNumPoints( g_lCustomVolaBasePoints ) + 1;
						m_cGroup2.GetData().SetNumPoints( g_lCustomVolaBasePoints, nBasePoints );
						m_cGroup2.GetData().SetX(g_lCustomVolaBasePoints, nBasePoints, dStrike );
						m_cGroup2.GetData().SetY(g_lCustomVolaBasePoints, nBasePoints, dVola * m_lVolaMultiplier );
						m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaBasePoints, oc2dDisplayShow );
						}

					// Update min/max info
					if( dStrike < dMinX ) dMinX = dStrike;
					if( dStrike > dMaxX ) dMaxX = dStrike;
					if( dVola	< dMinY ) dMinY = dVola;
					if( dVola	> dMaxY ) dMaxY = dVola;

					it++;
				}

				m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVola, oc2dDisplayShow );
			}
			else
			{
				m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVola,		   oc2dDisplayHide );
				m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaATM,		   oc2dDisplayHide );
				m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaBasePoints, oc2dDisplayHide );

				EnableVolaManagement( false, false );
			}
		}

		long nPrevPoints = m_ssvPrev.size();
		if( nPrevPoints != 0 )
		{
			m_cGroup2.GetData().SetNumPoints( g_lCustomVolaPrev, nPrevPoints );

			std::vector<CCustomStrikePoint>::iterator pit = m_ssvPrev.begin();

			for( int i = 1; i <= nPrevPoints; i++ )				// For each point in prev skew
			{
				dStrike = pit->m_dStrike;
				dVola   = pit->m_dVola;

				// Add main points to chart
				m_cGroup2.GetData().SetX(g_lCustomVolaPrev, i, dStrike );
				m_cGroup2.GetData().SetY(g_lCustomVolaPrev, i, dVola * m_lVolaMultiplier );

				// Update min/max info
				if( dStrike < dMinX ) dMinX = dStrike;
				if( dStrike > dMaxX ) dMaxX = dStrike;
				if( dVola	< dMinY ) dMinY = dVola;
				if( dVola	> dMaxY ) dMaxY = dVola;

				pit++;
			}
			
			m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaPrev, oc2dDisplayShow );
		}
		else
		{
			m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaPrev, oc2dDisplayHide );
		}
	}
	catch(const _com_error&)
	{
		m_spSkew = NULL;

		m_ssv.clear();
		m_ssvPrev.clear();
		
		m_cGroup2.GetData().SetNumPoints( g_lCustomVola, 0 );
		m_cGroup2.GetData().SetNumPoints( g_lCustomVolaPrev, 0 );
		m_cGroup2.GetData().SetNumPoints( g_lCustomVolaATM, 0 );
		m_cGroup2.GetData().SetNumPoints( g_lCustomVolaBasePoints, 0 );

		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVola,		   oc2dDisplayHide );
		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaPrev,	   oc2dDisplayHide );
		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaATM,		   oc2dDisplayHide );
		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaBasePoints, oc2dDisplayHide );

		EnableVolaManagement( false, false );

//		ATLTRACE( _T("Error drawing custom volatility data. ") + e.Description() );
	}
	catch( ... )
	{
		m_spSkew = NULL;

		m_ssv.clear();
		m_ssvPrev.clear();
		
		m_cGroup2.GetData().SetNumPoints( g_lCustomVola, 0 );
		m_cGroup2.GetData().SetNumPoints( g_lCustomVolaPrev, 0 );
		m_cGroup2.GetData().SetNumPoints( g_lCustomVolaATM, 0 );
		m_cGroup2.GetData().SetNumPoints( g_lCustomVolaBasePoints, 0 );

		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVola,		   oc2dDisplayHide );
		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaPrev,	   oc2dDisplayHide );
		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaATM,		   oc2dDisplayHide );
		m_cGroup2.GetData().SetSeriesDisplay( g_lCustomVolaBasePoints, oc2dDisplayHide );

		EnableVolaManagement( false, false );

		strInfo += _T("Error drawing custom volatility data. ");
	}
#endif //_VOLA_MANAGER

	try
	{
		CDataProcessor::CLockVolaBands lv;
		COleDateTimeSpan dtPeriod = m_dtExp - COleDateTime::GetCurrentTime();
		long lExpPeriod = static_cast<long>(dtPeriod.GetTotalDays());
		long lExpPeriodLow,lExpPeriodHi;

		ivbands_data_map ivbandsData;		
		int nCntBands = 0;

		const vola_bands_map& volaBands = g_DataProcessor.GetVolaBands();

		if(volaBands.empty())
			throw E_FAIL;
		
		typedef vola_bands_map::const_iterator vola_bands_it;
		
		vola_bands_it itFind = volaBands.find(lExpPeriod);
		vola_bands_it itBand;

		std::pair  <vola_bands_it, vola_bands_it> bandsBounds = volaBands.equal_range (lExpPeriod);

		if(itFind != volaBands.end())
		{
			for( itBand = bandsBounds.first; itBand != bandsBounds.second; itBand++ ) 
			{
				const CVolaBandsData& data = itBand->second;
				double dStrike = priceData.GetMidMarket() * data.m_dStrikeMoneyness;

				if( bRange && (dStartStrike > dStrike ||
					dEndStrike < dStrike ) )
					continue;

				if( g_DataProcessor.GetChartMode() == enChart_5Penny || g_DataProcessor.GetChartMode() == enChart_5Penny_Range )
				{

					if( dMinX > dStrike || dMaxX < dStrike )
					{
						continue;
					}

				}

				if ( IsValidVola ( data.m_dIVBandLow) && IsValidVola ( data.m_dIVBandHigh) )
				{

					ivbandsData.insert ( std::make_pair (dStrike, data) );
					nCntBands ++;
				}
			}

		}
		else 
		{
			if(bandsBounds.first == volaBands.end()) // all expirations less
			{
				itBand = --bandsBounds.first;
				lExpPeriodLow = itBand->first;
				
				bandsBounds = volaBands.equal_range (lExpPeriodLow);

				for( itBand = bandsBounds.first; itBand != bandsBounds.second; itBand++ ) 
				{
					const CVolaBandsData& data = itBand->second;
					double dStrike = priceData.GetMidMarket() * data.m_dStrikeMoneyness;

					if( bRange && (dStartStrike > dStrike || dEndStrike < dStrike ) )
						continue;

					if( g_DataProcessor.GetChartMode() == enChart_5Penny || g_DataProcessor.GetChartMode() == enChart_5Penny_Range )
					{

						if( dMinX > dStrike || dMaxX < dStrike )
						{
							continue;
						}

					}

					if ( IsValidVola ( data.m_dIVBandLow) && IsValidVola ( data.m_dIVBandHigh) )
					{

						ivbandsData.insert ( std::make_pair (dStrike, data) );
						nCntBands ++;
					}
				}
			}
			else if(bandsBounds.second == volaBands.begin()) // all expirations greater
			{
				lExpPeriodHi = bandsBounds.second->first;

				bandsBounds = volaBands.equal_range (lExpPeriodHi);

				for( itBand = bandsBounds.first; itBand != bandsBounds.second; itBand++ ) 
				{
					const CVolaBandsData& data = itBand->second;
					double dStrike = priceData.GetMidMarket() * data.m_dStrikeMoneyness;

					if( bRange && (dStartStrike > dStrike || dEndStrike < dStrike) )
						continue;

					if( g_DataProcessor.GetChartMode() == enChart_5Penny || g_DataProcessor.GetChartMode() == enChart_5Penny_Range )
					{

						if( dMinX > dStrike || dMaxX < dStrike )
						{
							continue;
						}

					}

					if ( IsValidVola ( data.m_dIVBandLow) && IsValidVola ( data.m_dIVBandHigh) )
					{

						ivbandsData.insert ( std::make_pair (dStrike, data) );
						nCntBands ++;
					}
				}

			}
			else // making time interpolation
			{
				itBand = --bandsBounds.first;
				lExpPeriodLow = itBand->first;

				lExpPeriodHi = bandsBounds.second->first;
		
				std::pair  <vola_bands_it, vola_bands_it> bandsBoundsLow = volaBands.equal_range (lExpPeriodLow);
				std::pair  <vola_bands_it, vola_bands_it> bandsBoundsHi = volaBands.equal_range (lExpPeriodHi);

				ivbandsref_data_map ivbandsDataLow,ivbandsDataHi;

				for( itBand = bandsBoundsLow.first; itBand != bandsBoundsLow.second; itBand++ ) 
				{
					const CVolaBandsData& data = itBand->second;
					double dStrike = priceData.GetMidMarket() * data.m_dStrikeMoneyness;

					if( bRange && (dStartStrike > dStrike || dEndStrike < dStrike) )
						continue;

					if( g_DataProcessor.GetChartMode() == enChart_5Penny || g_DataProcessor.GetChartMode() == enChart_5Penny_Range )
					{
						if( dMinX > dStrike || dMaxX < dStrike )
						{
							continue;
						}
					}

					if ( IsValidVola ( data.m_dIVBandLow) && IsValidVola ( data.m_dIVBandHigh) )
					{
						ivbandsDataLow.insert ( std::make_pair (dStrike, &data) );
					}
				}
				for( itBand = bandsBoundsHi.first; itBand != bandsBoundsHi.second; itBand++ ) 
				{
					const CVolaBandsData& data = itBand->second;
					double dStrike = priceData.GetMidMarket() * data.m_dStrikeMoneyness;

					if( bRange && (dStartStrike > dStrike || dEndStrike < dStrike) )
						continue;

					if( g_DataProcessor.GetChartMode() == enChart_5Penny || g_DataProcessor.GetChartMode() == enChart_5Penny_Range )
					{

						if( dMinX > dStrike || dMaxX < dStrike )
						{
							continue;
						}

					}

					if ( IsValidVola ( data.m_dIVBandLow) && IsValidVola ( data.m_dIVBandHigh) )
					{
						ivbandsDataHi.insert ( std::make_pair (dStrike, &data) );
					}
				}

				if(ivbandsDataLow.size() != ivbandsDataHi.size())
					throw E_FAIL;

				ivbandsref_data_map::iterator iterLow,iterHi;
					
				for(iterLow = ivbandsDataLow.begin(),iterHi  = ivbandsDataHi.begin();
					iterLow != ivbandsDataLow.end() && iterHi != ivbandsDataHi.end();
						iterLow++,iterHi++)
				{													
					CVolaBandsData data;
					double dStrike = iterLow->first;
					
					data.m_dStrikeMoneyness = (iterLow->second)->m_dStrikeMoneyness;

					data.m_dIVBandHigh = CDataProcessor::GetTimeInterpolation(lExpPeriod,lExpPeriodLow,(iterLow->second)->m_dIVBandHigh,
												lExpPeriodHi,(iterHi->second)->m_dIVBandHigh);
								
					data.m_dIVBandLow = CDataProcessor::GetTimeInterpolation(lExpPeriod,lExpPeriodLow,(iterLow->second)->m_dIVBandLow,
												lExpPeriodHi,(iterHi->second)->m_dIVBandLow);
								
					ivbandsData.insert ( std::make_pair (dStrike, data) );
					nCntBands ++;
				}								
			}

		}		

		if ( nCntBands > 0 )
		{
			const CVolaBandsData* pFirst	= &ivbandsData.begin()->second;
			const CVolaBandsData* pLast	= &(--ivbandsData.end())->second;

			if ( dMinX <  pFirst->m_dStrikeMoneyness * priceData.GetMidMarket())
			{
				ivbandsData.insert (std::make_pair (dMinX, *pFirst));
				nCntBands++;
			}

			if ( dMaxX > pLast->m_dStrikeMoneyness * priceData.GetMidMarket() )
			{
				ivbandsData.insert (std::make_pair (dMaxX, *pLast));
				nCntBands++;
			}

			m_cGroup1.GetData().SetNumPoints(g_lHistVolaHi, nCntBands);
			m_cGroup1.GetData().SetNumPoints(g_lHistVolaLo, nCntBands);
			m_cGroup2.GetData().SetNumPoints(g_lHistVolaAvg, nCntBands);

			m_cGroup2.GetData().SetNumPoints(g_lHistVolaMax, nCntBands);
			m_cGroup2.GetData().SetNumPoints(g_lHistVolaMin, nCntBands);
			
			int n = 1;
			for (ivbands_data_map::iterator itBand = ivbandsData.begin();
				itBand != ivbandsData.end(); itBand++ )
			{
				const CVolaBandsData* pData = &itBand->second;
				
				double dStrike = itBand->first;  // Don't get it from pData! See pFirst & pLast related code

				double dV1 = pData->m_dIVBandHigh;
				double dV2 = pData->m_dIVBandLow;


				m_cGroup1.GetData().SetX(g_lHistVolaHi, n, dStrike);
				m_cGroup1.GetData().SetY(g_lHistVolaHi, n, dV1  * m_lVolaMultiplier);
				m_cGroup1.GetData().SetX(g_lHistVolaLo, n, dStrike);
				m_cGroup1.GetData().SetY(g_lHistVolaLo, n, dV2 * m_lVolaMultiplier);
				m_cGroup2.GetData().SetX(g_lHistVolaAvg, n, dStrike);

				m_cGroup2.GetData().SetX(g_lHistVolaMax, n, dStrike);
				m_cGroup2.GetData().SetY(g_lHistVolaMax, n, dV1  * m_lVolaMultiplier);
				m_cGroup2.GetData().SetX(g_lHistVolaMin, n, dStrike);
				m_cGroup2.GetData().SetY(g_lHistVolaMin, n, dV2 * m_lVolaMultiplier);

				double dAvg = (dV1 + dV2) /2.0;
				m_cGroup2.GetData().SetY(g_lHistVolaAvg, n, dAvg * m_lVolaMultiplier);

				if(dV1 < dMinY) dMinY = dV1;
				if(dV1 > dMaxY) dMaxY = dV1;
				
				if(dV2 < dMinY) dMinY = dV2;
				if(dV2 > dMaxY) dMaxY = dV2;

				if (dStrike < dMinX) dMinX = dStrike;
				if (dStrike > dMaxX) dMaxX = dStrike;

				n++;
			}

			m_cGroup1.GetData().SetSeriesDisplay(g_lHistVolaHi, oc2dDisplayShow);
			m_cGroup1.GetData().SetSeriesDisplay(g_lHistVolaLo, oc2dDisplayShow);
			m_cGroup2.GetData().SetSeriesDisplay(g_lHistVolaAvg, oc2dDisplayShow);
			m_cGroup2.GetData().SetSeriesDisplay(g_lHistVolaMax, oc2dDisplayShow);
			m_cGroup2.GetData().SetSeriesDisplay(g_lHistVolaMin, oc2dDisplayShow);
		}
		else
		{
			if ( strInfo.GetLength() > 0)
				strInfo += _T("\n");

			strInfo += _T("Not enough volatility bands data to display!");

			m_cGroup1.GetData().SetSeriesDisplay(g_lHistVolaHi, oc2dDisplayHide);
			m_cGroup1.GetData().SetSeriesDisplay(g_lHistVolaLo, oc2dDisplayHide);
			m_cGroup2.GetData().SetSeriesDisplay(g_lHistVolaAvg, oc2dDisplayHide);
			m_cGroup2.GetData().SetSeriesDisplay(g_lHistVolaMax, oc2dDisplayHide);
			m_cGroup2.GetData().SetSeriesDisplay(g_lHistVolaMin, oc2dDisplayHide);
		}
	}
	catch( const _com_error& e )
	{
		strInfo += _T("Error drawing volatility bands data. ");
		strInfo += (LPCSTR)e.Description();
	}
	catch( ... )
	{
		strInfo += _T("Error drawing volatility bands data. ");
	}

	if (dMinY != DBL_MAX && dMaxY != -DBL_MAX  && dMinX != DBL_MAX && dMaxX != -DBL_MAX)
	{
		m_AxisX.GetMax().SetValue(dMaxX);
		m_AxisX.GetMin().SetValue(dMinX);
		m_AxisX.GetDataMax().SetValue(dMaxX);
		m_AxisX.GetDataMin().SetValue(dMinX);

		AdjustBack (dMinX, dMaxX, dMinY, dMaxY);
	}
	
	if(strInfo.GetLength() > 0)
	{
		m_lblInfoIndicator.SetIsShowing(TRUE);
		m_lblInfo.GetText().SetText(strInfo);
	}
	else
	{
		m_lblInfoIndicator.SetIsShowing(FALSE);
		m_lblInfo.SetIsShowing(FALSE);
		m_lblInfo.GetText().SetText(_T(""));
	}
	AdjustLogo();

	SetChartIsBatched(FALSE);
}


#ifdef _VOLA_MANAGER
void CStrikeSkewDlg::UpdateCustomVolatility( UINT nMessage, CCustomVolaNotification *pNotification )
{
	ASSERT( pNotification );

	if( pNotification->m_dtMonth != 0 && pNotification->m_dtMonth != m_dtMonth )
		return;

	if( nMessage == WM_CUSTOM_VOLA_CHANGED )
		m_bIsDirty = TRUE;
	else if( nMessage == WM_CUSTOM_VOLA_SAVED || nMessage == WM_CUSTOM_VOLA_RESTORED )
		m_bIsDirty = FALSE;

	UpdateChart( false );
}
#endif //_VOLA_MANAGER


BOOL CStrikeSkewDlg::OnInitDialog() 
{
	CChartDlg::OnInitDialog();
	
	m_AxisX.SetAnnotationMethod(oc2dAnnotateValues);
	
	m_strTitle.LoadString(IDS_STRIKE_SKEW_TITLE);
	m_strTitle = m_dtMonth.Format(_T("%b,%d %y ")) + m_strTitle;
	
	char cFirst = m_strTitle.GetAt(0);
		 cFirst = (TCHAR)CharUpper( (LPTSTR)MAKELONG(cFirst, 0) );
	m_strTitle.SetAt( 0, cFirst );
	SetWindowText( m_strTitle );

	return TRUE;
}
bool CStrikeSkewDlg::UpdateCustomVolaPoint( long lMovingPtIdx, double dVola, double& dtMonth )
{
#ifdef _VOLA_MANAGER
	dtMonth = (double)m_dtMonth;

	if( lMovingPtIdx > 0 && m_ssv[lMovingPtIdx-1].m_dVola != dVola )
	{
		m_ssv[lMovingPtIdx-1].m_dVola = dVola;
		if( !m_nShifted )
			m_ssv[lMovingPtIdx-1].m_bIsBasePoint = true;
		else if( lMovingPtIdx == m_lMovingPtIdx )
			m_ssv[lMovingPtIdx-1].m_bIsBasePoint = true;
		UpdateSkewPoint( m_ssv[lMovingPtIdx-1] );

		return true;
	}
#endif //_VOLA_MANAGER

	return false;
}

void CStrikeSkewDlg::InterpolateCustomVolaToImplied()
{
#ifdef _VOLA_MANAGER
	strikes_map strikesData;
	{
		CDataProcessor::CLockVola lv;

		long nCntVola = g_DataProcessor.GetOptionsDataByEpiration( m_dtMonth, m_dtExp, strikesData );

		if( nCntVola == 0 )
			return;

		bool bFlatNoBid =  g_DataProcessor.GetFlatNoBid();
		

		if (m_lATMPointIdx < 1) return;

		double dATMStrike = m_ssv[m_lATMPointIdx - 1].m_dStrike;

		double dStrikeFirstWithBid, dStrikeLastWithBid;
		dStrikeFirstWithBid = dStrikeLastWithBid = dATMStrike;

		m_spSkew->RemoveAll();
	
		if(bFlatNoBid)
		{

			double dStrike = 0, dVola = 0;

			strikes_map::iterator sit = strikesData.lower_bound( dATMStrike );
			if( sit == strikesData.end() )
				sit--;

			dStrike = sit->second->m_dStrike;
			dVola   = sit->second->m_dVola;
			
			if( dStrike == dATMStrike )
			{
				m_ssv[m_lATMPointIdx - 1].m_bIsBasePoint = true;
				m_ssv[m_lATMPointIdx - 1].m_dVola = dVola;
				UpdateSkewPoint( m_ssv[m_lATMPointIdx - 1]);
			}

												
			// For each put in current setup mode
			for(CStrikeSkewValues::reverse_iterator rit = m_ssv.rbegin(); rit != m_ssv.rend(); rit++ )
			{
				if (rit->m_dStrike >= dATMStrike)
					continue;

				dStrike = 0, dVola = 0;

				strikes_map::iterator sit = strikesData.lower_bound( rit->m_dStrike );

				if( sit == strikesData.end() )
					sit--;

				dStrike = sit->second->m_dStrike;
				dVola   = sit->second->m_dVola;

				if(sit->second->m_dPriceBid > BAD_DOUBLE_VALUE && sit->second->m_dPriceBid != 0.0f)
				{
					if( dStrike == rit->m_dStrike )
					{
						dStrikeFirstWithBid = dStrike;
						rit->m_bIsBasePoint = true;
						rit->m_dVola = dVola;
						UpdateSkewPoint( *rit );
					}
				}
			}
			// For each call in current setup mode
			for( CStrikeSkewValues::iterator it = m_ssv.begin(); it != m_ssv.end(); it++ )
			{
				if (it->m_dStrike <= dATMStrike)
					continue;

				dStrike = 0, dVola = 0;

				strikes_map::iterator sit = strikesData.lower_bound( it->m_dStrike );

				if( sit == strikesData.end() )
					sit--;

				dStrike = sit->second->m_dStrike;
				dVola   = sit->second->m_dVola;

				if(sit->second->m_dPriceBid > BAD_DOUBLE_VALUE && sit->second->m_dPriceBid != 0.0f)
				{
					if( dStrike == it->m_dStrike )
					{
						dStrikeLastWithBid = dStrike;
						it->m_bIsBasePoint = true;
						it->m_dVola = dVola;
						UpdateSkewPoint( *it );
					}
				}
			}

			for(CStrikeSkewValues::reverse_iterator rit = m_ssvAll.rbegin(); rit != m_ssvAll.rend(); rit++ )
			{
				if (rit->m_dStrike >= dATMStrike)
					continue;

				dStrike = 0, dVola = 0;

				strikes_map::iterator sit = strikesData.lower_bound( rit->m_dStrike );

				if( sit == strikesData.end() )
					sit--;

				dStrike = sit->second->m_dStrike;
				dVola   = sit->second->m_dVola;

				if(sit->second->m_dPriceBid > BAD_DOUBLE_VALUE && sit->second->m_dPriceBid != 0.0f)
				{
					if( dStrike == rit->m_dStrike && Deleted == m_spSkew->Status[dStrike])
					{
						dStrikeFirstWithBid = dStrike;
						m_spSkew->Add( dStrike, dVola * 100L, VARIANT_TRUE );
					}
				}
			}

			for(CStrikeSkewValues::iterator it = m_ssvAll.begin(); it != m_ssvAll.end(); it++ )
			{
				if (it->m_dStrike <= dATMStrike)
					continue;

				dStrike = 0, dVola = 0;

				strikes_map::iterator sit = strikesData.lower_bound( it->m_dStrike );

				if( sit == strikesData.end() )
					sit--;

				dStrike = sit->second->m_dStrike;
				dVola   = sit->second->m_dVola;

				if(sit->second->m_dPriceBid > BAD_DOUBLE_VALUE && sit->second->m_dPriceBid != 0.0f)
				{
					if( dStrike == it->m_dStrike && Deleted == m_spSkew->Status[dStrike])
					{
						dStrikeLastWithBid = dStrike;
						m_spSkew->Add( dStrike, dVola * 100L, VARIANT_TRUE );
					}
				}
			}
		}
		else
		{
			// For each custom point in current setup mode
			for( CStrikeSkewValues::iterator it = m_ssv.begin(); it != m_ssv.end(); it++ )
			{
				double dStrike = 0, dVola = 0;

				strikes_map::iterator sit = strikesData.lower_bound( it->m_dStrike );

				if( sit == strikesData.end() )
					sit--;

				dStrike = sit->second->m_dStrike;
				dVola   = sit->second->m_dVola;

				if( dStrike == it->m_dStrike )
				{
					it->m_bIsBasePoint = true;
					it->m_dVola = dVola;
					UpdateSkewPoint( *it );
				}
			}

			for(CStrikeSkewValues::iterator it = m_ssvAll.begin(); it != m_ssvAll.end(); it++ )
			{

				double dStrike = 0, dVola = 0;
				dStrike = 0, dVola = 0;

				strikes_map::iterator sit = strikesData.lower_bound( it->m_dStrike );

				if( sit == strikesData.end() )
					sit--;

				dStrike = sit->second->m_dStrike;
				dVola   = sit->second->m_dVola;

				if( dStrike == it->m_dStrike && Deleted == m_spSkew->Status[dStrike])
				{
					m_spSkew->Add( dStrike, dVola * 100L, VARIANT_TRUE );
				}
			}

		}



		if(bFlatNoBid)
		{
			double dExtrVola = m_spSkew->Volatility[dStrikeFirstWithBid];	
			m_spSkew->Add( dStrikeFirstWithBid, dExtrVola, VARIANT_TRUE );
			dExtrVola = m_spSkew->Volatility[dStrikeLastWithBid];	
			m_spSkew->Add( dStrikeLastWithBid, dExtrVola, VARIANT_TRUE );			
		}
		_ExtrapolateMonthVolatility(bFlatNoBid,dStrikeFirstWithBid,dStrikeLastWithBid);

	}

	PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, (m_dtMonth) );
#endif //_VOLA_MANAGER
}


void CStrikeSkewDlg::RestoreCustomVola()
{
#ifdef _VOLA_MANAGER

	m_spSkew->Reload();

	PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, (m_dtMonth) );
#endif //_VOLA_MANAGER
}


void CStrikeSkewDlg::UpdateCustomVola()
{
#ifdef _VOLA_MANAGER
	UpdateSkew();

	try
		{
		m_spSkew->Save();

		PostNotification( WM_CUSTOM_VOLA_SAVED, CCustomVolaNotification::enSkew, (m_dtMonth) );
		}
	catch( const _com_error& e )
		{
		MessageBox( e.Description(), NULL, MB_OK|MB_ICONERROR );
		}
#endif //_VOLA_MANAGER
}


bool CStrikeSkewDlg::SetupVolaEditor( long lPtIdx, CVolaEditorDlg& ve )
{
#ifdef _VOLA_MANAGER
	ve.m_dtExpiration = static_cast<long>(m_dtExp);
	ve.m_dPrice		  = g_DataProcessor.GetContractPrice();
	ve.m_bIsCall	  = ve.m_dPrice < m_ptPrev.m_dStrike;

	bool   bVolaInPercent  = true;
	double lVolaMultiplier = bVolaInPercent ? 100 : 1;

	ve.SetCurStrike( m_ptPrev.m_dStrike );
	ve.SetNewStrike( m_ptPrev.m_dStrike );
	ve.SetCurVola( m_ptPrev.m_dVola * lVolaMultiplier );
	ve.SetNewVola( m_ptPrev.m_dVola * lVolaMultiplier );

	return true;
#endif //_VOLA_MANAGER
	return false;
}


void CStrikeSkewDlg::SavePointValue( long lMovingPtIdx )
{
#ifdef _VOLA_MANAGER
	m_ptPrev = m_ssv[lMovingPtIdx - 1];
#endif //_VOLA_MANAGER
}


double CStrikeSkewDlg::GetPointValue( long lMovingPtIdx )
{
#ifdef _VOLA_MANAGER
	bool bVolaInPercent = true;
	double lVolaMultiplier = bVolaInPercent ? 100 : 1;

	return m_ssv[lMovingPtIdx - 1].m_dVola * lVolaMultiplier;
#endif //_VOLA_MANAGER
	 return 0;
}

CString CStrikeSkewDlg::GetXLabel (long lSeries, long lPtIdx)
{
	double dX = m_cGroup2.GetData().GetX(lSeries, lPtIdx);

	CString strStrike;
	
	if( lSeries == g_lVola && g_DataProcessor.GetCurveMode() == enOTM )
	{
		bool bIsCall = m_vecTypes[lPtIdx-1];
		if (bIsCall)
			strStrike += _T("(Call)");
		else
			strStrike += _T("(Put)");
	}

	CString strTemp;
	strTemp.Format (_T(" S: %.2f"), dX);
	strStrike += strTemp;

	return strStrike;
}


#ifdef _VOLA_MANAGER
void CStrikeSkewDlg::ConstructSkew( bool bATMPointOnly )
{
	if( !bATMPointOnly )
	{
		bool bVolaInPercent = true; //m_rMgr.GetShowVolatilityInPercents();
		m_lVolaMultiplier = bVolaInPercent ? 100L : 1L;

		double dStartStrike = 0.;
		double dEndStrike   = 0.;
		bool   bRange		= g_DataProcessor.GetRangeInfo( dStartStrike, dEndStrike );

		long nCurBasePointNumber = 1;

		m_ssv.clear();
		m_ssvAll.clear();
		for( int i = 0; i < m_spSkew->Count; i++ )
		{
			CCustomStrikePoint pt;

			pt.m_dStrike = m_spSkew->Strike[i];

			m_ssvAll.push_back( pt );

			if( bRange )
			{
				if( pt.m_dStrike < dStartStrike )
					continue;
				else if( pt.m_dStrike > dEndStrike )
					continue;
			}
			
			if( m_spSkew->Status[pt.m_dStrike] == Deleted )
				continue;
			pt.m_dVola        = m_spSkew->Volatility[pt.m_dStrike]/m_lVolaMultiplier;
			pt.m_enOptType    = OTM;
			pt.m_bIsBasePoint = m_spSkew->IsBasePoint[pt.m_dStrike] == VARIANT_TRUE ? true : false;
			if( pt.m_bIsBasePoint )
				pt.m_nBasePointNumber = nCurBasePointNumber++;

			m_ssv.push_back( pt );
		}

		std::sort( m_ssvAll.begin(), m_ssvAll.end() );
		std::sort( m_ssv.begin(), m_ssv.end() );

		if( !m_bIsDirty )
			m_ssvPrev = m_ssv;
	}

	// Find ATM point index
	long idx = 1; m_lATMPointIdx = 0;
	for( CStrikeSkewValues::iterator it = m_ssv.begin(); it != m_ssv.end(); it++, idx++ )
	{
		it->m_bATM = m_spSkew->IsATM[it->m_dStrike] == VARIANT_TRUE ? true : false;
		if( it->m_bATM )
			m_lATMPointIdx = idx;
	}
}


bool CStrikeSkewDlg::UpdateSkewPoint( const CCustomStrikePoint& pt )
{
	bool bVolaInPercent = true; //m_rMgr.GetShowVolatilityInPercents();
	m_lVolaMultiplier = bVolaInPercent ? 100L : 1L;

	m_spSkew->Add( pt.m_dStrike, pt.m_dVola * m_lVolaMultiplier, pt.m_bIsBasePoint ? VARIANT_TRUE : VARIANT_FALSE );
	return true;
}


void CStrikeSkewDlg::UpdateSkew()
{
	for( CStrikeSkewValues::iterator it = m_ssv.begin(); it != m_ssv.end(); it++ )
		UpdateSkewPoint( *it );
}

void CStrikeSkewDlg::_ExtrapolateMonthVolatility( bool bIsFlatWings, double dStrikeLow, double dStrikeHi )
{

	// For each custom point in current setup mode
	for( CStrikeSkewValues::iterator it = m_ssv.begin(); it != m_ssv.end(); it++ )
	{
		double dStrike = 0, dVola = 0;
		dStrike = it->m_dStrike;

		// Check if there is explicitly set point for strike
		EVolaItemStatus enStatus = Deleted;
		try
		{
			enStatus = m_spSkew->Status[dStrike];
		}
		catch( const _com_error& )
		{
		}

		if( enStatus == Deleted )
		{
			try
			{

				if(bIsFlatWings && ( dStrike < dStrikeLow || dStrike > dStrikeHi ) )
				{
					if(dStrike < dStrikeLow)
						dVola = m_spSkew->Volatility[dStrikeLow];
					else if(dStrike > dStrikeHi)
						dVola = m_spSkew->Volatility[dStrikeHi];

					it->m_dVola = dVola;
					it->m_bIsBasePoint = true;

					m_spSkew->Add( dStrike, dVola, VARIANT_TRUE );
				}
				else
				{
					dVola = m_spSkew->Volatility[dStrike];
					
					it->m_dVola = dVola;
					it->m_bIsBasePoint = false;

					m_spSkew->Add( dStrike, dVola, VARIANT_FALSE );
					
				}

			}
			catch( const _com_error& )
			{
				break;
			}
		}
	}

	for(CStrikeSkewValues::iterator it = m_ssvAll.begin(); it != m_ssvAll.end(); it++ )
	{
		double dStrike = 0, dVola = 0;
		dStrike = it->m_dStrike;

		// Check if there is explicitly set point for strike
		EVolaItemStatus enStatus = Deleted;
		try
		{
			enStatus = m_spSkew->Status[dStrike];
		}
		catch( const _com_error& )
		{
		}

		if( enStatus == Deleted )
		{
			try
			{

				if(bIsFlatWings && ( dStrike < dStrikeLow || dStrike > dStrikeHi ) )
				{
					if(dStrike < dStrikeLow)
						dVola = m_spSkew->Volatility[dStrikeLow];
					else if(dStrike > dStrikeHi)
						dVola = m_spSkew->Volatility[dStrikeHi];

					m_spSkew->Add( dStrike, dVola, VARIANT_TRUE );
				}
				else
				{
					dVola = m_spSkew->Volatility[dStrike];

					m_spSkew->Add( dStrike, dVola, VARIANT_FALSE );					
				}

			}
			catch( const _com_error& )
			{
				break;
			}
		}
	}

}

#endif //_VOLA_MANAGER