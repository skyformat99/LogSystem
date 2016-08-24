#include "logExport.h"
#include "AsyncLog.h"
#include <time.h>

#ifdef _WIN32
#define vsnprintf_s(_Buffer,_BufferCount,_MaxCount,_Format,_ArgList)  vsnprintf_s(_Buffer,_BufferCount,_MaxCount,_Format,_ArgList)
#elif defined __linux__ || defined __linux
#define vsnprintf_s(_Buffer,_BufferCount,_MaxCount,_Format,_ArgList) vsnprintf(_Buffer, _BufferCount, _Format, _ArgList)
#endif


CAsyncLog* g_Log = nullptr;
const int nRollSize =100* 1024 * 1024; // 100 M

void formatLog(const char* szFuncId, char* destText, const char* logcontent, int& len)
{
	char timebuf[32];
#ifdef _WIN32
	SYSTEMTIME st;
	GetLocalTime(&st);
#elif defined __linux__ || defined __linux
#endif
	int nLen;
	sprintf_s(timebuf, "%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); // 需要毫秒级别
#ifdef _WIN32
	nLen = sprintf_s(destText, 4096, "[%s][%04x][%-6s]: %s\r\n", timebuf , GetCurrentThreadId(), szFuncId, logcontent);
#elif defined __linux__ || defined __linux
	nLen = sprintf_s(destText, "[%s][%04x]: %s\r\n", timebuf, pthread_self(), logcontent);
#endif
	len = nLen;
	destText[len] = 0;
}

bool InitLog()
{
	if (!g_Log)
	{
		g_Log = new CAsyncLog("test", nRollSize);
		return g_Log->start();
	}
	return true;
}

bool LogIsOpen()
{
	return g_Log != nullptr;
}

int  GetWriteLogLevel()
{
	if (g_Log)
		return g_Log->getLogLevel();
}

bool  CheckWriteLogLevel(int nLogLevel)
{
	if (g_Log)
		return nLogLevel < g_Log->getLogLevel();  // nLogLevel 越小 级别越高
}

void WriteLog(const char* szFuncId, int nLogLevel, const char* szLog)
{
	if (g_Log && nLogLevel <= g_Log->getLogLevel())
	{
		char szLogC[4096] = { 0x00 };
		int nLen = 0;
		formatLog(szFuncId, szLogC, szLog, nLen);
		g_Log->append(szLogC, nLen);
	}
}

bool CloseLog()
{
	if (g_Log)
	{
		g_Log->stop();
		delete g_Log;
	}
	return true;
}