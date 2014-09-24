#pragma once

class DataProvider;
class Strategy;
class Row;

class StrategyCache: public SimpleCache<Strategy, long>
{
public:
	virtual 
	~StrategyCache(void);

	StrategyCache(DataProvider* data_provider);

	virtual
	void erase_item(Strategy* item);

	virtual 
	Strategy* create_item(Row* row);

	virtual
	container_key get_key(Strategy* item);
};

class StrategyCacheSingleton{
public:
	static StrategyCache* instance();
};
