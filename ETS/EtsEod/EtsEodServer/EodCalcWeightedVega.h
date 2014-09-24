#pragma once

#ifndef __EODCALCWEIGHTEDVEGA_H__
#define __EODCALCWEIGHTEDVEGA_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodStepCalcWeightedVega :
	public CEodStepProcessorBase
{
public:
	CEodStepCalcWeightedVega(void) {};
	virtual ~CEodStepCalcWeightedVega(void) {} ;

	DWORD Stop();
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();

private:
	long _GetWtdVegaMaturity();
	void _CalculateWtdVega(long nMaturity, EODMS::CMsExpDatePtr pExpDate);
	void _SaveWtdVega(EODMS::CMsExpDatePtr pExpDate);
};
typedef boost::shared_ptr<CEodStepCalcWeightedVega> CEodStepCalcWeightedVegaPtr;

};

#endif// __EODCALCWEIGHTEDVEGA_H__
