#include "StdAfx.h"
#include "StrategyCache.h"
#include "DataProvider.h"
#include "Strategy.h"

StrategyCache::StrategyCache(DataProvider* data_provider):
SimpleCache(data_provider){
};

/*virtual*/
StrategyCache::~StrategyCache(void){
};


/*virtual*/
void
StrategyCache::erase_item(Strategy* item){
	delete item;
};

/*virtual*/
StrategyCache::container_key 
StrategyCache::get_key(Strategy* item){
	return container_key(item->id());
};

Strategy*
StrategyCache::create_item(Row* row){
	Strategy* strategy = new Strategy();
	row->get_column("iID")->get_value(strategy->id());
	row->get_column("vcName")->get_value(strategy->name());
	row->get_column("vcDescription")->get_value(strategy->description());
	return strategy;
};

/*static*/
StrategyCache*
StrategyCacheSingleton::instance(){
	static StrategyCache* __instance = new StrategyCache(0);
	return __instance;
};