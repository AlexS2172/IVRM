#pragma once

class Trader;
class Row;
class DataProvider;

class TraderCache: public SimpleCache<Trader, long>{
public:
	TraderCache(DataProvider* data_provider);

	virtual ~TraderCache();

	virtual
	void erase_item(Trader* item);

	virtual 
	Trader* create_item(Row* row);

	virtual
	container_key get_key(Trader* item);
};

class TraderCacheSingleton{
public:
	static TraderCache* instance();
};
