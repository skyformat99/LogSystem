#ifndef __CASYNCLOG__H
#define __CASYNCLOG__H

#include "BufferL.h"
#include <stdint.h>
#include <vector>
#include "Thread.h"
#include "Mutex.h"
#include <memory>
#include "Condition.h"

#define  LOG_LEVEL_NORMAL   1
#define  LOG_LEVEL_WARING   2
#define  LOG_LEVEL_ERROR    3

const unsigned int BUFF_SIZE = 4000 * 1000;

class CAsyncLog
{
public:
	CAsyncLog(const std::string& basename, size_t rollSize, int flushInterval = 3); // basename应该是日期的文件夹的

	~CAsyncLog()
	{
		if (m_bRunning)
			stop();
	}

	void append(const char* logContent, int len); // 放入buffer中，当buffer满后写入到日志文件

public:
	bool start()
	{
		if (!m_bRunning)
		{
			m_bRunning = true;
			m_thread.start(); // 开始线程
		}
		return true;
	}

	void stop() // 停止线程
	{
		m_bRunning = false;
		m_cond.notify();
		m_thread.join(); // 等待线程结束
	}

	void setLogLevel(int logLevel) // 日志级别
	{
		m_LogLevel = logLevel;
	}

	int getLogLevel()
	{
		return m_LogLevel;
	}

private:
	typedef BufferL<BUFF_SIZE> Buffer;
	typedef std::vector<std::unique_ptr<Buffer>> BufferPtrVector;
	typedef std::unique_ptr<Buffer> BufferPtr;

	void threadFunc();

	const int m_nFlushInterval; // 刷新时间
	int       m_nRollSize; // 日志大小
	int       m_LogLevel;
	bool      m_bRunning;
	Thread    m_thread;       // 执行写入日志线程
	std::string m_BaseName;   // 日志名
	BufferPtr m_CurrentBuffer;
	BufferPtr m_NextBuffer;

	Mutex     m_lock;

	Condition m_cond;
	
	BufferPtrVector m_vectBuffers; // 存放写满buffer的vector
};

#endif

