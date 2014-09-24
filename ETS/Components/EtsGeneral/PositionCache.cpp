#include "StdAfx.h"
#include "PositionCache.h"
#include "Position.h"
#include "Trader.h"
#include "Strategy.h"
#include "DataProvider.h"
#include "Analytics.h"
#include "AccountItem.h"
#include "TraderCache.h"
#include "StrategyCache.h"
#include "ContractsCache.h"

PositionCache::PositionCache(DataProvider* data_provider):
SimpleCache(data_provider){
};

/*virtual*/
PositionCache::~PositionCache(void){
};


/*virtual*/
void
PositionCache::erase_item(Position* item){
	delete item;
};

/*virtual*/
PositionCache::container_key 
PositionCache::get_key(Position* item){
	return container_key(item->id());
};

Position*
PositionCache::create_item(Row* row){
	Position* position = new Position();
	Analytics* anx = position->get_analytics();

	row->get_column("iID")->get_value(position->id());
	row->get_column("dtPositionDate")->get_value(position->time_stamp());
	row->get_column("iQty")->get_value(anx->qty);

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

	position->set_account_item(new AccountItem(trader, strategy, contract));

	return position;
};

/*static*/
PositionCache*
PositionCacheSingleton::instance(){
	static PositionCache* __instance = new PositionCache(0);
	return __instance;
};