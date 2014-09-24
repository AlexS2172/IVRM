#include "StdAfx.h"
#include "AccountItem.h"
#include "Trader.h"
#include "Strategy.h"
#include "AbstractContract.h"

AccountItem::AccountItem(){
	trader = 0;
	strategy = 0;
	contract = 0;
};

AccountItem::~AccountItem(){
	trader = 0;
	strategy = 0;
	contract = 0;
};

AccountItem::AccountItem(Trader *trader, Strategy *strategy, CAbstractContract *contract){
	this->trader = trader;
	this->strategy = strategy;
	this->contract = contract;
};

Trader*
AccountItem::get_trader(){
	return trader;
};

Strategy*
AccountItem::get_strategy(){
	return strategy;
};

CAbstractContract* 
AccountItem::get_contract(){
	return contract;
};
//---------------------------------------------------//
AccountItemKey::account_key 
AccountItemKey::get_key(AccountItem *account_item){
	if (account_item){
		std::ostringstream key;

		Trader* trader = account_item->get_trader();
		Strategy* strategy = account_item->get_strategy();
		CAbstractContract* contract = account_item->get_contract();

		if (trader){
			key << trader->id();
		};
		key << ":";

		if (strategy){
			key << strategy->id();
		};
		key << ":";

		if (contract){
			key << contract->m_nID;
		};
		
		return account_key(key.str());
	}
	return account_key("");
};
//---------------------------------------------------//