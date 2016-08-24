#ifndef _CAPPENDFILE__H
#define _CAPPENDFILE__H
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#elif defined __linux__ || defined __linux
#include <pthread.h>
#endif

class CAppendFile
{
public:
	explicit CAppendFile(std::string fileName) :
		m_nWrittenBytes(0)
	{
#ifdef _WIN32
		fopen_s(&m_fp, fileName.c_str(), "ab+");
#elif defined __linux__ || defined __linux
		m_fp = fopen(fileName.c_str(), "ab+");
#endif
	}
	~CAppendFile()
	{
		fclose(m_fp);
	}

	void append(const char* logcontent, int len);
	void flush();
	size_t writtenBytes() const { return m_nWrittenBytes; }

private:
	FILE* m_fp;
	size_t m_nWrittenBytes;
};

#endif
