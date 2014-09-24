#pragma once
#include "EtsMmRisks.h"
#include "MmRvPrice.h"
#include <vector>
#include <functional>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>

class CMmRvPosColl;
class CMmRvUndColl;
class CMmRvUndAtom;
class CMmRvRowData;
class CRiskViewFieldSort;

_COM_SMARTPTR_TYPEDEF(IMmRvUndColl, IID_IMmRvUndColl); 
_COM_SMARTPTR_TYPEDEF(IMmRvPosAtom, IID_IMmRvPosAtom);

_COM_SMARTPTR_TYPEDEF(IMmRvRowData, IID_IMmRvRowData);

class CMmRvAggData {
	size_t CreateRowsForPositions(CMmRvUndAtom* pUndAtom, long lExpilter, unsigned outlnLvl, 
											const CRiskViewFieldSort *Cmp, std::vector<IMmRvRowDataPtr>& posRows, long& nOptPositions,long& nUndPositions,
											CComObject<CMmRvUndColl>* pUndColl  );
	void CreateRowsForAggregations(CMmRvUndAtom* pUndAtom, long lExpilter, unsigned outlnLvl, const CRiskViewFieldSort *Cmp, std::vector<IMmRvRowDataPtr>& posRows);
	bool CreateRow(CMmRvUndAtom* pUndAtom, unsigned outlnLvl, std::vector<IMmRvRowDataPtr>& aggRows);
public:
	typedef boost::shared_ptr<CMmRvPosAtom>	MmRvPosAtomPtr;
	typedef boost::shared_ptr<CMmRvAggData>	MmRvAggDataPtr;
	typedef std::vector<MmRvPosAtomPtr>		Positions;
	typedef std::vector<MmRvAggDataPtr>		Aggregations;
	
	typedef enum{
		enUndAgg,
		enFutAgg,
		enExpAgg
	} AggregationType;

	_bstr_t				Name_;
	_bstr_t				internalName_;						// used as key in map
	Positions			Positions_;
	Aggregations		Aggregations_;
	AggregationType		Type_;
	CMmRvUndAtom*		pUnd_;
	CMmRvFutAtom*		pFut_;

	LONG					UndPos_, FutQty_, OptQty_;		// underlying position, future quantity and options quantity within aggregation
	double				NetDlt_, NetDlt$_, OptDlt_, OptDlt$_;	// net delta, net delta $, option delta
	double				Gma1P_, NetGma$_, Gma1$_, NetGma_;		// gamma 1 %, net gamma in dollars, gamma in shares & net gamma in dollars
	double				PnLTheo_, PnlMTM_, PnlEdge_;	//
	double				Vga$_, WtdVga$_, VgaGma_, VgaDlt_; //Vega$, WtdVga$, VgaGma, VgaDlt
	double				Tht$_, ThtDlt_, ThtGma_; //Tht$, ThtDlt, ThtGma
	double				Rho$_; //Rho$
	double				TmVal_, TmValInShares_; //TmVal (Time Value)
	double				Beta_;	// beta used in aggregation
	double				BetaWtdDelta$_;
	double				UndPosForRates_;
	double              NetExposure_;
	double              NetExposureSort_;
	double              NetExposureLong_;

	VARIANT_BOOL		BadBetaWtdDelta$_,BadRho$_, BadTht$_, BadThtDlt_, BadThtGma_,BadVga$_, BadWtdVga$_, BadVgaGma_, BadVgaDlt_,
							BadGma1$_, BadGma1P_, BadNetGma$_,BadNetGma_,BadPnlMtm_, BadPnlTheo_, BadNetDelta_, 
							BadOptDelta_, BadNetDlt$_, BadOptDlt$_, BadTmVal_, BadTmValInShares_, BadNetExposure_;
	
	VARIANT_BOOL		m_bIsSyntheticAggregation;	// this left for compatibility - not really used now
	VARIANT_BOOL		m_bIsHeadAggregation;

	CMmRvAggData( AggregationType Type, const _bstr_t& Name = _bstr_t(L"") ) : 
		Name_(Name)
		,Type_(Type)
		,pUnd_(NULL)
		,pFut_(NULL)
		,UndPos_(BAD_LONG_VALUE)
		,FutQty_(BAD_LONG_VALUE)
		,OptQty_(BAD_LONG_VALUE)
		,NetDlt_(BAD_DOUBLE_VALUE),NetDlt$_(BAD_DOUBLE_VALUE),OptDlt_(BAD_DOUBLE_VALUE), OptDlt$_(BAD_DOUBLE_VALUE)
		,Gma1P_(BAD_DOUBLE_VALUE), NetGma$_(BAD_DOUBLE_VALUE), Gma1$_(BAD_DOUBLE_VALUE), NetGma_(BAD_DOUBLE_VALUE)
		,PnLTheo_(BAD_DOUBLE_VALUE), PnlMTM_(BAD_DOUBLE_VALUE), PnlEdge_(BAD_DOUBLE_VALUE)
		,Vga$_(BAD_DOUBLE_VALUE), WtdVga$_(BAD_DOUBLE_VALUE), VgaGma_(BAD_DOUBLE_VALUE), VgaDlt_(BAD_DOUBLE_VALUE)
		,Tht$_(BAD_DOUBLE_VALUE), ThtDlt_(BAD_DOUBLE_VALUE), ThtGma_(BAD_DOUBLE_VALUE)
		,Rho$_(BAD_DOUBLE_VALUE)
		,TmVal_(BAD_DOUBLE_VALUE), TmValInShares_(BAD_DOUBLE_VALUE)
		,Beta_(BAD_DOUBLE_VALUE)
		,BetaWtdDelta$_(BAD_DOUBLE_VALUE)
		,UndPosForRates_(0)
		,BadPnlMtm_(VARIANT_FALSE), BadPnlTheo_(VARIANT_FALSE)
		,BadNetDelta_(VARIANT_FALSE), BadOptDelta_(VARIANT_FALSE)
		,BadNetDlt$_(VARIANT_FALSE), BadOptDlt$_(VARIANT_FALSE)
		,BadGma1$_(VARIANT_FALSE), BadGma1P_(VARIANT_FALSE), BadNetGma$_(VARIANT_FALSE), BadNetGma_(VARIANT_FALSE)
		,BadVga$_(VARIANT_FALSE), BadWtdVga$_(VARIANT_FALSE), BadVgaGma_(VARIANT_FALSE), BadVgaDlt_(VARIANT_FALSE)
		,BadTht$_(VARIANT_FALSE), BadThtDlt_(VARIANT_FALSE), BadThtGma_(VARIANT_FALSE)
		,BadTmVal_(VARIANT_FALSE),BadTmValInShares_(VARIANT_FALSE)
		,BadRho$_(VARIANT_FALSE)
		,BadBetaWtdDelta$_(VARIANT_FALSE)
		,m_bIsSyntheticAggregation(VARIANT_FALSE)
		,NetExposure_(BAD_DOUBLE_VALUE)
		,BadNetExposure_(VARIANT_FALSE)
		,NetExposureSort_(BAD_DOUBLE_VALUE)
		,NetExposureLong_(BAD_DOUBLE_VALUE)
		,m_bIsHeadAggregation(VARIANT_FALSE)
	{}
	void ClearAggregatedValues(){
		UndPos_= FutQty_= OptQty_ = BAD_LONG_VALUE;
		OptDlt$_ =  NetDlt_= NetDlt$_ = OptDlt_ = Gma1P_= NetGma$_= Gma1$_= NetGma_ =PnLTheo_= PnlMTM_= PnlEdge_=
		Vga$_=  WtdVga$_=  VgaGma_=  VgaDlt_= Tht$_=  ThtDlt_=  ThtGma_= 	Rho$_= TmVal_ = BAD_DOUBLE_VALUE;
		UndPosForRates_ = 0.;
		NetExposure_=BAD_DOUBLE_VALUE;
		BadNetExposure_=VARIANT_FALSE;

		NetExposureSort_ = BAD_DOUBLE_VALUE;
		NetExposureLong_ = BAD_DOUBLE_VALUE;


		BadBetaWtdDelta$_=BadRho$_=BadTmValInShares_=BadTmVal_=BadTht$_= BadThtDlt_=BadThtGma_=BadVga$_= BadWtdVga$_= BadVgaGma_= BadVgaDlt_=BadGma1$_= BadGma1P_= BadNetGma$_= BadNetGma_=BadNetDlt$_ = 
		BadOptDlt$_ = BadPnlMtm_ = BadPnlTheo_ = BadNetDelta_ = BadOptDelta_ = m_bIsHeadAggregation = VARIANT_FALSE;
		std::for_each( Aggregations_.begin(), Aggregations_.end(), boost::bind(&CMmRvAggData::ClearAggregatedValues, _1) );
	}
	void CalcNetDeltas(EtsContractTypeEnum underlyingContractType);
	void AddAggregatedValues( const CMmRvRowData* pRow);
	void CreateRows(CMmRvUndAtom* pUndAtom, long lExpiryFilter, unsigned outlineLevel,
							const CRiskViewFieldSort *, std::vector<IMmRvRowDataPtr>& posRows,long& nOptPositions,
							long& nUndPositions,CComObject<CMmRvUndColl>* pUndColl );
	void SetAllValuesAsBad()	{
		BadNetExposure_ = BadBetaWtdDelta$_=BadRho$_=BadTmValInShares_=BadTmVal_=BadTht$_= BadThtDlt_=BadThtGma_=BadVga$_= BadWtdVga$_= BadVgaGma_= BadVgaDlt_=BadGma1$_= BadGma1P_= BadNetGma$_= BadNetDlt$_ = BadOptDlt$_ = 
		BadPnlMtm_ = BadPnlTheo_ = BadNetDelta_ = BadOptDelta_ = VARIANT_TRUE;
	}

	static bool IsAggregationRowVisible( const CMmRvAggData* pAgg);
	bool CheckPositions(long lExpiryFilter);

};