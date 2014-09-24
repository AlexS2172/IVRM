// EodSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EtsEodManager.h"
#include "EodSettingsDlg.h"

// CEodSettingsDlg dialog
IMPLEMENT_DYNAMIC(CEodSettingsDlg, CDialog)
CEodSettingsDlg::CEodSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEodSettingsDlg::IDD, pParent)
	, m_nRunPeriodically(0)
	, m_dtStartTime(COleDateTime::GetCurrentTime())
	, m_nCalcModel(0)
	, m_nStepFuturePrice(0)
	, m_nLogLevel(0)
	, m_nLogHistory(0)
	, m_nStepMktStructure(0)
	//, m_nStepParamMktStructureCheck(0)
	, m_nStepParamMktStructureActiveOnly(0)
	, m_nStepParamMktStructureDeleteOldOpt(0)
	, m_nStepAvgVolume(0)
	, m_nStepIVBand(0)
	, m_nStepSkewKurt(0)
	, m_nStepOptClosePrice(0)
	, m_nStepParamOptClosePriceIV(0)
	, m_nStepParamOptClosePricePP(0)
	, m_nStepUndClosePrice(0)
	, m_nStepParamUndClosePriceIV(0)
	, m_nStepParamUndClosePricePP(0)
	, m_nStepCalcWtdVega(0)
	, m_bUpdateOptionsPricesInMarketStructureUpdate(FALSE)
	, m_bFilterDividends(TRUE)
	, m_bUpdateUnderlyingPricesInMarketStructureUpdate(FALSE)
	, m_bOptionsLoad(TRUE)
	, m_nStepFuture(0)
{
}

CEodSettingsDlg::~CEodSettingsDlg()
{
}

void CEodSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_RUN_PERIODICALLY, m_nRunPeriodically);
	DDX_DateTimeCtrl(pDX, IDC_DT_START_TIME, m_dtStartTime);
	DDX_CBIndex(pDX, IDC_COMBO_CALC_MODEL, m_nCalcModel);
	DDX_CBIndex(pDX, IDC_STEP_IDXFUT_PRICES_COMBO, m_nStepFuturePrice);
	DDX_CBIndex(pDX, IDC_COMBO_LOG_LEVELS, m_nLogLevel);
	DDX_Text(pDX, IDC_EDIT_LOG_HISTORY, m_nLogHistory);
	DDV_MinMaxInt(pDX, m_nLogHistory, 0, 100);

	DDX_Check(pDX, IDC_STEP_MKT_STRUCTURE, m_nStepMktStructure);
	DDX_Check(pDX, IDC_STEP_IDX_FUTURE, m_nStepFuture);
	//DDX_Check(pDX, IDC_PARAM_MKT_STRUCTURE_CHECK, m_nStepParamMktStructureCheck);
	DDX_Check(pDX, IDC_PARAM_MKT_STRUCTURE_ACTIVE_ONLY, m_nStepParamMktStructureActiveOnly);
	DDX_Check(pDX, IDC_PARAM_MKT_STRUCTURE_DEL_OLD_OPT, m_nStepParamMktStructureDeleteOldOpt);
	DDX_Check(pDX, IDC_STEP_AVG_VOLUME, m_nStepAvgVolume);
	DDX_Check(pDX, IDC_STEP_IV_BAND, m_nStepIVBand);
	DDX_Check(pDX, IDC_STEP_SKEW_KURT, m_nStepSkewKurt);
	DDX_Check(pDX, IDC_STEP_OPT_CLOSE_PRICE, m_nStepOptClosePrice);
	DDX_Check(pDX, IDC_PARAM_OPT_CLOSE_PRICE_IV, m_nStepParamOptClosePriceIV);
	DDX_Check(pDX, IDC_PARAM_OPT_CLOSE_PRICE_PP, m_nStepParamOptClosePricePP);
	DDX_Check(pDX, IDC_STEP_UND_CLOSE_PRICE, m_nStepUndClosePrice);
	DDX_Check(pDX, IDC_PARAM_UND_CLOSE_PRICE_IV, m_nStepParamUndClosePriceIV);
	DDX_Check(pDX, IDC_PARAM_UND_CLOSE_PRICE_PP, m_nStepParamUndClosePricePP);
	DDX_Check(pDX, IDC_STEP_CALC_WTD_VEGA, m_nStepCalcWtdVega);
	DDX_Check(pDX, IDC_CHK_UPDATEOPTIONS, m_bUpdateOptionsPricesInMarketStructureUpdate);
	DDX_Check(pDX, IDC_CHK_UPDATEUND, m_bUpdateUnderlyingPricesInMarketStructureUpdate);
	DDX_Check(pDX, IDC_CHK_LOADOPTIONS, m_bOptionsLoad); 
	DDX_Check(pDX, IDC_CHK_FILTERDIVIDENDS, m_bFilterDividends);
}


BEGIN_MESSAGE_MAP(CEodSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_RUN_PERIODICALLY, OnRunPeriodicallyBnClick)
	ON_BN_CLICKED(IDC_STEP_IDX_FUTURE, OnStepIdxFutureBnClick)
	ON_BN_CLICKED(IDC_STEP_MKT_STRUCTURE, OnStepMktStructureBnClick)
	ON_BN_CLICKED(IDC_STEP_OPT_CLOSE_PRICE, OnStepOptClosePriceBnClick)
	ON_BN_CLICKED(IDC_STEP_UND_CLOSE_PRICE, OnStepUndClosePriceBnClick)
	ON_BN_CLICKED(IDC_CHK_LOADOPTIONS, OnStepLoadOptionsClicked)
END_MESSAGE_MAP()

BOOL CEodSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnRunPeriodicallyBnClick();
	OnStepMktStructureBnClick();
	OnStepOptClosePriceBnClick();
	OnStepUndClosePriceBnClick();


	GetDlgItem(IDC_DT_START_TIME)->ModifyStyle(0, DTS_TIMEFORMAT);

	return TRUE;
}

long CEodSettingsDlg::GetSteps()
{
	long nSteps = 0L;
	nSteps |= m_nStepMktStructure ? enSmLoadMktStructure : 0;
//	nSteps |= m_nStepParamMktStructureCheck ? enSmpCheckMktStrUpdates : 0;
	nSteps |= m_nStepParamMktStructureActiveOnly ? enSmpLoadOnlyActiveUnd : 0;
	nSteps |= m_nStepParamMktStructureDeleteOldOpt ? enSmpDeleteOldOptions : 0;
	nSteps |= m_nStepAvgVolume ? enSmLoadAvgVolumes : 0;
	nSteps |= m_nStepIVBand ? enSmLoadIVBands : 0;
	nSteps |= m_nStepSkewKurt ? enSmLoadSkewKurtosis : 0;
	//nSteps |= m_nStepOptClosePrice ? enSmLoadOptClosePrice : 0;
	nSteps |= m_nStepParamOptClosePriceIV ? enSmpOptLoadFromIV : 0;
	//nSteps |= m_nStepUndClosePrice ? enSmLoadUndClosePrice : 0;
	nSteps |= m_nStepParamUndClosePriceIV ? enSmpUndLoadFromIV : 0;
	nSteps |= m_nStepCalcWtdVega ? enSmCalculateVega : 0;

	nSteps |= m_bUpdateUnderlyingPricesInMarketStructureUpdate? enSmpMarketStructureUndLoadClosePrices :0;
	nSteps |= m_bUpdateOptionsPricesInMarketStructureUpdate ? enSmpMarketStructureOptLoadClosePrices:0;
	nSteps |= m_bFilterDividends?enSmpMarketStructureFilterWrongDividends:0;
	nSteps |= m_bOptionsLoad?enSmpMarketStructureOptionsLoad:0;
	
	nSteps |= m_nStepFuture ? enSmpLoadFutures : 0;


	switch(m_nStepFuturePrice){
		case 1:
			nSteps |= enSmpLoadFuturesPrice;
			break;
		case 2:
			nSteps |= enSmpLoadFuturesLastPrice;
			break;
	}
	
	return nSteps;
}

void CEodSettingsDlg::SetSteps(long nSteps)
{
	m_nStepMktStructure = (nSteps & enSmLoadMktStructure) ? 1 : 0;
//	m_nStepParamMktStructureCheck = (nSteps & enSmpCheckMktStrUpdates) ? 1 : 0;
	m_nStepParamMktStructureActiveOnly = (nSteps & enSmpLoadOnlyActiveUnd) ? 1 : 0;
	m_nStepParamMktStructureDeleteOldOpt = (nSteps & enSmpDeleteOldOptions) ? 1 : 0;
	m_nStepAvgVolume = (nSteps & enSmLoadAvgVolumes) ? 1 : 0;
	m_nStepIVBand = (nSteps & enSmLoadIVBands) ? 1 : 0;
	m_nStepSkewKurt = (nSteps & enSmLoadSkewKurtosis) ? 1 : 0;
	m_nStepOptClosePrice = (nSteps & enSmLoadOptClosePrice) ? 1 : 0;
	m_nStepParamOptClosePriceIV = (nSteps & enSmpOptLoadFromIV) ? 1 : 0;
	m_nStepParamOptClosePricePP = (nSteps & enSmpOptLoadFromIV) ? 0 : 1;
	m_nStepUndClosePrice = (nSteps & enSmLoadUndClosePrice) ? 1 : 0;
	m_nStepParamUndClosePriceIV = (nSteps & enSmpUndLoadFromIV) ? 1 : 0;
	m_nStepParamUndClosePricePP = (nSteps & enSmpUndLoadFromIV) ? 0 : 1;
	m_nStepCalcWtdVega = (nSteps & enSmCalculateVega) ? 1 : 0;

	m_bUpdateUnderlyingPricesInMarketStructureUpdate = (nSteps& enSmpMarketStructureUndLoadClosePrices)?TRUE:FALSE;
	m_bUpdateOptionsPricesInMarketStructureUpdate = (nSteps& enSmpMarketStructureOptLoadClosePrices)?TRUE:FALSE;
	m_bFilterDividends =(nSteps& enSmpMarketStructureFilterWrongDividends)?TRUE:FALSE;
	m_bOptionsLoad =(nSteps& enSmpMarketStructureOptionsLoad)?TRUE:FALSE;

	m_nStepFuture = (nSteps & enSmpLoadFutures) ? 1 : 0;

	if((nSteps & enSmpLoadFuturesPrice) == enSmpLoadFuturesPrice)
		m_nStepFuturePrice = 1;
	else if((nSteps & enSmpLoadFuturesLastPrice) == enSmpLoadFuturesLastPrice)
		m_nStepFuturePrice = 2;

}


void CEodSettingsDlg::OnRunPeriodicallyBnClick()
{
	BOOL bEnabled = _ControlChecked[IDC_RUN_PERIODICALLY];
	_ControlEnabled[IDC_DT_START_TIME] = bEnabled;
}


void CEodSettingsDlg::OnStepIdxFutureBnClick()
{
	BOOL bEnabled = _ControlChecked[IDC_STEP_IDX_FUTURE];
	_ControlEnabled[IDC_STEP_IDXFUT_PRICES_COMBO] = bEnabled;
}


void CEodSettingsDlg::OnStepMktStructureBnClick()
{
	BOOL bEnabled = _ControlChecked[IDC_STEP_MKT_STRUCTURE];

	_ControlEnabled[IDC_PARAM_MKT_STRUCTURE_CHECK] = bEnabled;
	_ControlEnabled[IDC_PARAM_MKT_STRUCTURE_ACTIVE_ONLY] = bEnabled;
	_ControlEnabled[IDC_PARAM_MKT_STRUCTURE_DEL_OLD_OPT] = bEnabled;
	_ControlEnabled[IDC_CHK_LOADOPTIONS] = bEnabled;
	_ControlEnabled[IDC_CHK_UPDATEOPTIONS] = bEnabled && _ControlChecked[IDC_CHK_LOADOPTIONS];
	_ControlEnabled[IDC_CHK_FILTERDIVIDENDS] = bEnabled;
	_ControlEnabled[IDC_CHK_UPDATEUND] = bEnabled;
	_ControlEnabled[IDC_STEP_IDX_FUTURE]= bEnabled;
	_ControlEnabled[IDC_STEP_IDXFUT_PRICES_COMBO]= bEnabled && _ControlChecked[IDC_STEP_IDX_FUTURE];
}

void CEodSettingsDlg::OnStepLoadOptionsClicked()
{
	_ControlEnabled[IDC_CHK_UPDATEOPTIONS] = _ControlChecked[IDC_STEP_MKT_STRUCTURE]&&_ControlChecked[IDC_CHK_LOADOPTIONS];
}


void CEodSettingsDlg::OnStepOptClosePriceBnClick()
{
	_ControlEnabled[IDC_PARAM_OPT_CLOSE_PRICE_IV] = _ControlChecked[IDC_STEP_OPT_CLOSE_PRICE];
	_ControlEnabled[IDC_PARAM_OPT_CLOSE_PRICE_PP] = _ControlChecked[IDC_STEP_OPT_CLOSE_PRICE];
}

void CEodSettingsDlg::OnStepUndClosePriceBnClick()
{
	_ControlEnabled[IDC_PARAM_UND_CLOSE_PRICE_IV] = _ControlChecked[IDC_STEP_UND_CLOSE_PRICE];
	_ControlEnabled[IDC_PARAM_UND_CLOSE_PRICE_PP] = _ControlChecked[IDC_STEP_UND_CLOSE_PRICE];
}
