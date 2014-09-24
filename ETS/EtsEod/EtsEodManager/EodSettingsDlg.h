#pragma once
#include "atlcomtime.h"
#include "afxwin.h"


// CEodSettingsDlg dialog

class CEodSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CEodSettingsDlg)

public:
	CEodSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEodSettingsDlg();

// Dialog Data
	enum DlgResEnum { IDD = IDD_EODSETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStepLoadOptionsClicked();
	afx_msg void OnRunPeriodicallyBnClick();
	afx_msg void OnStepMktStructureBnClick();
	afx_msg void OnStepOptClosePriceBnClick();
	afx_msg void OnStepUndClosePriceBnClick();

	afx_msg void OnStepIdxFutureBnClick();

public:
	virtual BOOL OnInitDialog();

public:
	long GetSteps();
	void SetSteps(long nSteps);

private:
	__declspec(property(get = _ControlEnabledGet, put = _ControlEnabledSet)) BOOL _ControlEnabled[];
		BOOL _ControlEnabledSet(int nID)
		{
			return GetDlgItem(nID)->IsWindowEnabled();
		}
		void _ControlEnabledSet(int nID, BOOL bEnable)
		{
			GetDlgItem(nID)->EnableWindow(bEnable);
		}

	__declspec(property(get = _ControlCheckedGet, put = _ControlCheckedSet)) BOOL _ControlChecked[];
		BOOL _ControlCheckedGet(int nID)
		{
			return (GetDlgItem(nID)->SendMessage(BM_GETCHECK, 0, 0) != BST_UNCHECKED);
		}
		void _ControlCheckedSet(int nID, BOOL bChecked)
		{
			GetDlgItem(nID)->SendMessage(BM_SETCHECK, bChecked ? BST_CHECKED : BST_UNCHECKED);
		}

public:
	int				m_nRunPeriodically;
	COleDateTime	m_dtStartTime;

	__declspec(property(get = LogLevelGet, put = LogLevelSet)) int LogLevel;
	int LogLevelGet()
	{
		return m_nLogLevel;
	}
	void LogLevelSet(int nLogLevel)
	{
		m_nLogLevel = nLogLevel;
	}

	__declspec(property(get = LogHistoryGet, put = LogHistorySet)) int LogHistory;
	int LogHistoryGet()
	{
		return m_nLogHistory;
	}
	void LogHistorySet(int nLogHistory)
	{
		m_nLogHistory = nLogHistory;
	}

	__declspec(property(get = CalcModelGet, put = CalcModelSet)) EodCalcModelTypeEnum CalcModel;
		EodCalcModelTypeEnum CalcModelGet()
		{
			if(1 == m_nCalcModel)
				return enEodCmBinomialOpt;
			else if(2 == m_nCalcModel)
				return enEodCmVskLog;

			return enEodCmBinomial;
		}
		void CalcModelSet(EodCalcModelTypeEnum enCalcModel)
		{
			if(enEodCmBinomialOpt == enCalcModel)
				m_nCalcModel = 1;
			else if(enEodCmVskLog == enCalcModel)
				m_nCalcModel = 2;
			else
				m_nCalcModel = 0;
		}

protected:
	int				m_nStepFuturePrice;
	int				m_nCalcModel;
	int				m_nLogLevel;
	int				m_nLogHistory;
	int				m_nStepMktStructure;
	int				m_nStepParamMktStructureActiveOnly;
	int				m_nStepParamMktStructureDeleteOldOpt;
	int				m_nStepAvgVolume;
	int				m_nStepIVBand;
	int				m_nStepSkewKurt;
	int				m_nStepOptClosePrice;
	int				m_nStepParamOptClosePriceIV;
	int				m_nStepParamOptClosePricePP;
	int				m_nStepUndClosePrice;
	int				m_nStepParamUndClosePriceIV;
	int				m_nStepParamUndClosePricePP;
	int				m_nStepCalcWtdVega;
	int				m_nStepFuture;

public:
	BOOL m_bUpdateOptionsPricesInMarketStructureUpdate;
	BOOL m_bUpdateUnderlyingPricesInMarketStructureUpdate;
	BOOL m_bFilterDividends;
	BOOL m_bOptionsLoad;
};
