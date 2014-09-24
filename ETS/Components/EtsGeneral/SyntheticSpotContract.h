#pragma once
#include "abstractcontract.h"
//----------------------------------------------------------------------------------------------//
class CSyntheticSpotComponent
{
public:
	CSyntheticSpotComponent()
	{
		m_pContract = NULL;
		m_dWeight = BAD_DOUBLE_VALUE;
		m_dSettlement = BAD_DOUBLE_VALUE;
	};

	~CSyntheticSpotComponent()
	{
		m_pContract = NULL;
		m_dWeight = BAD_DOUBLE_VALUE;
		m_dSettlement = BAD_DOUBLE_VALUE;
	};

public:
	CSyntheticSpotComponent(CAbstractContract* pContract, double dWeight, double dSettlement)
	{
		if (pContract)
		{
			m_pContract = pContract;
			m_dWeight = dWeight;
			m_dSettlement = dSettlement;
		}
	};

public:
	CAbstractContract*	m_pContract;
	double				m_dWeight;
	double				m_dSettlement;
};

typedef boost::shared_ptr<CSyntheticSpotComponent>	CSyntheticSpotComponentPtr;
typedef std::map<long, CSyntheticSpotComponentPtr>	CSyntheticSpotContainer;
typedef boost::shared_ptr<CSyntheticSpotContainer>	CSyntheticSpotContainerPtr;
//---------------------------------------------------------------------------------------------//
class CSyntheticSpotContract: public CAbstractContract
{
public:
	CSyntheticSpotContract(void);
	~CSyntheticSpotContract(void);

public:
	CSyntheticSpotContainerPtr	m_spComponents;
	//------------------------------------------------------------------------------------------//
	CSyntheticSpotComponentPtr	
	Add(CAbstractContract* pContract, double dWeight, double dSettlement);
	//------------------------------------------------------------------------------------------//

public:
	//------------------------------------------------------------------------------------------//
	void Clear();
	//------------------------------------------------------------------------------------------//
	double	
	getPriceMid();
	//------------------------------------------------------------------------------------------//
	CRisksPtr
	Calculate(CMarketSimulator::ptr MarketSimulator);
	//------------------------------------------------------------------------------------------//
};
