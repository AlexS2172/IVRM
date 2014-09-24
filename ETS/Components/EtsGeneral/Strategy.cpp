#include "StdAfx.h"
#include "Strategy.h"

Strategy::~Strategy(void){
};

Strategy::Strategy(std::string name /* = N/A*/, long id /* = 0 */){
	this->_name = name;
	this->_id = id;
};

long&
Strategy::id(){
	return _id;
};

std::string&
Strategy::name(){
	return _name;
};

std::string&
Strategy::description(){
	return _description;
};



