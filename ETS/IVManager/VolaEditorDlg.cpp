// VolaEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "VolaEditorDlg.h"

#include "DataProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define VOLA_FRACTIONALDIGITCOUNT		4
#define VOLA_PERC_FRACTIONALDIGITCOUNT	2
#define VOLA_DECIMALDIGITCOUNT			1
#define VOLA_PERC_DECIMALDIGITCOUNT		3

#define STRIKE_FRACTIONALDIGITCOUNT		2
#define STRIKE_DECIMALDIGITCOUNT		4
#define PRICE_FRACTIONALDIGITCOUNT		2
#define PRICE_DECIMALDIGITCOUNT			4

#define VOLA_POSITIVE_FORMAT			_T("1.1")
#define VOLA_PERC_POSITIVE_FORMAT		_T("1.1%")
#define VOLA_NEGATIVE_FORMAT			_T("-1.1")
#define VOLA_PERC_NEGATIVE_FORMAT		_T("-1.1%")

#define MONEY_POSITIVE_FORMAT			_T("1.1")
#define MONEY_NEGATIVE_FORMAT			_T("-1.1")

/////////////////////////////////////////////////////////////////////////////
// CVolaEditorDlg dialog


CVolaEditorDlg::CVolaEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVolaEditorDlg::IDD, pParent), m_nTrackChangesSemaphor(1)
{
	//{{AFX_DATA_INIT(CVolaEditorDlg)
	m_dCurPrice = 0.0;
	m_dCurVola = -1;
	m_dNewPrice = 0.0;
	m_dNewVola = -1;
	m_dCurStrike = 0.0;
	m_dNewStrike = 0.0;
	//}}AFX_DATA_INIT
}


void CVolaEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolaEditorDlg)
	DDX_Control(pDX, IDC_NEW_VOLA, m_edNewVola);
	DDX_Control(pDX, IDC_NEW_STRIKE, m_edNewStrike);
	DDX_Control(pDX, IDC_NEW_PRICE, m_edNewPrice);
	DDX_Control(pDX, IDC_CUR_VOLA, m_edCurVola);
	DDX_Control(pDX, IDC_CUR_STRIKE, m_edCurStrike);
	DDX_Control(pDX, IDC_CUR_PRICE, m_edCurPrice);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolaEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CVolaEditorDlg)
	ON_EN_CHANGE(IDC_NEW_VOLA, OnChangeNewVola)
	ON_EN_CHANGE(IDC_NEW_PRICE, OnChangeNewPrice)
	//}}AFX_MSG_MAP
	ON_CONTROL(EN_EX_COMMIT,  IDC_NEW_VOLA,  OnCommitNewVola )
	ON_CONTROL(EN_EX_DISCARD, IDC_NEW_VOLA,  OnDiscardNewVola )
	ON_CONTROL(EN_EX_COMMIT,  IDC_NEW_PRICE, OnCommitNewPrice )
	ON_CONTROL(EN_EX_DISCARD, IDC_NEW_VOLA,  OnDiscardNewVola )
END_MESSAGE_MAP()


void CVolaEditorDlg::SetCurVola( double dVola )
{
	m_dCurVola = dVola;
}


void CVolaEditorDlg::SetNewVola( double dVola )
{
	m_dNewVola = dVola;
}


void CVolaEditorDlg::SetCurStrike( double dStrike )
{
	m_dCurStrike = dStrike;
}


void CVolaEditorDlg::SetNewStrike( double dStrike )
{
	m_dNewStrike = dStrike;
}


void CVolaEditorDlg::CalcCurPrice()
{
	bool bVolaInPercent = true;
	double lVolaMultiplier = bVolaInPercent ? 100 : 1;

	COptionData opt;
	opt.m_dtExpiration = m_dtExpiration;
	opt.m_bIsCall	   = m_bIsCall;
	opt.m_dStrike	   = m_dCurStrike;
	opt.m_dVola		   = m_dCurVola/lVolaMultiplier;
	
	g_DataProcessor.CalcOptPrice( opt );

	m_dCurPrice = opt.m_dPrice > 0 ? opt.m_dPrice : 0;
	m_edCurPrice.SetValue( m_dCurPrice );
}


void CVolaEditorDlg::CalcNewPrice()
{
	bool bVolaInPercent = true;
	double lVolaMultiplier = bVolaInPercent ? 100 : 1;

	COptionData opt;
	opt.m_dtExpiration = m_dtExpiration;
	opt.m_bIsCall	   = m_bIsCall;
	opt.m_dStrike	   = m_dNewStrike;
	opt.m_dVola		   = m_edNewVola.GetValue()/lVolaMultiplier;

	g_DataProcessor.CalcOptPrice( opt );

	m_dNewPrice = opt.m_dPrice > 0 ? opt.m_dPrice : 0;
	m_edNewPrice.SetValue( m_dNewPrice );
}


void CVolaEditorDlg::CalcNewVola()
{
	bool bVolaInPercent = true;
	double lVolaMultiplier = bVolaInPercent ? 100 : 1;

	COptionData opt;
	opt.m_dtExpiration = m_dtExpiration;
	opt.m_bIsCall	   = m_bIsCall;
	opt.m_dStrike	   = m_dNewStrike;
	opt.m_dPrice	   = m_edNewPrice.GetValue();

	g_DataProcessor.CalcOptIVola( opt );

	m_dNewVola = opt.m_dVola > 0 ? opt.m_dVola : 0;
	m_edNewVola.SetValue( m_dNewVola * lVolaMultiplier );
}


/////////////////////////////////////////////////////////////////////////////
// CVolaEditorDlg message handlers


BOOL CVolaEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	bool bVolaInPercent = true;
	int nVolaFracDigCnt = bVolaInPercent ? VOLA_PERC_FRACTIONALDIGITCOUNT : VOLA_FRACTIONALDIGITCOUNT;
	int nVolaDecDigCnt = bVolaInPercent  ? VOLA_PERC_DECIMALDIGITCOUNT    : VOLA_DECIMALDIGITCOUNT;
	CString strPosFormat(bVolaInPercent  ? VOLA_PERC_POSITIVE_FORMAT	  : VOLA_POSITIVE_FORMAT);
	CString strNegFormat(bVolaInPercent  ? VOLA_PERC_NEGATIVE_FORMAT	  : VOLA_NEGATIVE_FORMAT);
	
//	m_edNewVola.SetFlat(TRUE);
	m_edNewVola.SetForceToUsePeriod( FALSE );
	m_edNewVola.SetPositiveFormat(strPosFormat);
	m_edNewVola.SetNegativeFormat(strNegFormat);
	m_edNewVola.SetFractionalDigitCount(nVolaFracDigCnt);
	m_edNewVola.SetDecimalDigitCount(nVolaDecDigCnt);

//	m_edNewStrike.SetFlat(TRUE);
	m_edNewStrike.SetForceToUsePeriod( FALSE );
	m_edNewStrike.SetPositiveFormat(MONEY_POSITIVE_FORMAT);
	m_edNewStrike.SetNegativeFormat(MONEY_NEGATIVE_FORMAT);
	m_edNewStrike.SetFractionalDigitCount(STRIKE_FRACTIONALDIGITCOUNT);
	m_edNewStrike.SetDecimalDigitCount(STRIKE_DECIMALDIGITCOUNT);

//	m_edNewPrice.SetFlat(TRUE);
	m_edNewPrice.SetForceToUsePeriod( FALSE );
	m_edNewPrice.SetPositiveFormat(MONEY_POSITIVE_FORMAT);
	m_edNewPrice.SetNegativeFormat(MONEY_NEGATIVE_FORMAT);
	m_edNewPrice.SetFractionalDigitCount(PRICE_FRACTIONALDIGITCOUNT);
	m_edNewPrice.SetDecimalDigitCount(PRICE_DECIMALDIGITCOUNT);

//	m_edCurVola.SetFlat(TRUE);
	m_edCurVola.SetForceToUsePeriod( FALSE );
	m_edCurVola.SetPositiveFormat(strPosFormat);
	m_edCurVola.SetNegativeFormat(strNegFormat);
	m_edCurVola.SetFractionalDigitCount(nVolaFracDigCnt);
	m_edCurVola.SetDecimalDigitCount(nVolaDecDigCnt);

//	m_edCurStrike.SetFlat(TRUE);
	m_edCurStrike.SetForceToUsePeriod( FALSE );
	m_edCurStrike.SetPositiveFormat(MONEY_POSITIVE_FORMAT);
	m_edCurStrike.SetNegativeFormat(MONEY_NEGATIVE_FORMAT);
	m_edCurStrike.SetFractionalDigitCount(STRIKE_FRACTIONALDIGITCOUNT);
	m_edCurStrike.SetDecimalDigitCount(STRIKE_DECIMALDIGITCOUNT);
	
//	m_edCurPrice.SetFlat(TRUE);
	m_edCurPrice.SetForceToUsePeriod( FALSE );
	m_edCurPrice.SetPositiveFormat(MONEY_POSITIVE_FORMAT);
	m_edCurPrice.SetNegativeFormat(MONEY_NEGATIVE_FORMAT);
	m_edCurPrice.SetFractionalDigitCount(PRICE_FRACTIONALDIGITCOUNT);
	m_edCurPrice.SetDecimalDigitCount(PRICE_DECIMALDIGITCOUNT);

	m_edCurVola.SetValue( m_dCurVola );
	m_edNewVola.SetValue( m_dNewVola );
	m_edCurStrike.SetValue( m_dCurStrike );
	m_edNewStrike.SetValue( m_dNewStrike );

	CalcCurPrice();
	CalcNewPrice();

	m_nTrackChangesSemaphor = 0;	// Start track new vola an dprice changes

	return TRUE;
}


void CVolaEditorDlg::OnChangeNewVola() 
{
	if( m_nTrackChangesSemaphor == 0 )
		{
		m_nTrackChangesSemaphor++;

		CalcNewPrice();
		
		m_nTrackChangesSemaphor--;
		}
}


void CVolaEditorDlg::OnCommitNewVola()
{
	OnOK();
}


void CVolaEditorDlg::OnDiscardNewVola()
{
	OnCancel();
}


void CVolaEditorDlg::OnChangeNewPrice() 
{
	if( m_nTrackChangesSemaphor == 0 )
		{
		m_nTrackChangesSemaphor++;

		CalcNewVola();
		
		m_nTrackChangesSemaphor--;
		}
}


void CVolaEditorDlg::OnCommitNewPrice()
{
	OnOK();
}


void CVolaEditorDlg::OnDiscardNewPrice()
{
	OnCancel();
}


void CVolaEditorDlg::OnOK() 
{
	m_dNewVola = m_edNewVola.GetValue();
	
	CDialog::OnOK();
}
