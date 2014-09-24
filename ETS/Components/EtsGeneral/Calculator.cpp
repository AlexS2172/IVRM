#include "StdAfx.h"
#include "Calculator.h"
#include "AbstractContract.h"
#include "CalculationBatch.h"
//----------------------------------------------------------//
CCalculator::CCalculator(){
	m_ThreadPool.Initialize(NULL, 0);
	m_hComplete = ::CreateEvent(NULL, false, false, NULL);
	event_listener_ = 0;
};
//----------------------------------------------------------//
CCalculator::~CCalculator(){
	m_ThreadPool.Shutdown(5000);
	::CloseHandle(m_hComplete);
};
//----------------------------------------------------------//
void
CCalculator::attach(IEventListener* listener){
	event_listener_ = listener;
};
//----------------------------------------------------------//
void
CCalculator::notify(CalculationBatch* batch){
	if (event_listener_) event_listener_->on_complete(batch);
};
//----------------------------------------------------------//
void 
CCalculator::FitToMarketVolatility(CAbstractContract* pContract){
	if (pContract){
		m_ThreadPool.QueueRequest((eg_lib::CEGThreadPool::RequestType)new CVolatilityFittingAtom(pContract, this));
	};
};
//----------------------------------------------------------//
void 
CCalculator::Calculate(CAbstractContract* pContract){
	if (pContract){
		m_ThreadPool.QueueRequest((eg_lib::CEGThreadPool::RequestType)new CCalculationAtom(pContract, this));
	};
};
//----------------------------------------------------------//
void 
CCalculator::Calculate(CCalculator::contract_vector& contracts) {
	if (!contracts.empty()) {
		InterlockedExchange(&m_nContractsProcessed, static_cast<long>(contracts.size()));

		CCalculator::contract_vector::iterator it = contracts.begin();
		CCalculator::contract_vector::iterator it_end = contracts.end();
		for (; it != it_end; it++) {
			CAbstractContract* pContract = *it;
			if (pContract != NULL)
				Calculate(pContract);
		};
		//wait to complete
		::WaitForSingleObject(m_hComplete, INFINITE);
	};
};
//----------------------------------------------------------//
void 
CCalculator::FitToMarketVolatility(CCalculator::contract_vector& contracts){
	if (!contracts.empty())	{
		InterlockedExchange(&m_nContractsProcessed, static_cast<long>(contracts.size()));

		CCalculator::contract_vector::iterator it = contracts.begin();
		CCalculator::contract_vector::iterator it_end = contracts.end();
		for (; it != it_end; it++) {
			CAbstractContract* pContract = *it;
			if (pContract != NULL)
				FitToMarketVolatility(pContract);
		};
		//wait to complete
		::WaitForSingleObject(m_hComplete, INFINITE);
	};
};
//----------------------------------------------------------//
void
CCalculator::Calculate(CalculationBatch* batch){
	m_ThreadPool.QueueRequest((eg_lib::CEGThreadPool::RequestType)new BatchCalculationAtom(this, batch));
};
//----------------------------------------------------------//
void 
CCalculator::OnComplete(CAbstractContract* pContract){
	//CAutoLock cs(m_lock);
	if (!InterlockedDecrement(&m_nContractsProcessed))
		SetEvent(m_hComplete);
};
//----------------------------------------------------------//
void 
CCalculator::SetPoolSize(int lCount){
	m_ThreadPool.SetSize(lCount);
};
//----------------------------------------------------------//
/*ctor*/
CVolatilityFittingAtom::CVolatilityFittingAtom(CAbstractContract* pContract,
											   CCalculator* pCalculator)
:CCalculationAtom(pContract, pCalculator){
	/*ctor code here*/
};
//----------------------------------------------------------//
/*virtual dtor*/
CVolatilityFittingAtom::~CVolatilityFittingAtom(){
	/*dtor code here*/
};
//----------------------------------------------------------//
/*virtual*/
void
CVolatilityFittingAtom::DoTask() {
	try	{
		if (m_pContract) {
			m_pContract->FitToMarketVolatility();
			m_pCalculator->OnComplete(m_pContract);
		}
	}
	catch (...) {
		if (m_pCalculator)
			m_pCalculator->OnComplete(m_pContract);
		ATLASSERT(false);			
	};
};
//----------------------------------------------------------//
/*virtual*/
void
CCalculationAtom::DoTask() {
	try {
		if (m_pContract){
			try	{
				m_pContract->Calculate(CMarketSimulator::getDefault());
			}
			catch (const IvRmCore::IvRmException::AbstractException&){
			}
			m_pCalculator->OnComplete(m_pContract);
		}
	}
	catch (...){
		if (m_pCalculator)
			m_pCalculator->OnComplete(m_pContract);
		ATLASSERT(false);			
	};
};
//----------------------------------------------------------//
/*ctor*/
CCalculationAtom::CCalculationAtom(CAbstractContract* pContract, 
								   CCalculator* pCalculator):
m_pContract(pContract),
m_pCalculator(pCalculator){
};
//----------------------------------------------------------//
/*virtual dtor*/
CCalculationAtom::~CCalculationAtom(){
};
//----------------------------------------------------------//
/*ctor*/
CCalculationAtom::CCalculationAtom(){
};
//----------------------------------------------------------//
/*private ctor*/
BatchCalculationAtom::BatchCalculationAtom(){
};
//----------------------------------------------------------//
/*ctor*/
BatchCalculationAtom::BatchCalculationAtom(CCalculator *calculator, 
										   CalculationBatch *batch):
calculator_(calculator),
batch_(batch){
};
//----------------------------------------------------------//
/*dtor*/
BatchCalculationAtom::~BatchCalculationAtom(){
};
//----------------------------------------------------------//
/*virtual*/
void
BatchCalculationAtom::DoTask(){
	try {
		batch_->get_contracts(contracts_);

		container::iterator it = contracts_.begin();
		container::iterator it_end = contracts_.end();

		for ( ;it != it_end; it++){
			CAbstractContract* contract = *it;
			if (contract){
				try {
					contract->Calculate(CMarketSimulator::getDefault());
				}
				catch (...) {
				};
			}
		};

		calculator_->notify(batch_);
	}
	catch (...){
		calculator_->notify(batch_);
	};
};
//----------------------------------------------------------//