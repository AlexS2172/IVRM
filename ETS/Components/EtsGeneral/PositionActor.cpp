#include "StdAfx.h"
#include "PositionActor.h"
#include "Analytics.h"
#include "Trade.h"
#include "Position.h"
#include "AbstractContract.h"
#include "AccountItem.h"

GeneralPositionActor::GeneralPositionActor(){
};

/*virtual*/
void 
GeneralPositionActor::process_trade(Trade* trade, Position* position){
	if (trade && position){
		if (trade->action() == Trade::enNewTrade){
			add_trade(trade, position);
		}
		else if (!position->is_empty()){
			delete_trade(trade, position);
		}
	}
};

/*virtual*/
void 
GeneralPositionActor::reconcile_position(Position* source, Position* dest){
	if (source && dest){
		source->get_analytics()->clone(dest->get_analytics());
	};
};

/*virtual*/
void 
GeneralPositionActor::overwrite_position(Position* source, Position* dest){
	if (source && dest){
		dest->get_analytics()->clone(source->get_analytics());	
	};
};

/*virtual*/
void 
GeneralPositionActor::process_position(Position* position){
	Analytics* anx = position->get_analytics();
	CAbstractContract* contract = position->get_account_item()->get_contract();

	long qty = anx->qty;
	bool is_buy = (qty > 0);
	long qty_in_asset = qty;
	double price = contract->GetClosePrice();

	long&	qty_prev = (is_buy ? anx->qty_prev_buy : anx->qty_prev_sell);
	double& ammount_prev = (is_buy ? anx->ammount_prev_buy : anx->ammount_prev_sell);

	zero_if_bad(qty_prev);
	qty_prev += qty_in_asset;

	zero_if_bad(ammount_prev);
	ammount_prev += qty_in_asset * price;

	zero_if_bad(anx->qty_in_asset);
	anx->qty_in_asset += qty_in_asset;
};

/*virtual*/
void 
GeneralPositionActor::add_trade(Trade* trade, Position* position){

	Analytics* anx = position->get_analytics();
	CAbstractContract* contract = position->get_account_item()->get_contract();

	bool is_buy = trade->_side;
	long qty = trade->_size * (is_buy ? 1: -1);
	long qty_in_asset = qty * static_cast<long>(contract->getContractSizeInAsset());

	double price = trade->_price;
	double price_close = contract->GetClosePrice();

	long&	qty_ltd = (is_buy ? anx->qty_ltd_buy : anx->qty_ltd_sell);
	double& ammount_ltd = (is_buy ? anx->ammount_ltd_buy : anx->ammount_ltd_sell);
	long&	qty_prev = (is_buy ? anx->qty_prev_buy : anx->qty_prev_sell);
	double& ammount_prev = (is_buy ? anx->ammount_prev_buy : anx->ammount_prev_sell);
	long&	qty_today = (is_buy ? anx->qty_today_buy : anx->qty_today_sell);
	double& ammount_today = (is_buy ? anx->ammount_today_buy : anx->ammount_today_sell);

	zero_if_bad(qty_ltd);
	qty_ltd += qty_in_asset;

	zero_if_bad(ammount_ltd);
	ammount_ltd += qty_in_asset * price;

	double current_date = CCalculationSettingsSingleton::Instance()->getCurrentDate();

	if (trade->_time_stamp < current_date){
		zero_if_bad(qty_prev);
		qty_prev += qty_in_asset;

		zero_if_bad(ammount_prev);
		ammount_prev += qty_in_asset * (price_close > 0 ? price_close : price);
	}
	else{
		zero_if_bad(qty_today);
		qty_today += qty_in_asset;

		zero_if_bad(ammount_today);
		ammount_today += qty_in_asset * price;
	};

	zero_if_bad(anx->qty);
	anx->qty += qty;

	zero_if_bad(anx->qty_in_asset);
	anx->qty_in_asset += qty_in_asset;
};

/*virtual*/
void 
GeneralPositionActor::delete_trade(Trade* trade, Position* position){
	trade->_size *= -1;
	add_trade(trade, position);
	trade->_size *= -1;
};
