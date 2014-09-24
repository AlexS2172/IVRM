#pragma once
// Created 08 - 04 - 2005
// Created by Artem Shilaev
// Egar T

class CDbInterfaceBo ;
class CIVOptionData
{
	friend			CDbInterfaceBo ;

	long			m_lOptionID ;
	long			m_lUnderlyingID ;
	double			m_dModelIV ;
	double			m_dIV ;
	double			m_dDelta ;
	double			m_dGamma ;
	double			m_dVega ;
	double			m_dThetta ;
	double			m_dRho ;
	double			m_dBidPrice ;
	double			m_dAskPrice ;
	long			m_lBidSize ;
	long			m_lAskSize ;
	EgLib::vt_date	m_dtBidTime ;
	EgLib::vt_date	m_dtAskTime ;
	char			m_cBidExchange ;
	char			m_cAskExchange ;
	long			m_lVolume ;

public:
	CIVOptionData(CString _sData );

	CIVOptionData ( const CIVOptionData& _C)
	{
		Copy ( _C ) ;
	}
	CIVOptionData& operator=(const CIVOptionData& _C)
	{
		Copy ( _C ) ;
	}
	virtual ~CIVOptionData(void);
protected:
	void Copy ( const CIVOptionData& _C ) ;
public:
	long			GetOptionID() const
	{
		return m_lOptionID ;
	}
	long GetUnderlyingID() const
	{
		return m_lUnderlyingID ;
	}
	double			GetIV() const
	{
		return m_dIV ;
	}

};

class CIVStockData
{
	friend			CDbInterfaceBo ;
	long			m_lStockId ;
	double			m_dBidPrice ;
	double			m_dAskPrice ;
	double			m_dLastPrice ;
	long			m_lSize ;
	long			m_lAskSize ;
	long			m_lLastSize ;
	EgLib::vt_date	m_dtBidTime ;
	EgLib::vt_date	m_dtAsktime ;
	EgLib::vt_date	m_dtLastTime ;
	char			m_cBidEx ;
	char			m_cAskEx ;
	char			m_cLastPrEx ;
	long			m_lVolume ;

public:
	CIVStockData( CString ) ;

	CIVStockData( const CIVStockData& _C)  
	{
		Copy ( _C ) ;
	}
	CIVStockData& operator=(const CIVStockData& _C) 
	{
		Copy ( _C ) ;
		return *this ;
	}
protected:
	void Copy( const CIVStockData&) ;
public:
	long GetStockID() const
	{
		return m_lStockId ;
	}

	double GetLastPrice() const
	{
		return m_dLastPrice ;
	}

} ;
