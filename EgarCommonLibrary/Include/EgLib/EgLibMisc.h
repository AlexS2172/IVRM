//**************************************************************************************************************//
// abstract:	interface & implementation of miscellaneous classes
//
// content:		OmLib::CDataQueueBuffered
//				EgLib::CArrayPtr
//				EgLib::CSmartPtr
//				EgLib::CSmartPtrBase
//				EgLib::CComRecPtr
//				EgLib::CGuid
//				EgLib::CProcessStrategy
// 
// copyright:	(c) 2003 EGAR Technology Inc.
//**************************************************************************************************************//
#pragma once
#include <string>
#include <algorithm>
//**************************************************************************************************************//
// logic guardians
//**************************************************************************************************************//
#if !defined(__EGLIBSYNC_H__)
	#error 'EgLibMisc.h requires EgLibSync.h to be included first'
#endif

#if !defined(__EGLIBTHREAD_H__)
#error 'EgLibMisc.h requires EgLibThread.h to be included first'
#endif

//**************************************************************************************************************//
// header identifier
//**************************************************************************************************************//
#define __EGLIBMISC_H__

//**************************************************************************************************************//
// includes
//**************************************************************************************************************//
#include <deque>
#include <set>
#include <map>
#include <vector>
#include <comutil.h>
#include <ComDef.h>
#include <atlbase.h>
#include <atlstr.h>
#include <time.h>

//#include <EgStdLib/Trace.h>

//**************************************************************************************************************//
// namespace EgLib
//**************************************************************************************************************//
namespace EgLib {

	template< class _ValueType>
	class CDoubleQueue
	{
	public:
		//
		CDoubleQueue()
		{
			m_lQueueSize	= 0;
			m_nInIdx	    = 0;
			m_nOutIdx		= 1;
			m_lStatistics   = 0;
			m_hQueueNotification = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		}

		//
		virtual ~CDoubleQueue()
		{
			if (NULL != m_hQueueNotification) 
				::CloseHandle(m_hQueueNotification);
		//		Reset();
		}

		//
		void Swap()
		{
			std::swap(m_nInIdx, m_nOutIdx);
			//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("\t"));
		}

		//
		void Reset()
		{
			InterlockedExchange(&m_lQueueSize, 0L);

			CAutoLock lockOut(GetLockOut());
			CAutoLock lockIn(GetLockIn());
			ClearIn();
			ClearOut();
			InterlockedExchange(&m_lQueueSize, 0);
		}

		long GetStatistics(bool bReset = true)
		{
			if(bReset)
				return InterlockedExchange(&m_lStatistics, 0L);
			else
				return m_lStatistics;
		}

		//
		void AddIn(const _ValueType& Value)
		{
			InterlockedIncrement(&m_lQueueSize);
			m_buffer[m_nInIdx].push_back(Value);
			SetEvent(m_hQueueNotification);
		}

		//
		void ClearIn(bool bEraseSpace = false)
		{
			if(InterlockedExchangeAdd(&m_lQueueSize, static_cast<long>(m_buffer[m_nInIdx].size()))<0)
				InterlockedExchange(&m_lQueueSize, 0);
			m_buffer[m_nInIdx].clear();
		}

		//
		unsigned int InSize()
		{
			return static_cast<unsigned int>(m_buffer[m_nInIdx].size());
		}

		//
		CLock& GetLockOut() {return m_csLockOut;}
		CLock& GetLockIn()  {return m_csLockIn;}

		//

		//
		void ClearOut(bool bEraseSpace = false, bool bClearNumbers=true)
		{
			if(bClearNumbers && InterlockedExchangeAdd(&m_lQueueSize, static_cast<long>(m_buffer[m_nOutIdx].size()))<0)
				InterlockedExchange(&m_lQueueSize, 0);
    		m_buffer[m_nOutIdx].clear();
		}

		//
		unsigned int OutSize()
		{
			return return static_cast<unsigned int>(m_buffer[m_nOutIdx].size());
		}


		const HANDLE GetNotificationEvent() const
		{
			return m_hQueueNotification;
		}

		void ProcessOutData(HANDLE hStopEvent)
		{
			CAutoLock lockOut(GetLockOut());
			{
				CAutoLock lockIn(GetLockIn());
				//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Swap()"));
				Swap();
			}
	
			std::deque<_ValueType>::iterator itr      = m_buffer[m_nOutIdx].begin();
			std::deque<_ValueType>::iterator itrEnd   = m_buffer[m_nOutIdx].end();
			for(; itr!=itrEnd; ++itr)
			{
				if(::WaitForSingleObject(hStopEvent, 0)!=WAIT_TIMEOUT)
					break;

				OnData(*itr);

				InterlockedIncrement(&m_lStatistics);
				if(InterlockedDecrement(&m_lQueueSize)<0)
					InterlockedExchange(&m_lQueueSize, 0);

			}
			ClearOut(false, false);
		}

		virtual void OnData(_ValueType& Data) = 0;

		long  TotalQueueSize() const { return m_lQueueSize; }
	protected:
		unsigned int					m_nInIdx;
		unsigned int					m_nOutIdx;

		CLock							m_csLockIn;
		CLock 							m_csLockOut;

		std::deque<_ValueType>			m_buffer[2];

		HANDLE			m_hQueueNotification;
		volatile long   m_lQueueSize;
		volatile long   m_lStatistics;

	};

	//**************************************************************************************************************//
	template<class __Key, class _ValueType, class _Pr = less<__Key> >
	class CDoubleMap
	{
	public:
		typedef std::map<__Key, _ValueType, _Pr> SETTYPE;
		typedef typename SETTYPE::const_iterator SETITERATOR;
		//
		CDoubleMap()
		{
			m_nInIdx = 0;
			m_nOutIdx = 1;
			m_hSetNotification = ::CreateEvent(NULL, FALSE, FALSE, NULL);

		}

		//
		virtual ~CDoubleMap()
		{
			if (NULL != m_hSetNotification) 
				::CloseHandle(m_hSetNotification);
			Reset();
		}

		//
		void Swap()
		{
			unsigned int nTemp = m_nInIdx;
			m_nInIdx = m_nOutIdx;
			m_nOutIdx = nTemp;
		}

		//
		void Reset()
		{
			CAutoLock lockOut(GetLockOut());
			CAutoLock lockIn(GetLockOut());
			ClearIn();
			ClearOut();
		}

		//
		CLock& GetLockOut() {return m_csLockOut;}
		CLock& GetLockIn()  {return m_csLockIn;}

		//
		std::pair<typename SETTYPE::iterator, bool> AddIn(const __Key& key, const _ValueType& Value)
		{
			std::pair<SETTYPE::iterator, bool> pr = m_map[m_nInIdx].insert(std::pair<__Key, _ValueType>(key, Value));
			if(!pr.second)
				pr.first->second = Value;

			SetEvent(m_hSetNotification);
			return pr;
		}

		//
		void ClearIn(bool bEraseSpace = false)
		{
			m_map[m_nInIdx].clear();
		}

		//
		unsigned int InSize()
		{
			return m_map[m_nInIdx].size();
		}
		unsigned int OutSize()
		{
			return m_map[m_nOutIdx].size();
		}

		bool empty()
		{
			return  InEmpty() && OutEmpty();
		}
		bool InEmpty()
		{
			return m_map[m_nInIdx].empty();
		}
		bool OutEmpty()
		{
			return m_map[m_nOutIdx].empty();
		}

		SETITERATOR GetOutStart()
		{
			return m_map[m_nOutIdx].begin();
		}
		SETITERATOR GetOutEnd()
		{
			return m_map[m_nOutIdx].end();
		}

		//
		void ClearOut()
		{
			m_map[m_nOutIdx].clear();
		}

		//
	

		const HANDLE GetNotificationEvent() const
		{
			return m_hSetNotification;
		}

	public:
		EgLib::CLock					m_csSet;

	protected:
		unsigned int					m_nInIdx;
		unsigned int					m_nOutIdx;

		CLock							m_csLockIn;
		CLock 							m_csLockOut;

		SETTYPE							m_map[2];
		HANDLE							m_hSetNotification;
	};

	//----------------------------------------------------------------------------------------------------------------------------------------

	template< class _ValueType, class _Pr = less<_ValueType> >
	class CDoubleSet
	{
	public:
		typedef std::set<_ValueType, _Pr> SETTYPE;
		typedef std::deque<_ValueType> VECTYPE;

		typedef typename SETTYPE::const_iterator SETITERATOR;
		//
		CDoubleSet()
		{
			m_nInIdx = 0;
			m_nOutIdx = 1;
			m_hSetNotification = ::CreateEvent(NULL, FALSE, FALSE, NULL);

		}

		//
		virtual ~CDoubleSet()
		{
			if (NULL != m_hSetNotification) 
				::CloseHandle(m_hSetNotification);
			Reset();
		}

		//
		void Swap()
		{
			unsigned int nTemp = m_nInIdx;
			m_nInIdx = m_nOutIdx;
			m_nOutIdx = nTemp;
		}

		//
		void Reset()
		{
			CAutoLock lockOut(GetLockOut());
			CAutoLock lockIn(GetLockOut());
			ClearIn();
			ClearOut();
		}

		//
		CLock& GetLockOut() {return m_csLockOut;}
		CLock& GetLockIn()  {return m_csLockIn;}

		//
		void AddIn(const _ValueType& Value)
		{
			{
				EgLib::CAutoLock lock(m_csSet);
				SETTYPE::iterator itSet = m_set.find(Value);
				if(itSet != m_set.end())
				{
					*itSet = Value;
					return;
				}
				else
					m_set.insert(Value);
			}
			m_buffer[m_nInIdx].push_back(Value);
			SetEvent(m_hSetNotification);
		}

		//
		void ClearIn(bool bEraseSpace = false)
		{
			m_buffer[m_nInIdx].clear();
		}

		//
		unsigned int InSize()
		{
			return m_buffer[m_nInIdx].size();
		}

		//
		void ProcessOutData(HANDLE hStopEvent)
		{
			//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Enter"));
			CAutoLock lockOut(GetLockOut());
			{
				CAutoLock lockIn(GetLockIn());
				//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Swap"));
				Swap();
			}

			for(VECTYPE::iterator itr = m_buffer[m_nOutIdx].begin(); itr!=m_buffer[m_nOutIdx].end(); ++itr)
			{
				if(::WaitForSingleObject(hStopEvent, 0)!=WAIT_TIMEOUT)
				{
					//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Cycle break on: (::WaitForSingleObject(hStopEvent, 0)!=WAIT_TIMEOUT)"));
					break;
				}
				_ValueType data;
				{
					EgLib::CAutoLock lock(m_csSet);
					SETTYPE::iterator itrSet = m_set.find(*itr);
					if(itrSet!=m_set.end())
					{
						data = *itrSet;
						m_set.erase(itrSet);
					}
					else
						continue;
				}
				//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("OnData()"));
				OnData(data);
				Sleep(0);
			}
			//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("ClearOut"));
			ClearOut();
			//EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__ , _T("Exit"));
		}

		virtual void OnData(_ValueType& Data) = 0;


		_ValueType& GetOut(unsigned int nIdx)
		{
			int i = 0;
			for(SETTYPE::iterator itr = m_buffer[m_nOutIdx].begin(); itr!=m_buffer[m_nOutIdx].end(); ++itr)
			{
				if(i == nIdx)
					return *itr;
				++i;
			}
			
			ATLASSERT (FALSE);
			return *GetOutStart();
		}


		SETITERATOR GetOutStart()
		{
			return m_buffer[m_nOutIdx].begin();
		}
		SETITERATOR GetOutEnd()
		{
			return m_buffer[m_nOutIdx].end();
		}

		//
		void ClearOut()
		{
			m_buffer[m_nOutIdx].clear();
		}

		//
		unsigned int OutSize()
		{
			return (unsigned int) m_buffer[m_nOutIdx].size();
		}

		const HANDLE GetNotificationEvent() const
		{
			return m_hSetNotification;
		}

public:
	EgLib::CLock					m_csSet;

	protected:
		unsigned int					m_nInIdx;
		unsigned int					m_nOutIdx;

		CLock							m_csLockIn;
		CLock 							m_csLockOut;

		VECTYPE							m_buffer[2];
		SETTYPE							m_set;

		HANDLE							m_hSetNotification;
	};




//**************************************************************************************************************//
// class CDataQueueBuffered
//**************************************************************************************************************//
template<class _Type, class _Container = std::deque<_Type> >
class CDataQueueBuffered
{
private:

	typedef _Container container_type;

// Data members

	container_type	m_Data;
	CLock			m_Lock;
	HANDLE			m_hNotification;

public:

// Constructor/Destructor

	CDataQueueBuffered()
	{
		m_hNotification = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	~CDataQueueBuffered()
	{
		if (NULL != m_hNotification) ::CloseHandle(m_hNotification);
	}

// Attributes

	const HANDLE GetNotificationEvent() const
	{
		return m_hNotification;
	}

	bool IsEmpty()
	{
		CAutoLock Lock(m_Lock);
		return m_Data.empty();
	}

// Operations

	bool Push(const _Type& Value)
	{
		CAutoLock Lock(m_Lock);
		m_Data.push_back(Value);
		SetEvent(m_hNotification);
		return true;
	}
	
	bool PushFront(const _Type& Value)
	{
		CAutoLock Lock(m_Lock);
		m_Data.push_front(Value);
		SetEvent(m_hNotification);
		return true;
	}
	
	bool Pop(_Type& Value)
	{
		CAutoLock Lock(m_Lock);
		if (m_Data.empty()) return false;
		Value = m_Data.front();
		m_Data.pop_front();
		return true;
	}

	void Clear()
	{
		CAutoLock Lock(m_Lock);
		m_Data.clear();
	};

	size_t GetSize()
	{
		CAutoLock Lock(m_Lock);
		return m_Data.size();
	}
};

//**************************************************************************************************************//
// class CDataQueueBuffered
//**************************************************************************************************************//
template<typename _Type, class _Pr = less<_Type>, class _Container = std::deque<_Type> >
class CDataQueueBufferedUnique
{
private:

	typedef _Container container_type;
	typedef std::set<_Type, _Pr> CDataSet;
	typedef typename CDataSet::iterator CDataSetIt;

// Data members

	container_type	m_Data;
	CDataSet		m_setData;
	CLock			m_Lock;
	HANDLE			m_hNotification;

public:

// Constructor/Destructor

	CDataQueueBufferedUnique()
	{
		m_hNotification = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	~CDataQueueBufferedUnique()
	{
		if (NULL != m_hNotification) ::CloseHandle(m_hNotification);
	}

// Attributes

	const HANDLE GetNotificationEvent() const
	{
		return m_hNotification;
	}

	bool IsEmpty()
	{
		CAutoLock Lock(m_Lock);
		return m_Data.empty();
	}

// Operations

	bool Push(const _Type& Value)
	{
		CAutoLock Lock(m_Lock);
		std::pair<CDataSet::iterator, bool> pr = m_setData.insert(Value);
		if (pr.second) 
		{
			m_Data.push_back(Value);
			SetEvent(m_hNotification);
		}
		else 
			*pr.first = Value;

		return pr.second;
	}
	
	bool PushFront(const _Type& Value)
	{
		CAutoLock Lock(m_Lock);
		std::pair<CDataSet::iterator, bool> pr = m_setData.insert(Value);
		if (pr.second) 
		{
			m_Data.push_front(Value);
			SetEvent(m_hNotification);
		}
		else 
			*pr.first = Value;

		return pr.second;
	}
	
	bool Pop(_Type& Value)
	{
		CAutoLock Lock(m_Lock);
		if (m_Data.empty()) return false;
		Value = m_Data.front();
		m_Data.pop_front();
		CDataSet::iterator iter =  m_setData.find(Value);
		if(iter!=m_setData.end())
		{
			Value = *iter;
			m_setData.erase(iter);
		}
		return true;
	}

	void Clear()
	{
		CAutoLock Lock(m_Lock);
		m_Data.clear();
		m_setData.clear();
	};

	size_t GetSize()
	{
		CAutoLock Lock(m_Lock);
		return m_Data.size();
	}
};

#pragma warning(push)
#pragma warning( disable : 4284 )

template <class T>
class ATL_NO_VTABLE CSmartPtrBase
{
protected:
	CSmartPtrBase (T * ptr = NULL ): m_pData ( new CData<T> (ptr) )
	{ 
		ATLASSERT (m_pData);
		m_pData->AddRef(); 
	}

	explicit CSmartPtrBase ( size_t nSize ): m_pData ( new CData<T> (nSize) )
	{ 
		ATLASSERT (m_pData);
		m_pData->AddRef(); 
	}

public:

	CSmartPtrBase (const CSmartPtrBase <T> & src ): m_pData (src.m_pData)	
	{	
		ATLASSERT (m_pData);	
		m_pData->AddRef(); 
	}

	~CSmartPtrBase ()
	{
		m_pData->Release();
	}

	operator T* ()	const	{return m_pData->GetPtr(); }
	T* operator -> () const	{return m_pData->GetPtr(); }

	T* operator = (const CSmartPtrBase <T> & src)
	{
		m_pData->Release();
		m_pData = NULL;
		m_pData = src.m_pData;
		ATLASSERT(m_pData);
		m_pData->AddRef();

		return m_pData->GetPtr();
	}

	T* operator = (T* pSrc)
	{
		m_pData->Release();
		m_pData = NULL;
		m_pData = new CData <T> (pSrc) ;
		ATLASSERT (m_pData);
		m_pData->AddRef();

		return m_pData->GetPtr();
	}

	bool operator !() const	
	{return !m_pData->GetPtr(); }
	bool operator ==(const T * ptr)	const
	{
		return m_pData->GetPtr() == ptr; 
	}

protected:
	template <class T>
	class CData 
	{
	public:
		CData (T * ptr): m_nRefCount (0), m_ptr (ptr), m_bIsArray(false) { }
		CData (size_t nSize): m_nRefCount (0), m_ptr (new T [nSize]), m_bIsArray(true) { }
		~CData ()	
		{ 
			if (m_bIsArray)
				delete [] m_ptr; 
			else
				delete m_ptr; 
		}
		void AddRef ()	{::InterlockedIncrement (&m_nRefCount);};
		void Release() 
		{ 
			if ( ::InterlockedDecrement(&m_nRefCount) <=0 )  
				delete this;	
		}

		T* GetPtr()	 { return m_ptr; }
	protected:
		T* m_ptr;
		long m_nRefCount;
		bool m_bIsArray;
	};

	CData <T> * m_pData;
};

template <class T>
class CSmartPtr: public CSmartPtrBase <T>
{
public:
	CSmartPtr (T * ptr = NULL ): CSmartPtrBase <T> (ptr) {}
	T* operator = (T* pSrc)  {return CSmartPtrBase <T>::operator = (pSrc); }
};

template <class T>
class CArrayPtr: public CSmartPtrBase <T>
{
public:
	CArrayPtr () {}
	explicit CArrayPtr (size_t nSize): CSmartPtrBase <T> (nSize) {}
	T* operator = (T* pSrc)  {return CSmartPtrBase <T>::operator = (pSrc); }
};

#pragma warning(pop)


class vt_date_span
{
	friend class vt_date;
private:
	static  double get_half_second () { return 1.0/(2.0*60.0*60.0*24.0);}
public:

	vt_date_span (DATE span): m_span (span)
	{
	}

	vt_date_span (long nDays=0, long nHours=0, long nMinutes=0, long nSeconds=0)
	{
		m_span = nDays + (double)nHours / 24 + double (nMinutes) /(24*60) +
			double (nSeconds)/(24*60*60);
	}

	long get_days () const
	{ return  (long) m_span;}
	long get_hours () const
	{ 
		long nHours = static_cast<long>((get_fract(m_span) + get_half_second()) * 24);
		if (nHours >= 24) nHours -= 24;
		return nHours;
	}
	long get_minutes () const
	{ 
		long nMinutes = static_cast<long>(( get_fract (m_span * 24)+ get_half_second()) * 60) ;
		if (nMinutes >= 60 ) nMinutes -= 60;
		return nMinutes;
	}
	long get_seconds () const
	{ 
		long nSeconds = static_cast<long>((get_fract (m_span * 24 *60)+ get_half_second()) * 60);
		if (nSeconds >= 60 ) nSeconds -= 60;
		return nSeconds;
	}
	long get_total_days () const 
	{ return static_cast<long>(m_span);  }
	long get_total_hours() const 
	{ return static_cast<long>(m_span * 24 + get_half_second());  }
	long get_total_minutes() const 
	{ return static_cast<long>(m_span * 24*60 + get_half_second());  }
	long get_total_seconds() const 
	{ return static_cast<long>(m_span * 24*60*60 + get_half_second());  }
protected:
	DATE m_span;

	double get_fract(double dVal) const { return  dVal - (long )dVal ; }
};


class vt_date
{
public:
	vt_date (DATE date): m_date (date){}
	vt_date (): m_date (0)  {}
	vt_date (WORD wYear, WORD wMonth, WORD wDay,   
		WORD wHour = 0, WORD wMinute = 0, WORD wSecond =0 ) : m_date (0)
	{
		SYSTEMTIME st;
		memset (&st, 0, sizeof (st) );

		st.wYear = wYear;
		st.wMonth = wMonth;
		st.wDay = wDay;
		st.wHour = wHour;
		st.wMinute = wMinute;
		st.wSecond = wSecond;

		int nResult = ::SystemTimeToVariantTime (&st, &m_date);
		ATLASSERT (nResult);
	}
	vt_date (const SYSTEMTIME & st)
	{
		int nResult = ::SystemTimeToVariantTime (const_cast <LPSYSTEMTIME> (&st), &m_date);
		ATLASSERT (nResult);
	}

	vt_date (const vt_date & src): m_date (src.m_date) {}
	vt_date & operator = (const vt_date & src)
	{ 
		m_date = src.m_date;
		return *this;
	}

	vt_date_span  operator - (const vt_date & dt )
	{ return vt_date_span (m_date - dt.m_date); }
	vt_date & operator += (const vt_date_span & span)
	{ m_date += span.m_span; return *this;  }

	vt_date & operator -= (const vt_date_span & span)
	{ m_date -= span.m_span; return *this;  }

	operator DATE () const { return m_date;}
	WORD get_day_of_week () const
	{ SYSTEMTIME st; GetSystemTime (st); return st.wDayOfWeek; }
	WORD get_day () const
	{ SYSTEMTIME st; GetSystemTime (st); return st.wDay; }
	WORD get_month () const
	{ SYSTEMTIME st; GetSystemTime (st); return st.wMonth; }
	WORD get_year () const
	{ SYSTEMTIME st; GetSystemTime (st); return st.wYear; }
	WORD get_hour () const
	{ SYSTEMTIME st; GetSystemTime (st); return st.wHour; }
	WORD get_minute () const
	{ SYSTEMTIME st; GetSystemTime (st); return st.wMinute; }
	WORD get_second () const
	{ SYSTEMTIME st; GetSystemTime (st); return st.wSecond; }

	WORD get_millisecond() const
	{ SYSTEMTIME st; GetSystemTime (st); return st.wMilliseconds; }

	bool IsZeroDate () { return m_date == 0;}

	CString Format(LPCSTR lpszFormat) const
	{
		UDATE ud;
		CString strDate;
		if (S_OK == VarUdateFromDate(m_date, 0, &ud))
		{
			struct tm tmTemp;
			tmTemp.tm_sec	= ud.st.wSecond;
			tmTemp.tm_min	= ud.st.wMinute;
			tmTemp.tm_hour	= ud.st.wHour;
			tmTemp.tm_mday	= ud.st.wDay;
			tmTemp.tm_mon	= ud.st.wMonth - 1;
			tmTemp.tm_year	= ud.st.wYear - 1900;
			tmTemp.tm_wday	= ud.st.wDayOfWeek;
			tmTemp.tm_yday	= ud.wDayOfYear - 1;
			tmTemp.tm_isdst	= 0;

			LPTSTR lpszTemp = strDate.GetBufferSetLength(256);
			_tcsftime(lpszTemp, strDate.GetLength(), lpszFormat, &tmTemp);
			strDate.ReleaseBuffer();
		}
		else
		{
			ATLASSERT(false);
		}

		return strDate;
	}

	static vt_date GetCurrentDate ( bool bNoTime = false )
	{
		SYSTEMTIME systime; 
		GetLocalTime (&systime);
		if( bNoTime )
		{
			systime.wHour = 0;
			systime.wMinute = 0;
			systime.wSecond = 0;
			systime.wMilliseconds = 0;
		}

		DATE dt;
		int nResult = ::SystemTimeToVariantTime (&systime, &dt);		
		ATLASSERT(nResult);

		return vt_date(dt);
	}

	static vt_date GetCurrentDateWOS ( bool bNoSM = true )
	{
		SYSTEMTIME systime; 
		GetLocalTime (&systime);
		if( bNoSM )
		{
			systime.wSecond = 0;
			systime.wMilliseconds = 0;
		}

		DATE dt;
		int nResult = ::SystemTimeToVariantTime (&systime, &dt);		
		ATLASSERT(nResult);

		return vt_date(dt);
	}

	void GetSystemTime (SYSTEMTIME & rst) const
	{
		int nResult = ::VariantTimeToSystemTime (m_date, &rst);
		ATLASSERT (nResult);
	}
	_variant_t GetVtDate()
	{
		_variant_t vtRet;
		vtRet = m_date;
		vtRet.vt = VT_DATE;
		return vtRet;
	}
	
	void ClearTime()
	{
		SYSTEMTIME st ;
		::VariantTimeToSystemTime ( m_date , &st);
		st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = 0 ;
		::SystemTimeToVariantTime ( &st , &m_date ) ;
	}

	operator DATE () { return m_date; }
protected:
	DATE m_date;
};

#pragma warning(push)
#pragma warning(disable: 4290)

namespace binary_utils
{
	void var_to_binary (const _variant_t & vtVal, void * pData, size_t nSize) throw(_com_error);
	void binary_to_var (const void * pData, size_t nSize, _variant_t & rvt ) throw(_com_error);
};


//**************************************************************************************************************//
// class CDataMarshaler
//**************************************************************************************************************//
template <class T>
class CDataMarshaler
{
public:
	CDataMarshaler ()
	{
	}

	CDataMarshaler (const T & dataVal ) throw (_com_error)
	{
		SetData (dataVal);
	}

	CDataMarshaler (const _variant_t & rvt) throw (_com_error)
	{
		m_vtData = rvt;
	}

	void SetData (const T & dataVal ) throw (_com_error)
	{
		ULONG nSize  = sizeof (GUID) + sizeof (T);

		SAFEARRAYBOUND rgBounds [1] = { nSize , 0};

		SAFEARRAY * pArray = ::SafeArrayCreate (VT_I1, 1, rgBounds);
		if (!pArray) _com_util::CheckError (E_OUTOFMEMORY);

		HRESULT hr = SafeArrayLock (pArray);
		ATLASSERT (SUCCEEDED (hr));

		memcpy (pArray->pvData, &__uuidof(T), sizeof (GUID));
		memcpy ( (LPBYTE) pArray->pvData + sizeof (GUID), &dataVal, sizeof (T) );

		hr = SafeArrayUnlock (pArray);
		ATLASSERT (SUCCEEDED (hr));

		m_vtData.vt = VT_ARRAY | VT_I1;
		m_vtData.parray = pArray;
	}

	void GetData (T & rdataVal) throw (_com_error)
	{
		if ( ! utils::HasValue (m_vtData) || (m_vtData.vt && (VT_ARRAY | VT_I1 )) == 0 )
			_com_util::CheckError (	E_INVALIDARG);

		SAFEARRAY * pArray = m_vtData.parray;

		HRESULT hr = ::SafeArrayLock (pArray);
		_com_util::CheckError (hr);

		size_t nDataSize = pArray->rgsabound[0].cElements;
		if (nDataSize !=  sizeof (GUID) + sizeof (T) )
			_com_util::CheckError (	E_INVALIDARG);

		GUID guid;
		memcpy (&guid, pArray->pvData, sizeof (GUID));

		if ( ! ::InlineIsEqualGUID (guid, __uuidof(T)) )
			_com_util::CheckError (	E_INVALIDARG);

		memcpy (&rdataVal, (LPBYTE) pArray->pvData + sizeof (GUID), sizeof (T));

		hr = ::SafeArrayUnlock (pArray);
		ATLASSERT (SUCCEEDED(hr));
	}

	operator _variant_t () throw (_com_error)
	{
		return m_vtData;
	}

	CDataMarshaler & operator = (const _variant_t & vtVal) throw (_com_error)
	{
		m_vtData = vtVal;
		return *this;
	}

private:
	variant_t m_vtData;
};

//**************************************************************************************************************//
// namespace utils
//**************************************************************************************************************//
namespace utils
{
	inline bool HasValue(const _variant_t & vtVal)
	{
		return vtVal.vt != VT_NULL && vtVal.vt != VT_EMPTY && vtVal.vt != VT_ERROR;
	}

	template <class T>
		inline HRESULT ComError2ErrInfo (const _com_error & e, T*, const IID & iid = GUID_NULL )
	{
		if (e.ErrorInfo())
			return T::Error (static_cast <BSTR> (e.Description()), iid, e.Error());
		return e.Error();
	}

	template <class T>
		inline HRESULT ComError2ErrInfo (const _com_error & e, T*, LPCOLESTR szDesc, const IID & iid = GUID_NULL )
	{
		if (e.ErrorInfo())
		{
			ATLASSERT (szDesc);
			_bstr_t bstrDesc = szDesc;
			bstrDesc += L" ";
			bstrDesc += e.Description();
			return T::Error (static_cast <BSTR> (bstrDesc), iid, e.Error());
		}
		return e.Error();
	}

	template <class T>
		inline HRESULT ComError2ErrInfo (const _com_error & e, T* pT, UINT uid, const IID & iid = GUID_NULL )
	{
		CComBSTR bstrDesc;
		bstrDesc.LoadString (uid);

		return ComError2ErrInfo (e,pT, bstrDesc, iid);
	}

	inline void CheckError (HRESULT hr)
	{
		_com_issue_error (hr);
	};

	template <class T> 
		inline void CheckError (HRESULT hr, T* pT)
	{
		_com_issue_errorex (hr, pT, __uuidof (T));
	}

	template <class T> 
		inline void CheckError (HRESULT hr, const CComPtr <T> & spT)
	{
		_com_issue_errorex (hr, spT, __uuidof (T));
	}
	template <class T> 
		inline void CheckError (HRESULT hr, const CComQIPtr <T> & spT)
	{
		_com_issue_errorex (hr, spT, __uuidof (T));
	}

	template <class T> 
		inline void CheckError (HRESULT hr, const _com_ptr_t <T> & spT)
	{
		_com_issue_errorex (hr, spT, T::GetIID() );
	}

	inline void ThrowError ( HRESULT hrError, LPCOLESTR szError)
	{
		ATLASSERT (szError);

		CComPtr <ICreateErrorInfo> spCreateErrorInfo;
		HRESULT hr = ::CreateErrorInfo (&spCreateErrorInfo);
		if (SUCCEEDED (hr))
		{
			spCreateErrorInfo->SetDescription (const_cast <LPOLESTR> (szError));
			CComQIPtr <IErrorInfo> spErrInfo = spCreateErrorInfo;
			ATLASSERT (spErrInfo);
			SetErrorInfo (0, spErrInfo);
			throw _com_error (hrError, spErrInfo, true);
		}

	}

	inline void ThrowError ( HRESULT hr, UINT uID)
	{
		CComBSTR bstrError;
		bstrError.LoadString (uID);
		ThrowError (hr, bstrError);
	}

	inline void ThrowErrorNoSetErrorInfo ( HRESULT hrError, LPCOLESTR szError)
	{
		ATLASSERT (szError);

		CComPtr <ICreateErrorInfo> spCreateErrorInfo;
		HRESULT hr = ::CreateErrorInfo (&spCreateErrorInfo);
		if (SUCCEEDED (hr))
		{
			spCreateErrorInfo->SetDescription (const_cast <LPOLESTR> (szError));
			CComQIPtr <IErrorInfo> spErrInfo = spCreateErrorInfo;
			ATLASSERT (spErrInfo);
			throw _com_error (hrError, spErrInfo, true);
		}		
	}

	template<typename T>
	inline T upcase(const T& s)
	{
		/*  Convert S to uppercase     */
		T sResult;
		std::transform(s.begin(), s.end(), std::back_inserter(sResult), std::ptr_fun(::toupper));
		return sResult;
	}

	template<typename T>
	T lwcase(const T& s)
	{
		/*  Convert S to uppercase     */
		T sResult;
		std::transform(s.begin(), s.end(), std::back_inserter(sResult), std::ptr_fun(::tolower));
		return sResult;
	}

	template<typename T>
	T trim(const T& s, char chTrim = ' ')
	{
		/*  Convert S to uppercase     */
		T sResult = "";
		size_t sBegin = s.find_first_not_of(chTrim);
		size_t sEnd   = s.find_last_not_of(chTrim);
		if(sBegin != T::npos)
			sResult = s.substr(sBegin,sEnd!=T::npos?sEnd-sBegin+1:sEnd);
		return sResult;
	}


}; //namespace utils

#pragma warning(pop)

//**************************************************************************************************************//
// class CGuid
//**************************************************************************************************************//
class CGuid
{
// Data members
	GUID m_guid;

// Constructor/Destructor
public:
	CGuid()
	{
		HRESULT hr = CoCreateGuid(&m_guid);
		 if (FAILED(hr) == TRUE) 
			 ZeroMemory(&m_guid, sizeof(m_guid));
	}

	explicit CGuid(const GUID& rGuid)
	{
		m_guid = rGuid;
	}

	explicit CGuid(const std::basic_string<TCHAR>& rsGuid)
	{
#if defined(UNICODE)
		_bstr_t bsGUID = rsGuid.c_str();
		DWORD dwErrCode = ::UuidFromString((unsigned short*)(LPOLESTR)bsGUID, &m_guid);
#else
		DWORD dwErrCode = ::UuidFromString((UCHAR*)rsGuid.c_str(), &m_guid);
#endif
		if (dwErrCode != ERROR_SUCCESS) 
			ZeroMemory(&m_guid, sizeof(m_guid));
		
	}

	~CGuid()
	{
	}

	CGuid(const CGuid& rhs)
	{
		m_guid = rhs.m_guid;
	}

// Operators
	operator std::basic_string<TCHAR>()
	{
		CComBSTR bsGuid(m_guid);
		return std::basic_string<TCHAR>((LPCTSTR)_bstr_t(bsGuid));
	}

	operator _bstr_t ()
	{
		CComBSTR bsGuid(m_guid);
		return _bstr_t(bsGuid);
	}

	operator const GUID& () const
	{
		return m_guid;
	}

	CGuid& operator=(const CGuid& rhs)
	{
		m_guid = rhs.m_guid;
	}

	bool operator == (const CGuid& rhs)
	{
		if (this == &rhs) return true;
		int iRes = memcmp(&m_guid, &rhs.m_guid, sizeof(GUID));
		if (iRes == 0) return true;
		return false;
	}

	bool operator != (const CGuid& rhs)
	{
		if (this == &rhs) return false;
		int iRes = memcmp(&m_guid, &rhs.m_guid, sizeof(GUID));
		if (iRes == 0) return false;
		return true;
	}

	// Operations

	CGuid& NewGuid()
	{
		HRESULT hr = CoCreateGuid(&m_guid);
		if (FAILED(hr) == TRUE)
			ZeroMemory(&m_guid, sizeof(m_guid));
		return *this;
	}
};

//**************************************************************************************************************//
// class CProcessStrategy
//**************************************************************************************************************//
template<typename _Item, int _ThreadIdent = 0, typename _DataQueue = EgLib::CDataQueueBuffered<_Item> >
class CProcessStrategy
	:	public EgLib::CThread
{
	template<typename _Item, int _ThreadIdent>
	class CDataQueueBufferedExt : public _DataQueue
	{

	};
public:

	typedef _Item _Item_t;
	typedef CDataQueueBufferedExt<_Item_t, _ThreadIdent> DataQueue;

private:

	DataQueue m_Queue;

public:

// constructors / destructor

	CProcessStrategy() {}
	virtual ~CProcessStrategy() {}

	size_t GetSize()
	{
		return m_Queue.GetSize();
	}
    
// operations
protected:

	bool Push(const _Item_t&);
	bool Pop(_Item_t&);
	DWORD Start();
	DWORD Stop();
	virtual DWORD Process(DataQueue& rData) = 0;
	void GetDataQueue(DataQueue* pDataQueue);

	// Overrideables from CThread
private:
	virtual UINT Run();
// Message map and handlers

// Implementation

protected:

private:

	//No copies do not implement
	CProcessStrategy(const CProcessStrategy& rhs);
	CProcessStrategy& operator=(const CProcessStrategy& rhs);
};


//**************************************************************************************************************//
// implementation of CProcessStrategy
//**************************************************************************************************************//

//--------------------------------------------------------------------------------------------------------------//
template<typename _Item, int _ThreadIdent, typename _DataQueue /*= EgLib::CDataQueueBuffered<_Item>*/ >
bool CProcessStrategy<_Item, _ThreadIdent, _DataQueue>::Push(const _Item_t& item)
{
	return m_Queue.Push(item);
}

//--------------------------------------------------------------------------------------------------------------//
template<typename _Item, int _ThreadIdent, typename _DataQueue /*= EgLib::CDataQueueBuffered<_Item>*/ >
bool CProcessStrategy<_Item, _ThreadIdent, _DataQueue>::Pop(_Item_t& item)
{
	DWORD dwError = WaitForSingleObject(GetStopEventHandle(), 0L);
	if (dwError == WAIT_OBJECT_0)
		return false;
	else
	if (dwError != WAIT_TIMEOUT)
		return false;

	return m_Queue.Pop(item) != 0;
}

//--------------------------------------------------------------------------------------------------------------//
template<typename _Item, int _ThreadIdent, typename _DataQueue /*= EgLib::CDataQueueBuffered<_Item>*/ >
DWORD CProcessStrategy<_Item, _ThreadIdent, _DataQueue>::Start()
{
	return EgLib::CThread::Start();
}

//--------------------------------------------------------------------------------------------------------------//
template<typename _Item, int _ThreadIdent, typename _DataQueue /*= EgLib::CDataQueueBuffered<_Item>*/ >
DWORD CProcessStrategy<_Item, _ThreadIdent, _DataQueue>::Stop()
{
	return EgLib::CThread::Stop(); 
}

//--------------------------------------------------------------------------------------------------------------//
template<typename _Item, int _ThreadIdent, typename _DataQueue /*= EgLib::CDataQueueBuffered<_Item>*/>
UINT CProcessStrategy<_Item, _ThreadIdent, _DataQueue>::Run()
{
	const HANDLE hNewDataNotification = m_Queue.GetNotificationEvent();
	const HANDLE hStopEvent = EgLib::CThread::GetStopEventHandle();
	const HANDLE hEvents[] = {hNewDataNotification, hStopEvent};

	while(true)
	{
		DWORD dwRes = WaitForMultipleObjects(
				sizeof(hEvents) / sizeof(HANDLE), 
				hEvents, 
				FALSE, 
				INFINITE);

		switch(dwRes)
		{
			case WAIT_OBJECT_0:
			{
				Process(m_Queue);
				break;
			}

			case (WAIT_OBJECT_0 + 1):
				return 0; // end thread

			default:
				break;
		} // switch
	} // while 
	return 0;
}


//--------------------------------------------------------------------------------------------------------------//
template<typename _Item, int _ThreadIdent, typename _DataQueue /*= EgLib::CDataQueueBuffered<_Item>*/>
void CProcessStrategy<_Item, _ThreadIdent, _DataQueue>::GetDataQueue(DataQueue* pDataQueue)
{
	pDataQueue = &m_Queue;
}

//**************************************************************************************************************//
// class CComRecPtr
//**************************************************************************************************************//
template <class T, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,WORD wMinor = 0 >
class CComRecPtr
{
public:
	typedef CComRecPtr<T, plibid, wMajor, wMinor> _This;

	CComRecPtr() : m_pRec (NULL) {}
	CComRecPtr(const _This&  spRec) 
	{
		m_spRecInfo = spRec.m_spRecInfo;
		if (m_spRecInfo && spRec.m_pRec)
			m_spRecInfo->RecordCreateCopy (spRec.m_pRec, (PVOID *) &m_pRec);
	}

	CComRecPtr(const VARIANT & vt) 
	{
		if ( (vt.vt & VT_RECORD) == VT_RECORD  )
		{
			m_spRecInfo = vt.pRecInfo;
			if (m_spRecInfo  && vt.pvRecord  )
				m_spRecInfo->RecordCreateCopy (vt.pvRecord, (PVOID *) &m_pRec);
		}
		else
			ATLASSERT (FALSE);
	}

	~CComRecPtr()
	{
		ReleaseRecord();
	}

	HRESULT Init(const VARIANT & vt)
	{
		HRESULT hr =E_FAIL;

		if ( (vt.vt & VT_RECORD) == VT_RECORD  )
		{
			m_spRecInfo = NULL;
			m_spRecInfo = vt.pRecInfo;
			if (m_spRecInfo  && vt.pvRecord  )
				hr = m_spRecInfo->RecordCreateCopy (vt.pvRecord, (PVOID *) &m_pRec);
		}
		return hr;
	}

	HRESULT Init(T* pVal)
	{
		HRESULT hr = ::GetRecordInfoFromGuids(*plibid, wMajor, wMinor, 
			::GetUserDefaultLCID(), __uuidof(T), &m_spRecInfo); 

		if (SUCCEEDED(hr) && m_spRecInfo != NULL)
		{
			hr = m_spRecInfo->RecordCreateCopy(pVal, (PVOID *) &m_pRec);
			if (!m_pRec)
				hr = E_OUTOFMEMORY;
		}

		return hr;
	}

	HRESULT Init (IRecordInfo* pInfo)
	{
		m_spRecInfo = pInfo;
		m_pRec = (T*) m_spRecInfo->RecordCreate();
		if (!m_pRec)
			return E_OUTOFMEMORY;

		return S_OK;
	}

	HRESULT Init ()
	{
		HRESULT hr = ::GetRecordInfoFromGuids(*plibid, wMajor, wMinor, 
			::GetUserDefaultLCID(), __uuidof(T), &m_spRecInfo); 

		if (SUCCEEDED(hr))
		{
			m_pRec = (T*)	m_spRecInfo->RecordCreate();
			if (!m_pRec)
				hr = E_OUTOFMEMORY;
		}

		return hr;
	}
	
	HRESULT Clear()
	{
		HRESULT hr = S_OK;
		if (m_pRec)
		{
			hr = m_spRecInfo->RecordClear(m_pRec);
			if ( SUCCEEDED(hr) )
				hr = m_spRecInfo->RecordInit(m_pRec);
		}	
		return hr;
	}

	HRESULT CopyTo (VARIANT & rvt)
	{
		HRESULT hr = S_OK;
		::VariantClear(&rvt);
		if (m_pRec)
		{
			rvt.vt = VT_RECORD;
			m_spRecInfo.CopyTo (&rvt.pRecInfo);
			hr = m_spRecInfo->RecordCreateCopy (m_pRec, (PVOID *) &rvt.pvRecord);
		}

		return hr;
	}

	HRESULT CopyTo (T* pVal)
	{
		HRESULT hr = S_OK;
		if (m_pRec != 0 && pVal != 0)
		{
			hr = m_spRecInfo->RecordClear(pVal);
			hr = m_spRecInfo->RecordCopy(m_pRec, pVal);
		}

		return hr;
	}


	T* operator->() const
	{ 
		ATLASSERT (m_pRec);
		return m_pRec; 
	}

	operator T*() const
	{
		return m_pRec;
	}

	IRecordInfo* GetRecordInfo() const
	{
		return m_spRecInfo;
	}

	_This& operator=(const _This& spRec)
	{
		ReleaseRecord();
		m_spRecInfo = spRec.m_spRecInfo;
		if(m_spRecInfo && spRec.m_pRec)
			m_spRecInfo->RecordCreateCopy(spRec.m_pRec, (PVOID *) &m_pRec);
		return *this;
	}

protected:
	void ReleaseRecord()
	{
		if(m_pRec && m_spRecInfo)
		{
			m_spRecInfo->RecordDestroy(m_pRec);
		}

		m_pRec = NULL;
		m_spRecInfo = NULL;
	}

	CComPtr <IRecordInfo> m_spRecInfo;
	T * m_pRec;
};

//---------------------------------------------------------------------------------------------------------------------
namespace  Comparable
{
	inline bool CompareStringByMask(LPCTSTR szString, LPCTSTR szMask)
	{
		bool IsMatch = true;

		while (IsMatch && *szString && *szMask && _tcscmp(szString, szMask) != 0)
		{
			// What is the mask symbol ?
			switch (*szMask)	
			{
			default:	
				// This is ordinary symbol. 
				IsMatch = (*szString == *szMask); // Check it matched with current string symbol

			case _T('?'): 
				// This is '?' (any one symbol in the string)
				szMask++;	// Skip one symbol of the mask
				szString++;	// Skip one symbol of the string
				break;

			case _T('*'): 
				// This is '*' any (zero count included)
				szMask++;	// Skip '*' symbol

				// The next code row checks : 
				// if nothing more in the mask or 
				// estimated symbols in the mask and in the string are equal sequence
				// then it matched
				IsMatch = (*szMask == _T('\0') || _tcscmp(szString, szMask) == 0);

				while (!IsMatch && *szString) // while not matched or string has symbols
				{
					IsMatch = CompareStringByMask(szString, szMask); // recursive call of the function

					if (!IsMatch)	// If not matched
						szString++;	// skip one more symbol from string ('*' eats it)
				}

				// There is string and mask matched
				szString += _tcslen(szString);	// go to end of string
				szMask += _tcslen(szMask);		// go to end of mask 
				break;
			}
		}

		return (IsMatch && _tcscmp(szString, szMask) == 0);
	}
}

} //namespace EgLib


