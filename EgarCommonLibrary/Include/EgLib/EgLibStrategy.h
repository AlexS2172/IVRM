//**************************************************************************************************************//
// abstract:	interface & implementation of strategy classes
//
// content:		-
// copyright:	(c) 2003 EGAR Technology Inc.
//**************************************************************************************************************//
#pragma once

#if !defined(__EGLIBTHREAD_H__)
#error 'EgLibStrategy.h requires EgLibThread.h to be included first'
#endif

//#include <EgStdLib/Trace.h>
#define __EGLIBSTRATEGY_H__
#include <Boost/shared_ptr.hpp>

namespace EgLib
{
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

	//**************************************************************************************************************//
	// class CStrategyT
	//**************************************************************************************************************//
	template<class _cls, typename t_Data , class _Container = EgLib::CDoubleQueue<t_Data> >
	class CStrategyT : 
		protected EgLib::CThread,
		private _Container
	{
	public:

		
		// Data members

	// Constructor/Destructor

		CStrategyT()
			:m_bRemoveAll(false)
		{
		}

		CStrategyT(int iStrategyID)
			:m_bRemoveAll(false)
			,m_iStrategyID(iStrategyID)
		{
		}

		~CStrategyT()
		{
		}
	// Operators

	// Attributes

	// Operations
		unsigned int GetSize() 
		{
			return TotalQueueSize();
		}
		bool PushData(const t_Data& Data)
		{
			CAutoLock lockIn(GetLockIn());
			AddIn(Data);
			return true;
		}

	// Overrides
		void RemoveAll(bool bClearOutgoing = true)
		{
			m_bRemoveAll = true;
			{
				CAutoLock lockIn(GetLockIn());
				ClearIn();
			}
			if(bClearOutgoing)
			{
				CAutoLock lockOut(GetLockOut());
				ClearOut();
			}
			m_bRemoveAll = false;
		}
	 
		long GetStrategyStatistics(bool bReset = true)
		{
			return GetStatistics(bReset);
		}

	private:
		virtual UINT Run()
		{
			
			//using EgStd::CEgTracingClass::TraceStatic(
			//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Enter"));


			const HANDLE hEvents[] = { GetNotificationEvent(), GetStopEventHandle()	};
			_cls* pT = static_cast<_cls*>(this);
			pT->OnStrategyInitialize();

			try
			{

				bool bContinue = true;
				while (bContinue)
				{
					//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("while (true) iteration start"));
					
					DWORD dwResult = WaitWithEvents(ARRAY_SIZE(hEvents), hEvents, INFINITE);
					if (WAIT_OBJECT_0 == dwResult )
					{
						while(true)
						{
							if(!IsRemoveAll())
							{
								if(::WaitForSingleObject(GetStopEventHandle(),0)!=WAIT_TIMEOUT)
								{
									bContinue = false;
									break;
								}

								ProcessOutData(GetStopEventHandle());
								SwitchToThread();
								if( _Container::InSize() )
								{
									if(::WaitForSingleObject(GetStopEventHandle(),0)!=WAIT_TIMEOUT)
									{
										bContinue = false;
										break;
									}
								}
								else
									break;
							}
						}
					}
					else 
					{
						
						//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Break while(true)"));
						if (WAIT_OBJECT_0 + 1 == dwResult)
						{
							ATLTRACE("Stop event %s\n",__FUNCTION__);
							bContinue = false;
							break;
						}
						else
						{
							ATLTRACE("Stop by result %d in %s\n",dwResult, __FUNCTION__);
							bContinue = false;
							break;
						}
					}
				}
				//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("pT->OnStrategyUninitialize() before call"));
				pT->OnStrategyUninitialize();
			}
			catch(...)
			{
				//EgStd::CEgTracingClass::TraceStatic(enlogFaults, __FUNCTION__ , _T("catch(...)"));
			//	MessageBox(NULL, "Strategy's thread .... EXCEPTION!", "", 0);
				ATLTRACE("Unknown exception in %s\n",__FUNCTION__);
			}

			//MessageBox(NULL, "Strategy's thread .... exit", "", 0);
			//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Exit"));
			return 0;
		}
	// Implementation

	public:
		HRESULT OnStrategyInitialize(){return S_OK;}
		void    OnStrategyUninitialize(){}

		bool    IsRemoveAll(){ return m_bRemoveAll; }
	private:

		UINT    m_iStrategyID;
		volatile bool m_bRemoveAll;
		//No copies do not implement
		CStrategyT(const CStrategyT& rhs);
		CStrategyT& operator=(const CStrategyT& rhs);
	};


	//-------------------------------------------------------------------------------------------------------------------
	template <typename __cls>
	class CStrategyPoolObjectBase
	{
	public:
		CStrategyPoolObjectBase(__cls* pAdaptor)
			:m_pAdaptor(pAdaptor){}

		virtual ~CStrategyPoolObjectBase()	{	}

		virtual void DoTask() = 0;
	public:

		void OnData()
		{
			try
			{
				DoTask();
			} 
			catch(...)
			{
			}
		}
	protected:
		__cls* m_pAdaptor;
	};


	//-------------------------------------------------------------------------------------------------------------------
	template<typename __cls, class _Container = EgLib::CDoubleQueue<boost::shared_ptr<CStrategyPoolObjectBase<__cls> > > >
	class CSimplyPoolThreadT
		: public CStrategyT<CSimplyPoolThreadT<__cls, _Container>, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>
	{
	public:
		typedef CStrategyPoolObjectBase<__cls> PoolDataType;
		typedef boost::shared_ptr<PoolDataType> PoolDataTypePtr;


		CSimplyPoolThreadT(){};
		virtual ~CSimplyPoolThreadT()
		{
			Stop(true, 100);
		}

		virtual void OnData(PoolDataTypePtr& Data)
		{
			Data->OnData();
		}

		void Start()
		{
			CStrategyT<CSimplyPoolThreadT, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>::Start();
		}
		void Clear()
		{
			CStrategyT<CSimplyPoolThreadT, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>::RemoveAll(false);
		}
		void Stop(bool bWait = false, DWORD dwTimeout = INFINITE)
		{
			CStrategyT<CSimplyPoolThreadT, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>::Stop(bWait, dwTimeout);
		}


		HRESULT OnStrategyInitialize() {return S_OK;}
		void OnStrategyUninitialize()  {}
	};

	//-------------------------------------------------------------------------------------------------------------------
	template<typename __cls, class _Container = EgLib::CDoubleQueue<boost::shared_ptr<CStrategyPoolObjectBase<__cls> > > >
	class CComPoolThreadT
		: public CStrategyT<CComPoolThreadT<__cls,_Container>, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>
	{
	public:

		typedef CStrategyPoolObjectBase<__cls> PoolDataType;
		typedef boost::shared_ptr<PoolDataType> PoolDataTypePtr;

		CComPoolThreadT(){};
		CComPoolThreadT(UINT iThreadID):
			CStrategyT<CComPoolThreadT, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>(iThreadID)
			{
			};
		virtual ~CComPoolThreadT()
		{
			Stop(true, 100);
		}

		void Start()
		{
			CStrategyT<CComPoolThreadT, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>::Start();
		}
		void Stop(bool bWait = false, DWORD dwTimeout = INFINITE)
		{
			CStrategyT<CComPoolThreadT, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>::Stop(bWait, dwTimeout);
		}
		void Clear()
		{
			CStrategyT<CComPoolThreadT, boost::shared_ptr<CStrategyPoolObjectBase<__cls> >, _Container>::RemoveAll(false);
		}

		virtual void OnData(PoolDataTypePtr& Data)
		{
			Data->OnData();
		}

		HRESULT OnStrategyInitialize()
		{
			return CoInitializeEx(NULL, COINIT_MULTITHREADED);
		}
		void OnStrategyUninitialize()
		{
			CoUninitialize();
		}
	};

	//---------------------------------------------------------------------------------------------------------------------
	typedef std::vector<long> CStrategyPoolStatistics;
	template<class _clsPoolThread >
	class CStrategyPoolT 
	{
	private:
		typedef boost::shared_ptr<_clsPoolThread> CStrategyPtr;
		typedef std::vector<CStrategyPtr> CVecThreadPool;

	public:
		typedef _clsPoolThread  CStrategyThread;
	public:

		CStrategyPoolT()
			:m_lNumThreads(0)
			,m_bIsStarted(false)
		{
			SetSize(0);
		}

		CStrategyPoolT(int iNumThread )
			:m_lNumThreads(0)
			,m_bIsStarted(false)
		{
			SetSize(iNumThread);
		}

		void SetSize(int nNumThreads) 
		{
			if (nNumThreads == 0)
				m_lNumThreads = -ATLS_DEFAULT_THREADSPERPROC;
			else
			if (nNumThreads < 0)
			{
				SYSTEM_INFO si;
				GetSystemInfo(&si);
				m_lNumThreads = static_cast<long>((-nNumThreads) * si.dwNumberOfProcessors);
			}
			else
				m_lNumThreads =nNumThreads;
			if(m_bIsStarted)
				Resize(m_lNumThreads);
		}

		void Start()
		{
			CAutoTryLock lock(m_csThreadPoolLock);
			if(!m_bIsStarted)
			{
				Resize(m_lNumThreads);
				m_bIsStarted = true;
			}
		}
		void Stop()
		{
			m_bIsStarted = false;
			CAutoTryLock lock(m_csThreadPoolLock);
			Resize(0);
		}

		void GetPoolStatistics(CStrategyPoolStatistics& statistics)
		{
			statistics.clear();

			CAutoLock lock(m_csThreadPoolLock);
			statistics.reserve(m_ThreadPool.size());
			CVecThreadPool::iterator itr = m_ThreadPool.begin();
			while(itr != m_ThreadPool.end())
			{
                statistics.insert(statistics.end(), (*itr)->GetStrategyStatistics());
				++itr;

			}   
		}
		bool PushBack(const typename _clsPoolThread::PoolDataTypePtr& Data)
		{
			bool bRet = false;
			if(!m_bIsStarted)
				return false;

			CAutoTryLock lock(m_csThreadPoolLock);

			if(lock.IsLocked())
			{	
				CVecThreadPool::iterator itQueue = m_ThreadPool.end();
				size_t  iMinQueue  =  UINT_MAX;
				int iPos = 0;
				int iCurPos =0;

				// Find first empty or thread with shortest queue
				for(CVecThreadPool::iterator itr = m_ThreadPool.begin(); itr != m_ThreadPool.end(); ++itr, ++iPos)
				{	
					if(*itr!=NULL)
					{
						size_t lSize = (*itr)->GetSize();
						if(!lSize)
						{	iCurPos = iPos;
							itQueue = itr;
							break;
						}
						else
							if(iMinQueue > lSize)
							{
								iMinQueue = lSize;
								itQueue = itr;
								iCurPos = iPos;
							}
					}
				}
				if(itQueue!=m_ThreadPool.end())
				{
					CStrategyPtr pStrategy = *itQueue;
					pStrategy->PushData(Data);
					bRet = true;
				}
			}
			return bRet;
		}
		size_t          GetSize()const {return m_lNumThreads;} 


	private:
		size_t			m_lNumThreads;
		bool			m_bIsStarted;
		CLock			m_csThreadPoolLock;

		CVecThreadPool  m_ThreadPool;


		void Resize(size_t lNewSize)
		{
			CAutoLock lock(m_csThreadPoolLock);
			size_t szCurrentPool = m_ThreadPool.size();
			if(szCurrentPool)
			{
				if(szCurrentPool < lNewSize)
				{
					// increase thread number 
					m_ThreadPool.reserve(szCurrentPool);
					UINT numthreadsToAdd = static_cast<UINT>(lNewSize - szCurrentPool);

					for(UINT i = 0; i<numthreadsToAdd; ++i )
					{
						CVecThreadPool::iterator itrInsert = m_ThreadPool.insert(m_ThreadPool.end(), CStrategyPtr(new _clsPoolThread(i+szCurrentPool)) );
						if(*itrInsert!=NULL)
							(*itrInsert)->Start();
					}
				}
				else
				if(szCurrentPool > lNewSize)
				{
					//  decrease thread number
					size_t szNumThreadsToDelete = szCurrentPool - lNewSize;
					
					CVecThreadPool::reverse_iterator itrRemove = m_ThreadPool.rbegin();
					while(itrRemove != m_ThreadPool.rend())
					{
						if((*itrRemove)!=NULL)
						{
							(*itrRemove)->Clear();
							(*itrRemove)->Stop(true);

						}
						if(!--szNumThreadsToDelete)
							break;
						else
							++itrRemove;
					}
					::SwitchToThread();
					if(!lNewSize)
						m_ThreadPool.clear();
					else
					{
						CVecThreadPool::iterator itrStart = itrRemove.base();
						if(--itrStart != m_ThreadPool.begin())
							m_ThreadPool.erase(itrStart,m_ThreadPool.end());
						else
							m_ThreadPool.clear();

					}
				}

			}
			else
				if(lNewSize)
				{
					m_ThreadPool.reserve(lNewSize);
					for(UINT i=0; i< static_cast<UINT>(lNewSize); ++i)
					{
						CVecThreadPool::iterator itrInsert = m_ThreadPool.insert(m_ThreadPool.end(), CStrategyPtr(new _clsPoolThread(i) ) );
						if(*itrInsert!=NULL)
							(*itrInsert)->Start();
					}
				}
		}
	};
};