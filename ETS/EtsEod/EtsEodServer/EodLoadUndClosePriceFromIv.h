#pragma once
#ifndef __EODLOADUNDCLOSEPRICEFROMIV_H__
#define __EODLOADUNDCLOSEPRICEFROMIV_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodLoadUndClosePriceFromIv : 
	virtual public CEodStepProcessorBase, 
	public EODIV::CIvDataLoader<EODIV::CIvUndClosePriceRequest,EODIV::CIvUndClosePriceResultPtr>
{
public:
	CEodLoadUndClosePriceFromIv(void) : m_nUndLoaded(0L) {};
	virtual ~CEodLoadUndClosePriceFromIv(void) {};
	
	void LoadFromIv();
	virtual void	OnIvData(EODIV::CIvUndClosePriceResultPtr pResult);

protected:
	virtual const HANDLE _StopEventHandle() { return GetStopEventHandle(); }
	virtual EodStepsMaskEnum _StepMask() { return StepMask(); }
private:
	EODMS::CMsUndCollPtr _PrepareUndCollection(EODMS::CMsUndCollPtr pUndColl);
	long _GetUpdatedUnderlyings(EODMS::CMsUndCollPtr pUndColl);
	void _IncrementProgress(EODMS::CMsUndCollPtr pUndColl);

private:
	EODIV::CIvUndClosePriceRequest	m_Request;
	long							m_nUndLoaded;
};

};

#endif// __EODLOADUNDCLOSEPRICEFROMIV_H__