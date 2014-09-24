#pragma once

#include "CommonSPtr.h"
#include <OptionCalc\OptionCalc.h>
#include "Asset.h"
#include "MarketSimulator.h"
//------------------------------------------------------------------//
class CAbstractContract;

typedef	std::vector<CAbstractContract*>	ContractsVector;
//------------------------------------------------------------------//
class CAbstractContract
{
public:
	CAbstractContract(void);
	~CAbstractContract(void);

private:
	CTickerPtr				m_spTicker;

public:
	LONG					m_nID;

	EtsContractTypeEnum		m_enType; //Stock, Index, Option, Future, ...

	CComBSTR				m_bstrSymbol; //Ticker
	CComBSTR				m_bstrContractName; //Apple Corp
	CComBSTR				m_bstrImportID; //N/A

	DOUBLE					m_dManualPrice;
	DOUBLE					m_dClosePrice;
	DOUBLE					m_dTheoClosePrice;
	
	IContractPtr			m_spBaseContract;
	IAssetPtr				m_spAsset;
	IAssetPtr				m_spPricingAsset;
	IContractCollPtr		m_spDerivativeContracts;

	long					m_nBaseContractID;
	long					m_nAssetID;

	CQuotePtr				m_spQuote;
	CRisksPtr				m_spRisks;

	IContractRootPtr		m_spRoot;
	long					m_nRootID;
	//--------------------------------------------------------------//
	virtual
	double
	getPricingUnit();
	//--------------------------------------------------------------//
	virtual
	double
	getContractSizeInAsset();
	//--------------------------------------------------------------//
	virtual	
	double			
	getPriceMid();
	//--------------------------------------------------------------//
	virtual 
	double
	getPriceBid();
	//--------------------------------------------------------------//
	virtual 
	double
	getPriceAsk();
	//--------------------------------------------------------------//
	virtual 
	CRisksPtr		
	Calculate(CMarketSimulator::ptr MarketSimulator);
	//--------------------------------------------------------------//
	virtual 
	void 
	Clear();
	//--------------------------------------------------------------//
	virtual 
	void 
	FitToMarketVolatility();
	//--------------------------------------------------------------//
	virtual 
	void 
	AssignGREEKS(GREEKS& resultGreeks, CRisksPtr Risks);
	//--------------------------------------------------------------//
	virtual
	double	
	getMaturityWeight(double Period, double CurrentDate);
	//--------------------------------------------------------------//
	virtual
	IEtsPriceProfileAtomPtr
	getPriceProfile();
	//--------------------------------------------------------------//
	long
	get_id();
	//--------------------------------------------------------------//
	void
	get_derivative(ContractsVector& derivative);
	//--------------------------------------------------------------//
	CAsset*
	get_asset();
	//--------------------------------------------------------------//
	CAsset*
	get_pricing_asset();
	//--------------------------------------------------------------//
	CAbstractContract*
	get_base_contract();
	//--------------------------------------------------------------//

public:
	IUndAtomPtr			m_spUnd;
	IEtsFutAtomPtr		m_spFut;
	IEtsFutRootAtomPtr	m_spFutRoot;
	
	virtual EtsContractTypeEnum	GetType();
	virtual void SetType(EtsContractTypeEnum enType);

	virtual LONG GetID();
	virtual void SetID(LONG lID);

	virtual BSTR GetSymbol();
	virtual void SetSymbol(BSTR bstrSymbol);

	virtual BSTR GetContractName();
	virtual void SetContractName(BSTR bstrSymbol);

	virtual DOUBLE	GetManualPrice();
	virtual void	SetManualPrice(DOUBLE dValue);

	virtual DOUBLE	GetClosePrice();
	virtual void	SetClosePrice(DOUBLE dValue);

	virtual DOUBLE	GetTheoClosePrice();
	virtual void	SetTheoClosePrice(DOUBLE dValue);

	virtual IContractPtr	GetBaseContract();
	virtual void			SetBaseContract(IContractPtr spBaseContract);

	virtual IAssetPtr		GetAsset();
	virtual void			SetAsset(IAssetPtr spAsset);

	CTickerPtr	GetTicker();
};
//--------------------------------------------------------------------------------------------------------//
typedef boost::shared_ptr<CAbstractContract>	CAbstractContractPtr;
//--------------------------------------------------------------------------------------------------------//
