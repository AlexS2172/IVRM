using System;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Data.OleDb;
using System.Globalization;
using System.Diagnostics;




namespace OTCOptionCalc
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	/// 
    delegate void OnLastQuoteDelegate(ref PRICEPROVIDERSLib.QuoteUpdateParams Params, ref PRICEPROVIDERSLib.QuoteUpdateInfo Results);
    delegate void OnErrorDelegate(PRICEPROVIDERSLib.ErrorNumberEnum ErrorNumber , string Description, PRICEPROVIDERSLib.RequestsTypeEnum ReqType, System.Object oRequest );


	public class OptionCalculatorForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
		private System.Windows.Forms.TextBox tbVolatility;
        private System.Windows.Forms.GroupBox gbResults;
		private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox gbDividends;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox tbDivAmount;
        private System.Windows.Forms.RadioButton rPeriodic;
		private System.Windows.Forms.RadioButton rbDiscreteDividends;
		private System.Windows.Forms.TextBox tbContinousRate;
        private System.Windows.Forms.RadioButton rbContinous;
		private System.Windows.Forms.Label label37;
		private System.Windows.Forms.Label label38;
		private System.Windows.Forms.Label label39;
		private System.Windows.Forms.Label label40;
		private System.Windows.Forms.Label label41;
		private System.Windows.Forms.Label label42;
		private System.Windows.Forms.Label label43;
		private System.Windows.Forms.Label label44;
		private System.Windows.Forms.Label label45;
		private System.Windows.Forms.Label label46;
		private System.Windows.Forms.Label label47;
		private System.Windows.Forms.Label label48;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.TextBox tbDomesticRate;
		private System.Windows.Forms.TextBox tbResCImpliedVola;
		private System.Windows.Forms.TextBox tbResCGammaTheta;
		private System.Windows.Forms.TextBox tbResCGammaVega;
		private System.Windows.Forms.TextBox tbResCDeltaTheta;
		private System.Windows.Forms.TextBox tbResCDeltaVega;
		private System.Windows.Forms.TextBox tbResCAlpha;
		private System.Windows.Forms.TextBox tbResCRho;
		private System.Windows.Forms.TextBox tbResCVega;
		private System.Windows.Forms.TextBox tbResCTheta;
		private System.Windows.Forms.TextBox tbResCGamma;
		private System.Windows.Forms.TextBox tbResCDelta;
		private System.Windows.Forms.TextBox tbResCPrice;
		private System.Windows.Forms.TextBox tbResPPrice;
		private System.Windows.Forms.TextBox tbResPGamma;
		private System.Windows.Forms.TextBox tbResPDelta;
		private System.Windows.Forms.TextBox tbResPImpliedVola;
		private System.Windows.Forms.TextBox tbResPGammaTheta;
		private System.Windows.Forms.TextBox tbResPGammaVega;
		private System.Windows.Forms.TextBox tbResPDeltaTheta;
		private System.Windows.Forms.TextBox tbResPDeltaVega;
		private System.Windows.Forms.TextBox tbResPAlpha;
		private System.Windows.Forms.TextBox tbResPRho;
		private System.Windows.Forms.TextBox tbResPVega;
		private System.Windows.Forms.TextBox tbResPTheta;
        private System.Windows.Forms.GroupBox gbPeriodicDivs;
		private System.Windows.Forms.GroupBox gbCalcModel;
		private System.Windows.Forms.RadioButton rbStandart;
		private System.Windows.Forms.RadioButton rbOptimized;
		private System.ComponentModel.IContainer components;
		private DivStreamForm fDivForm;
		private frmAbout	  fAboutForm;
		private NumberFormatInfo nfi;
		private NumberFormatInfo nfi2;
		private double dOldMidPrice = -1;

		private double[] m_StreamDivAmounts;
		private double[] m_StreamDivYears;

        private long lContractType = 2;
        private double m_dPriceLast = 0.0;
        private double m_dStrike = 0.0;
		
		//private int iInitCount = 100;
	
		private int iProviderType = 1;
		PRICEPROVIDERSLib.BatchPriceInfo m_PriceProvider;

		PRICEPROVIDERSLib.BatchPriceInfo oPriceProvider;
		PRICEPROVIDERSLib.IGroupPriceWithNotify m_GroupPriceProvider;
		PRICEPROVIDERSLib.IBatchPriceProvider m_BatchPriceProvider;
		
		VolatilitySourcesLib.VolatilitySource	m_VolaSource;

		private System.Windows.Forms.TextBox tbSymbol;
		private string sDBConnection;
        private string sDBUserGroup;
		private int iVolaSource;
		private int iProvider;
		private bool bGotFeed;
		private bool bGotDB;
		private System.Data.OleDb.OleDbConnection oleDbConnection;
		private System.Data.OleDb.OleDbCommand oleDbCommand;
		private System.Data.OleDb.OleDbCommand ReadCommand;
		private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox tbCOptionPrice;
		private System.Windows.Forms.Label label15;
		private System.Windows.Forms.Label label16;
		private System.Windows.Forms.TextBox tbPOptionPrice;
		private bool bGotVola;
		
		private string sOldSymbol;
		private string sInOptionSymbol;
		
		private string sAtmCall;
		private string sAtmPut;
		
		private System.Timers.Timer FeedTimer;
		
		private long lSymbolType;
		private double dMaxValue = 1.0E+30D;
        private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label lbSpotOrFuturePrice;
		private double dMinValue = -1.0E+30D;
		private System.Timers.Timer InitTimer;
		private bool   bPreventCheckDate = false;
		private bool  bCanClear = true;
		private bool bGotAtms = false;
		
		private int  m_iStockID = -1;
		private bool m_bManual = false;
        private bool bContractTypeSet = false;
        private double m_dHTBRate = (double)-1E+308;


		private System.Data.OleDb.OleDbCommand ReadExpCommand;
		private System.Data.OleDb.OleDbCommand ReadOptsCommand;
		private System.Data.OleDb.OleDbCommand ReadFutOptsCommand;
		private bool bGotStockPrice = false;
		private bool bGotAtmPut = false;
        private bool bGotAtmCall = false;
		private System.Data.OleDb.OleDbCommand ReadCurveCommand;
		
		private const int MIN_EXPIRY_DAYS = 20;
		private CIRCurve oIRCurve;
		private OCWrapper.OCWrapperClass oOCWrapper;
		private bool bUseMidRate = true;
		private System.Data.OleDb.OleDbCommand GetCustomDivs;
		private bool bHTB = false;
		private System.Data.OleDb.OleDbCommand ReadCustCommand;
		private EtsGeneralLib.IEtsDivColl m_CustDivs;		// - custom dividends are temporary disabled
        private EtsGeneralLib.IEtsIndexDivAtom m_UndDivs;
		private ArrayList listTmpCust;
		private ArrayList listNewDBDivs;
		private ArrayList listDBDivs;
		private System.Windows.Forms.GroupBox StyleGroupBox;
		private System.Windows.Forms.RadioButton rbAmerican;
        private System.Windows.Forms.RadioButton rbEuropean;
        private System.Windows.Forms.Label label7;
		private System.Windows.Forms.RadioButton rCustomDivs;
		private System.Windows.Forms.GroupBox cbFutureUseSpot;
        private System.Windows.Forms.GroupBox gbFuturePrice;
		private System.Windows.Forms.Timer timer1;
		private System.Timers.Timer AtmTimer;
		private bool bFirstElapse;
        private DevExpress.XtraEditors.DateEdit dtpExpirationDate;
        private DevExpress.XtraBars.BarDockControl barDockControl3;
        private DevExpress.XtraBars.BarDockControl barDockControl4;
        private DevExpress.XtraBars.BarDockControl barDockControl2;
        private DevExpress.XtraBars.BarDockControl barDockControl1;
        private DevExpress.XtraBars.BarManager barManager2;
        private DevExpress.XtraBars.Bar bar1;
        private DevExpress.XtraBars.BarStaticItem barStatusText;
        private DevExpress.XtraBars.BarStaticItem barStaticItem1;
        private DevExpress.XtraEditors.GroupControl groupControl1;
        private DevExpress.XtraEditors.GroupControl groupControl2;
        private DevExpress.LookAndFeel.DefaultLookAndFeel defaultLookAndFeel2;
        private DevExpress.XtraEditors.SimpleButton btnClose;
        private DevExpress.XtraEditors.SimpleButton btnCalc;
        private DevExpress.XtraEditors.SimpleButton btnSetCustomDividends;
        private DevExpress.XtraEditors.TextEdit tbDTE;
        private DevExpress.XtraEditors.GroupControl groupControl3;
        private DevExpress.XtraEditors.ComboBoxEdit cbDivFreq;
        private DevExpress.XtraEditors.DateEdit dtpDivDate;
        private DevExpress.XtraBars.BarAndDockingController barAndDockingController1;
        private DevExpress.XtraEditors.CheckEdit cbUseSpotPrice;
        private DevExpress.XtraEditors.ComboBoxEdit cbSymbolType;
        private DevExpress.XtraEditors.TextEdit tbSpotPrice;
        private DevExpress.XtraEditors.TextEdit tbStrikePrice;
        private DevExpress.XtraEditors.Repository.RepositoryItemTextEdit repositoryItemTextEdit1;
        private DevExpress.XtraBars.Bar bar2;
        private DevExpress.XtraBars.BarSubItem subFile;
        private DevExpress.XtraBars.BarButtonItem btnExit;
        private DevExpress.XtraBars.BarSubItem subData;
        private DevExpress.XtraBars.BarButtonItem btnRefreshQuotes;
        private DevExpress.XtraBars.BarButtonItem btnRefreshVolatility;
        private DevExpress.XtraBars.BarButtonItem btnCalculate;
        private DevExpress.XtraBars.BarSubItem subHelp;
        private DevExpress.XtraBars.BarButtonItem btnContents;
        private DevExpress.XtraBars.BarButtonItem btnAbout;
        private ImageList imgMenu;
		private int  iSaveStockID;
        private OleDbCommand UserActionControl;

        private bool m_bFirstLoad;

		
		private struct CIRCurve
		{
			public int iID;
			public string sName;
			public int iRuleType;
			public int iCurveType;
			public double dPosThreshold;
			public int iPointsCount;
			public double[] irShortRateYTE;
			public double[] drShortRateValue;
			public double[] irLongRateYTE;
			public double[] drLongRateValue;
			public double[] NeutralRateYTE;
			public double[] drNeutralRateValue;
			public double[] irHTBRateYTE;
			public double[] drHTBRateValue;
			public double[] irNeutralHTBRateYTE;
			public double[] drNeutralHTBRateValue;
			public double[] irNeutralRateYTE;
		};


		/*
			0 - unrecognized
			1 - index
			2 - stock
			3 - option
			4 - future
			5 - future option
		*/
		
		//PRICEPROVIDERSLib.QuoteUpdateFullInfo m_gQuotesUpdateFullInfo;

		public  void  PriceProvider_OnQuoteUpdate()
		{
			return;
		}

		public  void  PriceProvider_OnSubscribed(ref PRICEPROVIDERSLib.QuoteUpdateParams Params)
		{
			return;
		}

		public  void  PriceProvider_OnError(PRICEPROVIDERSLib.ErrorNumberEnum ErrorNumber , string Description, PRICEPROVIDERSLib.RequestsTypeEnum ReqType , System.Object oRequest )
        {
            FeedTimer.Enabled = false;
            FeedTimer.Stop();

            OnErrorDelegate onError = new OnErrorDelegate(OnErrorData);
            this.Invoke(onError, new Object[] { ErrorNumber, Description, ReqType, oRequest });

        }
        private void  OnErrorData(PRICEPROVIDERSLib.ErrorNumberEnum ErrorNumber , string Description, PRICEPROVIDERSLib.RequestsTypeEnum ReqType , System.Object oRequest )
		{
			
            try
            {
                if (ReqType == PRICEPROVIDERSLib.RequestsTypeEnum.enSubscribeQuote)
                {
                    barStatusText.Caption = "Failed to subscribe: " + Description;
                }

                if (ReqType == PRICEPROVIDERSLib.RequestsTypeEnum.enRequestLastQuote)
                {
                    barStatusText.Caption = "Failed to request last quotes: " + Description;
                }

                if ((ReqType != PRICEPROVIDERSLib.RequestsTypeEnum.enSubscribeQuote) && (ReqType != PRICEPROVIDERSLib.RequestsTypeEnum.enRequestLastQuote))
                {
                    if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enProviderConnected)
                    {
                        barStatusText.Caption = "Provider connected";
                    }

                    if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enProviderConnecting)
                    {
                        barStatusText.Caption = "Provider is connecting";
                    }

                    if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enConnectionWasDisconnected)
                    {
                        barStatusText.Caption = "Provider disconnected";
                    }

                    if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enCouldNotConnectToProvider)
                    {
                        barStatusText.Caption = "Could not connect to provider: " + Description;
                    }

                    if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enNotConnected)
                    {
                        barStatusText.Caption = "Provider is not connected";
                    }
                }

            }
            catch
            {
            }
			return;
		}

		private bool IsCorrectDouble(double dParam)
		{
			if ((Double.IsInfinity(dParam))||(Double.IsNaN(dParam)))
				return false;
			if((dParam>= dMaxValue)||(dParam <=dMinValue ))
				return false;
	
			return true;
		}


        private void OnLastQuoteData(ref PRICEPROVIDERSLib.QuoteUpdateParams Params, ref PRICEPROVIDERSLib.QuoteUpdateInfo Results)
        {
            double dPriceBid = Results.BidPrice;
            double dPriceAsk = Results.AskPrice;
            double dPriceLast = Results.LastPrice;
            double dMidPrice = 0;

            try
            {

                if ((Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enSTK) ||
                    (Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enIDX) ||
                    (Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enFUT)
                    )
                {
                    if (!IsCorrectDouble(dPriceLast))
                    {
                        dPriceLast = 0;
                        dMidPrice = 0;
                    }
                    else
                        dMidPrice = dPriceLast;
                    
                    tbSpotPrice.EditValue = dPriceLast;

                    //                tbSpotPrice.EditValue = dPriceLast;// Convert.ToString(dPriceLast/*dMidPrice*/);
                    //                tbSpotPrice.Refresh();
                    bGotStockPrice = true;
                    //btnCalc_Click(null ,null);
                    if (m_bManual)
                    {
                        if (dOldMidPrice != dMidPrice)
                        {
                            if (TryGetAtmStrike(dMidPrice, m_iStockID) >= 0)
                                TryLoadSymbolVolatility();

                            dOldMidPrice = dMidPrice;
                            AtmTimer.Enabled = true;

                        }
                    }
                }

                if ((/*(*/Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT)
                    //||(Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enFUT))
                    || (Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enFOP))
                {

                    if ((!IsCorrectDouble(dPriceBid)) && (!IsCorrectDouble(dPriceAsk)))
                    {
                        dPriceBid = 0;
                        dPriceAsk = 0;

                        if (!IsCorrectDouble(dPriceLast))
                        {
                            dPriceLast = 0;
                            dMidPrice = 0;
                        }
                        else
                            dMidPrice = dPriceLast;

                    }
                    else
                    {
                        if (!IsCorrectDouble(dPriceBid))
                            dPriceBid = 0;


                        if (!IsCorrectDouble(dPriceAsk))
                            dPriceAsk = 0;

                        if (!IsCorrectDouble(dPriceLast))
                            dPriceLast = 0;

                        if ((dPriceAsk == 0) || (dPriceBid == 0))
                            dMidPrice = dPriceLast;
                        else
                            dMidPrice = (dPriceBid + dPriceAsk) / 2.0;
                    }


                    if (!IsCorrectDouble(dPriceLast))
                        dPriceLast = 0;


                    //tbSpotPrice.Text = Convert.ToString(dPriceLast);
                    if (Params.Symbol == sAtmCall)
                    {
                        tbCOptionPrice.Text = Convert.ToString(dMidPrice);
                        bGotAtmCall = true;

                        if ((bGotStockPrice) && (bGotAtmPut))
                        {
                            AtmTimer.Enabled = false;
                            btnCalc_Click(null, null);
                        }
                    }

                    if (Params.Symbol == sAtmPut)
                    {
                        tbPOptionPrice.Text = Convert.ToString(dMidPrice);
                        bGotAtmPut = true;

                        if ((bGotStockPrice) && (bGotAtmCall))
                        {
                            AtmTimer.Enabled = false;
                            btnCalc_Click(null, null);
                        }

                    }
                }

                string sSymbol = Params.Symbol;
                PRICEPROVIDERSLib.InstrumentTypeEnum itType = Params.Type;
                string sExchange = Params.Exchange;

                barStatusText.Caption = "Last quotes for " + sSymbol + ": Last " + Convert.ToString(dPriceLast) + "; Bid: " + Convert.ToString(dPriceBid) + "; Ask: " + Convert.ToString(dPriceAsk);

            }
            catch
            {
            }
        }

		public void PriceProvider_OnLastQuote(ref PRICEPROVIDERSLib.QuoteUpdateParams Params, ref PRICEPROVIDERSLib.QuoteUpdateInfo Results)
		{
            FeedTimer.Enabled = false;
            FeedTimer.Stop();

            OnLastQuoteDelegate lastQuote = new OnLastQuoteDelegate(OnLastQuoteData);
            this.Invoke(lastQuote, new Object[] { Params, Results });
			return;
		}

		private int SetContinous()
		{
			gbDividends.Enabled = false;

			rbContinous.Checked = true;
			rbDiscreteDividends.Checked = false;
			//rPeriodic
			gbPeriodicDivs.Enabled = false;
			//rCustomDivs
			//gbCustomDivs.Enabled = false;
			btnSetCustomDividends.Enabled = false;

			tbContinousRate.Enabled = true;
			return 0;
		}

		private int SetPeriodic()
		{
			gbDividends.Enabled = true;

			rbContinous.Checked = false;
			rbDiscreteDividends.Checked = true;
			rPeriodic.Checked = true;
			gbPeriodicDivs.Enabled = true;
			rCustomDivs.Checked = false;
			btnSetCustomDividends.Enabled = false;

			tbContinousRate.Enabled = false;
			return 0;
		}

		private int SetStream()
		{
			gbDividends.Enabled = true;

			rbContinous.Checked = false;
			rbDiscreteDividends.Checked = true;
			rPeriodic.Checked = false;
			gbPeriodicDivs.Enabled = false;
			rCustomDivs.Checked = true;

			tbContinousRate.Enabled = false;
			return 0;
		}

		private double GetNearestBaseStrikes(double dStrike, int iUndId, DateTime dtExp, ref double dNLessStrike, ref double dNGreaterStrike)
		{
			//m_iStockID
			double dRet = -1;

			if (!bGotDB)
				return -1;

			if (iUndId <= 0)
				iUndId = m_iStockID;

			
			if (iUndId <= 0)
				return -1;

			dNLessStrike = 0.0;
			dNGreaterStrike = 0.0;
			
			double dStrikePrice = dStrike;
			if (dStrikePrice <0)
			{
				try
				{
					dStrikePrice = Convert.ToDouble(tbStrikePrice.EditValue);
				}
				catch
				{
					dStrikePrice = - 1;
                    tbStrikePrice.Text = "0.00";
                    m_dStrike = 0.0;
				}

			}
			
			if (!IsCorrectDouble(dStrikePrice))
			{
				return -1;
			}
			
			if (dStrikePrice < 0)
				return -1;


			DateTime dtMinExpiry = dtExp;

			ReadExpCommand.CommandText= "usp_UnderlyingExpiry_Get";
			ReadExpCommand.CommandType = CommandType.StoredProcedure;
			ReadExpCommand.Parameters["@iContractID"].Value = iUndId;
			ReadExpCommand.Parameters["@iTraderID"].Value = 0;
			ReadExpCommand.Parameters["@iGroupID"].Value = 0;
			ReadExpCommand.Parameters["@dtMinExpiry"].Value = dtMinExpiry;

			OleDbDataReader ReadExp = ReadExpCommand.ExecuteReader();
			DateTime dtTmp;
			DateTime dtMinExp = DateTime.Today;
			double dDifference;
			double dMinDiff = 1e+30;

			while (ReadExp.Read())
			{
				dtTmp = DateTime.Parse(ReadExp["dtExpiryDate"].ToString());
				dDifference = dtTmp.ToOADate();
				if (dMinDiff > dDifference)
				{
					dMinDiff = dDifference;
					dtMinExp = dtTmp;
				}
			}
			ReadExp.Close();

			ReadOptsCommand.CommandText= "usp_UnderlyingOptionByExpiry_Get";
			ReadOptsCommand.CommandType = CommandType.StoredProcedure;
			ReadOptsCommand.Parameters["@iUnderlyingID"].Value = iUndId;
            ReadOptsCommand.Parameters["@iExpiryDate"].Value = dtMinExp;


			double dMinLessDiff = 1e+30;
			double dMinGreaterDiff = 1e+30;

			double dCurStrike = 0;
			OleDbDataReader ReadOpts = ReadOptsCommand.ExecuteReader();
			while (ReadOpts.Read())
			{
				dCurStrike = Convert.ToDouble(ReadOpts["fStrike"].ToString());

				if (dCurStrike < dStrikePrice)
				{
					if (dMinLessDiff > Math.Abs(dCurStrike - dStrikePrice))
					{
						dMinLessDiff = Math.Abs(dCurStrike - dStrikePrice);
						dNLessStrike = dCurStrike;
					}
				
				}
				else
				{
					if (dMinGreaterDiff > Math.Abs(dCurStrike - dStrikePrice))
					{
						dMinGreaterDiff = Math.Abs(dCurStrike - dStrikePrice);
						dNGreaterStrike = dCurStrike;
					}
				
				}

				
			} //while (ReadOpts.Read())

			ReadOpts.Close();
	
			return dRet;
		}


		private double TryGetAtmStrike(double dSpot, int iUndId)
		{
			double dRet = -1;
			if ((!bGotDB)||(iUndId <= 0))
				return -1;
			double dStrikePrice = dSpot;
			
			double lDTE;
			int lCurExpDate;
			
			lDTE = GetDTE(tbDTE.Text);

			DateTime dtMinExpiry = DateTime.Today;
			dtMinExpiry.AddDays(MIN_EXPIRY_DAYS);
			lCurExpDate = System.Convert.ToInt32(dtMinExpiry.ToOADate()) + (int)lDTE;
			
			ReadExpCommand.CommandText= "usp_UnderlyingExpiry_Get";
			ReadExpCommand.CommandType = CommandType.StoredProcedure;
			ReadExpCommand.Parameters["@iContractID"].Value = iUndId;
			ReadExpCommand.Parameters["@iTraderID"].Value = 0;
			ReadExpCommand.Parameters["@iGroupID"].Value = 0;
			ReadExpCommand.Parameters["@dtMinExpiry"].Value = dtMinExpiry;

			OleDbDataReader ReadExp = ReadExpCommand.ExecuteReader();
			DateTime dtTmp;
			DateTime dtMinExp = DateTime.Today;
			double dDifference;
			double dMinDiff = 1e+30;

			while (ReadExp.Read())
			{
				dtTmp = DateTime.Parse(ReadExp["dtExpiryDate"].ToString());
				dDifference = Math.Abs(lCurExpDate - dtTmp.ToOADate());
				if (dMinDiff > dDifference)
				{
					dMinDiff = dDifference;
					dtMinExp = dtTmp;
				}
			}
			ReadExp.Close();

			ReadOptsCommand.CommandText= "usp_UnderlyingOptionByExpiry_Get";
			ReadOptsCommand.CommandType = CommandType.StoredProcedure;
			ReadOptsCommand.Parameters["@iUnderlyingID"].Value = iUndId;
            ReadOptsCommand.Parameters["@iExpiryID"].Value = dtMinExp;

			double dNearestCallStrike = 0;
			double dNearestPutStrike = 0;

			double dMinCallDiff = 1e+30;
			double dMinPutDiff = 1e+30;

			string sAtmTmpPut = "";
			string sAtmTmpCall = "";

			int iIsCall = 0;
			double dCurStrike = 0;

			double dAvCStrike = 0;
			double dAvPStrike = 0;
			double dAvStrike = 0;
			int iStrikes = 0;
			int iCStrikes = 0;
			int iPStrikes = 0;

			OleDbDataReader ReadAvOpts = ReadOptsCommand.ExecuteReader();
			while (ReadAvOpts.Read())
			{
				iIsCall = Convert.ToInt32(ReadAvOpts["tiIsCall"].ToString());
				dCurStrike = Convert.ToDouble(ReadAvOpts["fStrike"].ToString());

				if (iIsCall == 0)
				{
					dAvCStrike += dCurStrike;
					iCStrikes++;
				}
				else
				{
					dAvPStrike += dCurStrike;
					iPStrikes++;
				} //if (iIsCall == 0)
				
			} //while (ReadOpts.Read())
				
			if ((iCStrikes > 0)&&(dAvCStrike > 0))
			{
				dAvStrike += dAvCStrike;
				iStrikes += iCStrikes;
			}
				
			if ((iPStrikes > 0)&&(dAvPStrike > 0))
			{
				dAvStrike += dAvPStrike;
				iStrikes += iPStrikes;
			}
				
			if (iStrikes > 0)
			{
				dAvStrike = dAvStrike / iStrikes;
			}
			else
			{
				dAvStrike = 0;
			}
			dStrikePrice = dAvStrike;
			ReadAvOpts.Close();

			OleDbDataReader ReadOpts = ReadOptsCommand.ExecuteReader();
			while (ReadOpts.Read())
			{
				iIsCall = Convert.ToInt32(ReadOpts["tiIsCall"].ToString());

				dCurStrike = Convert.ToDouble(ReadOpts["fStrike"].ToString());


				if (iIsCall == 0)
				{

					if (dMinPutDiff > Math.Abs(dCurStrike - dStrikePrice))
					{
						dMinPutDiff = Math.Abs(dCurStrike - dStrikePrice);
						dNearestPutStrike = dCurStrike;
						sAtmTmpPut = ReadOpts["vcSymbol"].ToString();
					}


				}
				else
				{
					if (dMinCallDiff > Math.Abs(dCurStrike - dStrikePrice))
					{
						dMinCallDiff = Math.Abs(dCurStrike - dStrikePrice);
						dNearestCallStrike = dCurStrike;
						sAtmTmpCall = ReadOpts["vcSymbol"].ToString();
					}

				} //if (iIsCall == 0)
				
			} //while (ReadOpts.Read())

			ReadOpts.Close();
	
			if (Math.Abs(dNearestCallStrike-dStrikePrice)>=Math.Abs(dNearestPutStrike-dStrikePrice))
			{
				dRet = dNearestCallStrike;
                tbStrikePrice.EditValue = dNearestCallStrike;

			}
			else
			{
				dRet = dNearestPutStrike;
                tbStrikePrice.EditValue = dNearestPutStrike;
            }		
	
			return dRet;
		}

		private int TryGetAtmSymbols(int iUndId, bool bManual)
		{
			if ((!bGotDB)||(iUndId <= 0))
				return -1;
			double dStrikePrice = 0;
			try
			{
				dStrikePrice = (double)System.Convert.ToDouble(tbStrikePrice.EditValue);
			}
			catch
			{
				barStatusText.Caption = "Wrong strike, can't manage ATMs";
				return -1;
			}		
			bGotAtms = false;

			string sAtmTmpPut = "";
			string sAtmTmpCall = "";

			if(cbSymbolType.SelectedIndex == 1)
			{
				ReadFutOptsCommand.CommandText= "usp_OTCFutureOptions_Get";
				ReadFutOptsCommand.CommandType = CommandType.StoredProcedure;
				ReadFutOptsCommand.Parameters["@futureID"].Value = iUndId;
				ReadFutOptsCommand.Parameters["@expiryDate"].Value = dtpExpirationDate.DateTime;
				ReadFutOptsCommand.Parameters["@strike"].Value = dStrikePrice;

				OleDbDataReader ReadOpts = ReadFutOptsCommand.ExecuteReader();
				bool b = false;
				while (ReadOpts.Read())
				{
					if(!b)
					{
						sAtmTmpPut = ReadOpts["symbol"].ToString();
						b = true;
					}
					else
					{
						sAtmTmpCall = ReadOpts["symbol"].ToString();
						break;
					}
				} //while (ReadOpts.Read())

				ReadOpts.Close();
			}
			else
			{
				double lCurExpDate = 0;
				double lDTE =  0;

				DateTime dtMinExpiry = DateTime.Today;
                DateTime dtExpiryOV = dtMinExpiry;
                DateTime dtTradingClose;
				dtMinExpiry.AddDays(MIN_EXPIRY_DAYS);
				//lDTE = System.Convert.ToDouble(tbDTE.Text);

				dtMinExpiry.AddDays(MIN_EXPIRY_DAYS);
				lCurExpDate = System.Convert.ToDouble(dtMinExpiry.ToOADate()) + lDTE;
	
				ReadExpCommand.CommandText= "usp_UnderlyingExpiry_Get";
				ReadExpCommand.CommandType = CommandType.StoredProcedure;
				ReadExpCommand.Parameters["@iContractID"].Value = iUndId;
				ReadExpCommand.Parameters["@iTraderID"].Value = 0;
				ReadExpCommand.Parameters["@iGroupID"].Value = 0;
				ReadExpCommand.Parameters["@dtMinExpiry"].Value = dtMinExpiry;

				OleDbDataReader ReadExp = ReadExpCommand.ExecuteReader();
				DateTime dtTmp;
				DateTime dtMinExp = DateTime.Today;
				double dDifference;
				double dMinDiff = 1e+30;

				while (ReadExp.Read())
				{
					dtTmp = DateTime.Parse(ReadExp["dtExpiryDate"].ToString());
					dDifference = Math.Abs(lCurExpDate - dtTmp.ToOADate());

					if (dMinDiff > dDifference)
					{
						dMinDiff = dDifference;
						dtMinExp = dtTmp;
                        dtExpiryOV = DateTime.Parse(ReadExp["dtExpiryOV"].ToString());
                        dtTradingClose = DateTime.Parse(ReadExp["dtTradingClose"].ToString());
					}
				}

				if (m_bManual)
				{
					dtpExpirationDate.DateTime = dtExpiryOV;
				}
	
				ReadExp.Close();

				ReadOptsCommand.CommandText= "usp_UnderlyingOptionByExpiry_Get";
				ReadOptsCommand.CommandType = CommandType.StoredProcedure;
				ReadOptsCommand.Parameters["@iUnderlyingID"].Value = iUndId;
                ReadOptsCommand.Parameters["@iExpiryID"].Value = dtMinExp;

				double dNearestCallStrike = 0;
				double dNearestPutStrike = 0;

				double dMinCallDiff = 1e+30;
				double dMinPutDiff = 1e+30;

				int iIsCall = 0;
				double dCurStrike = 0;



				OleDbDataReader ReadOpts = ReadOptsCommand.ExecuteReader();
				while (ReadOpts.Read())
				{
					iIsCall = Convert.ToInt32(ReadOpts["tiIsCall"].ToString());

					dCurStrike = Convert.ToDouble(ReadOpts["fStrike"].ToString());


					if (iIsCall == 0)
					{

						if (dMinPutDiff > Math.Abs(dCurStrike - dStrikePrice))
						{
							dMinPutDiff = Math.Abs(dCurStrike - dStrikePrice);
							dNearestPutStrike = dCurStrike;
							sAtmTmpPut = ReadOpts["vcSymbol"].ToString();
						}


					}
					else
					{
						if (dMinCallDiff > Math.Abs(dCurStrike - dStrikePrice))
						{
							dMinCallDiff = Math.Abs(dCurStrike - dStrikePrice);
							dNearestCallStrike = dCurStrike;
							sAtmTmpCall = ReadOpts["vcSymbol"].ToString();
						}

					} //if (iIsCall == 0)
				
				} //while (ReadOpts.Read())

				ReadOpts.Close();
			}
			sAtmPut = sAtmTmpPut;
			sAtmCall = sAtmTmpCall;

			if (sAtmTmpPut.Length <= 0 )
			{
				sAtmPut = sAtmCall;
			}
			
			if	(sAtmTmpCall.Length <= 0 )
			{
				sAtmCall = sAtmPut;
			}

			return 0;
		}
		
		
		private int TryGetType(int iID)
		{
			int iRet = 0;
			try
			{
				OleDbDataReader oDBReader;
				ReadCommand.CommandText = "select top 1 ContractTypeID from [ContractView] where contractID = " + Convert.ToString(iID);
				oDBReader = ReadCommand.ExecuteReader();
				string sTmp;
				while (oDBReader.Read())
				{
					sTmp = oDBReader["ContractTypeID"].ToString();
					if (sTmp.Length <= 0)
						break;
					else
					{
						int iTmp = Convert.ToInt32(sTmp);
						if (iTmp > 3)
						{
							cbSymbolType.SelectedIndex = 1;
							break;
						}
						else
						{
							cbSymbolType.SelectedIndex = 0;
							break;
						}
					}
				} // while oDBReader.Read()
				oDBReader.Close();
			}
			catch
			{
				iRet = -1; 
			}
			return iRet;
		}
															
		private int TryGetYield(int iID)
		{
			int iRet = 0;
			try
			{
				//MessageBox.Show("Entered");
				OleDbDataReader oDBReader;
				ReadCommand.CommandText = "select * from [indexview] where [indexview].indexid = " + Convert.ToString(iID);
				oDBReader = ReadCommand.ExecuteReader();
				string sTmp;
				int iTmpType = 0;
				int iHTB = 0;

				while (oDBReader.Read())
				{

					try
					{
						bHTB = false;
						iHTB = Convert.ToInt32(oDBReader["isHTB"].ToString());
						if (iHTB == 1)
							bHTB = true;
					}
					catch
					{
						bHTB = false;
					}

                    try
                    {
                        if (oDBReader["isBasket"].ToString() != "0")
                        {
                            tbContinousRate.Text = "0.00";

                            return 1;
                        }


                    }
                    catch
                    {
                        return -1;
                    }


                    if(!bContractTypeSet)
                    {
                        sTmp = oDBReader["CalcOptionType"].ToString();
                        if (sTmp.Length > 0)
                        {
                            iTmpType = Convert.ToInt32(sTmp);
                            if (iTmpType == 0)
                                rbEuropean.Checked = true;
                            else
                                rbAmerican.Checked = true;	// American at list
                        }
                        else
                            rbAmerican.Checked = true;	// American at list
                    }

					sTmp = oDBReader["yield"].ToString();
					//	MessageBox.Show(sTmp);
					if (sTmp.Length <= 0)
						tbDivAmount.Text = "0.00";
					else
					{
							
						double dTmp = Convert.ToDouble(sTmp);
						if (dTmp > 0)
						{
							SetContinous();
							dTmp = dTmp * 100;
							tbContinousRate.Text = dTmp.ToString("N", nfi2);
							iRet = 1;
							break;
						}
					}
				} // while oDBReader.Read()
				oDBReader.Close();
			}
			catch
			{
				iRet = -1;
			}
			return iRet;
		}


		private int TryGetFutureType(int iID)
		{
			int iRet = 0;
			try
			{
				//MessageBox.Show("Entered");
				OleDbDataReader oDBReader;
				ReadCommand.CommandText = "select  * from future where futureid =  " + Convert.ToString(iID);
				oDBReader = ReadCommand.ExecuteReader();
				string sTmp;
	

				while (oDBReader.Read())
				{

					int iTmpType = -1;
					sTmp = oDBReader["CalcOptionType"].ToString();
					if (sTmp.Length > 0)
					{
						iTmpType = Convert.ToInt32(sTmp);
						if (iTmpType == 0)
							rbEuropean.Checked = true;
						else
							rbAmerican.Checked = true;	// American at list
					}
					else
						rbAmerican.Checked = true;	// American at list

		
				} // while oDBReader.Read()
				oDBReader.Close();
			}
			catch
			{
				iRet = -1;
			}
			return iRet;
		}

		private int TryToGetDivs(string sSymbol, bool bManual)
		{
			if (!bGotDB)
				return -1;
			bool bGotDivs =false;
			
			bool bGotFreq;
			int iStockID = -1;	
			bool bWithEndSymbol = false;
			int iDivCustom = 0;

			try
			{

				OleDbDataReader oDBIDReader;

				int iIDCount = 0;
				
				ReadCommand.CommandText = "select Count(contractid) As GotIDs from contractview where symbol = '" + sSymbol+ "'";
				oDBIDReader = ReadCommand.ExecuteReader();
				if(oDBIDReader.HasRows)
				{
					while (oDBIDReader.Read())
						iIDCount = Convert.ToInt32(oDBIDReader["GotIDs"].ToString());
				}
				oDBIDReader.Close();

				if (iIDCount <= 0)
				{
					ReadCommand.CommandText = "select Count(contractid) As GotIDs from contractview where symbol like '" + sSymbol+ "_'";
					oDBIDReader = ReadCommand.ExecuteReader();
					if(oDBIDReader.HasRows)
					{
						while (oDBIDReader.Read())
							iIDCount = Convert.ToInt32(oDBIDReader["GotIDs"].ToString());
					}

					oDBIDReader.Close();
					if (iIDCount <= 0)
					{
						return -1;
					}
					else
					{
						bWithEndSymbol = true;
					}

				}
				int[] iIDs = new int[iIDCount];
				int iCount = 0;
				if (bWithEndSymbol)
					ReadCommand.CommandText = "select contractid from contractview where symbol like '" + sSymbol+ "_'";
				else
					ReadCommand.CommandText = "select contractid from contractview where symbol = '" + sSymbol+ "'";
				oDBIDReader = ReadCommand.ExecuteReader();
				while (oDBIDReader.Read())
				{
					iIDs[iCount] = Convert.ToInt32(oDBIDReader["ContractID"].ToString());
					iCount++;
				}
				oDBIDReader.Close();


				for(int i = 0; i<iIDCount; i++)
				{
					if (bGotDivs)
						break;
					iStockID = iIDs[i];
					if (iStockID > 0)
					{
						TryGetType(iStockID);
						if (TryGetYield(iStockID)> 0)
							bGotDivs = true;
						
						if (TryGetFutureType(iStockID)>0)
							bGotDivs = true;
						
						OleDbDataReader oDBReader;
						ReadCommand.CommandText = "select top 1 isHTB, divFreq, divAmt, divDate, isDivCustom, divAmtCustom, divDateCustom, divFreqCustom, CalcOptionType from  StockView where StockId = " + Convert.ToString(iStockID);
						oDBReader = ReadCommand.ExecuteReader();
                        if (oDBReader.HasRows)
                        {
                            bGotDivs = true;
                        }
                        else
                        {
                            oDBReader.Close();
                            ReadCommand.CommandText = "select top 1 isHTB, divFreq, divAmt, divDate, isDivCustom, divAmtCustom, divDateCustom, divFreqCustom, CalcOptionType from  IndexView where IndexId = " + Convert.ToString(iStockID);
                            oDBReader = ReadCommand.ExecuteReader();
                            bGotDivs = false;
                        }
						while (oDBReader.Read())
						{
							string sTmp;
							int iTmp;
							int iTmpType;
							
							int iHTB = 0;
						
							try
							{
								bHTB = false;
								iHTB = Convert.ToInt32(oDBReader["isHTB"].ToString());
								if (iHTB == 1)
									bHTB = true;
							}
							catch
							{
								bHTB = false;
							}


                            if(!bContractTypeSet)
                            {
                                sTmp = oDBReader["CalcOptionType"].ToString();
                                if (sTmp.Length > 0)
                                {
                                    iTmpType = Convert.ToInt32(sTmp);
                                    if (iTmpType == 0)
                                        rbEuropean.Checked = true;
                                    else
                                        rbAmerican.Checked = true;	// American at list
                                }
                                else
                                    rbAmerican.Checked = true;	// American at list
                            }
					        
							iDivCustom = Convert.ToInt32(oDBReader["isDivCustom"].ToString());
							if ( iDivCustom == 0 )
							{
								SetPeriodic();
								sTmp = oDBReader["divAmt"].ToString();
								if (sTmp.Length <= 0)
									tbDivAmount.Text = "0.00";
								else
								{
									tbDivAmount.Text = sTmp;
									bGotDivs = true;
                                    m_UndDivs.DivAmt = Convert.ToDouble(sTmp);
								}
									

								sTmp = oDBReader["divDate"].ToString();
                                if (sTmp.Length > 0){
                                    dtpDivDate.DateTime = DateTime.Parse(sTmp);
                                    m_UndDivs.DivDate = dtpDivDate.DateTime;
                                }
                                else
                                    dtpDivDate.DateTime = DateTime.Today;

								bGotFreq = false;
						
								try
								{
									iTmp = Convert.ToInt32(oDBReader["divFreq"].ToString());
                                    m_UndDivs.DivFreq = iTmp;
								}
								catch
								{
									iTmp = 	0;
								}
						
								if (iTmp == 12)
								{
									cbDivFreq.SelectedIndex = 0;
									bGotFreq = true;
								}
								if (iTmp == 4)
								{
									cbDivFreq.SelectedIndex = 1;
									bGotFreq = true;
								}
								if (iTmp == 2)
								{
									cbDivFreq.SelectedIndex = 2;
									bGotFreq = true;
								}

								if (iTmp == 1)
								{
									cbDivFreq.SelectedIndex = 3;
									bGotFreq = true;
								}

								if (!bGotFreq)
									cbDivFreq.SelectedIndex = 0;

				
							} //iDivCustom == 0
							else
							if ( iDivCustom == 1 ) // Custom Periodical
							{
								SetPeriodic();
								sTmp = oDBReader["divAmtCustom"].ToString();
								if (sTmp.Length <= 0)
									tbDivAmount.Text = "0.00";
								else
								{
									tbDivAmount.Text = sTmp;
                                    m_UndDivs.DivAmtCust = Convert.ToDouble(sTmp);
									bGotDivs = true;
								}
									

								sTmp = oDBReader["divDateCustom"].ToString();
								if (sTmp.Length > 0){
                                    dtpDivDate.DateTime = DateTime.Parse(sTmp);
                                    m_UndDivs.DivDateCust = dtpDivDate.DateTime;
                                }
								else
									dtpDivDate.DateTime = DateTime.Today;

								bGotFreq = false;
						
								try
								{
									iTmp = Convert.ToInt32(oDBReader["divFreqCustom"].ToString());
                                    m_UndDivs.DivFreqCust = iTmp;
								}
								catch
								{
									iTmp = 	0;
								}
						
								if (iTmp == 12)
								{
									cbDivFreq.SelectedIndex = 0;
									bGotFreq = true;
								}

								if (iTmp == 4)
								{
									cbDivFreq.SelectedIndex = 1;
									bGotFreq = true;
								}

								if (iTmp == 2)
								{
									cbDivFreq.SelectedIndex = 2;
									bGotFreq = true;
								}

								if (iTmp == 1)
								{
									cbDivFreq.SelectedIndex = 3;
									bGotFreq = true;
								}

								if (!bGotFreq)
									cbDivFreq.SelectedIndex = 0;

							
							} // if ( iDivCustom == 1 )
							else
							if ( iDivCustom == 2 ) // Custom stream
							{
							
								//SetStream();
								//long lRet = TryToGetCustomDivs(iStockID);
								
							
							} //if ( iDivCustom == 2 ) // Custom stream
							
							
					
						} // for
						oDBReader.Close();
				
					}


				}
				oDBIDReader.Close();
				
							
				if (iStockID > 0)
				{
					long lRet = TryToGetCustomDivs(iStockID);

					if ( iDivCustom == 2 ) // Custom stream
					{
						SetStream();
					} 

				} 



			}
			catch(Exception /*ex*/)
			{
				iStockID = -1;
			}

			/*
			if (!bGotDivs)
			{
				tbDivAmount.Text = "0.00";
				cbDivFreq.SelectedIndex = 0;
				dtpDivDate.DateTime = DateTime.Today;
				tbContinousRate.Text = "0.00";
				SetContinous();
			}
			*/
			
			m_iStockID = iStockID;
			TryGetAtmSymbols(iStockID, bManual);

			return 0;
		}

		private double GetCustomLinearInterpolatedVola(string sSymbol, DateTime dtExpDate, double dStrike)
		{
			double dRes = -1;
			try
			{
				MSGSTRUCTLib.SYMBOL_TYPE msgStruct = new MSGSTRUCTLib.SYMBOL_TYPE();
				if (cbSymbolType.SelectedIndex == 0)
					msgStruct = MSGSTRUCTLib.SYMBOL_TYPE.enStStock;

				if (cbSymbolType.SelectedIndex == 1)
					msgStruct = MSGSTRUCTLib.SYMBOL_TYPE.enStFutureOption;

				VADBLAYOUTLib.EOptType tEOptType;
				tEOptType = VADBLAYOUTLib.EOptType.OTM ;
	
				VMELib.IVolatilityManagement oVManagement = new VMELib.VolatilityManagementClass();
				oVManagement.DataSource = m_VolaSource.DataSource; 
				int iSurfID = oVManagement.get_ActiveSurfaceID(sSymbol);
				oVManagement.set_DefaultSurfaceID(tbSymbol.Text,iSurfID); 
				VMELib.SymbolVolatilitySurface oSurface = oVManagement.get_Surface(iSurfID, tEOptType);

				oSurface.DiscreteAcceleration= false;
				oSurface.PriceOverride = false;
				oSurface.UnderlinePrice = dStrike;
				oSurface.InterpolationFactor = 0.0;
				VMELib.TimeSkewVolatilities oTimeSkew = new VMELib.TimeSkewVolatilities();
				oTimeSkew = oSurface.TimeSkew;

				
				double dVola1 = -1;
				double dVola2 = -1;
				double dDate1 = 0;
				double dDate2 = 0;
				
				double dDate;
				double dVola = 0;
				bool bSkipSecond = false;

				
				VMELib.StrikeSkewVolatilities oStrikeSkew = oSurface.get_StrikeSkew(dtExpDate);
				oStrikeSkew.BasePointsInterpolation = false;
				
				dDate = dtExpDate.ToOADate();
				// Get Left-Date Vola
				DateTime dtDay = DateTime.FromOADate(0);
				dtDay.AddDays(1);
				TimeSpan tsDay = TimeSpan.FromDays(1);
				DateTime dtTmp =  dtExpDate;
				for (int i = 0; i<1000; i++)
				{
					dVola1 = oSurface.get_Volatility(0, dStrike, dtTmp );
					if (dVola1 > 0)
					{
						dDate1 = dtTmp.ToOADate();
						//	dVola1 = GetMMVola(sSymbol, dtTmp, dStrike);
						//	if ((dtTmp.Month == dtExpDate.Month )&&(dtTmp.Day  == 1 ))
						//	{
						//		dVola = dVola1;
						//		bSkipSecond = true;
						//	}
						break;
					}
						
					else
						dtTmp = dtTmp.Subtract(tsDay);
				}
				

				// Get Right-Date Vola
				if (!bSkipSecond)
				{
					dtTmp =  dtExpDate;
					for (int i = 0; i <1000; i++)
					{
						dVola2 = oSurface.get_Volatility(0, dStrike, dtTmp );
						if (dVola2 > 0)
						{
							dDate2 = dtTmp.ToOADate();
							//	dVola2 = GetMMVola(sSymbol, dtTmp, dStrike);
							break;
						}
						else
							dtTmp = dtTmp.AddDays(1.0);
					}
					
					// Get Coeffs
					double a, b;
					if (dDate2 != dDate1)
					{
						if ((dVola1 >= 0)&&(dVola2 >= 0))
						{
							a = (dVola2 - dVola1)/(dDate2 - dDate1);
							b = dVola1 - a*dDate1;
							// Calc Vola
							dVola = a*dDate + b;
						}
						else
						{
							if (dVola1 >= 0)
							{
								dVola =  0;
							}
							if (dVola2 >= 0)
							{
								dVola =  0;
							}
						}
					}
					else
					{
						dVola = dVola1;
					}

				} // !bSkipSecond


				
				if (!IsCorrectDouble(dVola)||dVola<0 )
				{
					dRes =  -1;
				}
				else
					dRes = dVola;

			}
			catch
			{
				dRes = -1;
			}
			return dRes;		
		}

		private double GetMMVola(string sSymbol, DateTime dtExpDate, double dStrike, int iStockID)
		{
			double dRes = -1;
			try
			{
				/*double dNLessStrike = 0.0;
				double dNGreaterStrike = 0.0;*/
				double dIntVola = 0.0;
				/*double dLessVola = 0.0;
				double dGreaterVola = 0.0;*/

				/*GetNearestBaseStrikes(dStrike, iStockID, dtpExpirationDate.DateTime, ref dNLessStrike, ref dNGreaterStrike);
				if ((dNLessStrike== dStrike)||(dNGreaterStrike==dStrike))
				{*/
				dIntVola =GetStrikeVola(sSymbol, dtExpDate, dStrike);
				/*}
				else
				{
					if (dNLessStrike >= 0)
					{
						dLessVola = GetStrikeVola(sSymbol, dtExpDate, dNLessStrike);
				
					}
				
					if (dNGreaterStrike >= 0)
					{
						dGreaterVola = GetStrikeVola(sSymbol, dtExpDate, dNGreaterStrike);
					
					}
				
					if ((dLessVola < 0 )||(dGreaterVola < 0))
					{
						if (dLessVola >= 0 )
							dIntVola = dLessVola;
						else
							dIntVola = dGreaterVola;

					}
					else
					{
							dIntVola = GetStrikeLinearInterpolatedVola(dNLessStrike, dNGreaterStrike, dLessVola, dGreaterVola, dStrike);
					}
				}*/

				if (dIntVola >=0 )
					dRes = dIntVola;
				else
					dRes = - 1;

			}
			catch
			{
				dRes = -1;
			}
			return dRes;
		}

		private double GetStrikeVola(string sSymbol, DateTime dtExpDate, double dStrike)
		{
			double dRes = -1;
			try
			{
				EtsGeneralLib.IVolaControl oVolaControl = new EtsGeneralLib.VolaControlClass();
				oVolaControl.Init(sSymbol, EtsGeneralLib.EtsContractTypeEnum.enCtStock, m_VolaSource, false);
				if (oVolaControl.IsInitialized )
				{
                    int lSurfaceID = oVolaControl.GetSurfaceByRoot(-1);
					dRes = oVolaControl.get_OptionVola(dtExpDate, dStrike, lSurfaceID);
				} 
					
					
				if (!IsCorrectDouble(dRes)||dRes<0 )
				{
					dRes =  -1;
				}
			}
			catch
			{
				dRes = -1;
			}
			return dRes;
		}

		private double GetStrikeLinearInterpolatedVola(double dStrike1, double dStrike2, double dVola1, double dVola2, double dStrike)
		{
			double dRet = 0;
			double dVola = -1;

			// Get Coeffs
			double a, b;
			if (dStrike1 != dStrike2)
			{
				if ((dVola1 >= 0)&&(dVola2 >= 0))
				{
					a = (dVola2 - dVola1)/(dStrike2 - dStrike1);
					b = dVola1 - a*dStrike1;
					// Calc Vola
					dVola = a*dStrike + b;
				}
				else
				{
					if (dVola1 >= 0)
					{
						dVola =  0;
					}
					if (dVola2 >= 0)
					{
						dVola =  0;
					}
				}
			}
			else
			{
				dVola = dVola1;
			}
			dRet = dVola;
			return dRet;
		}
		

		private string GetUndSymbol(int iID, out int iStockID)
		{
			string sTmp = "";
			iStockID = 0;
			try
			{
				OleDbDataReader oDBReader;
				ReadCommand.CommandText = "SELECT     TOP 1 ContractView.symbol, ContractView.contractID FROM Future INNER JOIN FutureRoot ON Future.futureRootID = FutureRoot.futureRootID INNER JOIN ContractView ON FutureRoot.underlyingID = ContractView.contractID WHERE     Future.futureID = "
					+ Convert.ToString(iID);
				oDBReader = ReadCommand.ExecuteReader();
				while (oDBReader.Read())
				{
					sTmp = oDBReader["symbol"].ToString();
					iStockID = Convert.ToInt32(oDBReader["contractID"]);
				}
				oDBReader.Close();
			}
			catch
			{
			}
			return sTmp;
		}

		private double TryLoadSymbolVolatility()
		{
		
			if (!bGotVola)
				return -1;
			double dRes;
			double dStrikePrice;
			string sSymbol = tbSymbol.Text;
			DateTime dtExpDate = dtpExpirationDate.DateTime;

			int iStockID = m_iStockID;
			if(cbSymbolType.SelectedIndex != 0)
			{
				sSymbol = GetUndSymbol(m_iStockID, out iStockID);
			}

			if (sSymbol.Length ==0 )
				return -1;

			try
			{
				dStrikePrice = (double)System.Convert.ToDouble(tbStrikePrice.EditValue);
			}
			catch
			{
				barStatusText.Caption = "Wrong strike, Volatility value is not loaded";
				return -1;
			}			


			try
			{
				
				dRes = GetMMVola(sSymbol, dtExpDate, dStrikePrice, iStockID);
				dRes = dRes * 100; // to %
				
				if (!IsCorrectDouble(dRes)||dRes<0 )
				{
					tbVolatility.Text = "0.00";
					barStatusText.Caption = "Can't get volatility";
					dRes =  -1;
				}
				else
				{
					tbVolatility.Text = dRes.ToString("N", nfi);
					barStatusText.Caption = "Got volatility for " + tbSymbol.Text;
				}


			}
			catch
			{
				barStatusText.Caption = "Can't get volatility";
				dRes =  -1;
			}
			return dRes;
		}


		private int FeedRequestLastQuotes(string sSymbol, bool bCheckActiveFutures)
		{
            string sRequestSymbol;

			if ((sSymbol.Length <= 0)||(sSymbol.Length > 15))
				return -1;

			bGotStockPrice = false;
			bGotAtmPut = false;
			bGotAtmCall = false;

			//sSymbol = sSymbol.ToUpper();
			//tbSymbol.Text = sSymbol;

            m_dPriceLast = 0.0;
			tbSpotPrice.Text = "0.00";
			tbCOptionPrice.Text = "0.00";
			tbPOptionPrice.Text = "0.00";
			

			if (!bGotFeed)
			{
				barStatusText.Caption = "Can't request last quotes: no feed";
				return -1;
			}
			barStatusText.Caption = "Requested last quotes for " + tbSymbol.Text;
			//PRICEPROVIDERSLib.QuoteUpdateParams aParam = new PRICEPROVIDERSLib.QuoteUpdateParams();
			PRICEPROVIDERSLib.QuoteUpdateParams aArgParam = new PRICEPROVIDERSLib.QuoteUpdateParams();
			
			// Stock prices only for now...

			/*aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			if ((cbSymbolType.SelectedIndex == 1)&&(sSymbol.IndexOf("/") <= 0))
			{
				sSymbol = "/" + sSymbol;
			}
			*/
			//aArgParam.Exchange = "G";
			switch(lContractType) 
			{
				case (long)EtsGeneralLib.EtsContractTypeEnum.enCtFutOption:
					aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enFOP;
           			aArgParam.Symbol = sSymbol;
					break;
				case (long)EtsGeneralLib.EtsContractTypeEnum.enCtFuture:
					aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enFUT;
                    sRequestSymbol =sSymbol;
                    aArgParam.Symbol = sRequestSymbol;
					break;
				case (long)EtsGeneralLib.EtsContractTypeEnum.enCtIndex:
					aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enIDX;
                    aArgParam.Symbol = sSymbol;//.ToUpper();
                    break;
				case (long)EtsGeneralLib.EtsContractTypeEnum.enCtOption:
					aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT;
                    aArgParam.Symbol = sSymbol;
                    break;
				case (long)EtsGeneralLib.EtsContractTypeEnum.enCtStock:
					aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enSTK;
                    sRequestSymbol = sSymbol;//.ToUpper();

                    aArgParam.Symbol = sRequestSymbol;
                    break;
			}
			oPriceProvider.RequestLastQuote(ref aArgParam);

			/*aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);
			*/
			/*
			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enGrSTK;
			oPriceProvider.RequestLastQuote(ref aArgParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enIDX;
			oPriceProvider.RequestLastQuote(ref aArgParam);
			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enGrIDX;
			oPriceProvider.RequestLastQuote(ref aArgParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);
			
			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT;
			oPriceProvider.RequestLastQuote(ref aArgParam);
			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enFUT;
			oPriceProvider.RequestLastQuote(ref aArgParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enFOP;
			oPriceProvider.RequestLastQuote(ref aArgParam);
			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOFT;
			oPriceProvider.RequestLastQuote(ref aArgParam);
			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOST;
			oPriceProvider.RequestLastQuote(ref aArgParam);

		
			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);
			*/

			if (sAtmPut != null && sAtmCall != null)
				FeedRequestAtms(sAtmPut, sAtmCall);

			FeedTimer.Enabled = true;
			AtmTimer.Enabled = true;
            FeedTimer.Start();

			return 0;
		}

		
		private int FeedRequestAtms(string sAtmPut, string sAtmCall)
		{
			/*if ((sAtmCall.Length <= 0)||(sAtmPut.Length > 8))
				return -1;
			*/
			tbCOptionPrice.Text = "0.00";
			tbPOptionPrice.Text = "0.00";
			
			if (!bGotFeed)
			{
				barStatusText.Caption = "Can't request last quotes: no feed";
				return -1;
			}
			//barStatusText.Caption = "Requested last quotes for " + sAtmPut + " and " + sAtmCall;
			PRICEPROVIDERSLib.QuoteUpdateParams aParam = new PRICEPROVIDERSLib.QuoteUpdateParams();
			PRICEPROVIDERSLib.QuoteUpdateParams aParamCall = new PRICEPROVIDERSLib.QuoteUpdateParams();
			PRICEPROVIDERSLib.QuoteUpdateParams aParamPut = new PRICEPROVIDERSLib.QuoteUpdateParams();
	

			//			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			//			oPriceProvider.RequestLastQuote(ref aParam);

			if(sAtmPut != null && (sAtmPut.Length > 0))
			{
				aParamPut.Symbol = sAtmPut;
				//aParamPut.Exchange = "G";
				if(cbSymbolType.SelectedIndex == 1)
					aParamPut.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enFOP;
				else
					aParamPut.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT;
				oPriceProvider.RequestLastQuote(ref aParamPut);
				barStatusText.Caption = "Requested last quotes for " + sAtmPut;
			}

			//			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			//			oPriceProvider.RequestLastQuote(ref aParam);
			//			
			//
			//			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			//			oPriceProvider.RequestLastQuote(ref aParam);

			if(sAtmCall != null && (sAtmCall.Length > 0))
			{
				aParamCall.Symbol = sAtmCall;
				//aParamCall.Exchange = "G";
				if(cbSymbolType.SelectedIndex == 1)
					aParamCall.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enFOP;
				else
					aParamCall.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT;
				oPriceProvider.RequestLastQuote(ref aParamCall);
				barStatusText.Caption = "Requested last quotes for " + sAtmCall;
			}

			//			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			//			oPriceProvider.RequestLastQuote(ref aParam);
			
			/*
			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enGrSTK;
			oPriceProvider.RequestLastQuote(ref aArgParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enIDX;
			oPriceProvider.RequestLastQuote(ref aArgParam);
			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enGrIDX;
			oPriceProvider.RequestLastQuote(ref aArgParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);
			
			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT;
			oPriceProvider.RequestLastQuote(ref aArgParam);
			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enFUT;
			oPriceProvider.RequestLastQuote(ref aArgParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enFOP;
			oPriceProvider.RequestLastQuote(ref aArgParam);
			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOFT;
			oPriceProvider.RequestLastQuote(ref aArgParam);
			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOST;
			oPriceProvider.RequestLastQuote(ref aArgParam);

		
			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);
			*/

			//FeedTimer.Enabled = true;
			return 0;
		}

		public OptionCalculatorForm()
		{
            //
            // Required for Windows Form Designer support
            //
            InitializeComponent();

            //try
            //{
            //    System.Data.OleDb.OleDbConnection oleDbConnection1;
            //    ETSMANAGERLib.Settings oSettings = new ETSMANAGERLib.Settings();
            //    sDBConnection = oSettings.DatabaseConnection;
            //    int at;
            //    at = sDBConnection.IndexOf("Initial Catalog=");
            //    sDBUserGroup = sDBConnection.Substring(at + 16);
            //    at = sDBUserGroup.IndexOf(";");
            //    sDBUserGroup = sDBUserGroup.Remove(at);

            //    oleDbConnection1 = new System.Data.OleDb.OleDbConnection();
            //    oleDbConnection1.ConnectionString = sDBConnection;
            //    oleDbConnection1.Open();
            //    this.UserActionControl.Connection = oleDbConnection1;
            //    this.UserActionControl.CommandText = "usp_RegUserAction_Save";
            //    this.UserActionControl.Parameters["@i_dtTime"].Value = DateTime.Now;
            //    this.UserActionControl.Parameters["@i_bLogOnOut"].Value = 6;
            //    this.UserActionControl.Parameters["@i_DBLogin"].Value = Environment.UserName;
            //    this.UserActionControl.Parameters["@i_ServerPath"].Value = SystemInformation.ComputerName;
            //    this.UserActionControl.Parameters["@i_CompanyName"].Value = sDBUserGroup;
            //    this.UserActionControl.ExecuteReader();
            //    oleDbConnection1.Close();
            //}
            //catch
            //{

            //}

			this.ReadFutOptsCommand = new System.Data.OleDb.OleDbCommand();
			this.ReadFutOptsCommand.Connection = this.oleDbConnection;
			this.ReadFutOptsCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@futureID", System.Data.OleDb.OleDbType.Integer));
			this.ReadFutOptsCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@expiryDate", System.Data.OleDb.OleDbType.Date));
			this.ReadFutOptsCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@strike", System.Data.OleDb.OleDbType.Double));

			m_StreamDivAmounts = new double[0];
			m_StreamDivYears = new double[0];
            m_bFirstLoad = true;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );

		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OptionCalculatorForm));
            this.lbSpotOrFuturePrice = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.tbVolatility = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.tbDomesticRate = new System.Windows.Forms.TextBox();
            this.gbResults = new System.Windows.Forms.GroupBox();
            this.groupControl2 = new DevExpress.XtraEditors.GroupControl();
            this.tbResPImpliedVola = new System.Windows.Forms.TextBox();
            this.tbResPTheta = new System.Windows.Forms.TextBox();
            this.tbResPGammaTheta = new System.Windows.Forms.TextBox();
            this.tbResPPrice = new System.Windows.Forms.TextBox();
            this.tbResPGammaVega = new System.Windows.Forms.TextBox();
            this.tbResPDelta = new System.Windows.Forms.TextBox();
            this.tbResPDeltaTheta = new System.Windows.Forms.TextBox();
            this.tbResPGamma = new System.Windows.Forms.TextBox();
            this.tbResPDeltaVega = new System.Windows.Forms.TextBox();
            this.tbResPVega = new System.Windows.Forms.TextBox();
            this.tbResPAlpha = new System.Windows.Forms.TextBox();
            this.tbResPRho = new System.Windows.Forms.TextBox();
            this.groupControl1 = new DevExpress.XtraEditors.GroupControl();
            this.tbResCImpliedVola = new System.Windows.Forms.TextBox();
            this.tbResCDeltaTheta = new System.Windows.Forms.TextBox();
            this.tbResCGammaTheta = new System.Windows.Forms.TextBox();
            this.tbResCPrice = new System.Windows.Forms.TextBox();
            this.tbResCGammaVega = new System.Windows.Forms.TextBox();
            this.tbResCDelta = new System.Windows.Forms.TextBox();
            this.tbResCGamma = new System.Windows.Forms.TextBox();
            this.tbResCDeltaVega = new System.Windows.Forms.TextBox();
            this.tbResCTheta = new System.Windows.Forms.TextBox();
            this.tbResCAlpha = new System.Windows.Forms.TextBox();
            this.tbResCVega = new System.Windows.Forms.TextBox();
            this.tbResCRho = new System.Windows.Forms.TextBox();
            this.label46 = new System.Windows.Forms.Label();
            this.label48 = new System.Windows.Forms.Label();
            this.label47 = new System.Windows.Forms.Label();
            this.label37 = new System.Windows.Forms.Label();
            this.label38 = new System.Windows.Forms.Label();
            this.label39 = new System.Windows.Forms.Label();
            this.label40 = new System.Windows.Forms.Label();
            this.label41 = new System.Windows.Forms.Label();
            this.label42 = new System.Windows.Forms.Label();
            this.label43 = new System.Windows.Forms.Label();
            this.label44 = new System.Windows.Forms.Label();
            this.label45 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tbContinousRate = new System.Windows.Forms.TextBox();
            this.rbContinous = new System.Windows.Forms.RadioButton();
            this.rbDiscreteDividends = new System.Windows.Forms.RadioButton();
            this.gbDividends = new System.Windows.Forms.GroupBox();
            this.btnSetCustomDividends = new DevExpress.XtraEditors.SimpleButton();
            this.rCustomDivs = new System.Windows.Forms.RadioButton();
            this.gbPeriodicDivs = new System.Windows.Forms.GroupBox();
            this.dtpDivDate = new DevExpress.XtraEditors.DateEdit();
            this.cbDivFreq = new DevExpress.XtraEditors.ComboBoxEdit();
            this.label12 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.tbDivAmount = new System.Windows.Forms.TextBox();
            this.rPeriodic = new System.Windows.Forms.RadioButton();
            this.label13 = new System.Windows.Forms.Label();
            this.tbSymbol = new System.Windows.Forms.TextBox();
            this.gbCalcModel = new System.Windows.Forms.GroupBox();
            this.rbStandart = new System.Windows.Forms.RadioButton();
            this.rbOptimized = new System.Windows.Forms.RadioButton();
            this.tbCOptionPrice = new System.Windows.Forms.TextBox();
            this.oleDbConnection = new System.Data.OleDb.OleDbConnection();
            this.oleDbCommand = new System.Data.OleDb.OleDbCommand();
            this.ReadCommand = new System.Data.OleDb.OleDbCommand();
            this.label16 = new System.Windows.Forms.Label();
            this.tbPOptionPrice = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.FeedTimer = new System.Timers.Timer();
            this.label10 = new System.Windows.Forms.Label();
            this.InitTimer = new System.Timers.Timer();
            this.ReadExpCommand = new System.Data.OleDb.OleDbCommand();
            this.ReadOptsCommand = new System.Data.OleDb.OleDbCommand();
            this.cbFutureUseSpot = new System.Windows.Forms.GroupBox();
            this.tbStrikePrice = new DevExpress.XtraEditors.TextEdit();
            this.tbSpotPrice = new DevExpress.XtraEditors.TextEdit();
            this.cbSymbolType = new DevExpress.XtraEditors.ComboBoxEdit();
            this.groupControl3 = new DevExpress.XtraEditors.GroupControl();
            this.tbDTE = new DevExpress.XtraEditors.TextEdit();
            this.btnCalc = new DevExpress.XtraEditors.SimpleButton();
            this.btnClose = new DevExpress.XtraEditors.SimpleButton();
            this.dtpExpirationDate = new DevExpress.XtraEditors.DateEdit();
            this.gbFuturePrice = new System.Windows.Forms.GroupBox();
            this.cbUseSpotPrice = new DevExpress.XtraEditors.CheckEdit();
            this.label7 = new System.Windows.Forms.Label();
            this.StyleGroupBox = new System.Windows.Forms.GroupBox();
            this.rbEuropean = new System.Windows.Forms.RadioButton();
            this.rbAmerican = new System.Windows.Forms.RadioButton();
            this.ReadCurveCommand = new System.Data.OleDb.OleDbCommand();
            this.GetCustomDivs = new System.Data.OleDb.OleDbCommand();
            this.ReadCustCommand = new System.Data.OleDb.OleDbCommand();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.AtmTimer = new System.Timers.Timer();
            this.barManager2 = new DevExpress.XtraBars.BarManager();
            this.bar1 = new DevExpress.XtraBars.Bar();
            this.barStatusText = new DevExpress.XtraBars.BarStaticItem();
            this.bar2 = new DevExpress.XtraBars.Bar();
            this.subFile = new DevExpress.XtraBars.BarSubItem();
            this.btnExit = new DevExpress.XtraBars.BarButtonItem();
            this.subData = new DevExpress.XtraBars.BarSubItem();
            this.btnRefreshQuotes = new DevExpress.XtraBars.BarButtonItem();
            this.btnRefreshVolatility = new DevExpress.XtraBars.BarButtonItem();
            this.btnCalculate = new DevExpress.XtraBars.BarButtonItem();
            this.subHelp = new DevExpress.XtraBars.BarSubItem();
            this.btnContents = new DevExpress.XtraBars.BarButtonItem();
            this.btnAbout = new DevExpress.XtraBars.BarButtonItem();
            this.barAndDockingController1 = new DevExpress.XtraBars.BarAndDockingController(this.components);
            this.barDockControl1 = new DevExpress.XtraBars.BarDockControl();
            this.barDockControl2 = new DevExpress.XtraBars.BarDockControl();
            this.barDockControl3 = new DevExpress.XtraBars.BarDockControl();
            this.barDockControl4 = new DevExpress.XtraBars.BarDockControl();
            this.imgMenu = new System.Windows.Forms.ImageList(this.components);
            this.barStaticItem1 = new DevExpress.XtraBars.BarStaticItem();
            this.repositoryItemTextEdit1 = new DevExpress.XtraEditors.Repository.RepositoryItemTextEdit();
            this.defaultLookAndFeel2 = new DevExpress.LookAndFeel.DefaultLookAndFeel(this.components);
            this.UserActionControl = new System.Data.OleDb.OleDbCommand();
            this.gbResults.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.groupControl2)).BeginInit();
            this.groupControl2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.groupControl1)).BeginInit();
            this.groupControl1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.gbDividends.SuspendLayout();
            this.gbPeriodicDivs.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dtpDivDate.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cbDivFreq.Properties)).BeginInit();
            this.gbCalcModel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FeedTimer)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.InitTimer)).BeginInit();
            this.cbFutureUseSpot.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbStrikePrice.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbSpotPrice.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cbSymbolType.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.groupControl3)).BeginInit();
            this.groupControl3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbDTE.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dtpExpirationDate.Properties)).BeginInit();
            this.gbFuturePrice.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.cbUseSpotPrice.Properties)).BeginInit();
            this.StyleGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.AtmTimer)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.barManager2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.barAndDockingController1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemTextEdit1)).BeginInit();
            this.SuspendLayout();
            // 
            // lbSpotOrFuturePrice
            // 
            this.lbSpotOrFuturePrice.Location = new System.Drawing.Point(8, 60);
            this.lbSpotOrFuturePrice.Name = "lbSpotOrFuturePrice";
            this.lbSpotOrFuturePrice.Size = new System.Drawing.Size(112, 16);
            this.lbSpotOrFuturePrice.TabIndex = 7;
            this.lbSpotOrFuturePrice.Text = "Spot Price:";
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(8, 80);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(72, 16);
            this.label5.TabIndex = 10;
            this.label5.Text = "Strike Price:";
            // 
            // label6
            // 
            this.label6.BackColor = System.Drawing.Color.Gainsboro;
            this.label6.Location = new System.Drawing.Point(8, 40);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(104, 16);
            this.label6.TabIndex = 12;
            this.label6.Text = "TimeToExp";
            // 
            // label8
            // 
            this.label8.BackColor = System.Drawing.Color.Gainsboro;
            this.label8.Location = new System.Drawing.Point(8, 98);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(104, 16);
            this.label8.TabIndex = 16;
            this.label8.Text = "Volatility, %:";
            // 
            // tbVolatility
            // 
            this.tbVolatility.BackColor = System.Drawing.SystemColors.Window;
            this.tbVolatility.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbVolatility.Location = new System.Drawing.Point(136, 94);
            this.tbVolatility.Name = "tbVolatility";
            this.tbVolatility.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbVolatility.Size = new System.Drawing.Size(88, 20);
            this.tbVolatility.TabIndex = 7;
            this.tbVolatility.Text = "0.00";
            this.tbVolatility.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbVolatility_KeyPress);
            this.tbVolatility.TextChanged += new System.EventHandler(this.tbVolatility_TextChanged);
            // 
            // label9
            // 
            this.label9.BackColor = System.Drawing.Color.Gainsboro;
            this.label9.Location = new System.Drawing.Point(8, 114);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(104, 16);
            this.label9.TabIndex = 18;
            this.label9.Text = "Interest Rate, %:";
            // 
            // tbDomesticRate
            // 
            this.tbDomesticRate.BackColor = System.Drawing.SystemColors.Window;
            this.tbDomesticRate.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbDomesticRate.Location = new System.Drawing.Point(136, 113);
            this.tbDomesticRate.Name = "tbDomesticRate";
            this.tbDomesticRate.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbDomesticRate.Size = new System.Drawing.Size(88, 20);
            this.tbDomesticRate.TabIndex = 8;
            this.tbDomesticRate.Text = "0.00";
            this.tbDomesticRate.TextChanged += new System.EventHandler(this.tbDomesticRate_TextChanged);
            // 
            // gbResults
            // 
            this.gbResults.Controls.Add(this.groupControl2);
            this.gbResults.Controls.Add(this.groupControl1);
            this.gbResults.Controls.Add(this.label46);
            this.gbResults.Controls.Add(this.label48);
            this.gbResults.Controls.Add(this.label47);
            this.gbResults.Controls.Add(this.label37);
            this.gbResults.Controls.Add(this.label38);
            this.gbResults.Controls.Add(this.label39);
            this.gbResults.Controls.Add(this.label40);
            this.gbResults.Controls.Add(this.label41);
            this.gbResults.Controls.Add(this.label42);
            this.gbResults.Controls.Add(this.label43);
            this.gbResults.Controls.Add(this.label44);
            this.gbResults.Controls.Add(this.label45);
            this.gbResults.Location = new System.Drawing.Point(264, 136);
            this.gbResults.Name = "gbResults";
            this.gbResults.Size = new System.Drawing.Size(296, 232);
            this.gbResults.TabIndex = 80;
            this.gbResults.TabStop = false;
            this.gbResults.Text = "Results:";
            // 
            // groupControl2
            // 
            this.groupControl2.Controls.Add(this.tbResPImpliedVola);
            this.groupControl2.Controls.Add(this.tbResPTheta);
            this.groupControl2.Controls.Add(this.tbResPGammaTheta);
            this.groupControl2.Controls.Add(this.tbResPPrice);
            this.groupControl2.Controls.Add(this.tbResPGammaVega);
            this.groupControl2.Controls.Add(this.tbResPDelta);
            this.groupControl2.Controls.Add(this.tbResPDeltaTheta);
            this.groupControl2.Controls.Add(this.tbResPGamma);
            this.groupControl2.Controls.Add(this.tbResPDeltaVega);
            this.groupControl2.Controls.Add(this.tbResPVega);
            this.groupControl2.Controls.Add(this.tbResPAlpha);
            this.groupControl2.Controls.Add(this.tbResPRho);
            this.groupControl2.Location = new System.Drawing.Point(193, 9);
            this.groupControl2.Name = "groupControl2";
            this.groupControl2.Size = new System.Drawing.Size(90, 217);
            this.groupControl2.TabIndex = 115;
            this.groupControl2.Text = "Put";
            // 
            // tbResPImpliedVola
            // 
            this.tbResPImpliedVola.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPImpliedVola.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPImpliedVola.Location = new System.Drawing.Point(5, 199);
            this.tbResPImpliedVola.Name = "tbResPImpliedVola";
            this.tbResPImpliedVola.ReadOnly = true;
            this.tbResPImpliedVola.Size = new System.Drawing.Size(80, 13);
            this.tbResPImpliedVola.TabIndex = 73;
            this.tbResPImpliedVola.Text = "0.00";
            this.tbResPImpliedVola.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPTheta
            // 
            this.tbResPTheta.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPTheta.Location = new System.Drawing.Point(5, 71);
            this.tbResPTheta.Name = "tbResPTheta";
            this.tbResPTheta.ReadOnly = true;
            this.tbResPTheta.Size = new System.Drawing.Size(80, 13);
            this.tbResPTheta.TabIndex = 65;
            this.tbResPTheta.Text = "0.00";
            this.tbResPTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPGammaTheta
            // 
            this.tbResPGammaTheta.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPGammaTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPGammaTheta.Location = new System.Drawing.Point(5, 183);
            this.tbResPGammaTheta.Name = "tbResPGammaTheta";
            this.tbResPGammaTheta.ReadOnly = true;
            this.tbResPGammaTheta.Size = new System.Drawing.Size(80, 13);
            this.tbResPGammaTheta.TabIndex = 72;
            this.tbResPGammaTheta.Text = "0.00";
            this.tbResPGammaTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPPrice
            // 
            this.tbResPPrice.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPPrice.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPPrice.Location = new System.Drawing.Point(5, 23);
            this.tbResPPrice.Name = "tbResPPrice";
            this.tbResPPrice.ReadOnly = true;
            this.tbResPPrice.Size = new System.Drawing.Size(80, 13);
            this.tbResPPrice.TabIndex = 62;
            this.tbResPPrice.Text = "0.00";
            this.tbResPPrice.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPGammaVega
            // 
            this.tbResPGammaVega.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPGammaVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPGammaVega.Location = new System.Drawing.Point(5, 167);
            this.tbResPGammaVega.Name = "tbResPGammaVega";
            this.tbResPGammaVega.ReadOnly = true;
            this.tbResPGammaVega.Size = new System.Drawing.Size(80, 13);
            this.tbResPGammaVega.TabIndex = 71;
            this.tbResPGammaVega.Text = "0.00";
            this.tbResPGammaVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPDelta
            // 
            this.tbResPDelta.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPDelta.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPDelta.Location = new System.Drawing.Point(5, 39);
            this.tbResPDelta.Name = "tbResPDelta";
            this.tbResPDelta.ReadOnly = true;
            this.tbResPDelta.Size = new System.Drawing.Size(80, 13);
            this.tbResPDelta.TabIndex = 63;
            this.tbResPDelta.Text = "0.00";
            this.tbResPDelta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPDeltaTheta
            // 
            this.tbResPDeltaTheta.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPDeltaTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPDeltaTheta.Location = new System.Drawing.Point(5, 151);
            this.tbResPDeltaTheta.Name = "tbResPDeltaTheta";
            this.tbResPDeltaTheta.ReadOnly = true;
            this.tbResPDeltaTheta.Size = new System.Drawing.Size(80, 13);
            this.tbResPDeltaTheta.TabIndex = 70;
            this.tbResPDeltaTheta.Text = "0.00";
            this.tbResPDeltaTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPGamma
            // 
            this.tbResPGamma.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPGamma.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPGamma.Location = new System.Drawing.Point(5, 55);
            this.tbResPGamma.Name = "tbResPGamma";
            this.tbResPGamma.ReadOnly = true;
            this.tbResPGamma.Size = new System.Drawing.Size(80, 13);
            this.tbResPGamma.TabIndex = 64;
            this.tbResPGamma.Text = "0.00";
            this.tbResPGamma.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPDeltaVega
            // 
            this.tbResPDeltaVega.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPDeltaVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPDeltaVega.Location = new System.Drawing.Point(5, 135);
            this.tbResPDeltaVega.Name = "tbResPDeltaVega";
            this.tbResPDeltaVega.ReadOnly = true;
            this.tbResPDeltaVega.Size = new System.Drawing.Size(80, 13);
            this.tbResPDeltaVega.TabIndex = 69;
            this.tbResPDeltaVega.Text = "0.00";
            this.tbResPDeltaVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPVega
            // 
            this.tbResPVega.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPVega.Location = new System.Drawing.Point(5, 87);
            this.tbResPVega.Name = "tbResPVega";
            this.tbResPVega.ReadOnly = true;
            this.tbResPVega.Size = new System.Drawing.Size(80, 13);
            this.tbResPVega.TabIndex = 66;
            this.tbResPVega.Text = "0.00";
            this.tbResPVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPAlpha
            // 
            this.tbResPAlpha.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPAlpha.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPAlpha.Location = new System.Drawing.Point(5, 119);
            this.tbResPAlpha.Name = "tbResPAlpha";
            this.tbResPAlpha.ReadOnly = true;
            this.tbResPAlpha.Size = new System.Drawing.Size(80, 13);
            this.tbResPAlpha.TabIndex = 68;
            this.tbResPAlpha.Text = "0.00";
            this.tbResPAlpha.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResPRho
            // 
            this.tbResPRho.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResPRho.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResPRho.Location = new System.Drawing.Point(5, 103);
            this.tbResPRho.Name = "tbResPRho";
            this.tbResPRho.ReadOnly = true;
            this.tbResPRho.Size = new System.Drawing.Size(80, 13);
            this.tbResPRho.TabIndex = 67;
            this.tbResPRho.Text = "0.00";
            this.tbResPRho.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // groupControl1
            // 
            this.groupControl1.Controls.Add(this.tbResCImpliedVola);
            this.groupControl1.Controls.Add(this.tbResCDeltaTheta);
            this.groupControl1.Controls.Add(this.tbResCGammaTheta);
            this.groupControl1.Controls.Add(this.tbResCPrice);
            this.groupControl1.Controls.Add(this.tbResCGammaVega);
            this.groupControl1.Controls.Add(this.tbResCDelta);
            this.groupControl1.Controls.Add(this.tbResCGamma);
            this.groupControl1.Controls.Add(this.tbResCDeltaVega);
            this.groupControl1.Controls.Add(this.tbResCTheta);
            this.groupControl1.Controls.Add(this.tbResCAlpha);
            this.groupControl1.Controls.Add(this.tbResCVega);
            this.groupControl1.Controls.Add(this.tbResCRho);
            this.groupControl1.Location = new System.Drawing.Point(99, 9);
            this.groupControl1.LookAndFeel.Style = DevExpress.LookAndFeel.LookAndFeelStyle.Style3D;
            this.groupControl1.Name = "groupControl1";
            this.groupControl1.Size = new System.Drawing.Size(90, 217);
            this.groupControl1.TabIndex = 114;
            this.groupControl1.Text = "Call";
            // 
            // tbResCImpliedVola
            // 
            this.tbResCImpliedVola.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCImpliedVola.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCImpliedVola.Location = new System.Drawing.Point(5, 199);
            this.tbResCImpliedVola.Name = "tbResCImpliedVola";
            this.tbResCImpliedVola.ReadOnly = true;
            this.tbResCImpliedVola.Size = new System.Drawing.Size(80, 13);
            this.tbResCImpliedVola.TabIndex = 61;
            this.tbResCImpliedVola.Text = "0.00";
            this.tbResCImpliedVola.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCDeltaTheta
            // 
            this.tbResCDeltaTheta.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCDeltaTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCDeltaTheta.Location = new System.Drawing.Point(5, 151);
            this.tbResCDeltaTheta.Name = "tbResCDeltaTheta";
            this.tbResCDeltaTheta.ReadOnly = true;
            this.tbResCDeltaTheta.Size = new System.Drawing.Size(80, 13);
            this.tbResCDeltaTheta.TabIndex = 58;
            this.tbResCDeltaTheta.Text = "0.00";
            this.tbResCDeltaTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCGammaTheta
            // 
            this.tbResCGammaTheta.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCGammaTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCGammaTheta.Location = new System.Drawing.Point(5, 183);
            this.tbResCGammaTheta.Name = "tbResCGammaTheta";
            this.tbResCGammaTheta.ReadOnly = true;
            this.tbResCGammaTheta.Size = new System.Drawing.Size(80, 13);
            this.tbResCGammaTheta.TabIndex = 60;
            this.tbResCGammaTheta.Text = "0.00";
            this.tbResCGammaTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCPrice
            // 
            this.tbResCPrice.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCPrice.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCPrice.Location = new System.Drawing.Point(5, 23);
            this.tbResCPrice.Name = "tbResCPrice";
            this.tbResCPrice.ReadOnly = true;
            this.tbResCPrice.Size = new System.Drawing.Size(80, 13);
            this.tbResCPrice.TabIndex = 50;
            this.tbResCPrice.Text = "0.00";
            this.tbResCPrice.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCGammaVega
            // 
            this.tbResCGammaVega.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCGammaVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCGammaVega.Location = new System.Drawing.Point(5, 167);
            this.tbResCGammaVega.Name = "tbResCGammaVega";
            this.tbResCGammaVega.ReadOnly = true;
            this.tbResCGammaVega.Size = new System.Drawing.Size(80, 13);
            this.tbResCGammaVega.TabIndex = 59;
            this.tbResCGammaVega.Text = "0.00";
            this.tbResCGammaVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCDelta
            // 
            this.tbResCDelta.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCDelta.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCDelta.Location = new System.Drawing.Point(5, 39);
            this.tbResCDelta.Name = "tbResCDelta";
            this.tbResCDelta.ReadOnly = true;
            this.tbResCDelta.Size = new System.Drawing.Size(80, 13);
            this.tbResCDelta.TabIndex = 51;
            this.tbResCDelta.Text = "0.00";
            this.tbResCDelta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCGamma
            // 
            this.tbResCGamma.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCGamma.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCGamma.Location = new System.Drawing.Point(5, 55);
            this.tbResCGamma.Name = "tbResCGamma";
            this.tbResCGamma.ReadOnly = true;
            this.tbResCGamma.Size = new System.Drawing.Size(80, 13);
            this.tbResCGamma.TabIndex = 52;
            this.tbResCGamma.Text = "0.00";
            this.tbResCGamma.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCDeltaVega
            // 
            this.tbResCDeltaVega.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCDeltaVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCDeltaVega.Location = new System.Drawing.Point(5, 135);
            this.tbResCDeltaVega.Name = "tbResCDeltaVega";
            this.tbResCDeltaVega.ReadOnly = true;
            this.tbResCDeltaVega.Size = new System.Drawing.Size(80, 13);
            this.tbResCDeltaVega.TabIndex = 57;
            this.tbResCDeltaVega.Text = "0.00";
            this.tbResCDeltaVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCTheta
            // 
            this.tbResCTheta.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCTheta.Location = new System.Drawing.Point(5, 71);
            this.tbResCTheta.Name = "tbResCTheta";
            this.tbResCTheta.ReadOnly = true;
            this.tbResCTheta.Size = new System.Drawing.Size(80, 13);
            this.tbResCTheta.TabIndex = 53;
            this.tbResCTheta.Text = "0.00";
            this.tbResCTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCAlpha
            // 
            this.tbResCAlpha.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCAlpha.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCAlpha.Location = new System.Drawing.Point(5, 119);
            this.tbResCAlpha.Name = "tbResCAlpha";
            this.tbResCAlpha.ReadOnly = true;
            this.tbResCAlpha.Size = new System.Drawing.Size(80, 13);
            this.tbResCAlpha.TabIndex = 56;
            this.tbResCAlpha.Text = "0.00";
            this.tbResCAlpha.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCVega
            // 
            this.tbResCVega.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCVega.Location = new System.Drawing.Point(5, 87);
            this.tbResCVega.Name = "tbResCVega";
            this.tbResCVega.ReadOnly = true;
            this.tbResCVega.Size = new System.Drawing.Size(80, 13);
            this.tbResCVega.TabIndex = 54;
            this.tbResCVega.Text = "0.00";
            this.tbResCVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbResCRho
            // 
            this.tbResCRho.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tbResCRho.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbResCRho.Location = new System.Drawing.Point(5, 103);
            this.tbResCRho.Name = "tbResCRho";
            this.tbResCRho.ReadOnly = true;
            this.tbResCRho.Size = new System.Drawing.Size(80, 13);
            this.tbResCRho.TabIndex = 55;
            this.tbResCRho.Text = "0.00";
            this.tbResCRho.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label46
            // 
            this.label46.Location = new System.Drawing.Point(8, 61);
            this.label46.Name = "label46";
            this.label46.Size = new System.Drawing.Size(54, 16);
            this.label46.TabIndex = 12;
            this.label46.Text = "Gamma:";
            // 
            // label48
            // 
            this.label48.Location = new System.Drawing.Point(10, 29);
            this.label48.Name = "label48";
            this.label48.Size = new System.Drawing.Size(62, 16);
            this.label48.TabIndex = 8;
            this.label48.Text = "Price:";
            // 
            // label47
            // 
            this.label47.Location = new System.Drawing.Point(8, 45);
            this.label47.Name = "label47";
            this.label47.Size = new System.Drawing.Size(62, 16);
            this.label47.TabIndex = 10;
            this.label47.Text = "Delta:";
            // 
            // label37
            // 
            this.label37.Location = new System.Drawing.Point(10, 208);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(88, 16);
            this.label37.TabIndex = 30;
            this.label37.Text = "Implied Vola, %:";
            // 
            // label38
            // 
            this.label38.Location = new System.Drawing.Point(10, 192);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(87, 16);
            this.label38.TabIndex = 28;
            this.label38.Text = "Gamma Theta:";
            // 
            // label39
            // 
            this.label39.Location = new System.Drawing.Point(10, 176);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(80, 16);
            this.label39.TabIndex = 26;
            this.label39.Text = "Gamma Vega:";
            // 
            // label40
            // 
            this.label40.Location = new System.Drawing.Point(10, 157);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(72, 16);
            this.label40.TabIndex = 24;
            this.label40.Text = "Delta Theta:";
            // 
            // label41
            // 
            this.label41.Location = new System.Drawing.Point(10, 141);
            this.label41.Name = "label41";
            this.label41.Size = new System.Drawing.Size(80, 16);
            this.label41.TabIndex = 22;
            this.label41.Text = "Delta Vega:";
            // 
            // label42
            // 
            this.label42.Location = new System.Drawing.Point(10, 125);
            this.label42.Name = "label42";
            this.label42.Size = new System.Drawing.Size(54, 16);
            this.label42.TabIndex = 20;
            this.label42.Text = "Alpha:";
            // 
            // label43
            // 
            this.label43.Location = new System.Drawing.Point(10, 109);
            this.label43.Name = "label43";
            this.label43.Size = new System.Drawing.Size(54, 16);
            this.label43.TabIndex = 18;
            this.label43.Text = "Rho:";
            // 
            // label44
            // 
            this.label44.Location = new System.Drawing.Point(8, 93);
            this.label44.Name = "label44";
            this.label44.Size = new System.Drawing.Size(54, 16);
            this.label44.TabIndex = 16;
            this.label44.Text = "Vega:";
            // 
            // label45
            // 
            this.label45.Location = new System.Drawing.Point(8, 77);
            this.label45.Name = "label45";
            this.label45.Size = new System.Drawing.Size(54, 16);
            this.label45.TabIndex = 14;
            this.label45.Text = "Theta:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.tbContinousRate);
            this.groupBox1.Controls.Add(this.rbContinous);
            this.groupBox1.Controls.Add(this.rbDiscreteDividends);
            this.groupBox1.Controls.Add(this.gbDividends);
            this.groupBox1.Location = new System.Drawing.Point(9, 204);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(249, 208);
            this.groupBox1.TabIndex = 13;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Dividend Yield:";
            // 
            // tbContinousRate
            // 
            this.tbContinousRate.BackColor = System.Drawing.SystemColors.Window;
            this.tbContinousRate.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbContinousRate.Location = new System.Drawing.Point(128, 16);
            this.tbContinousRate.Name = "tbContinousRate";
            this.tbContinousRate.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbContinousRate.Size = new System.Drawing.Size(88, 20);
            this.tbContinousRate.TabIndex = 12;
            this.tbContinousRate.Text = "0.00";
            this.tbContinousRate.TextChanged += new System.EventHandler(this.tbContinousRate_TextChanged);
            // 
            // rbContinous
            // 
            this.rbContinous.Checked = true;
            this.rbContinous.Location = new System.Drawing.Point(8, 16);
            this.rbContinous.Name = "rbContinous";
            this.rbContinous.Size = new System.Drawing.Size(120, 16);
            this.rbContinous.TabIndex = 11;
            this.rbContinous.TabStop = true;
            this.rbContinous.Text = "Continuous Rate %:";
            this.rbContinous.CheckedChanged += new System.EventHandler(this.rbContinous_CheckedChanged);
            // 
            // rbDiscreteDividends
            // 
            this.rbDiscreteDividends.Location = new System.Drawing.Point(8, 40);
            this.rbDiscreteDividends.Name = "rbDiscreteDividends";
            this.rbDiscreteDividends.Size = new System.Drawing.Size(104, 16);
            this.rbDiscreteDividends.TabIndex = 13;
            this.rbDiscreteDividends.Text = "Discrete:";
            this.rbDiscreteDividends.CheckedChanged += new System.EventHandler(this.rbDiscreteDividends_CheckedChanged);
            // 
            // gbDividends
            // 
            this.gbDividends.Controls.Add(this.btnSetCustomDividends);
            this.gbDividends.Controls.Add(this.rCustomDivs);
            this.gbDividends.Controls.Add(this.gbPeriodicDivs);
            this.gbDividends.Controls.Add(this.rPeriodic);
            this.gbDividends.Enabled = false;
            this.gbDividends.Location = new System.Drawing.Point(4, 56);
            this.gbDividends.Name = "gbDividends";
            this.gbDividends.Size = new System.Drawing.Size(236, 144);
            this.gbDividends.TabIndex = 17;
            this.gbDividends.TabStop = false;
            // 
            // btnSetCustomDividends
            // 
            this.btnSetCustomDividends.Location = new System.Drawing.Point(80, 112);
            this.btnSetCustomDividends.Name = "btnSetCustomDividends";
            this.btnSetCustomDividends.Size = new System.Drawing.Size(144, 23);
            this.btnSetCustomDividends.TabIndex = 19;
            this.btnSetCustomDividends.Text = "Show";
            this.btnSetCustomDividends.Click += new System.EventHandler(this.btnSetCustomDividends_Click);
            // 
            // rCustomDivs
            // 
            this.rCustomDivs.Location = new System.Drawing.Point(8, 112);
            this.rCustomDivs.Name = "rCustomDivs";
            this.rCustomDivs.Size = new System.Drawing.Size(64, 20);
            this.rCustomDivs.TabIndex = 18;
            this.rCustomDivs.Text = "Stream:";
            this.rCustomDivs.CheckedChanged += new System.EventHandler(this.rCustomDivs_CheckedChanged_1);
            // 
            // gbPeriodicDivs
            // 
            this.gbPeriodicDivs.Controls.Add(this.dtpDivDate);
            this.gbPeriodicDivs.Controls.Add(this.cbDivFreq);
            this.gbPeriodicDivs.Controls.Add(this.label12);
            this.gbPeriodicDivs.Controls.Add(this.label4);
            this.gbPeriodicDivs.Controls.Add(this.label3);
            this.gbPeriodicDivs.Controls.Add(this.tbDivAmount);
            this.gbPeriodicDivs.Enabled = false;
            this.gbPeriodicDivs.Location = new System.Drawing.Point(5, 24);
            this.gbPeriodicDivs.Name = "gbPeriodicDivs";
            this.gbPeriodicDivs.Size = new System.Drawing.Size(219, 82);
            this.gbPeriodicDivs.TabIndex = 19;
            this.gbPeriodicDivs.TabStop = false;
            // 
            // dtpDivDate
            // 
            this.dtpDivDate.EditValue = new System.DateTime(2005, 12, 7, 0, 0, 0, 0);
            this.dtpDivDate.Location = new System.Drawing.Point(118, 8);
            this.dtpDivDate.Name = "dtpDivDate";
            this.dtpDivDate.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.dtpDivDate.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.dtpDivDate.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.dtpDivDate.Size = new System.Drawing.Size(88, 20);
            this.dtpDivDate.TabIndex = 15;
            this.dtpDivDate.DateTimeChanged += new System.EventHandler(this.dtpDivDate_ValueChanged);
            // 
            // cbDivFreq
            // 
            this.cbDivFreq.Location = new System.Drawing.Point(118, 50);
            this.cbDivFreq.Name = "cbDivFreq";
            this.cbDivFreq.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.cbDivFreq.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.cbDivFreq.Properties.Items.AddRange(new object[] {
            "Monthly",
            "Quarterly",
            "Semiannually",
            "Annually"});
            this.cbDivFreq.Properties.TextEditStyle = DevExpress.XtraEditors.Controls.TextEditStyles.DisableTextEditor;
            this.cbDivFreq.Properties.ValidateOnEnterKey = true;
            this.cbDivFreq.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.cbDivFreq.Size = new System.Drawing.Size(88, 20);
            this.cbDivFreq.TabIndex = 17;
            this.cbDivFreq.TextChanged += new System.EventHandler(this.cbDivFreq_TextChanged);
            // 
            // label12
            // 
            this.label12.Location = new System.Drawing.Point(4, 14);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(100, 14);
            this.label12.TabIndex = 20;
            this.label12.Text = "Dividends Date:";
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(4, 53);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(115, 15);
            this.label4.TabIndex = 18;
            this.label4.Text = "Dividends Frequency:";
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(3, 31);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(104, 15);
            this.label3.TabIndex = 17;
            this.label3.Text = "Dividends Amount:";
            // 
            // tbDivAmount
            // 
            this.tbDivAmount.BackColor = System.Drawing.SystemColors.Window;
            this.tbDivAmount.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbDivAmount.Location = new System.Drawing.Point(118, 29);
            this.tbDivAmount.Name = "tbDivAmount";
            this.tbDivAmount.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbDivAmount.Size = new System.Drawing.Size(88, 20);
            this.tbDivAmount.TabIndex = 16;
            this.tbDivAmount.Text = "0.00";
            this.tbDivAmount.TextChanged += new System.EventHandler(this.tbDivAmount_TextChanged);
            // 
            // rPeriodic
            // 
            this.rPeriodic.Checked = true;
            this.rPeriodic.Location = new System.Drawing.Point(8, 8);
            this.rPeriodic.Name = "rPeriodic";
            this.rPeriodic.Size = new System.Drawing.Size(80, 16);
            this.rPeriodic.TabIndex = 14;
            this.rPeriodic.TabStop = true;
            this.rPeriodic.Text = "Periodic:";
            this.rPeriodic.CheckedChanged += new System.EventHandler(this.rPeriodic_CheckedChanged);
            // 
            // label13
            // 
            this.label13.Location = new System.Drawing.Point(264, 16);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(80, 16);
            this.label13.TabIndex = 29;
            this.label13.Text = "Symbol:";
            // 
            // tbSymbol
            // 
            this.tbSymbol.BackColor = System.Drawing.SystemColors.Window;
            this.tbSymbol.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbSymbol.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.tbSymbol.Location = new System.Drawing.Point(264, 37);
            this.tbSymbol.Name = "tbSymbol";
            this.tbSymbol.Size = new System.Drawing.Size(88, 21);
            this.tbSymbol.TabIndex = 1;
            this.tbSymbol.Leave += new System.EventHandler(this.tbSymbol_Leave);
            this.tbSymbol.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbSymbol_KeyPress);
            this.tbSymbol.Validated += new System.EventHandler(this.tbSymbol_Validate);
            this.tbSymbol.TextChanged += new System.EventHandler(this.tbSymbol_TextChanged);
            // 
            // gbCalcModel
            // 
            this.gbCalcModel.Controls.Add(this.rbStandart);
            this.gbCalcModel.Controls.Add(this.rbOptimized);
            this.gbCalcModel.Location = new System.Drawing.Point(360, 51);
            this.gbCalcModel.Name = "gbCalcModel";
            this.gbCalcModel.Size = new System.Drawing.Size(200, 42);
            this.gbCalcModel.TabIndex = 22;
            this.gbCalcModel.TabStop = false;
            this.gbCalcModel.Text = "Model:";
            // 
            // rbStandart
            // 
            this.rbStandart.Location = new System.Drawing.Point(11, 16);
            this.rbStandart.Name = "rbStandart";
            this.rbStandart.Size = new System.Drawing.Size(85, 16);
            this.rbStandart.TabIndex = 27;
            this.rbStandart.Text = "Standard";
            this.rbStandart.CheckedChanged += new System.EventHandler(this.rbStandart_CheckedChanged);
            // 
            // rbOptimized
            // 
            this.rbOptimized.Checked = true;
            this.rbOptimized.Location = new System.Drawing.Point(112, 16);
            this.rbOptimized.Name = "rbOptimized";
            this.rbOptimized.Size = new System.Drawing.Size(79, 16);
            this.rbOptimized.TabIndex = 28;
            this.rbOptimized.TabStop = true;
            this.rbOptimized.Text = "Optimized";
            this.rbOptimized.CheckedChanged += new System.EventHandler(this.rbOptimized_CheckedChanged);
            // 
            // tbCOptionPrice
            // 
            this.tbCOptionPrice.BackColor = System.Drawing.SystemColors.Window;
            this.tbCOptionPrice.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbCOptionPrice.Location = new System.Drawing.Point(128, 19);
            this.tbCOptionPrice.Name = "tbCOptionPrice";
            this.tbCOptionPrice.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbCOptionPrice.Size = new System.Drawing.Size(88, 20);
            this.tbCOptionPrice.TabIndex = 9;
            this.tbCOptionPrice.Text = "0.00";
            this.tbCOptionPrice.TextChanged += new System.EventHandler(this.tbCOptionPrice_TextChanged);
            // 
            // oleDbConnection
            // 
            this.oleDbConnection.ConnectionString = "Provider=SQLNCLI.1;Data Source=GROMADSKY;Integrated Security=SSPI;Initial Catalog" +
                "=CIBC";
            // 
            // oleDbCommand
            // 
            this.oleDbCommand.Connection = this.oleDbConnection;
            this.oleDbCommand.Parameters.AddRange(new System.Data.OleDb.OleDbParameter[] {
            new System.Data.OleDb.OleDbParameter("@sInSymbol", System.Data.OleDb.OleDbType.VarChar, 20),
            new System.Data.OleDb.OleDbParameter("@iRowCount", System.Data.OleDb.OleDbType.Integer, 0, System.Data.ParameterDirection.Output, false, ((byte)(0)), ((byte)(0)), "", System.Data.DataRowVersion.Current, null),
            new System.Data.OleDb.OleDbParameter("@iStockID", System.Data.OleDb.OleDbType.Integer, 0, System.Data.ParameterDirection.Output, false, ((byte)(0)), ((byte)(0)), "", System.Data.DataRowVersion.Current, null)});
            // 
            // ReadCommand
            // 
            this.ReadCommand.Connection = this.oleDbConnection;
            // 
            // label16
            // 
            this.label16.Location = new System.Drawing.Point(86, 39);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(32, 16);
            this.label16.TabIndex = 46;
            this.label16.Text = "Put:";
            // 
            // tbPOptionPrice
            // 
            this.tbPOptionPrice.BackColor = System.Drawing.SystemColors.Window;
            this.tbPOptionPrice.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbPOptionPrice.Location = new System.Drawing.Point(128, 37);
            this.tbPOptionPrice.Name = "tbPOptionPrice";
            this.tbPOptionPrice.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbPOptionPrice.Size = new System.Drawing.Size(88, 20);
            this.tbPOptionPrice.TabIndex = 10;
            this.tbPOptionPrice.Text = "0.00";
            this.tbPOptionPrice.TextChanged += new System.EventHandler(this.tbPOptionPrice_TextChanged);
            // 
            // label15
            // 
            this.label15.Location = new System.Drawing.Point(86, 19);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(32, 15);
            this.label15.TabIndex = 44;
            this.label15.Text = "Call:";
            // 
            // FeedTimer
            // 
            this.FeedTimer.Interval = 1000;
            this.FeedTimer.SynchronizingObject = this;
            this.FeedTimer.Elapsed += new System.Timers.ElapsedEventHandler(this.FeedTimer_Elapsed);
            // 
            // label10
            // 
            this.label10.Location = new System.Drawing.Point(264, 96);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(80, 16);
            this.label10.TabIndex = 48;
            this.label10.Text = "Symbol type:";
            // 
            // InitTimer
            // 
            this.InitTimer.Interval = 10;
            this.InitTimer.SynchronizingObject = this;
            this.InitTimer.Elapsed += new System.Timers.ElapsedEventHandler(this.InitTimer_Elapsed);
            // 
            // ReadExpCommand
            // 
            this.ReadExpCommand.CommandType = System.Data.CommandType.StoredProcedure;
            this.ReadExpCommand.Connection = this.oleDbConnection;
            this.ReadExpCommand.Parameters.AddRange(new System.Data.OleDb.OleDbParameter[] {
            new System.Data.OleDb.OleDbParameter("@iContractID", System.Data.OleDb.OleDbType.Integer),
            new System.Data.OleDb.OleDbParameter("@iTraderID", System.Data.OleDb.OleDbType.Integer),
            new System.Data.OleDb.OleDbParameter("@iGroupID", System.Data.OleDb.OleDbType.Integer),
            new System.Data.OleDb.OleDbParameter("@dtMinExpiry", System.Data.OleDb.OleDbType.Date)});
            // 
            // ReadOptsCommand
            // 
            this.ReadOptsCommand.Connection = this.oleDbConnection;
            this.ReadOptsCommand.Parameters.AddRange(new System.Data.OleDb.OleDbParameter[] {
            new System.Data.OleDb.OleDbParameter("@iUnderlyingID", System.Data.OleDb.OleDbType.Integer),
            new System.Data.OleDb.OleDbParameter("@iExpiryID", System.Data.OleDb.OleDbType.Date)});
            // 
            // cbFutureUseSpot
            // 
            this.cbFutureUseSpot.BackColor = System.Drawing.Color.Gainsboro;
            this.cbFutureUseSpot.Controls.Add(this.tbDomesticRate);
            this.cbFutureUseSpot.Controls.Add(this.tbVolatility);
            this.cbFutureUseSpot.Controls.Add(this.label9);
            this.cbFutureUseSpot.Controls.Add(this.tbStrikePrice);
            this.cbFutureUseSpot.Controls.Add(this.label8);
            this.cbFutureUseSpot.Controls.Add(this.tbSpotPrice);
            this.cbFutureUseSpot.Controls.Add(this.cbSymbolType);
            this.cbFutureUseSpot.Controls.Add(this.groupControl3);
            this.cbFutureUseSpot.Controls.Add(this.tbDTE);
            this.cbFutureUseSpot.Controls.Add(this.btnCalc);
            this.cbFutureUseSpot.Controls.Add(this.btnClose);
            this.cbFutureUseSpot.Controls.Add(this.dtpExpirationDate);
            this.cbFutureUseSpot.Controls.Add(this.gbFuturePrice);
            this.cbFutureUseSpot.Controls.Add(this.label7);
            this.cbFutureUseSpot.Controls.Add(this.StyleGroupBox);
            this.cbFutureUseSpot.Controls.Add(this.tbSymbol);
            this.cbFutureUseSpot.Controls.Add(this.label13);
            this.cbFutureUseSpot.Controls.Add(this.label10);
            this.cbFutureUseSpot.Controls.Add(this.gbCalcModel);
            this.cbFutureUseSpot.Controls.Add(this.gbResults);
            this.cbFutureUseSpot.Controls.Add(this.lbSpotOrFuturePrice);
            this.cbFutureUseSpot.Controls.Add(this.label5);
            this.cbFutureUseSpot.Controls.Add(this.groupBox1);
            this.cbFutureUseSpot.Controls.Add(this.label6);
            this.cbFutureUseSpot.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.cbFutureUseSpot.Location = new System.Drawing.Point(0, 19);
            this.cbFutureUseSpot.Name = "cbFutureUseSpot";
            this.cbFutureUseSpot.Size = new System.Drawing.Size(572, 417);
            this.cbFutureUseSpot.TabIndex = 101;
            this.cbFutureUseSpot.TabStop = false;
            // 
            // tbStrikePrice
            // 
            this.tbStrikePrice.EditValue = "0.00";
            this.tbStrikePrice.Location = new System.Drawing.Point(136, 75);
            this.tbStrikePrice.Name = "tbStrikePrice";
            this.tbStrikePrice.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.tbStrikePrice.Properties.Mask.EditMask = "n";
            this.tbStrikePrice.Properties.Mask.MaskType = DevExpress.XtraEditors.Mask.MaskType.Numeric;
            this.tbStrikePrice.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbStrikePrice.Size = new System.Drawing.Size(88, 20);
            this.tbStrikePrice.TabIndex = 6;
            this.tbStrikePrice.TextChanged += new System.EventHandler(this.tbSpotPrice_TextChanged);
            // 
            // tbSpotPrice
            // 
            this.tbSpotPrice.EditValue = "0.00";
            this.tbSpotPrice.Location = new System.Drawing.Point(136, 56);
            this.tbSpotPrice.Name = "tbSpotPrice";
            this.tbSpotPrice.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.tbSpotPrice.Properties.Mask.EditMask = "c2";
            this.tbSpotPrice.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbSpotPrice.Size = new System.Drawing.Size(88, 20);
            this.tbSpotPrice.TabIndex = 5;
            this.tbSpotPrice.TextChanged += new System.EventHandler(this.tbSpotPrice_TextChanged);
            // 
            // cbSymbolType
            // 
            this.cbSymbolType.Location = new System.Drawing.Point(264, 114);
            this.cbSymbolType.Name = "cbSymbolType";
            this.cbSymbolType.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.cbSymbolType.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.cbSymbolType.Properties.Items.AddRange(new object[] {
            "Index/Stock",
            "Future"});
            this.cbSymbolType.Properties.TextEditStyle = DevExpress.XtraEditors.Controls.TextEditStyles.DisableTextEditor;
            this.cbSymbolType.Size = new System.Drawing.Size(88, 20);
            this.cbSymbolType.TabIndex = 2;
            this.cbSymbolType.SelectedIndexChanged += new System.EventHandler(this.cbSymbolType_SelectedIndexChanged);
            // 
            // groupControl3
            // 
            this.groupControl3.Controls.Add(this.label16);
            this.groupControl3.Controls.Add(this.tbCOptionPrice);
            this.groupControl3.Controls.Add(this.tbPOptionPrice);
            this.groupControl3.Controls.Add(this.label15);
            this.groupControl3.Location = new System.Drawing.Point(8, 134);
            this.groupControl3.LookAndFeel.SkinName = "The Asphalt World";
            this.groupControl3.LookAndFeel.UseDefaultLookAndFeel = false;
            this.groupControl3.Name = "groupControl3";
            this.groupControl3.Size = new System.Drawing.Size(249, 68);
            this.groupControl3.TabIndex = 9;
            this.groupControl3.Text = "Option price (for IV only):";
            // 
            // tbDTE
            // 
            this.tbDTE.EditValue = "0 days 00:00";
            this.tbDTE.Location = new System.Drawing.Point(107, 37);
            this.tbDTE.Name = "tbDTE";
            this.tbDTE.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.tbDTE.Properties.Mask.AutoComplete = DevExpress.XtraEditors.Mask.AutoCompleteType.Optimistic;
            this.tbDTE.Properties.Mask.EditMask = "(0\\d?\\d?\\d?\\d?) day\\(s\\) (0\\d|1\\d|2[0-4])\\:(0\\d|1\\d|2\\d|3\\d|4\\d|5\\d|6[0-0])";
            this.tbDTE.Properties.Mask.IgnoreMaskBlank = false;
            this.tbDTE.Properties.Mask.MaskType = DevExpress.XtraEditors.Mask.MaskType.RegEx;
            this.tbDTE.Properties.Mask.PlaceHolder = '0';
            this.tbDTE.Properties.Mask.UseMaskAsDisplayFormat = true;
            this.tbDTE.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbDTE.Size = new System.Drawing.Size(117, 20);
            this.tbDTE.TabIndex = 4;
            this.tbDTE.Leave += new System.EventHandler(this.tbDTE_Leave);
            this.tbDTE.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbDTE_KeyPress);
            this.tbDTE.TextChanged += new System.EventHandler(this.tbDTE_TextChanged);
            // 
            // btnCalc
            // 
            this.btnCalc.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnCalc.Location = new System.Drawing.Point(264, 379);
            this.btnCalc.Name = "btnCalc";
            this.btnCalc.Size = new System.Drawing.Size(215, 24);
            this.btnCalc.TabIndex = 114;
            this.btnCalc.Text = "Calculate";
            this.btnCalc.Click += new System.EventHandler(this.btnCalc_Click);
            // 
            // btnClose
            // 
            this.btnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnClose.Location = new System.Drawing.Point(485, 380);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 114;
            this.btnClose.Text = "Close";
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click_1);
            // 
            // dtpExpirationDate
            // 
            this.dtpExpirationDate.EditValue = new System.DateTime(2008, 2, 15, 17, 37, 9, 0);
            this.dtpExpirationDate.Location = new System.Drawing.Point(107, 12);
            this.dtpExpirationDate.Name = "dtpExpirationDate";
            this.dtpExpirationDate.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.dtpExpirationDate.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.dtpExpirationDate.Properties.DisplayFormat.FormatString = "d/M/yyyy hh:mm tt";
            this.dtpExpirationDate.Properties.DisplayFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dtpExpirationDate.Properties.EditFormat.FormatString = "d/M/yyyy hh:mm tt";
            this.dtpExpirationDate.Properties.EditFormat.FormatType = DevExpress.Utils.FormatType.DateTime;
            this.dtpExpirationDate.Properties.Mask.EditMask = "d/M/yyyy hh:mm tt";
            this.dtpExpirationDate.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.dtpExpirationDate.Size = new System.Drawing.Size(135, 20);
            this.dtpExpirationDate.TabIndex = 3;
            this.dtpExpirationDate.Leave += new System.EventHandler(this.dtpExpirationDate_Leave);
            this.dtpExpirationDate.EditValueChanged += new System.EventHandler(this.dtpExpirationDate_EditValueChanged);
            this.dtpExpirationDate.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.dtpExpirationDate_KeyPress);
            // 
            // gbFuturePrice
            // 
            this.gbFuturePrice.Controls.Add(this.cbUseSpotPrice);
            this.gbFuturePrice.Location = new System.Drawing.Point(360, 94);
            this.gbFuturePrice.Name = "gbFuturePrice";
            this.gbFuturePrice.Size = new System.Drawing.Size(200, 42);
            this.gbFuturePrice.TabIndex = 104;
            this.gbFuturePrice.TabStop = false;
            this.gbFuturePrice.Text = "Future:";
            // 
            // cbUseSpotPrice
            // 
            this.cbUseSpotPrice.EditValue = true;
            this.cbUseSpotPrice.Location = new System.Drawing.Point(11, 16);
            this.cbUseSpotPrice.Name = "cbUseSpotPrice";
            this.cbUseSpotPrice.Properties.Caption = "Spot Greeks";
            this.cbUseSpotPrice.Size = new System.Drawing.Size(180, 18);
            this.cbUseSpotPrice.TabIndex = 102;
            // 
            // label7
            // 
            this.label7.BackColor = System.Drawing.Color.Gainsboro;
            this.label7.Location = new System.Drawing.Point(8, 16);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(93, 16);
            this.label7.TabIndex = 103;
            this.label7.Text = "Expiration for OV:";
            // 
            // StyleGroupBox
            // 
            this.StyleGroupBox.Controls.Add(this.rbEuropean);
            this.StyleGroupBox.Controls.Add(this.rbAmerican);
            this.StyleGroupBox.Location = new System.Drawing.Point(360, 9);
            this.StyleGroupBox.Name = "StyleGroupBox";
            this.StyleGroupBox.Size = new System.Drawing.Size(200, 42);
            this.StyleGroupBox.TabIndex = 20;
            this.StyleGroupBox.TabStop = false;
            this.StyleGroupBox.Text = "Option Style";
            // 
            // rbEuropean
            // 
            this.rbEuropean.Location = new System.Drawing.Point(112, 16);
            this.rbEuropean.Name = "rbEuropean";
            this.rbEuropean.Size = new System.Drawing.Size(79, 16);
            this.rbEuropean.TabIndex = 1;
            this.rbEuropean.Text = "European";
            this.rbEuropean.CheckedChanged += new System.EventHandler(this.rbEuropean_CheckedChanged);
            // 
            // rbAmerican
            // 
            this.rbAmerican.Checked = true;
            this.rbAmerican.Location = new System.Drawing.Point(11, 16);
            this.rbAmerican.Name = "rbAmerican";
            this.rbAmerican.Size = new System.Drawing.Size(85, 16);
            this.rbAmerican.TabIndex = 0;
            this.rbAmerican.TabStop = true;
            this.rbAmerican.Text = "American";
            this.rbAmerican.CheckedChanged += new System.EventHandler(this.rbAmerican_CheckedChanged);
            // 
            // ReadCurveCommand
            // 
            this.ReadCurveCommand.Connection = this.oleDbConnection;
            this.ReadCurveCommand.Parameters.AddRange(new System.Data.OleDb.OleDbParameter[] {
            new System.Data.OleDb.OleDbParameter("@iCurveID", System.Data.OleDb.OleDbType.Integer)});
            // 
            // GetCustomDivs
            // 
            this.GetCustomDivs.CommandText = "dbo.[usp_MmEtsCustomDividend_Get]";
            this.GetCustomDivs.CommandType = System.Data.CommandType.StoredProcedure;
            this.GetCustomDivs.Connection = this.oleDbConnection;
            this.GetCustomDivs.Parameters.AddRange(new System.Data.OleDb.OleDbParameter[] {
            new System.Data.OleDb.OleDbParameter("@RETURN_VALUE", System.Data.OleDb.OleDbType.Integer, 4, System.Data.ParameterDirection.ReturnValue, false, ((byte)(0)), ((byte)(0)), "", System.Data.DataRowVersion.Current, ""),
            new System.Data.OleDb.OleDbParameter("@iStockID", System.Data.OleDb.OleDbType.Integer, 4, System.Data.ParameterDirection.Input, false, ((byte)(0)), ((byte)(0)), "", System.Data.DataRowVersion.Current, "0")});
            // 
            // ReadCustCommand
            // 
            this.ReadCustCommand.Connection = this.oleDbConnection;
            // 
            // AtmTimer
            // 
            this.AtmTimer.Interval = 11000;
            this.AtmTimer.SynchronizingObject = this;
            this.AtmTimer.Elapsed += new System.Timers.ElapsedEventHandler(this.AtmTimer_Elapsed);
            // 
            // barManager2
            // 
            this.barManager2.Bars.AddRange(new DevExpress.XtraBars.Bar[] {
            this.bar1,
            this.bar2});
            this.barManager2.Categories.AddRange(new DevExpress.XtraBars.BarManagerCategory[] {
            new DevExpress.XtraBars.BarManagerCategory("Built-in Menus", new System.Guid("b4efef5a-8151-46d6-a441-53365cdaabe6")),
            new DevExpress.XtraBars.BarManagerCategory("Commands", new System.Guid("c740b439-694a-4de3-b3b2-cf438d297684"))});
            this.barManager2.Controller = this.barAndDockingController1;
            this.barManager2.DockControls.Add(this.barDockControl1);
            this.barManager2.DockControls.Add(this.barDockControl2);
            this.barManager2.DockControls.Add(this.barDockControl3);
            this.barManager2.DockControls.Add(this.barDockControl4);
            this.barManager2.Form = this;
            this.barManager2.Images = this.imgMenu;
            this.barManager2.Items.AddRange(new DevExpress.XtraBars.BarItem[] {
            this.barStaticItem1,
            this.barStatusText,
            this.subFile,
            this.btnExit,
            this.subData,
            this.btnRefreshQuotes,
            this.btnRefreshVolatility,
            this.btnCalculate,
            this.subHelp,
            this.btnContents,
            this.btnAbout});
            this.barManager2.MainMenu = this.bar2;
            this.barManager2.MaxItemId = 12;
            this.barManager2.RepositoryItems.AddRange(new DevExpress.XtraEditors.Repository.RepositoryItem[] {
            this.repositoryItemTextEdit1});
            this.barManager2.StatusBar = this.bar1;
            // 
            // bar1
            // 
            this.bar1.Appearance.BackColor = System.Drawing.Color.Gainsboro;
            this.bar1.Appearance.Options.UseBackColor = true;
            this.bar1.BarName = "Custom 1";
            this.bar1.CanDockStyle = DevExpress.XtraBars.BarCanDockStyle.Bottom;
            this.bar1.DockCol = 0;
            this.bar1.DockRow = 0;
            this.bar1.DockStyle = DevExpress.XtraBars.BarDockStyle.Bottom;
            this.bar1.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(this.barStatusText)});
            this.bar1.OptionsBar.AllowQuickCustomization = false;
            this.bar1.OptionsBar.DisableClose = true;
            this.bar1.OptionsBar.DisableCustomization = true;
            this.bar1.OptionsBar.DrawDragBorder = false;
            this.bar1.OptionsBar.DrawSizeGrip = true;
            this.bar1.OptionsBar.UseWholeRow = true;
            this.bar1.Text = "Custom 1";
            // 
            // barStatusText
            // 
            this.barStatusText.AutoSize = DevExpress.XtraBars.BarStaticItemSize.Spring;
            this.barStatusText.Border = DevExpress.XtraEditors.Controls.BorderStyles.Office2003;
            this.barStatusText.Id = 1;
            this.barStatusText.Name = "barStatusText";
            this.barStatusText.PaintStyle = DevExpress.XtraBars.BarItemPaintStyle.CaptionGlyph;
            this.barStatusText.TextAlignment = System.Drawing.StringAlignment.Near;
            this.barStatusText.Width = 32;
            // 
            // bar2
            // 
            this.bar2.Appearance.BackColor = System.Drawing.Color.Gainsboro;
            this.bar2.Appearance.BackColor2 = System.Drawing.Color.Gainsboro;
            this.bar2.Appearance.Options.UseBackColor = true;
            this.bar2.BarName = "GeneratedMainMenu";
            this.bar2.DockCol = 0;
            this.bar2.DockRow = 0;
            this.bar2.DockStyle = DevExpress.XtraBars.BarDockStyle.Top;
            this.bar2.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(this.subFile),
            new DevExpress.XtraBars.LinkPersistInfo(this.subData),
            new DevExpress.XtraBars.LinkPersistInfo(this.subHelp)});
            this.bar2.OptionsBar.AllowQuickCustomization = false;
            this.bar2.OptionsBar.DisableClose = true;
            this.bar2.OptionsBar.DisableCustomization = true;
            this.bar2.OptionsBar.DrawDragBorder = false;
            this.bar2.OptionsBar.RotateWhenVertical = false;
            this.bar2.OptionsBar.UseWholeRow = true;
            this.bar2.Text = "Custom 2";
            // 
            // subFile
            // 
            this.subFile.Caption = "&File";
            this.subFile.CategoryGuid = new System.Guid("b4efef5a-8151-46d6-a441-53365cdaabe6");
            this.subFile.Id = 3;
            this.subFile.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(this.btnExit)});
            this.subFile.Name = "subFile";
            // 
            // btnExit
            // 
            this.btnExit.Caption = "E&xit";
            this.btnExit.CategoryGuid = new System.Guid("c740b439-694a-4de3-b3b2-cf438d297684");
            this.btnExit.Id = 4;
            this.btnExit.ItemShortcut = new DevExpress.XtraBars.BarShortcut((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4));
            this.btnExit.Name = "btnExit";
            this.btnExit.PaintStyle = DevExpress.XtraBars.BarItemPaintStyle.CaptionInMenu;
            this.btnExit.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.btnExit_ItemClick);
            // 
            // subData
            // 
            this.subData.Caption = "D&ata";
            this.subData.CategoryGuid = new System.Guid("b4efef5a-8151-46d6-a441-53365cdaabe6");
            this.subData.Id = 5;
            this.subData.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(this.btnRefreshQuotes),
            new DevExpress.XtraBars.LinkPersistInfo(this.btnRefreshVolatility),
            new DevExpress.XtraBars.LinkPersistInfo(this.btnCalculate, true)});
            this.subData.Name = "subData";
            // 
            // btnRefreshQuotes
            // 
            this.btnRefreshQuotes.Caption = "Refresh &Quotes";
            this.btnRefreshQuotes.CategoryGuid = new System.Guid("c740b439-694a-4de3-b3b2-cf438d297684");
            this.btnRefreshQuotes.Id = 6;
            this.btnRefreshQuotes.ImageIndex = 2;
            this.btnRefreshQuotes.ItemShortcut = new DevExpress.XtraBars.BarShortcut(System.Windows.Forms.Keys.F5);
            this.btnRefreshQuotes.Name = "btnRefreshQuotes";
            this.btnRefreshQuotes.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.btnRefreshQuotes_ItemClick);
            // 
            // btnRefreshVolatility
            // 
            this.btnRefreshVolatility.Caption = "Refresh &Volatility";
            this.btnRefreshVolatility.CategoryGuid = new System.Guid("c740b439-694a-4de3-b3b2-cf438d297684");
            this.btnRefreshVolatility.Id = 7;
            this.btnRefreshVolatility.ItemShortcut = new DevExpress.XtraBars.BarShortcut(System.Windows.Forms.Keys.F6);
            this.btnRefreshVolatility.Name = "btnRefreshVolatility";
            this.btnRefreshVolatility.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.btnRefreshVolatility_ItemClick);
            // 
            // btnCalculate
            // 
            this.btnCalculate.Caption = "&Calculate";
            this.btnCalculate.CategoryGuid = new System.Guid("c740b439-694a-4de3-b3b2-cf438d297684");
            this.btnCalculate.Id = 8;
            this.btnCalculate.ImageIndex = 1;
            this.btnCalculate.ItemShortcut = new DevExpress.XtraBars.BarShortcut(System.Windows.Forms.Keys.F8);
            this.btnCalculate.Name = "btnCalculate";
            this.btnCalculate.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.btnCalculate_ItemClick);
            // 
            // subHelp
            // 
            this.subHelp.Caption = "&Help";
            this.subHelp.CategoryGuid = new System.Guid("b4efef5a-8151-46d6-a441-53365cdaabe6");
            this.subHelp.Id = 9;
            this.subHelp.LinksPersistInfo.AddRange(new DevExpress.XtraBars.LinkPersistInfo[] {
            new DevExpress.XtraBars.LinkPersistInfo(DevExpress.XtraBars.BarLinkUserDefines.PaintStyle, this.btnContents, DevExpress.XtraBars.BarItemPaintStyle.Standard),
            new DevExpress.XtraBars.LinkPersistInfo(this.btnAbout, true)});
            this.subHelp.Name = "subHelp";
            // 
            // btnContents
            // 
            this.btnContents.Caption = "Contents";
            this.btnContents.CategoryGuid = new System.Guid("c740b439-694a-4de3-b3b2-cf438d297684");
            this.btnContents.Enabled = false;
            this.btnContents.Id = 10;
            this.btnContents.ImageIndex = 3;
            this.btnContents.ItemShortcut = new DevExpress.XtraBars.BarShortcut(System.Windows.Forms.Keys.F1);
            this.btnContents.Name = "btnContents";
            this.btnContents.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.btnContents_ItemClick);
            // 
            // btnAbout
            // 
            this.btnAbout.Caption = "&About";
            this.btnAbout.CategoryGuid = new System.Guid("c740b439-694a-4de3-b3b2-cf438d297684");
            this.btnAbout.Id = 11;
            this.btnAbout.ImageIndex = 0;
            this.btnAbout.Name = "btnAbout";
            this.btnAbout.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.btnAbout_ItemClick);
            // 
            // barAndDockingController1
            // 
            this.barAndDockingController1.PaintStyleName = "Office2003";
            this.barAndDockingController1.PropertiesBar.AllowLinkLighting = false;
            // 
            // imgMenu
            // 
            this.imgMenu.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imgMenu.ImageStream")));
            this.imgMenu.TransparentColor = System.Drawing.Color.Transparent;
            this.imgMenu.Images.SetKeyName(0, "about.ico");
            this.imgMenu.Images.SetKeyName(1, "calculator.ico");
            this.imgMenu.Images.SetKeyName(2, "refresh.ico");
            this.imgMenu.Images.SetKeyName(3, "help.ico");
            // 
            // barStaticItem1
            // 
            this.barStaticItem1.Caption = "Text";
            this.barStaticItem1.Id = 0;
            this.barStaticItem1.Name = "barStaticItem1";
            this.barStaticItem1.TextAlignment = System.Drawing.StringAlignment.Near;
            // 
            // repositoryItemTextEdit1
            // 
            this.repositoryItemTextEdit1.AutoHeight = false;
            this.repositoryItemTextEdit1.Name = "repositoryItemTextEdit1";
            // 
            // defaultLookAndFeel2
            // 
            this.defaultLookAndFeel2.LookAndFeel.SkinName = "Money Twins";
            this.defaultLookAndFeel2.LookAndFeel.Style = DevExpress.LookAndFeel.LookAndFeelStyle.Office2003;
            this.defaultLookAndFeel2.LookAndFeel.UseWindowsXPTheme = false;
            // 
            // UserActionControl
            // 
            this.UserActionControl.CommandType = System.Data.CommandType.StoredProcedure;
            this.UserActionControl.Connection = this.oleDbConnection;
            this.UserActionControl.Parameters.AddRange(new System.Data.OleDb.OleDbParameter[] {
            new System.Data.OleDb.OleDbParameter("@i_dtTime", System.Data.OleDb.OleDbType.Date),
            new System.Data.OleDb.OleDbParameter("@i_bLogOnOut", System.Data.OleDb.OleDbType.SmallInt),
            new System.Data.OleDb.OleDbParameter("@i_DBLogin", System.Data.OleDb.OleDbType.VarChar),
            new System.Data.OleDb.OleDbParameter("@i_ServerPath", System.Data.OleDb.OleDbType.VarChar),
            new System.Data.OleDb.OleDbParameter("@i_CompanyName", System.Data.OleDb.OleDbType.VarChar)});
            // 
            // OptionCalculatorForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.BackColor = System.Drawing.Color.Gainsboro;
            this.ClientSize = new System.Drawing.Size(572, 455);
            this.Controls.Add(this.cbFutureUseSpot);
            this.Controls.Add(this.barDockControl3);
            this.Controls.Add(this.barDockControl4);
            this.Controls.Add(this.barDockControl2);
            this.Controls.Add(this.barDockControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "OptionCalculatorForm";
            this.Text = "OTC Option Calculator";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OptionCalculatorForm_FormClosing);
            this.Load += new System.EventHandler(this.OptionCalculatorForm_Load);
            this.gbResults.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.groupControl2)).EndInit();
            this.groupControl2.ResumeLayout(false);
            this.groupControl2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.groupControl1)).EndInit();
            this.groupControl1.ResumeLayout(false);
            this.groupControl1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.gbDividends.ResumeLayout(false);
            this.gbPeriodicDivs.ResumeLayout(false);
            this.gbPeriodicDivs.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dtpDivDate.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cbDivFreq.Properties)).EndInit();
            this.gbCalcModel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.FeedTimer)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.InitTimer)).EndInit();
            this.cbFutureUseSpot.ResumeLayout(false);
            this.cbFutureUseSpot.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbStrikePrice.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbSpotPrice.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cbSymbolType.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.groupControl3)).EndInit();
            this.groupControl3.ResumeLayout(false);
            this.groupControl3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbDTE.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dtpExpirationDate.Properties)).EndInit();
            this.gbFuturePrice.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.cbUseSpotPrice.Properties)).EndInit();
            this.StyleGroupBox.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.AtmTimer)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.barManager2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.barAndDockingController1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.repositoryItemTextEdit1)).EndInit();
            this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new OptionCalculatorForm());
		}



		private void btnClose_Click(object sender, System.EventArgs e)
		{
			CloseApplication();
			Application.Exit();
		}
		private void CloseApplication()
		{
			FeedTimer.Enabled = false;
            FeedTimer.Stop();

			try
			{

				if (bGotFeed)
				{
					if(oPriceProvider!=null)
					{
						oPriceProvider.CancelLastQuote(null);
						oPriceProvider.Disconnect();
						oPriceProvider = null;
					}
					if(m_PriceProvider!=null)
					{
						m_PriceProvider.Disconnect();
						m_PriceProvider = null;								
					}
					bGotFeed = false;
				}

				if (bGotDB)
				{
					oleDbConnection.Close();
					bGotDB = false;
				}
	
			
			}
			catch
			{
			
			}

		}

	
		private int ClearResults(int iParam )
		{
			
			if (iParam != 1)
			{
				if (!bCanClear)
					return -1;
				bCanClear = false;
			}
			else
				bCanClear = true;

			if (iParam != 2)
			{
				tbResCPrice.Text = "0.00";
				tbResCAlpha.Text = "0.00";
				tbResCDelta.Text = "0.00";
				tbResCGamma.Text = "0.00";
				tbResCVega.Text = "0.00";
				tbResCTheta.Text = "0.00";
				tbResCRho.Text = "0.00";
				tbResCDeltaVega.Text = "0.00";
				tbResCDeltaTheta.Text = "0.00";
				tbResCGammaVega.Text = "0.00";
				tbResCGammaTheta.Text = "0.00";

				tbResPPrice.Text = "0.00";
				tbResPAlpha.Text = "0.00";
				tbResPDelta.Text = "0.00";
				tbResPGamma.Text = "0.00";
				tbResPVega.Text = "0.00";
				tbResPTheta.Text = "0.00";
				tbResPRho.Text = "0.00";
				tbResPDeltaVega.Text = "0.00";
				tbResPDeltaTheta.Text = "0.00";
				tbResPGammaVega.Text = "0.00";
				tbResPGammaTheta.Text = "0.00";
			
			}
			else
			{
				sAtmPut = "";
				sAtmCall = "";
			}

			tbResCImpliedVola.Text = "0.00";
			tbResPImpliedVola.Text = "0.00";
			//btnExport.Enabled = bCanClear;
			if (sAtmPut == null)
				sAtmPut ="";
			if(sAtmCall==null)
				sAtmCall = "";


			return 0;
		}

		private void btnCalc_Click(object sender, System.EventArgs e)
		{
			ClearResults(1);

			double	dStrikePrice;
			double	dSpotPrice;
			double	dCOptionPrice;
			double	dPOptionPrice;
			
			double	dVolatility;
			double	dDomesticRate;
			double	dForeignRate;
			double	dSkew;
			double	dKurtosis;
			long	lIsAmerican;
			long	lDivCount	= 1;
			long	lSteps		= 100;
			int	iRet = 0;
			int		lDivFrequency;
			double	dAmount;
			double  dCIV; 
			double  dPIV;
			string	sResStatus = "";
			bool	bCalculatingFuture = false;
			OCWrapper.DIVIDEND [] oDivs = new OCWrapper.DIVIDEND[0];
			bool bSpotGreeks = true;        

			if (cbSymbolType.SelectedIndex == 1)
				bCalculatingFuture = true;
			
			bSpotGreeks = cbUseSpotPrice.Checked;

			long	lModel		= 3;	// Optimized binomial
			if (rbStandart.Checked )
				lModel		= 2;		// standard binomial

			DateTime    dtNow, dtLastDivDate, dtCloseTime;

            double dDTE = GetDTE(tbDTE.Text);
            double dYTE = dDTE / 365.0;
            dtCloseTime = Convert.ToDateTime("1899/12/30 16:00:00.000");

            double dCl = dtCloseTime.ToOADate();
			
            try
			{
                dtNow = oOCWrapper.GetNewYorkTime();
			}
			catch
			{
				barStatusText.Caption = "Wrong date, nothing calculated";
				return;
			}

            try
            {
                dtLastDivDate = dtpDivDate.DateTime.Date;
            }
            catch
            {
                barStatusText.Caption = "Wrong Expiry Date, nothing calculated";
                return;
            }


			OCWrapper.GREEKS refResults;
			refResults = new OCWrapper.GREEKS();

			int iMask =	(int)(GREEK_TYPE.GT_THEOPRICE | 
				GREEK_TYPE.GT_DELTA | 
				GREEK_TYPE.GT_GAMMA | 
				GREEK_TYPE.GT_THETA | 
				GREEK_TYPE.GT_VEGA |
				GREEK_TYPE.GT_RHO | 
				GREEK_TYPE.GT_ALPHA | 
				GREEK_TYPE.GT_DELTA_VEGA | 
				GREEK_TYPE.GT_DELTA_THETA | 
				GREEK_TYPE.GT_GAMMA_VEGA | 
				GREEK_TYPE.GT_GAMMA_THETA);
			
			refResults.nMask = iMask;

			double[] dAmounts	= new double[0];
			double[] dYears		= new double[0];

			lDivCount = 0;
			

			try
			{
				dSpotPrice = (double)System.Convert.ToDouble(tbSpotPrice.EditValue);
			}
			catch
			{
				barStatusText.Caption = "Wrong spot price, nothing calculated";
				return;
			}			
			
			try
			{
				dStrikePrice = (double)System.Convert.ToDouble(tbStrikePrice.EditValue);
			}
			catch
			{
				barStatusText.Caption = "Wrong strike, nothing calculated";
				return;
			}			

			try
			{
				dCOptionPrice = (double)System.Convert.ToDouble(tbCOptionPrice.Text);
			}
			catch
			{
				//barStatusText.Caption = "Wrong call option price, nothing calculated";
				tbCOptionPrice.Text = "0.00";
				dCOptionPrice = 0;
			}			


			try
			{
				dPOptionPrice = (double)System.Convert.ToDouble(tbPOptionPrice.Text);
			}
			catch
			{
				//barStatusText.Caption = "Wrong put option price, nothing calculated";
				tbPOptionPrice.Text = "0.00";
				dPOptionPrice = 0;
			}			
			

			try
			{
				dDomesticRate = ((double)System.Convert.ToDouble(tbDomesticRate.Text))/100.0;
			}
			catch
			{
				barStatusText.Caption = "Wrong domestic rate, nothing calculated";
				return;
			}			

			try
			{
				dVolatility = ((double)System.Convert.ToDouble(tbVolatility.Text))/100.0;
			}
			catch
			{
				barStatusText.Caption = "Wrong volatility value, nothing calculated";
				return;
			}			

			


			dSkew = 0.0;
			dKurtosis = 0.0;
			lIsAmerican = System.Convert.ToInt32(rbAmerican.Checked);

			int iGotCount = 0;
		
			dForeignRate = 0;
			if (rbContinous.Checked)
			{
				// Continuous
				try
				{
					dForeignRate = ((double)System.Convert.ToDouble(tbContinousRate.Text))/100.0;
				}
				catch
				{
					barStatusText.Caption = "Wrong foreign rate, nothing calculated";
					return;
				}
			}
			else
			{
				if (rPeriodic.Checked )
				{
					// Periodic
					switch(cbDivFreq.SelectedIndex)       
					{         
						case 0:   
							lDivFrequency = 12;		// FREQUENCY_MONTHLY
							break;                  
						case 1:            
							lDivFrequency = 4;		// FREQUENCY_QUATERLY
							break;                  
						case 2:            
							lDivFrequency = 2;		// FREQUENCY_SEMIANNUALY
							break;                  
						case 3:            
							lDivFrequency = 1;		//	FREQUENCY_ANNUALY
							break;                  
						default:            
							lDivFrequency = 0;		// Unknown param...
							break;      
					}
					
					try
					{
						if (tbDivAmount.Text.Length==0)
							tbDivAmount.Text = "0.00";

						dAmount = (double)System.Convert.ToDouble(tbDivAmount.Text);
					}
					catch
					{
						barStatusText.Caption = "Wrong dividend amount, nothing calculated";
						return;
					}

                    DateTime dtExpiryOV = dtpExpirationDate.DateTime;

                    m_UndDivs.DivType = EtsGeneralLib.EtsDivTypeEnum.enDivCustomPeriodical;
                    m_UndDivs.DivFreqCust = lDivFrequency;
                    m_UndDivs.DivAmtCust = dAmount;
                    m_UndDivs.DivDateCust = dtLastDivDate;

                    int nRetCount = 0;
					int iCount = 0;
					System.Array arTmpAmounts = new double[0];
					System.Array arTmpDates = new double[0];

                    Array.Clear(dAmounts, 0, dAmounts.Length);
                    Array.Clear(dYears, 0, dYears.Length);

                    m_UndDivs.GetDividendCount2(dtNow, dtExpiryOV, dtCloseTime, out iCount);

                    if (iCount > 0)
                    {
                        dAmounts = new double[iCount];
                        dYears = new double[iCount];
                        m_UndDivs.GetDividends2(dtNow, dtExpiryOV, dtCloseTime, iCount, out arTmpAmounts, out arTmpDates, out iCount);
                        double dN = dtCloseTime.ToOADate();
                        arTmpAmounts.CopyTo(dAmounts, 0);
                        arTmpDates.CopyTo(dYears, 0);
                    }
                    
                    lDivCount = iCount;
				}
				else
				{
					// Stream

					/**************************************/
					// Custom divs 
					
					Array.Clear(dAmounts, 0, dAmounts.Length );
					Array.Clear(dYears, 0, dYears.Length);

                    DateTime dtExpiryOV = dtpExpirationDate.DateTime;

                    dtLastDivDate = dtNow;
                    dtLastDivDate = dtLastDivDate.AddDays(dDTE); //= System.Convert.ToDateTime(dDTE + dNow);

					int nRetCount = 0;
					int iCount = 0;
					System.Array arTmpAmounts = new double[0];
					System.Array arTmpDates = new double[0];

					m_CustDivs.Clear();
                    fDivForm.GetCustomDividendList(ref listTmpCust);
					if (listTmpCust.Count > 0 )
					{
						Record tmpRecord;
						DateTime dtTmp;
						for ( int iCounter = 0; iCounter<listTmpCust.Count; iCounter++)
						{
					
							tmpRecord = (Record)listTmpCust[iCounter];
							dtTmp = tmpRecord.DivDate;
							m_CustDivs.AddNonUnique(dtTmp, Convert.ToDouble(tmpRecord.DivAmount));
						}
					}
					
					m_CustDivs.GetDividendCount2(dtNow, dtExpiryOV, dtCloseTime, out iCount);
					if (iCount > 0)
					{
						dAmounts = new double[iCount];
						dYears = new double[iCount];
						m_CustDivs.GetDividends2(dtNow, dtExpiryOV, dtCloseTime, iCount, out arTmpAmounts, out arTmpDates, out nRetCount);
						
						arTmpAmounts.CopyTo(dAmounts, 0);
						arTmpDates.CopyTo(dYears, 0);
						
						iCount = nRetCount;
						
					}
					lDivCount = iCount;
				}
			}

			// Calculating Greeks


			dCIV = 0;
			if (dCOptionPrice > 0)
			{
				if (bCalculatingFuture)
				{
					try
					{
						iGotCount = 0;
						dCIV = oOCWrapper.OCWCalcFutureOptionVolatility( dDomesticRate, dSpotPrice, dCOptionPrice, dStrikePrice , dYTE, 1 /* Call */, (int)lIsAmerican,  (int)lSteps , dSkew,  dKurtosis , (int)lModel, out iGotCount);
					}
					catch
					{

						dCIV = 0;
					}
				}
				else
				{
					try
					{
						dCIV = oOCWrapper.OCWCalcVolatilityMM3( dDomesticRate, dForeignRate, m_dHTBRate, dSpotPrice, dCOptionPrice, dStrikePrice , dYTE, 1 /* Call */, (int)lIsAmerican, (int)lDivCount, ref dAmounts  ,  ref dYears , (int)lSteps , dSkew,  dKurtosis , (int)lModel);
					}
					catch
					{
						dCIV = 0;
					}
				}
			}
			if (dCIV > 0)
			{
				dCIV = dCIV * 100;
				tbResCImpliedVola.Text = dCIV.ToString("N", nfi);
			}
			else
			{
				tbResCImpliedVola.Text = "n/a";
			}

			if (bCalculatingFuture)
			{
				try
				{
					iRet = oOCWrapper.OCWCalcFutureOptionGreeks3(dDomesticRate, dForeignRate, dSpotPrice, bSpotGreeks,   dStrikePrice , dVolatility, dYTE, 1 /* Call */, (int)lIsAmerican, (int)lSteps , dSkew,  dKurtosis , (int)lModel, (int)lDivCount, ref dAmounts, ref dYears , refResults);
				}
				catch(Exception /*ex*/)
				{
					iRet = -2;
					sResStatus = "Call: OptionCalc library call error";
				}
			}
			else
			{
				try
				{
					iRet = oOCWrapper.OCWCalcGreeksMM(dDomesticRate, dForeignRate, m_dHTBRate, dSpotPrice, dStrikePrice , dVolatility, dYTE, 1 /* Call */, lIsAmerican, lDivCount, ref dAmounts  ,  ref dYears , lSteps , dSkew,  dKurtosis , lModel, refResults);			
				}
				catch
				{
					iRet = -2;
					sResStatus = "Call: OptionCalc library call error";
				}
			
			}

			if (iRet == 1) // Calc OK
			{
				if (IsCorrectDouble(refResults.dTheoPrice))
					tbResCPrice.Text =	refResults.dTheoPrice.ToString("N", nfi);
				else
					tbResCPrice.Text =	"n/a";

				if (IsCorrectDouble(refResults.dAlpha))
					tbResCAlpha.Text = 	refResults.dAlpha.ToString("N", nfi);
				else
					tbResCAlpha.Text =	"n/a";

				if (IsCorrectDouble(refResults.dDelta))
					tbResCDelta.Text = 	refResults.dDelta.ToString("N", nfi);
				else
					tbResCDelta.Text =	"n/a";

				if (IsCorrectDouble(refResults.dGamma))
					tbResCGamma.Text = 	refResults.dGamma.ToString("N", nfi);
				else
					tbResCGamma.Text =	"n/a";

				if (IsCorrectDouble(refResults.dVega))
					tbResCVega.Text = 	refResults.dVega.ToString("N", nfi);
				else
					tbResCVega.Text =	"n/a";

				if (IsCorrectDouble(refResults.dTheta))
					tbResCTheta.Text = 	refResults.dTheta.ToString("N", nfi);
				else
					tbResCTheta.Text =	"n/a";
				
				if (IsCorrectDouble(refResults.dRho))
					tbResCRho.Text = 	refResults.dRho.ToString("N", nfi);
				else
					tbResCRho.Text =	"n/a";

				if (IsCorrectDouble(refResults.dDeltaVega))
					tbResCDeltaVega.Text = 	refResults.dDeltaVega.ToString("N", nfi);
				else
					tbResCDeltaVega.Text =	"n/a";

				if (IsCorrectDouble(refResults.dDeltaTheta))
					tbResCDeltaTheta.Text = 	refResults.dDeltaTheta.ToString("N", nfi);
				else
					tbResCDeltaTheta.Text =	"n/a";

				if (IsCorrectDouble(refResults.dGammaVega))
					tbResCGammaVega.Text = 	refResults.dGammaVega.ToString("N", nfi);
				else
					tbResCGammaVega.Text =	"n/a";

				if (IsCorrectDouble(refResults.dGammaTheta))
					tbResCGammaTheta.Text = 	refResults.dGammaTheta.ToString("N", nfi);
				else
					tbResCGammaTheta.Text =	"n/a";

				sResStatus = "Call done OK";
			}
			
			if (iRet == -1)
			{
				sResStatus = "Call failed";
			}

			dPIV = 0;
			if (dPOptionPrice > 0)
			{

				if (bCalculatingFuture)
				{
					try
					{
						iGotCount = 0;
						dPIV = oOCWrapper.OCWCalcFutureOptionVolatility( dDomesticRate, dSpotPrice, dPOptionPrice, dStrikePrice , dYTE, 0 /* Put */, (int)lIsAmerican,  (int)lSteps , dSkew,  dKurtosis , (int)lModel, out iGotCount);
					}
					catch
					{
						dPIV = 0;
					}
				}
				else
				{
					try
					{
						dPIV = oOCWrapper.OCWCalcVolatilityMM3(dDomesticRate, dForeignRate, m_dHTBRate, dSpotPrice, dPOptionPrice, dStrikePrice , dYTE, 0 /* Put */, (int)lIsAmerican, (int)lDivCount, ref dAmounts  ,  ref dYears , (int)lSteps , dSkew,  dKurtosis , (int)lModel);
					}
					catch
					{
						dPIV = 0;
					}

				}

			}

			if (dPIV > 0)
			{
				dPIV = dPIV * 100;
				tbResPImpliedVola.Text = dPIV.ToString("N", nfi);
			}
			else
			{
				tbResPImpliedVola.Text = "n/a";
			}
			

			if (bCalculatingFuture)
			{
				try
				{
					iRet = oOCWrapper.OCWCalcFutureOptionGreeks3(dDomesticRate, dForeignRate, dSpotPrice, bSpotGreeks,   dStrikePrice , dVolatility, dYTE, 0 /* Put */, (int)lIsAmerican, (int)lSteps , dSkew,  dKurtosis , (int)lModel, (int)lDivCount, ref dAmounts, ref dYears , refResults);
				}
				catch
				{
					iRet = -2;
					sResStatus = sResStatus + "; Put: OptionCalc library call error";
				}
			}
			else
			{
				try
				{
					iRet = oOCWrapper.OCWCalcGreeksMM(dDomesticRate, dForeignRate, m_dHTBRate, dSpotPrice, dStrikePrice , dVolatility, dYTE, 0 /* Put */, (int)lIsAmerican, (int)lDivCount, ref dAmounts  ,  ref dYears , (int)lSteps , dSkew,  dKurtosis , (int)lModel, refResults);
				}
				catch
				{
					iRet = -2;
					sResStatus = sResStatus + "; Put: OptionCalc library call error";
				}
			}

			if (iRet == 1) // Calc OK
			{
				if (IsCorrectDouble(refResults.dTheoPrice))
					tbResPPrice.Text =	refResults.dTheoPrice.ToString("N", nfi);
				else
					tbResPPrice.Text =	"n/a";

				if (IsCorrectDouble(refResults.dAlpha))
					tbResPAlpha.Text = 	refResults.dAlpha.ToString("N", nfi);
				else
					tbResPAlpha.Text =	"n/a";

				if (IsCorrectDouble(refResults.dDelta))
					tbResPDelta.Text = 	refResults.dDelta.ToString("N", nfi);
				else
					tbResPDelta.Text =	"n/a";

				if (IsCorrectDouble(refResults.dGamma))
					tbResPGamma.Text = 	refResults.dGamma.ToString("N", nfi);
				else
					tbResPGamma.Text =	"n/a";

				if (IsCorrectDouble(refResults.dVega))
					tbResPVega.Text = 	refResults.dVega.ToString("N", nfi);
				else
					tbResPVega.Text =	"n/a";

				if (IsCorrectDouble(refResults.dTheta))
					tbResPTheta.Text = 	refResults.dTheta.ToString("N", nfi);
				else
					tbResPTheta.Text =	"n/a";
				
				if (IsCorrectDouble(refResults.dRho))
					tbResPRho.Text = 	refResults.dRho.ToString("N", nfi);
				else
					tbResPRho.Text =	"n/a";

				if (IsCorrectDouble(refResults.dDeltaVega))
					tbResPDeltaVega.Text = 	refResults.dDeltaVega.ToString("N", nfi);
				else
					tbResPDeltaVega.Text =	"n/a";

				if (IsCorrectDouble(refResults.dDeltaTheta))
					tbResPDeltaTheta.Text = 	refResults.dDeltaTheta.ToString("N", nfi);
				else
					tbResPDeltaTheta.Text =	"n/a";

				if (IsCorrectDouble(refResults.dGammaVega))
					tbResPGammaVega.Text = 	refResults.dGammaVega.ToString("N", nfi);
				else
					tbResPGammaVega.Text =	"n/a";

				if (IsCorrectDouble(refResults.dGammaTheta))
					tbResPGammaTheta.Text = 	refResults.dGammaTheta.ToString("N", nfi);
				else
					tbResPGammaTheta.Text =	"n/a";

				sResStatus = sResStatus + "; Put done OK";
				
			}
			
			if (iRet == -1)
			{
				sResStatus = sResStatus + "; Put failed";
			}
	
			barStatusText.Caption = sResStatus;
		}



		private void cbStyle_Leave(object sender, System.EventArgs e)
		{
			//			if (cbStyle.SelectedIndex == -1)
			//				cbStyle.SelectedIndex = 0;
		}
		
		private int TryToGetOption()
		{
			// It'll try to get option prices some day... I hope...
			/*
			DateTime dtExp = dtpExpirationDate.DateTime;
			int iMonth = dtExp.Month;
			...
			...
			*/
			
			return 0;
		}

		private void dtpExpirationDate_Leave(object sender, System.EventArgs e)
		{
			CheckDate();
		}

		private void tbDTE_Leave(object sender, System.EventArgs e)
		{
			bPreventCheckDate = true;
			double dDte;
			try
			{
                dDte = GetDTE(tbDTE.Text);
			}
			catch
			{
				barStatusText.Caption = "Wrong DTE";
				dDte = 0;
			}
			
			if (dDte < 0)
			{
				tbDTE.Text = "0";
				dDte = 0;
			}
			else
			{
				dtpExpirationDate.DateTime = oOCWrapper.GetNewYorkTime().AddDays(dDte);
			}
			
			bPreventCheckDate = false;
			TryToGetOption();

		}
		
		private int CheckDate()
		{
			if (bPreventCheckDate)
				return -1;

            DateTime dtNow = oOCWrapper.GetNewYorkTime();

			if (dtpExpirationDate.DateTime.ToOADate()  < dtNow.ToOADate()  )
			{
                dtpExpirationDate.DateTime = dtNow;
				tbDTE.Text = "0";
				tbDomesticRate.Text = "0.00";
				return 0;
			}

            if (dtpExpirationDate.DateTime.ToOADate() == dtNow.ToOADate())
			{
				tbDTE.Text = "0";
				tbDomesticRate.Text = "0.00";
				return 0;
			}
            
            double dDTE = dtpExpirationDate.DateTime.ToOADate() - dtNow.ToOADate();

            
            tbDTE.Text = GetDTE(dDTE);
			TryGetRates();

			return 0;
		}


        private String GetDTE(double dValue)
        {
            double d1Hour = 1.0 / 24.0;
            double d1Minute = 1.0 / (24.0 * 60.0);

            double dDays = Math.Floor(dValue);
            double dHour = Math.Floor((dValue - dDays) / d1Hour);
            double dMinute = Math.Floor((dValue - dDays - dHour * d1Hour) / d1Minute) + 1.0;

            String strRet;
            strRet = "0" + Convert.ToString((int)dDays) + " day(s) ";

            if ((int)dHour < 10) strRet = strRet + "0";
            strRet = strRet + Convert.ToString((int)dHour) + ":";

            if((int)dMinute < 10) strRet = strRet + "0";
            strRet = strRet + Convert.ToString((int)dMinute);

            return strRet;
        }

		private void dtpExpirationDate_ValueChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void rbContinous_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);

			gbDividends.Enabled = false;

			tbContinousRate.Enabled = true;
		
		}

		private void rbDiscreteDividends_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);

			gbDividends.Enabled = true;

			tbContinousRate.Enabled = false;
			gbPeriodicDivs.Enabled = true;


		}

		private void rPeriodic_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);

			//gbCustomDivs.Enabled = false;
			gbPeriodicDivs.Enabled = false;
			btnSetCustomDividends.Enabled = false;
			gbPeriodicDivs.Enabled = true;
		}

		private void rCustomDivs_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);

			//gbCustomDivs.Enabled = true;
			btnSetCustomDividends.Enabled = true;
			gbPeriodicDivs.Enabled = false;
		
		}

		private void tbDTE_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
		if(e.KeyChar == (char)13)
			{
				e.Handled=true;
				bPreventCheckDate = true;
				double dDte = 0;
                try
                {
                    dDte = GetDTE(tbDTE.Text);
                }
                catch
                {
                    barStatusText.Caption = "Wrong DTE";
                    dDte = 0;
                }
				if (dDte <= 0)
				{
					tbDTE.Text = "0";
					dDte = 0;
				}
				else
				{
					dtpExpirationDate.DateTime = oOCWrapper.GetNewYorkTime().AddDays(dDte);

					TryGetRates();
				}
                FeedRequestLastQuotes(tbSymbol.Text, true);
				bPreventCheckDate = false;
			}
		}

		private void btnSetCustomDividends_Click(object sender, System.EventArgs e)
		{
			if (m_iStockID >= 0)
			{
				//TryToGetCustomDivs(m_iStockID);
				CheckNewCustomDivs(m_iStockID);
			}
			if (fDivForm.ShowDialog() == DialogResult.OK)
			{
				ClearResults(0);
				btnCalc_Click(null, null);
				/*
				if (MessageBox.Show("Save custom dividends to Database?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
				{
					if (SaveCustomDivs() >= 0)
					{
						m_CustDivs.Clear();
						btnCalc_Click(null, null);
					}
					else
					{
						MessageBox.Show("Error saving custom dividends", "Error",  MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
					}
				}
				else
				{
					btnCalc_Click(null, null);
				}
			*/

			}		
		}


		private long SaveCustomDivs()
		{
			long lRet = 0;
			long lRowsAffected = 0;
			//DateTime dtDate;
			//double dCurDivAmount = 0;
			try
			{

				//OleDbDataReader oDBReader;

				ReadCustCommand = new System.Data.OleDb.OleDbCommand();
				ReadCustCommand.Connection = oleDbConnection;

				ReadCustCommand.CommandText = "delete from CustomDividend where StockId =" + Convert.ToString(m_iStockID);
				lRowsAffected = ReadCustCommand.ExecuteNonQuery();
				if (listTmpCust.Count > 0 )
				{
					Record tmpRecord;
					//DateTime dtTmp;
					for (int iCount = 0; iCount<listTmpCust.Count; iCount++)
					{
						tmpRecord = (Record)listTmpCust[iCount];
						ReadCustCommand.CommandText = "insert into CustomDividend (StockID, DivAmnt, DivYtes) Values ( " + Convert.ToString(m_iStockID) + " , " + tmpRecord.DivAmount  + " , " + tmpRecord.DivDate + " )";
						lRowsAffected = ReadCustCommand.ExecuteNonQuery();
						lRet += lRowsAffected;
					}
				}
				
			}
			catch
			{
				lRet = -1;
			}
			return lRet;

		}

		private int ParseCalcCommandLine()
		{
			double dBid;
			double dAsk;
			//long lContractType;
			long lDivType;
			long lCDStockID;
			long lCDCount;
			int lRet = -1;

			try
			{
				//MessageBox.Show(Environment.CommandLine);
				string[] CLParams = Environment.GetCommandLineArgs(); 
				if (CLParams.Length >=2)
				{
					if ((CLParams[1] == "1")||(CLParams[1] == "0"))
					{
						tbSymbol.Text = "";
					}
					else
						tbSymbol.Text = CLParams[1];

					if (tbSymbol.Text.Length > 0)
					{
						lRet = 1;
					}
					

					if (CLParams.Length >=3)
						sInOptionSymbol = CLParams[2];

					
					if (CLParams.Length >=4)
					{
						try
						{
							if(Convert.ToInt32(CLParams[3])!=1)
								rbAmerican.Checked = true;
							else
								rbEuropean.Checked = true;
                            bContractTypeSet = true;
						}
						catch
						{
							rbAmerican.Checked = true;
						}
						
					}

					if (CLParams.Length >=5)
					{
						tbStrikePrice.Text  = CLParams[4];
						lRet = 2;
						if(tbStrikePrice.Text.Length==0 || Convert.ToDouble(tbStrikePrice.Text)==0.0)
							m_bManual = true;
					}
					else
						m_bManual = true;

					if (CLParams.Length >=7)
					{
						dBid = Convert.ToDouble(CLParams[5]);
						dAsk = Convert.ToDouble(CLParams[6]);
					}


					if (CLParams.Length >=8)
					{
                        String strExpiry;
						try
						{
                            strExpiry = Convert.ToString(CLParams[7]);
                            strExpiry.Replace('"',' ');
                            dtpExpirationDate.DateTime = DateTime.Parse(strExpiry);
						}
						catch
						{
							dtpExpirationDate.DateTime = DateTime.Today;
						}
				
					}
					
					lContractType = 0;
                    if (CLParams.Length >= 9)
                        lContractType = Convert.ToInt32(CLParams[8]);
                    else
                        lContractType = 2;

					if (lContractType<=3)
						cbSymbolType.SelectedIndex = 0;
					else
						cbSymbolType.SelectedIndex = 1;

					if (lContractType > 5)
						tbSymbol.Text = sInOptionSymbol;

					
					if (CLParams.Length >= 10)
					{
						tbContinousRate.Text = CLParams[9];
						//tbDomesticRate.Text  = CLParams[8];
					
					}

				
					if (CLParams.Length >= 11)
						lDivType = Convert.ToInt32(CLParams[10]);

					if (CLParams.Length >= 12)
					{
						double dTmp = 0;
						try
						{
							dTmp = Convert.ToDouble(CLParams[11]);
							tbDivAmount.Text = 	dTmp.ToString("N", nfi);
						}
						catch
						{
							tbDivAmount.Text =  "0.00";
						}

					}

					if (CLParams.Length >= 13)
					{
						string sTmp = CLParams[12];
						if (sTmp.Length > 0 && sTmp!="0")
						{
							try
							{
								dtpDivDate.DateTime = DateTime.FromOADate(Convert.ToDouble(sTmp));
								
							}
							catch
							{
								dtpDivDate.DateTime = DateTime.Today;
							}
						}
					}					

					if (CLParams.Length >= 14)
					{
						long lFreqType = Convert.ToInt32(CLParams[13]);
						switch(lFreqType)
						{
							case 12:
								cbDivFreq.SelectedIndex = 0;
								break;
							case 4:
								cbDivFreq.SelectedIndex = 1;
								break;
							case 2:
								cbDivFreq.SelectedIndex = 2;
								break;
							case 1:
								cbDivFreq.SelectedIndex = 3;
								break;
							case 0:
								cbDivFreq.SelectedIndex = 0;
								break;
						}
					}

					if (CLParams.Length >= 15)
					{
						lCDStockID = Convert.ToInt32(CLParams[14]);
						if (lCDStockID > 0)
						{
							try
							{
								if ((Convert.ToInt32(CLParams[3]) == 1)&&(Convert.ToInt32(CLParams[9]) == 0))
								{
									TryGetYield((int)lCDStockID);
									TryGetFutureType((int)lCDStockID);

								}
							
							}
							catch
							{
								TryGetYield((int)lCDStockID);
								TryGetFutureType((int)lCDStockID);
							}
						
						}
					}

					if (CLParams.Length >= 16)
					{
						lCDCount = Convert.ToInt32(CLParams[15]);
					}

					if (CLParams.Length >= 17)
					{
						double dTmp = 0;
						try
						{
							dTmp = Convert.ToDouble(CLParams[16]);
							dTmp = dTmp * 100.0;
							tbDomesticRate.Text =	dTmp.ToString("N", nfi);
						}
						catch
						{
							tbDomesticRate.Text = "0.00";
						}
					}

					if (CLParams.Length >= 18)
					{
						double dTmp = 0;
						try
						{
							dTmp = Convert.ToDouble(CLParams[17]);
							if (dTmp > 0)
							{
								dTmp = dTmp * 100;
								tbVolatility.Text =	dTmp.ToString("N", nfi);
							}
							
						}
						catch
						{
							tbVolatility.Text = "0.00";
						}
					}


					if (CLParams.Length >= 19)
					{
						int iTmp = 0;
						try
						{
							iTmp = Convert.ToInt32(CLParams[18]);
							if (iTmp == 2) // standard
							{
								rbOptimized.Checked = false;
								rbStandart.Checked = true;
							}
							else
							{
								rbOptimized.Checked = true;
								rbStandart.Checked = false;							
							}							
						}
						catch
						{
						}
					}

                    if (CLParams.Length >= 20)
                    {
                        double dTmp = 0;
                        try
                        {
                            dTmp = Convert.ToDouble(CLParams[19]);
                            m_dHTBRate = dTmp;
                        }
                        catch
                        {
                            m_dHTBRate = (double)-1E+308;
                        }
                    }

				} //Got some params
			}
			catch
			{
				lRet = -1;
			}
			return 	lRet;
		}

		private void OptionCalculatorForm_Load(object sender, System.EventArgs e)
		{
			lSymbolType = 0;
			sOldSymbol = "ZZZZZZZZZZZZZZ";
			barStatusText.Caption = "Initializing...";
			bFirstElapse = true;
			DisableFormControls();
			InitTimer.Enabled = true;			
			
		}

		//		private void btnRequestMarket_Click(object sender, System.EventArgs e)
		//		{
		//			FeedRequestLastQuotes(tbSymbol.Text);
		//		}

		private void tbSymbol_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			
			if (e.KeyChar == (char)13)
			{
				m_bManual = true;
				e.Handled = true;
				CheckTheStock(true);
                FeedRequestLastQuotes(tbSymbol.Text, true);
				//TryLoadSymbolVolatility();
			}
			
		}


		private double GetShortRate(DateTime dtToday, DateTime dtDate)
		{
			double dRet = 0;
			
			if (oIRCurve.iPointsCount > 0)
			{
				//TimeSpan tsTmp = dtDate.Subtract(dtToday);
                double dYTE = (dtDate.ToOADate() - dtToday.ToOADate()) / 365.0;
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drShortRateValue, ref oIRCurve.irShortRateYTE, dYTE);
			}
			else
				dRet = 0;

			
			return dRet;
		}
		
		private double GetLongRate(DateTime dtToday, DateTime dtDate)
		{
			double dRet = 0;
			
			if (oIRCurve.iPointsCount > 0)
			{
				//TimeSpan tsTmp = dtDate.Subtract(dtToday);
                double dYTE = (dtDate.ToOADate() - dtToday.ToOADate()) / 365.0;
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drLongRateValue, ref oIRCurve.irLongRateYTE, dYTE);
			}
			else
				dRet = 0;

			
			return dRet;
		}

		private double GetNeutralRate(DateTime dtToday, DateTime dtDate)
		{
			double dRet = 0;
			
			if (oIRCurve.iPointsCount > 0)
			{
				//TimeSpan tsTmp = dtDate.Subtract(dtToday);
                double dYTE = (dtDate.ToOADate() - dtToday.ToOADate()) / 365.0;
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drNeutralRateValue, ref oIRCurve.irNeutralRateYTE, dYTE);
			}
			else
				dRet = 0;

			
			return dRet;
		}

		private double GetHTBRate(DateTime dtToday, DateTime dtDate)
		{
			double dRet = 0;
			
			if (oIRCurve.iPointsCount > 0)
			{
				//TimeSpan tsTmp = dtDate.Subtract(dtToday);
                double dYTE = (dtDate.ToOADate() - dtToday.ToOADate()) / 365.0;
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drHTBRateValue, ref oIRCurve.irHTBRateYTE, dYTE);
			}
			else
				dRet = 0;

			
			return dRet;
		}

		private double GetNeutralHTBRate(DateTime dtToday, DateTime dtDate)
		{
			double dRet = 0;
			
			if (oIRCurve.iPointsCount > 0)
			{
                double dYTE = (dtDate.ToOADate() - dtToday.ToOADate()) / 365.0;
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drNeutralHTBRateValue, ref oIRCurve.irNeutralHTBRateYTE, dYTE);
			}
			else
				dRet = 0;

			
			return dRet;
		}
		private int GetDTE(int iNum, int iPeriodType)
		{
			// lPeriodType: 1 - days, 2 - weeks, 3 - months, 4 - years
			int iRet = 0;
			DateTime dtTmp = DateTime.Today;
			TimeSpan tsDiff;

			switch(iPeriodType)
			{
				case 1:
					dtTmp = dtTmp.AddDays(iNum);
					break;
				case 2:
					dtTmp = dtTmp.AddDays(iNum*7);
					break;
				case 3:
					dtTmp = dtTmp.AddMonths(iNum);
					break;
				case 4:
					dtTmp = dtTmp.AddYears(iNum);
					break;

			}
			tsDiff = dtTmp.Subtract(DateTime.Today);
			iRet = (int)tsDiff.Days;

			return iRet;
		}

        private double GetYTE(int iNum, int iPeriodType)
        {
            double dRet = Convert.ToDouble(GetDTE(iNum, iPeriodType));
            return dRet / (double)365.0;
        }

		private int LoadInterestRates()
		{
			int iRet = 0;
			oIRCurve = new CIRCurve();
			ReadCurveCommand.CommandText= "usp_IRCurve_Get";
			ReadCurveCommand.CommandType = CommandType.StoredProcedure;
			OleDbDataReader ReadRateCurves = ReadCurveCommand.ExecuteReader();
			if (ReadRateCurves.Read())
			{
			
				oIRCurve.iID  = Convert.ToInt32(ReadRateCurves["iCurveID"].ToString());
				oIRCurve.sName = ReadRateCurves["vcCurveName"].ToString();
				oIRCurve.iRuleType = Convert.ToInt32(ReadRateCurves["iRuleTypeID"].ToString());
				oIRCurve.iCurveType  = Convert.ToInt32(ReadRateCurves["iCurveTypeID"].ToString());
				oIRCurve.dPosThreshold = Convert.ToDouble(ReadRateCurves["mPositionThreshold"].ToString());
				oIRCurve.iPointsCount = 0;

			}
			ReadRateCurves.Close();

			ReadCurveCommand.CommandText= "usp_IRPoint_Get";
			ReadCurveCommand.CommandType = CommandType.StoredProcedure;
			ReadCurveCommand.Parameters["@iCurveID"].Value = oIRCurve.iID;
			ReadRateCurves = ReadCurveCommand.ExecuteReader();
			int iCount = 0;
			while (ReadRateCurves.Read())
			{
				iCount++;
			}
			oIRCurve.iPointsCount = iCount;
			ReadRateCurves.Close();

			oIRCurve.irHTBRateYTE = new double[iCount];
			oIRCurve.irLongRateYTE = new double[iCount];
			oIRCurve.irShortRateYTE  = new double[iCount];
			oIRCurve.irNeutralHTBRateYTE  = new double[iCount];
			oIRCurve.irNeutralRateYTE =  new double[iCount];
				
			oIRCurve.drHTBRateValue = new double[iCount];
			oIRCurve.drLongRateValue  = new double[iCount];
			oIRCurve.drNeutralHTBRateValue = new double[iCount];
			oIRCurve.drNeutralRateValue  = new double[iCount];
			oIRCurve.drShortRateValue  = new double[iCount];


			ReadRateCurves = ReadCurveCommand.ExecuteReader();
			int iCur = 0;
            double dYTE = 0;
			int iPeriodTypeID = 0;
			int iNum = 0;
			double dShortRate = 0.0;
			double dNeutralRate = 0.0;
			double dLongRate = 0.0;
			double dHTBRate  = 0.0;

			while (ReadRateCurves.Read())
			{
				iNum = Convert.ToInt32(ReadRateCurves["iNum"].ToString());
				iPeriodTypeID = Convert.ToInt32(ReadRateCurves["iPeriodTypeID"].ToString());
				dYTE = GetYTE(iNum, iPeriodTypeID);

				oIRCurve.irHTBRateYTE[iCur] = dYTE;
				oIRCurve.irLongRateYTE[iCur] = dYTE;
				oIRCurve.irShortRateYTE[iCur] = dYTE;
				oIRCurve.irNeutralHTBRateYTE[iCur] = dYTE;
				oIRCurve.irNeutralRateYTE[iCur] = dYTE;
				
				dShortRate		= Convert.ToDouble(ReadRateCurves["fShortRate"].ToString());
				dLongRate		= Convert.ToDouble(ReadRateCurves["fLongRate"].ToString());
				dNeutralRate	= Convert.ToDouble(ReadRateCurves["neutralRate"].ToString());
				dHTBRate		= Convert.ToDouble(ReadRateCurves["fHTBRate"].ToString());
				
				oIRCurve.drLongRateValue[iCur] = dLongRate;
				oIRCurve.drShortRateValue[iCur] = dShortRate;
				oIRCurve.drHTBRateValue[iCur] = dHTBRate;

				oIRCurve.drNeutralRateValue[iCur] = dNeutralRate;
				oIRCurve.drNeutralHTBRateValue[iCur] = (dHTBRate + dLongRate) /2.0;

				
				iCur++;
			}
			
			ReadRateCurves.Close();
			/*
			 * 
				 If GetIrRuleType = enRateBasedOnPosition Then
					cPosThreshold = Abs(GetPosThreshold)
					bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
    
					If Not bUseMidRates Then _
						bForceRateUpdate = (Sgn(dPos) * Sgn(m_Aux.Grp.Und.UndPosForRates) < 0)
    
				Else
					bUseMidRates = True
				End If

			 * */

			
			return iRet;
		}
		private double TryGetRates()
		{
			double dRet = 0;

            DateTime dtNow = oOCWrapper.GetNewYorkTime();
			try
			{
				if (bUseMidRate)
				{
					if (bHTB)
						dRet = GetNeutralHTBRate(dtNow , dtpExpirationDate.DateTime );
					else
						dRet = GetNeutralRate(dtNow , dtpExpirationDate.DateTime );
						
				
				}
				else
				{
					if (bHTB)
						dRet = GetNeutralHTBRate(dtNow , dtpExpirationDate.DateTime );
					else
						dRet = GetNeutralRate(dtNow , dtpExpirationDate.DateTime );
						

				}
			}
			catch
			{
				dRet = -1;
			}

			if (dRet >= 0)
			{
				dRet = dRet * 100;
				tbDomesticRate.Text = dRet.ToString("N", nfi);
			}
			else
				tbDomesticRate.Text = "0.00";
			return dRet;
		
		}

		private int CheckTheStock(bool bManual)
		{
			if (sOldSymbol != tbSymbol.Text)
			{
				tbContinousRate.Text = "0.00";
				tbDivAmount.Text  = "0.00";
				dtpDivDate.DateTime = DateTime.Today;
				cbDivFreq.SelectedIndex = 0;

				tbStrikePrice.Text = "0.00";
				dtpExpirationDate.DateTime = DateTime.Today;
				tbDTE.Text = "0";
				tbVolatility.Text = "0.00";
				tbDomesticRate.Text  = "0.00";
				tbCOptionPrice.Text = "0.00";
				tbPOptionPrice.Text = "0.00";
				
				
				TryToGetDivs(tbSymbol.Text, bManual);
				TryGetRates();
                FeedRequestLastQuotes(tbSymbol.Text, true);
				sOldSymbol = tbSymbol.Text;
			}
			return 0;
		}
		private void tbSymbol_Leave(object sender, System.EventArgs e)
		{
			//CheckTheStock();
		}

        private void OptionCalculatorForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            CloseApplication();
        }

		private void tbSymbol_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
			if (fDivForm != null)
				fDivForm.SetFirstTime();
		}

		private void FeedTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			FeedTimer.Enabled = false;
            FeedTimer.Stop();
			if (cbSymbolType.SelectedIndex == 0)
				return;

			barStatusText.Caption = "No feed prices";
			oPriceProvider.CancelLastQuote(null);

// 			System.Text.StringBuilder s = new System.Text.StringBuilder();
// 			s.Append(tbSymbol.Text);
// 			if (cbSymbolType.SelectedIndex != 0)
// 				s.Append("G");

//            FeedRequestLastQuotes(tbSymbol.Text, false);
//            sOldSymbol = tbSymbol.Text;
		}

		private void cbSymbolType_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			CheckSelected();
		}

		private int CheckSelected()
		{
			if (cbSymbolType.SelectedIndex == 0)
				SetStockIndex();
			else
				SetFuture();

			return 0;
		}

		private int SetStockIndex()
		{
			rbContinous.Enabled = true;
			rbDiscreteDividends.Enabled  = true;
			lbSpotOrFuturePrice.Text = "Spot price:";
			if (rbContinous.Checked )
			{
				tbContinousRate.Enabled = true;
				gbDividends.Enabled = false;
			}
			else
			{
				gbDividends.Enabled = true;
			}

			cbUseSpotPrice.Enabled = false;
			gbFuturePrice.Enabled = false;

			
			return 0;
		}
	
		private int SetFuture()
		{
			rbContinous.Enabled = true;
			rbDiscreteDividends.Enabled  = true;
			
			rbContinous.Checked = true;
			rbDiscreteDividends.Checked = false;
			tbContinousRate.Enabled = true;

			lbSpotOrFuturePrice.Text = "Future price:";
			
			gbDividends.Enabled = false;
			cbUseSpotPrice.Enabled = true;
			gbFuturePrice.Enabled = true;
			lContractType = (long)EtsGeneralLib.EtsContractTypeEnum.enCtFuture;
			return 0;
		}


        private void TryToLoadHolidays()
        {
            if (m_UndDivs != null)
            {
                OleDbDataReader oDBIDReader;
                try
                {
                    int iActionID = 0;
                    DateTime dtHoliday;

                    m_UndDivs.Holidays.Clear();

                    ReadCommand.CommandText = "exec usp_Holiday_Get";
                    oDBIDReader = ReadCommand.ExecuteReader();
                    if (oDBIDReader.HasRows)
                    {
                        while (oDBIDReader.Read())
                        {
                            iActionID = Convert.ToInt32(oDBIDReader["tiActionID"].ToString());

                            if (iActionID < 3)
                            {
                                dtHoliday = Convert.ToDateTime(oDBIDReader["dtHolidayDate"].ToString());
                                m_UndDivs.Holidays.Add(dtHoliday);
                            }
                        }
                    }
                    oDBIDReader.Close();
                }
                catch
                {
                    
                }
            }
        }

		private void InitTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			InitTimer.Enabled = false;
			AtmTimer.Enabled = false;
			if (bFirstElapse)
			{
				bFirstElapse = false;
			}
			else
			{
				return;
			}
			
			m_CustDivs = new EtsGeneralLib.EtsDivCollClass();
            m_CustDivs.Holidays = new EtsGeneralLib.EtsHolidayAtomClass();

            m_UndDivs = new EtsGeneralLib.EtsIndexDivAtomClass();
            m_UndDivs.Holidays = m_CustDivs.Holidays;


			nfi = new CultureInfo( "en-US", false ).NumberFormat;
			nfi.NumberDecimalDigits = 4;
			nfi2 = new CultureInfo( "en-US", false ).NumberFormat;
			nfi2.NumberDecimalDigits = 3;
			m_bManual = false;
			oOCWrapper = new OCWrapper.OCWrapperClass();

            btnRefreshQuotes.Enabled = false;
            btnRefreshVolatility.Enabled = false;
			//cbDivFreq.SelectedIndex = 0;
			long lParams = ParseCalcCommandLine();
			if (GetInfoSources() > 0)
			{
                TryToLoadHolidays();

				if (tbSymbol.Text.Length > 0)
					TryToGetDivs(tbSymbol.Text, false); //?!
				LoadInterestRates();
				
				
				if (lParams > 0)
				{

					double dTmpRate = 0;
					if (tbSymbol.Text.Length > 0)
					{
                        FeedRequestLastQuotes(tbSymbol.Text, true);
						sOldSymbol = tbSymbol.Text;			
						TryToGetOption();
						
						try
						{
							dTmpRate = Convert.ToDouble(tbDomesticRate.Text);
							if ( dTmpRate <= 0)
							{
								dTmpRate = TryGetRates();
								if (dTmpRate>= 0)
									tbDomesticRate.Text = dTmpRate.ToString("N", nfi2);
							}
						}
						catch
						{
							tbDomesticRate.Text = "0.00";
						}

						if (lParams == 2)
						{
							if (TryLoadSymbolVolatility() > 0)
								barStatusText.Caption = "Got volatility for " + tbSymbol.Text + ", requesting quotes...";
						}
							
					}
				}

				if ( bGotDB && bGotFeed)
					btnRefreshQuotes.Enabled = true;

				if ( bGotDB && bGotVola )
					btnRefreshVolatility.Enabled = true;
			}
			
			listTmpCust = new ArrayList();
			listNewDBDivs  = new ArrayList();
			listDBDivs = new  ArrayList();
			fDivForm.GetCustomDividendList(ref listTmpCust);
			EnableFormControls();
            bContractTypeSet = false;
		}

		private int GetInfoSources()
		{
			this.Cursor = Cursors.AppStarting;
			bool bGotSettings = false;
			bGotFeed = false;
			bGotDB = false;
			bGotVola = false;
			string  sDescTmp;
			


			string sResults = "";
			bool bGroupRequestSupported;
			try
			{
				//ETSMANAGERLib.ISettings2 oSettings;
				ETSMANAGERLib.Settings oSettings = new ETSMANAGERLib.Settings();
				ETSMANAGERLib.ISettings2 oSettings2 = (ETSMANAGERLib.ISettings2)oSettings;
				oSettings2.InitForProfile("");
				
			
				sDBConnection = oSettings.DatabaseConnection;
				iVolaSource = oSettings.VolatilitySource;
				iProvider = oSettings.PriceProvider;
				if (iProvider > 0)
					iProviderType = iProvider;
				bGotSettings = true;
			}
			catch
			{
				bGotSettings = false;
				sResults = "No settings...";
			}
			

			Application.DoEvents();
			
			if (bGotSettings)
			{
				if (sDBConnection.Length > 0)
				{

					try
					{
						oleDbConnection.ConnectionString = sDBConnection;
						oleDbConnection.Open();
						bGotDB = true;
						sResults = " DB: " + oleDbConnection.Database;
					}
					catch
					{
						bGotDB = false;
						sResults = " DB: None ";
					}
				
				}
			
			}
			
			Application.DoEvents();

			if ( bGotSettings && bGotDB )
			{
				try
				{
					string sName, sFileName, sVersion, sProgID;
					VolatilitySourcesLib.VolatilitySources oVolaSources =  new VolatilitySourcesLib.VolatilitySources();
					VADBLAYOUTLib.DBLayout  oVMDB =  new VADBLAYOUTLib.DBLayout();
					oVolaSources.GetVolatilitySourceInfoByID( iVolaSource, out sName, out sFileName, out sVersion, out sProgID);
					m_VolaSource = oVolaSources.get_VolatilitySource(iVolaSource);
					oVMDB.ConnectionString = sDBConnection;
					m_VolaSource.DataSource  = (VADBLAYOUTLib.IVAManagement)oVMDB;
					m_VolaSource.EnableEvents = true;
					m_VolaSource.EnableEditing = true;
					sResults += "; Vola Source: " + sName;
					bGotVola = true;
				}
				catch
				{
					bGotVola = false;
					sResults += "; Vola Source: None";
				}
			}
			
			Application.DoEvents();

			fDivForm = new DivStreamForm();
			
			
			Application.DoEvents();
			try
			{
				PRICEPROVIDERSLib.IProvider oIProvider;
				PRICEPROVIDERSLib.IProviderData aProviderData;
				PRICEPROVIDERSLib.Providers oProviders;
				

				oProviders = new PRICEPROVIDERSLib.Providers();
				oProviders.Initialize();
				oProviders.GetProvider(iProviderType, out aProviderData);
				
				
				m_PriceProvider = new PRICEPROVIDERSLib.BatchPriceInfo();
				oIProvider = (PRICEPROVIDERSLib.IProvider)m_PriceProvider;
				oIProvider.Type = iProviderType; // HF
				m_PriceProvider.Connect();
				
				oProviders.GetProvider(iProviderType , out aProviderData);
				bGroupRequestSupported = aProviderData.IsGroupRequestSupported;
				//sResults += "; Feed: " + aProviderData.Description;
				sDescTmp = aProviderData.Description;


				oPriceProvider = new PRICEPROVIDERSLib.BatchPriceInfo();
				
				oPriceProvider.OnError += new PRICEPROVIDERSLib._IBatchPriceInfoEvents_OnErrorEventHandler(PriceProvider_OnError);
				oPriceProvider.OnLastQuote += new PRICEPROVIDERSLib._IBatchPriceInfoEvents_OnLastQuoteEventHandler(PriceProvider_OnLastQuote);
				oPriceProvider.OnQuoteUpdate += new PRICEPROVIDERSLib._IBatchPriceInfoEvents_OnQuoteUpdateEventHandler(PriceProvider_OnQuoteUpdate);
				oPriceProvider.OnSubscribed += new PRICEPROVIDERSLib._IBatchPriceInfoEvents_OnSubscribedEventHandler(PriceProvider_OnSubscribed);

				oIProvider = (PRICEPROVIDERSLib.IProvider)oPriceProvider;
				oIProvider.Type = iProviderType;

				oPriceProvider.Connect();
				if (bGroupRequestSupported)
				{
					m_GroupPriceProvider = (PRICEPROVIDERSLib.IGroupPriceWithNotify)oPriceProvider;
				}

				m_BatchPriceProvider = (PRICEPROVIDERSLib.IBatchPriceProvider)oPriceProvider;
				
				sResults += "; Feed: " + sDescTmp;
				bGotFeed = true;
			}
			catch
			{
				bGotFeed = false;
				sResults += "; Feed: None ";
			}


			barStatusText.Caption = sResults;
			this.Cursor = Cursors.Default;

			if (bGotDB&&bGotVola)
			{
				return 1;
			}

			return 0;
		}

		private void tbVolatility_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)13)
			{
				e.Handled = true;
				TryLoadSymbolVolatility();
			}
		}

		private void tbSpotPrice_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void tbStrikePrice_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void tbDTE_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

        private double GetDTE(String strValue)
        {
            double dDte = 0;
            double dHour = 1.0 / 24.0;
            double dMinute = 1.0 / (24.0 * 60.0); 
            try
            {
                if (strValue.Length > 0)
                {
                    String [] split = strValue.Split(new String [] {"day(s)", ":"}, 3, StringSplitOptions.RemoveEmptyEntries);
                    if (split.Length == 3)
                    {
                        try
                        {
                            dDte = Convert.ToDouble(split[0]); //days
                        }
                        catch { dDte = 0.0; }
                        
                        try
                        {
                            dDte = dDte + Convert.ToDouble(split[1]) * dHour; //hours
                        }
                        catch { dDte = dDte + 0.0; }
                        
                        try 
                        {
                            dDte = dDte + Convert.ToDouble(split[2]) * dMinute; //minute
                        }
                        catch { dDte = dDte + 0.0; }

                    }
                }
            }
            catch
            {
                dDte = 0;
            }
            return dDte;
        }

		private void tbVolatility_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void tbDomesticRate_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void tbContinousRate_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void dtpDivDate_ValueChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void tbDivAmount_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void cbDivFreq_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void rbStandart_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void rbOptimized_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void cbStyle_Click(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void cbSymbolType_Click(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void tbCOptionPrice_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(2);
		}

		private void tbPOptionPrice_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(2);
		}

		private void mnuExit_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void mnuQuotes_Click(object sender, System.EventArgs e)
		{
            FeedRequestLastQuotes(tbSymbol.Text, true);
		}

		private void mnuVolatility_Click(object sender, System.EventArgs e)
		{
			TryLoadSymbolVolatility();
		}

		private void mnuCalculate_Click(object sender, System.EventArgs e)
		{
			btnCalc_Click(null, null);
		}

		private void mnuAbout_Click(object sender, System.EventArgs e)
		{
			fAboutForm = new frmAbout();
			fAboutForm.ShowDialog();
		}

		private void mnuHelpContents_Click(object sender, System.EventArgs e)
		{
			// Show help		
			string sPath = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
			sPath = sPath + "\\Egar\\Emm.Fw\\Documents\\ETS Market Maker Guide.chm";
			Process.Start(sPath);

		}
	
		private void DisableFormControls()
		{
            subFile.Enabled = false;
            subData.Enabled = false;
			btnCalc.Enabled = false;
			btnClose.Enabled = false;
		}

		private long CheckNewCustomDivs(int iStockID)
		{
			long lRet = 0;
			m_CustDivs.Clear();
			//OleDbDataReader oDBCustReader;
			DateTime dtDate;
			double dCurDivAmount = 0;
			OleDbDataReader oDBReader;
			ArrayList lstTmp;
			lstTmp = new ArrayList();

			Record recNew;
			Record recLegacy;

			bool bChanged;
			try
			{

				listNewDBDivs.Clear();
				ReadCustCommand.CommandText = "select * from CustomDividend where StockID = " + Convert.ToString(iStockID);
				oDBReader = ReadCustCommand.ExecuteReader();
				while (oDBReader.Read())
				{
					dCurDivAmount	 = Convert.ToDouble(oDBReader["DivAmnt"].ToString());
					dtDate = DateTime.Parse(oDBReader["DivYtes"].ToString());

                    m_CustDivs.AddNonUnique(dtDate, dCurDivAmount);

					listNewDBDivs.Add(new Record(dtDate, dCurDivAmount));
					lstTmp.Add(new Record(dtDate, dCurDivAmount));
				}
				oDBReader.Close();
				bChanged = false;
				if (listNewDBDivs.Count != listDBDivs.Count)
				{
					bChanged = true;
				}
				else
				{
					for (int i = 0; i<listNewDBDivs.Count; i++)
					{
						recNew = (Record)listNewDBDivs[i];
						recLegacy = (Record)listDBDivs[i];
						if ((recNew.DivDate != recLegacy.DivDate ) || (recNew.DivAmount != recLegacy.DivAmount))
						{
							bChanged = true;
							break;
						}
					}
				}
				if (bChanged)
				{
					listDBDivs.Clear();
					listDBDivs = (ArrayList)listNewDBDivs.Clone();
					if (iSaveStockID!=iStockID || MessageBox.Show("Custom dividend data at the ETS DB is changed. Reload data?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
					{
						listTmpCust.Clear();
						listTmpCust = (ArrayList)lstTmp.Clone();
						iSaveStockID = iStockID;
					}
                    fDivForm.SetCustomDividendList(ref listTmpCust);
                }
			}
			catch
			{
				
				lRet = -1;
			}

			return lRet;
		
		}
		private long TryToGetCustomDivs(int iStockID)
		{
			long lRet = 0;
			listTmpCust = new ArrayList();
			listTmpCust.Clear();
			m_CustDivs.Clear();

			DateTime dtDate;
			double dCurDivAmount = 0;
			OleDbDataReader oDBReader;

			listDBDivs = new ArrayList();
			listNewDBDivs= new ArrayList();
			listDBDivs.Clear();

			try
			{

				ReadCustCommand.CommandText = "select * from CustomDividend where StockID = " + Convert.ToString(iStockID);
				oDBReader = ReadCustCommand.ExecuteReader();

                m_UndDivs.CustomDivs = (EtsGeneralLib.EtsDivColl)m_CustDivs;
				while (oDBReader.Read())
				{
					dCurDivAmount	 = Convert.ToDouble(oDBReader["DivAmnt"].ToString());
					dtDate = DateTime.Parse(oDBReader["DivYtes"].ToString());
					m_CustDivs.AddNonUnique(dtDate, dCurDivAmount);
					
					listTmpCust.Add(new Record(dtDate, dCurDivAmount));
					listDBDivs.Add(new Record(dtDate, dCurDivAmount));
					

				}

				oDBReader.Close();			
				
				fDivForm.SetCustomDividendList(ref listTmpCust);
			}
			catch
			{
				
				lRet = -1;
			}

			return lRet;
		}




		private void EnableFormControls()
		{
            subFile.Enabled = true;
			subData.Enabled = true;
			btnCalc.Enabled = true;
			btnClose.Enabled = true;
		}

		private void tbStrikePrice_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)13)
			{
				e.Handled = true;
				CheckTheStock(m_bManual);
                FeedRequestLastQuotes(tbSymbol.Text, true);
				TryLoadSymbolVolatility();
			}
		}

		private void rbAmerican_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);	
		}

		private void rbEuropean_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);	
		}

		private void rCustomDivs_CheckedChanged_1(object sender, System.EventArgs e)
		{
			ClearResults(0);
			btnSetCustomDividends.Enabled = true;
			gbPeriodicDivs.Enabled = false;
		}

		private void mnuFile_Click(object sender, System.EventArgs e)
		{
		
		}

		private void dtpExpirationDate_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			if(e.KeyChar == (char)13)
			{
				e.Handled=true;
                CheckDate();
                FeedRequestLastQuotes(tbSymbol.Text, true);
			}
		}

		private void AtmTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			AtmTimer.Enabled = false;
			btnCalc.Enabled = true;
			btnCalc_Click(null ,null);
		}

		private void tbSymbol_Validate(object sender, System.EventArgs e)
		{
			m_bManual = true;
			CheckTheStock(true);
            if (m_bFirstLoad)
            {
                String[] sCLParams = Environment.GetCommandLineArgs();
                if (sCLParams.Length > 1)
                {
                    if (sCLParams[1] == this.tbSymbol.Text/*.ToUpper()*/)
                    {
                        m_bManual = false;
                        return;
                    }
                }
                m_bFirstLoad = false;
            }
            FeedRequestLastQuotes(tbSymbol.Text, true);

		}

        private void sbStatus_PanelClick(object sender, StatusBarPanelClickEventArgs e)
        {

        }

        private void dtpExpirationDate_EditValueChanged(object sender, EventArgs e)
        {
            ClearResults(0);
            CheckDate();
        }

        private void btnClose_Click_1(object sender, EventArgs e)
        {
            CloseApplication();
            Application.Exit();
        }

        private void btnAbout_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            fAboutForm = new frmAbout();
            fAboutForm.ShowDialog();
        }

        private void btnContents_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            string sPath = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
            sPath = sPath + "\\Egar\\Emm.Fw\\Documents\\ETS Market Maker Guide.chm";
            Process.Start(sPath);
        }

        private void btnRefreshQuotes_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            FeedRequestLastQuotes(tbSymbol.Text, true);
        }

        private void btnRefreshVolatility_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            TryLoadSymbolVolatility();
        }

        private void btnCalculate_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            btnCalc_Click(null, null);
        }

        private void btnExit_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            Application.Exit();
        }

        private void btnExport_Click(object sender, EventArgs e)
        {

        }

        private void LogExiting()
        {
            //try
            //{
            //    System.Data.OleDb.OleDbConnection oleDbConnection1;
            //    System.Data.OleDb.OleDbCommand oleDbCommand1;

            //    oleDbCommand1 = UserActionControl.Clone();
            //    oleDbConnection1 = new System.Data.OleDb.OleDbConnection();
            //    oleDbConnection1.ConnectionString = sDBConnection;
            //    oleDbConnection1.Open();

            //    oleDbCommand1.Connection = oleDbConnection1;
            //    oleDbCommand1.CommandText = "usp_RegUserAction_Save";
            //    oleDbCommand1.Parameters["@i_dtTime"].Value = DateTime.Now;
            //    oleDbCommand1.Parameters["@i_bLogOnOut"].Value = 7;
            //    oleDbCommand1.Parameters["@i_DBLogin"].Value = Environment.UserName;
            //    oleDbCommand1.Parameters["@i_ServerPath"].Value = SystemInformation.ComputerName;
            //    oleDbCommand1.Parameters["@i_CompanyName"].Value = sDBUserGroup;
            //    oleDbCommand1.ExecuteReader();
                
            //    oleDbConnection1.Close();
            //}
            //catch
            //{

            //}
        }

        private void OptionCalculatorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            LogExiting();
        }
	}
}



public enum	GREEK_TYPE:int  
{
	GT_NOTHING	 = 0x00000000,

	GT_THEOPRICE	= 0x00000001,
	GT_ALPHA		= 0x00000002,
	GT_DELTA		= 0x00000004,
	GT_GAMMA		= 0x00000008,
	GT_VEGA			= 0x00000010,
	GT_THETA		= 0x00000020,
	GT_RHO			= 0x00000040,
    GT_VOLGA        = 0x00000080,

	GT_DELTA_VEGA	= 0x00000100,
	GT_DELTA_THETA	= 0x00000200,
	GT_GAMMA_VEGA	= 0x00000400,
	GT_GAMMA_THETA	= 0x00000800,

	GT_ALL			= -1

};

/*
	EtsContractTypeEnum
	{
	enCtNone = 0,
	enCtIndex = 1,
	enCtStock = 2,
	enCtOption = 3,
	enCtFuture = 4,
	enCtFutOption = 5,
	enCtFutRoot = 100
	};
*/

//CMCSA CCQAD 1 20 -1E+308 -1E+308 1/22/2005 2 0 0 0 0 0 0 0 0