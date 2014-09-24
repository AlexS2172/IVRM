using System;
using System.Runtime.InteropServices;
using System.Collections;
using System.ComponentModel;
using System.Data;

/*
 *
 *	A C# Wrapper to some functions at OptionCalc.dll
 * 
 * */

namespace OCWrapper
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>


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

		//GT_ALL			= (int)0xFFFFFFFF
	};

	public enum FREQUENCY:int
	{
		FREQUENCY_MONTHLY	  = 12,
		FREQUENCY_QUATERLY	  = 4,
		FREQUENCY_SEMIANNUALY = 2,
		FREQUENCY_ANNUALY	  = 1
	};

	/*
	struct DIVIDEND
	{
		double dTimeFraction;	// Dividend years to exp
		double dAmount;			// Dividend amount
	};

	*/
	public struct DIVIDEND
	{
		public double dTimeFraction;
		public double dAmount;
	}

	[StructLayout(LayoutKind.Sequential, Pack = 4)]
	public class GREEKS
	{
	
		public int		nMask;
		public double	dTheoPrice;
		public double	dAlpha;
		public double	dDelta;
		public double	dGamma;
		public double	dVega;
		public double	dTheta;
		public double	dRho;
        public double   dVolga;
		public double	dDeltaVega;
		public double	dDeltaTheta;
		public double	dGammaVega;
		public double	dGammaTheta;
	};





	public class OCWrapperClass
	{
        [System.Runtime.InteropServices.DllImport("OptionCalc.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern
            bool OleDateToUnixDate(double dtDateIn,
            [Out, MarshalAs(UnmanagedType.I8)] out long dtDateOut);

		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", CallingConvention=CallingConvention.Cdecl)]
		private static extern 
			int GetDividendsCount(long dtToday, 
			double dYTE, 
			long dtLastDivDate, 
			int nFrequency );
		

		
		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", CallingConvention=CallingConvention.Cdecl)]
		private static extern 
			int GetDividends2(	long dtToday,
			double dYTE, 
			long dtLastDivDate,
			int nFrequency,
			double dAmount, 
			int nCount, 
			[In, Out, MarshalAs(UnmanagedType.LPArray) ]  double[]  pDivAmnts,
			[In, Out, MarshalAs(UnmanagedType.LPArray) ]  double[]	pDivDays, 
			[Out, MarshalAs(UnmanagedType.I4) ] out int pnCount );
		
		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", SetLastError=true)]
		private static extern
			int IsCall([MarshalAs(UnmanagedType.LPTStr) ] string sInStr );
		

		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", SetLastError=true, CallingConvention=CallingConvention.Cdecl)]
		private static extern double InterpolateRates2(
			int	nCount,
			[MarshalAs(UnmanagedType.LPArray) ]  double[]   pRates,
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pYTEs,
			double	dYte
			);

        [System.Runtime.InteropServices.DllImport("OptionCalc.dll", SetLastError = true, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool GetNYDateTimeAsDATE(
            [Out, MarshalAs(UnmanagedType.R8)] out  double dtDate
            );
		
		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", SetLastError=true, CallingConvention=CallingConvention.Cdecl)]
		private static extern double CalcVolatilityMM3(
			double	dDomesticRate,
			double	dForeignRate,
            double  dHTBRate,
			double	dSpotPrice,
			double	dOptionPrice,
			double	dStrike,		
			double  dYTE,		
			int		nIsCall,	
			int		nIsAmerican,
			int		nCount,
			[MarshalAs(UnmanagedType.LPArray) ]  double[]   pDivAmnts,
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pDivYears,
            int     nSteps,
            double  dSkew,
            double  dKurt,
            int     nModel,
            [Out, MarshalAs(UnmanagedType.I4)] out int pnFlag	
			);

		
		
		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", SetLastError=true, CallingConvention=CallingConvention.Cdecl)]
		private static extern double CalcFutureOptionVolatilityCustDivs(
			double	dDomesticRate,
			double	dFuturePrice,
			double	dOptionPrice,
			double	dStrike,		
			double  dYTE,		
			int		nIsCall,	
			int		nIsAmerican,
			int		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			int		nCount, 	
			[MarshalAs(UnmanagedType.LPArray) ]  double[]   pDivAmnts,
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pDivYears,
			int		nModel,
			[Out, MarshalAs(UnmanagedType.I4) ] out int pnFlag 
			);
		

		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", SetLastError=true, CallingConvention=CallingConvention.Cdecl)]
		private static extern double CalcFutureOptionVolatility(
			double	dDomesticRate,
			double	dFuturePrice,
			double	dOptionPrice,
			double	dStrike,		
			double  dYTE,		
			int		nIsCall,	
			int		nIsAmerican,
			int		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			int		nModel,
			[Out, MarshalAs(UnmanagedType.I4) ] out int pnFlag 
			);


		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", CallingConvention=CallingConvention.Cdecl)]
		private static extern
			int CalcFutureOptionGreeksCustDivs(
			double	dDomesticRate,
			double	dYield,
			double	dFuturePrice,		
			double	dStrike,		
			double	dVolatility, 	
			double  dYTE,		
			int		nIsCall,	
			int		nIsAmerican,
			int		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			int		nModel,	
			int		nCount, 	
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pDivAmnts,
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pDivYears,
			[In, Out, MarshalAs(UnmanagedType.LPStruct)] GREEKS   pGreeks);


		
		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", CallingConvention=CallingConvention.Cdecl)]
		private static extern
			int CalcFutureOptionGreeks3(
			double	dDomesticRate,
			double dYield,
			double	dFuturePrice,
			bool	bSpotGreeks,
			double	dStrike,
			double	dVolatility, 	
			double  dYTE,		
			int		nIsCall,	
			int		nIsAmerican,
			int		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			int		nModel,	
			int		nCount, 	
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pDivAmnts,
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pDivYears,
			[In, Out, MarshalAs(UnmanagedType.LPStruct)] GREEKS   pGreeks);


		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", CallingConvention=CallingConvention.Cdecl)]
		private static extern
			int CalcFutureOptionGreeks(
			double	dDomesticRate,
			double	dFuturePrice,		
			double	dStrike,		
			double	dVolatility, 	
			double  dYTE,		
			int		nIsCall,	
			int		nIsAmerican,
			int		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			int		nModel,	
			[In, Out, MarshalAs(UnmanagedType.LPStruct)] GREEKS   pGreeks);


		
		[System.Runtime.InteropServices.DllImport("OptionCalc.dll", CallingConvention=CallingConvention.Cdecl)]
		private static extern
			int CalcGreeksMM2(
			double	dDomesticRate,
			double	dForeignRate,
            double  dHTBRate,
			double	dSpotPrice,		
			double	dStrike,		
			double	dVolatility, 	
			double  dYTE,		
			int		nIsCall,	
			int		nIsAmerican,
			int		nCount, 	
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pDivAmnts,
			[MarshalAs(UnmanagedType.LPArray)]   double[]   pDivYears,
			int		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			int		nModel,	
			[In, Out, MarshalAs(UnmanagedType.LPStruct)] GREEKS   pGreeks);
	
	
        public DateTime GetNewYorkTime()
        {
            double dNYTime = 0;
            bool bRet = GetNYDateTimeAsDATE(out dNYTime);
            return DateTime.FromOADate(dNYTime);
        }

        public bool OCWOleDateToUnixDate(double dtOleDate, out long dtUnixDate)
        {
            bool bRet = OleDateToUnixDate(dtOleDate, out dtUnixDate);
            return bRet; 
        }


		public int OCWCalcGreeksMM(	double	dDomesticRate,
			double	dForeignRate,
            double  dHTBRate,
			double	dSpotPrice,		
			double	dStrike,		
			double	dVolatility, 	
			double	dYTE,		
			long		nIsCall,	
			long		nIsAmerican,
			long		nCount, 	
			ref double[]   pDivAmnts,
			ref double[]   pDivYears,
			long		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			long		nModel,		
			GREEKS pdGreeks)
		{
			
			int iRet = 0;
			
			iRet = CalcGreeksMM2(	dDomesticRate,
				dForeignRate,
                dHTBRate,
				dSpotPrice,		
				dStrike,		
				dVolatility, 	
				dYTE,		
				(int)nIsCall,	
				(int)nIsAmerican,
				(int)nCount, 	
				pDivAmnts,
				pDivYears,
				(int)nSteps,		
				dSkew,			
				dKurtosis,		
				(int)nModel,		
				pdGreeks );
			
			return iRet; 
		}

		public int OCWGetDividends(
			long dtToday,
			double dYTE, 
			long dtLastDivDate,
			int nFrequency,
			double dAmount, 
			int nCount, 
			ref double[]  pDivAmnts,
			ref double[]	pDivDays, 
			out int pnCount )
		{
			int iRet = 0;
			iRet = GetDividends2(	dtToday,
				dYTE, 
				dtLastDivDate,
				nFrequency,
				dAmount, 
				nCount, 
				pDivAmnts,
				pDivDays, 
				out pnCount );

			return iRet;
		}


		public int OCWGetDividendsCount(
			long dtToday, 
			double dYTE, 
			long dtLastDivDate, 
			int nFrequency )
		{
			int iRet = 0;
			iRet = GetDividendsCount(
				dtToday, 
				dYTE, 
				dtLastDivDate, 
				nFrequency );

			return iRet;
		}


		public double OCWInterpolateRates(
			int	nCount,
			ref  double[]   pRates,
			ref  double[]   pYTEs,
			double		dYte)
		{
			double dRet = 0;
			dRet = InterpolateRates2(
				nCount,
				pRates,
				pYTEs,
				dYte);
			return dRet;
		}		


		public double OCWCalcVolatilityMM3(
			double	dDomesticRate,
			double	dForeignRate,
            double  dHTBRate,
			double	dSpotPrice,
			double	dOptionPrice,
			double	dStrike,		
			double	dYTE,		
			long		nIsCall,	
			long		nIsAmerican,
			long		nCount, 	
			ref  double[]   pDivAmnts,
			ref  double[]   pDivYears,
			long	nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			long	nModel	
			)
		{
			double  dRet = 0;
            int     pnFlag = 0; 
			dRet = CalcVolatilityMM3(
				dDomesticRate,
				dForeignRate,
                dHTBRate, 
				dSpotPrice,
				dOptionPrice,
				dStrike,		
				dYTE,		
				(int)nIsCall,	
				(int)nIsAmerican,
				(int)nCount, 	
				pDivAmnts,
				pDivYears,
				(int)nSteps,
		        dSkew,
                dKurtosis,
				(int)nModel,
	            out pnFlag
				);		
			return dRet;
		}		

		public int OCWCalcFutureOptionGreeksMM(	
			double	dDomesticRate,
			double  dYield,
			double	dFuturePrice,		
			double	dStrike,		
			double	dVolatility, 	
			double	dYTE,		
			long		nIsCall,	
			long		nIsAmerican,
			long		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			long		nModel,		
			long		nCount, 	
			ref double[]   pDivAmnts,
			ref double[]   pDivYears,
			GREEKS pdGreeks)
		{
			
			int iRet = 0;
			
			iRet = CalcFutureOptionGreeksCustDivs(
				dDomesticRate,
				dYield,
				dFuturePrice,		
				dStrike,		
				dVolatility, 	
				dYTE,		
				(int)nIsCall,	
				(int)nIsAmerican,
				(int)nSteps,		
				dSkew,			
				dKurtosis,		
				(int)nModel,		
				(int)nCount, 	
				pDivAmnts,
				pDivYears,
				pdGreeks );
			
			return iRet; 
		}

		public int OCWCalcFutureOptionGreeks3(	
			double	dDomesticRate,
			double  dYield,
			double	dFuturePrice,
			bool	bSpotGreeks,
			double	dStrike,		
			double	dVolatility, 	
			double	dYTE,		
			long		nIsCall,	
			long		nIsAmerican,
			long		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			long		nModel,		
			long		nCount, 	
			ref double[]   pDivAmnts,
			ref double[]   pDivYears,
			GREEKS pdGreeks)
		{
			
			int iRet = 0;
			
			iRet = CalcFutureOptionGreeks3(
				dDomesticRate,
				dYield,
				dFuturePrice,
				bSpotGreeks,
				dStrike,		
				dVolatility, 	
				dYTE,		
				(int)nIsCall,	
				(int)nIsAmerican,
				(int)nSteps,		
				dSkew,			
				dKurtosis,		
				(int)nModel,		
				(int)nCount, 	
				pDivAmnts,
				pDivYears,
				pdGreeks );
			
			return iRet; 
		}

	
		public double OCWCalcFutureOptionVolatilityMM(
			double	dDomesticRate,
			double	dFuturePrice,
			double	dOptionPrice,
			double	dStrike,		
			double	dYTE,		
			long	nIsCall,	
			long	nIsAmerican,
			long	nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			long	nCount, 	
			ref  double[]   pDivAmnts,
			ref  double[]   pDivYears,
			int		nModel,
			out int pnCount
			)
		{
			double dRet = 0;
			dRet = CalcFutureOptionVolatilityCustDivs(
				dDomesticRate,
				dFuturePrice,
				dOptionPrice,
				dStrike,		
				dYTE,		
				(int)nIsCall,	
				(int)nIsAmerican,
				(int)nSteps,		
				dSkew,			
				dKurtosis,	
				(int)nCount, 	
				pDivAmnts,
				pDivYears,
				(int)nModel,
				out  pnCount
				);		
			return dRet;
		}		


		public double OCWCalcFutureOptionVolatility(
			double	dDomesticRate,
			double	dFuturePrice,
			double	dOptionPrice,
			double	dStrike,		
			double	dYTE,		
			long	nIsCall,	
			long	nIsAmerican,
			long	nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			int		nModel,
			out int pnCount
			)
		{
			double dRet = 0;
			dRet = CalcFutureOptionVolatility(
				dDomesticRate,
				dFuturePrice,
				dOptionPrice,
				dStrike,		
				dYTE,		
				(int)nIsCall,	
				(int)nIsAmerican,
				(int)nSteps,		
				dSkew,			
				dKurtosis,	
				(int)nModel,
				out  pnCount
				);		
			return dRet;
		}		


		public int OCWCalcFutureOptionGreeks(	
			double	dDomesticRate,
			double	dFuturePrice,		
			double	dStrike,		
			double	dVolatility, 	
			double	dYTE,		
			long		nIsCall,	
			long		nIsAmerican,
			long		nSteps,		
			double	dSkew,			
			double	dKurtosis,		
			long		nModel,		
			GREEKS pdGreeks)
		{
			
			int iRet = 0;
			
			iRet = CalcFutureOptionGreeks(
				dDomesticRate,
				dFuturePrice,		
				dStrike,		
				dVolatility, 	
				dYTE,		
				(int)nIsCall,	
				(int)nIsAmerican,
				(int)nSteps,		
				dSkew,			
				dKurtosis,		
				(int)nModel,		
				pdGreeks );
			
			return iRet; 
		}

		
		public OCWrapperClass()
		{
		}

	
	}


}

