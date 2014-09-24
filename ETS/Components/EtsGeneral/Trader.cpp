#include "StdAfx.h"
#include "Trader.h"

Trader::Trader(void){
};

Trader::~Trader(void){
};

Trader::Trader(std::string name, long id /* = 0*/){
	this->_name = name;
	this->_id = id;
};

std::string&
Trader::name(){
	return _name;
};

long& 
Trader::id(){
	return _id;
};

std::string&
Trader::description(){
	return _description;
};
