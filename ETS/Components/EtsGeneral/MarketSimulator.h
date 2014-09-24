#pragma once

class CAbstractContract;
class CSpotContract;
class COptionContract;
class CSpotOptionContract;
class CFutureContract;
class CFutureOptionContract;
class CAsset;

#include "boost/shared_ptr.hpp"
#include "Curve.h"

//--------------------------------------------------------------//
class Shift{
public:
	typedef boost::shared_ptr<Shift> ptr;

	virtual
	double
	getShift(CAbstractContract* contract) = 0;
};

class DefaultShift: public Shift{
public:
	double default_shift;

	DefaultShift(double default_shift);

	/*virtual*/
	double
	getShift(CAbstractContract* contract);
};

class ShiftDecorator: public Shift{
public:
	ShiftDecorator(Shift::ptr shift);

	virtual
	~ShiftDecorator();
	
	/*virtual*/
	double
	getShift(CAbstractContract* contract);

	Shift::ptr
	setDecorator(Shift::ptr decorator);

protected:
	Shift::ptr shift;
};

class WeightedShiftDecorator: public ShiftDecorator{
public:
	WeightedShiftDecorator(Shift::ptr shift);

	/*virtual*/
	double
	getShift(CAbstractContract* contract);
};

class SpecificShiftDecorator: public ShiftDecorator{
public:
	typedef std::string	container_key;
	typedef std::map<container_key, double> value_container;

	SpecificShiftDecorator(Shift::ptr shift, long shift_type);
	~SpecificShiftDecorator();

	void
	setValue(container_key key, double value);

	virtual
	container_key
	getKey(CAbstractContract* contract);
	
	double
	getValue(container_key key);

	/*virtual*/
	double
	getShift(CAbstractContract* contract);

	void
	clear();

private:
	value_container container; 
	long shift_type;
};
//--------------------------------------------------------------//
class Scenario{
public:
	typedef boost::shared_ptr<Scenario> ptr;

	Scenario();
	
	virtual
	~Scenario();

	virtual 
	double	getShift(	CSpotContract& contract, 
						double term = 0.,
						Interpolation interpolation = FLAT_FORWARD_INTERPOLATION,
						Extrapolation extrapolation = FLAT_EXTRAPOLATION);

	virtual
	double	getShift(CSpotOptionContract& contract);

	virtual
	double	getShift(CFutureContract& contract);

	virtual
	double	getShift(CFutureOptionContract& contract);

	void
	setShiftUnit(long ShiftUnit);

	long
	getShiftUnit();

	virtual
	double
	getAbstractShiftValue(CAbstractContract* contract);

	virtual
	double
	getShift(double value, double shift);

	void
	setShifter(Shift::ptr shifter);

protected:
	Shift::ptr shifter;

	double	m_defaultShift;
	long	m_shiftUnit;

	virtual
	double
	get_shift(	CAbstractContract* contract, 
				double term = 0.,
				Interpolation interpolation = FLAT_FORWARD_INTERPOLATION,
				Extrapolation extrapolation = FLAT_EXTRAPOLATION);
};
//--------------------------------------------------------------//
#define SHIFT_VALUE (0)
#define SHIFT_RATIO (1)

#define SHIFT_UNIT_ABSOLUTE (0)
#define SHIFT_UNIT_PERCENT (1)
//--------------------------------------------------------------//
class CorrelatedPriceScenario: public Scenario{
public:
	CorrelatedPriceScenario();
	~CorrelatedPriceScenario();

	void
	setShiftDriverAsset(CAsset* Asset);

	CAsset*
	getShiftDriverAsset();

	/*virtual*/
	double
	getShift(CSpotOptionContract& contract);

	/*virtual*/
	double
	getShift(CFutureOptionContract& contract);

private:
	CAsset* m_shiftDriverAsset;
	
	CAsset* 
	get_contract_asset(CAbstractContract* contract);

	double
	get_driver_price();

	double
	get_correlation(CAbstractContract* contract);

protected:

	/*virtual*/
	double
	get_shift(	CAbstractContract* contract, 
				double term = 0.,
				Interpolation interpolation = FLAT_FORWARD_INTERPOLATION,
				Extrapolation extrapolation = FLAT_EXTRAPOLATION);
};
//--------------------------------------------------------------//
class GeneralPriceScenario: public Scenario{
public:
	GeneralPriceScenario();
	~GeneralPriceScenario();

	/*virtual*/
	double
	getShift(CFutureOptionContract& contract);

	/*virtual*/
	double
	getShift(CSpotOptionContract& contract);

protected:
	/*virtual*/
	double
	get_shift(	CAbstractContract* contract, 
				double term = 0.,
				Interpolation interpolation = FLAT_FORWARD_INTERPOLATION,
				Extrapolation extrapolation = FLAT_EXTRAPOLATION);
};
//--------------------------------------------------------------//
class GeneralTimeScenario: public Scenario{
public:
	GeneralTimeScenario();

protected:
	/*virtual*/
	double
	get_shift(	CAbstractContract* contract, 
				double term = 0.,
				Interpolation interpolation = FLAT_FORWARD_INTERPOLATION,
				Extrapolation extrapolation = FLAT_EXTRAPOLATION);
};
//--------------------------------------------------------------//
class GeneralVolatilityScenario: public Scenario{
public:
	GeneralVolatilityScenario();
	~GeneralVolatilityScenario();

	/*virtual*/
	double
	getShift(CFutureContract& contract);

	/*virtual*/
	double
	getShift(CSpotContract& contract);

protected:
	/*virtual*/
	double
	get_shift(	CAbstractContract* contract, 
				double term = 0.,
				Interpolation interpolation = FLAT_FORWARD_INTERPOLATION,
				Extrapolation extrapolation = FLAT_EXTRAPOLATION);
};
//--------------------------------------------------------------//
class CMarketSimulator
{
public:

	typedef boost::shared_ptr<CMarketSimulator> ptr;

	CMarketSimulator(void);

	virtual 
	~CMarketSimulator(void);

	virtual
	double	getPrice(CSpotContract& contract);

	virtual
	double	getPrice(CSpotOptionContract& contract);

	virtual
	double	getPrice(CFutureContract& contract);

	virtual
	double	getPrice(CFutureOptionContract& contract);

	virtual
	double	getPriceShift(	CSpotContract& contract, 
							double term = 0.,
							Interpolation interpolation = FLAT_FORWARD_INTERPOLATION,
							Extrapolation extrapolation = FLAT_EXTRAPOLATION);

	virtual
	double	getPriceShift(CSpotOptionContract& contract);

	virtual
	double	getPriceShift(CFutureContract& contract);

	virtual
	double	getPriceShift(CFutureOptionContract& contract);

	virtual
	double	getVolatility(CSpotOptionContract& contract);

	virtual
	double	getVolatility(CFutureOptionContract& contract);

	virtual
	double	getTime(CFutureOptionContract& contract);

	virtual
	double	getTime(CSpotOptionContract& contract);

	static
	CMarketSimulator::ptr	getDefault();
	
	void
	setTimeScenario(Scenario::ptr scenario_ptr);

	void
	setPriceScenario(Scenario::ptr scenario_ptr);

	void
	setVolatilityScenario(Scenario::ptr scenario_ptr);

private:
	Scenario::ptr m_timeScenario;
	Scenario::ptr m_priceScenario;
	Scenario::ptr m_volatilityScenario;
};
//--------------------------------------------------------------//