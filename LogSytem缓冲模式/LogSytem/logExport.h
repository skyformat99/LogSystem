#ifndef _LOGEXPORT__H
#define _LOGEXPORT__H

#if defined(_MSC_VER)
#define JSTP_LOGWRITE_API __declspec(dllexport)
#else
#define JSTP_LOGWRITE_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	JSTP_LOGWRITE_API bool InitLog();

	JSTP_LOGWRITE_API bool LogIsOpen();

	JSTP_LOGWRITE_API int  GetWriteLogLevel();

	JSTP_LOGWRITE_API bool CheckWriteLogLevel(int nLogLevel); // 检查该级别是否可以写日志
	
	JSTP_LOGWRITE_API void WriteLog(const char* szFuncId, int nLogLevel, const char* szLog);

	JSTP_LOGWRITE_API bool CloseLog();

#ifdef __cplusplus
}
#endif











#endif
