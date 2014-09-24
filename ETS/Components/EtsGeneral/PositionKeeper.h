#pragma once

class PositionActor;
class AccountItem;
class AccountItemKey;
class Position;
class Trade;
class TradeKey;

class PositionKeeper{
public:
	typedef std::string position_key;
	typedef std::map<position_key, Position*>	position_container;
	typedef position_container::iterator position_iterator;

	PositionKeeper(AccountItemKey* account_key, PositionActor* position_actor);

	void
	process_trade(Trade* trade);

	void
	process_position(Position* position);

	void
	overwrite_position(Position* position);

	position_container& get_container();

private:

	AccountItemKey* account_key;
	TradeKey* trade_key;
	PositionActor* position_actor;

	Position*
	get_position(AccountItem* account_item);

	Position* 
	create_position(AccountItem* account_item);

	Position*
	add_position(Position* position);

	void 
	delete_position(AccountItem* account_item);

	position_container position_table;
};

class PositionKeeperSingleton{
public:
	static PositionKeeper* instance();
};
