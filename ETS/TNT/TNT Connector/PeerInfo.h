#pragma once
#include "MMHHMessage.h"


struct CRequest
{
	long   m_lRequestType;
	string m_strName;
	CRequest():m_lRequestType(0){}
	CRequest(const CRequest& cn){Copy(cn);}
	CRequest& operator=(const CRequest& cn){Copy(cn); return *this;}
private:
	void Copy(const CRequest& cn)
	{
		m_lRequestType = cn.m_lRequestType;
		m_strName      = cn.m_strName;
	}
};

struct CPeerContext
{
	typedef queue<CRequest>      REQUESTQUEUE;

	string				m_sAddress;
	long				m_lIncomingSeqNum;
	time_t				m_tmHB;
	CFixBuffer			m_InputStream;

	string				m_sLogonID;
	bool				m_bLoggedOn;

	DWORD               m_dwCurrentDT;

	REQUESTQUEUE        m_quRequests;
	DWORD               m_dwRequestDT;

	bool                m_bRequestSendingEnabled;
	vt_date             m_dtLAstSendDate;

	void UpdateSendTimer(){	m_dtLAstSendDate = vt_date::GetCurrentDate();}
	CPeerContext()
	{
		m_lIncomingSeqNum = 35;
		m_tmHB = 0;
		m_dwCurrentDT = GetTickCount();
		UpdateSendTimer();

		m_bLoggedOn = false;
	};
};

struct	CPeerIdentity
{
	SOCKET				m_Socket;
	GUID				m_Guid;

	CPeerIdentity(){}

	CPeerIdentity(const SOCKET Socket)
	{
		m_Socket = Socket;
	}

	bool operator < (const CPeerIdentity& Origin) const
	{
		return m_Socket < Origin.m_Socket;
	}
};

class	CGuidWrapper
{
	GUID	m_Guid;

public:

	CGuidWrapper()
	{
		CoCreateGuid(&m_Guid);
	}

	operator const GUID& () const
	{
		return m_Guid;
	}
};

struct CMessageItem
{
	CPeerIdentity	m_Client;
	tnt_msg::CMMHHMessage*	m_pMessage;
};

typedef map<CPeerIdentity, CPeerContext>	TMapPeers;
