#pragma once

class CAbstractContract;
class Trader;
class Strategy;

class AccountItem
{
public:
	AccountItem(void);
	~AccountItem(void);

	AccountItem(Trader* trader, Strategy* strategy, CAbstractContract* contract);

	Trader* get_trader();
	Strategy* get_strategy();
	CAbstractContract* get_contract();

private:
	CAbstractContract* contract;
	Trader* trader;
	Strategy* strategy;
};

class AccountItemKey{
public:
	typedef std::string account_key;

	virtual
	account_key get_key(AccountItem* account_item);
};
