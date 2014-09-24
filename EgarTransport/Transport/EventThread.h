// EventThread.h: interface for the CEventThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTTHREAD_H__D09C1373_01C6_44FE_93CB_A205F597476D__INCLUDED_)
#define AFX_EVENTTHREAD_H__D09C1373_01C6_44FE_93CB_A205F597476D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <EgLib\EgLibDbg.h>
#include <EgLib\EgLibThread.h>
#include <EgLib\EgLibMisc.h>
#include <EgLib\EgLibStrategy.h>
#include <boost\shared_ptr.hpp>

//forward declaration
class CHost;
class CRouter;
class CSession;
//forward declaration

class CTpEvent
{
public:
	CTpEvent() : m_bsParams(NULL) {}
	CTpEvent(DISP_TRANSPORT_IDS nID, BSTR Params)
		:m_enDispId(nID)
		,m_bsParams(Params){}


	virtual ~CTpEvent() 
	{
		if(m_bsParams)
			SysFreeString(m_bsParams);
		m_bsParams = NULL;
	};


	DISP_TRANSPORT_IDS	m_enDispId;
	BSTR				m_bsParams;
};
typedef boost::shared_ptr<CTpEvent> CTpEventPtr;

class CEventThread  : public EgLib::CStrategyT<CEventThread, CTpEventPtr>
{
public:
	CEventThread() {};
	virtual ~CEventThread() {};

	virtual void OnData(CTpEventPtr& spEvent) 
	{
		try
		{
			if(spEvent)
				Fire_OnMessage(spEvent);
		}
		catch (_com_error& /*e*/)
		{

		}
		catch(...)
		{

		}
	}


	virtual HRESULT Fire_OnMessage(CTpEventPtr pEvent) = 0;

	DWORD Start()
	{
		return EgLib::CStrategyT<CEventThread, CTpEventPtr>::Start();
	};

	DWORD Stop(bool bWait = true, DWORD dwWait = INFINITE)
	{
		DWORD dwErr = EgLib::CStrategyT<CEventThread, CTpEventPtr>::Stop(bWait, dwWait);
		return dwErr;
	}

	void PushEvent(CTpEventPtr spEvent)
	{
		EgLib::CStrategyT<CEventThread, CTpEventPtr>::PushData(spEvent);
	}

	HRESULT OnStrategyInitialize(){ return  CoInitializeEx(NULL, COINIT_MULTITHREADED);}
	void OnStrategyUninitialize(){CoUninitialize();}
};
//-------------------------------------------------------------------------------
class CHostEventThread : public CEventThread
{
public:
	CHostEventThread(CHost* pHost) : m_pHost(pHost) {};
	virtual ~CHostEventThread() {};

	virtual HRESULT Fire_OnMessage(CTpEventPtr pEvent);

private:
	CHost* m_pHost;

};

class CRouterEventThread : public CEventThread
{
public:
	CRouterEventThread(CRouter* pRouter) : m_pRouter(pRouter) {};
	virtual ~CRouterEventThread() {};

	virtual HRESULT Fire_OnMessage(CTpEventPtr pEvent);

private:
	CRouter* m_pRouter;

};

class CTpSessionEvent : public CTpEvent
{
public:
	CTpSessionEvent() : m_pSession(NULL) {}
	virtual ~CTpSessionEvent() ;
	CSession*	m_pSession;
};
typedef boost::shared_ptr<CTpSessionEvent> CTpSessionEventPtr;

class CSessionEventThread : public CEventThread
{
public:
	CSessionEventThread(){};
	virtual ~CSessionEventThread() {};

	virtual HRESULT Fire_OnMessage(CTpEventPtr pEvent);
};

#endif // !defined(AFX_EVENTTHREAD_H__D09C1373_01C6_44FE_93CB_A205F597476D__INCLUDED_)
