
#ifndef _EG_THREAD_POLL_H_
#define _EG_THREAD_POLL_H_


#endif // _EG_THREAD_POLL_H_

namespace eg_lib
{

	////////////////////////////////////////////////////////////////////////////
	///// CObjectWorkerInterface
	/////////////////////////////////////////////////////////////////////////////

	class CObjectWorkerInterface
	{
	public:
		virtual void DoTask() = 0;
		virtual ~CObjectWorkerInterface(){};
	};


	////////////////////////////////////////////////////////////////////////////
	///// CEGThreadPool
	/////////////////////////////////////////////////////////////////////////////

	class CEGThreadPool
	{
	public:
		typedef DWORD_PTR RequestType;

		CEGThreadPool(void)/*:m_hEventStop(INVALID_HANDLE_VALUE)*/{};
		virtual ~CEGThreadPool(void){};

		virtual BOOL Initialize(void* /*pvParam*/)
		{
			return SUCCEEDED(::CoInitializeEx(NULL, COINIT_MULTITHREADED))?TRUE:FALSE;
		}

		virtual void Terminate(void* /*pvParam*/)
		{
			::CoUninitialize();
		}

		void Execute(RequestType dw, void* /*pvParam*/, OVERLAPPED* /*pOverlapped*/) throw()
		{
			CObjectWorkerInterface* pTask = reinterpret_cast<CObjectWorkerInterface*>(dw);
			try
			{
				pTask->DoTask(/*pvParam, pOverlapped*/);
			}
			catch (...){ATLASSERT(FALSE);}
			delete pTask;
		};
	};
};
