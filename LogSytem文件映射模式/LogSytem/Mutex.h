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
	Mutex(const char* mutexName = NULL)
	{
#ifdef _WIN32
		m_mutex = CreateMutex(NULL, FALSE, mutexName);
#elif defined __linux__ || defined __linux
		pthread_mutex_init(&m_cs, NULL);
#endif
	}

	void lock()
	{
#ifdef _WIN32
		WaitForSingleObject(m_mutex, INFINITE);
#elif defined __linux__ || defined __linux
		pthread_mutex_lock(&m_cs);
#endif;
	}

	void unlock()
	{
#ifdef _WIN32
		ReleaseMutex(m_mutex);
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
		return m_mutex;
	}

private:
#ifdef _WIN32
	HANDLE m_mutex;
#elif defined __linux__ || defined __linux
	pthread_mutex_t  m_cs;
#endif
};

#endif