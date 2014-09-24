#ifndef __MSFUTURE_H__
#define __MSFUTURE_H__

#include "MsContract.h"
#include "MsFutureOption.h"
namespace EODMS
{

	/////////////////////////////////////////////////////////////////////////////
	//
	class  CMsFuture : public CMsContract
	{
	public:
		CMsFuture(void)
			: m_spFuturesOpt(new CMsFutureOptionColl)
			, m_dtMaturity(0L)
			, m_bIsActive(false)
			, m_bMsUpdated(false)
			, m_dPos(BAD_DOUBLE_VALUE)
			, m_bClosePriceUpdated(false)
			, m_nIsAmerican(1)
		{
		}

		~CMsFuture(void)
		{
		}
		
		SYMBOL_TYPE Type() const
		{
			return enStFuture;
		}

		CMsFutureOptionCollPtr Opt()
		{
			return m_spFuturesOpt;
		}

		CMsFutureOptionPtr Opt(const CString& sOptSymbol)
		{
			return GetCollectionItem<CMsFutureOptionPtr>(m_spFuturesOpt, sOptSymbol);
		}

		long ApplyRates(CMsRatePtr spRate)
		{
			Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
			if(spRate!= NULL)
			{
				bool bUseMidRates = true;

				if(spRate->m_enRule == enMsRrRateBasedOnPosition
				&& !EODCO::IsBadValue(m_dPos) && EODCO::DoubleNEQZero(spRate->m_dPosThreshold))
				{
					double dPosThreshold = fabs(spRate->m_dPosThreshold);
					bUseMidRates = (fabs(m_dPos) < fabs(spRate->m_dPosThreshold));
				}

				long nToday = static_cast<long>(floor(COleDateTime::GetCurrentTime()));

				CMsFutureOptionPtr pFutOpt;
				long nExpiration = 0;
				if (m_spFuturesOpt != NULL)
				{
					if (m_spFuturesOpt->size() > 0)
					{
						for (CMsFutureOptionColl::iterator itrOpt = m_spFuturesOpt->begin(); itrOpt != m_spFuturesOpt->end(); itrOpt++)
						{
							pFutOpt = itrOpt->second;
							if (pFutOpt != NULL)
							{
								nExpiration = static_cast<long>(floor(pFutOpt->m_dtExpiration));

								if(bUseMidRates)
								{
									pFutOpt->m_dRate = spRate->NeutralRate(nToday, nExpiration);
								}
								else
								{
									if(m_dPos < 0.)
										pFutOpt->m_dRate = spRate->ShortRate(nToday, nExpiration);
									else
										pFutOpt->m_dRate = spRate->LongRate(nToday, nExpiration);
								}


							} //if (pFutOpt != NULL)

						} // for 
					} //if (m_spFuturesOpt->size() > 0)
				} // if (m_spFuturesOpt != NULL)
			} // if(spRate!= NULL)
			Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
			return 0;
		} //ApplyRates

	public:
		bool			m_bIsActive;
		DATE			m_dtMaturity;
		bool			m_bMsUpdated;
		double			m_dPos;
		bool			m_bClosePriceUpdated;
		long			m_nIsAmerican;
	protected:
		CMsFutureOptionCollPtr	m_spFuturesOpt; // options with positions
	};

	typedef boost::shared_ptr<CMsFuture> CMsFuturePtr;
	typedef std::map<CString, CMsFuturePtr>	CMsFutureColl;
	typedef std::pair<CString, CMsFuturePtr>	CMsFuturePair;

	typedef boost::shared_ptr<CMsFutureColl>	CMsFutureCollPtr;
	typedef std::map<long, CMsFuturePtr>	CMsFutureByIdColl;
	typedef std::pair<long, CMsFuturePtr>	CMsFutureByIdPair;

	typedef boost::shared_ptr<CMsFutureByIdColl>	CMsFutureByIdCollPtr;

};

#endif //__MSFUTURE_H__