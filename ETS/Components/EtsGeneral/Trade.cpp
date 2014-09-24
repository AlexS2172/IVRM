#include "StdAfx.h"
#include "Trade.h"

Trade::Trade(void):
_account_item(0),
_size(BAD_LONG_VALUE),
_side(false),
_price(BAD_DOUBLE_VALUE),
_id(BAD_LONG_VALUE),
_traded_iv(BAD_DOUBLE_VALUE),
_spot_reference(BAD_DOUBLE_VALUE),
_exec_id(""),
_time_stamp(0){
};

Trade::~Trade(void){
};

Trade::Trade(long size, bool side, double price){
	this->_account_item = 0;
	this->_side = side;
	this->_price = price;
	this->_size = size;
	this->_action = enNewTrade;
};

AccountItem*
Trade::get_account_item(){
	return _account_item;
};

void
Trade::set_account_item(AccountItem* account_item){
	this->_account_item = account_item;
};

long&
Trade::size(){
	return _size;
};

double&
Trade::price(){
	return _price;
};

bool&
Trade::side(){
	return _side;
};

long&
Trade::id(){
	return _id;
};

Trade::TradeAction&
Trade::action(){
	return _action;
};

std::string& 
Trade::exec_id(){
	return _exec_id;
};

double& 
Trade::spot_reference(){
	return _spot_reference;
};

double& 
Trade::time_stamp(){
	return _time_stamp;
};

double& 
Trade::traded_iv(){
	return _traded_iv;
};
//--------------------------------------------------//
TradeKey::TradeKey(){
};

TradeKey::trade_key
TradeKey::get_key(Trade *trade){
	if (trade){
		std::ostringstream key;
		key << trade->id();
		return trade_key(key.str());
	};
	return trade_key("");
};
//--------------------------------------------------//


