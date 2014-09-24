#include "StdAfx.h"

#include "CalculationBatch.h"
#include "AbstractContract.h"
//---------------------------------------------------------//
CalculationBatch::CalculationBatch(long batch_id):
batch_id_(batch_id){
	items_.clear();
};
//---------------------------------------------------------//
/*private Ctor*/
CalculationBatch::CalculationBatch(){
};
//---------------------------------------------------------//
CalculationBatch::~CalculationBatch(){
	items_.clear();
};
//---------------------------------------------------------//
long
CalculationBatch::get_id(){
	return batch_id_;
};
//---------------------------------------------------------//
long
CalculationBatch::get_size(){
	return static_cast<long>(items_.size());
};
//---------------------------------------------------------//
void
CalculationBatch::add_item(CAbstractContract *contract){
	items_[contract->get_id()] = contract;
};
//---------------------------------------------------------//
void
CalculationBatch::get_contracts(std::vector<CAbstractContract *>& contracts){
	
	contracts.clear();
	container::iterator it = items_.begin();
	container::iterator it_end = items_.end();

	for (; it != it_end; it++){
		contracts.push_back(it->second);
	};
};
//---------------------------------------------------------//
