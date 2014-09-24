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

	public class OptionCalculatorForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.TextBox tbDTE;
		private System.Windows.Forms.DateTimePicker dtpExpirationDate;
		private System.Windows.Forms.TextBox tbVolatility;
		private System.Windows.Forms.GroupBox gbResults;
		private System.Windows.Forms.Button btnClose;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox gbDividends;
		private System.Windows.Forms.RadioButton rCustomDivs;
		private System.Windows.Forms.ComboBox cbDivFreq;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox tbDivAmount;
		private System.Windows.Forms.RadioButton rPeriodic;
		private System.Windows.Forms.DateTimePicker dtpDivDate;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.Button btnCalc;
		private System.Windows.Forms.Button btnSetCustomDividends;
		private System.Windows.Forms.RadioButton rbDiscreteDividends;
		private System.Windows.Forms.TextBox tbContinousRate;
		private System.Windows.Forms.RadioButton rbContinous;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.TextBox tbSpotPrice;
		private System.Windows.Forms.TextBox tbStrikePrice;
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
		private System.Windows.Forms.ComboBox cbStyle;
		private System.Windows.Forms.Label label14;
		private System.Windows.Forms.Label lbContinousRate;
		private System.Windows.Forms.GroupBox gbPeriodicDivs;
		private System.Windows.Forms.GroupBox gbCustomDivs;
		private System.Windows.Forms.StatusBar sbStatus;
		private System.Windows.Forms.GroupBox gbCalcModel;
		private System.Windows.Forms.RadioButton rbStandart;
		private System.Windows.Forms.RadioButton rbOptimized;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private DivStreamForm fDivForm;
		private frmAbout	  fAboutForm;
		private NumberFormatInfo nfi;
		private NumberFormatInfo nfi2;
		private double dOldMidPrice = -1;
		
		private int iInitCount = 100;
	
		private int iProviderType = 1;
		PRICEPROVIDERSLib.PriceInfo m_PriceProvider;

		PRICEPROVIDERSLib.BatchPriceInfo oPriceProvider;
		PRICEPROVIDERSLib.IGroupPriceWithNotify m_GroupPriceProvider;
		PRICEPROVIDERSLib.IBatchPriceProvider m_BatchPriceProvider;
		
		VolatilitySourcesLib.VolatilitySource	m_VolaSource;

		private System.Windows.Forms.TextBox tbSymbol;
		private string sDBConnection;
		private int iVolaSource;
		private int iProvider;
		private bool bGotFeed;
		private bool bGotDB;
		private System.Data.OleDb.OleDbConnection oleDbConnection;
		private System.Data.OleDb.OleDbCommand oleDbCommand;
		private System.Data.OleDb.OleDbCommand ReadCommand;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.TextBox tbCOptionPrice;
		private System.Windows.Forms.GroupBox groupBox2;
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
		private System.Windows.Forms.ComboBox cbSymbolType;
		private System.Windows.Forms.Label lbSpotOrFuturePrice;
		private double dMinValue = -1.0E+30D;
		private System.Timers.Timer InitTimer;
		private bool   bPreventCheckDate = false;
		private bool  bCanClear = true;
		private bool bGotAtms = false;
		
		private int m_iStockID = -1;
		private bool m_bManual = false;

		private System.Data.OleDb.OleDbCommand ReadExpCommand;
		private System.Data.OleDb.OleDbCommand ReadOptsCommand;
		private bool bGotStockPrice = false;
		private bool bGotAtmPut = false;
		private bool bGotAtmCall = false;
		private System.Windows.Forms.MainMenu mainMenu;
		private System.Windows.Forms.MenuItem mnuFile;
		private System.Windows.Forms.MenuItem mnuExit;
		private System.Windows.Forms.MenuItem mnuData;
		private System.Windows.Forms.MenuItem mnuHelp;
		private System.Windows.Forms.MenuItem mnuQuotes;
		private System.Windows.Forms.MenuItem mnuVolatility;
		private System.Windows.Forms.MenuItem mnuCalculate;
		private System.Windows.Forms.MenuItem mnuAbout;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem mnuHelpContents;
		private System.Windows.Forms.GroupBox gbMain;
		private System.Windows.Forms.Button btnExport;
		private System.Data.OleDb.OleDbCommand ReadCurveCommand;
		
		private const int MIN_EXPIRY_DAYS = 20;
		private CIRCurve oIRCurve;
		private OCWrapper.OCWrapperClass oOCWrapper;
		private bool bUseMidRate = true;
		private System.Data.OleDb.OleDbCommand GetCustomDivs;
		private bool bHTB = false;
		private System.Data.OleDb.OleDbCommand ReadCustCommand;
		private EtsGeneralLib.IEtsDivColl m_CustDivs;
		private ArrayList listTmpCust;
		private bool bFirstElapse;

		
		private struct CIRCurve
		{
			public int iID;
			public string sName;
			public int iRuleType;
			public int iCurveType;
			public double dPosThreshold;
			public int iPointsCount;
			public int[] irShortRateDTE;
			public double[] drShortRateValue;
			public int[] irLongRateDTE;
			public double[] drLongRateValue;
			public int[] NeutralRateDTE;
			public double[] drNeutralRateValue;
			public int[] irHTBRateDTE;
			public double[] drHTBRateValue;
			public int[] irNeutralHTBRateDTE;
			public double[] drNeutralHTBRateValue;
			public int[] irNeutralRateDTE;
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
			if (ReqType == PRICEPROVIDERSLib.RequestsTypeEnum.enSubscribeQuote)
			{
				sbStatus.Text = "Failed to subscribe: " + Description;
			}

			if (ReqType == PRICEPROVIDERSLib.RequestsTypeEnum.enRequestLastQuote)
			{
				sbStatus.Text = "Failed to request last quotes: " + Description;
			}
    
			if ((ReqType != PRICEPROVIDERSLib.RequestsTypeEnum.enSubscribeQuote)&&(ReqType != PRICEPROVIDERSLib.RequestsTypeEnum.enRequestLastQuote))
			{
				if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enProviderConnected)
				{
					sbStatus.Text = "Provider connected";
				}
	                
				if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enProviderConnecting)
				{
					sbStatus.Text = "Provider is connecting";
				}

				if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enConnectionWasDisconnected)
				{
					sbStatus.Text = "Provider disconnected";
				}

				if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enCouldNotConnectToProvider)
				{
					sbStatus.Text = "Could not connect to provider: " + Description;
				}

				if (ErrorNumber == PRICEPROVIDERSLib.ErrorNumberEnum.enNotConnected)
				{
					sbStatus.Text = "Provider is not connected";
				}
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

	

		public void PriceProvider_OnLastQuote(ref PRICEPROVIDERSLib.QuoteUpdateParams Params, ref PRICEPROVIDERSLib.QuoteUpdateInfo Results)
		{
			
			FeedTimer.Enabled = false;
			double dPriceBid = Results.BidPrice;
			double dPriceAsk = Results.AskPrice;
			double dPriceLast = Results.LastPrice;
			double dMidPrice = 0;
	
			if ((!IsCorrectDouble(dPriceBid))&&(!IsCorrectDouble(dPriceAsk)))
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

				if ((dPriceAsk == 0)&&(dPriceBid == 0))
					dMidPrice = dPriceLast;
				else
					dMidPrice = (dPriceBid + dPriceAsk) / 2.0;
			}

				
			if (!IsCorrectDouble(dPriceLast))
				dPriceLast = 0;
		

			if ((Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enSTK )||
				(Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enIDX))
			{
				tbSpotPrice.Text = Convert.ToString(dMidPrice);
				bGotStockPrice = true;
				if (m_bManual)
				{
					if (dOldMidPrice != dMidPrice)
					{
						if (TryGetAtmStrike(dMidPrice, m_iStockID) >= 0)
							TryLoadSymbolVolatility();

						dOldMidPrice = dMidPrice;

					}
				}

			}
			
			if (((Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT)
				||(Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enFUT))
				||(Params.Type == PRICEPROVIDERSLib.InstrumentTypeEnum.enFOP))
			{
				
				//tbSpotPrice.Text = Convert.ToString(dPriceLast);
				if (Params.Symbol == sAtmCall)
				{
					tbCOptionPrice.Text = Convert.ToString(dMidPrice);	
					bGotAtmPut = true;

					if ((bGotStockPrice)&&(bGotAtmCall))
						btnCalc_Click(null ,null);

				}
				
				if (Params.Symbol == sAtmPut)
				{
					tbPOptionPrice.Text = Convert.ToString(dMidPrice);
					bGotAtmCall = true;

					if ((bGotStockPrice)&&(bGotAtmPut))
						btnCalc_Click(null ,null);
				}
			}

			string sSymbol = Params.Symbol;
			PRICEPROVIDERSLib.InstrumentTypeEnum itType = Params.Type;
			string sExchange = Params.Exchange;

			sbStatus.Text = "Last quotes for " + sSymbol + ": Last " + Convert.ToString(dPriceLast) + "; Bid: " + Convert.ToString(dPriceBid) + "; Ask: " +Convert.ToString(dPriceAsk);
			


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
			gbCustomDivs.Enabled = false;

			tbContinousRate.Enabled = true;
			lbContinousRate.Enabled = true;
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
			gbCustomDivs.Enabled = false;

			tbContinousRate.Enabled = false;
			lbContinousRate.Enabled = false;
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
			gbCustomDivs.Enabled = true;

			tbContinousRate.Enabled = false;
			lbContinousRate.Enabled = false;
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
					dStrikePrice = Convert.ToDouble(tbStrikePrice.Text);
				}
				catch
				{
					dStrikePrice = - 1;
					tbStrikePrice.Text = "0.00";
				}

			}
			
			if (!IsCorrectDouble(dStrikePrice))
			{
				return -1;
			}
			
			if (dStrikePrice < 0)
				return -1;

			int iExpID = 0;

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
			int iMinExpID = 0;


			while (ReadExp.Read())
			{
				iExpID = Convert.ToInt32(ReadExp["iExpiryID"].ToString());
				if ( iExpID != 0 )
				{
					dtTmp = DateTime.Parse(ReadExp["dtExpiryDate"].ToString());
					dDifference = dtTmp.ToOADate();
					if (dMinDiff > dDifference)
					{
						dMinDiff = dDifference;
						iMinExpID = iExpID;
						dtMinExp = dtTmp;
					}
					
				}
			}
			ReadExp.Close();

			ReadOptsCommand.CommandText= "usp_UnderlyingOptionByExpiry_Get";
			ReadOptsCommand.CommandType = CommandType.StoredProcedure;
			ReadOptsCommand.Parameters["@iUnderlyingID"].Value = iUndId;
			ReadOptsCommand.Parameters["@iExpiryID"].Value = iMinExpID;


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
			
			int iExpID = 0;

			DateTime dtMinExpiry = DateTime.Today;
			dtMinExpiry.AddDays(MIN_EXPIRY_DAYS);
			

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
			int iMinExpID = 0;


			while (ReadExp.Read())
			{
				iExpID = Convert.ToInt32(ReadExp["iExpiryID"].ToString());
				if ( iExpID != 0 )
				{
					dtTmp = DateTime.Parse(ReadExp["dtExpiryDate"].ToString());
					dDifference = dtTmp.ToOADate();
					if (dMinDiff > dDifference)
					{
						dMinDiff = dDifference;
						iMinExpID = iExpID;
						dtMinExp = dtTmp;
					}
					
				}
			}
			ReadExp.Close();

			ReadOptsCommand.CommandText= "usp_UnderlyingOptionByExpiry_Get";
			ReadOptsCommand.CommandType = CommandType.StoredProcedure;
			ReadOptsCommand.Parameters["@iUnderlyingID"].Value = iUndId;
			ReadOptsCommand.Parameters["@iExpiryID"].Value = iMinExpID;

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
				tbStrikePrice.Text = Convert.ToString(dNearestCallStrike);
			}
			else
			{
				dRet = dNearestPutStrike;
				tbStrikePrice.Text = Convert.ToString(dNearestPutStrike);
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
				dStrikePrice = (double)System.Convert.ToDouble(tbStrikePrice.Text);
			}
			catch
			{
				sbStatus.Text = "Wrong strike, can't manage ATMs";
				return -1;
			}		
			bGotAtms = false;
			
				
			int iExpID = 0;

			DateTime dtMinExpiry = DateTime.Today;
			dtMinExpiry.AddDays(MIN_EXPIRY_DAYS);
			

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
			int iMinExpID = 0;


			while (ReadExp.Read())
			{
				iExpID = Convert.ToInt32(ReadExp["iExpiryID"].ToString());
				if ( iExpID != 0 )
				{
					dtTmp = DateTime.Parse(ReadExp["dtExpiryDate"].ToString());
					dDifference = dtTmp.ToOADate();
					if (dMinDiff > dDifference)
					{
						dMinDiff = dDifference;
						iMinExpID = iExpID;
						dtMinExp = dtTmp;
					}
					
				}
			}
			if (m_bManual)
			{
				dtpExpirationDate.Value = dtMinExp;
			}
	
			ReadExp.Close();

			ReadOptsCommand.CommandText= "usp_UnderlyingOptionByExpiry_Get";
			ReadOptsCommand.CommandType = CommandType.StoredProcedure;
			ReadOptsCommand.Parameters["@iUnderlyingID"].Value = iUndId;
			ReadOptsCommand.Parameters["@iExpiryID"].Value = iMinExpID;

			double dNearestCallStrike = 0;
			double dNearestPutStrike = 0;

			double dMinCallDiff = 1e+30;
			double dMinPutDiff = 1e+30;

			string sAtmTmpPut = "";
			string sAtmTmpCall = "";

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
				ReadCommand.CommandText = "select top 1 ContractTypeID from [Contract] where contractID = " + Convert.ToString(iID);
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
				ReadCommand.CommandText = "select * from [index] where [index].indexid = " + Convert.ToString(iID);
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

						sTmp = oDBReader["CalcOptionType"].ToString();
						if (sTmp.Length > 0)
						{
							iTmpType = Convert.ToInt32(sTmp);
							if (iTmpType == 0)
								cbStyle.SelectedIndex = 1;
							else
								cbStyle.SelectedIndex = 0;	// American at list
						}
						else
						{
							cbStyle.SelectedIndex = 0;	// American at list
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
							cbStyle.SelectedIndex = 1;
						else
							cbStyle.SelectedIndex = 0;	// American at list
						break;
					}
					else
					{
						cbStyle.SelectedIndex = 0;	// American at list
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
				
				ReadCommand.CommandText = "select Count(contractid) As GotIDs from contract where symbol = '" + sSymbol+ "'";
				oDBIDReader = ReadCommand.ExecuteReader();
				while (oDBIDReader.Read())
				{
					iIDCount = Convert.ToInt32(oDBIDReader["GotIDs"].ToString());

				}
				oDBIDReader.Close();

				if (iIDCount <= 0)
				{
					ReadCommand.CommandText = "select Count(contractid) As GotIDs from contract where symbol like '" + sSymbol+ "_'";
					oDBIDReader = ReadCommand.ExecuteReader();
					while (oDBIDReader.Read())
					{
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
					ReadCommand.CommandText = "select contractid from contract where symbol like '" + sSymbol+ "_'";
				else
					ReadCommand.CommandText = "select contractid from contract where symbol = '" + sSymbol+ "'";
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
						ReadCommand.CommandText = "select top 1 isHTB, divFreq, divAmt, divDate, isDivCustom, divAmtCustom, divDateCustom, divFreqCustom, CalcOptionType from  Stock where StockId = " + Convert.ToString(iStockID);
						oDBReader = ReadCommand.ExecuteReader();
						while (oDBReader.Read())
						{
							string sTmp;
							int iTmp;
							int iTmpType;
							
							int iHTB = 0;
							DateTime dTmpDate;
							
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
					
							
							sTmp = oDBReader["CalcOptionType"].ToString();
							if (sTmp.Length > 0)
							{
								iTmpType = Convert.ToInt32(sTmp);
								if (iTmpType == 0)
									cbStyle.SelectedIndex = 1;
								else
									cbStyle.SelectedIndex = 0;	// American at list
							}
							else
							{
								cbStyle.SelectedIndex = 0;	// American at list
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
								}
									

								sTmp = oDBReader["divDate"].ToString();
								if (sTmp.Length > 0)
								{
									dTmpDate = DateTime.Parse(sTmp);
									dtpDivDate.Text = dTmpDate.ToShortDateString();
								}
								else
									dtpDivDate.Value = DateTime.Today;

								bGotFreq = false;
						
								try
								{
									iTmp = Convert.ToInt32(oDBReader["divFreq"].ToString());
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
							
							if ( iDivCustom == 1 ) // Custom Periodical
							{
								SetPeriodic();
								sTmp = oDBReader["divAmtCustom"].ToString();
								if (sTmp.Length <= 0)
									tbDivAmount.Text = "0.00";
								else
								{
									tbDivAmount.Text = sTmp;
									bGotDivs = true;
								}
									

								sTmp = oDBReader["divDateCustom"].ToString();
								if (sTmp.Length > 0)
								{
									dTmpDate = DateTime.Parse(sTmp);
									dtpDivDate.Text = dTmpDate.ToShortDateString();
								}
								else
									dtpDivDate.Value = DateTime.Today;

								bGotFreq = false;
						
								try
								{
									iTmp = Convert.ToInt32(oDBReader["divFreqCustom"].ToString());
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
			catch
			{
				//iStockID = -1;
			}

			/*
			if (!bGotDivs)
			{
				tbDivAmount.Text = "0.00";
				cbDivFreq.SelectedIndex = 0;
				dtpDivDate.Value = DateTime.Today;
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

		private double GetMMVola(string sSymbol, DateTime dtExpDate, double dStrike)
		{
			double dRes = -1;
			try
			{
				double dNLessStrike = 0.0;
				double dNGreaterStrike = 0.0;
				double dIntVola = 0.0;
				double dLessVola = 0.0;
				double dGreaterVola = 0.0;

				GetNearestBaseStrikes(dStrike, m_iStockID, dtpExpirationDate.Value, ref dNLessStrike, ref dNGreaterStrike);
				if ((dNLessStrike== dStrike)||(dNGreaterStrike==dStrike))
				{
					dIntVola =GetStrikeVola(sSymbol, dtExpDate, dStrike);
				}
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
				}

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
				oVolaControl.Init(sSymbol, EtsGeneralLib.EtsContractTypeEnum.enCtStock, m_VolaSource);
				if (oVolaControl.IsInitialized )
				{
					dRes = oVolaControl.get_OptionVola(dtExpDate, dStrike);
						
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
		

		private double TryLoadSymbolVolatility()
		{
		
			if (!bGotVola)
				return -1;
			double dRes;
			double dStrikePrice;
			string sSymbol = tbSymbol.Text;
			DateTime dtExpDate = dtpExpirationDate.Value;

			if (sSymbol.Length ==0 )
				return -1;

			try
			{
				dStrikePrice = (double)System.Convert.ToDouble(tbStrikePrice.Text);
			}
			catch
			{
				sbStatus.Text = "Wrong strike, Volatility value is not loaded";
				return -1;
			}			


			try
			{
				
				//dRes = GetCustomLinearInterpolatedVola(sSymbol, dtExpDate, dStrikePrice);
				//if (dRes < 0)
				//{
					dRes = GetMMVola(sSymbol, dtExpDate, dStrikePrice);
					dRes = dRes * 100; // to %
				//}
				
				if (!IsCorrectDouble(dRes)||dRes<0 )
				{
					tbVolatility.Text = "0.00";
					sbStatus.Text = "Can't get volatility";
					dRes =  -1;
				}
				else
				{
					
					
					tbVolatility.Text = dRes.ToString("N", nfi);
					sbStatus.Text = "Got volatility for " + tbSymbol.Text;

				}


			}
			catch
			{
				sbStatus.Text = "Can't get volatility";
				dRes =  -1;
			}
			return dRes;
		}


		private int FeedRequestLastQuotes(string sSymbol)
		{
			if ((sSymbol.Length <= 0)||(sSymbol.Length > 15))
				return -1;

			bGotStockPrice = false;
			bGotAtmPut = false;
			bGotAtmCall = false;

			sSymbol = sSymbol.ToUpper();
			tbSymbol.Text = sSymbol;
			
			tbSpotPrice.Text = "0.00";
			tbCOptionPrice.Text = "0.00";
			tbPOptionPrice.Text = "0.00";
			

			if (!bGotFeed)
			{
				sbStatus.Text = "Can't request last quotes: no feed";
				return -1;
			}
			sbStatus.Text = "Requested last quotes for " + tbSymbol.Text;
			PRICEPROVIDERSLib.QuoteUpdateParams aParam = new PRICEPROVIDERSLib.QuoteUpdateParams();
			PRICEPROVIDERSLib.QuoteUpdateParams aArgParam = new PRICEPROVIDERSLib.QuoteUpdateParams();
			
			// Stock prices only for now...

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			if ((cbSymbolType.SelectedIndex == 1)&&(sSymbol.IndexOf("/") <= 0))
			{
				sSymbol = "/" + sSymbol;
			}

			aArgParam.Symbol = sSymbol;
			//aArgParam.Exchange = "G";
			aArgParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enSTK;
			oPriceProvider.RequestLastQuote(ref aArgParam);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);
			
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

			FeedRequestAtms(sAtmPut, sAtmCall);

			FeedTimer.Enabled = true;
			return 0;
		}

		
		private int FeedRequestAtms(string sAtmPut, string sAtmCall)
		{
			if ((sAtmCall.Length <= 0)||(sAtmPut.Length > 8))
				return -1;

			tbCOptionPrice.Text = "0.00";
			tbPOptionPrice.Text = "0.00";
			
			if (!bGotFeed)
			{
				sbStatus.Text = "Can't request last quotes: no feed";
				return -1;
			}
			sbStatus.Text = "Requested last quotes for " + sAtmPut + " and " + sAtmCall;
			PRICEPROVIDERSLib.QuoteUpdateParams aParam = new PRICEPROVIDERSLib.QuoteUpdateParams();
			PRICEPROVIDERSLib.QuoteUpdateParams aParamCall = new PRICEPROVIDERSLib.QuoteUpdateParams();
			PRICEPROVIDERSLib.QuoteUpdateParams aParamPut = new PRICEPROVIDERSLib.QuoteUpdateParams();
	

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParamPut.Symbol = sAtmPut;
			//aParamPut.Exchange = "G";
			aParamPut.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT;
			oPriceProvider.RequestLastQuote(ref aParamPut);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);
			

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStart;
			oPriceProvider.RequestLastQuote(ref aParam);

			aParamCall.Symbol = sAtmCall;
			//aParamCall.Exchange = "G";
			aParamCall.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enOPT;
			oPriceProvider.RequestLastQuote(ref aParamCall);

			aParam.Type = PRICEPROVIDERSLib.InstrumentTypeEnum.enMStop;
			oPriceProvider.RequestLastQuote(ref aParam);
			
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
			
			//
			// TODO: Add any constructor code after InitializeComponent call
			//

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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(OptionCalculatorForm));
			this.lbSpotOrFuturePrice = new System.Windows.Forms.Label();
			this.tbSpotPrice = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.tbStrikePrice = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.tbDTE = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.dtpExpirationDate = new System.Windows.Forms.DateTimePicker();
			this.label8 = new System.Windows.Forms.Label();
			this.tbVolatility = new System.Windows.Forms.TextBox();
			this.label9 = new System.Windows.Forms.Label();
			this.tbDomesticRate = new System.Windows.Forms.TextBox();
			this.btnCalc = new System.Windows.Forms.Button();
			this.gbResults = new System.Windows.Forms.GroupBox();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.tbResCImpliedVola = new System.Windows.Forms.TextBox();
			this.tbResCGammaTheta = new System.Windows.Forms.TextBox();
			this.tbResCGammaVega = new System.Windows.Forms.TextBox();
			this.tbResCDeltaTheta = new System.Windows.Forms.TextBox();
			this.tbResCDeltaVega = new System.Windows.Forms.TextBox();
			this.tbResCAlpha = new System.Windows.Forms.TextBox();
			this.tbResCRho = new System.Windows.Forms.TextBox();
			this.tbResCVega = new System.Windows.Forms.TextBox();
			this.tbResCTheta = new System.Windows.Forms.TextBox();
			this.tbResCGamma = new System.Windows.Forms.TextBox();
			this.tbResCDelta = new System.Windows.Forms.TextBox();
			this.tbResCPrice = new System.Windows.Forms.TextBox();
			this.groupBox6 = new System.Windows.Forms.GroupBox();
			this.tbResPImpliedVola = new System.Windows.Forms.TextBox();
			this.tbResPGammaTheta = new System.Windows.Forms.TextBox();
			this.tbResPGammaVega = new System.Windows.Forms.TextBox();
			this.tbResPDeltaTheta = new System.Windows.Forms.TextBox();
			this.tbResPDeltaVega = new System.Windows.Forms.TextBox();
			this.tbResPAlpha = new System.Windows.Forms.TextBox();
			this.tbResPRho = new System.Windows.Forms.TextBox();
			this.tbResPVega = new System.Windows.Forms.TextBox();
			this.tbResPTheta = new System.Windows.Forms.TextBox();
			this.tbResPGamma = new System.Windows.Forms.TextBox();
			this.tbResPDelta = new System.Windows.Forms.TextBox();
			this.tbResPPrice = new System.Windows.Forms.TextBox();
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
			this.btnClose = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.lbContinousRate = new System.Windows.Forms.Label();
			this.tbContinousRate = new System.Windows.Forms.TextBox();
			this.rbContinous = new System.Windows.Forms.RadioButton();
			this.rbDiscreteDividends = new System.Windows.Forms.RadioButton();
			this.gbDividends = new System.Windows.Forms.GroupBox();
			this.rCustomDivs = new System.Windows.Forms.RadioButton();
			this.gbPeriodicDivs = new System.Windows.Forms.GroupBox();
			this.label12 = new System.Windows.Forms.Label();
			this.dtpDivDate = new System.Windows.Forms.DateTimePicker();
			this.cbDivFreq = new System.Windows.Forms.ComboBox();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.tbDivAmount = new System.Windows.Forms.TextBox();
			this.gbCustomDivs = new System.Windows.Forms.GroupBox();
			this.btnSetCustomDividends = new System.Windows.Forms.Button();
			this.rPeriodic = new System.Windows.Forms.RadioButton();
			this.btnExport = new System.Windows.Forms.Button();
			this.label13 = new System.Windows.Forms.Label();
			this.tbSymbol = new System.Windows.Forms.TextBox();
			this.cbStyle = new System.Windows.Forms.ComboBox();
			this.label14 = new System.Windows.Forms.Label();
			this.sbStatus = new System.Windows.Forms.StatusBar();
			this.gbCalcModel = new System.Windows.Forms.GroupBox();
			this.rbStandart = new System.Windows.Forms.RadioButton();
			this.rbOptimized = new System.Windows.Forms.RadioButton();
			this.tbCOptionPrice = new System.Windows.Forms.TextBox();
			this.oleDbConnection = new System.Data.OleDb.OleDbConnection();
			this.oleDbCommand = new System.Data.OleDb.OleDbCommand();
			this.ReadCommand = new System.Data.OleDb.OleDbCommand();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.label16 = new System.Windows.Forms.Label();
			this.tbPOptionPrice = new System.Windows.Forms.TextBox();
			this.label15 = new System.Windows.Forms.Label();
			this.FeedTimer = new System.Timers.Timer();
			this.label10 = new System.Windows.Forms.Label();
			this.cbSymbolType = new System.Windows.Forms.ComboBox();
			this.InitTimer = new System.Timers.Timer();
			this.ReadExpCommand = new System.Data.OleDb.OleDbCommand();
			this.ReadOptsCommand = new System.Data.OleDb.OleDbCommand();
			this.mainMenu = new System.Windows.Forms.MainMenu();
			this.mnuFile = new System.Windows.Forms.MenuItem();
			this.mnuExit = new System.Windows.Forms.MenuItem();
			this.mnuData = new System.Windows.Forms.MenuItem();
			this.mnuQuotes = new System.Windows.Forms.MenuItem();
			this.mnuVolatility = new System.Windows.Forms.MenuItem();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.mnuCalculate = new System.Windows.Forms.MenuItem();
			this.mnuHelp = new System.Windows.Forms.MenuItem();
			this.mnuHelpContents = new System.Windows.Forms.MenuItem();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.mnuAbout = new System.Windows.Forms.MenuItem();
			this.gbMain = new System.Windows.Forms.GroupBox();
			this.ReadCurveCommand = new System.Data.OleDb.OleDbCommand();
			this.GetCustomDivs = new System.Data.OleDb.OleDbCommand();
			this.ReadCustCommand = new System.Data.OleDb.OleDbCommand();
			this.gbResults.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.groupBox6.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.gbDividends.SuspendLayout();
			this.gbPeriodicDivs.SuspendLayout();
			this.gbCustomDivs.SuspendLayout();
			this.gbCalcModel.SuspendLayout();
			this.groupBox2.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.FeedTimer)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.InitTimer)).BeginInit();
			this.SuspendLayout();
			// 
			// lbSpotOrFuturePrice
			// 
			this.lbSpotOrFuturePrice.Location = new System.Drawing.Point(15, 55);
			this.lbSpotOrFuturePrice.Name = "lbSpotOrFuturePrice";
			this.lbSpotOrFuturePrice.Size = new System.Drawing.Size(112, 16);
			this.lbSpotOrFuturePrice.TabIndex = 7;
			this.lbSpotOrFuturePrice.Text = "Spot Price:";
			// 
			// tbSpotPrice
			// 
			this.tbSpotPrice.BackColor = System.Drawing.SystemColors.Window;
			this.tbSpotPrice.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbSpotPrice.Location = new System.Drawing.Point(141, 55);
			this.tbSpotPrice.Name = "tbSpotPrice";
			this.tbSpotPrice.Size = new System.Drawing.Size(88, 13);
			this.tbSpotPrice.TabIndex = 4;
			this.tbSpotPrice.Text = "0.00";
			this.tbSpotPrice.TextChanged += new System.EventHandler(this.tbSpotPrice_TextChanged);
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(15, 71);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(72, 16);
			this.label5.TabIndex = 10;
			this.label5.Text = "Strike Price:";
			// 
			// tbStrikePrice
			// 
			this.tbStrikePrice.BackColor = System.Drawing.SystemColors.Window;
			this.tbStrikePrice.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbStrikePrice.Location = new System.Drawing.Point(141, 71);
			this.tbStrikePrice.Name = "tbStrikePrice";
			this.tbStrikePrice.Size = new System.Drawing.Size(88, 13);
			this.tbStrikePrice.TabIndex = 5;
			this.tbStrikePrice.Text = "0.00";
			this.tbStrikePrice.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbStrikePrice_KeyPress);
			this.tbStrikePrice.TextChanged += new System.EventHandler(this.tbStrikePrice_TextChanged);
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(15, 111);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(104, 16);
			this.label6.TabIndex = 12;
			this.label6.Text = "Days to Expiration:";
			// 
			// tbDTE
			// 
			this.tbDTE.BackColor = System.Drawing.SystemColors.Window;
			this.tbDTE.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbDTE.Location = new System.Drawing.Point(141, 111);
			this.tbDTE.Name = "tbDTE";
			this.tbDTE.Size = new System.Drawing.Size(88, 13);
			this.tbDTE.TabIndex = 7;
			this.tbDTE.Text = "0";
			this.tbDTE.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbDTE_KeyPress);
			this.tbDTE.TextChanged += new System.EventHandler(this.tbDTE_TextChanged);
			this.tbDTE.Leave += new System.EventHandler(this.tbDTE_Leave);
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(14, 87);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(88, 16);
			this.label7.TabIndex = 13;
			this.label7.Text = "Expiration Date:";
			// 
			// dtpExpirationDate
			// 
			this.dtpExpirationDate.Format = System.Windows.Forms.DateTimePickerFormat.Short;
			this.dtpExpirationDate.Location = new System.Drawing.Point(141, 87);
			this.dtpExpirationDate.Name = "dtpExpirationDate";
			this.dtpExpirationDate.Size = new System.Drawing.Size(88, 20);
			this.dtpExpirationDate.TabIndex = 6;
			this.dtpExpirationDate.Leave += new System.EventHandler(this.dtpExpirationDate_Leave);
			this.dtpExpirationDate.ValueChanged += new System.EventHandler(this.dtpExpirationDate_ValueChanged);
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(15, 127);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(104, 16);
			this.label8.TabIndex = 16;
			this.label8.Text = "Volatility, %:";
			// 
			// tbVolatility
			// 
			this.tbVolatility.BackColor = System.Drawing.SystemColors.Window;
			this.tbVolatility.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbVolatility.Location = new System.Drawing.Point(141, 127);
			this.tbVolatility.Name = "tbVolatility";
			this.tbVolatility.Size = new System.Drawing.Size(88, 13);
			this.tbVolatility.TabIndex = 8;
			this.tbVolatility.Text = "0.00";
			this.tbVolatility.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbVolatility_KeyPress);
			this.tbVolatility.TextChanged += new System.EventHandler(this.tbVolatility_TextChanged);
			// 
			// label9
			// 
			this.label9.Location = new System.Drawing.Point(15, 143);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(104, 16);
			this.label9.TabIndex = 18;
			this.label9.Text = "Interest Rate, %:";
			// 
			// tbDomesticRate
			// 
			this.tbDomesticRate.BackColor = System.Drawing.SystemColors.Window;
			this.tbDomesticRate.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbDomesticRate.Location = new System.Drawing.Point(141, 143);
			this.tbDomesticRate.Name = "tbDomesticRate";
			this.tbDomesticRate.Size = new System.Drawing.Size(88, 13);
			this.tbDomesticRate.TabIndex = 9;
			this.tbDomesticRate.Text = "0.00";
			this.tbDomesticRate.TextChanged += new System.EventHandler(this.tbDomesticRate_TextChanged);
			// 
			// btnCalc
			// 
			this.btnCalc.BackColor = System.Drawing.Color.Silver;
			this.btnCalc.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnCalc.Location = new System.Drawing.Point(299, 315);
			this.btnCalc.Name = "btnCalc";
			this.btnCalc.Size = new System.Drawing.Size(212, 24);
			this.btnCalc.TabIndex = 30;
			this.btnCalc.Text = "Calculate";
			this.btnCalc.Click += new System.EventHandler(this.btnCalc_Click);
			// 
			// gbResults
			// 
			this.gbResults.Controls.Add(this.groupBox4);
			this.gbResults.Controls.Add(this.groupBox6);
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
			this.gbResults.Location = new System.Drawing.Point(299, 75);
			this.gbResults.Name = "gbResults";
			this.gbResults.Size = new System.Drawing.Size(300, 232);
			this.gbResults.TabIndex = 80;
			this.gbResults.TabStop = false;
			this.gbResults.Text = "Results:";
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.tbResCImpliedVola);
			this.groupBox4.Controls.Add(this.tbResCGammaTheta);
			this.groupBox4.Controls.Add(this.tbResCGammaVega);
			this.groupBox4.Controls.Add(this.tbResCDeltaTheta);
			this.groupBox4.Controls.Add(this.tbResCDeltaVega);
			this.groupBox4.Controls.Add(this.tbResCAlpha);
			this.groupBox4.Controls.Add(this.tbResCRho);
			this.groupBox4.Controls.Add(this.tbResCVega);
			this.groupBox4.Controls.Add(this.tbResCTheta);
			this.groupBox4.Controls.Add(this.tbResCGamma);
			this.groupBox4.Controls.Add(this.tbResCDelta);
			this.groupBox4.Controls.Add(this.tbResCPrice);
			this.groupBox4.Location = new System.Drawing.Point(93, 8);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(96, 217);
			this.groupBox4.TabIndex = 81;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Call:";
			// 
			// tbResCImpliedVola
			// 
			this.tbResCImpliedVola.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCImpliedVola.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCImpliedVola.Location = new System.Drawing.Point(8, 192);
			this.tbResCImpliedVola.Name = "tbResCImpliedVola";
			this.tbResCImpliedVola.ReadOnly = true;
			this.tbResCImpliedVola.Size = new System.Drawing.Size(80, 13);
			this.tbResCImpliedVola.TabIndex = 61;
			this.tbResCImpliedVola.Text = "0.00";
			this.tbResCImpliedVola.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCGammaTheta
			// 
			this.tbResCGammaTheta.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCGammaTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCGammaTheta.Location = new System.Drawing.Point(8, 176);
			this.tbResCGammaTheta.Name = "tbResCGammaTheta";
			this.tbResCGammaTheta.ReadOnly = true;
			this.tbResCGammaTheta.Size = new System.Drawing.Size(80, 13);
			this.tbResCGammaTheta.TabIndex = 60;
			this.tbResCGammaTheta.Text = "0.00";
			this.tbResCGammaTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCGammaVega
			// 
			this.tbResCGammaVega.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCGammaVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCGammaVega.Location = new System.Drawing.Point(8, 160);
			this.tbResCGammaVega.Name = "tbResCGammaVega";
			this.tbResCGammaVega.ReadOnly = true;
			this.tbResCGammaVega.Size = new System.Drawing.Size(80, 13);
			this.tbResCGammaVega.TabIndex = 59;
			this.tbResCGammaVega.Text = "0.00";
			this.tbResCGammaVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCDeltaTheta
			// 
			this.tbResCDeltaTheta.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCDeltaTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCDeltaTheta.Location = new System.Drawing.Point(8, 144);
			this.tbResCDeltaTheta.Name = "tbResCDeltaTheta";
			this.tbResCDeltaTheta.ReadOnly = true;
			this.tbResCDeltaTheta.Size = new System.Drawing.Size(80, 13);
			this.tbResCDeltaTheta.TabIndex = 58;
			this.tbResCDeltaTheta.Text = "0.00";
			this.tbResCDeltaTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCDeltaVega
			// 
			this.tbResCDeltaVega.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCDeltaVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCDeltaVega.Location = new System.Drawing.Point(8, 128);
			this.tbResCDeltaVega.Name = "tbResCDeltaVega";
			this.tbResCDeltaVega.ReadOnly = true;
			this.tbResCDeltaVega.Size = new System.Drawing.Size(80, 13);
			this.tbResCDeltaVega.TabIndex = 57;
			this.tbResCDeltaVega.Text = "0.00";
			this.tbResCDeltaVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCAlpha
			// 
			this.tbResCAlpha.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCAlpha.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCAlpha.Location = new System.Drawing.Point(8, 112);
			this.tbResCAlpha.Name = "tbResCAlpha";
			this.tbResCAlpha.ReadOnly = true;
			this.tbResCAlpha.Size = new System.Drawing.Size(80, 13);
			this.tbResCAlpha.TabIndex = 56;
			this.tbResCAlpha.Text = "0.00";
			this.tbResCAlpha.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCRho
			// 
			this.tbResCRho.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCRho.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCRho.Location = new System.Drawing.Point(8, 96);
			this.tbResCRho.Name = "tbResCRho";
			this.tbResCRho.ReadOnly = true;
			this.tbResCRho.Size = new System.Drawing.Size(80, 13);
			this.tbResCRho.TabIndex = 55;
			this.tbResCRho.Text = "0.00";
			this.tbResCRho.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCVega
			// 
			this.tbResCVega.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCVega.Location = new System.Drawing.Point(8, 80);
			this.tbResCVega.Name = "tbResCVega";
			this.tbResCVega.ReadOnly = true;
			this.tbResCVega.Size = new System.Drawing.Size(80, 13);
			this.tbResCVega.TabIndex = 54;
			this.tbResCVega.Text = "0.00";
			this.tbResCVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCTheta
			// 
			this.tbResCTheta.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCTheta.Location = new System.Drawing.Point(8, 64);
			this.tbResCTheta.Name = "tbResCTheta";
			this.tbResCTheta.ReadOnly = true;
			this.tbResCTheta.Size = new System.Drawing.Size(80, 13);
			this.tbResCTheta.TabIndex = 53;
			this.tbResCTheta.Text = "0.00";
			this.tbResCTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCGamma
			// 
			this.tbResCGamma.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCGamma.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCGamma.Location = new System.Drawing.Point(8, 48);
			this.tbResCGamma.Name = "tbResCGamma";
			this.tbResCGamma.ReadOnly = true;
			this.tbResCGamma.Size = new System.Drawing.Size(80, 13);
			this.tbResCGamma.TabIndex = 52;
			this.tbResCGamma.Text = "0.00";
			this.tbResCGamma.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCDelta
			// 
			this.tbResCDelta.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCDelta.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCDelta.Location = new System.Drawing.Point(8, 32);
			this.tbResCDelta.Name = "tbResCDelta";
			this.tbResCDelta.ReadOnly = true;
			this.tbResCDelta.Size = new System.Drawing.Size(80, 13);
			this.tbResCDelta.TabIndex = 51;
			this.tbResCDelta.Text = "0.00";
			this.tbResCDelta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResCPrice
			// 
			this.tbResCPrice.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResCPrice.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResCPrice.Location = new System.Drawing.Point(8, 16);
			this.tbResCPrice.Name = "tbResCPrice";
			this.tbResCPrice.ReadOnly = true;
			this.tbResCPrice.Size = new System.Drawing.Size(80, 13);
			this.tbResCPrice.TabIndex = 50;
			this.tbResCPrice.Text = "0.00";
			this.tbResCPrice.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// groupBox6
			// 
			this.groupBox6.Controls.Add(this.tbResPImpliedVola);
			this.groupBox6.Controls.Add(this.tbResPGammaTheta);
			this.groupBox6.Controls.Add(this.tbResPGammaVega);
			this.groupBox6.Controls.Add(this.tbResPDeltaTheta);
			this.groupBox6.Controls.Add(this.tbResPDeltaVega);
			this.groupBox6.Controls.Add(this.tbResPAlpha);
			this.groupBox6.Controls.Add(this.tbResPRho);
			this.groupBox6.Controls.Add(this.tbResPVega);
			this.groupBox6.Controls.Add(this.tbResPTheta);
			this.groupBox6.Controls.Add(this.tbResPGamma);
			this.groupBox6.Controls.Add(this.tbResPDelta);
			this.groupBox6.Controls.Add(this.tbResPPrice);
			this.groupBox6.Location = new System.Drawing.Point(197, 8);
			this.groupBox6.Name = "groupBox6";
			this.groupBox6.Size = new System.Drawing.Size(96, 217);
			this.groupBox6.TabIndex = 82;
			this.groupBox6.TabStop = false;
			this.groupBox6.Text = "Put:";
			// 
			// tbResPImpliedVola
			// 
			this.tbResPImpliedVola.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPImpliedVola.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPImpliedVola.Location = new System.Drawing.Point(8, 192);
			this.tbResPImpliedVola.Name = "tbResPImpliedVola";
			this.tbResPImpliedVola.ReadOnly = true;
			this.tbResPImpliedVola.Size = new System.Drawing.Size(80, 13);
			this.tbResPImpliedVola.TabIndex = 73;
			this.tbResPImpliedVola.Text = "0.00";
			this.tbResPImpliedVola.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPGammaTheta
			// 
			this.tbResPGammaTheta.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPGammaTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPGammaTheta.Location = new System.Drawing.Point(8, 176);
			this.tbResPGammaTheta.Name = "tbResPGammaTheta";
			this.tbResPGammaTheta.ReadOnly = true;
			this.tbResPGammaTheta.Size = new System.Drawing.Size(80, 13);
			this.tbResPGammaTheta.TabIndex = 72;
			this.tbResPGammaTheta.Text = "0.00";
			this.tbResPGammaTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPGammaVega
			// 
			this.tbResPGammaVega.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPGammaVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPGammaVega.Location = new System.Drawing.Point(8, 160);
			this.tbResPGammaVega.Name = "tbResPGammaVega";
			this.tbResPGammaVega.ReadOnly = true;
			this.tbResPGammaVega.Size = new System.Drawing.Size(80, 13);
			this.tbResPGammaVega.TabIndex = 71;
			this.tbResPGammaVega.Text = "0.00";
			this.tbResPGammaVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPDeltaTheta
			// 
			this.tbResPDeltaTheta.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPDeltaTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPDeltaTheta.Location = new System.Drawing.Point(8, 144);
			this.tbResPDeltaTheta.Name = "tbResPDeltaTheta";
			this.tbResPDeltaTheta.ReadOnly = true;
			this.tbResPDeltaTheta.Size = new System.Drawing.Size(80, 13);
			this.tbResPDeltaTheta.TabIndex = 70;
			this.tbResPDeltaTheta.Text = "0.00";
			this.tbResPDeltaTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPDeltaVega
			// 
			this.tbResPDeltaVega.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPDeltaVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPDeltaVega.Location = new System.Drawing.Point(8, 128);
			this.tbResPDeltaVega.Name = "tbResPDeltaVega";
			this.tbResPDeltaVega.ReadOnly = true;
			this.tbResPDeltaVega.Size = new System.Drawing.Size(80, 13);
			this.tbResPDeltaVega.TabIndex = 69;
			this.tbResPDeltaVega.Text = "0.00";
			this.tbResPDeltaVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPAlpha
			// 
			this.tbResPAlpha.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPAlpha.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPAlpha.Location = new System.Drawing.Point(8, 112);
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
			this.tbResPRho.Location = new System.Drawing.Point(8, 96);
			this.tbResPRho.Name = "tbResPRho";
			this.tbResPRho.ReadOnly = true;
			this.tbResPRho.Size = new System.Drawing.Size(80, 13);
			this.tbResPRho.TabIndex = 67;
			this.tbResPRho.Text = "0.00";
			this.tbResPRho.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPVega
			// 
			this.tbResPVega.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPVega.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPVega.Location = new System.Drawing.Point(8, 80);
			this.tbResPVega.Name = "tbResPVega";
			this.tbResPVega.ReadOnly = true;
			this.tbResPVega.Size = new System.Drawing.Size(80, 13);
			this.tbResPVega.TabIndex = 66;
			this.tbResPVega.Text = "0.00";
			this.tbResPVega.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPTheta
			// 
			this.tbResPTheta.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPTheta.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPTheta.Location = new System.Drawing.Point(8, 64);
			this.tbResPTheta.Name = "tbResPTheta";
			this.tbResPTheta.ReadOnly = true;
			this.tbResPTheta.Size = new System.Drawing.Size(80, 13);
			this.tbResPTheta.TabIndex = 65;
			this.tbResPTheta.Text = "0.00";
			this.tbResPTheta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPGamma
			// 
			this.tbResPGamma.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPGamma.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPGamma.Location = new System.Drawing.Point(8, 48);
			this.tbResPGamma.Name = "tbResPGamma";
			this.tbResPGamma.ReadOnly = true;
			this.tbResPGamma.Size = new System.Drawing.Size(80, 13);
			this.tbResPGamma.TabIndex = 64;
			this.tbResPGamma.Text = "0.00";
			this.tbResPGamma.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPDelta
			// 
			this.tbResPDelta.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPDelta.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPDelta.Location = new System.Drawing.Point(8, 32);
			this.tbResPDelta.Name = "tbResPDelta";
			this.tbResPDelta.ReadOnly = true;
			this.tbResPDelta.Size = new System.Drawing.Size(80, 13);
			this.tbResPDelta.TabIndex = 63;
			this.tbResPDelta.Text = "0.00";
			this.tbResPDelta.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// tbResPPrice
			// 
			this.tbResPPrice.BackColor = System.Drawing.Color.WhiteSmoke;
			this.tbResPPrice.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbResPPrice.Location = new System.Drawing.Point(8, 16);
			this.tbResPPrice.Name = "tbResPPrice";
			this.tbResPPrice.ReadOnly = true;
			this.tbResPPrice.Size = new System.Drawing.Size(80, 13);
			this.tbResPPrice.TabIndex = 62;
			this.tbResPPrice.Text = "0.00";
			this.tbResPPrice.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// label46
			// 
			this.label46.Location = new System.Drawing.Point(8, 56);
			this.label46.Name = "label46";
			this.label46.Size = new System.Drawing.Size(54, 16);
			this.label46.TabIndex = 12;
			this.label46.Text = "Gamma:";
			// 
			// label48
			// 
			this.label48.Location = new System.Drawing.Point(8, 24);
			this.label48.Name = "label48";
			this.label48.Size = new System.Drawing.Size(62, 16);
			this.label48.TabIndex = 8;
			this.label48.Text = "Price:";
			// 
			// label47
			// 
			this.label47.Location = new System.Drawing.Point(8, 40);
			this.label47.Name = "label47";
			this.label47.Size = new System.Drawing.Size(62, 16);
			this.label47.TabIndex = 10;
			this.label47.Text = "Delta:";
			// 
			// label37
			// 
			this.label37.Location = new System.Drawing.Point(8, 200);
			this.label37.Name = "label37";
			this.label37.Size = new System.Drawing.Size(88, 16);
			this.label37.TabIndex = 30;
			this.label37.Text = "Implied Vola, %:";
			// 
			// label38
			// 
			this.label38.Location = new System.Drawing.Point(8, 184);
			this.label38.Name = "label38";
			this.label38.Size = new System.Drawing.Size(87, 16);
			this.label38.TabIndex = 28;
			this.label38.Text = "Gamma Theta:";
			// 
			// label39
			// 
			this.label39.Location = new System.Drawing.Point(8, 168);
			this.label39.Name = "label39";
			this.label39.Size = new System.Drawing.Size(80, 16);
			this.label39.TabIndex = 26;
			this.label39.Text = "Gamma Vega:";
			// 
			// label40
			// 
			this.label40.Location = new System.Drawing.Point(8, 152);
			this.label40.Name = "label40";
			this.label40.Size = new System.Drawing.Size(72, 16);
			this.label40.TabIndex = 24;
			this.label40.Text = "Delta Theta:";
			// 
			// label41
			// 
			this.label41.Location = new System.Drawing.Point(8, 136);
			this.label41.Name = "label41";
			this.label41.Size = new System.Drawing.Size(80, 16);
			this.label41.TabIndex = 22;
			this.label41.Text = "Delta Vega:";
			// 
			// label42
			// 
			this.label42.Location = new System.Drawing.Point(8, 120);
			this.label42.Name = "label42";
			this.label42.Size = new System.Drawing.Size(54, 16);
			this.label42.TabIndex = 20;
			this.label42.Text = "Alpha:";
			// 
			// label43
			// 
			this.label43.Location = new System.Drawing.Point(8, 104);
			this.label43.Name = "label43";
			this.label43.Size = new System.Drawing.Size(54, 16);
			this.label43.TabIndex = 18;
			this.label43.Text = "Rho:";
			// 
			// label44
			// 
			this.label44.Location = new System.Drawing.Point(8, 88);
			this.label44.Name = "label44";
			this.label44.Size = new System.Drawing.Size(54, 16);
			this.label44.TabIndex = 16;
			this.label44.Text = "Vega:";
			// 
			// label45
			// 
			this.label45.Location = new System.Drawing.Point(8, 72);
			this.label45.Name = "label45";
			this.label45.Size = new System.Drawing.Size(54, 16);
			this.label45.TabIndex = 14;
			this.label45.Text = "Theta:";
			// 
			// btnClose
			// 
			this.btnClose.BackColor = System.Drawing.Color.Silver;
			this.btnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnClose.Location = new System.Drawing.Point(521, 435);
			this.btnClose.Name = "btnClose";
			this.btnClose.Size = new System.Drawing.Size(80, 23);
			this.btnClose.TabIndex = 100;
			this.btnClose.Text = "Close";
			this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.lbContinousRate);
			this.groupBox1.Controls.Add(this.tbContinousRate);
			this.groupBox1.Controls.Add(this.rbContinous);
			this.groupBox1.Controls.Add(this.rbDiscreteDividends);
			this.groupBox1.Controls.Add(this.gbDividends);
			this.groupBox1.Location = new System.Drawing.Point(15, 219);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(272, 240);
			this.groupBox1.TabIndex = 13;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Dividend Yield:";
			// 
			// lbContinousRate
			// 
			this.lbContinousRate.Location = new System.Drawing.Point(10, 33);
			this.lbContinousRate.Name = "lbContinousRate";
			this.lbContinousRate.Size = new System.Drawing.Size(104, 16);
			this.lbContinousRate.TabIndex = 20;
			this.lbContinousRate.Text = "Rate, %:";
			// 
			// tbContinousRate
			// 
			this.tbContinousRate.BackColor = System.Drawing.SystemColors.Window;
			this.tbContinousRate.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbContinousRate.Location = new System.Drawing.Point(127, 35);
			this.tbContinousRate.Name = "tbContinousRate";
			this.tbContinousRate.Size = new System.Drawing.Size(88, 13);
			this.tbContinousRate.TabIndex = 14;
			this.tbContinousRate.Text = "0.00";
			this.tbContinousRate.TextChanged += new System.EventHandler(this.tbContinousRate_TextChanged);
			// 
			// rbContinous
			// 
			this.rbContinous.Checked = true;
			this.rbContinous.Location = new System.Drawing.Point(8, 11);
			this.rbContinous.Name = "rbContinous";
			this.rbContinous.TabIndex = 15;
			this.rbContinous.TabStop = true;
			this.rbContinous.Text = "Continous:";
			this.rbContinous.CheckedChanged += new System.EventHandler(this.rbContinous_CheckedChanged);
			// 
			// rbDiscreteDividends
			// 
			this.rbDiscreteDividends.Location = new System.Drawing.Point(8, 49);
			this.rbDiscreteDividends.Name = "rbDiscreteDividends";
			this.rbDiscreteDividends.Size = new System.Drawing.Size(104, 16);
			this.rbDiscreteDividends.TabIndex = 16;
			this.rbDiscreteDividends.Text = "Discrete:";
			this.rbDiscreteDividends.CheckedChanged += new System.EventHandler(this.rbDiscreteDividends_CheckedChanged);
			// 
			// gbDividends
			// 
			this.gbDividends.Controls.Add(this.rCustomDivs);
			this.gbDividends.Controls.Add(this.gbPeriodicDivs);
			this.gbDividends.Controls.Add(this.gbCustomDivs);
			this.gbDividends.Controls.Add(this.rPeriodic);
			this.gbDividends.Enabled = false;
			this.gbDividends.Location = new System.Drawing.Point(4, 63);
			this.gbDividends.Name = "gbDividends";
			this.gbDividends.Size = new System.Drawing.Size(252, 169);
			this.gbDividends.TabIndex = 17;
			this.gbDividends.TabStop = false;
			// 
			// rCustomDivs
			// 
			this.rCustomDivs.Location = new System.Drawing.Point(8, 106);
			this.rCustomDivs.Name = "rCustomDivs";
			this.rCustomDivs.Size = new System.Drawing.Size(64, 20);
			this.rCustomDivs.TabIndex = 23;
			this.rCustomDivs.Text = "Stream:";
			this.rCustomDivs.CheckedChanged += new System.EventHandler(this.rCustomDivs_CheckedChanged);
			// 
			// gbPeriodicDivs
			// 
			this.gbPeriodicDivs.Controls.Add(this.label12);
			this.gbPeriodicDivs.Controls.Add(this.dtpDivDate);
			this.gbPeriodicDivs.Controls.Add(this.cbDivFreq);
			this.gbPeriodicDivs.Controls.Add(this.label4);
			this.gbPeriodicDivs.Controls.Add(this.label3);
			this.gbPeriodicDivs.Controls.Add(this.tbDivAmount);
			this.gbPeriodicDivs.Location = new System.Drawing.Point(5, 23);
			this.gbPeriodicDivs.Name = "gbPeriodicDivs";
			this.gbPeriodicDivs.Size = new System.Drawing.Size(243, 82);
			this.gbPeriodicDivs.TabIndex = 19;
			this.gbPeriodicDivs.TabStop = false;
			// 
			// label12
			// 
			this.label12.Location = new System.Drawing.Point(4, 16);
			this.label12.Name = "label12";
			this.label12.Size = new System.Drawing.Size(100, 16);
			this.label12.TabIndex = 20;
			this.label12.Text = "Dividends Date:";
			// 
			// dtpDivDate
			// 
			this.dtpDivDate.Format = System.Windows.Forms.DateTimePickerFormat.Short;
			this.dtpDivDate.Location = new System.Drawing.Point(120, 11);
			this.dtpDivDate.Name = "dtpDivDate";
			this.dtpDivDate.Size = new System.Drawing.Size(88, 20);
			this.dtpDivDate.TabIndex = 20;
			this.dtpDivDate.ValueChanged += new System.EventHandler(this.dtpDivDate_ValueChanged);
			// 
			// cbDivFreq
			// 
			this.cbDivFreq.Items.AddRange(new object[] {
														   "Monthly",
														   "Quarterly",
														   "Semiannually",
														   "Annually"});
			this.cbDivFreq.Location = new System.Drawing.Point(120, 56);
			this.cbDivFreq.Name = "cbDivFreq";
			this.cbDivFreq.Size = new System.Drawing.Size(88, 21);
			this.cbDivFreq.TabIndex = 22;
			this.cbDivFreq.Text = "Monthly";
			this.cbDivFreq.TextChanged += new System.EventHandler(this.cbDivFreq_TextChanged);
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(2, 57);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(129, 16);
			this.label4.TabIndex = 18;
			this.label4.Text = "Dividends Frequency:";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(3, 35);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(104, 16);
			this.label3.TabIndex = 17;
			this.label3.Text = "Dividends Amount:";
			// 
			// tbDivAmount
			// 
			this.tbDivAmount.BackColor = System.Drawing.SystemColors.Window;
			this.tbDivAmount.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbDivAmount.Location = new System.Drawing.Point(120, 35);
			this.tbDivAmount.Name = "tbDivAmount";
			this.tbDivAmount.Size = new System.Drawing.Size(88, 13);
			this.tbDivAmount.TabIndex = 21;
			this.tbDivAmount.Text = "0.00";
			this.tbDivAmount.TextChanged += new System.EventHandler(this.tbDivAmount_TextChanged);
			// 
			// gbCustomDivs
			// 
			this.gbCustomDivs.Controls.Add(this.btnSetCustomDividends);
			this.gbCustomDivs.Enabled = false;
			this.gbCustomDivs.Location = new System.Drawing.Point(8, 121);
			this.gbCustomDivs.Name = "gbCustomDivs";
			this.gbCustomDivs.Size = new System.Drawing.Size(240, 40);
			this.gbCustomDivs.TabIndex = 24;
			this.gbCustomDivs.TabStop = false;
			// 
			// btnSetCustomDividends
			// 
			this.btnSetCustomDividends.BackColor = System.Drawing.Color.Silver;
			this.btnSetCustomDividends.Location = new System.Drawing.Point(117, 10);
			this.btnSetCustomDividends.Name = "btnSetCustomDividends";
			this.btnSetCustomDividends.Size = new System.Drawing.Size(89, 23);
			this.btnSetCustomDividends.TabIndex = 25;
			this.btnSetCustomDividends.Text = "Show";
			this.btnSetCustomDividends.Click += new System.EventHandler(this.btnSetCustomDividends_Click);
			// 
			// rPeriodic
			// 
			this.rPeriodic.Checked = true;
			this.rPeriodic.Location = new System.Drawing.Point(8, 10);
			this.rPeriodic.Name = "rPeriodic";
			this.rPeriodic.Size = new System.Drawing.Size(80, 16);
			this.rPeriodic.TabIndex = 18;
			this.rPeriodic.TabStop = true;
			this.rPeriodic.Text = "Periodic:";
			this.rPeriodic.CheckedChanged += new System.EventHandler(this.rPeriodic_CheckedChanged);
			// 
			// btnExport
			// 
			this.btnExport.BackColor = System.Drawing.Color.Silver;
			this.btnExport.Enabled = false;
			this.btnExport.Location = new System.Drawing.Point(515, 315);
			this.btnExport.Name = "btnExport";
			this.btnExport.Size = new System.Drawing.Size(84, 24);
			this.btnExport.TabIndex = 31;
			this.btnExport.Text = "Export";
			// 
			// label13
			// 
			this.label13.Location = new System.Drawing.Point(303, 19);
			this.label13.Name = "label13";
			this.label13.Size = new System.Drawing.Size(80, 16);
			this.label13.TabIndex = 29;
			this.label13.Text = "Symbol:";
			// 
			// tbSymbol
			// 
			this.tbSymbol.BackColor = System.Drawing.SystemColors.Window;
			this.tbSymbol.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbSymbol.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(204)));
			this.tbSymbol.Location = new System.Drawing.Point(303, 35);
			this.tbSymbol.Name = "tbSymbol";
			this.tbSymbol.Size = new System.Drawing.Size(104, 14);
			this.tbSymbol.TabIndex = 1;
			this.tbSymbol.Text = "";
			this.tbSymbol.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbSymbol_KeyPress);
			this.tbSymbol.TextChanged += new System.EventHandler(this.tbSymbol_TextChanged);
			this.tbSymbol.Leave += new System.EventHandler(this.tbSymbol_Leave);
			// 
			// cbStyle
			// 
			this.cbStyle.Items.AddRange(new object[] {
														 "American",
														 "European"});
			this.cbStyle.Location = new System.Drawing.Point(141, 6);
			this.cbStyle.Name = "cbStyle";
			this.cbStyle.Size = new System.Drawing.Size(88, 21);
			this.cbStyle.TabIndex = 2;
			this.cbStyle.Text = "American";
			this.cbStyle.Click += new System.EventHandler(this.cbStyle_Click);
			this.cbStyle.Leave += new System.EventHandler(this.cbStyle_Leave);
			// 
			// label14
			// 
			this.label14.Location = new System.Drawing.Point(15, 9);
			this.label14.Name = "label14";
			this.label14.Size = new System.Drawing.Size(80, 16);
			this.label14.TabIndex = 32;
			this.label14.Text = "Style:";
			// 
			// sbStatus
			// 
			this.sbStatus.Location = new System.Drawing.Point(0, 467);
			this.sbStatus.Name = "sbStatus";
			this.sbStatus.Size = new System.Drawing.Size(610, 16);
			this.sbStatus.TabIndex = 41;
			this.sbStatus.Text = "Status";
			// 
			// gbCalcModel
			// 
			this.gbCalcModel.Controls.Add(this.rbStandart);
			this.gbCalcModel.Controls.Add(this.rbOptimized);
			this.gbCalcModel.Location = new System.Drawing.Point(423, 19);
			this.gbCalcModel.Name = "gbCalcModel";
			this.gbCalcModel.Size = new System.Drawing.Size(176, 40);
			this.gbCalcModel.TabIndex = 26;
			this.gbCalcModel.TabStop = false;
			this.gbCalcModel.Text = "Model:";
			// 
			// rbStandart
			// 
			this.rbStandart.Location = new System.Drawing.Point(11, 11);
			this.rbStandart.Name = "rbStandart";
			this.rbStandart.Size = new System.Drawing.Size(69, 24);
			this.rbStandart.TabIndex = 27;
			this.rbStandart.Text = "Standard";
			this.rbStandart.CheckedChanged += new System.EventHandler(this.rbStandart_CheckedChanged);
			// 
			// rbOptimized
			// 
			this.rbOptimized.Checked = true;
			this.rbOptimized.Location = new System.Drawing.Point(88, 16);
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
			this.tbCOptionPrice.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbCOptionPrice.Location = new System.Drawing.Point(128, 18);
			this.tbCOptionPrice.Name = "tbCOptionPrice";
			this.tbCOptionPrice.Size = new System.Drawing.Size(88, 13);
			this.tbCOptionPrice.TabIndex = 11;
			this.tbCOptionPrice.Text = "0.00";
			this.tbCOptionPrice.TextChanged += new System.EventHandler(this.tbCOptionPrice_TextChanged);
			// 
			// oleDbConnection
			// 
			this.oleDbConnection.ConnectionString = "Provider=SQLOLEDB.1;Password=sa;Persist Security Info=True;User ID=sa;Initial Cat" +
				"alog=ETS_ASP_TESTDB;Data Source=CROCODILE\\CROC2000";
			// 
			// oleDbCommand
			// 
			this.oleDbCommand.Connection = this.oleDbConnection;
			this.oleDbCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@sInSymbol", System.Data.OleDb.OleDbType.VarChar, 20));
			this.oleDbCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iRowCount", System.Data.OleDb.OleDbType.Integer, 0, System.Data.ParameterDirection.Output, false, ((System.Byte)(0)), ((System.Byte)(0)), "", System.Data.DataRowVersion.Current, null));
			this.oleDbCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iStockID", System.Data.OleDb.OleDbType.Integer, 0, System.Data.ParameterDirection.Output, false, ((System.Byte)(0)), ((System.Byte)(0)), "", System.Data.DataRowVersion.Current, null));
			// 
			// ReadCommand
			// 
			this.ReadCommand.Connection = this.oleDbConnection;
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.label16);
			this.groupBox2.Controls.Add(this.tbPOptionPrice);
			this.groupBox2.Controls.Add(this.label15);
			this.groupBox2.Controls.Add(this.tbCOptionPrice);
			this.groupBox2.Location = new System.Drawing.Point(15, 163);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(272, 56);
			this.groupBox2.TabIndex = 10;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Option price (for IV only):";
			// 
			// label16
			// 
			this.label16.Location = new System.Drawing.Point(8, 34);
			this.label16.Name = "label16";
			this.label16.Size = new System.Drawing.Size(32, 15);
			this.label16.TabIndex = 46;
			this.label16.Text = "Put:";
			// 
			// tbPOptionPrice
			// 
			this.tbPOptionPrice.BackColor = System.Drawing.SystemColors.Window;
			this.tbPOptionPrice.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.tbPOptionPrice.Location = new System.Drawing.Point(128, 36);
			this.tbPOptionPrice.Name = "tbPOptionPrice";
			this.tbPOptionPrice.Size = new System.Drawing.Size(88, 13);
			this.tbPOptionPrice.TabIndex = 12;
			this.tbPOptionPrice.Text = "0.00";
			this.tbPOptionPrice.TextChanged += new System.EventHandler(this.tbPOptionPrice_TextChanged);
			// 
			// label15
			// 
			this.label15.Location = new System.Drawing.Point(8, 16);
			this.label15.Name = "label15";
			this.label15.Size = new System.Drawing.Size(32, 15);
			this.label15.TabIndex = 44;
			this.label15.Text = "Call:";
			// 
			// FeedTimer
			// 
			this.FeedTimer.Interval = 10000;
			this.FeedTimer.SynchronizingObject = this;
			this.FeedTimer.Elapsed += new System.Timers.ElapsedEventHandler(this.FeedTimer_Elapsed);
			// 
			// label10
			// 
			this.label10.Location = new System.Drawing.Point(15, 33);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(80, 16);
			this.label10.TabIndex = 48;
			this.label10.Text = "Symbol type:";
			// 
			// cbSymbolType
			// 
			this.cbSymbolType.Items.AddRange(new object[] {
															  "Index/Stock",
															  "Future"});
			this.cbSymbolType.Location = new System.Drawing.Point(141, 30);
			this.cbSymbolType.Name = "cbSymbolType";
			this.cbSymbolType.Size = new System.Drawing.Size(88, 21);
			this.cbSymbolType.TabIndex = 3;
			this.cbSymbolType.Text = "Index/Stock";
			this.cbSymbolType.Click += new System.EventHandler(this.cbSymbolType_Click);
			this.cbSymbolType.SelectedIndexChanged += new System.EventHandler(this.cbSymbolType_SelectedIndexChanged);
			// 
			// InitTimer
			// 
			this.InitTimer.Interval = 300;
			this.InitTimer.SynchronizingObject = this;
			this.InitTimer.Elapsed += new System.Timers.ElapsedEventHandler(this.InitTimer_Elapsed);
			// 
			// ReadExpCommand
			// 
			this.ReadExpCommand.CommandType = System.Data.CommandType.StoredProcedure;
			this.ReadExpCommand.Connection = this.oleDbConnection;
			this.ReadExpCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iContractID", System.Data.OleDb.OleDbType.Integer));
			this.ReadExpCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iTraderID", System.Data.OleDb.OleDbType.Integer));
			this.ReadExpCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iGroupID", System.Data.OleDb.OleDbType.Integer));
			this.ReadExpCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@dtMinExpiry", System.Data.OleDb.OleDbType.Date));
			// 
			// ReadOptsCommand
			// 
			this.ReadOptsCommand.Connection = this.oleDbConnection;
			this.ReadOptsCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iUnderlyingID", System.Data.OleDb.OleDbType.Integer));
			this.ReadOptsCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iExpiryID", System.Data.OleDb.OleDbType.Integer));
			// 
			// mainMenu
			// 
			this.mainMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.mnuFile,
																					 this.mnuData,
																					 this.mnuHelp});
			// 
			// mnuFile
			// 
			this.mnuFile.Index = 0;
			this.mnuFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.mnuExit});
			this.mnuFile.Text = "&File";
			// 
			// mnuExit
			// 
			this.mnuExit.DefaultItem = true;
			this.mnuExit.Index = 0;
			this.mnuExit.Text = "E&xit";
			this.mnuExit.Click += new System.EventHandler(this.mnuExit_Click);
			// 
			// mnuData
			// 
			this.mnuData.Index = 1;
			this.mnuData.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.mnuQuotes,
																					this.mnuVolatility,
																					this.menuItem1,
																					this.mnuCalculate});
			this.mnuData.Text = "D&ata";
			// 
			// mnuQuotes
			// 
			this.mnuQuotes.Index = 0;
			this.mnuQuotes.Shortcut = System.Windows.Forms.Shortcut.F5;
			this.mnuQuotes.Text = "Refresh &Quotes";
			this.mnuQuotes.Click += new System.EventHandler(this.mnuQuotes_Click);
			// 
			// mnuVolatility
			// 
			this.mnuVolatility.Index = 1;
			this.mnuVolatility.Shortcut = System.Windows.Forms.Shortcut.F6;
			this.mnuVolatility.Text = "Refresh &Volatility";
			this.mnuVolatility.Click += new System.EventHandler(this.mnuVolatility_Click);
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 2;
			this.menuItem1.Text = "-";
			// 
			// mnuCalculate
			// 
			this.mnuCalculate.DefaultItem = true;
			this.mnuCalculate.Index = 3;
			this.mnuCalculate.Shortcut = System.Windows.Forms.Shortcut.F8;
			this.mnuCalculate.Text = "&Calculate";
			this.mnuCalculate.Click += new System.EventHandler(this.mnuCalculate_Click);
			// 
			// mnuHelp
			// 
			this.mnuHelp.Index = 2;
			this.mnuHelp.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.mnuHelpContents,
																					this.menuItem3,
																					this.mnuAbout});
			this.mnuHelp.Text = "&Help";
			// 
			// mnuHelpContents
			// 
			this.mnuHelpContents.Enabled = false;
			this.mnuHelpContents.Index = 0;
			this.mnuHelpContents.Shortcut = System.Windows.Forms.Shortcut.F1;
			this.mnuHelpContents.Text = "Contents";
			this.mnuHelpContents.Visible = false;
			this.mnuHelpContents.Click += new System.EventHandler(this.mnuHelpContents_Click);
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 1;
			this.menuItem3.Text = "-";
			this.menuItem3.Visible = false;
			// 
			// mnuAbout
			// 
			this.mnuAbout.Index = 2;
			this.mnuAbout.Text = "&About";
			this.mnuAbout.Click += new System.EventHandler(this.mnuAbout_Click);
			// 
			// gbMain
			// 
			this.gbMain.Location = new System.Drawing.Point(4, -2);
			this.gbMain.Name = "gbMain";
			this.gbMain.Size = new System.Drawing.Size(603, 467);
			this.gbMain.TabIndex = 101;
			this.gbMain.TabStop = false;
			// 
			// ReadCurveCommand
			// 
			this.ReadCurveCommand.Connection = this.oleDbConnection;
			this.ReadCurveCommand.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iCurveID", System.Data.OleDb.OleDbType.Integer));
			// 
			// GetCustomDivs
			// 
			this.GetCustomDivs.CommandText = "dbo.[usp_MmCustomDividend_Get]";
			this.GetCustomDivs.CommandType = System.Data.CommandType.StoredProcedure;
			this.GetCustomDivs.Connection = this.oleDbConnection;
			this.GetCustomDivs.Parameters.Add(new System.Data.OleDb.OleDbParameter("@RETURN_VALUE", System.Data.OleDb.OleDbType.Integer, 4, System.Data.ParameterDirection.ReturnValue, false, ((System.Byte)(0)), ((System.Byte)(0)), "", System.Data.DataRowVersion.Current, ""));
			this.GetCustomDivs.Parameters.Add(new System.Data.OleDb.OleDbParameter("@iStockID", System.Data.OleDb.OleDbType.Integer, 4, System.Data.ParameterDirection.Input, false, ((System.Byte)(0)), ((System.Byte)(0)), "", System.Data.DataRowVersion.Current, "0"));
			// 
			// ReadCustCommand
			// 
			this.ReadCustCommand.Connection = this.oleDbConnection;
			// 
			// OptionCalculatorForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.Gainsboro;
			this.ClientSize = new System.Drawing.Size(610, 483);
			this.Controls.Add(this.label10);
			this.Controls.Add(this.cbSymbolType);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.tbSymbol);
			this.Controls.Add(this.tbDomesticRate);
			this.Controls.Add(this.tbVolatility);
			this.Controls.Add(this.tbDTE);
			this.Controls.Add(this.tbStrikePrice);
			this.Controls.Add(this.tbSpotPrice);
			this.Controls.Add(this.gbCalcModel);
			this.Controls.Add(this.sbStatus);
			this.Controls.Add(this.label14);
			this.Controls.Add(this.cbStyle);
			this.Controls.Add(this.label13);
			this.Controls.Add(this.btnExport);
			this.Controls.Add(this.btnClose);
			this.Controls.Add(this.gbResults);
			this.Controls.Add(this.label9);
			this.Controls.Add(this.label8);
			this.Controls.Add(this.dtpExpirationDate);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.lbSpotOrFuturePrice);
			this.Controls.Add(this.btnCalc);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.gbMain);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Menu = this.mainMenu;
			this.Name = "OptionCalculatorForm";
			this.Text = "OTC Option Calculator";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.OptionCalculatorForm_Closing);
			this.Load += new System.EventHandler(this.OptionCalculatorForm_Load);
			this.gbResults.ResumeLayout(false);
			this.groupBox4.ResumeLayout(false);
			this.groupBox6.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.gbDividends.ResumeLayout(false);
			this.gbPeriodicDivs.ResumeLayout(false);
			this.gbCustomDivs.ResumeLayout(false);
			this.gbCalcModel.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.FeedTimer)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.InitTimer)).EndInit();
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
			Application.Exit();
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

			tbResCImpliedVola.Text = "0.00";
			tbResPImpliedVola.Text = "0.00";
			sAtmPut = "";
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
			long	lDTE;
			long	lIsAmerican;
			long	lDivCount	= 1;
			long	lSteps		= 100;
			int		iRet = 0;
			int		lLastDivDate;
			int		lDivFrequency;
			double	dAmount;
			double  dCIV; 
			double  dPIV;
			string	sResStatus = "";
			bool	bCalculatingFuture = false;
			OCWrapper.DIVIDEND [] oDivs = new OCWrapper.DIVIDEND[0];

			if (cbSymbolType.SelectedIndex == 1)
				bCalculatingFuture = true;
			
			long	lModel		= 3;	// Optimized Binominal
			if (rbStandart.Checked )
				lModel		= 2;		// Standart Binominal
			int lDateTimeNow;
			try
			{
				lDateTimeNow = Convert.ToInt32(DateTime.Now.ToOADate());
			}
			catch
			{
				sbStatus.Text = "Wrong date, nothing calculated";
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
				dSpotPrice = (double)System.Convert.ToDouble(tbSpotPrice.Text);
			}
			catch
			{
				sbStatus.Text = "Wrong spot price, nothing calculated";
				return;
			}			
			
			try
			{
				dStrikePrice = (double)System.Convert.ToDouble(tbStrikePrice.Text);
			}
			catch
			{
				sbStatus.Text = "Wrong strike, nothing calculated";
				return;
			}			

			try
			{
				dCOptionPrice = (double)System.Convert.ToDouble(tbCOptionPrice.Text);
			}
			catch
			{
				//sbStatus.Text = "Wrong call option price, nothing calculated";
				tbCOptionPrice.Text = "0.00";
				dCOptionPrice = 0;
				//return;
			}			


			try
			{
				dPOptionPrice = (double)System.Convert.ToDouble(tbPOptionPrice.Text);
			}
			catch
			{
				//sbStatus.Text = "Wrong put option price, nothing calculated";
				tbPOptionPrice.Text = "0.00";
				dPOptionPrice = 0;
				//return;
			}			
			

			try
			{
				dDomesticRate = ((double)System.Convert.ToDouble(tbDomesticRate.Text))/100.0;
			}
			catch
			{
				sbStatus.Text = "Wrong domestic rate, nothing calculated";
				return;
			}			

			try
			{
				dVolatility = ((double)System.Convert.ToDouble(tbVolatility.Text))/100.0;
			}
			catch
			{
				sbStatus.Text = "Wrong volatility value, nothing calculated";
				return;
			}			

			


			dSkew = 0.0;
			dKurtosis = 0.0;
			lDTE = System.Convert.ToInt32(tbDTE.Text);
			lIsAmerican = System.Convert.ToInt32(cbStyle.SelectedIndex == 0);

			int iGotCount = 0;
		
			dForeignRate = 0;
			if (rbContinous.Checked)
			{
				// Continous
				
				try
				{
					dForeignRate = ((double)System.Convert.ToDouble(tbContinousRate.Text))/100.0;
				}
				catch
				{
					sbStatus.Text = "Wrong foreign rate, nothing calculated";
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
						dAmount = (double)System.Convert.ToDouble(tbDivAmount.Text);
					}
					catch
					{
						sbStatus.Text = "Wrong dividend amount, nothing calculated";
						return;
					}			
					

					try
					{
						double dTmp = System.Convert.ToDouble(dtpDivDate.Value.ToOADate());				
						lLastDivDate = System.Convert.ToInt32(dTmp);
					}
					catch
					{
						sbStatus.Text = "Wrong dividend date, nothing calculated";
						return;
					}			

					try
					{
						lDivCount = oOCWrapper.OCWGetDividendsCount( lDateTimeNow, (int)lDTE, lLastDivDate,lDivFrequency  );
					}
					catch
					{
						sbStatus.Text = "OptionCalc GetDividendsCount failed, nothing calculated";
						return;
					}			

					if (lDivCount > 0)
					{
						iGotCount = (int)lDivCount;
						Array.Clear(dAmounts, 0, dAmounts.Length );
						Array.Clear(dYears, 0, dYears.Length );
						dAmounts = new double[lDivCount];
						dYears	= new double[lDivCount];
						oOCWrapper.OCWGetDividends(lDateTimeNow, (int)lDTE, lLastDivDate, lDivFrequency, dAmount, (int)lDivCount, ref dAmounts, ref dYears, out iGotCount);
					
					}
					
				
				}
				else
				{
					// Stream

					Array.Clear(dAmounts, 0, dAmounts.Length );
					Array.Clear(dYears, 0, dYears.Length);

					listTmpCust.Clear();
					fDivForm.GetCustomDividendList(ref listTmpCust);


					double dToday = DateTime.Now.ToOADate();
					int nToday = Convert.ToInt32(dToday);
					int nRetCount = 0;
					int iCount = 0;
					System.Array arTmpAmounts = new double[0];
					System.Array arTmpDates = new double[0];

					m_CustDivs.Clear();
					if (listTmpCust.Count > 0 )
					{
						Record tmpRecord;
						DateTime dtTmp;
						for ( int iCounter = 0; iCounter<listTmpCust.Count; iCounter++)
						{
					
							tmpRecord = (Record)listTmpCust[iCounter];
							dtTmp = DateTime.Parse(tmpRecord.DivDate);
							m_CustDivs.Add(dtTmp, Convert.ToDouble(tmpRecord.DivAmount));
						}
					}
					

					m_CustDivs.GetDividendCount((int)nToday, (int)(nToday + lDTE), out iCount);
					if (iCount > 0)
					{
						dAmounts = new double[iCount];
						dYears = new double[iCount];
						m_CustDivs.GetDividends((int)nToday, (int)(nToday + lDTE), (int)iCount, out arTmpAmounts, out arTmpDates, out (int)nRetCount);
						
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
						// force no divs
						lDivCount = 0;
						dAmounts = new double[lDivCount];
						dYears	= new double[lDivCount];
						iGotCount = 0;

						//dCIV = oOCWrapper.OCWCalcFutureOptionVolatilityMM( dDomesticRate, dSpotPrice, dCOptionPrice, dStrikePrice , (int)lDTE, 1 /* Call */, (int)lIsAmerican,  (int)lSteps , dSkew,  dKurtosis ,(int)lDivCount, ref dAmounts  ,  ref dYears , (int)lModel, out iGotCount);
						dCIV = oOCWrapper.OCWCalcFutureOptionVolatility( dDomesticRate, dSpotPrice, dCOptionPrice, dStrikePrice , (int)lDTE, 1 /* Call */, (int)lIsAmerican,  (int)lSteps , dSkew,  dKurtosis , (int)lModel, out iGotCount);
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
						dCIV = oOCWrapper.OCWCalcVolatilityMM( dDomesticRate, dForeignRate, dSpotPrice, dCOptionPrice, dStrikePrice , (int)lDTE, 1 /* Call */, (int)lIsAmerican, (int)lDivCount, ref dAmounts  ,  ref dYears , (int)lSteps , dSkew,  dKurtosis , (int)lModel);
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
					//iRet = oOCWrapper.OCWCalcFutureOptionGreeksMM(dDomesticRate, dSpotPrice, dStrikePrice , dVolatility, (int)lDTE, 1 /* Call */, (int)lIsAmerican, (int)lSteps , dSkew,  dKurtosis , (int)lModel, (int)lDivCount, ref dAmounts  ,  ref dYears ,  refResults);			
					iRet = oOCWrapper.OCWCalcFutureOptionGreeks(dDomesticRate, dSpotPrice, dStrikePrice , dVolatility, (int)lDTE, 1 /* Call */, (int)lIsAmerican, (int)lSteps , dSkew,  dKurtosis , (int)lModel,  refResults);			
				}
				catch
				{
					iRet = -2;
					sResStatus = "Call: OptionCalc library call error";
				}
			}
			else
			{
				try
				{
					iRet = oOCWrapper.OCWCalcGreeksMM(dDomesticRate, dForeignRate, dSpotPrice, dStrikePrice , dVolatility, (int)lDTE, 1 /* Call */, (int)lIsAmerican, (int)lDivCount, ref dAmounts  ,  ref dYears , (int)lSteps , dSkew,  dKurtosis , (int)lModel, refResults);			
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
						//dPIV = oOCWrapper.OCWCalcFutureOptionVolatilityMM( dDomesticRate, dSpotPrice, dPOptionPrice, dStrikePrice , (int)lDTE, 0 /* Put */, (int)lIsAmerican,  (int)lSteps , dSkew,  dKurtosis ,(int)lDivCount, ref dAmounts  ,  ref dYears , (int)lModel, out iGotCount);
						dPIV = oOCWrapper.OCWCalcFutureOptionVolatility( dDomesticRate, dSpotPrice, dPOptionPrice, dStrikePrice , (int)lDTE, 0 /* Put */, (int)lIsAmerican,  (int)lSteps , dSkew,  dKurtosis , (int)lModel, out iGotCount);
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
						dPIV = oOCWrapper.OCWCalcVolatilityMM(dDomesticRate, dForeignRate, dSpotPrice, dPOptionPrice, dStrikePrice , (int)lDTE, 0 /* Put */, (int)lIsAmerican, (int)lDivCount, ref dAmounts  ,  ref dYears , (int)lSteps , dSkew,  dKurtosis , (int)lModel);
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
					// force no divs
					lDivCount = 0;
					dAmounts = new double[lDivCount];
					dYears	= new double[lDivCount];

					//iRet = oOCWrapper.OCWCalcFutureOptionGreeksMM(dDomesticRate, dSpotPrice, dStrikePrice , dVolatility, (int)lDTE, 0 /* Put */, (int)lIsAmerican, (int)lSteps , dSkew,  dKurtosis , (int)lModel, (int)lDivCount, ref dAmounts  ,  ref dYears ,  refResults);			
					iRet = oOCWrapper.OCWCalcFutureOptionGreeks(dDomesticRate, dSpotPrice, dStrikePrice , dVolatility, (int)lDTE, 0 /* Put */, (int)lIsAmerican, (int)lSteps , dSkew,  dKurtosis , (int)lModel,  refResults);			
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
					iRet = oOCWrapper.OCWCalcGreeksMM(dDomesticRate, dForeignRate, dSpotPrice, dStrikePrice , dVolatility, (int)lDTE, 0 /* Put */, (int)lIsAmerican, (int)lDivCount, ref dAmounts  ,  ref dYears , (int)lSteps , dSkew,  dKurtosis , (int)lModel, refResults);
				}
				catch
				{
					iRet = -2;
					sResStatus = sResStatus + "; Put: OptionCalc library call error";
				}
			}

			if (iRet == 1) // Calc OK
			{
				/*
				tbResPPrice.Text =	refResults.dTheoPrice.ToString("N", nfi);
				tbResPAlpha.Text = 	refResults.dAlpha.ToString("N", nfi);
				tbResPDelta.Text = 	refResults.dDelta.ToString("N", nfi);
				tbResPGamma.Text = 	refResults.dGamma.ToString("N", nfi);
				tbResPVega.Text = 	refResults.dVega.ToString("N", nfi);
				tbResPTheta.Text = 	refResults.dTheta.ToString("N", nfi);
				tbResPRho.Text = 	refResults.dRho.ToString("N", nfi);
				tbResPDeltaVega.Text = 	refResults.dDeltaVega.ToString("N", nfi);
				tbResPDeltaTheta.Text = 	refResults.dDeltaTheta.ToString("N", nfi);
				tbResPGammaVega.Text = 	refResults.dGammaVega.ToString("N", nfi);
				tbResPGammaTheta.Text = 	refResults.dGammaTheta.ToString("N", nfi);
				*/
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
	
			sbStatus.Text = sResStatus;
		}



		private void cbStyle_Leave(object sender, System.EventArgs e)
		{
			if (cbStyle.SelectedIndex == -1)
				cbStyle.SelectedIndex = 0;
		}
		
		private int TryToGetOption()
		{
			// It'll try to get option prices some day... I hope...
			/*
			DateTime dtExp = dtpExpirationDate.Value;
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
			int iDte;
			try
			{
				iDte = System.Convert.ToInt32(tbDTE.Text);
			}
			catch
			{
				sbStatus.Text = "Wrong DTE";
				iDte = 0;
			}
			
			if (iDte < 0)
			{
				tbDTE.Text = "0";
				iDte = 0;
				dtpExpirationDate.Value = DateTime.Now;
			}
			else
			{
				dtpExpirationDate.Value = DateTime.Now;
				dtpExpirationDate.Value = dtpExpirationDate.Value.AddDays(iDte);
			}
			
			bPreventCheckDate = false;
			TryToGetOption();

		}
		
		private int CheckDate()
		{
			if (bPreventCheckDate)
				return -1;
			if (dtpExpirationDate.Value.Date  < DateTime.Now.Date  )
			{
				dtpExpirationDate.Value = DateTime.Now;
				tbDTE.Text = "0";
				tbDomesticRate.Text = "0.00";
				return 0;
			}
			
			if (dtpExpirationDate.Value.Date  == DateTime.Now.Date  )
			{
				tbDTE.Text = "0";
				tbDomesticRate.Text = "0.00";
				return 0;
			}

			TimeSpan dtTmp = dtpExpirationDate.Value.Subtract(DateTime.Now);
			tbDTE.Text = Convert.ToString((int)dtTmp.Days + 1);
			TryGetRates();

			return 0;
		}

		private void dtpExpirationDate_ValueChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
			CheckDate();
		}

		private void rbContinous_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);

			gbDividends.Enabled = false;

			lbContinousRate.Enabled = true;
			tbContinousRate.Enabled = true;
		
		}

		private void rbDiscreteDividends_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);

			gbDividends.Enabled = true;

			lbContinousRate.Enabled = false;
			tbContinousRate.Enabled = false;

		}

		private void rPeriodic_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);

			gbCustomDivs.Enabled = false;
			gbPeriodicDivs.Enabled = true;

		}

		private void rCustomDivs_CheckedChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);

			gbCustomDivs.Enabled = true;
			gbPeriodicDivs.Enabled = false;
		
		}

		private void tbDTE_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			if(e.KeyChar == (char)13)
			{
				bPreventCheckDate = true;
				e.Handled=true;
				int iDte;
				try
				{
					iDte = System.Convert.ToInt32(tbDTE.Text);
				}
				catch
				{
					sbStatus.Text = "Wrong DTE";
					iDte = 0;
				}
				if (iDte <= 0)
				{
					tbDTE.Text = "0";
					iDte = 0;
					dtpExpirationDate.Value = DateTime.Now;
				}
				else
				{
					dtpExpirationDate.Value = DateTime.Now;
					dtpExpirationDate.Value = dtpExpirationDate.Value.AddDays(iDte);
				}
	
				FeedRequestLastQuotes(tbSymbol.Text);
				bPreventCheckDate = false;


			}
            

		}

		private void btnSetCustomDividends_Click(object sender, System.EventArgs e)
		{
			if (fDivForm.ShowDialog() == DialogResult.OK)
			{
				ClearResults(0);
				listTmpCust.Clear();
				fDivForm.GetCustomDividendList(ref listTmpCust);
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
			}
		}


		private long SaveCustomDivs()
		{
			long lRet = 0;
			long lRowsAffected = 0;
			//DateTime dtDate;
			double dCurDivAmount = 0;
			try
			{

				OleDbDataReader oDBReader;

				ReadCustCommand.CommandText = "delete from CustomDividend where StockId =" + Convert.ToString(m_iStockID);
				lRowsAffected = ReadCustCommand.ExecuteNonQuery();
				if (listTmpCust.Count > 0 )
				{
					Record tmpRecord;
					DateTime dtTmp;
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
			long lContractType;
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
							cbStyle.SelectedIndex  = Convert.ToInt32(CLParams[3]);
						}
						catch
						{
							cbStyle.SelectedIndex  = 0;
						}
						
					}
					
					
					if (CLParams.Length >=5)
					{
						tbStrikePrice.Text  = CLParams[4];
						lRet = 2;
					}

					if (CLParams.Length >=7)
					{
						dBid = Convert.ToDouble(CLParams[5]);
						dAsk = Convert.ToDouble(CLParams[6]);
					}


					if (CLParams.Length >=8)
					{
						try
						{
							dtpExpirationDate.Value = DateTime.Parse(CLParams[7]);
						}
						catch
						{
							dtpExpirationDate.Value = DateTime.Today;
						}
				
					}
					
					lContractType = 0;
					if (CLParams.Length >= 9)
						lContractType = Convert.ToInt32(CLParams[8]);

					if (lContractType == 0)
					{
						if (lContractType<=3)
							cbSymbolType.SelectedIndex = 0;
						else
							cbSymbolType.SelectedIndex = 1;
					}
					
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
						if (sTmp.Length > 0)
						{
							try
							{
								dtpDivDate.Value = DateTime.FromOADate(Convert.ToDouble(sTmp));
								
							}
							catch
							{
								dtpDivDate.Value = DateTime.Today;
							}
						}
						

					}
					

					if (CLParams.Length >= 14)
					{
						long lFreqType = Convert.ToInt32(CLParams[13]);
							
						if (lFreqType == 12)
							cbDivFreq.SelectedIndex = 0;

						if (lFreqType == 4)
							cbDivFreq.SelectedIndex = 1;

						if (lFreqType == 2)
							cbDivFreq.SelectedIndex = 2;

						if (lFreqType == 1)
							cbDivFreq.SelectedIndex = 3;

						if (lFreqType == 0)
						{
							cbDivFreq.SelectedIndex = 0;
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
			sbStatus.Text = "Initializing...";
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
				FeedRequestLastQuotes(tbSymbol.Text);
				//TryLoadSymbolVolatility();
			}
			
		}


		private double GetShortRate(DateTime dtToday, DateTime dtDate)
		{
			double dRet = 0;
			
			if (oIRCurve.iPointsCount > 0)
			{
				TimeSpan tsTmp = dtDate.Subtract(dtToday);
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drShortRateValue, ref oIRCurve.irShortRateDTE, (int)tsTmp.Days);
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
				TimeSpan tsTmp = dtDate.Subtract(dtToday);
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drLongRateValue, ref oIRCurve.irLongRateDTE, (int)tsTmp.Days);
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
				TimeSpan tsTmp = dtDate.Subtract(dtToday);
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drNeutralRateValue, ref oIRCurve.irNeutralRateDTE, (int)tsTmp.Days);
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
				TimeSpan tsTmp = dtDate.Subtract(dtToday);
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drHTBRateValue, ref oIRCurve.irHTBRateDTE, (int)tsTmp.Days);
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
				TimeSpan tsTmp = dtDate.Subtract(dtToday);
				dRet = oOCWrapper.OCWInterpolateRates((int)oIRCurve.iPointsCount, ref oIRCurve.drNeutralHTBRateValue, ref oIRCurve.irNeutralHTBRateDTE, (int)tsTmp.Days);
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

			oIRCurve.irHTBRateDTE = new int[iCount];
			oIRCurve.irLongRateDTE = new int[iCount];
			oIRCurve.irShortRateDTE  = new int[iCount];
			oIRCurve.irNeutralHTBRateDTE  = new int[iCount];
			oIRCurve.irNeutralRateDTE =  new int[iCount];
				
			oIRCurve.drHTBRateValue = new double[iCount];
			oIRCurve.drLongRateValue  = new double[iCount];
			oIRCurve.drNeutralHTBRateValue = new double[iCount];
			oIRCurve.drNeutralRateValue  = new double[iCount];
			oIRCurve.drShortRateValue  = new double[iCount];


			ReadRateCurves = ReadCurveCommand.ExecuteReader();
			int iCur = 0;
			int nDTE = 0;
			int iPeriodTypeID = 0;
			int iNum = 0;
			double dShortRate = 0.0;
			double dLongRate = 0.0;
			double dHTBRate  = 0.0;

			while (ReadRateCurves.Read())
			{
				iNum = Convert.ToInt32(ReadRateCurves["iNum"].ToString());
				iPeriodTypeID = Convert.ToInt32(ReadRateCurves["iPeriodTypeID"].ToString());
				nDTE = GetDTE(iNum, iPeriodTypeID);

				oIRCurve.irHTBRateDTE[iCur] = nDTE;
				oIRCurve.irLongRateDTE[iCur] = nDTE;
				oIRCurve.irShortRateDTE[iCur] = nDTE;
				oIRCurve.irNeutralHTBRateDTE[iCur] = nDTE;
				oIRCurve.irNeutralRateDTE[iCur] = nDTE;
				
				dShortRate = Convert.ToDouble(ReadRateCurves["fShortRate"].ToString());
				dLongRate = Convert.ToDouble(ReadRateCurves["fLongRate"].ToString());
				dHTBRate = Convert.ToDouble(ReadRateCurves["fHTBRate"].ToString());
				
				oIRCurve.drLongRateValue[iCur] = dLongRate;
				oIRCurve.drShortRateValue[iCur] = dShortRate;
				oIRCurve.drHTBRateValue[iCur] = dHTBRate;

				oIRCurve.drNeutralRateValue[iCur] = (dShortRate + dLongRate) /2.0;
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

			try
			{
				if (bUseMidRate)
				{
					if (bHTB)
						dRet = GetNeutralHTBRate(DateTime.Today , dtpExpirationDate.Value );
					else
						dRet = GetNeutralRate(DateTime.Today , dtpExpirationDate.Value );
						
				
				}
				else
				{
					if (bHTB)
						dRet = GetNeutralHTBRate(DateTime.Today , dtpExpirationDate.Value );
					else
						dRet = GetNeutralRate(DateTime.Today , dtpExpirationDate.Value );
						

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
			if (sOldSymbol.ToUpper() != tbSymbol.Text.ToUpper())
			{
				tbContinousRate.Text = "0.00";
				tbDivAmount.Text  = "0.00";
				dtpDivDate.Value = DateTime.Today;
				cbDivFreq.SelectedIndex = 0;

				tbStrikePrice.Text = "0.00";
				dtpExpirationDate.Value = DateTime.Today;
				tbDTE.Text = "0";
				tbVolatility.Text = "0.00";
				tbDomesticRate.Text  = "0.00";
				tbCOptionPrice.Text = "0.00";
				tbPOptionPrice.Text = "0.00";
				
				
				TryToGetDivs(tbSymbol.Text, bManual);
				TryGetRates();
				FeedRequestLastQuotes(tbSymbol.Text);
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
			try
			{
				if (bGotFeed)
				{
					oPriceProvider.CancelLastQuote(null);
					oPriceProvider.UnSubscribeQuote(null);
					oPriceProvider.Disconnect();
					m_PriceProvider.Disconnect();
				
				}

				if (bGotDB)
				{
					oleDbConnection.Close();
				}
	
			
			}
			catch
			{
			
			}
		
		}

		private void tbSymbol_TextChanged(object sender, System.EventArgs e)
		{
			ClearResults(0);
		}

		private void FeedTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			FeedTimer.Enabled = false;
			sbStatus.Text = "No feed prices";
			oPriceProvider.CancelLastQuote(null);
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
				lbContinousRate.Enabled = true;
				tbContinousRate.Enabled = true;
				gbDividends.Enabled = false;
			}
			else
			{
				gbDividends.Enabled = true;
			}
			
			return 0;
		}
	
		private int SetFuture()
		{
			rbContinous.Enabled = false;
			rbDiscreteDividends.Enabled  = false;
			
			rbContinous.Checked = false;
			rbDiscreteDividends.Checked = true;

			lbContinousRate.Enabled = false;
			tbContinousRate.Enabled = false;


			lbSpotOrFuturePrice.Text = "Future price:";
			
			gbDividends.Enabled = false;

			return 0;
		}

		private void InitTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			InitTimer.Enabled = false;
			if (bFirstElapse)
			{
				bFirstElapse = false;
			}
			else
			{
				return;
			}
			
			m_CustDivs = new EtsGeneralLib.EtsDivCollClass();

			nfi = new CultureInfo( "en-US", false ).NumberFormat;
			nfi.NumberDecimalDigits = 4;
			nfi2 = new CultureInfo( "en-US", false ).NumberFormat;
			nfi2.NumberDecimalDigits = 3;
			m_bManual = false;
			oOCWrapper = new OCWrapper.OCWrapperClass();

			mnuQuotes.Enabled = false;
			mnuVolatility.Enabled = false;
			long lParams = ParseCalcCommandLine();
			if (GetInfoSources() > 0)
			{
				if (tbSymbol.Text.Length > 0)
					TryToGetDivs(tbSymbol.Text, false); //?!
				LoadInterestRates();
				
				
				if (lParams > 0)
				{

					double dTmpRate = 0;
					if (tbSymbol.Text.Length > 0)
					{
						FeedRequestLastQuotes(tbSymbol.Text);
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
							TryLoadSymbolVolatility();
					}
				}
				if ((bGotDB)&&(bGotFeed))
					mnuQuotes.Enabled = true;

				if ((bGotDB)&&(bGotVola))
					mnuVolatility.Enabled = true;
			}
			
			listTmpCust = new ArrayList();
			EnableFormControls();
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
				if (iProvider >= 0)
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
				
				
				m_PriceProvider = new PRICEPROVIDERSLib.PriceInfo();
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


			sbStatus.Text = sResults;
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
			FeedRequestLastQuotes(tbSymbol.Text);
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
			mnuFile.Enabled = false;
			mnuData.Enabled = false;
			btnCalc.Enabled = false;
			btnClose.Enabled = false;
		}
		private long TryToGetCustomDivs(int iStockID)
		{
			long lRet = 0;
			listTmpCust.Clear();
			OleDbDataReader oDBCustReader;
			DateTime dtDate;
			double dCurDivAmount = 0;
			try
			{

				OleDbDataReader oDBReader;
				ReadCustCommand.CommandText = "select * from CustomDividend where StockID = " + Convert.ToString(iStockID);
				oDBReader = ReadCustCommand.ExecuteReader();
				while (oDBReader.Read())
				{
					dCurDivAmount	 = Convert.ToDouble(oDBReader["DivAmnt"].ToString());
					dtDate = DateTime.Parse(oDBReader["DivYtes"].ToString());
					m_CustDivs.Add(dtDate, dCurDivAmount);
					
					listTmpCust.Add(new Record(dtDate.ToShortDateString(),Convert.ToString(dCurDivAmount)));
					

				}
				oDBReader.Close();
				
				lRet = fDivForm.SetCustomDividendList(ref listTmpCust);


				/*
				GetCustomDivs.Parameters["@iStockID"].Value = iStockID;
				oDBCustReader = GetCustomDivs.ExecuteReader();

				while (oDBCustReader.Read())
				{
					dCurDivAmount	 = Convert.ToDouble(oDBCustReader["DivAmnt"].ToString());
					DateTime dtDate = DateTime.FromOADate(Convert.ToDouble(oDBCustReader["DivYtes"].ToString()));
					m_CustDivs.Add(dtDate, dCurDivAmount);
				}
				oDBCustReader.Close();
				*/
			}
			catch
			{
				lRet = -1;
			}

			return lRet;
		}



		private void EnableFormControls()
		{
			mnuFile.Enabled = true;
			mnuData.Enabled = true;
			btnCalc.Enabled = true;
			btnClose.Enabled = true;
		}

		private void tbStrikePrice_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			if (e.KeyChar == (char)13)
			{
				e.Handled = true;
				FeedRequestLastQuotes(tbSymbol.Text);
				TryLoadSymbolVolatility();
			}
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