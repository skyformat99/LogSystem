#ifndef __CASYNCLOG__H
#define __CASYNCLOG__H

#include <stdint.h>
#include <vector>
#include "Mutex.h"
#include <memory>
#include "LogFile.h"
#define  LOG_LEVEL_NORMAL   1
#define  LOG_LEVEL_WARING   2
#define  LOG_LEVEL_ERROR    3

class CAsyncLog
{
public:
	CAsyncLog(const std::string& basename, size_t rollSize, int flushInterval = 1); // basename应该是日期的文件夹的

	~CAsyncLog()
	{
		if (m_bRunning)
			stop();
		delete m_LogFile;
	}

	void append(const char* logContent, int len);

public:
	bool start()
	{
		return true;
	}

	void stop()
	{
		m_bRunning = false;
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

	int       m_nRollSize; // 日志大小
	int       m_LogLevel;
	bool      m_bRunning;
	std::string m_BaseName;   // 日志名

	CLogFile*  m_LogFile;
};

#endif

