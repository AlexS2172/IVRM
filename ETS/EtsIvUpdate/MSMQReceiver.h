#pragma once

#include "Mq.h"
#include "main.h"
#include <boost/shared_ptr.hpp>

class CMSMQReceiver
{
	BIV_Data_Update_Service*	m_pService ;
	HANDLE						m_hQuOpt ;
	HANDLE						m_hQuSt ;
	_bstr_t						m_sMSQMStrOpt ;
	_bstr_t						m_sMSQMStrStock ;

	MQMSGPROPS     m_msgprops;     
	MQPROPVARIANT  m_aPropVar[20];     
	QUEUEPROPID    m_aqPropId[20];     
	MSGPROPID      m_amPropId[20]; 

public:
	CMSMQReceiver(BIV_Data_Update_Service* _pService);
	virtual ~CMSMQReceiver(void);

public:
	bool Open ( bool bStock );
	void PurgeQue( );
	void Close () ;
	void SetMSQMStr( _bstr_t _strOpt , _bstr_t _strSt )
	{
		m_sMSQMStrOpt = _strOpt ;
		m_sMSQMStrStock = _strSt ;
	}
	void GetMessages () ;
private:
	void GetOptionMessages () ;
	void GetStockMessages () ;
private:
	bool GetMessageSize ( DWORD* dwSize , HANDLE _hndl ) ;
	bool PeekMessage ( DWORD* dwSize , PBYTE _pByte , HANDLE _hndl)  ;
};

typedef boost::shared_ptr<CMSMQReceiver> CMSMQReceiverPtr ;
