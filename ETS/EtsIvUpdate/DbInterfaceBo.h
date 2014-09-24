// created:		07-April-2005 by Shilaev Artem
// copyright:	(c) 2005 EGAR Technology Inc.
//**************************************************************************************************************//
#pragma once

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//

//**************************************************************************************************************//
// class CDbInterfaceBo
//**************************************************************************************************************//

class CIVOptionData ;
class CIVStockData ;

class CDbInterfaceBo
{
	// Data members

private:

	static CDbInterfaceBo*	m_pDbInterface;
	static _bstr_t			m_bsConnectionString;
	static DWORD			m_nReconnectionAttempts;
	static DWORD			m_nReconnectionDelay;

	EgLib::CDBConnection	m_Connection;

	// Constructor/Destructor

public:

	CDbInterfaceBo() ;

	~CDbInterfaceBo();

	// Operators

	// Attributes

	static void GetInstance(CDbInterfaceBo** ppDbInterface) ;

	static void SetConnectionString(const std::string& rsConnectionString);
	static void SetReconnectionAttempts(DWORD nReconnectionAttempts);
	static void SetReconnectionDelay(DWORD nReconnectionDelay);
	static void DestroyConnection()
	{
		if(m_pDbInterface)
		{
			delete m_pDbInterface;
			m_pDbInterface = NULL;
		}
	}


	EgLib::CDBConnection& GetConnection();

	// Operations

	void Connect() ;
	void Disconnect() ;

	// Overrides

	// Message map and handlers

	// Implementation

protected:

private:

	//No copies do not implement
	CDbInterfaceBo(const CDbInterfaceBo& rhs);
	CDbInterfaceBo& operator=(const CDbInterfaceBo& rhs);
public:
	DWORD BeginOfDay(void);
	bool SaveIVOptionData(const CIVOptionData& _IVData);
	bool SaveIVStockData(const CIVStockData& _IVData);
};

