#pragma once
#ifndef __EODLOADDEFAULTIV_H__
#define __EODLOADDEFAULTIV_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodStepLoadDefaultIv : 
	public CEodStepProcessorBase, 
	public EODIV::CIvDataLoader<EODIV::CIvDefaultIvRequest,EODIV::CIvDefaultIvResultPtr>
{
public:
	CEodStepLoadDefaultIv(void)  : m_bOverwriteExistingIv(false) {};
	virtual ~CEodStepLoadDefaultIv(void) {};

	void Init(bool bOverwriteExistingIv);

	DWORD Stop();
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();
	virtual void	OnIvData(EODIV::CIvDefaultIvResultPtr pResult);

protected:
	virtual const HANDLE _StopEventHandle() { return GetStopEventHandle(); }
	virtual EodStepsMaskEnum _StepMask() { return StepMask(); }

private:
	EODMS::CMsUndCollPtr _GetUndWithEmtpyIvCurve(EODMS::CMsUndCollPtr pUndColl);

private:
	EODIV::CIvDefaultIvRequest	m_Request;
	CDBConnection		m_spDB;
	bool						m_bOverwriteExistingIv;
};

typedef boost::shared_ptr<CEodStepLoadDefaultIv> CEodStepLoadDefaultIvPtr;

};

#endif// __EODLOADDEFAULTIV_H__
