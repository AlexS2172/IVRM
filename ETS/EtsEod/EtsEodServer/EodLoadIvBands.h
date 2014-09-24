#pragma once
#ifndef __EODLOADIVBANDS_H__
#define __EODLOADIVBANDS_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

class CEodStepLoadIvBands : 
	public CEodStepProcessorBase, 
	public EODIV::CIvDataLoader<EODIV::CIvIvBandsRequest,EODIV::CIvIvBandsResultPtr>
{
public:
	CEodStepLoadIvBands(void)  : m_nUndInQuery(0L), m_sQueryBuffer(_T("")) {};
	virtual ~CEodStepLoadIvBands(void) {};

	DWORD Stop();
	virtual EodStepsMaskEnum GetStartMask();
	virtual EodStepsMaskEnum StepMask();
	virtual void	EodStepMainFunc();
	virtual void	OnIvData(EODIV::CIvIvBandsResultPtr pResult);

protected:
	virtual const HANDLE _StopEventHandle() { return GetStopEventHandle(); }
	virtual EodStepsMaskEnum _StepMask() { return StepMask(); }

private:
	void _SaveBuffer();

private:
	EODIV::CIvIvBandsRequest	m_Request;
	CDBConnection		m_spDB;
	CString						m_sQueryBuffer;
	long						m_nUndInQuery;
};

typedef boost::shared_ptr<CEodStepLoadIvBands> CEodStepLoadIvBandsPtr;

};

#endif// __EODLOADIVBANDS_H__