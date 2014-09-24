#pragma once
#ifndef __EODLOADUNDINDEXBETA_H__
#define __EODLOADUNDINDEXBETA_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodStepLoadUndIndexBeta : 
	public CEodStepProcessorBase, 
	public EODIV::CIvDataLoader<EODIV::CIvUndIndexBetaRequest,EODIV::CIvUndIndexBetaResultPtr>
{
public:
	CEodStepLoadUndIndexBeta(void) {};
	virtual ~CEodStepLoadUndIndexBeta(void) {};

	DWORD Stop();
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();
	virtual void	OnIvData(EODIV::CIvUndIndexBetaResultPtr pResult);

protected:
	virtual const HANDLE _StopEventHandle() { return GetStopEventHandle(); }
	virtual EodStepsMaskEnum _StepMask() { return StepMask(); }
private:
	EODMS::CMsIndexCollPtr _GetIndexColl(EODMS::CMsUndCollPtr pUndColl);

private:
	EODIV::CIvUndIndexBetaRequest	m_Request;
};

typedef boost::shared_ptr<CEodStepLoadUndIndexBeta> CEodStepLoadUndIndexBetaPtr;

};

#endif// __EODLOADUNDINDEXBETA_H__
