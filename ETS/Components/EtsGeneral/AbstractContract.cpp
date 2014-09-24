#include "StdAfx.h"
#include "AbstractContract.h"
#include "ContractColl.h"
//--------------------------------------------------------------------//
CAbstractContract::CAbstractContract(void): 
m_nID(0),
m_enType(enCtNone),
m_bstrSymbol(L"NONE"),
m_bstrContractName(L"NONE"),
m_dClosePrice(BAD_DOUBLE_VALUE),
m_dTheoClosePrice(BAD_DOUBLE_VALUE),
m_dManualPrice(BAD_DOUBLE_VALUE),
m_nBaseContractID(0),
m_nAssetID(0),
m_nRootID(BAD_LONG_VALUE),
m_spRoot(NULL){
	m_spQuote = CQuotePtr(new CQuote());
	m_spRisks = CRisksPtr(new CRisks());
};
//--------------------------------------------------------------------//
CAbstractContract::~CAbstractContract(void){
};
//--------------------------------------------------------------------//
/*virtual*/
void 
CAbstractContract::Clear(){
	
	//IAsset
	m_spAsset = NULL;
	m_spBaseContract = NULL;
	m_spPricingAsset = NULL;
	m_spDerivativeContracts = NULL;

	//IUndAtom
	m_spUnd = NULL;
	m_spFut = NULL;
	m_spFutRoot = NULL;
};
//--------------------------------------------------------------------//
long
CAbstractContract::get_id(){
	return m_nID;
};
//--------------------------------------------------------------------//
CAsset*
CAbstractContract::get_asset(){
	return dynamic_cast<CAsset*>(m_spAsset.GetInterfacePtr());
};
//--------------------------------------------------------------------//
CAsset*
CAbstractContract::get_pricing_asset(){
	return dynamic_cast<CAsset*>(m_spPricingAsset.GetInterfacePtr());
};
//--------------------------------------------------------------------//
CAbstractContract*
CAbstractContract::get_base_contract(){
	return dynamic_cast<CAbstractContract*>(m_spBaseContract.GetInterfacePtr());
};
//--------------------------------------------------------------------//
void
CAbstractContract::get_derivative(ContractsVector& derivative){
	if (m_spDerivativeContracts){

		CContractColl* pContracts = 
			dynamic_cast<CContractColl*>(m_spDerivativeContracts.GetInterfacePtr());

		CContractColl::CollType::iterator itr = 
			pContracts->m_collRef.begin();

		CContractColl::CollType::iterator itr_end = 
			pContracts->m_collRef.end();

		for(; itr != itr_end; ++itr){

			CAbstractContract* contract = 
				dynamic_cast<CAbstractContract*>(itr->second->second);

			ATLASSERT(contract != 0);

			derivative.push_back(contract);
		};
	};
};
//--------------------------------------------------------------------//
EtsContractTypeEnum	
CAbstractContract::GetType(){

	EtsContractTypeEnum enType;
	
	if(m_spFut != NULL)
		enType = enCtFuture;
	else
		_CHK(m_spUnd->get_UndType(&enType));

	return enType;
};
//--------------------------------------------------------------------//
void 
CAbstractContract::SetType(enum EtsContractTypeEnum enType){
};
//--------------------------------------------------------------------//
LONG 
CAbstractContract::GetID(){
	LONG lID = 0;

	if(m_spFut != NULL)
	{
		_CHK(m_spFut->get_ID(&lID));
	}
	else if(m_spUnd != NULL)
	{
		_CHK(m_spUnd->get_ID(&lID));
	}
	return lID;
};

void 
CAbstractContract::SetID(LONG lID){
};
//--------------------------------------------------------------------//
BSTR 
CAbstractContract::GetSymbol(){

	CComBSTR bsSymbol;
	if(m_spFut != NULL)
	{
		_CHK(m_spFut->get_Symbol(&bsSymbol));
	}
	else if(m_spUnd != NULL)
	{
		_CHK(m_spUnd->get_Symbol(&bsSymbol));
	}
	return bsSymbol.Copy();
};

void 
CAbstractContract::SetSymbol(BSTR bstrSymbol){
};
//--------------------------------------------------------------------//
BSTR 
CAbstractContract::GetContractName(){

	CComBSTR bsContractName;
	if(m_spFut != NULL)
	{
		_CHK(m_spFut->get_ContractName(&bsContractName));
	}
	else if(m_spUnd != NULL)
	{
		_CHK(m_spUnd->get_ContractName(&bsContractName));
	}
	return bsContractName.Copy();
};

void 
CAbstractContract::SetContractName(BSTR bstrSymbol){
};
//--------------------------------------------------------------------//
DOUBLE	
CAbstractContract::GetManualPrice(){
	return m_dManualPrice;
};

void	
CAbstractContract::SetManualPrice(DOUBLE dValue){
	m_dManualPrice = dValue;
};
//--------------------------------------------------------------------//
DOUBLE	
CAbstractContract::GetClosePrice(){
	return m_dClosePrice;
};

void	
CAbstractContract::SetClosePrice(DOUBLE dValue){
	m_dClosePrice = dValue;
};
//--------------------------------------------------------------------//
DOUBLE	
CAbstractContract::GetTheoClosePrice(){
	return m_dTheoClosePrice;
};

void	
CAbstractContract::SetTheoClosePrice(DOUBLE dValue){
	m_dTheoClosePrice = dValue;
};

//--------------------------------------------------------------------//
IContractPtr 
CAbstractContract::GetBaseContract(){
	return m_spBaseContract;
};

void 
CAbstractContract::SetBaseContract(IContractPtr spBaseContract){
	try{

		m_spBaseContract = spBaseContract;

		if (static_cast<bool>(m_spBaseContract)){

			IContractCollPtr spDerivatives = NULL; 
			_CHK(m_spBaseContract->get_DerivativeContracts(&spDerivatives), _T("Fail to get Derivatives coll of Contract"));
			
			if (static_cast<bool>(spDerivatives)){

				IContractPtr	spAdded;
				CContractColl*	pDerivatives = dynamic_cast<CContractColl*>(spDerivatives.GetInterfacePtr());

				_CHK(pDerivatives->Add(	m_nID, m_bstrSymbol,
										dynamic_cast<IContract*>(this),
										&spAdded), _T("Fail to Add  derived contract."));
			};
		};
	}
	catch (_com_error&){
		ATLASSERT(FALSE);
		throw;		
	}
	catch (...){
		ATLASSERT(FALSE);
		throw;
	}
};
//--------------------------------------------------------------------//
/*virtual*/
double
CAbstractContract::getMaturityWeight(double Period, double CurrentDate){
	return 1.;
};
//--------------------------------------------------------------------//
IAssetPtr	
CAbstractContract::GetAsset(){
	return m_spAsset;
};
//--------------------------------------------------------------------//
void 
CAbstractContract::SetAsset(IAssetPtr spAsset){
	try{

		m_spAsset = spAsset;

		if (static_cast<bool>(m_spAsset)){

			IContractCollPtr spContracts = NULL; 
			_CHK(m_spAsset->get_Contracts(&spContracts), _T("Fail to get Contracts coll of Asset"));
			
			if (static_cast<bool>(spContracts)){

				IContractPtr	spAdded;
				CContractColl*	pContracts = dynamic_cast<CContractColl*>(spContracts.GetInterfacePtr());

				_CHK(pContracts->Add(	m_nID, m_bstrSymbol,
										dynamic_cast<IContract*>(this),
										&spAdded), _T("Fail to Add contract."));
			};
		};
	}
	catch (_com_error&){
		ATLASSERT(FALSE);
		throw;		
	}
	catch (...){
		ATLASSERT(FALSE);
		throw;
	}
};
//------------------------------------------------------------------------//
CRisksPtr 
CAbstractContract::Calculate(CMarketSimulator::ptr MarketSimulator){
	return m_spRisks;
};
//------------------------------------------------------------------------//
void 
CAbstractContract::FitToMarketVolatility(){
};
//------------------------------------------------------------------------//
CTickerPtr 
CAbstractContract::GetTicker(){

	if (m_spTicker.get() == NULL){

		m_spTicker = CTickerPtr(new CTicker());
		
		char* p_Symbol = _com_util::ConvertBSTRToString(m_bstrSymbol.m_str);
		m_spTicker->m_sSymbol = p_Symbol;
		delete[] p_Symbol;

		if (m_enType == enCtOption)
			m_spTicker->m_enType = enOPT;
		else if (m_enType == enCtFutOption)
			m_spTicker->m_enType= enFOP;
		else if (m_enType == enCtStock)
			m_spTicker->m_enType= enSTK;
		else if (m_enType == enCtIndex)
			m_spTicker->m_enType = enIDX;
		else if (m_enType == enCtFuture)
			m_spTicker->m_enType= enFUT;

		m_spTicker->m_sExchange = "";
	}
	return m_spTicker;
};
//------------------------------------------------------------------------//
void 
CAbstractContract::AssignGREEKS(GREEKS& resultGreeks, CRisksPtr Risks){
	Risks->fill_by(resultGreeks);
	Risks->fill_by(m_spQuote);
	Risks->dtUpdateDate = EgLib::vt_date::GetCurrentDate(false);
};
//------------------------------------------------------------------------//
/*virtual*/
double 
CAbstractContract::getPriceMid(){	
	double	dPrice = 0;

	try{

		IEtsPriceProfileAtomPtr spPriceProfile = getPriceProfile();

		if (static_cast<bool>(spPriceProfile)){

			if ( m_dManualPrice > 0. ){
				return m_dManualPrice;
			}
			else{

				//set to global
				CCalculationSettingsPtr spUsedSettings = 
					CCalculationSettingsSingleton::Instance();

				//get price according to price profile
				spPriceProfile->GetUndPriceMid(	m_spQuote->BidPrice, 
												m_spQuote->AskPrice,
												m_spQuote->LastPrice,
												spUsedSettings->GetPriceTolerance(),	
												static_cast<EtsPriceRoundingRuleEnum>(spUsedSettings->GetPriceRoundingRule()), 
												NULL, 
												VARIANT_FALSE,
												&dPrice);


				if (dPrice < 0./*DBL_EPSILON*/){
					std::string description =	"No contract price for: " 
						+ std::string(GetTicker()->m_sSymbol);

					throw	IvRmCore::IvRmException::NoContractPrice(description);
				}
			}
		}
	}
	catch (const IvRmException::AbstractException& err){
		throw err;
	}
	catch (_com_error err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw err;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return dPrice;
};
//----------------------------------------------------------------------------------------//
/*virtual*/
double
CAbstractContract::getPriceBid(){	
	double	dPrice = 0;

	try{

		IEtsPriceProfileAtomPtr spPriceProfile = getPriceProfile();

		if (static_cast<bool>(spPriceProfile)){

			if ( m_dManualPrice > 0. ){
				return m_dManualPrice;
			}
			else{

				//set to global
				CCalculationSettingsPtr spUsedSettings = 
					CCalculationSettingsSingleton::Instance();

				//get price according to price profile
				spPriceProfile->GetUndPriceBid(	m_spQuote->BidPrice, 
												m_spQuote->AskPrice,
												m_spQuote->LastPrice,
												spUsedSettings->GetPriceTolerance(),	
												static_cast<EtsPriceRoundingRuleEnum>(spUsedSettings->GetPriceRoundingRule()), 
												NULL,
												&dPrice);

				if (dPrice < 0./*DBL_EPSILON*/){
					std::string description =	"No contract price Bid for: " 
						+ std::string(GetTicker()->m_sSymbol);

					throw	IvRmCore::IvRmException::NoContractPrice(description);
				}
			}
		}
	}
	catch (const IvRmException::AbstractException& err){
		throw err;
	}
	catch (_com_error err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw err;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return dPrice;
};
//----------------------------------------------------------------------------------------//
/*virtual*/
double
CAbstractContract::getPriceAsk(){

	double	dPrice = 0;

	try{

		IEtsPriceProfileAtomPtr spPriceProfile = getPriceProfile();

		if (static_cast<bool>(spPriceProfile)){

			if ( m_dManualPrice > 0. ){
				return m_dManualPrice;
			}
			else{

				//set to global
				CCalculationSettingsPtr spUsedSettings = 
					CCalculationSettingsSingleton::Instance();

				//get price according to price profile
				spPriceProfile->GetUndPriceAsk(	m_spQuote->BidPrice, 
												m_spQuote->AskPrice,
												m_spQuote->LastPrice,
												spUsedSettings->GetPriceTolerance(),	
												static_cast<EtsPriceRoundingRuleEnum>(spUsedSettings->GetPriceRoundingRule()), 
												NULL,
												&dPrice);
				
				if (dPrice < 0./*DBL_EPSILON*/){
					std::string description =	"No contract price Ask for: " 
						+ std::string(GetTicker()->m_sSymbol);

					throw	IvRmCore::IvRmException::NoContractPrice(description);
				}
			}
		}
	}
	catch (const IvRmException::AbstractException& err){
		throw err;
	}
	catch (_com_error err){
		TRACE_COM_ERROR(err);
		ATLASSERT(FALSE);
		throw err;
	}
	catch (...){
		TRACE_UNKNOWN_ERROR();
		ATLASSERT(FALSE);
		throw;
	}
	return dPrice;
};
//------------------------------------------------------------------------//
/*virtual*/
IEtsPriceProfileAtomPtr
CAbstractContract::getPriceProfile(){
	IEtsPriceProfileAtomPtr spPriceProfile = NULL;
	if (m_spAsset){
		m_spAsset->get_UndPriceProfile(&spPriceProfile);
	}
	return spPriceProfile;
};
//------------------------------------------------------------------------//
/*virtual*/
double
CAbstractContract::getContractSizeInAsset(){
	return 1.;
};
//------------------------------------------------------------------------//
/*virtual*/
double
CAbstractContract::getPricingUnit(){
	return 1.;
};
//------------------------------------------------------------------------//