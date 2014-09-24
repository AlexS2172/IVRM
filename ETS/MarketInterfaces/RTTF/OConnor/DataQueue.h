#ifndef _DATA_QUEUE_H
#define _DATA_QUEUE_H

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

	void Push(const _Type& Value)
	{
		CAutoLock  Lock(m_Lock);
		m_Data.push(Value);
		SetEvent(m_hNotification);
	}

	bool IsEmpty()
	{
		CAutoLock  Lock(m_Lock);
		return m_Data.empty();
	}

	BOOL Pop(_Type& Value)
	{
		CAutoLock  Lock(m_Lock);

		if(m_Data.empty())
			return FALSE;

		Value = m_Data.front();
		m_Data.pop();

		return TRUE;
	}

	void Clear()
	{
		CAutoLock  Lock(&m_Lock);
		m_Data.clear();
	};
};


#endif