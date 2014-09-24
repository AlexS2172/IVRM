#include "StdAfx.h"
#include "CalculationManager.h"
#include "Calculator.h"
#include "BatchCollector.h"
#include "CalculationBatch.h"
#include "DataPacker.h"
#include "EtsMain.h"
//-------------------------------------------------------//
/*private ctor*/
CalculationManager::CalculationManager(){
};
//-------------------------------------------------------//
/*dtor*/
CalculationManager::~CalculationManager(){
};
//-------------------------------------------------------//
CalculationManager::CalculationManager(BatchCollector *bc,
									   CCalculator* clc,
									   DataPacker *dp,
									   CEtsMain* core):
batch_collector_(bc),
calculator_(clc),
data_packer_(dp),
core_(core){
	calculator_->attach(this);
};
//-------------------------------------------------------//
void
CalculationManager::on_timer() {

	for (int i = 0; i < 4; i++) {
		CalculationBatch* batch = batch_collector_->next_batch();
		if (batch){
			calculator_->Calculate(batch);
		}
	};
};
//-------------------------------------------------------//
void
CalculationManager::add_contract(CAbstractContract* contract){
	ContractsVector derivative;
	
	batch_collector_->add_contract(contract);

	contract->get_derivative(derivative);

	ContractsVector::iterator it = derivative.begin();
	ContractsVector::iterator it_end = derivative.end();

	for ( ;it != it_end; it++) {
		batch_collector_->add_contract(*it);
	};
};
//-------------------------------------------------------//
/*virtual*/
void
CalculationManager::on_complete(CalculationBatch* batch){

	LPSAFEARRAY	my_data = 0;
	std::vector<CAbstractContract*> contracts;

	batch->get_contracts(contracts);

	data_packer_->pack_analytics(contracts, my_data);

	core_->Fire_OnAnalytics(my_data);

	SafeArrayDestroy(my_data);

	delete batch;
};
//-------------------------------------------------------//
