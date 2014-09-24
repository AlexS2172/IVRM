#pragma once

class CAbstractContract;

class DataPacker {
public:
	typedef std::vector<CAbstractContract*> contract_vector;

	DataPacker();
	~DataPacker();
	
	virtual 
	long pack_analytics(contract_vector& contracts, LPSAFEARRAY& result);	
};
