#pragma once
#include "abstractcontract.h"


class COptionContract: public CAbstractContract
{
public:
	COptionContract(void);
	~COptionContract(void);

public:
	EtsOptionTypeEnum	m_enOptType;
	DATE				m_dtExpiry;
	DOUBLE				m_dStrike;
	CComBSTR			m_bstrSeries;
	LONG				m_IsFlex;

	DATE				m_dtExpiryOV;
	DATE				m_dtTradingClose;
	long				is_american;
	//-------------------------------------------------------//
	/*virtual*/
	double
	getMaturityWeight(double Period, double CurrentDate);
	//-------------------------------------------------------//
	virtual
	double
	GetTheoVolatility();
	//-------------------------------------------------------//
	/*virtual*/
	void 
	FitToMarketVolatility();
	//-------------------------------------------------------//
	/*virtual*/	
	double			
	getPriceMid();
	//-------------------------------------------------------//
	/*virtual*/ 
	double
	getPriceBid();
	//-------------------------------------------------------//
	/*virtual*/ 
	double
	getPriceAsk();
	//-------------------------------------------------------//
	/*virtual*/
	double
	getContractSizeInAsset();
	//-------------------------------------------------------//
public:
	void				Clear();
};

class SpotOptionModel: public CalculationModel{
public:
	/*virtual*/
	bool 
		calculate_greeks(	CalculationParametersPtr parameters, 
							CCalculationSettingsPtr settings, 
							CRisksPtr analytics);

	/*virtual*/
	bool 
		calculate_volatility(	CalculationParametersPtr parameters, 
								CCalculationSettingsPtr settings, 
								CRisksPtr analytics);
};

class FutureOptionModel: public CalculationModel{
public:
	/*virtual*/
	bool 
		calculate_greeks(	CalculationParametersPtr parameters, 
							CCalculationSettingsPtr settings, 
							CRisksPtr analytics);

	/*virtual*/
	bool 
		calculate_volatility(	CalculationParametersPtr parameters, 
								CCalculationSettingsPtr settings, 
								CRisksPtr analytics);
};