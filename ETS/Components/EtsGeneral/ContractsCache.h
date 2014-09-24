#pragma once
#include "CommonSPtr.h"
#include "Cache.h"
#include "ContractColl.h"
#include "MarketDataSource\MarketDataSource.h"
#include "Calculator.h"
#include "AbstractContract.h"
#include <hash_map>

class CSpotOptionContract;
class CAsset;

typedef	std::map<CTicker, IContractPtr>	CTickerContractMap;

typedef boost::shared_ptr<CTickerContractMap>		CTickerContractMapPtr;

class CFactory{
public:
	typedef CFactory* ptr;
	
	static CFactory::ptr getInstance();

	virtual IContractPtr 
	CreateSpotOption();

	virtual IContractRootPtr 
	CreateContractRoot();

	virtual IContractRootPtr
	CreateFutureRoot();

	virtual IContractRootCollPtr
	CreateContractRootColl();

private:
	CFactory();
};

//class CSerializerSettings{
//public:
//	typedef CSerializerSettings* ptr;
//};
//
//class CDbSerializerSettings: public CSerializerSettings{
//public:
//	typedef CDbSerializerSettings* ptr;
//
//	std::string connection_string;
//	CDbSerializerSettings(std::string ConnectionString);
//};
//
//class CAbstractSerializer
//{
//public:
//	typedef CAbstractSerializer* ptr;
//
//	virtual long 
//	Serialize(CSpotOptionContract& Contract) = 0;
//
//	virtual void 
//	Initialize(CSerializerSettings::ptr Settings) = 0;
//};
//
//class CDbSerializer: public CAbstractSerializer
//{
//public:
//	/*ctor*/
//	CDbSerializer();
//
//	virtual
//	~CDbSerializer();
//
//	/*virtual*/	long
//	Serialize(CSpotOptionContract& Contract);
//
//	/*virtual*/ void 
//	Initialize(CSerializerSettings::ptr Settings);
//
//private:
//	CDbSerializerSettings::ptr	__settings;
//};
//
//class CSerializer
//{
//public:
//	static
//	CAbstractSerializer::ptr getInstance();
//};

class CContractsCache:	public CCache, 
						public CMarketDataSource
{
public:
	CContractsCache(void);
	virtual ~CContractsCache(void);

//private:
	typedef					stdext::hash_map<long, CAbstractContract*>	IDContractMap;
	
	IAssetPtr				GetAsset(long lAssetID);
	IEtsPriceProfileAtomPtr	GetPriceProfile(long lID);
	IVolatilitySourcePtr	GetVolatilitySource();

	IContractPtr			LoadSingleFutureContract(long lContractID);
	IContractPtr			LoadSingleSpotContract(long lContractID);
	IContractPtr			LoadSingleSpotOptionContract(long lContractID);
	IContractPtr			LoadSingleFutureOptionContract(long lContractID);

	//recordset parser
	IContractPtr			ParseSpotOptionContract(CStoredProc<CClientRecordset>& recordset);
	IContractPtr			ParseFutureOptionContract(CStoredProc<CClientRecordset>& recordset);
	IContractPtr			ParseFutureContract(CStoredProc<CClientRecordset>& recordset);
	IContractPtr			ParseSpotContract(CStoredProc<CClientRecordset>& recordset);
	//multiple initilaization
	void					LoadMultipleSpotOptionContracts(long lAssetID);

	bool					AddContract(IContractPtr spContract);
	void					SubscribeContract(IContractPtr spContract);
	bool					IsContractExist(IContractPtr spContract);

	IContractCollPtr		m_spContracts;
	CTickerContractMapPtr	m_spTickerContract;
	CCalculatorPtr			m_spCalculator;
	
	IDContractMap			changed_contracts;	
	CLock					updated_contract_lock;

public:
	IContractCollPtr		GetContractsColl();	
	
	IContractPtr			GetContract(long lContractID);
	long					GetContractID(CTicker* ptrTicker);

	EtsContractTypeEnum		GetContractType(long lContractID);
	IContractPtr			LoadSingleContract(long lContractID);
	CTickerContractMapPtr	GetTickerContractMap() {return m_spTickerContract;};
	CCalculatorPtr			GetCalculator() {return m_spCalculator;};

	void					Clear();

public:
	void					DoCalc();
	void					DoRisksUpdate();

public:

	CAbstractContract*		find_contract_by_ticker(CTicker& ticker);
	
	CAbstractContract*		SetQuote(CTicker* ptrTicker, CQuote* ptrQuote);
	void					GetRecalculationNeededContracts(ContractsVector& contracts);
	void					InitializeForAsset(CTicker* ptrAsset);
	void					InitializeFromTrades();

	void					getDerivativeContracts(	CAbstractContract* Contract,
													IDContractMap& Contracts);


	IContractPtr 
	CreateFlexContract(	LONG					AssetID,
						DATE					Expiration,
						DOUBLE					Strike,
						EtsOptionTypeEnum		Type,
						DATE					TradingClose,
						DATE					SettlementValueTime,
						ExerciseStyleEnum		ExerciseStyle);

};
//--------------------------------------------------------------------//
class CContractRootCache: public CCache
{
public:
	
	typedef CContractRootCache* ptr;

	virtual
	~CContractRootCache(void);

	IContractRootPtr
	getContractRoot(long RootID);

	IContractRootPtr
	LoadSingleRoot(long RootID);

	static 
	CContractRootCache::ptr
	getInstance();

	IContractRootCollPtr
	getCollection();

private:
	IContractRootPtr
	ParseContractRoot(CStoredProc<CClientRecordset>& recordset);

	void
	addContractRoot(IContractRootPtr spContractRoot);

private:
	CContractRootCache(void);

	IContractRootCollPtr	m_spColl;
};
//--------------------------------------------------------------------//
class CAssetCorrelation: public CCache{
public:
	class CCorrelation{
	public:
		typedef CCorrelation* ptr;

		CCorrelation();

		double	Correlation;

		long	ComponentA;
		long	ComponentB;
	};

public:
	typedef CAssetCorrelation* ptr;

	virtual
	~CAssetCorrelation();

	static
	CAssetCorrelation::ptr
	getInstance();

	double
	getCorrelation(	const CAsset& assetA,
					const CAsset& assetB);

	CCorrelation::ptr
	LoadCorrelation(const CAsset& assetA, 
					const CAsset& assetB);

	void
	addCorrelation(	const CAsset& assetA, 
					const CAsset& assetB, 
					double Correlation);

	CCorrelation::ptr
	ParseCorrelationRecordset(CStoredProc<CClientRecordset>& recordset);

private:
	CAssetCorrelation();

	typedef std::map<long, CCorrelation::ptr>	correlation_set;
	typedef std::map<long, correlation_set>		correlation_surface;

	correlation_surface	m_Surface;

	CCorrelation::ptr
	add_new(long ComponentA,
			long ComponentB, 
			CCorrelation::ptr Correlation);

	CCorrelation::ptr
	find_correlation(long ComponentA, long ComponentB);

	void
	clear_surface();
};
//--------------------------------------------------------------------//
typedef	boost::shared_ptr<CContractsCache>	CContractsCachePtr;
