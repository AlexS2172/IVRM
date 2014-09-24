#pragma once

class Analytics
{
public:
	long qty;
	long qty_in_asset;

	long qty_ltd_buy;
	long qty_ltd_sell;
	double ammount_ltd_buy;
	double ammount_ltd_sell;

	long qty_prev_buy;
	long qty_prev_sell;
	double ammount_prev_buy;
	double ammount_prev_sell;

	long qty_today_buy;
	long qty_today_sell;
	double ammount_today_buy;
	double ammount_today_sell;

	Analytics();
	void clone(Analytics* anx);

	double values[32];
	
	typedef std::map<std::string, long> variables;
	typedef variables::iterator variables_iterator;
	static variables& get_variables();
};
