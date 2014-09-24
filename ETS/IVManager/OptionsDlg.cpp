// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivanalyzer.h"
#include "OptionsDlg.h"

#define DBINITCONSTANTS
#include <initguid.h>
///#include <sqloledb.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_strDisplayConnection = _T("");
	//}}AFX_DATA_INIT
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

#ifndef _VOLA_MANAGER
	if (! pDX->m_bSaveAndValidate)
	{
#ifndef _IVAN_DB_ACCESS 
		if (m_opts.m_strConnection.IsEmpty() )
			m_strDisplayConnection.Empty();
		else
		{
			IDataInitialize * pDataInit =  GetDataInit ();
			if (pDataInit)
			{
				HRESULT hr = pDataInit->GetDataSource (NULL, CLSCTX_INPROC_SERVER, _bstr_t(m_opts.m_strConnection),
					IID_IDBInitialize, (IUnknown ** )  &m_spDBInitialize );
				if (SUCCEEDED (hr) )
				{
					LPOLESTR szConnect=NULL;
					hr = pDataInit->GetInitializationString (m_spDBInitialize, FALSE, &szConnect);
					if (SUCCEEDED (hr))
					{
						m_strDisplayConnection = szConnect;
						::CoTaskMemFree (szConnect);
					}
				}
			}
		}
#endif

	}

	DDX_Text(pDX, IDC_IV_PROXY_ACCOUNT, m_opts.m_IVOpts.m_strProxyLogin);
	DDX_Text(pDX, IDC_IV_PROXY_PASSWORD, m_opts.m_IVOpts.m_strProxyPassword);
	DDX_Check(pDX, IDC_CHECK_PROXYAUTH, m_opts.m_IVOpts.m_iProxyAuth);

	DDX_Text(pDX, IDC_PRICE_ACCOUNT, m_opts.m_PriceProvOpts.m_strAccount);
	DDX_Text(pDX, IDC_PRICE_PASSWORD, m_opts.m_PriceProvOpts.m_strPassword);
	
	DDX_Text (pDX, IDC_IV_ACCOUNT, m_opts.m_IVOpts.m_strAccount);
	DDX_Text (pDX, IDC_IV_PASSWORD, m_opts.m_IVOpts.m_strPassword);
		
	DDX_Text (pDX, IDC_PROXY_ADDR, m_opts.m_PriceProvOpts.m_strProxy);
	DDX_Text (pDX, IDC_PROXY_ADDR2, m_opts.m_IVOpts.m_strProxy);
	DDX_Radio(pDX, IDC_CALL, (int&) m_opts.m_enCurveMode);
	DDX_Radio(pDX, IDC_MODEL_BINOMINAL,(int&) m_opts.m_enCalculationMode);

	
	DDX_Check(pDX, IDC_FLAT_OUT_RANGE, m_iFlatOutRange);
	DDX_Check(pDX, IDC_FLAT_WHEN_NO_BID, m_iFlatNoBid);
#endif
	DDX_Radio(pDX, IDC_CHART_ALL, m_iChartMode);


	bool bDoExchangeIV = false;
	bool bDoExchangePrice = false;
	bool bChartExchangeFrom = false;
	bool bChartExchangeTo = false;

	if (!pDX->m_bSaveAndValidate)	
	{
#ifndef _VOLA_MANAGER
		bDoExchangeIV = m_opts.m_IVOpts.m_nPort !=0;
		bDoExchangePrice = m_opts.m_PriceProvOpts.m_iProxyPort !=0;
#endif
		bChartExchangeFrom = !fEQZero(m_opts.m_fChartRangeFrom);
		bChartExchangeTo = !fEQZero(m_opts.m_fChartRangeTo);
	}
	else
	{
		CString m_strText;
#ifndef _VOLA_MANAGER
		m_edProxyIV.GetWindowText(m_strText);
		m_strText.TrimRight();
		bDoExchangeIV = ! m_strText.IsEmpty();
		m_opts.m_IVOpts.m_nPort =0;

		m_edProxyPrice.GetWindowText(m_strText);
		m_strText.TrimRight();
		bDoExchangePrice = ! m_strText.IsEmpty();
		m_opts.m_PriceProvOpts.m_iProxyPort =0;
#endif
		m_edChatrModeFrom.GetWindowText(m_strText);
		m_strText.TrimRight();
		bChartExchangeFrom = !m_strText.IsEmpty();

		m_edChatrModeTo.GetWindowText(m_strText);
		m_strText.TrimRight();
		bChartExchangeTo = !m_strText.IsEmpty();

		m_opts.m_fChartRangeFrom = 0.;
		m_opts.m_fChartRangeTo = 0.;

	}
	
#ifndef _VOLA_MANAGER
	if (bDoExchangePrice)
		DDX_Text (pDX, IDC_PROXY_PORT, m_opts.m_PriceProvOpts.m_iProxyPort);
	if (bDoExchangeIV)
		DDX_Text (pDX, IDC_PROXY_PORT2, m_opts.m_IVOpts.m_nPort);
	DDX_Check (pDX, IDC_USE_PROXY, m_opts.m_PriceProvOpts.m_iUseProxy);
	DDX_Check (pDX, IDC_USE_PROXY2, m_opts.m_IVOpts.m_nUseProxy);

	if (pDX->m_bSaveAndValidate)
	{
		m_opts.m_strConnection.TrimRight();
		m_opts.m_IVOpts.m_strAccount.TrimRight();
		m_opts.m_IVOpts.m_strPassword.TrimRight();
		m_opts.m_IVOpts.m_strURL.TrimRight();
		m_opts.m_IVOpts.m_strProxy.TrimRight();


		if ( m_opts.m_IVOpts.m_nUseProxy == 1 && 
			( m_opts.m_IVOpts.m_strProxy.IsEmpty()  || m_opts.m_IVOpts.m_nPort==0) )
		{
			MessageBox (  CString ( (LPCSTR) ERR_IDS_NO_PROXY_SETTINGS) );
			pDX->Fail();
		}

		m_opts.m_PriceProvOpts.m_strAccount.TrimRight();
		m_opts.m_PriceProvOpts.m_strPassword.TrimRight();
		m_opts.m_PriceProvOpts.m_strProxy.TrimRight();

		if ( m_opts.m_PriceProvOpts.m_iUseProxy == 1 && 
			( m_opts.m_PriceProvOpts.m_strProxy.IsEmpty()  || m_opts.m_PriceProvOpts.m_iProxyPort==0) )
		{
			MessageBox (  CString ( (LPCSTR) ERR_IDS_NO_PROXY_SETTINGS) );
			pDX->Fail();
		}

	}

#endif
	DDX_Check(pDX, IDC_CHECK_RANGE, m_iRangeCheck);

	if(bChartExchangeTo)
		DDX_Text (pDX, IDC_CHATR_TO, m_opts.m_fChartRangeTo);

	if(bChartExchangeFrom)
		DDX_Text (pDX, IDC_CHATR_FROM, m_opts.m_fChartRangeFrom);
/*
	if(m_opts.m_fChartRangeTo<0.) m_opts.m_fChartRangeTo = 0.;
	if(m_opts.m_fChartRangeFrom<0.) m_opts.m_fChartRangeFrom = 0.;
	if(m_opts.m_fChartRangeTo<=m_opts.m_fChartRangeFrom) m_opts.m_fChartRangeTo =m_opts.m_fChartRangeFrom+100;
*/	
#ifndef _VOLA_MANAGER

	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_CHECK_PROXYAUTH, m_btProxyAuth);
	DDX_Control(pDX, IDC_IV_PROXY_ACCOUNT, m_edProxyUserID);
	DDX_Control(pDX, IDC_IV_PROXY_PASSWORD, m_edProxyUserPass);

	DDX_Control(pDX, IDC_USE_PROXY, m_btUseProxyPrice);
	DDX_Control(pDX, IDC_PROXY_PORT, m_edPortPrice);
	DDX_Control(pDX, IDC_PROXY_ADDR, m_edProxyPrice);

	DDX_Control(pDX, IDC_USE_PROXY2, m_btUseProxyIV);
	DDX_Control(pDX, IDC_PROXY_PORT2, m_edPortIV);
	DDX_Control(pDX, IDC_PROXY_ADDR2, m_edProxyIV);


	DDX_Control(pDX, IDC_PROVIDERTYPE, m_listProviders);

#ifndef _IVAN_DB_ACCESS
	DDX_Text(pDX, IDC_CONNECTION, m_strDisplayConnection);
#endif
	//}}AFX_DATA_MAP
#endif
	DDX_Control(pDX, IDC_CHATR_FROM, m_edChatrModeFrom);
	DDX_Control(pDX, IDC_CHATR_TO, m_edChatrModeTo);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_SETUP_CONNECTION, OnSetupConnection)
	ON_BN_CLICKED(IDC_USE_PROXY, OnUseProxy)
	ON_BN_CLICKED(IDC_USE_PROXY2, OnUseProxy2)
	ON_BN_CLICKED(IDC_CHECK_PROXYAUTH, OnProxyAuth)
	ON_BN_CLICKED(IDC_CHECK_RANGE , OnChartMode)
	ON_CBN_SELCHANGE(IDC_PROVIDERTYPE, OnSelchangeProviderType)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{

	m_iChartMode = 0;
	m_iRangeCheck = 0;
	m_iFlatOutRange = 0;
	m_iFlatNoBid = 0;
	theApp.GetOptions (m_opts);

	if( m_opts.m_enChartMode == enChart_5Penny || 
		m_opts.m_enChartMode == enChart_5Penny_Range)
		m_iChartMode = 1;

	if(m_opts.m_enChartMode >= enChart_All_Range)
		m_iRangeCheck = 1;

	m_iFlatOutRange = 0;
	if(m_opts.m_bFlatOutRange)
		m_iFlatOutRange = 1;
	
	m_iFlatNoBid = 0;
	if(m_opts.m_bFlatNoBid)
		m_iFlatNoBid = 1;

	CDialog::OnInitDialog();
	UpdateData(TRUE);

#ifndef _VOLA_MANAGER
	OnUseProxy();
	OnUseProxy2();
	OnProxyAuth();
	m_spProvider.CreateInstance(__uuidof(PP::Providers));
	InitProvidersList();
#endif
	OnChartMode();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#ifndef _VOLA_MANAGER

void COptionsDlg::InsertProvider( LPCTSTR szDescription, long type)
{
	int id = m_listProviders.AddString(szDescription);

	if(LB_ERR !=id)
		m_listProviders.SetItemData(id, (DWORD)type);
}

void COptionsDlg::InsertProvider(const CLSID& clsid, LPCTSTR szDescription, long type)
{
	if(GetObject(clsid))
		InsertProvider(szDescription, type);
}

void COptionsDlg::InitProvidersList()
{
	m_listProviders.EnableWindow(TRUE);
	m_listProviders.ResetContent();

#ifdef _MULTIPROVIDERS
	PP::IProvidersPtr  spProviders;
	spProviders.CreateInstance(__uuidof(PP::Providers));
	if(SUCCEEDED(spProviders->raw_Initialize()))
	{
		long lCount;
		lCount = spProviders->GetCount();
		for(long l=1; l<=lCount;l++)
		{
			PP::IProviderDataPtr spData;
			if(SUCCEEDED(spProviders->get_Item(l, &spData)))
			{
				_bstr_t bsProg = spData->Description;
				long lID = spData->ProviderID;

				InsertProvider((LPCTSTR)bsProg, lID);				
			}
		}
	}
	
#else
	InsertProvider(__uuidof(MT::MyTrackPriceProvider),_T("ProTrack Price Provider"), 0);
	m_listProviders.EnableWindow(FALSE);
#endif
	int count = m_listProviders.GetCount();

	for(int i = 0; i<count;i++)
	{
		if(m_opts.m_enProviderType == m_listProviders.GetItemData(i))
		{
			m_listProviders.SetCurSel(i);
			OnSelchangeProviderType();
			break;
		}
	}
}
#endif
bool COptionsDlg::GetObject(const CLSID& clsid)
{
	LPOLESTR szCLSID=NULL;
	StringFromCLSID (clsid, &szCLSID);
	
	CString strKey = _T("CLSID\\");
	strKey += CString  (szCLSID);
	CoTaskMemFree (szCLSID);

	CRegKey key;	
	long lResult =  key.Open(HKEY_CLASSES_ROOT, strKey , KEY_READ );
	if (lResult == ERROR_SUCCESS)
	{
		CRegKey keyModule;
		long lResult = keyModule.Open( key,  _T("LocalServer32"), KEY_READ );
		if (lResult != ERROR_SUCCESS)
			lResult = keyModule.Open( key,  _T("InprocServer32"), KEY_READ );

		key.Close();
	}
	return (lResult == ERROR_SUCCESS);
}


void COptionsDlg::OnSetupConnection() 
{
	IDBPromptInitialize * pPromptInit = GetDBPromptInit();
	IDataInitialize * pDataInit = GetDataInit();
	
	if (!pPromptInit || !pDataInit )
	{
		MessageBox (_T("Cannot initialize OLE DB"));
		return;
	}
	
	IDBInitialize * pDBInit=m_spDBInitialize.Detach();

#ifdef _IVAN_DB_ACCESS
	LPCOLESTR szProviders = L"Microsoft.Jet.OLEDB.4.0\0";
#else
	LPCOLESTR szProviders = L"SQLOLEDB\0";
#endif

	HRESULT hr= pPromptInit->PromptDataSource (NULL, m_hWnd, DBPROMPTOPTIONS_PROPERTYSHEET,
		0, NULL , szProviders, IID_IDBInitialize, (IUnknown **) &pDBInit );

	m_spDBInitialize.Attach(pDBInit);
	
	if (SUCCEEDED(hr))
	{
		LPOLESTR szInit = NULL;
		hr = pDataInit->GetInitializationString (m_spDBInitialize, TRUE, &szInit);
		if (SUCCEEDED(hr))
		{
			m_opts.m_strConnection = szInit;
			CoTaskMemFree (szInit);
			szInit = NULL;

			hr = pDataInit->GetInitializationString (m_spDBInitialize, FALSE, &szInit);
			if (SUCCEEDED(hr))
			{
				m_strDisplayConnection = szInit;
				CoTaskMemFree (szInit);

				CDataExchange dx (this, FALSE);
				DDX_Text (&dx, IDC_CONNECTION,  m_strDisplayConnection);
			}
		}
	}

}


void COptionsDlg::OnUseProxy() 
{
	bool bEnable = m_btUseProxyPrice.GetCheck() == 1;
	m_edProxyPrice.EnableWindow (bEnable);
	m_edPortPrice.EnableWindow (bEnable);

	if (! bEnable)
	{
		m_edProxyPrice.SetWindowText (_T(""));
		m_edPortPrice.SetWindowText (_T(""));
	}
}
void COptionsDlg::OnUseProxy2() 
{
	bool bEnable = m_btUseProxyIV.GetCheck() == 1;
	m_edProxyIV.EnableWindow (bEnable);
	m_edPortIV.EnableWindow (bEnable);
	if(!bEnable)
	{
		m_btProxyAuth.EnableWindow (FALSE);
		m_btProxyAuth.SetCheck(0);
		OnProxyAuth();
	}
	else 
		m_btProxyAuth.EnableWindow (TRUE);

	

	if (! bEnable)
	{
		m_edProxyIV.SetWindowText (_T(""));
		m_edPortIV.SetWindowText (_T(""));
	}
}
void COptionsDlg::OnChartMode()
{
	UpdateData(TRUE);
	if(m_iRangeCheck)
	{
			m_edChatrModeFrom.EnableWindow(TRUE);
			m_edChatrModeTo.EnableWindow(TRUE);
	}
	else
	{
			m_edChatrModeFrom.EnableWindow(FALSE);
			m_edChatrModeTo.EnableWindow(FALSE);

	}
}

void COptionsDlg::OnOK()
{
	UpdateData(TRUE);
	if(m_iRangeCheck)
	{

		if(m_iChartMode)
			m_opts.m_enChartMode = enChart_5Penny_Range;
		else
			m_opts.m_enChartMode = enChart_All_Range;

		if(m_opts.m_fChartRangeFrom < 0.)
		{
			CString strError;
			strError.LoadString(IDS_ERROR_WRONG_LBOUND);
			MessageBox (strError);
			return;
		}
		if(m_opts.m_fChartRangeTo < 0.)
		{
			CString strError;
			strError.LoadString(IDS_ERROR_WRONG_UBOUND);
			MessageBox (strError);
			return;
		}
		if(m_opts.m_fChartRangeFrom >= m_opts.m_fChartRangeTo)
		{
			CString strError;
			strError.LoadString(IDS_ERROR_WRONGRANGE);
			MessageBox (strError);
			return;
		}
	}
	else
	{
		if(m_iChartMode)
			m_opts.m_enChartMode = enChart_5Penny;
		else
			m_opts.m_enChartMode = enChart_All;

	}
#ifndef _VOLA_MANAGER

	if(m_iFlatOutRange)
		m_opts.m_bFlatOutRange = true;
	else
		m_opts.m_bFlatOutRange = false;

	if(m_iFlatNoBid)
		m_opts.m_bFlatNoBid = true;
	else
		m_opts.m_bFlatNoBid = false;

#ifdef _MULTIPROVIDERS
	m_opts.m_enProviderType = m_listProviders.GetItemData(m_listProviders.GetCurSel());
#else
	m_opts.m_enProviderType = 1;
#endif
#endif
	if ( ! theApp.SaveOptions (m_opts) )
	{
		MessageBox (_T("Unable to write into registry"));
	}

	CDialog::OnOK();
}

void COptionsDlg::OnSelchangeProviderType()
{
	int	id = m_listProviders.GetCurSel();

	long lProvider = m_listProviders.GetItemData(id);
	if(m_spProvider!=NULL)
	{
		PP::IProviderDataPtr spProviderData;
		if(SUCCEEDED(m_spProvider->raw_GetProvider(lProvider, &spProviderData)))
		{
			VARIANT_BOOL bValue = VARIANT_FALSE;
			spProviderData->get_NeedLogin(&bValue);
			if(bValue!=VARIANT_FALSE)
			{
				PP::ProviderUseProxyType enType;
				_bstr_t bsValue;

				spProviderData->get_UseProxy(&enType);

				GetDlgItem(IDC_PRICE_ACCOUNT)->EnableWindow(TRUE);
				GetDlgItem(IDC_PRICE_PASSWORD)->EnableWindow(TRUE);

				bsValue = spProviderData->UserAccount;
				GetDlgItem(IDC_PRICE_ACCOUNT)->SetWindowText((LPCTSTR)bsValue);

				bsValue = spProviderData->UserPassword;
				GetDlgItem(IDC_PRICE_PASSWORD)->SetWindowText((LPCTSTR)bsValue);

				switch(enType)
				{
				case PP::enProxyNone:
					{
						m_btUseProxyPrice.SetCheck(BST_UNCHECKED);
						m_edPortPrice.SetWindowText(_T(""));
						m_edProxyPrice.SetWindowText(_T(""));
						m_btUseProxyPrice.EnableWindow(FALSE);
						m_edPortPrice.EnableWindow(FALSE) ;
						m_edProxyPrice.EnableWindow(FALSE);
					}
					break;
				case PP::enProxyDisable:
					{
						m_btUseProxyPrice.SetCheck(BST_UNCHECKED);
						m_edPortPrice.SetWindowText(_T(""));
						m_edProxyPrice.SetWindowText(_T(""));
						m_btUseProxyPrice.EnableWindow(TRUE);
						m_edPortPrice.EnableWindow(FALSE) ;
						m_edProxyPrice.EnableWindow(FALSE);
					} break;
				case PP::enProxyDefault:
					{
						m_btUseProxyPrice.SetCheck(BST_INDETERMINATE);
						m_edPortPrice.SetWindowText(_T(""));
						m_edProxyPrice.SetWindowText(_T(""));
						m_btUseProxyPrice.EnableWindow(TRUE);
						m_edPortPrice.EnableWindow(FALSE) ;
						m_edProxyPrice.EnableWindow(FALSE);
					} break;
				case PP::enProxyCustom:
					{
						_bstr_t bsValue;
						m_btUseProxyPrice.SetCheck(BST_CHECKED);

						CString str;
						str.Format(_T("%d"),spProviderData->ProxyPort);
						 
						m_edPortPrice.SetWindowText(str);

						bsValue = spProviderData->ProxyName;
						m_edProxyPrice.SetWindowText((LPCTSTR)bsValue);
						m_btUseProxyPrice.EnableWindow(TRUE);
						m_edPortPrice.EnableWindow(TRUE) ;
						m_edProxyPrice.EnableWindow(TRUE);
					} break;

				}
			}
			else
			{
				m_btUseProxyPrice.EnableWindow(FALSE);
				m_edPortPrice.EnableWindow(FALSE) ;
				m_edProxyPrice.EnableWindow(FALSE);
				m_edPortPrice.SetWindowText(_T(""));
				m_edProxyPrice.SetWindowText(_T(""));


				GetDlgItem(IDC_PRICE_ACCOUNT)->EnableWindow(FALSE);
				GetDlgItem(IDC_PRICE_PASSWORD)->EnableWindow(FALSE);
				GetDlgItem(IDC_PRICE_ACCOUNT)->SetWindowText(_T(""));
				GetDlgItem(IDC_PRICE_PASSWORD)->SetWindowText(_T(""));

			}			
		}
		else
		{
			m_btUseProxyPrice.EnableWindow(FALSE);
			m_edPortPrice.EnableWindow(FALSE) ;
			m_edProxyPrice.EnableWindow(FALSE);
			m_edPortPrice.SetWindowText(_T(""));
			m_edProxyPrice.SetWindowText(_T(""));


			GetDlgItem(IDC_PRICE_ACCOUNT)->EnableWindow(FALSE);
			GetDlgItem(IDC_PRICE_PASSWORD)->EnableWindow(FALSE);
			GetDlgItem(IDC_PRICE_ACCOUNT)->SetWindowText(_T(""));
			GetDlgItem(IDC_PRICE_PASSWORD)->SetWindowText(_T(""));
		}
	}
	else
	{
		bool bEnable = m_btUseProxyPrice.GetCheck() == 1;		
		m_btUseProxyPrice.EnableWindow(TRUE);
		m_edPortPrice.EnableWindow(bEnable) ;
		m_edProxyPrice.EnableWindow(bEnable);

		GetDlgItem(IDC_PRICE_ACCOUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PRICE_PASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_PRICE_ACCOUNT)->SetWindowText(_T(""));
		GetDlgItem(IDC_PRICE_PASSWORD)->SetWindowText(_T(""));
	}
}

void COptionsDlg::OnProxyAuth()
{
	bool bEnable = m_btProxyAuth.GetCheck() == 1;

	m_edProxyUserID.EnableWindow(bEnable);
	m_edProxyUserPass.EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_PROXYACC)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_PROXYPASS)->EnableWindow(bEnable);
}