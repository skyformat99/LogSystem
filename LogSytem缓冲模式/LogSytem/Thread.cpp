#include "Thread.h"
#include <assert.h>

class ThreadData
{
public:
	ThreadData(std::function<void()>& func) : m_func(func)
	{
	}

	void runInThread()
	{
		m_func();
	}

private:
	std::function<void()> m_func;
};

#ifdef _WIN32
DWORD WINAPI startThread(LPVOID pdata)
#elif defined __linux__ || defined __linux
void* startThread(void*)
#endif
{
	ThreadData* thread = static_cast<ThreadData*>(pdata);
	thread->runInThread();
	delete thread;
	thread = NULL;
	return 0;
}

Thread::Thread(const ThreadFunc& func, const std::string threadName)
{
	m_func = func;
	m_strName = threadName;
	m_bStart = false;
	m_hThread = NULL;
}

bool Thread::start()
{
	assert(!m_bStart);
	m_bStart = true;
	ThreadData* pThreadData = new ThreadData(m_func);
#ifdef _WIN32
	m_hThread = CreateThread(NULL, 0, startThread, pThreadData, 0, NULL);
#elif defined __linux__ || defined __linux
	pthread_create(&m_hThread, NULL, startThread, pThreadData)
#endif
	return true;
}

int Thread::join()
{
#ifdef _WIN32
	return WaitForSingleObject(m_hThread, INFINITE);
#elif defined __linux__ || defined __linux
	return pthread_join(m_hThread, NULL);
#endif
}

Thread::~Thread()
{
}
