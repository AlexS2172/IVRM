#pragma once

class Value{
public:
	Value(std::string name, _variant_t value);

	_variant_t& get_value();
	std::string& get_name();
	
	void get_value(std::string& strg);
	void get_value(long& lng);
	void get_value(bool& bl);
	void get_value(double& dbl);

	Value();
private:
	std::string name;
	_variant_t value;
};

class Row{
public:
	typedef std::map<std::string, Value> container;
	typedef container::iterator container_iterator;
	
	Row();

	Row* set_column(Value value);
	Value* get_column(std::string name);
	container& get_values();

private:
	container values;
};

class Table{
public:
	typedef std::list<Row*>	container;
	typedef container::iterator container_iterator;

	Table();

	virtual	
	~Table();

	void clear();

	bool insert(Row* row);

	container& get_rows();

private:
	container rows;
};



class DataProvider{
public:

	class DataRecordset{
	public:
		DataRecordset();

		virtual
		~DataRecordset();
	};

	class Command{
	public:
		virtual
		DataRecordset* execute(DataProvider*) = 0;
	};

	class Exception{
	public:
		Exception(std::string description);

		std::string
		get_description();

	private:
		std::string description;
	};

	virtual
	Table* parse(DataRecordset* recordset) = 0;

	virtual 
	bool open(std::string connection_string) = 0;

	virtual
	bool close() = 0;

	virtual
	Table* execute_command(DataProvider::Command* command) = 0;
};
