// HFServer.h: interface for the CHFServer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __HFSERVER_H__
#define __HFSERVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPrice;
class CHFServer:  
	protected EgLib::CThread
{
	typedef std::set<CPrice*> subset;

public:
	CHFServer();
	~CHFServer();

protected:
	long m_nStatus;
	bool m_bTerminate;
	HANDLE m_hThread;
	long m_nCount;

	EgLib::CLock m_csQueue;

	char m_etiPblk[256];
	ETI_TABLE* m_etiPtr;

	subset m_setSubs;

protected:
	virtual UINT Run();

	void Terminate();

public:
	long Initialize();
	long Uninitialize();
	long GetStatus();

	void Subscribe(CPrice* pSub);
	void Unsubscribe(CPrice* pSub);
};

inline long CHFServer::GetStatus()
{
	return m_nStatus;
}

#endif // __HFSERVER_H__
