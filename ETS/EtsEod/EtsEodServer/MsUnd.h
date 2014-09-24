#ifndef __MSUND_H__
#define __MSUND_H__

#include "MsContract.h"
#include "MsOptRoot.h"
#include "MsOption.h"
#include "MsOptionKey.h"
#include "MsRate.h"
#include "SmSettingsMgr.h"
#include "MsUndExp.h"

namespace EODCO
{
typedef vector<double> EsDoubleVec;
}
namespace EODMS
{

/////////////////////////////////////////////////////////////////////////////
// Forward declaration
class CMsUnd;
typedef boost::shared_ptr<CMsUnd> CMsUndPtr;
typedef std::map<CString, CMsUndPtr>	CMsUndColl;
typedef std::pair<CString, CMsUndPtr>	CMsUndPair;

typedef boost::shared_ptr<CMsUndColl>	CMsUndCollPtr;
typedef std::map<long, CMsUndPtr>	CMsUndByIDColl;
typedef std::pair<long, CMsUndPtr>	CMsUndByIDPair;

typedef boost::shared_ptr<CMsUndByIDColl>	CMsUndByIDCollPtr;

/////////////////////////////////////////////////////////////////////////////
//
class  CMsUnd : public CMsContract
{
public:
	CMsUnd(void)
		: m_bIsHTB(false)
		,m_nLotSize(0L)
		,m_dSkew(0.)
		,m_dPos(BAD_DOUBLE_VALUE)
		,m_bIsActive(false) 
		,m_dKurt(0.)
		,m_bUseMidRates(true)
		,m_nState(0L)
		,m_dtLastMsUpdate(0.)
		,m_bNeedMsUpdate(true)
		,m_dtIvMsUpdate(0.)
		,m_bHasSynthOptWithPos(false)
		,m_bUndClosePriceUpdated(false)
		,m_nOptClosePriceUpdated(0L)
		,m_bMsUpdated(false)
		,m_spOptRoot(new CMsOptRootByNameColl)
		,m_spOptRootWithPos(new CMsOptRootColl)
		,m_spOpt(new CMsOptionColl)
		,m_spOptByKey(new CMsOptionByKeyColl)
		,m_spEmptyIvExp(new CMsUndExpColl)
		,m_enContractType(enStStock)
	{
	}

	~CMsUnd(void)
	{
//		m_spOptRoot = NULL;
//		m_spOpt = NULL;
//		m_spOptByKey = NULL;
//		m_spOptRootWithPos = NULL;
//		m_spVolaSource = NULL;
//		m_spEmptyIvExp = NULL;
	}

	virtual void ApplyRates(CMsRatePtr pRate);

	CMsOptRootByNameCollPtr OptRoot()
	{
		return m_spOptRoot;
	}

	CMsOptRootPtr OptRoot(const CString& sName)
	{
		return GetCollectionItem<CMsOptRootPtr>(m_spOptRoot, sName);
	}

	CMsOptRootCollPtr OptRootWithPos()
	{
		return m_spOptRootWithPos;
	}

	CMsOptRootPtr OptRootWithPos(long nOptRootID)
	{
		return GetCollectionItem<CMsOptRootPtr>(m_spOptRootWithPos, nOptRootID);
	}

	CMsOptionCollPtr Opt()
	{
		return m_spOpt;
	}

	CMsOptionPtr Opt(const CString& sOptSymbol)
	{
		return GetCollectionItem<CMsOptionPtr>(m_spOpt, sOptSymbol);
	}
	
	CMsUndExpCollPtr EmptyIvExp()
	{
		return m_spEmptyIvExp;
	}

	CMsUndExpPtr EmptyIvExp(long nExpiryID)
	{
		return GetCollectionItem<CMsUndExpPtr>(m_spEmptyIvExp, nExpiryID);
	}

	EODVP::CVpUndVolaSourcePtr VolaSource()
	{
		return m_spVolaSource;
	}

	void SetVolaSource(EODVP::CVpUndVolaSourcePtr pVolaSource)
	{
		m_spVolaSource = pVolaSource;
	}

	CMsOptionByKeyCollPtr OptByKey()
	{
		return m_spOptByKey;
	}

	CMsOptionPtr OptByKey(const CMsOptionKey& aKey)
	{
		return GetCollectionItem<CMsOptionPtr>(m_spOptByKey,aKey);
	}

	void MakeOptionKey(CMsOptRootPtr pRoot, CMsOptionPtr pOpt, CMsOptionKey& aKey);

	void MakeGroupPriceRequestKey(EODPP::CPpPriceRequestKey& aKey);

	virtual long FillDivsForCalc(EODCO::EsDoubleVec& vecDivDte, EODCO::EsDoubleVec& vecDivAmt,
										long nToday, long nDTE, double& dYield) = 0;

	void CalculateAllOptionsTheoPrice(CMsUndByIDCollPtr pUndColl, EODSM::CSmSettingsMgrPtr pSettingsMgr);
private:
	void _CalcOptionGreeks(CMsOptionPtr pOpt,  CMsOptRootPtr pOptRoot, CMsUndByIDCollPtr pUndColl, EODSM::CSmSettingsMgrPtr pSettingsMgr);

public:
	double			m_dPos;
	bool			m_bIsAmerican;
	bool			m_bIsHTB;
	bool			m_bIsActive;
	long			m_nLotSize;
	double			m_dSkew;
	double			m_dKurt;
	bool			m_bUseMidRates;
	long			m_nState;
	DATE			m_dtLastMsUpdate;
	bool			m_bNeedMsUpdate;
	DATE			m_dtIvMsUpdate;
	bool			m_bHasSynthOptWithPos;

	bool			m_bMsUpdated;
	bool			m_bUndClosePriceUpdated;
	long			m_nOptClosePriceUpdated;
	SYMBOL_TYPE		m_enContractType;

protected:
	CMsOptRootByNameCollPtr			m_spOptRoot; // all option roots by name
	
	CMsOptRootCollPtr				m_spOptRootWithPos; // option roots with positions by option root ID
	CMsOptionCollPtr				m_spOpt; // option references by option symbol
	CMsOptionByKeyCollPtr			m_spOptByKey; // option references by CMsOptionKey
	EODVP::CVpUndVolaSourcePtr		m_spVolaSource;
	CMsUndExpCollPtr				m_spEmptyIvExp;// expirations with empty IV points
};

typedef boost::shared_ptr<CMsUnd> CMsUndPtr;
typedef std::map<CString, CMsUndPtr>	CMsUndColl;
typedef std::pair<CString, CMsUndPtr>	CMsUndPair;

typedef boost::shared_ptr<CMsUndColl>	CMsUndCollPtr;
typedef std::map<long, CMsUndPtr>	CMsUndByIDColl;
typedef std::pair<long, CMsUndPtr>	CMsUndByIDPair;

typedef boost::shared_ptr<CMsUndByIDColl>	CMsUndByIDCollPtr;

};

#endif //__MSUND_H__