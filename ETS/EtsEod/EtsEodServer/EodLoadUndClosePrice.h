#pragma once
#ifndef __EODLOADUNDCLOSEPRICE_H__
#define __EODLOADUNDCLOSEPRICE_H__

#include "EodLoadUndClosePriceFromIv.h"
#include "EodLoadUndClosePriceFromPp.h"
#include "EodLoadFutClosePriceFromPp.h"
#include "EodLoadClosePriceStage.h"

namespace EOD
{

class CEodStepLoadUndClosePrice :
	public CEodLoadUndClosePriceFromIv,
	public CEodLoadUndClosePriceFromPp,
	public CEodLoadFutClosePriceFromPp
{
public:
	CEodStepLoadUndClosePrice(void) : m_enCurrentStage(enEodCpInvalid){};
	virtual ~CEodStepLoadUndClosePrice(void) {};

	DWORD Stop();
	void Init(bool bLoadFromIv);
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();

private:
	EodLoadClosePriceStage	m_enCurrentStage;
};

typedef boost::shared_ptr<CEodStepLoadUndClosePrice> CEodStepLoadUndClosePricePtr;

};

#endif// __EODLOADUNDCLOSEPRICE_H__
