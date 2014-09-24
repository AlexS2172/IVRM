#pragma once
#ifndef __EODCALCOCOMMVOLA_H__
#define __EODCALCOCOMMVOLA_H__

#include "EodStepProcessorBase.h"

namespace EOD
{

	class CEodStepCalcCommVola :
		public CEodStepProcessorBase
	{
	public:
		CEodStepCalcCommVola(void) {};
		virtual ~CEodStepCalcCommVola(void) {} ;

		DWORD Stop();
		virtual EodStepsMaskEnum GetStartMask();
		virtual EodStepsMaskEnum StepMask();
		virtual void	EodStepMainFunc();
		

	private:

		void _IncrementProgress(size_t nAllUnderlyings, size_t& nUndCount);
		double	_GetContractClosePrice(long nContractID, CDBConnection * pDB);
		long	_FutureOptsCalcFutureOptionVola(long nFutureID, EODMS::CMsFutureOptionCollPtr pFutOpts, double dFutureClosePrice, CDBConnection * pDB, EOD::CEodMktStructureMgrPtr pMktStrMngrPtr, EodCalcModelTypeEnum enModel, long nIsAmerican);
		double	_FutOptCalcFutureOptionVolatility(double dRate, double dFuturePrice, double dOptionPrice, double dStrike, DATE nDTE, long nIsCall, long nIsAmerican, long nModel);
		long	SaveContractVola(CDBConnection * pDB, long nContractID, double dStrike, DATE dtExpiry, double dVola );
	};
	typedef boost::shared_ptr<CEodStepCalcCommVola> CEodStepCalcCommVolaPtr;
};

#endif	// __EODCALCOCOMMVOLA_H__
