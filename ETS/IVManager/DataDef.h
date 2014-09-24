// DataDef.h: interface for the CDataDef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATADEF_H__871910F8_1DA5_48B7_B2D7_F483D9CAD01C__INCLUDED_)
#define AFX_DATADEF_H__871910F8_1DA5_48B7_B2D7_F483D9CAD01C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define BAD_DOUBLE_VALUE	-1E+308

struct CContractData;

struct CSyntheticOptionRootParam
{
	CContractData*	m_pUnderlying;
	double			m_dWeight;

	CSyntheticOptionRootParam()  { ZeroMemory (this, sizeof (CSyntheticOptionRootParam));}
};

typedef std::map < std::wstring, CSyntheticOptionRootParam > synthunderl_param_map;

struct COptionData
{
	long   m_lOptionRootID;
	DATE m_dtExpiration;
	DATE	m_dtExpiryOV;
	DATE	m_dtTradingClose;
	bool m_bIsCall;
	double m_dStrike;
	double m_dPrice;
	double m_dVola;
	DATE   m_dtPriceDate;
	TCHAR  m_strName[24];
	long   m_lOptionID; // Only filled for Access version
	
	bool   m_bIsChanged;
	double m_dPriceBid;
	double m_dPriceAsk;
	double m_dPriceLast;
	bool   m_bIsFitEnabled;

	COptionData()  { ZeroMemory (this, sizeof (COptionData)); m_bIsCall = true;m_bIsFitEnabled = true; }
	long GetDTE()
	{
		EgLib::vt_date dtCurrent(vt_date::GetCurrentDate());
		
		return static_cast<long>(m_dtExpiration) - static_cast<long>(dtCurrent);
	}

	DOUBLE	GetYTE()
	{
		DOUBLE	dNYDateTime;
		GetNYDateTimeAsDATE(&dNYDateTime);
		return	(static_cast<DOUBLE>(m_dtExpiryOV) - dNYDateTime) / 365.;
	}
};

typedef std::map < std::wstring, COptionData* > root_options_map;

struct COptionRootData
{
	long					m_lOptionRootID;
	long					m_lLotSize;
	bool					m_bIsSynthetic;
	bool                    m_bIsFitEnabled;

	root_options_map		m_Options;	 

	COptionRootData()
	{
		Clear();
	}

	virtual ~COptionRootData()
	{
	}

	virtual void Clear()
	{
		m_lOptionRootID = 0;
		m_lLotSize = 0;
		m_bIsSynthetic = false;
		m_bIsFitEnabled = true;
		m_Options.clear();
	}
};

typedef std::vector< REGULAR_DIVIDENDS >   dividends_vector;

struct CSyntheticOptionRootData : public COptionRootData
{
	double					m_dCashAmount;
	synthunderl_param_map	m_mapSynthRootComponents;
	bool					m_bIsBasket;
	double					m_dYield;
	double					m_dSkew;
	double					m_dKurt;

	IEtsIndexDivCollPtr		m_Divs;
	
	CSyntheticOptionRootData()
	{
		Clear();
	}

	virtual ~CSyntheticOptionRootData()
	{
	}

	virtual void Clear()
	{
		COptionRootData::Clear();
		m_dCashAmount = 0.0f;
		m_mapSynthRootComponents.clear();
		m_bIsBasket = false;
		m_dYield = 0.0f;
		m_dSkew = 0.0f;
		m_dKurt = 0.0f;
		m_Divs=NULL;
	}
};

typedef std::map < long, COptionRootData* > option_roots_map;

struct CContractData
{
	int nID;
	double m_dPrice;
	bool m_bIsIndex;
	bool m_bIsAmerican;
	double m_dYield;
	double m_dSkew;
	double m_dKurt;
	double m_dPriceBid;
	double m_dPriceAsk;
	double m_dPriceLast;
	IEtsIndexDivAtomPtr m_spDividend;
	IEtsIndexDivCollPtr m_Divs;

	
#ifdef _VOLA_MANAGER
	double m_dPosition;

	bool m_bIsHTB;       // Hard to Borrow for VM
#endif

	option_roots_map m_mapOptionRoots;		//storage of option roots
	option_roots_map m_mapRefSynthOptRoots;	// references to synthetic roots

	CContractData () { Clear(); }
	~CContractData()
	{
		Clear();
	}
	void Clear () 
		{  

			nID = 0;
			m_dPrice = 0.0f;
			m_bIsIndex = false;
			m_bIsAmerican = false;
			m_dYield = 0.0f;
			m_dSkew = 0.0f;
			m_dKurt = 0.0f;
			m_dPriceBid = 0.0f;
			m_dPriceAsk = 0.0f;
			m_dPriceLast = 0.0f;
			m_spDividend = NULL;

#ifdef _VOLA_MANAGER
			m_dPosition = -DBL_MAX;
			m_bIsHTB = false;
#endif
			m_Divs = NULL;
			m_mapRefSynthOptRoots.clear(); // just clear map
			// clear map & data in map
			for(option_roots_map::iterator it  = m_mapOptionRoots.begin();it != m_mapOptionRoots.end();it++)
			{
				delete it->second;
			}
			m_mapOptionRoots.clear();
		}
};

typedef std::map<double, const COptionData *> strikes_map; // Don't find anything in these maps (double!). For sorting only

struct CTimeSkewData
{
	COptionData * m_pOption;

	CTimeSkewData() 
		{ Clear(); }
	CTimeSkewData(COptionData * pOption )
	{
		Clear();
		m_pOption = pOption;
	}

	void Clear ()
	{ 
		ZeroMemory (this, sizeof (CTimeSkewData)); 
	}

};

struct CHistRangeData
{
	double	m_dStrike;
	double	m_dHistVolaHigh;
	double	m_dHistVolaLow;
	bool	m_bATM;	
	
	CHistRangeData () { Clear(); }

	void Clear ()
		{ ZeroMemory (this, sizeof (CHistRangeData)); }
};

struct CVolaBandsData
{
	double	m_dStrikeMoneyness;
	double	m_dIVBandHigh;
	double	m_dIVBandLow;
	
	CVolaBandsData () { Clear(); }

	void Clear ()
		{ ZeroMemory (this, sizeof (CVolaBandsData)); }
};

struct CVolaNotification
{
	enum Type
	{
		enMonth,
		enTimeSkew,
		enAll
	};

	Type m_enType;
	DATE m_dtMonth;

	CVolaNotification (Type enType, DATE dtMonth = 0 ):
		m_enType (enType), m_dtMonth (dtMonth)
	{
	}

	bool operator < (const CVolaNotification & key) const
	{
		if(m_enType != key.m_enType)
			return  (m_enType < key.m_enType);
		else 
			return (m_dtMonth < key.m_dtMonth);
	}
};

typedef std::set <CVolaNotification> notif_set;


class CVolaNotificationClient
{
public:
	CVolaNotificationClient() {}

	virtual ~CVolaNotificationClient() {}  


	void VolaChangedNotification ()
	{
		m_csNotifs.Lock();
		
		m_notifs.clear();	
		m_notifs.insert (CVolaNotification::enAll);

		m_csNotifs.Unlock();

	}
		
	void VolaChangedNotification (bool bTSChanged, DATE m_dtMonth )
	{
		m_csNotifs.Lock();
		if (m_notifs.find (CVolaNotification::enAll) == m_notifs.end()   )
		{
			if (bTSChanged)
				m_notifs.insert (CVolaNotification::enTimeSkew);

			m_notifs.insert ( CVolaNotification( CVolaNotification::enMonth, m_dtMonth));
		}

		m_csNotifs.Unlock();
	}
	
	void PopNotications (notif_set & rns)
	{
		m_csNotifs.Lock();
		
		rns.clear();
		m_notifs.swap (rns)  ;

		m_csNotifs.Unlock();
	}

private:
	notif_set m_notifs;
	CCriticalSection m_csNotifs;
};


#ifdef _VOLA_MANAGER

/////////////////////////////////////////////////////////////////////////////
//
struct CString_less
{
	bool operator()( const CString& p1, const CString& p2 ) const
	{
		return (p1.Compare(p2) < 0);
	}
};

struct CString_less_NoCase
{
	bool operator()( const CString& p1, const CString& p2 ) const
	{
		return (p1.CompareNoCase(p2) < 0);
	}
};

/////////////////////////////////////////////////////////////////////////////
//
#define RULE_TYPE_VOLA_PTS				0
#define RULE_TYPE_VOLA_PERC				1
#define RULE_TYPE_VOLA_PERC_PREV		2
#define RULE_TYPE_CURVE_FIT				3
#define RULE_TYPE_DUAL_QUADRATIC		4

#define RULE_TYPE_VOLA_PTS_NAME			_T("Volatility Points")
#define RULE_TYPE_VOLA_PERC_NAME		_T("Volatility %")
#define RULE_TYPE_VOLA_PERC_PREV_NAME	_T("Previous Volatility %")
#define RULE_TYPE_CURVE_FIT_NAME		_T("Curve Fitting")
#define RULE_TYPE_DUAL_QUADRATIC_NAME	_T("Dual Quadratic Model")

/////////////////////////////////////////////////////////////////////////////
//
struct CCustomVolaNotification
{
	enum Type
	{
		enPoint		= 1,
		enATMPoint	= 2,
		enSkew		= 3
	};

	Type m_enType;
	double m_dtMonth;

	CCustomVolaNotification( Type enType, double dtMonth = 0. ):
		m_enType(enType), 
		m_dtMonth(dtMonth)
	{}
};


/////////////////////////////////////////////////////////////////////////////
//
struct CSurfaceData
{
	long	m_nID;
	CString m_strName;
	double  m_dUnderlinePrice;
	double	m_dSmileAccelerator;
	long	m_nInterpolationFactor;
	double	m_dStrikeMoneynessHi;
	double	m_dStrikeMoneynessLo;
	BOOL	m_bIsDefault;
	BOOL    m_bIsPriceOverride;
	BOOL    m_bIsDiscreteAccel;
	BOOL    m_bIsNextDaySurface;

	CSurfaceData()
	{
		Clear();
	}

	void Clear()
	{
		m_nID = 0;
		m_strName.Empty();
		m_dUnderlinePrice	   = 0.0f;
		m_dSmileAccelerator	   = -1.0f;
		m_nInterpolationFactor = -1;
		m_dStrikeMoneynessHi   = -1;
		m_dStrikeMoneynessLo   = -1;
		m_bIsDefault		   = false;
		m_bIsPriceOverride	   = false;
		m_bIsDiscreteAccel     = false;
		m_bIsNextDaySurface	   = false;
	}
};

typedef std::vector<CSurfaceData>   CSurfacesVector;

/////////////////////////////////////////////////////////////////////////////
//
struct CNextDaySurfaceData
{
	long	m_nID;
	CString m_strContract;
	long	m_nSurfaceID;

	CNextDaySurfaceData()
	{
		Clear();
	}

	void Clear()
	{
		m_nID		  = 0;
		m_nSurfaceID  = 0;
		
		m_strContract.Empty();
	}
};

typedef std::vector<CNextDaySurfaceData>   CNextDaySurfacesVector;


/////////////////////////////////////////////////////////////////////////////
//
struct CRuleData
{
	long			m_nID;
	bool			m_bIsNew;
	bool			m_bChanged;
	long			m_nType;
	CString			m_strName;
	long			m_nStrikeIncrement;
	long			m_nRequiredPoints;
	double			m_dSmileAccelerator;
	double			m_dCallShift;
	double			m_dPutShift;
	double			m_dInterpolationFactor;
	double			m_dStrikeMoneynessHi;
	double			m_dStrikeMoneynessLo;
	bool			m_bIsDefault;
	bool			m_bIsGroupRule;
	bool			m_bIsContractRule;

	CRuleData()
	{
		Clear();
	}

	void Clear()
	{
		m_nID = 0L;
		m_bIsNew = false;
		m_bChanged = false;
		m_nType = RULE_TYPE_VOLA_PTS;
		m_strName.Empty();
		m_nStrikeIncrement = 0;
		m_nRequiredPoints = 0;
		m_dSmileAccelerator = 0.;
		m_dCallShift = 0;
		m_dPutShift = 0;
		m_dInterpolationFactor = 0.;
		m_dStrikeMoneynessHi = 0;
		m_dStrikeMoneynessLo = 0;
		m_bIsDefault = false;
		m_bIsGroupRule = false;
		m_bIsContractRule = false;
	}
};

typedef std::vector<CRuleData>	CRulesVector;

/////////////////////////////////////////////////////////////////////////////
//
struct CGroupSymbolData
{
	long		m_nID;
	long		m_nRuleID;
	CString		m_strName;

	CGroupSymbolData()
	{
		Clear();
	}

	void Clear()
	{
		m_nID = 0L;
		m_nRuleID = 0L;
		m_strName.Empty();
	}
};

typedef std::vector<CGroupSymbolData>	CGroupSymbolsVector;

class CExpDate{
public:
	double date_;
public:
	CExpDate(): date_(0.) {}
	CExpDate(const double &r): date_(r) {}
	bool operator==( const CExpDate& r) const {
		return fabs( r.date_ - date_) < (1./(24.*60.));
	}
	bool operator<( const CExpDate& r) const {
		return  !(*this==r) && ( (date_ - r.date_) < 0. );
	}
	CExpDate& operator=(const double& r)
	{
		date_	=	r;
		return *this;
	}
};
/////////////////////////////////////////////////////////////////////////////
//
struct CGroupData
{
	long					m_nID;
	long					m_bIsNew;
	long					m_nRuleID;
	bool					m_bChanged;
	CString					m_strName;
	CGroupSymbolsVector		m_vecSymbols;

	CGroupData()
	{
		Clear();
	}

	~CGroupData()
	{
		m_vecSymbols.clear();
	}

	void Clear()
	{
		m_nID = 0L;
		m_bIsNew = false;
		m_nRuleID = 0L;
		m_bChanged = false;
		m_strName.Empty();
		m_vecSymbols.clear();
	}
};

typedef std::vector<CGroupData>										CGroupsVector;

/////////////////////////////////////////////////////////////////////////////
//
typedef std::map<CString, long, CString_less_NoCase>				CNamesMap;
typedef std::map<long, long>										CIdMap;

#endif	//_VOLA_MANAGER

/////////////////////////////////////////////////////////////////

static inline bool fEQZero (double dVal)
{
	return fabs(dVal) < DBL_EPSILON;		
}

static inline bool fNEQZero (double dVal)
{
	return fabs(dVal) > DBL_EPSILON;		
}

static inline bool fEQ (double dVal1 , double dVal2 )
{
	return fEQZero (dVal1 - dVal2);
}

struct sUndPrice
{
	double m_fBid;
	double m_fAsk;
	double m_fLast;
	int    m_iID;

	void Clear()
	{
		ZeroMemory(this, sizeof(sUndPrice));
	}
	bool operator==(const sUndPrice& data)
	{
		return (data.m_iID == m_iID && data.m_fAsk == m_fAsk && data.m_fBid == m_fBid && data.m_fLast == m_fLast);		
	}

	bool operator!=(const sUndPrice& data)
	{
		return !operator==(data);
	}
	sUndPrice& operator=(const sUndPrice& data)
	{
		m_iID = data.m_iID;
		m_fAsk = data.m_fAsk;
		m_fBid = data.m_fBid;
		m_fLast = data.m_fLast;
		return *this;
	}

	sUndPrice()
	{
		Clear();
	}

	double GetMidMarket() const
	{
		double dMidMarket =0;
		
		if ( fEQZero(m_fAsk) )
			dMidMarket = m_fBid;
		else
		if ( fEQZero(m_fBid) )	
			dMidMarket = m_fAsk;
		else
			dMidMarket = (m_fBid + m_fAsk) / 2.0;

		if ( fEQZero(dMidMarket) )
			dMidMarket = m_fLast;
		
		return dMidMarket;
	}
};

struct CIRCurveData
{
	enum IrRuleType
	{
		enAlwaysUseNeutralRate = 0,
		enRateBasedOnPosition = 1
	};

	long	m_lID;
	IrRuleType	m_RuleID;
	long	m_lCurveType;
	double	m_dbPosThreshold;

	CIRCurveData()  { ZeroMemory (this, sizeof (CIRCurveData)); }
};

/////////////////////////////////////////////////////////////////////////////
//
struct CDualQuadParam
{
	double  m_dATMVola;
	double	m_dCoeffA1;
	double	m_dCoeffB1;
	double	m_dCoeffA2;
	double	m_dCoeffB2;

	CDualQuadParam()
	{
		Clear();
	}

	void Clear()
	{
		m_dATMVola	    = 0.0f;
		m_dCoeffA1	    = 1.0f;
		m_dCoeffB1		= 1.0f;
		m_dCoeffA2	    = -1.0f;
		m_dCoeffB2		= 1.0f;
	}
};

typedef std::vector<CDualQuadParam>   CDualQuadParVector;

//struct CStockDividends
//{
//	int nID;
//	DATE m_dtLastDivDate;
//    long m_lDivFrequency;
//    double m_dLastDivAmount;
//	CStockDividends()
//	{
//		Clear();
//	}
//	void Clear()
//	{
//		ZeroMemory(this,sizeof(CStockDividends));
//	}
//};
//
//typedef std::map< long,CStockDividends >   stock_dividends_map;


//////////////////////////////////////////////////////////////////

typedef std::map <CExpDate, CTimeSkewData > time_skew_map;
typedef std::multimap <long, CHistRangeData > hist_ranges_map;
typedef std::multimap <long, CVolaBandsData > vola_bands_map;

typedef std::map < std::wstring, COptionData > options_map;
typedef std::vector < RATE > rates_vec;
typedef std::vector <int> dte_vec;

typedef std::map < std::wstring, CContractData > synth_underl_map;
typedef std::map < long, sUndPrice > underl_price_map;


#endif // !defined(AFX_DATADEF_H__871910F8_1DA5_48B7_B2D7_F483D9CAD01C__INCLUDED_)
