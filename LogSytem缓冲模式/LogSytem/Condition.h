#ifndef _CONDITION__H
#define _CONDITION__H
#include "noncopyble.h"
#include "Mutex.h"

#ifdef _WIN32
#include <Windows.h>
#elif defined __linux__ || defined __linux
#include <pthread.h>
#endif

class Condition : private noncopyble // 条件变量 用来唤醒等待线程 等待线程在无数据时睡眠3秒或者被唤醒
{
public:
	Condition()
	{
#ifdef _WIN32
		InitializeConditionVariable(&m_condition);
#elif defined __linux__ || defined __linux
		pthread_cond_init(&m_condition, NULL);
#endif
	}
	~Condition()
	{
		notifyAll();
#ifdef __linux__
		pthread_cond_destroy(&m_condition);
#endif
	}

	bool wait(int waitTime)
	{
#ifdef _WIN32
		return SleepConditionVariableCS(&m_condition, (PCRITICAL_SECTION)m_lock.getMutex(), waitTime);
#elif defined __linux__ || defined __linux
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);

		const int64_t kNanoSecondsPerSecond = 1e9;
		int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

		abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
		abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

		Mutex lock;
		return ETIMEDOUT == pthread_cond_timedwait(&pcond_, (pthread_mutex_t*)mutex_.getPthreadMutex(), &abstime);
#endif
	}

	void notify()
	{
#ifdef _WIN32
		WakeConditionVariable(&m_condition);
#elif defined __linux__ || defined __linux
		pthread_cond_signal(&m_condition);
#endif
	}

	void notifyAll()
	{
#ifdef _WIN32
		WakeAllConditionVariable(&m_condition);
#elif defined __linux__ || defined __linux
		pthread_cond_broadcast(&m_condition);
#endif
	}

private:
#ifdef _WIN32
	CONDITION_VARIABLE m_condition;
#elif defined __linux__ || defined __linux
	pthread_cond_t m_condition;
#endif
	Mutex m_lock;
};


#endif