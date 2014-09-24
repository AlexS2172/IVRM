#pragma once
#ifndef __EODLOADAVGUNDVOLUMES_H__
#define __EODLOADAVGUNDVOLUMES_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodStepLoadAvgUndVolumes : 
	public CEodStepProcessorBase, 
	public EODIV::CIvDataLoader<EODIV::CIvAvrgUndVolumesRequest,EODIV::CIvAvrgUndVolumesResultPtr>
{
public:
	CEodStepLoadAvgUndVolumes(void) {};
	virtual ~CEodStepLoadAvgUndVolumes(void) {};

	DWORD Stop();
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();
	virtual void	OnIvData(EODIV::CIvAvrgUndVolumesResultPtr pResult);

protected:
	virtual const HANDLE _StopEventHandle() { return GetStopEventHandle(); }
	virtual EodStepsMaskEnum _StepMask() { return StepMask(); }
private:
	EODIV::CIvAvrgUndVolumesRequest	m_Request;
};

typedef boost::shared_ptr<CEodStepLoadAvgUndVolumes> CEodStepLoadAvgUndVolumesPtr;

};

#endif// __EODLOADAVGUNDVOLUMES_H__