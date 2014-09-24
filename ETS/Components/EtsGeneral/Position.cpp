#include "StdAfx.h"
#include "AbstractContract.h"
#include "Position.h"
#include "Analytics.h"
#include "Trade.h"
#include "AccountItem.h"

Position::Position(void):
_id(BAD_LONG_VALUE),
_time_stamp(0.){
	this->analytics = new Analytics;
};

Position::~Position(void){
	delete analytics;
};

Position::Position(AccountItem *account_item, TradeKey *trade_key){
	this->trade_key = trade_key;
	this->account_item = account_item;
	this->analytics = new Analytics;
};

AccountItem*
Position::get_account_item(){
	return account_item;
};

void
Position::set_account_item(AccountItem* account_item){
	this->account_item = account_item;
};

void
Position::set_trade_key(TradeKey* trade_key){
	this->trade_key = trade_key;
};

Analytics*
Position::get_analytics(){
	return analytics;
};

long&
Position::id(){
	return _id;
};

double&
Position::time_stamp(){
	return _time_stamp;
};

Trade* 
Position::get_trade(Trade* trade){
	container_key key = trade_key->get_key(trade);
	trade_iterator it = trade_table.find(key);
	if (it != trade_table.end()){
		return it->second;
	};
	return 0;
};

Trade* 
Position::add_trade(Trade* trade){
	container_key key = trade_key->get_key(trade);
	trade_table[key] = trade;
	return trade;
};

void 
Position::delete_trade(Trade* trade){
	container_key key = trade_key->get_key(trade);
	trade_iterator it = trade_table.find(key);
	if (it != trade_table.end()){
		trade_table.erase(it);
	};
};

bool 
Position::is_empty(){
	return trade_table.empty();
};

void 
Position::process_trade(Trade* trade){
	if (trade){
		if (trade->action() == Trade::enNewTrade){
			add_trade(trade);
		}
		else if (!is_empty()){
			delete_trade(trade);
		}
	}		
};

/*virtual */
void 
Position::calculate(){

	CAbstractContract* contract = account_item->get_contract();
	double pricing_unit = contract->getPricingUnit();
	double contract_size_in_asset = contract->getContractSizeInAsset();
	CRisksPtr contract_risks = contract->m_spRisks;
	double qty = analytics->qty;
	double vega_weight = 1.;
	
	double base_contract_price = contract_risks->dBaseContractPrice * pricing_unit;
	double contract_price_bid = contract_risks->dContractPriceBid;
	double contract_price_ask = contract_risks->dContractPriceAsk;
	double contract_price_mid = contract_risks->dContractPrice;

	if(!bad_value(contract_risks->dDelta)){
		analytics->values[0/*DeltaInShares*/] = contract_risks->dDelta * qty * contract_size_in_asset;
		analytics->values[1/*DeltaEq*/] = analytics->values[0/*DeltaInShares*/] * base_contract_price;	
	};

	if(!bad_value(contract_risks->dGamma)){
		analytics->values[2/*GammaInShares*/] = contract_risks->dGamma * qty * contract_size_in_asset;
		analytics->values[3/*GammaInSharesPerc*/] = contract_risks->dGamma * qty * contract_size_in_asset * base_contract_price / 100.;
		analytics->values[4/*NetGamma*/] = contract_risks->dGamma * qty * contract_size_in_asset * base_contract_price * base_contract_price / 100.;
	};

	if(!bad_value(contract_risks->dVega)){
		analytics->values[5/*VegaInShares*/] = contract_risks->dVega * qty * contract_size_in_asset;
		analytics->values[6/*WtdVega*/] = analytics->values[5/*VegaInShares*/] * vega_weight;
	};

	if(!bad_value(contract_risks->dTheta)){
		analytics->values[7/*ThetaInShares*/] = contract_risks->dTheta * contract_size_in_asset * qty;
	};

	if(!bad_value(contract_risks->dRho)){
		analytics->values[8/*RhoInShares*/] = contract_risks->dRho * contract_size_in_asset * qty;
	};

	if(!bad_value(contract_risks->dDeltaVega)){
		analytics->values[9/*VegaDeltaInShares*/] =	contract_risks->dDeltaVega * contract_size_in_asset * qty;
	};

	if(!bad_value(contract_risks->dDeltaTheta)){
		analytics->values[10/*ThetaDeltaInShares*/] = contract_risks->dDeltaTheta * contract_size_in_asset * qty;
	};

	if (!bad_value(contract_risks->dGammaTheta)){
		analytics->values[11/*ThetaGammaInShares*/] = contract_risks->dGammaTheta * contract_size_in_asset * 
														qty * base_contract_price / 100.;
	};

	if (!bad_value(contract_risks->dGammaVega)){
		analytics->values[12/*VegaGammaInShares*/] = contract_risks->dGammaVega * contract_size_in_asset * 
													qty  * base_contract_price / 100.;
	};
};
