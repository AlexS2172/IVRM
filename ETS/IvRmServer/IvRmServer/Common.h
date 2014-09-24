#pragma once
//-------------------------------------------------------------------------------------------------------------//
class CTrade
{
public:
	void	Copy(struct ITradeUpdate* Data)
	{
		m_dtTradeDate	= Data->GetTradeDate();
		m_nSeqNum		= Data->GetSeqNum();
		m_nTradeID		= Data->GetTradeID();		
		m_dPrice		= Data->GetPrice();	
		m_nQuantity		= Data->GetQuantity();	
		m_bIsBuy		= (Data->GetIsBuy() == VARIANT_TRUE);			
		m_dSpotReference= Data->GetSpotReference();
		m_dTradedIV		= Data->GetTradedIV();
		m_bIsPosition	= (Data->GetIsPosition() == VARIANT_TRUE);
		m_enTradeStatus = static_cast<EtsTradeStatusEnum>(Data->GetTradeStatus());
		m_dBrokerCommission = Data->GetBrokerCommission();
		m_dClearingBrokerCommission = Data->GetClearingBrokerCommission();
		m_nMark			= Data->GetMark();
		m_nContractID	= Data->GetSymbolID();
	};

public:
	DATE	m_dtTradeDate;
	long	m_nSeqNum;
	long	m_nTradeID;
	double	m_dPrice;
	long	m_nQuantity;
	bool	m_bIsBuy;
	double	m_dSpotReference;
	double	m_dTradedIV;
	bool	m_bIsPosition;
	EtsTradeStatusEnum m_enTradeStatus;
	double	m_dBrokerCommission;
	double	m_dClearingBrokerCommission;
	long	m_nMark;
	long	m_nContractID;
	IContractPtr m_spContract;

public:
	~CTrade()
	{
		m_spContract = NULL;
	}
};
typedef boost::shared_ptr<CTrade>	CTradePtr;
typedef std::vector<CTradePtr>	TradesColl;
//-------------------------------------------------------------------------------------------------------------//