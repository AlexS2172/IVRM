#pragma once

class CalculationBatch;
class CAbstractContract;

class BatchCollector {
public:

	/*Ctor*/
	BatchCollector();

	/*Dtor*/
	~BatchCollector();
	
	void add_contract(CAbstractContract* contract);

	virtual	
	CalculationBatch* next_batch();

private:
	typedef std::list<CalculationBatch*> container;

	virtual	
	long get_batch_id(CAbstractContract* contract);

	CalculationBatch* 
	find_batch(long batch_id);

	CalculationBatch* 
	add_calculation_batch(long batch_id);

	container batchs_;
};
