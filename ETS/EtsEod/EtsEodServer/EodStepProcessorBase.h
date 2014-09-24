#pragma once
#ifndef __EODSTEPPROCESSORBASE_H__
#define __EODSTEPPROCESSORBASE_H__

#include "EodGeneral.h"
#include "EtsEodServer.h"

namespace EOD
{

enum CEodStepStatus
{
	enSsInactive,
	enSsInProgress,
	enSsSuccessCompleted, 
	enSsErrorCompleted,
	enSsStopped
};

class CEodStepProcessorBase : 
	virtual public CThread, 
	 
	virtual public EODCO::CEgReportable,
	virtual public EODCO::CEgTraceable
{
public:
	CEodStepProcessorBase(void) : m_enStatus(enSsInactive), m_nProgress(0L) {};
	virtual ~CEodStepProcessorBase(void)
	{
		Stop();
	};
	
	size_t			GetProgress();
	CEodStepStatus	Status();
	const HANDLE	GetStepHandle() const;

	virtual EodStepsMaskEnum StepMask() = 0;
	virtual EodStepsMaskEnum GetStartMask() = 0;
protected:
	virtual void	EodStepMainFunc() = 0;

protected:
	UINT  Run();

	CLock			m_lockMembers;
	CEodStepStatus	m_enStatus;
	size_t			m_nProgress;
	CDBConnection	m_spDB;
};

typedef boost::shared_ptr<CEodStepProcessorBase>				CEodStepProcessorBasePtr;
typedef std::map<EodStepsMaskEnum, CEodStepProcessorBasePtr>	CEodStepProcessorBaseColl;
typedef CEodStepProcessorBaseColl::value_type					CEodStepProcessorBasePair;

typedef boost::shared_ptr<CEodStepProcessorBaseColl>			CEodStepProcessorBaseCollPtr;

};

#endif//__EODSTEPPROCESSORBASE_H__

