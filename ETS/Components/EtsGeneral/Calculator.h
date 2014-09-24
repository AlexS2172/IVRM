#pragma once

class CAbstractContract;
class CalculationBatch;
//-----------------------------------------------------------//
class CCalculator {
public:

	class IEventListener{
	public:
		virtual void on_complete(CalculationBatch* batch) = 0;
	};

	typedef std::vector<CAbstractContract*> contract_vector;
	
	void SetPoolSize(int lCount);

	/*ctor*/
	CCalculator();

	/*dtor*/
	~CCalculator();

	void Calculate(contract_vector& contracts);
	void FitToMarketVolatility(contract_vector& contracts);
	void Calculate(CalculationBatch* batch);

	virtual 
	void OnComplete(CAbstractContract* pContract);

	void attach(IEventListener* listener);
	void notify(CalculationBatch* batch);

private:
	CThreadPool<eg_lib::CEGThreadPool>	m_ThreadPool;

	HANDLE m_hComplete;

	volatile	
	long m_nContractsProcessed;

	CLock m_lock;

	void FitToMarketVolatility(CAbstractContract* pContract);
	void Calculate(CAbstractContract* pContract);

	IEventListener* event_listener_;
};

typedef boost::shared_ptr<CCalculator>	CCalculatorPtr;
//-----------------------------------------------------------//
class CCalculationAtom: public eg_lib::CObjectWorkerInterface {
public:
	/*ctor*/
	CCalculationAtom(CAbstractContract* pContract, 
					 CCalculator* pCalculator);

	/*dtor*/
	virtual 
	~CCalculationAtom();

	CAbstractContract*	m_pContract;
	CCalculator*		m_pCalculator;

	/*virtual*/
	void DoTask();

private:
	CCalculationAtom();
};
//-----------------------------------------------------------//
class CVolatilityFittingAtom: public CCalculationAtom {
public:
	/*ctor*/
	CVolatilityFittingAtom(CAbstractContract* pContract, 
						   CCalculator* pCalculator);
	
	/*dtor*/
	virtual 
	~CVolatilityFittingAtom();

	/*virtual*/
	void DoTask();
};
//-----------------------------------------------------------//
class BatchCalculationAtom: public eg_lib::CObjectWorkerInterface {
public:
	/*ctor*/
	BatchCalculationAtom(CCalculator* calculator, CalculationBatch* batch);

	/*dtor*/
	virtual
	~BatchCalculationAtom();

	void DoTask();

private:

	/*dtor*/
	BatchCalculationAtom();

	CCalculator* calculator_;
	CalculationBatch* batch_;

	typedef std::vector<CAbstractContract*> container;
	container contracts_;
};
//-----------------------------------------------------------//
typedef boost::shared_ptr<CCalculationAtom>	CCalculationAtomPtr;
typedef boost::shared_ptr<CVolatilityFittingAtom> CVolatilityFittingAtomPtr;
//-----------------------------------------------------------//