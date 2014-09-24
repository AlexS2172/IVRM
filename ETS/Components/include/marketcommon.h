#pragma once

/////////////////////////////////////////////////////////////////////////////
namespace IvRmCore
{
	class CTicker
	{	
	public:
		CTicker() 
		{
		};

		CTicker(const CTicker& ticker)
		{
			m_sSymbol = ticker.m_sSymbol;
			m_sExchange = ticker.m_sExchange;
			m_enType = ticker.m_enType;
		};

		CTicker(const QuoteUpdateParams& aKey)
		{
			char* symbol = _com_util::ConvertBSTRToString(aKey.Symbol);
			m_sSymbol = symbol;
			delete[] symbol;

			char* exchange = _com_util::ConvertBSTRToString(aKey.Exchange);
			m_sExchange = exchange;
			delete [] exchange;

			m_enType = aKey.Type;
		}

		CTicker(const ITicker& aKey)
		{
			char* symbol = _com_util::ConvertBSTRToString(aKey.Symbol);
			m_sSymbol = symbol;
			delete[] symbol;

			char* exchange = _com_util::ConvertBSTRToString(aKey.Exchange);
			m_sExchange = exchange;
			delete [] exchange;

			m_enType = aKey.Type;
		}

		virtual ~CTicker(void)
		{
		}

		void CopyTo(QuoteUpdateParams& aKey) const //ITicker must clear them self
		{
			aKey.Symbol = _com_util::ConvertStringToBSTR(m_sSymbol.c_str());
			aKey.Type = m_enType;
			aKey.Exchange = _com_util::ConvertStringToBSTR(m_sExchange.c_str());
		}

		void CopyTo(ITicker& aKey) const //ITicker must clear them self	 
		{
			aKey.Symbol = _com_util::ConvertStringToBSTR(m_sSymbol.c_str());
			aKey.Type = m_enType;
			aKey.Exchange = _com_util::ConvertStringToBSTR(m_sExchange.c_str());
		}

		bool operator < (const CTicker& aKey) const
		{
			return m_enType < aKey.m_enType
				|| m_enType == aKey.m_enType && m_sSymbol < aKey.m_sSymbol
				|| m_enType == aKey.m_enType && m_sSymbol == aKey.m_sSymbol && m_sExchange < aKey.m_sExchange;
		}

		bool operator == (const CTicker& aKey) const
		{
			return	m_enType == aKey.m_enType &&
					m_sSymbol == aKey.m_sSymbol && 
					m_sExchange == aKey.m_sExchange;
		}	

		CTicker& operator= (const CTicker& rvalue)
		{
			m_sSymbol = rvalue.m_sSymbol;
			m_sExchange = rvalue.m_sExchange;
			m_enType = rvalue.m_enType;
			return *this;
		}

	public:
		std::string m_sSymbol;
		InstrumentTypeEnum m_enType;
		std::string m_sExchange;
	};
	typedef boost::shared_ptr<CTicker>	CTickerPtr;
	//------------------------------------------------------------------------
	class CQuote
	{
	public:
		DATE UpdateDateTime;
		DATE BidDateTime;
		DATE AskDateTime;
		DATE LastDateTime;
		long SeqNum;
		double BidPrice;
		double AskPrice;
		long BidSize;
		long AskSize;
		long LotSize;
		double LastPrice;
		double OpenInterest;
		long Volume;
		double OpenPrice;
		double ClosePrice;
		double HighPrice;
		double LowPrice;
		long Skipped;
		long TotalRequests;
		long WaitTime;
		double NetChange;
		enum QuoteStatusType Status;
		long PriceTick;
		
		std::string Currency;
		std::string Exchange;
		std::string BidExchange;
		std::string AskExchange;

	public:
		CQuote(const QuoteUpdateInfo& refQuote)
		{
			Copy(refQuote);
		};

		CQuote()
		{
			Clear();
		};

		bool CopyTo(CQuote& refQuote) const
		{
			refQuote.UpdateDateTime	=	UpdateDateTime;
			refQuote.BidDateTime	=	BidDateTime;
			refQuote.AskDateTime	=	AskDateTime;
			refQuote.LastDateTime	=	LastDateTime;
			refQuote.SeqNum			=	SeqNum;
			refQuote.BidPrice		=	BidPrice;
			refQuote.AskPrice		=	AskPrice;
			refQuote.BidSize		=	BidSize;
			refQuote.AskSize		=	AskSize;
			refQuote.LotSize		=	LotSize;
			refQuote.LastPrice		=	LastPrice;
			refQuote.OpenInterest	=	OpenInterest;
			refQuote.Volume			=	Volume;
			refQuote.OpenPrice		=	OpenPrice;
			refQuote.HighPrice		=	HighPrice;
			refQuote.LowPrice		=	LowPrice;
			refQuote.Skipped		=	Skipped;
			refQuote.TotalRequests	=	TotalRequests;
			refQuote.WaitTime		=	WaitTime;
			refQuote.NetChange		=	NetChange;
			refQuote.Status			=	Status;
			refQuote.PriceTick		=	PriceTick;

			refQuote.Exchange		=	Exchange;
			refQuote.Currency		=	Currency;
			refQuote.BidExchange	=	BidExchange;
			refQuote.AskExchange	=	AskExchange;

			return true;
		};

		bool CopyTo(QuoteUpdateInfo& refQuote) const
		{

			refQuote.UpdateDateTime	=	UpdateDateTime;
			refQuote.BidDateTime	=	BidDateTime;
			refQuote.AskDateTime	=	AskDateTime;
			refQuote.LastDateTime	=	LastDateTime;
			refQuote.SeqNum			=	SeqNum;
			refQuote.BidPrice		=	BidPrice;
			refQuote.AskPrice		=	AskPrice;
			refQuote.BidSize		=	BidSize;
			refQuote.AskSize		=	AskSize;
			refQuote.LotSize		=	LotSize;
			refQuote.LastPrice		=	LastPrice;
			refQuote.OpenInterest	=	OpenInterest;
			refQuote.Volume			=	Volume;
			refQuote.OpenPrice		=	OpenPrice;
			refQuote.HighPrice		=	HighPrice;
			refQuote.LowPrice		=	LowPrice;
			refQuote.Skipped		=	Skipped;
			refQuote.TotalRequests	=	TotalRequests;
			refQuote.WaitTime		=	WaitTime;
			refQuote.NetChange		=	NetChange;
			refQuote.Status			=	Status;
			refQuote.PriceTick		=	PriceTick;

			refQuote.Currency		=	_com_util::ConvertStringToBSTR(Currency.c_str());
			refQuote.Exchange		=	_com_util::ConvertStringToBSTR(Exchange.c_str());
			refQuote.BidExchange	=	_com_util::ConvertStringToBSTR(BidExchange.c_str());
			refQuote.AskExchange	=	_com_util::ConvertStringToBSTR(AskExchange.c_str());

			return true;
		};

		void	Copy(const QuoteUpdateInfo& refQuote)
		{
			UpdateDateTime	=	refQuote.UpdateDateTime;
			BidDateTime		=	refQuote.BidDateTime;
			AskDateTime		=	refQuote.AskDateTime;
			LastDateTime	=	refQuote.LastDateTime;
			SeqNum			=	refQuote.SeqNum;
			BidPrice		=	refQuote.BidPrice;
			AskPrice		=	refQuote.AskPrice;
			BidSize			=	refQuote.BidSize;
			AskSize			=	refQuote.AskSize;
			LotSize			=	refQuote.LotSize;
			LastPrice		=	refQuote.LastPrice;
			OpenInterest	=	refQuote.OpenInterest;
			Volume			=	refQuote.Volume;
			OpenPrice		=	refQuote.OpenPrice;
			HighPrice		=	refQuote.HighPrice;
			LowPrice		=	refQuote.LowPrice;
			Skipped			=	refQuote.Skipped;
			TotalRequests	=	refQuote.TotalRequests;
			WaitTime		=	refQuote.WaitTime;
			NetChange		=	refQuote.NetChange;
			Status			=	refQuote.Status;
			PriceTick		=	refQuote.PriceTick;

			char* p_Currency = _com_util::ConvertBSTRToString(refQuote.Currency);
			Currency = p_Currency;
			delete[] p_Currency;

			char* p_Exchange = _com_util::ConvertBSTRToString(refQuote.Exchange);
			Exchange = p_Exchange;
			delete[] p_Exchange;

			char* p_BidExchange = _com_util::ConvertBSTRToString(refQuote.BidExchange);
			BidExchange = p_BidExchange;
			delete[] p_BidExchange;

			char* p_AskExchange = _com_util::ConvertBSTRToString(refQuote.AskExchange);
			AskExchange = p_AskExchange;
			delete[] p_AskExchange;
		};

		void Clear()
		{
			Currency		= "";
			Exchange		= "";
			BidExchange		= "";
			AskExchange		= "";

			UpdateDateTime	= 0.;
			BidDateTime		= 0.;
			AskDateTime		= 0.;
			LastDateTime	= 0.;
			SeqNum			= BAD_LONG_VALUE;
			BidPrice		= BAD_DOUBLE_VALUE;
			AskPrice		= BAD_DOUBLE_VALUE;
			BidSize			= BAD_LONG_VALUE;
			AskSize			= BAD_LONG_VALUE;
			LotSize			= BAD_LONG_VALUE;
			LastPrice		= BAD_DOUBLE_VALUE;
			OpenInterest	= BAD_DOUBLE_VALUE;
			Volume			= BAD_LONG_VALUE;
			OpenPrice		= BAD_DOUBLE_VALUE;
			ClosePrice		= BAD_DOUBLE_VALUE;
			HighPrice		= BAD_DOUBLE_VALUE;
			LowPrice		= BAD_DOUBLE_VALUE;
			Skipped			= BAD_LONG_VALUE;
			TotalRequests	= BAD_LONG_VALUE;
			WaitTime		= BAD_LONG_VALUE;
			NetChange		= BAD_DOUBLE_VALUE;
			PriceTick		= BAD_LONG_VALUE;
		};

	};
	typedef	boost::shared_ptr<CQuote>	CQuotePtr;
	//---------------------------------------------------------------//
	class CRisks
	{
	public:
		long	nMask;

		//contract analitics
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

		double	dIVola;
		double	dIVolaBid;
		double	dIVolaAsk;
		double	dTheoVola;
		
		double	dTheoPrice;
		double	dTheoPriceBid;
		double	dTheoPriceAsk;

		double	dIntrinsicValue;
		
		//prices adjusted by price profile
		double	dBaseContractPrice;
		double	dBaseContractPriceBid;
		double	dBaseContractPriceAsk;

		double	dContractPrice;
		double	dContractPriceBid;
		double	dContractPriceAsk;

		//market data from quote feeder
		double	dBidPrice;
		DATE	dtBidDate;
		long	lBidSize;

		double	dAskPrice;
		DATE	dtAskDate;
		long	lAskSize;
		
		double	dLastPrice;
		DATE	dtLastDate;

		long	lVolume;
		
		DATE	dtUpdateDate;

		CLock	m_csLock;

	public:
		CRisks():m_csLock()
		{
			Clear();
		};

		CRisks(const IRisks& refRisks):m_csLock()
		{
			Copy(refRisks);
		};

	public:
		void Clear()
		{
			dTheoPrice		= BAD_DOUBLE_VALUE;
			dTheoPriceBid	= BAD_DOUBLE_VALUE;
			dTheoPriceAsk	= BAD_DOUBLE_VALUE;

			dIntrinsicValue = BAD_DOUBLE_VALUE;

			dAlpha			= BAD_DOUBLE_VALUE;
			dDelta			= BAD_DOUBLE_VALUE;
			dGamma			= BAD_DOUBLE_VALUE;
			dVega			= BAD_DOUBLE_VALUE;
			dTheta			= BAD_DOUBLE_VALUE;
			dRho			= BAD_DOUBLE_VALUE;
			dVolga			= BAD_DOUBLE_VALUE;
			dDeltaVega		= BAD_DOUBLE_VALUE;
			dDeltaTheta		= BAD_DOUBLE_VALUE;
			dGammaVega		= BAD_DOUBLE_VALUE;
			dGammaTheta		= BAD_DOUBLE_VALUE;
			dIVola			= BAD_DOUBLE_VALUE;
			dIVolaBid		= BAD_DOUBLE_VALUE;
			dIVolaAsk		= BAD_DOUBLE_VALUE;
			dTheoVola		= BAD_DOUBLE_VALUE;

			dBaseContractPrice		= BAD_DOUBLE_VALUE;
			dBaseContractPriceBid	= BAD_DOUBLE_VALUE;
			dBaseContractPriceAsk	= BAD_DOUBLE_VALUE;

			dContractPrice			= BAD_DOUBLE_VALUE;
			dContractPriceBid		= BAD_DOUBLE_VALUE;
			dContractPriceAsk		= BAD_DOUBLE_VALUE;

			dBidPrice		= BAD_DOUBLE_VALUE;
			dtBidDate		= 0.;
			lBidSize		= BAD_LONG_VALUE;
			dAskPrice		= BAD_DOUBLE_VALUE;
			dtAskDate		= 0.;
			lAskSize		= BAD_LONG_VALUE;
			dLastPrice		= BAD_DOUBLE_VALUE;
			dtLastDate		= 0.;
			lVolume			= BAD_LONG_VALUE;
			dtUpdateDate	= 0.;
		};

	public:
		bool CopyTo(CRisks& refRisks) const
		{
			CAutoTryLock csLock(const_cast<CLock&>(refRisks.m_csLock));

			if (!csLock.IsLocked()) return false;

			refRisks.nMask = nMask;

			refRisks.dTheoPrice = dTheoPrice;
			refRisks.dTheoPriceBid = dTheoPriceBid;
			refRisks.dTheoPriceAsk = dTheoPriceAsk;

			refRisks.dIntrinsicValue = dIntrinsicValue;

			refRisks.dAlpha = dAlpha;
			refRisks.dDelta = dDelta;
			refRisks.dGamma = dGamma;
			refRisks.dVega = dVega;
			refRisks.dTheta = dTheta;
			refRisks.dRho = dRho;
			refRisks.dVolga = dVolga;

			refRisks.dDeltaVega = dDeltaVega;
			refRisks.dDeltaTheta = dDeltaTheta;
			refRisks.dGammaVega = dGammaVega;
			refRisks.dGammaTheta = dGammaTheta;
			
			refRisks.dIVola = dIVola;
			refRisks.dIVolaAsk = dIVolaAsk;
			refRisks.dIVolaBid = dIVolaBid;
			refRisks.dTheoVola = dTheoVola;

			refRisks.dContractPrice = dContractPrice;
			refRisks.dContractPriceBid = dContractPriceBid;
			refRisks.dContractPriceAsk = dContractPriceAsk;

			refRisks.dBaseContractPrice = dBaseContractPrice;
			refRisks.dBaseContractPriceBid = dBaseContractPriceBid;
			refRisks.dBaseContractPriceAsk = dBaseContractPriceAsk;

			refRisks.dBidPrice = dBidPrice;
			refRisks.dtBidDate = dtBidDate;
			refRisks.lBidSize = lBidSize;
			refRisks.dAskPrice = dAskPrice;
			refRisks.dtAskDate = dtAskDate;
			refRisks.lAskSize = lAskSize;
			refRisks.dLastPrice = dLastPrice;
			refRisks.dtLastDate = dtLastDate;
			refRisks.lVolume = lVolume;
			refRisks.dtUpdateDate = dtUpdateDate;

			return true;
		}

		bool CopyTo(IRisks& refRisks) const
		{
			refRisks.TheoPrice = dTheoPrice;
			refRisks.TheoPriceBid = dTheoPriceBid;
			refRisks.TheoPriceAsk = dTheoPriceAsk;

			refRisks.IntrinsicValue = dIntrinsicValue;

			refRisks.Alpha = dAlpha;
			refRisks.Delta = dDelta;
			refRisks.Gamma = dGamma;
			refRisks.Vega = dVega;
			refRisks.Theta = dTheta;
			refRisks.Rho = dRho;
			refRisks.Volga = dVolga;

			refRisks.DeltaVega = dDeltaVega;
			refRisks.DeltaTheta = dDeltaTheta;
			refRisks.GammaVega = dGammaVega;
			refRisks.GammaTheta = dGammaTheta;

			refRisks.IVola = dIVola;
			refRisks.IVolaAsk = dIVolaAsk;
			refRisks.IVolaBid = dIVolaBid;
			refRisks.TheoVola = dTheoVola;

			refRisks.ContractPrice = dContractPrice;
			refRisks.ContractPriceBid = dContractPriceBid;
			refRisks.ContractPriceAsk = dContractPriceAsk;

			refRisks.BaseContractPrice = dBaseContractPrice;
			refRisks.BaseContractPriceAsk = dBaseContractPriceAsk;
			refRisks.BaseContractPriceBid = dBaseContractPriceBid;

			refRisks.BidPrice = dBidPrice;
			refRisks.BidDate = dtBidDate;
			refRisks.BidSize = lBidSize;
			refRisks.AskPrice = dAskPrice;
			refRisks.AskDate = dtAskDate;
			refRisks.AskSize = lAskSize;
			refRisks.LastPrice = dLastPrice;
			refRisks.LastDate = dtLastDate;
			refRisks.Volume = lVolume;
			refRisks.UpdateDate = dtUpdateDate;

			return true;
		}

		void Copy(const IRisks& refRisks)
		{
			dTheoPrice		= refRisks.TheoPrice;
			dTheoPriceBid	= refRisks.TheoPriceBid;
			dTheoPriceAsk	= refRisks.TheoPriceAsk;

			dIntrinsicValue = refRisks.IntrinsicValue;

			dAlpha			= refRisks.Alpha;
			dDelta			= refRisks.Delta;
			dGamma			= refRisks.Gamma;
			dVega			= refRisks.Vega;
			dTheta			= refRisks.Theta;
			dRho			= refRisks.Rho;
			dVolga			= refRisks.Volga;
			dDeltaVega		= refRisks.DeltaVega;
			dDeltaTheta		= refRisks.DeltaTheta;
			dGammaVega		= refRisks.GammaVega;
			dGammaTheta		= refRisks.GammaTheta;
			dIVola			= refRisks.IVola;
			dIVolaBid		= refRisks.IVolaBid;
			dIVolaAsk		= refRisks.IVolaAsk;
			dTheoVola		= refRisks.TheoVola;

			dBaseContractPrice		= refRisks.BaseContractPrice;
			dBaseContractPriceBid	= refRisks.BaseContractPriceBid;
			dBaseContractPriceAsk	= refRisks.BaseContractPriceAsk;

			dContractPrice		= refRisks.ContractPrice;
			dContractPriceBid	= refRisks.ContractPriceBid;
			dContractPriceAsk	= refRisks.ContractPriceAsk;

			dBidPrice		= refRisks.BidPrice;
			dtBidDate		= refRisks.BidDate;
			lBidSize		= refRisks.BidSize;
			dAskPrice		= refRisks.AskPrice;
			dtAskDate		= refRisks.AskDate;
			lAskSize		= refRisks.AskSize;
			dLastPrice		= refRisks.LastPrice;
			dtLastDate		= refRisks.LastDate;
			lVolume			= refRisks.Volume;
			dtUpdateDate	= refRisks.UpdateDate;
		};

		void 
		fill_by(GREEKS& greeks){
			if((greeks.dTheoPrice > BAD_DOUBLE_VALUE)&&(greeks.dTheoPrice < 0))	greeks.dTheoPrice = 0;
			if((greeks.nMask & GT_THEOPRICE) && _finite(greeks.dTheoPrice) && greeks.dTheoPrice >= 0.)
				dTheoPrice = greeks.dTheoPrice;

			dTheoPriceAsk = dTheoPriceBid = dTheoPrice;

			if((greeks.nMask & GT_DELTA) && _finite(greeks.dDelta))	dDelta = greeks.dDelta;
			if((greeks.nMask & GT_GAMMA) && _finite(greeks.dGamma))	dGamma = greeks.dGamma;
			if((greeks.nMask & GT_THETA) && _finite(greeks.dTheta))	dTheta = greeks.dTheta;
			if((greeks.nMask & GT_VEGA) && _finite(greeks.dVega)) dVega = greeks.dVega;
			if((greeks.nMask & GT_RHO) && _finite(greeks.dRho))	dRho = greeks.dRho;
			if((greeks.nMask & GT_DELTA_VEGA) && _finite(greeks.dDeltaVega)) dDeltaVega = greeks.dDeltaVega;
			if((greeks.nMask & GT_GAMMA_VEGA) && _finite(greeks.dGammaVega)) dGammaVega = greeks.dGammaVega;
			if((greeks.nMask & GT_DELTA_THETA) && _finite(greeks.dDeltaTheta)) dDeltaTheta = greeks.dDeltaTheta;
			if((greeks.nMask & GT_GAMMA_THETA) && _finite(greeks.dGammaTheta)) dGammaTheta = greeks.dGammaTheta;
		};

		void
		fill_by(CQuotePtr quote){
			dBidPrice = quote->BidPrice;
			dtBidDate = quote->BidDateTime;
			lBidSize = quote->BidSize;
			dAskPrice = quote->AskPrice;
			dtAskDate = quote->AskDateTime;
			lAskSize = quote->AskSize;
			dLastPrice = quote->LastPrice;
			dtLastDate = quote->LastDateTime;
			lVolume = quote->Volume;
		};
	};	
	//---------------------------------------------------------------//
	//class CCalculationParametrs
	//{
	//private:
	//	DATE	m_dtCalculationDate;

	//public:
	//	void	SetCalulationDate(const DATE& dtDate)
	//	{
	//		m_dtCalculationDate = dtDate;
	//	};

	//	DATE	GetCalculationDate() const
	//	{
	//		return m_dtCalculationDate;
	//	};

	//	static	DATE	GetDate()
	//	{
	//		double	dtRetVal = 0.;
	//		::GetNYDateTimeAsDATE(&dtRetVal);
	//		return dtRetVal;
	//	};
	//};
	//typedef boost::shared_ptr<CCalculationParametrs>	CCalculationParametrsPtr;
	//---------------------------------------------------------------//
	enum GreeksCalculationModeEnum
	{
		enGcmTheoreticalVola = 0,
		enGcmImpliedVola
	};
	//---------------------------------------------------------------//
	class CCalculationSettings
	{
	private:
		double						m_dPriceTolerance;
		long						m_enGreeksCalcModel;
		long						m_enVolaCalcModel;
		long						m_enPriceRoundingRule;
		bool						m_bUseTimePrcision;
		
		//volatility
		bool						m_bUseTheoVala;

		bool						m_bUseMarketVola;
		bool						m_bUseTheoVolaIfNoBid;
		bool						m_bUseTheoVolaIfBadMarketVola;
		
		GreeksCalculationModeEnum	m_enGreeksCalculationMode;

		double						m_Normal;
		long						extrapolation_type_;
		long						interpolation_type_;

	public:
		CCalculationSettings():
		m_Normal(60.),
		m_enGreeksCalcModel(2),		//enCmBinomial;
		m_enVolaCalcModel(2),		//enCmBinomial;
		m_enPriceRoundingRule(0),	//enPrrRoundNone;
		m_dPriceTolerance(0.01),
		m_bUseTimePrcision(true),			
		m_enGreeksCalculationMode(enGcmTheoreticalVola),
		m_bUseTheoVala(true),
		m_bUseMarketVola(false),
		m_bUseTheoVolaIfNoBid(false),
		m_bUseTheoVolaIfBadMarketVola(true),
		interpolation_type_(1),
		extrapolation_type_(1){
		};

		~CCalculationSettings(){
		};

		double	getNormal(){
			return m_Normal;
		};

		void	setNormal(double Normal){
			m_Normal = Normal;
		};

		double	GetPriceTolerance() {return m_dPriceTolerance;}
		long GetPriceRoundingRule() {return m_enPriceRoundingRule;}
		long GetGreeksCalculationModel() {return m_enGreeksCalcModel;}
		long GetVolaCalcModel() {return m_enVolaCalcModel;}
		bool GetUseTimePrecision() {return m_bUseTimePrcision;}
		bool GetUseTheoVolaIfNoBid() {return m_bUseTheoVolaIfNoBid;}
		bool GetUseTheoVolaIfBadMarketVola() {return m_bUseTheoVolaIfBadMarketVola;}
		GreeksCalculationModeEnum GetGreeksCalculationMode() {return m_enGreeksCalculationMode;}

		void SetGreeksCalculationMode(GreeksCalculationModeEnum enMode){
			m_enGreeksCalculationMode = enMode;
		};

		void SetGreeksCalculationModel(long lModel){
			m_enGreeksCalcModel = lModel;
			m_enVolaCalcModel = lModel;
		};

		void SetUseTimePrecision(bool bStatus){
			m_bUseTimePrcision = bStatus;
		};

		void SetUseTheoVolaIfNoBid(bool bStatus){
			m_bUseTheoVolaIfNoBid = bStatus;
		};

		void SetUseTheoVolaIfBadMarketVola(bool bStatus){
			m_bUseTheoVolaIfBadMarketVola = bStatus; 	
		};

		void SetPriceTolerance(double dValue){
			m_dPriceTolerance = dValue;
		};

		void SetPriceRoundingRule(long lValue){
			m_enPriceRoundingRule = lValue;
		};

			
		double 
		getCurrentDate(){
			double	dtRetVal = 0.;
			::GetNYDateTimeAsDATE(&dtRetVal);
			return dtRetVal;
		};

		long 
		extrapolation_type(){
			return this->extrapolation_type_;
		};

		long 
		interpolation_type(){
			return this->interpolation_type_;
		};

		void 
		set_extrapolation_type(long type){
			this->extrapolation_type_ = type;
		};

		void 
		set_interpolation_type(long type){
			this->interpolation_type_ = type;
		};
	};
	typedef boost::shared_ptr<CCalculationSettings>		CCalculationSettingsPtr;
	//----------------------------------------------------------------------//
	class CCalculationSettingsSingleton
	{
	private:
		static	CCalculationSettingsPtr	m_spSettings;
	public:
		static	CCalculationSettingsPtr Instance()
		{
			if (!m_spSettings)
				m_spSettings = CCalculationSettingsPtr(new CCalculationSettings());

			return m_spSettings;
		};
	};
	//---------------------------------------------------------------//
	typedef boost::shared_ptr<CRisks>	CRisksPtr;

	typedef std::map<CTicker, CQuotePtr>	CQuoteTickerMap;
	typedef	std::pair<CTicker, CQuotePtr>			QTPair;
	typedef	boost::shared_ptr<CQuoteTickerMap>			CQuoteTickerMapPtr;

	typedef std::map<CTicker, CRisksPtr>	CRisksTickerMap;
	typedef std::pair<CTicker, CRisksPtr>		RTPair;
	typedef boost::shared_ptr<CRisksTickerMap>		CRisksTickerMapPtr;

	typedef std::vector<CTicker>		RequestVector;
	//-----------------------------------------------------------------------//

	class Price{
	public:
		double bid_price;
		double ask_price;
		double mid_price;

		Price(double bid_price = 0., double ask_price = 0., double mid_price = 0.){
			this->bid_price = bid_price;
			this->ask_price = ask_price;
			this->mid_price = mid_price;
		};

		double mid(){
			return mid_price;
		};

		double ask(){
			return ask_price;
		};

		double bid(){
			return bid_price;
		};
	};

	class Volatility{
	public:
		double implied_vola;
		double theo_vola;

		Volatility(	double implied_vola = BAD_DOUBLE_VALUE, 
					double theo_vola = BAD_DOUBLE_VALUE) 
		{
			this->implied_vola = implied_vola;
			this->theo_vola = theo_vola;
		};

		double implied(){
			return implied_vola;
		};

		double theo(){
			return theo_vola;
		};
	};

	class Dividend{
	public:
		double* ammount_div;
		double* dates_div;
		long count_div;

		Dividend(double* ammount_div = 0, double* dates_div = 0, long count_div = 0){
			this->ammount_div = ammount_div;
			this->dates_div = dates_div;
			this->count_div = count_div;
		};

		double* ammount(){
			return ammount_div;
		};

		double* dates(){
			return dates_div;
		};

		long count(){
			return count_div;
		};
	};

	class CalculationParameters{
	public:
		Price price;
		Volatility volatility;
		Dividend dividend;
		double skew;
		double kurt;
		double yield;
		double pricing_asset_borrowing_rate;
		double asset_borrowing_rate;
		double simulation_date;

		CalculationParameters(){
			this->skew = 0.;
			this->kurt = 0.;
			this->yield = 0.;
			this->pricing_asset_borrowing_rate = 0.;
			this->asset_borrowing_rate = 0.;
			this->simulation_date = 0.;
		};

		virtual 
		void copy_to(CRisksPtr anx){
			anx->dContractPrice = price.mid();
			anx->dContractPriceAsk = price.ask();
			anx->dContractPriceBid = price.bid();

			anx->dIVola = volatility.implied();
			anx->dTheoVola = volatility.theo();
		};
	};

	typedef boost::shared_ptr<CalculationParameters> CalculationParametersPtr;

	class OptionCalculationParameters: public CalculationParameters{
	public:
		double yte;
		Price base_contract_price;
		bool is_american;
		double strike;
		bool is_call;
		double expiration_date;

		OptionCalculationParameters():CalculationParameters(){
			this->yte = 0.;
			this->is_american = true;
			this->is_call = true;
			this->expiration_date = 0.;
		};

		/*virtual*/
		void copy_to(CRisksPtr anx){
			CalculationParameters::copy_to(anx);
			anx->dBaseContractPrice = base_contract_price.mid();
			anx->dBaseContractPriceAsk = base_contract_price.ask();
			anx->dBaseContractPriceBid = base_contract_price.bid();
		};
	};

	class CalculationModel{
	public:
		virtual bool calculate_greeks(	CalculationParametersPtr parameters,
										CCalculationSettingsPtr settings,
										CRisksPtr analytics) = 0;

		virtual bool calculate_volatility(	CalculationParametersPtr parameters, 
											CCalculationSettingsPtr settings,
											CRisksPtr analytics) = 0;
	};

	typedef boost::shared_ptr<CalculationModel> CalculationModelPtr;
};
//--------------------------------------------------------------------//

