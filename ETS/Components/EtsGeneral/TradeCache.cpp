#include "StdAfx.h"
#include "TradeCache.h"
#include "Trade.h"
#include "DBDataProvider.h"
#include "AccountItem.h"
#include "TraderCache.h"
#include "StrategyCache.h"
#include "ContractsCache.h"


TradeCache::TradeCache(DataProvider* data_provider):
SimpleCache(data_provider){
};

/*virtual*/
TradeCache::~TradeCache(void){
};


/*virtual*/
void
TradeCache::erase_item(Trade* item){
	delete item;
};

/*virtual*/
TradeCache::container_key 
TradeCache::get_key(Trade* item){
	return container_key(item->id());
};

Trade*
TradeCache::create_item(Row* row){
	Trade* trade = new Trade();

	row->get_column("iID")->get_value(trade->id());
	row->get_column("tiSide")->get_value(trade->side());
	row->get_column("iSize")->get_value(trade->size());
	row->get_column("fPrice")->get_value(trade->price());
	row->get_column("fSpotReference")->get_value(trade->spot_reference());
	row->get_column("fTradedIV")->get_value(trade->traded_iv());
	row->get_column("dtTradeDate")->get_value(trade->time_stamp());

	long action = 0;
	row->get_column("iAction")->get_value(action);
	trade->action() = (action == 0 ? Trade::enNewTrade : Trade::enDeleteTrade);

	long strategy_id = 0L;
	row->get_column("iStrategyID")->get_value(strategy_id);
	Strategy* strategy = StrategyCacheSingleton::instance()->get_item(strategy_id);

	long trader_id = 0L;
	row->get_column("iTraderID")->get_value(trader_id);
	Trader* trader = TraderCacheSingleton::instance()->get_item(trader_id);
	
	long contract_id = 0L;
	row->get_column("iContractID")->get_value(contract_id);

	IContractPtr sp_contract = CContractsCache::instance()->GetContract(contract_id);
	CAbstractContract* contract = dynamic_cast<CAbstractContract*>(sp_contract.GetInterfacePtr());

	trade->set_account_item(new AccountItem(trader, strategy, contract));

	return trade;
};

/*static*/
TradeCache*
TradeCacheSingleton::instance(){
	static TradeCache* __instance = new TradeCache(0);
	return __instance;
};
