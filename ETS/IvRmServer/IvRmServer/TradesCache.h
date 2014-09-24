#pragma once

class CTradesCache: public CCache,
					public CMessenger,
					public EgLib::CThread
{
public:
	CTradesCache(void);
	virtual ~CTradesCache(void);

public:
	void OnTradeUpdate(CTradePtr spTrade);
	void Clear();
	void LoadTrades();

	void SetSettingsImpl(CSettingsPtr spSettings);
	CSettingsPtr GetSettingsImpl();
	bool ProcessTradeForAnalytics(CTradePtr spTrade);
	void ReprocessTradesForAnalytics();
	UINT					Run();

private:
	IMmTradeChannelPtr	m_spTradeChannel;;
	CSettingsPtr		m_spSettings;
	BSTR				GetXMLString(IRisks* pRisks, CTradePtr spTrade);
	TradesColl			m_ReprocessTradesForAnalytics;
	CLock				m_ReprocessTradesForAnalyticsLock;
};

typedef boost::shared_ptr<CTradesCache>	CTradesCachePtr;
