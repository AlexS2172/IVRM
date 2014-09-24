#include "StdAfx.h"
#include "Analytics.h"

Analytics::Analytics(){
	qty = qty_ltd_buy = qty_ltd_sell = qty_in_asset = BAD_LONG_VALUE;
	qty_prev_buy = qty_prev_sell = BAD_LONG_VALUE;
	qty_today_buy = qty_today_sell = BAD_LONG_VALUE;

	ammount_ltd_buy = ammount_ltd_sell = 0.;
	ammount_prev_buy = ammount_prev_sell = 0.;
	ammount_today_buy = ammount_today_sell = 0.;

	for (int i = 0; i < 32; i++){
		values[i] = BAD_DOUBLE_VALUE;
	};
};

void 
Analytics::clone(Analytics* anx){
	qty = anx->qty;
	qty_in_asset = anx->qty_in_asset;

	qty_ltd_buy = anx->qty_ltd_buy;
	qty_ltd_sell = anx->qty_ltd_sell;
	ammount_ltd_buy = anx->ammount_ltd_buy;
	ammount_ltd_sell = anx->ammount_ltd_sell;

	qty_prev_buy = anx->qty_prev_buy;
	qty_prev_sell = anx->qty_prev_sell;
	ammount_prev_buy = anx->ammount_prev_buy;
	ammount_prev_buy = anx->ammount_prev_buy;

	qty_today_buy = anx->qty_today_buy;
	qty_today_sell = anx->qty_today_sell;
	ammount_today_buy = anx->ammount_today_buy;
	ammount_today_sell = anx->ammount_today_sell;
};

/*static*/
Analytics::variables&
Analytics::get_variables(){
	static variables v;
	long idx = 0;
	
	v["DeltaInShares"] = idx++;
	v["DeltaEq"] = idx++;
	v["GammaInShares"] = idx++;
	v["GammaInSharesPerc"] = idx++;
	v["NetGamma"] = idx++;
	v["VegaInShares"] = idx++;
	v["WtdVega"] = idx++;
	v["ThetaInShares"] = idx++;
	v["RhoInShares"] = idx++;
	v["VegaDeltaInShares"] = idx++;
	v["ThetaDeltaInShares"] = idx++;
	v["ThetaGammaInShares"] = idx++;
	v["VegaGammaInShares"] = idx++;

	return v;
};
