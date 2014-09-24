#include "StdAfx.h"

#include "PositionKeeper.h"
#include "AccountItem.h"
#include "PositionActor.h"
#include "Trade.h"
#include "Position.h"
#include "Analytics.h"


PositionKeeper::PositionKeeper(AccountItemKey* account_key, PositionActor* position_actor){
	this->account_key = account_key;
	this->trade_key = new TradeKey;
	this->position_actor = position_actor;
	position_table.clear();
};

PositionKeeper::position_container&
PositionKeeper::get_container(){
	return position_table;
};

void
PositionKeeper::process_trade(Trade* trade){
	if (trade){
		Position* position = get_position(trade->get_account_item());
		if (!position){
			position = create_position(trade->get_account_item());
		};
		position->process_trade(trade);
		if (position_actor){
			position_actor->process_trade(trade, position);
		};
	};
};

void
PositionKeeper::process_position(Position* position){
	if (position){
		Position* my_position = get_position(position->get_account_item());
		if (!my_position){
			my_position = add_position(position);
		};
		if (position_actor){
			position_actor->process_position(my_position);
		};
	};
};

void
PositionKeeper::overwrite_position(Position* position){
	if (position){
		Position* my_position = get_position(position->get_account_item());
		if (my_position && position_actor){
			position_actor->overwrite_position(position, my_position);					
		};
	}
};


Position*
PositionKeeper::get_position(AccountItem* account_item){
	position_key key = account_key->get_key(account_item);
	position_iterator it = position_table.find(key);
	if (it != position_table.end()){
		return it->second;
	};
	return 0;
};

Position* 
PositionKeeper::create_position(AccountItem* account_item){
	position_key key = account_key->get_key(account_item);
	Position* new_position = new Position(account_item, trade_key);
	position_table[key] = new_position;
	return new_position;
};

Position*
PositionKeeper::add_position(Position* position){
	position_key key = account_key->get_key(position->get_account_item());
	position_table[key] = position;
	position->set_trade_key(trade_key);
	return position;
};

void 
PositionKeeper::delete_position(AccountItem* account_item){
	position_key key = account_key->get_key(account_item);
	position_iterator it = position_table.find(key);
	if (it != position_table.end()){
		position_table.erase(it);
	};
};

PositionKeeper*
PositionKeeperSingleton::instance(){
	static PositionKeeper* keeper = 
		new PositionKeeper(new AccountItemKey, new GeneralPositionActor);
	return keeper;
};
