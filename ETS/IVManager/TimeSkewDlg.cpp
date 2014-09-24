// TimeSkewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVAnalyzer.h"
#include "TimeSkewDlg.h"
#include "DataProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTimeSkewDlg dialog


CTimeSkewDlg::CTimeSkewDlg()
{
	m_bManageableVola = FALSE; // Not implemented in current release
	//{{AFX_DATA_INIT(CTimeSkewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTimeSkewDlg::DoDataExchange(CDataExchange* pDX)
{
	CChartDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeSkewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeSkewDlg, CChartDlg)
	//{{AFX_MSG_MAP(CTimeSkewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeSkewDlg message handlers

void CTimeSkewDlg::InitializeChart()
{
#ifdef _VOLA_MANAGER
	EnableVolaManagement( false, false );
	SetIsDirty( false );
#endif //_VOLA_MANAGER
}

bool CTimeSkewDlg::GetStrikeInterpolationIVBands(double dStrikeMoneyness,vola_bands_it itFirst,
		vola_bands_it itLast,CVolaBandsData& Data)
{	
	vola_bands_it itBand,itBelow,itAbove;
	itBand = itBelow = itAbove = itFirst;
	const CVolaBandsData* pdata = &itBand->second;
	const CVolaBandsData* pdataAbove = &itAbove->second;
	const CVolaBandsData* pdataBelow = &itBelow->second;

	for( itBand = itFirst; itBand != itLast; itBand++ ) 
	{
		pdata = &itBand->second;
		pdataAbove = &itAbove->second;
		pdataBelow = &itBelow->second;

		if(pdata->m_dStrikeMoneyness <= dStrikeMoneyness
			&& abs(pdata->m_dStrikeMoneyness - dStrikeMoneyness) < 
			abs(pdataBelow->m_dStrikeMoneyness - dStrikeMoneyness))
			itBelow = itBand;
		
		if(pdata->m_dStrikeMoneyness >= dStrikeMoneyness
			&& abs(pdata->m_dStrikeMoneyness - dStrikeMoneyness) < abs(pdataAbove->m_dStrikeMoneyness - dStrikeMoneyness))
			itAbove = itBand;
	}
	pdataAbove = &itAbove->second;
	pdataBelow = &itBelow->second;
				
	if(dStrikeMoneyness >= pdataBelow->m_dStrikeMoneyness)
	{
		if( IsValidVola ( pdataBelow->m_dIVBandLow) && IsValidVola ( pdataBelow->m_dIVBandHigh) )
		{
			Data.Clear();
			Data.m_dStrikeMoneyness = dStrikeMoneyness;
			Data.m_dIVBandHigh = pdataBelow->m_dIVBandHigh;
			Data.m_dIVBandLow = pdataBelow->m_dIVBandLow;
			return true;
		}
		else
			return false;
	}
	else if(dStrikeMoneyness <= pdataAbove->m_dStrikeMoneyness)
	{
		if( IsValidVola ( pdataAbove->m_dIVBandLow) && IsValidVola ( pdataAbove->m_dIVBandHigh) )
		{
			Data.Clear();
			Data.m_dStrikeMoneyness = dStrikeMoneyness;
			Data.m_dIVBandHigh = pdataAbove->m_dIVBandHigh;
			Data.m_dIVBandLow = pdataAbove->m_dIVBandLow;
			return true;
		}
		else return false;
	}
	else
	{
		double dIVBandLow = CDataProcessor::GetStrikeInterpolation(dStrikeMoneyness,
				pdataBelow->m_dStrikeMoneyness,pdataBelow->m_dIVBandLow,
				pdataAbove->m_dStrikeMoneyness,pdataAbove->m_dIVBandLow);
		double dIVBandHigh = CDataProcessor::GetStrikeInterpolation(dStrikeMoneyness,
				pdataBelow->m_dStrikeMoneyness,pdataBelow->m_dIVBandHigh,
				pdataAbove->m_dStrikeMoneyness,pdataAbove->m_dIVBandHigh);

		if( IsValidVola ( dIVBandLow) && IsValidVola ( dIVBandHigh) )
		{
			Data.Clear();
			Data.m_dStrikeMoneyness = dStrikeMoneyness;
			Data.m_dIVBandHigh = dIVBandHigh;
			Data.m_dIVBandLow = dIVBandLow;
			return true;
		}
		else return false;
	}
	return false;
}


void CTimeSkewDlg::UpdateChart( bool bReinitialize )
{
	CString strStock;

	const sUndPrice& priceData = g_DataProcessor.GetUndPrice();

	if(!fEQZero(priceData.m_fAsk) || !fEQZero(priceData.m_fBid))
	{
		if(!fEQZero(priceData.m_fLast))
			strStock.Format (_T(" - [%s %.2f/%.2f %.2f]"), (LPCTSTR) g_DataProcessor.GetContractSymbol(), priceData.m_fBid, priceData.m_fAsk, priceData.m_fLast);
		else
			strStock.Format (_T(" - [%s %.2f/%.2f]"), (LPCTSTR) g_DataProcessor.GetContractSymbol(), priceData.m_fBid, priceData.m_fAsk);
	}
	else
	{
		if(!fEQZero(priceData.m_fLast))
			strStock.Format (_T(" - [%s %.2f]"), (LPCTSTR) g_DataProcessor.GetContractSymbol(), priceData.m_fLast);		
		else
			strStock.Format (_T(" - [%s]"), (LPCTSTR) g_DataProcessor.GetContractSymbol());		
	}

	CString strTitle = m_strTitle;
		
	strTitle += _T(' ');
	strTitle += GetCurveModeTitle() + _T(' ');
	strTitle += strStock;

	SetTitle(strTitle);

	bool bVolaInPercent = true; //m_rMgr.GetShowVolatilityInPercents();
	long lVolaMultiplier = bVolaInPercent ? 100L : 1L;
	
	BOOL bHaveLeap = FALSE;
	SetChartIsBatched(TRUE);

	m_ctlChart.GetLegend().SetIsShowing(FALSE);

	COleDateTime dtToday(floor(COleDateTime::GetCurrentTime().m_dt));
	int nMonth = dtToday.GetMonth(), nYear = dtToday.GetYear();
	COleDateTime dt;
	CString fmt;
	CString strInfo;

	// date : Nov 21 2001
	// fmt = _T("%d-%b"); // 21-Nov
	// fmt = _T("%b,%d"); // Nov,21
	// fmt = _T("%d/%m"); // 21/11
	// fmt = _T("%b-%y"); // Nov-01
	fmt = _T("%b,%d %y"); // Nov 01 

	double dV1, dV2, dX;
	
	double dMinX = DBL_MAX, dMaxX = -DBL_MAX;
	double dMinY = DBL_MAX, dMaxY = -DBL_MAX;

	int i, n;
	m_AxisX.GetValueLabels().RemoveAll();

	// clear chart
	ClearSeries( m_lMovingPtIdx != 0 );
	
	const time_skew_map& tsdata = g_DataProcessor.GetTimeSkewData();
	time_skew_map::const_iterator it;

	try
	{
		CDataProcessor::CLockVolaBands lv;
		CDataProcessor::CLockTimeSkew lts;

		int nCntBands = 0;
		int i = 0;

		vola_bands_map ivbandsData;

		const vola_bands_map& volaBands = g_DataProcessor.GetVolaBands();

		if(volaBands.empty())
			throw E_FAIL;

		if(!priceData.GetMidMarket())
			throw E_FAIL;

		for(it = tsdata.begin(); it != tsdata.end(); it++, i++ )
		{
			const COptionData* pOpt = it->second.m_pOption;
			double dStrikeMoneyness = pOpt->m_dStrike / priceData.GetMidMarket();

			COleDateTimeSpan dtPeriod = COleDateTime(pOpt->m_dtExpiration) - dtToday;
			long lExpPeriod = static_cast<long>( dtPeriod.GetTotalDays());
			long lExpPeriodLow=0;
			long lExpPeriodHi = 0;

			vola_bands_it itFind = volaBands.find(lExpPeriod);
			vola_bands_it itBand, itAbove,itBelow;

			std::pair  <vola_bands_it, vola_bands_it> bandsBounds = volaBands.equal_range (lExpPeriod);

			if(itFind != volaBands.end())
			{
				CVolaBandsData Data;
				if(!GetStrikeInterpolationIVBands(dStrikeMoneyness,bandsBounds.first,bandsBounds.second,Data))
					throw E_FAIL;

				ivbandsData.insert(std::make_pair(i,Data));
				nCntBands++;
			}
			else 
			{
				if(bandsBounds.first == volaBands.end()) // all expirations less
				{
					itBand = --bandsBounds.first;
					lExpPeriodLow = itBand->first;
					
					bandsBounds = volaBands.equal_range (lExpPeriodLow);
		
					CVolaBandsData Data;
					if(!GetStrikeInterpolationIVBands(dStrikeMoneyness,bandsBounds.first,bandsBounds.second,Data))
						throw E_FAIL;

					ivbandsData.insert(std::make_pair(i,Data));
					nCntBands++;
				}
				else if(bandsBounds.second == volaBands.begin()) // all expirations greater
				{
					lExpPeriodHi = bandsBounds.second->first;

					bandsBounds = volaBands.equal_range (lExpPeriodHi);
					
					CVolaBandsData Data;
					if(!GetStrikeInterpolationIVBands(dStrikeMoneyness,bandsBounds.first,bandsBounds.second,Data))
						throw E_FAIL;

					ivbandsData.insert(std::make_pair(i,Data));
					nCntBands++;
				}
				else // making time interpolation
				{
					itBand = --bandsBounds.first;
					lExpPeriodLow = itBand->first;

					lExpPeriodHi = bandsBounds.second->first;
			
					std::pair  <vola_bands_it, vola_bands_it> bandsBoundsLow = volaBands.equal_range (lExpPeriodLow);
					std::pair  <vola_bands_it, vola_bands_it> bandsBoundsHi = volaBands.equal_range (lExpPeriodHi);

					CVolaBandsData DataLow;
					CVolaBandsData DataHi;

					if(!GetStrikeInterpolationIVBands(dStrikeMoneyness,bandsBoundsLow.first,bandsBoundsLow.second,DataLow))
						throw E_FAIL;

					if(!GetStrikeInterpolationIVBands(dStrikeMoneyness,bandsBoundsHi.first,bandsBoundsHi.second,DataHi))
						throw E_FAIL;

					CVolaBandsData dataTime;

					dataTime.m_dStrikeMoneyness = dStrikeMoneyness;
					dataTime.m_dIVBandHigh = CDataProcessor::GetTimeInterpolation(lExpPeriod,lExpPeriodLow,DataLow.m_dIVBandHigh,
										lExpPeriodHi,DataHi.m_dIVBandHigh);
									
					dataTime.m_dIVBandLow = CDataProcessor::GetTimeInterpolation(lExpPeriod,lExpPeriodLow,DataLow.m_dIVBandLow,
										lExpPeriodHi,DataHi.m_dIVBandLow);
									
					ivbandsData.insert ( std::make_pair (i, dataTime) );
					nCntBands ++;
				}

			}		
		}

		
		if(nCntBands > 0)
		{
			m_cGroup1.GetData().SetNumPoints(g_lHistVolaHi, nCntBands);
			m_cGroup1.GetData().SetNumPoints(g_lHistVolaLo, nCntBands);
			m_cGroup2.GetData().SetNumPoints(g_lHistVolaAvg, nCntBands);

			m_cGroup2.GetData().SetNumPoints(g_lHistVolaMax, nCntBands);
			m_cGroup2.GetData().SetNumPoints(g_lHistVolaMin, nCntBands);
			// show IV bands data
			n = 1;
			for(vola_bands_map::iterator it = ivbandsData.begin(); it != ivbandsData.end(); it++)
			{
				dX = it->first * 30;
				
				const CVolaBandsData* pData = &it->second;

				dV1 = pData->m_dIVBandHigh;
				dV2 = pData->m_dIVBandLow;

				m_cGroup1.GetData().SetX(g_lHistVolaHi, n, dX);
				m_cGroup1.GetData().SetY(g_lHistVolaHi, n, dV1 * lVolaMultiplier);
				m_cGroup1.GetData().SetX(g_lHistVolaLo, n, dX);
				m_cGroup1.GetData().SetY(g_lHistVolaLo, n, dV2 * lVolaMultiplier);
				m_cGroup2.GetData().SetX(g_lHistVolaAvg, n, dX);

				m_cGroup2.GetData().SetX(g_lHistVolaMax, n, dX);
				m_cGroup2.GetData().SetY(g_lHistVolaMax, n, dV1 * lVolaMultiplier);
				m_cGroup2.GetData().SetX(g_lHistVolaMin, n, dX);
				m_cGroup2.GetData().SetY(g_lHistVolaMin, n, dV2 * lVolaMultiplier);

				double dAvg = (dV1 + dV2) /2.0;
				m_cGroup2.GetData().SetY(g_lHistVolaAvg, n, dAvg * lVolaMultiplier);

				if(dV1 < dMinY) dMinY = dV1;
				if(dV1 > dMaxY) dMaxY = dV1;
				
				if(dV2 < dMinY) dMinY = dV2;
				if(dV2 > dMaxY) dMaxY = dV2;

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
			// indicate insufficient data
			strInfo = _T("Not enough volatility bands data to display!");
			m_cGroup1.GetData().SetSeriesDisplay(g_lHistVolaHi,  oc2dDisplayHide);
			m_cGroup1.GetData().SetSeriesDisplay(g_lHistVolaLo,  oc2dDisplayHide);
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

	int nCntVola = 0;
	try
	{
		CDataProcessor::CLockVola lv;
		CDataProcessor::CLockTimeSkew lts;

		for( it = tsdata.begin(); it != tsdata.end(); it++ )
		{	
			const COptionData * pOpt = it->second.m_pOption;
			if ( IsValidVola ( pOpt->m_dVola)  )
				nCntVola++;
		}

		if( nCntVola > 0 )
		{
			m_cGroup2.GetData().SetNumPoints(g_lVola, nCntVola);
			n = 1;
			i = 0;
			m_vecDates.reserve  ( nCntVola );
			m_vecPrices.reserve ( nCntVola );
			m_vecSymbols.reserve( nCntVola );

			for( it = tsdata.begin(); it != tsdata.end(); it++, i++ )
			{
				dV1 = it->second.m_pOption->m_dVola;
				if( IsValidVola( dV1 ) )
				{
					dX = i * 30;

					m_cGroup2.GetData().SetX(g_lVola, n, dX);
					m_cGroup2.GetData().SetY(g_lVola, n, dV1 * lVolaMultiplier);

					if( dV1 < dMinY ) dMinY = dV1;
					if( dV1 > dMaxY ) dMaxY = dV1;
					
					m_vecSymbols.push_back( it->second.m_pOption->m_strName     );
					m_vecDates.push_back  ( it->second.m_pOption->m_dtPriceDate );
					m_vecPrices.push_back ( it->second.m_pOption->m_dPrice      );

					n++;
				}
			}

			m_cGroup2.GetData().SetSeriesDisplay(g_lVola, oc2dDisplayShow);
		}
		else
		{
			m_cGroup2.GetData().SetSeriesDisplay(g_lVola, oc2dDisplayHide);
			
			if(strInfo.GetLength() > 0)
				strInfo += _T("\n");
			strInfo += _T("Not enough implied volatility data to display!");
		}

		// show labels
		i = 0;
		for(it = tsdata.begin(); it != tsdata.end(); it++, i++ )
		{
			COleDateTime dtExpiration = (DATE)it->first.date_;

			CString str = dtExpiration.Format(fmt);
			
			dX = i*30;
			m_AxisX.GetValueLabels().Add(dX, str);
		}

		dMinX = 0;
		dMaxX = (tsdata.size()-1) * 30;
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
			m_spSkew  = spSurface->TimeSkew;
			spSurface = NULL;
		}
		spVM = NULL;

		if( !m_lMovingPtIdx )
		{
			ConstructSkew( tsdata );

			if( !m_bIsDirty )
			{
				m_tsvPrev.clear();
				m_tsvPrev = m_tsv;
			}

			long nPoints = m_tsv.size();
			if( nPoints != 0 )
			{
				EnableVolaManagement( true, nCntVola > 1 );

				m_cGroup2.GetData().SetNumPoints( g_lCustomVola, nPoints );

				int pti = 1;
				for( std::vector<CCustomTimePoint>::iterator it = m_tsv.begin(); it != m_tsv.end(); it++, i++ )
				{
					dX = it->m_nIndex * 30;

					double dV = it->m_dVola;
					m_cGroup2.GetData().SetX(g_lCustomVola, pti, dX );
					m_cGroup2.GetData().SetY(g_lCustomVola, pti, dV * m_lVolaMultiplier );

					if( dV < dMinY ) dMinY = dV;
					if( dV > dMaxY ) dMaxY = dV;

					pti++;
				}

				m_cGroup2.GetData().SetSeriesDisplay(g_lCustomVola, oc2dDisplayShow);
			}
			else
			{
				m_cGroup2.GetData().SetSeriesDisplay(g_lCustomVola, oc2dDisplayHide);

				EnableVolaManagement( false, false );
			}
		}

		long nPrevPoints = m_tsvPrev.size();
		if( nPrevPoints != 0 )
		{
			m_cGroup2.GetData().SetNumPoints( g_lCustomVolaPrev, nPrevPoints );

			int pti = 1;
			for( std::vector<CCustomTimePoint>::iterator itPrev = m_tsvPrev.begin(); itPrev != m_tsvPrev.end(); itPrev++, i++ )
			{
				dX = itPrev->m_nIndex * 30;

				double dV = itPrev->m_dVola;
				m_cGroup2.GetData().SetX(g_lCustomVolaPrev, pti, dX );
				m_cGroup2.GetData().SetY(g_lCustomVolaPrev, pti, dV * m_lVolaMultiplier );

				if( dV < dMinY ) dMinY = dV;
				if( dV > dMaxY ) dMaxY = dV;

				pti++;
			}

			m_cGroup2.GetData().SetSeriesDisplay(g_lCustomVolaPrev, oc2dDisplayShow);
		}
		else
		{
			m_cGroup2.GetData().SetSeriesDisplay(g_lCustomVolaPrev, oc2dDisplayHide);
		}
	}
	catch( const _com_error& )
	{
		m_tsv.clear();
		m_tsvPrev.clear();

//		strInfo += _T("Error getting custom volatility data. ") + e.Description();
	}
	catch( ... )
	{
		m_tsv.clear();
		m_tsvPrev.clear();

		strInfo += _T("Error drawing custom volatility data. ");
	}

#endif //_VOLA_MANAGER
	
	if (dMinY != DBL_MAX && dMaxY != -DBL_MAX  && dMinX != DBL_MAX && dMaxX != -DBL_MAX)
	{
		m_AxisX.GetMax().SetValue(dMaxX);
		m_AxisX.GetMin().SetValue(dMinX);
		
		AdjustBack (dMinX, dMaxX, dMinY, dMaxY);
	}
	
	//m_ctlChart.GetLegend().SetIsShowing(TRUE);

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
void CTimeSkewDlg::UpdateCustomVolatility( UINT nMessage, CCustomVolaNotification *pNotification )
{
	ASSERT( pNotification );

	if( pNotification->m_enType == CCustomVolaNotification::enPoint )
		return;

	m_bIsDirty = TRUE;

	if( pNotification->m_dtMonth == 0 )
	{
		if( nMessage == WM_CUSTOM_VOLA_SAVED || nMessage == WM_CUSTOM_VOLA_RESTORED )
			m_bIsDirty = FALSE;
	}
	else
	{
		UpdateChart( false );

		if( nMessage == WM_CUSTOM_VOLA_SAVED || nMessage == WM_CUSTOM_VOLA_RESTORED )
		{
			CTimeSkewValues::iterator it = m_tsv.begin();
			for( ; it != m_tsv.end(); it++ )
				if( it->m_dtMonth == pNotification->m_dtMonth )
					break;
			CTimeSkewValues::iterator pit = m_tsvPrev.begin();
			for( ; pit != m_tsvPrev.end(); pit++ )
				if( pit->m_dtMonth == pNotification->m_dtMonth )
					break;

			if( it != m_tsv.end() && pit != m_tsvPrev.end() )
				pit->m_dVola = it->m_dVola;
		}		
	}

	UpdateChart( false );
}
#endif //_VOLA_MANAGER


BOOL CTimeSkewDlg::OnInitDialog() 
{
	CChartDlg::OnInitDialog();
	
	m_strTitle.LoadString (IDS_TIME_SKEW_TITLE);
	
	return TRUE;
}


void CTimeSkewDlg::InterpolateCustomVolaToImplied()
{
#ifdef _VOLA_MANAGER

	time_skew_map tsdata;
	const time_skew_map& td = g_DataProcessor.GetTimeSkewData();
	time_skew_map::const_iterator tdi;
	
	{
		CDataProcessor::CLockVola lv;
		CDataProcessor::CLockTimeSkew lts;

		for( tdi = td.begin(); tdi != td.end(); tdi++ )
		{	
			const COptionData * pOpt = tdi->second.m_pOption;
			if( IsValidVola( pOpt->m_dVola ) && pOpt->m_bIsFitEnabled)
				tsdata.insert( time_skew_map::value_type( tdi->first, tdi->second ) );
		}
	}

	if( !tsdata.size() )
		return;

	// For each custom point
	for( CTimeSkewValues::iterator it = m_tsv.begin(); it != m_tsv.end(); it++ )
	{
		DATE dtTime = 0., dVola = 0.;

		time_skew_map::const_iterator tit = tsdata.lower_bound(static_cast<long>(it->m_dtMonth) );

		if( tit == tsdata.end() )
			tit--;
		
		dtTime = tit->first.date_;
		dVola  = tit->second.m_pOption->m_dVola;

		UpdateSkewPoint( *it, dVola );
	}

	PostNotification( WM_CUSTOM_VOLA_CHANGED, CCustomVolaNotification::enSkew, 0 );
#endif //_VOLA_MANAGER
}

	
bool CTimeSkewDlg::UpdateCustomVolaPoint( long lMovingPtIdx, double dVola, double& dtMonth )
{
#ifdef _VOLA_MANAGER
	dtMonth = m_nShifted ? 0 : (m_tsv[lMovingPtIdx-1].m_dtMonth);
	
	if( lMovingPtIdx > 0 )
		return UpdateSkewPoint( m_tsv[lMovingPtIdx-1], dVola );

#endif //_VOLA_MANAGER
	return false;
}


void CTimeSkewDlg::RestoreCustomVola() 
{
#ifdef _VOLA_MANAGER
	try
	{
		m_spSkew->Reload();
	}
	catch( const _com_error& e )
	{
		CString message;
		message.Format( _T("Error restoring custom volatility. (%X) "), e.Error() );
		message += e.Description().length() ? (LPCSTR)e.Description() : e.ErrorMessage();
		MessageBox( message, NULL, MB_OK|MB_ICONSTOP ); 
	}

	PostNotification( WM_CUSTOM_VOLA_RESTORED, CCustomVolaNotification::enSkew, 0 );
#endif //_VOLA_MANAGER
}


void CTimeSkewDlg::UpdateCustomVola() 
{
#ifdef _VOLA_MANAGER
	for( CTimeSkewValues::iterator it = m_tsv.begin(); it != m_tsv.end(); it++ )
		UpdateSkewPoint( *it, it->m_dVola );

	try
	{
		m_spSkew->Save();

		PostNotification( WM_CUSTOM_VOLA_SAVED, CCustomVolaNotification::enSkew, 0 );
	}
	catch( const _com_error& e )
	{
		MessageBox( e.Description(), NULL, MB_OK|MB_ICONERROR );
	}
#endif //_VOLA_MANAGER
}


bool CTimeSkewDlg::SetupVolaEditor( long lPtIdx, CVolaEditorDlg& ve )
{
#ifdef _VOLA_MANAGER
	ve.m_dtExpiration = (m_ptPrev.m_dtExpDate);
	ve.m_dPrice		  = g_DataProcessor.GetContractPrice();
	ve.m_bIsCall	  = true;

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


void CTimeSkewDlg::SavePointValue( long lMovingPtIdx )
{
#ifdef _VOLA_MANAGER
	m_ptPrev = m_tsv[lMovingPtIdx - 1];
#endif //_VOLA_MANAGER
}

double CTimeSkewDlg::GetPointValue( long lMovingPtIdx )
{
#ifdef _VOLA_MANAGER
	bool bVolaInPercent = true;
	double lVolaMultiplier = bVolaInPercent ? 100 : 1;

	return m_tsv[lMovingPtIdx - 1].m_dVola * lVolaMultiplier;
#endif //_VOLA_MANAGER
	return 0;
}

#ifdef _VOLA_MANAGER
void CTimeSkewDlg::ConstructSkew( const time_skew_map & tsdata )
{
	bool bVolaInPercent = true; //m_rMgr.GetShowVolatilityInPercents();
	m_lVolaMultiplier = bVolaInPercent ? 100L : 1L;

	CCustomTimePoint pt;

	COleDateTime dtMonth;
	COleDateTime dtExp;

	m_tsv.clear();
	for( int i = 0; i < m_spSkew->Count; i++ )
	{
		pt.m_dtMonth    = m_spSkew->ExpDate[i];
		pt.m_dVola      = m_spSkew->Volatility[pt.m_dtMonth]/m_lVolaMultiplier;
		pt.m_dStrike    = m_spSkew->Strike[pt.m_dtMonth];

		if( pt.m_dVola < 0 || pt.m_dStrike <= 0 )
			continue;

		CDataProcessor::CLockVola lv;
		CDataProcessor::CLockTimeSkew lts;

		dtMonth = pt.m_dtMonth;
		long nIndex = 0;
		for( time_skew_map::const_iterator it = tsdata.begin(); it != tsdata.end(); it++ )
		{
			dtExp = (DATE)it->first.date_;
			if( dtMonth.GetYear() == dtExp.GetYear() && dtMonth.GetMonth() == dtExp.GetMonth() )
			{
				pt.m_dtExpDate = dtExp;
				pt.m_nIndex    = nIndex;

				m_tsv.push_back( pt );
				
				break;
			}
			nIndex++;
		}
	}

	std::sort( m_tsv.begin(), m_tsv.end() );
}


bool CTimeSkewDlg::UpdateSkewPoint( CCustomTimePoint& pt, double dVola )
{
	bool bVolaInPercent = true; //m_rMgr.GetShowVolatilityInPercents();
	m_lVolaMultiplier = bVolaInPercent ? 100L : 1L;

	double dVolaDelta = dVola - pt.m_dVola;
	if( dVolaDelta != 0 )
	{
		pt.m_dVola = dVola;
		m_spSkew->VolatilityDelta[pt.m_dtMonth] = dVolaDelta * m_lVolaMultiplier;

		return true;
	}

	return false;
}
#endif //_VOLA_MANAGER

CString CTimeSkewDlg::GetXLabel (long lSeries, size_t lPtIdx)
{
	CString strStrike = _T("");

#ifdef _VOLA_MANAGER
	if( lPtIdx > 0 && lPtIdx <= m_tsv.size() && lSeries == g_lCustomVola )
		strStrike.Format( _T("\nS: %.2f"), m_tsv[lPtIdx-1].m_dStrike );
#endif //_VOLA_MANAGER

	return strStrike;
}
