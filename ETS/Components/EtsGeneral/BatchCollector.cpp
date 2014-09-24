#include "StdAfx.h"

#include "BatchCollector.h"
#include "CalculationBatch.h"
#include "AbstractContract.h"
//---------------------------------------------------------//
BatchCollector::BatchCollector(){
	batchs_.clear();
};
//---------------------------------------------------------//
BatchCollector::~BatchCollector(){
	batchs_.clear();
};
//---------------------------------------------------------//
/*virtual*/
long
BatchCollector::get_batch_id(CAbstractContract* contract){

	CAbstractContract* base_contract = 
		contract->get_base_contract();

	return (base_contract ? base_contract->get_id() : contract->get_id());
};
//---------------------------------------------------------//
void
BatchCollector::add_contract(CAbstractContract* contract){

	long batch_id = get_batch_id(contract);

	CalculationBatch* batch = find_batch(batch_id);
	if (!batch){
		batch = add_calculation_batch(batch_id);
	};

	batch->add_item(contract);
};
//---------------------------------------------------------//
/*virtual*/
CalculationBatch*
BatchCollector::next_batch(){
	if (!batchs_.empty()){
		CalculationBatch* cb = batchs_.back();
		batchs_.pop_back();
		return cb;
	};
	return 0;
};
//---------------------------------------------------------//
CalculationBatch* 
BatchCollector::find_batch(long batch_id){
	container::iterator it = batchs_.begin();
	container::iterator it_end = batchs_.end();

	for( ;it != it_end; it++){
		CalculationBatch* batch = (*it);
		if (batch->get_id() == batch_id) return batch;
	};

	return 0;
};
//---------------------------------------------------------//
CalculationBatch* 
BatchCollector::add_calculation_batch(long batch_id){
	CalculationBatch* cb = new CalculationBatch(batch_id);
	batchs_.push_front(cb);
	return cb;
};
//---------------------------------------------------------//