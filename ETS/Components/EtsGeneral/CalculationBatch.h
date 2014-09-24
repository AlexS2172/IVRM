#pragma once

class CAbstractContract;

class CalculationBatch {
public:
	/*ctor*/
	CalculationBatch(long batch_id);

	/*dtor*/
	~CalculationBatch();

	void 
	add_item(CAbstractContract* contract);

	long
	get_id();

	void
	get_contracts(std::vector<CAbstractContract*>& contracts);

	long
	get_size();

private:

	CalculationBatch();

	typedef stdext::hash_map<long, CAbstractContract*> container;

	container items_;
	long batch_id_;
};

