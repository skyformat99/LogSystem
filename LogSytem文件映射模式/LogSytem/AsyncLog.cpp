#include "AsyncLog.h"
#include <assert.h>
#include "LogFile.h"

CAsyncLog::CAsyncLog(const std::string& basename, size_t rollSize, int flushInterval) :
	m_bRunning(false), m_nRollSize(rollSize), 
	m_LogLevel(LOG_LEVEL_NORMAL), // 默认为写正常日志
	m_BaseName(basename), m_LogFile(new CLogFile(basename, rollSize))
{
}

void CAsyncLog::append(const char* logContent, int len)
{
	m_LogFile->append(logContent, len);
}