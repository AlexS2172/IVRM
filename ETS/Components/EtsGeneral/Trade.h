#pragma once

class AccountItem;
class GeneralPositionActor;

class Trade
{
public:

	enum TradeAction{
		enNewTrade = 0,
		enDeleteTrade = 1
	};

	Trade(void);
	~Trade(void);

	Trade(long qty, bool side, double price);
	
	long& id();
	double& price();
	long& size();
	bool& side();
	std::string& exec_id();
	double& spot_reference();
	double& time_stamp();
	double& traded_iv();

	TradeAction& action();

	AccountItem* get_account_item();
	void set_account_item(AccountItem* account_item);

private:
	
	long _id;
	long _size;
	bool _side;
	double _price;
	TradeAction _action;
	double _time_stamp;
	double _traded_iv;
	double _spot_reference;
	std::string _exec_id;

	AccountItem* _account_item;

	friend GeneralPositionActor;
};

class TradeKey{
public:
	typedef std::string trade_key;

	TradeKey();

	virtual
	trade_key get_key(Trade* trade);
};


