#include "StdAfx.h"
#include "SyntheticSpotContract.h"
#include "Asset.h"
#include "ContractColl.h"
//--------------------------------------------------------------------------------------------//
CSyntheticSpotContract::CSyntheticSpotContract(void)
{
	m_spComponents = CSyntheticSpotContainerPtr(new CSyntheticSpotContainer);
	m_enType = enCtStock;
}
//--------------------------------------------------------------------------------------------//
CSyntheticSpotContract::~CSyntheticSpotContract(void)
{
	m_spComponents->clear();
}
//--------------------------------------------------------------------------------------------//
void CSyntheticSpotContract::Clear()
{	
	CAbstractContract::Clear();
};
//--------------------------------------------------------------------------------------------//
double CSyntheticSpotContract::getPriceMid(){
	return 0.;
};
//--------------------------------------------------------------------------------------------//
CRisksPtr
CSyntheticSpotContract::Calculate(CMarketSimulator::ptr MarketSimulator){
	return CRisksPtr();
};
//--------------------------------------------------------------------------------------------//
CSyntheticSpotComponentPtr	
CSyntheticSpotContract::Add(CAbstractContract* pContract, double dWeight, double dSettlement)
{
	if (pContract)
	{
		CSyntheticSpotComponentPtr spComponent = 
			CSyntheticSpotComponentPtr(new CSyntheticSpotComponent(pContract, dWeight, dSettlement));

		(*m_spComponents)[pContract->m_nID] = spComponent;

		return spComponent;
	}
	return CSyntheticSpotComponentPtr();
};
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//