#pragma once
#ifndef __EODLOADOPTCLOSEPRICE_H__
#define __EODLOADOPTCLOSEPRICE_H__

#include "EodLoadOptClosePriceFromIv.h"
#include "EodLoadOptClosePriceFromPp.h"
#include "EodLoadFutOptClosePriceFromPp.h"
#include "EodLoadClosePriceStage.h"

namespace EOD
{

class CEodStepLoadOptClosePrice :
	public CEodLoadOptClosePriceFromIv,
	public CEodLoadOptClosePriceFromPp,
	public CEodLoadFutOptClosePriceFromPp
{
public:
	CEodStepLoadOptClosePrice(void) : m_enCurrentStage(enEodCpInvalid){};
	virtual ~CEodStepLoadOptClosePrice(void) {};

	DWORD Stop();
	void Init(bool bLoadFromIv);
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();

private:
	EodLoadClosePriceStage	m_enCurrentStage;
};

typedef boost::shared_ptr<CEodStepLoadOptClosePrice> CEodStepLoadOptClosePricePtr;

};

#endif// __EODLOADOPTCLOSEPRICE_H__
