#include "StdAfx.h"
#include "DataProvider.h"

Value::Value(std::string name, _variant_t value){
	this->name = name;
	this->value = value;
};

Value::Value(){
};

std::string&
Value::get_name(){
	return name;
};

_variant_t&
Value::get_value(){
	return value;
};

void 
Value::get_value(std::string& strg){
	strg = (utils::HasValue(value) ? bstr_to_string(_bstr_t(value)) : std::string(""));
};

void 
Value::get_value(long& lng){
	lng = (utils::HasValue(value) ? long(value) : 0L);
};

void 
Value::get_value(bool& bl){
	bl = (utils::HasValue(value) ? bool(value) : false);
};

void 
Value::get_value(double& dbl){
	dbl = (utils::HasValue(value) ? double(value) : 0.0);
};

Row::Row(){
	this->values.clear();
};

Row*
Row::set_column(Value value){
	container_iterator it = values.find(value.get_name());
	if (it != values.end()){
		it->second = value;
	}
	else{
		values[value.get_name()] = Value(value);
	};
	return this;
};

Value*
Row::get_column(std::string name){
	container_iterator it = values.find(name);
	if (it != values.end()){
		return &it->second;
	};
	return 0;
};

Row::container&
Row::get_values(){
	return values;
};

Table::Table(){
	this->clear();
};

Table::~Table(){
	this->clear();
};

void
Table::clear(){
	container_iterator it = rows.begin();
	container_iterator itEnd = rows.end();

	for ( ; it != itEnd; it++){
		Row* row_ptr = (*it);
		delete row_ptr;
	};
	rows.clear();
};

Table::container&
Table::get_rows(){
	return rows;
};

bool
Table::insert(Row* row){
	rows.push_back(row);
	return true;
};

DataProvider::Exception::Exception(std::string description){
	this->description = description;
};

std::string
DataProvider::Exception::get_description(){
	return description;
};

DataProvider::DataRecordset::DataRecordset(){
};

DataProvider::DataRecordset::~DataRecordset(){
};