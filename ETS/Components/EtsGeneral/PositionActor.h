#pragma once

class Trade;
class Position;

class PositionActor{
public:			
	virtual void process_trade(Trade* trade, Position* position) = 0;
	virtual void reconcile_position(Position* source, Position* dest) = 0;
	virtual void process_position(Position* position) = 0;
	virtual void overwrite_position(Position* source, Position* dest) = 0;
	virtual void add_trade(Trade* trade, Position* position) = 0;
	virtual void delete_trade(Trade* trade, Position* position) = 0;
};

class GeneralPositionActor: public PositionActor{
public:
	GeneralPositionActor();
	/*virtual */
	void process_trade(Trade* trade, Position* position);

	/*virtual */
	void reconcile_position(Position* source, Position* dest);

	/*virtual */
	void process_position(Position* position);

	/*virtual */
	void overwrite_position(Position* source, Position* dest);

	/*virtual */
	void add_trade(Trade* trade, Position* position);

	/*virtual */
	void delete_trade(Trade* trade, Position* position);
};
