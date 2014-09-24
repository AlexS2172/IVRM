#pragma once
#ifndef __EODSTEPMANAGER_H__
#define __EODSTEPMANAGER_H__

#include "EodGeneral.h"

namespace EOD
{

class CEodStepManager :
	 
	public EODCO::CEgReportable,
	virtual public EODCO::CEgTraceable
{
public:
	CEodStepManager(void) : m_nAllStepsMask(0L), m_nCompletedStepsMask(0L), m_enStatus(enStStopped), 
		m_spStepsColl(new CEodStepProcessorBaseColl), m_nProgress(0L) {};
	virtual ~CEodStepManager(void) 
	{
		//m_spStepsColl = NULL;
	};

	void Start(unsigned long nStepsMask);
	void Stop();

	void RescheduleSteps(vector<HANDLE>& vecSteps);

	EodStatusEnum Status()
	{
		CAutoLock lock(m_Lock);
		return m_enStatus;
	}
	void Status(EodStatusEnum enStatus)
	{
		CAutoLock lock(m_Lock);
		m_enStatus = enStatus;
	}
	void IncProgress();
private:
	void _CreateStepsColl(unsigned long nStepsMask);
	
	unsigned long _AllStepsMask()
	{
		CAutoLock lock(m_Lock);
		return m_nAllStepsMask;
	}
	void _AllStepsMask(unsigned long nAllStepsMask)
	{
		CAutoLock lock(m_Lock);
		m_nAllStepsMask = nAllStepsMask;
	}
	unsigned long _CompletedStepsMask()
	{
		CAutoLock lock(m_Lock);
		return m_nCompletedStepsMask;
	}
	void _CompletedStepsMask(unsigned long nCompletedStepsMask)
	{
		CAutoLock lock(m_Lock);
		m_nCompletedStepsMask = nCompletedStepsMask;
	}
	void _Progress(unsigned long nProgress)
	{
		CAutoLock lock(m_Lock);
		m_nProgress = nProgress;
	}

private:
	CLock						m_Lock;
	EodStatusEnum						m_enStatus;
	unsigned long						m_nAllStepsMask;
	unsigned long						m_nCompletedStepsMask;
	CEodStepProcessorBaseCollPtr		m_spStepsColl;
	size_t								m_nProgress;
};

typedef boost::shared_ptr<CEodStepManager> CEodStepManagerPtr;

};

#endif// __EODSTEPMANAGER_H__
