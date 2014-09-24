// Created 07 - 04 - 2005
// Created by Artem Shilaev
// Egar T

#pragma once

#include "DbInterfaceBo.h"
#include "MSMQReceiver.h"
#include "main.h"

#import "..\tlb\MMW.tlb"  no_namespace, named_guids
//named_guids , rename_namespace ("EtsTransport")
#import "..\tlb\MsgStruct.tlb"  no_namespace, named_guids
//named_guids , rename_namespace ("EtsMsg")

class CIV_Data_Update_Service : BIV_Data_Update_Service
{

	IPubManager*		m_pEtsaMessenger ;
	IIvBlobPtr			m_pIvBlob ;

	static LPCTSTR					mProfileSectionServive ;
	static LPCTSTR					mProfileSectionDatabase;

	CDbInterfaceBo					m_DbInterface ;
	CMSMQReceiverPtr				m_pMSMQRec ; 

	EgLib::vt_date					m_dStart ;
	EgLib::vt_date					m_dStop ;

	CHandle							m_hStop ;
private:
	void ReadProfile() ;
	bool WaitTillOpen() ;//true - just opening of database
public:
	CIV_Data_Update_Service();
	virtual ~CIV_Data_Update_Service(void);
	void Load(void);
	void LoadingFunction() ;
private:
	void LoadDataFromQueue () ;

public:
	bool SaveIVOptionData(const CIVOptionData& _IVData);
	bool SaveIVStockData(const CIVStockData& _IVData);
	virtual bool IsEndOfDay () ;
	virtual void SendUpdate() ; 
	void OnStopMainThread()
	{
		SetEvent ( m_hStop ) ;
	}
};
