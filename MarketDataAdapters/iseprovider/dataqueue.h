#ifndef DATAQUEUE_H
#define DATAQUEUE_H

template<class _Key, class _Type>
class CDataQueueNotBuffered
{
	typedef map<_Key, _Type>		data_queue_t;
	typedef data_queue_t::iterator	data_queue_it_t;

	data_queue_t	m_Data;
	CLock			m_Lock;
	data_queue_it_t	m_It;
	HANDLE			m_hNotification;

public:

	const HANDLE GetNotificationEvent() const
	{
		return m_hNotification;
	}

	CDataQueueNotBuffered():m_It(m_Data.end())
	{
		m_hNotification = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	~CDataQueueNotBuffered()
	{
		CloseHandle(m_hNotification);
	}

	void Push(const _Key	& Key, const _Type & Value);

	BOOL Pop(_Key & Key, _Type & Value);

	BOOL Get(const _Key & Key, _Type & Value);

	void Clear();
};

template<class _Type>
class CDataQueueBuffered
{
	typedef queue<_Type>			data_queue_t;

	data_queue_t	m_Data;
	CLock			m_Lock;
	HANDLE			m_hNotification;

public:

	const HANDLE GetNotificationEvent() const
	{
		return m_hNotification;
	}

	CDataQueueBuffered()
	{
		m_hNotification = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	~CDataQueueBuffered()
	{
		CloseHandle(m_hNotification);
	}

	void Push(const _Type & Value);

	BOOL Pop(_Type & Value);

	void Clear();
};

#endif