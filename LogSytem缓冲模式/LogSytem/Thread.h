#ifndef __NONCOPYBLETHREAD__H
#define __NONCOPYBLETHREAD__H
#include "noncopyble.h"
#include <functional>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#elif defined __linux__ || defined __linux
#include <pthread.h>
#endif

class Thread
{
public:
	typedef std::function<void()> ThreadFunc; // Ïß³Ìº¯Êý
	explicit Thread(const ThreadFunc&, const std::string threadName = std::string());
	~Thread();

	bool start();
	int join();

private:
	std::string m_strName;
	bool        m_bStart;
#ifdef _WIN32
	HANDLE      m_hThread;
#elif defined __linux__ || defined __linux
	tid         m_hThread;
#endif
	ThreadFunc  m_func;
};

#endif