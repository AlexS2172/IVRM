#pragma once

class Trade;
class AccountItem;
class TradeKey;
class Analytics;

class Position
{
public:
	typedef std::string container_key;
	typedef std::map<container_key, Trade*> trade_container;
	typedef trade_container::iterator trade_iterator;

	Position(AccountItem* account_item, TradeKey* trade_key);
	
	Position(void);
	~Position(void);
	
	Analytics* 
	get_analytics();

	AccountItem* 
	get_account_item();

	void 
	set_account_item(AccountItem* account_item);

	void
	set_trade_key(TradeKey* trade_key);

	long& id();
	double& time_stamp();

	bool 
	is_empty();

	void 
	process_trade(Trade* trade);

	virtual 
	void calculate();

private:
	
	Trade* 
	get_trade(Trade* trade);
	
	Trade* 
	add_trade(Trade* trade);
	
	void 
	delete_trade(Trade* trade);

	long _id;
	double _time_stamp;
	trade_container trade_table;
	AccountItem* account_item;
	Analytics* analytics;
	TradeKey* trade_key;
};
