#pragma once

class Row;
class DataProvider;
class Position;

class PositionCache: public SimpleCache<Position, long>{
public:
	PositionCache(DataProvider* data_provider);

	virtual ~PositionCache();

	virtual
	void erase_item(Position* item);

	virtual 
	Position* create_item(Row* row);

	virtual
	container_key get_key(Position* item);
};

class PositionCacheSingleton{
public:
	static PositionCache* instance();
};
