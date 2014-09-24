#pragma once
#ifndef __EODLOADSKEWKURTOSIS_H__
#define __EODLOADSKEWKURTOSIS_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodStepLoadSkewKurtosis : 
	public CEodStepProcessorBase, 
	public EODIV::CIvDataLoader<EODIV::CIvSkewKurtosisRequest,EODIV::CIvSkewKurtosisResultPtr>
{
public:
	CEodStepLoadSkewKurtosis(void) {};
	virtual ~CEodStepLoadSkewKurtosis(void) {};

	DWORD Stop();
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();
	virtual void	OnIvData(EODIV::CIvSkewKurtosisResultPtr pResult);

protected:
	virtual const HANDLE _StopEventHandle() { return GetStopEventHandle(); }
	virtual EodStepsMaskEnum _StepMask() { return StepMask(); }
private:
	EODIV::CIvSkewKurtosisRequest	m_Request;
};

typedef boost::shared_ptr<CEodStepLoadSkewKurtosis> CEodStepLoadSkewKurtosisPtr;

};

#endif// __EODLOADSKEWKURTOSIS_H__
