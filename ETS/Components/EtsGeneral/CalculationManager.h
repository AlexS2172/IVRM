#pragma once

#include "Calculator.h"

class BatchCollector;
class DataPacker;
class CalculationBatch;
class CEtsMain;

class CalculationManager: public CCalculator::IEventListener{
public:
	CalculationManager(	BatchCollector* bc,
						CCalculator* clc,
						DataPacker* dp, 
						CEtsMain* core);

	~CalculationManager();
	
	void add_contract(CAbstractContract* contract);
	void on_timer();
	/*virtual*/
	void on_complete(CalculationBatch* batch);

private:
	CalculationManager();

	BatchCollector* batch_collector_;
	DataPacker* data_packer_;
	CCalculator* calculator_;
	CEtsMain* core_;
};
