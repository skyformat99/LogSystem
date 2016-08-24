#ifndef _MUTEX__H
#define _MUTEX__H

#ifdef _WIN32
#include <Windows.h>
#elif defined __linux__ || defined __linux
#include <pthread.h>
#endif

class Mutex
{
public:
	Mutex()
	{
#ifdef _WIN32
		InitializeCriticalSection(&m_cs);
#elif defined __linux__ || defined __linux
		pthread_mutex_init(&m_cs, NULL);
#endif
	}

	void lock()
	{
#ifdef _WIN32
		EnterCriticalSection(&m_cs);
#elif defined __linux__ || defined __linux
		pthread_mutex_lock(&m_cs);
#endif;
	}

	void unlock()
	{
#ifdef _WIN32
		LeaveCriticalSection(&m_cs);
#elif defined __linux__ || defined __linux
		pthread_mutex_unlock(&m_cs);
#endif
	}

	~Mutex()
	{
#ifdef defined __linux__ || defined __linux
		pthread_mutex_unlock(&m_cs);
#endif
	}

	void* getMutex()
	{
		return &m_cs;
	}

private:
#ifdef _WIN32
	CRITICAL_SECTION m_cs;
#elif defined __linux__ || defined __linux
	pthread_mutex_t  m_cs;
#endif
};

#endif