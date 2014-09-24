#pragma once
#ifndef __EODCALCOPTTHEOCLOSEPRICE_H__
#define __EODCALCOPTTHEOCLOSEPRICE_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodStepCalcOptTheoClosePrice :
	public CEodStepProcessorBase
{
public:
	CEodStepCalcOptTheoClosePrice(void) {};
	virtual ~CEodStepCalcOptTheoClosePrice(void) {} ;

	DWORD Stop();
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();

private:
	EODMS::CMsUndByIDCollPtr _PrepareUndCollection(EODMS::CMsUndByIDCollPtr pUndColl);
	size_t _GetOptionsCount(EODMS::CMsUndByIDCollPtr pUndColl);
	size_t _GetFutureOptionsCount(EODMS::CMsUndByIDCollPtr pUndColl);
	void _SaveOptionClosePrice(EODMS::CMsUndPtr pUnd);
	void _SaveFutureOptionClosePrice(EODMS::CMsIndexPtr pIndex);
	void _IncrementProgress(size_t nAllUnderlyings, size_t& nUndCount);
};
typedef boost::shared_ptr<CEodStepCalcOptTheoClosePrice> CEodStepCalcOptTheoClosePricePtr;

};

#endif// __EODCALCOPTTHEOCLOSEPRICE_H__
