#pragma once
#ifndef __EODLOADOPTCLOSEPRICEFROMIV_H__
#define __EODLOADOPTCLOSEPRICEFROMIV_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodLoadOptClosePriceFromIv : 
	virtual public CEodStepProcessorBase, 
	public EODIV::CIvDataLoader<EODIV::CIvOptClosePriceRequest, EODIV::CIvOptClosePriceResultPtr>
{
public:
	CEodLoadOptClosePriceFromIv(void) : m_nUndLoaded(0L) {};
	virtual ~CEodLoadOptClosePriceFromIv(void) 
	{
		m_spCurrentUnd ;
	};

	virtual void	LoadFromIv();
	virtual void	OnIvData(EODIV::CIvOptClosePriceResultPtr pResult);

protected:
	virtual const HANDLE _StopEventHandle() { return GetStopEventHandle(); }
	virtual EodStepsMaskEnum _StepMask() { return StepMask(); }
private:
	EODMS::CMsUndCollPtr _PrepareUndCollection(EODMS::CMsUndCollPtr pUndColl);
	void _GetUpdatedContracts(EODMS::CMsUndCollPtr pUndColl, long& nUndCount, long& nOptCount, long& nTotalOptCount);
	void _IncrementProgress(EODMS::CMsUndCollPtr pUndColl);

private:
	EODIV::CIvOptClosePriceRequest	m_Request;
	long							m_nUndLoaded;
	EODMS::CMsUndPtr				m_spCurrentUnd;
};

};

#endif// __EODLOADOPTCLOSEPRICEFROMIV_H__