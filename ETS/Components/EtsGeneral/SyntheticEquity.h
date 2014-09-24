#pragma once
#include "CommonSPtr.h"
#include "Asset.h"

class CSyntheticEquity:	public CAsset
{
public:
	CSyntheticEquity(void);
	virtual ~CSyntheticEquity(void);

public:
	IBasketAssetComponentsCollPtr		m_spComponents;
	void								AddComponent(IBasketAssetComponentPtr spComponent);
};
