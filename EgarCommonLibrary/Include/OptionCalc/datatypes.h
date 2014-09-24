#if !defined(__OPTIONCALC_DATATYPES_H__)
#define __OPTIONCALC_DATATYPES_H__

#define DAYS_IN_YEAR	365.

#define CALL_CODES	_T("ABCDEFGHIJKL")
#define PUT_CODES	_T("MNOPQRSTUVWX")

#define  MM_EGAR_BS			0x001
#define  MM_EGAR_BINOMIAL	0x002
#define  MM_EGAR_OPTIMIZED	0x003
#define  MM_EGAR_VSKLOG		0x004

// Rate definition
#pragma pack( push, 4 )		// This alignment seems to be working fine with VB
struct RATE
{
	double  dRate;
	long	nDTE;
};
#pragma pack( pop )

enum FREQUENCY
{
	FREQUENCY_MONTHLY	  = 12,
	FREQUENCY_QUATERLY	  = 4,
	FREQUENCY_SEMIANNUALY = 2,
	FREQUENCY_ANNUALY	  = 1
};

// Dividend definition
#pragma pack( push, 4 )		// This alignment seems to be working fine with VB

struct REGULAR_DIVIDENDS
{
	long	nLastDivDate;
	long	nFrequency;
	double	dAmount;
};

struct DIVIDEND
{
	double dTimeFraction;	// Dividend years to exp
	double dAmount;			// Dividend amount
};

#pragma pack( pop )

enum GREEK_TYPE
{
	GT_NOTHING	 = 0x00000000,

	GT_THEOPRICE	= 0x00000001,
	GT_ALPHA		= 0x00000002,
	GT_DELTA		= 0x00000004,
	GT_GAMMA		= 0x00000008,
	GT_VEGA			= 0x00000010,
	GT_THETA		= 0x00000020,
	GT_RHO			= 0x00000040,
	GT_VOLGA		= 0x00000080,

	GT_DELTA_VEGA	= 0x00000100,
	GT_DELTA_THETA	= 0x00000200,
	GT_GAMMA_VEGA	= 0x00000400,
	GT_GAMMA_THETA	= 0x00000800,

	GT_ALL			= 0xFFFFFFFF
};


// Greeks 
#pragma pack( push, 4 )		// This alignment seems to be working fine with VB
struct GREEKS
{
	long	nMask;

	double	dTheoPrice;
	double  dAlpha;
	double	dDelta;
	double	dGamma;
	double	dVega;
	double	dTheta;
	double	dRho;
	double	dVolga;

	double	dDeltaVega;
	double	dDeltaTheta;
	double	dGammaVega;
	double	dGammaTheta;

	GREEKS() { memset(this, '\0', sizeof(GREEKS)); }
};
#pragma pack( pop )

// Greeks for variance swap
#pragma pack( push, 4 )		// This alignment seems to be working fine with VB
struct VSGREEKS
{
  long	nMask;

  double	dTheoPrice;
  double  dAlpha;
  double	dDelta;
  double	dGamma;
  double	dVega;
  double	dTheta;
  double	dRho;

  double	dDeltaVega;
  double	dDeltaTheta;
  double	dGammaVega;
  double	dGammaTheta;
  
  double  dVomma;
  double  dVommaUp;
  double  dVommaDown;
  double  dVanna;
  double  dVannaUp;
  double  dVannaDown;
  double	dDeltaUp;
  double	dDeltaDown;
  double	dVegaUp;
  double	dVegaDown;
  VSGREEKS() { memset(this, '\0', sizeof(VSGREEKS)); }
};
#pragma pack( pop )

enum VOLATILITY_FLAG
{
	VF_OK	        = 0x00000000,
	VF_TOOLOW       = 0x00000001,
	VF_TOOHIGH      = 0x00000002,
	VF_UNRELIABLE	= 0x00000003
};

#endif //!defined(__OPTIONCALC_DATATYPES_H__)
