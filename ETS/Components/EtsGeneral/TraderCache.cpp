#include "StdAfx.h"
#include "TraderCache.h"
#include "Trader.h"


TraderCache::TraderCache(DataProvider* data_provider):
SimpleCache(data_provider){
};

/*virtual*/
void
TraderCache::erase_item(Trader *item){
	delete item;
};

/*virtual*/
TraderCache::~TraderCache(void){
};

/*virtual*/
TraderCache::container_key 
TraderCache::get_key(Trader* item){
	return container_key(item->id());
};

Trader*
TraderCache::create_item(Row* row){
	Trader* trader = new Trader("Default");

	row->get_column("iID")->get_value(trader->id());
	row->get_column("vcName")->get_value(trader->name());
	row->get_column("vcDescription")->get_value(trader->description());

	return trader;
};

/*static*/
TraderCache*
TraderCacheSingleton::instance(){
	static TraderCache* __instance = new TraderCache(0);
	return __instance;
};