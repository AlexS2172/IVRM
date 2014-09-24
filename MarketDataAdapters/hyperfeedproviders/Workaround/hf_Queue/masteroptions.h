// MasterOptions.h : Declaration of the CMasterOptions

#ifndef __MASTEROPTIONS_H__
#define __MASTEROPTIONS_H__

#include "Database.h"
#include "HyperFeedBase.h"
/////////////////////////////////////////////////////////////////////////////
// CMasterOptions
class CMasterOptions : 
	public CDatabase<DBA_MASTER_OPTION>
{
	struct COptionRootInfo
	{
		string					m_sSymbol;
		bool					m_bIsMonthlyExpirationCalendar;
		OptionExersiseType		m_enExerciseType;
		long					m_nMultiplier;
		long					m_nSPC;
		double					m_dCash;
		long					m_nAdditionalUnderlyings;
		string					m_sExchanges;

		typedef struct
		{
			BYTE  expYear;      //  expiration year offset
			BYTE  expMonth;     //  expiration month
		} cycle_t;
		map<CHAR, cycle_t>		m_CyclesMap;
		USHORT					m_usStrikePriceType;
		map<CHAR, LONG>			m_StrikesMap;
	};

	typedef map<string, COptionRootInfo> optionrootmap;

	struct CUnderlyingInfo
	{
		string					m_sSymbol;
		optionrootmap			m_mapRoots;
	};

	typedef map<long, short> expirationsday;
public:
	CMasterOptions();
	virtual ~CMasterOptions();

	long Attach();
protected:
	void LoadOptionFundamentalInfo(BSTR bsUnderlying);
	void EnumOptionableStocks();
	void CancelRequest();

	virtual void PublishUnderlyingLoaded(BSTR bsUnderlying){};
	virtual void PublicGetMasterOptionError(ErrorNumberEnum nError, BSTR bsDescription){};
	virtual void PublicLastMasterOption(){};

	void CopyFundamentalInfoToInfo(_OptionInfo& Info);
	void CopyFundamentalInfoToResult(_OptionResults& Result);
	void CopyFundamentalInfoToResult(_OptionResultsEx& Result);
	void CopyFundamentalInfoToIvResult(_OptionForIvResult& Result);

	typedef vector<_bstr_t> optionlist;
	void RetrieveTheoOptListForCurUnderlying(optionlist &OptList);
	//DATE GetExpDate(const _bstr_t &bsSymbol);
	double GetStrike(const string &sSymbol);
	bool IsCallOption(const string &sSymbol);
	
private:
	bool _FillUnderlyingInfo();
	void _SetUndDbKey(BSTR bsUnderlying);
	void _SetDbKeyToFirstUnd();
	void _SetCalendarDbKey();

	DATE _CorrectExpiryDate(DATE dtExpiryDate);
	void _LoadMonthlyExpirationsDays();
	void _GetExchangesList(const SYMBOL_DEF& recOption, string& strExchanges);
	OptionDeliveryType _GetDeliveryType(const COptionRootInfo& clRoot);

	bool _CheckCanceled();
protected:
	CUnderlyingInfo	m_clCurrentUnderlying;
private:
	expirationsday	m_mapExpiryDays;

	HANDLE m_hCancel;
};
#endif //__MASTEROPTIONS_H__
