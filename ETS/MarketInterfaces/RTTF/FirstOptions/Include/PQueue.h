#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#ifdef _WIN32

#else
	#include <pthread.h>
#endif

#include <queue>

template<class T>
class PQueue
{
private:

#ifdef _WIN32
	CRITICAL_SECTION cs;
	HANDLE hevent;
#else
	pthread_mutex_t mutex;
	pthread_cond_t cond;
#endif

	std::queue<T> q;

public:
	PQueue()
	{
#ifdef _WIN32
		InitializeCriticalSection(&cs);
		hevent = CreateEvent(NULL, TRUE, FALSE, NULL);
#else
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&cond, NULL);
#endif
	}
	virtual ~PQueue()
	{
#ifdef _WIN32
		DeleteCriticalSection(&cs);
		CloseHandle(hevent);
#else
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
#endif
	}

	void push(const T&);
	T pop();
	bool empty();
};

template<class T>
void PQueue<T>::push(const T& t)
{
#ifdef _WIN32
	EnterCriticalSection(&cs);
#else
	pthread_mutex_lock(&mutex);
#endif

	q.push(t);

#ifdef _WIN32
	SetEvent(hevent);
	LeaveCriticalSection(&cs);
#else
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
#endif
}

template<class T>
T PQueue<T>::pop(void)
{
#ifdef _WIN32
	EnterCriticalSection(&cs);
	while (q.empty())
	{
		LeaveCriticalSection(&cs);
		WaitForSingleObject(hevent, INFINITE);
		ResetEvent(hevent);
		EnterCriticalSection(&cs);
	}
#else
	pthread_mutex_lock(&mutex);
	while (q.empty())
		pthread_cond_wait(&cond, &mutex);
#endif

	T t = q.front();
	q.pop();

#ifdef _WIN32
	LeaveCriticalSection(&cs);
#else
	pthread_mutex_unlock(&mutex);
#endif

	return t;
}

/*
template<class T>
void PQueue<T>::pop(void)
{
#ifdef _WIN32
	EnterCriticalSection(&cs);
#else
	pthread_mutex_lock(&mutex);
#endif

	q.pop();

#ifdef _WIN32
	LeaveCriticalSection(&cs);
#else
	pthread_mutex_unlock(&mutex);
#endif
}
*/

template<class T>
bool PQueue<T>::empty(void)
{
#ifdef _WIN32
	EnterCriticalSection(&cs);
#else
	pthread_mutex_lock(&mutex);
#endif

	bool mt = q.empty();

#ifdef _WIN32
	LeaveCriticalSection(&cs);
#else
	pthread_mutex_unlock(&mutex);
#endif

	return mt;
}

#endif //  __PQUEUE_H__
