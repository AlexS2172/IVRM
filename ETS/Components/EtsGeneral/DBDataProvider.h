#pragma once

#include "DataProvider.h"
class Position;

class DBDataRecordset: public DataProvider::DataRecordset{
public:
	DBDataRecordset();

	virtual 
	~DBDataRecordset();

	CStoredProc<CClientRecordset>* db_recordset;
};

class DBDataProvider: public DataProvider{
public:
	DBDataProvider(std::string connection_string);
	
	virtual 
	~DBDataProvider(void);

	virtual
	bool open(std::string connection_string = std::string(""));

	virtual
	bool close();

	virtual
	Table* parse(DataRecordset* recordset);

	virtual
	Table* execute_command(DataProvider::Command* command);

	CDBConnection& get_connection();
private:
	CDBConnection db_connection;
	std::string connection_string;
};

class DBCommand: public DataProvider::Command{
public:
};

class DBSelectTrades: public DBCommand{
public:
	virtual
	DataProvider::DataRecordset* execute(DataProvider* data_provider);
};

class DBSelectTraders: public DBCommand{
public:
	virtual
	DataProvider::DataRecordset* execute(DataProvider* data_provider);
};

class DBSelectStrategys: public DBCommand{
public:
	virtual
	DataProvider::DataRecordset* execute(DataProvider* data_provider);
};

class DBSelectPositions: public DBCommand{
public:
	virtual
	DataProvider::DataRecordset* execute(DataProvider* data_provider);
};

class DBImportPositionAnalytics{
public:
	virtual
	DataProvider::DataRecordset* execute(DataProvider* data_provider);

	void add_position(Position* position);
private:
	std::vector<Position*> positions;
};