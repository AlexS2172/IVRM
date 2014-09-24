#pragma once

class DataProvider;
class Trade;
class Row;

class TradeCache: public SimpleCache<Trade, long>
{
public:
	TradeCache(DataProvider* data_provider);

	virtual ~TradeCache();
	
	virtual
	void erase_item(Trade* item);

	virtual 
	Trade* create_item(Row* row);

	virtual
	container_key get_key(Trade* item);
};

class TradeCacheSingleton{
public:
	static TradeCache* instance();
};
