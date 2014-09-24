#include "StdAfx.h"
#include "EodStepProcessorBase.h"

namespace EOD
{

size_t CEodStepProcessorBase::GetProgress()
{
	CAutoLock lock(m_lockMembers);
	return m_nProgress;
}

CEodStepStatus CEodStepProcessorBase::Status()
{
	CAutoLock lock(m_lockMembers);
	return m_enStatus;
}

const HANDLE CEodStepProcessorBase::GetStepHandle() const
{
	return GetThreadHandle();
}

UINT  CEodStepProcessorBase::Run()
{
	::CoInitializeEx(NULL,COINIT_MULTITHREADED);

	{
		CAutoLock lock(m_lockMembers);
		m_enStatus = enSsInProgress;
	}
	try
	{
	
		EodStepMainFunc();
		CAutoLock lock(m_lockMembers);
		m_enStatus = enSsSuccessCompleted;
	}
	catch(EODCO::CEodStopException)
	{
		Report(StepMask(),enRpMtFinishWithError, _T("Forcedly stopped."));
		CAutoLock lock(m_lockMembers);
		m_enStatus = enSsStopped;
	}
	catch(exception& e)
	{
		Report(StepMask(),enRpMtFinishWithError, _T("Completed with exception: %s"), e.what());
		CAutoLock lock(m_lockMembers);		
		m_enStatus = enSsErrorCompleted;
	}
	catch(_com_error& err)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), err.Error(), (LPCTSTR)err.Description());
		Report(StepMask(),enRpMtFinishWithError, _T("Completed with ComError exception: %d"));
		CAutoLock lock(m_lockMembers);		
		m_enStatus = enSsErrorCompleted;
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Out"));
	::CoUninitialize();
	return static_cast<UINT>(m_enStatus);
}

}