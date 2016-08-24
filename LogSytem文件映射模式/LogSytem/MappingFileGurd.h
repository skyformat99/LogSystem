#ifndef __MAPPINGFILEGURD__H
#define __MAPPINGFILEGURD__h

#ifdef _WIN32
#include <Windows.h>
#elif defined __linux__ || defined __linux
#endif

#include <iostream>
#include "Mutex.h"

const int GurdSize = 140;

class MappingFileGurd // fTellSize 文件初始长度
{
public:
	MappingFileGurd(const char* strFilePath, const char* strFileName, int fTellSize = 0);
	~MappingFileGurd();

public:
	int getWrittenBytes() const
	{
		m_mutex->lock();
		int nLen = m_nWrittenBytes;
		m_mutex->unlock();
		return nLen;
	}

	void setWrittenByts(int& nLen)
	{
		m_nWrittenBytes += nLen;
		*(int*)((char*)m_ptr + 128) = m_nWrittenBytes;
	}

private:
#ifdef _WIN32
	HANDLE m_handleShared;
#else
#endif
	void* m_ptr;
	std::string m_strFilePath;
	int   m_nWrittenBytes;
	int   m_nfTellSize;
	Mutex* m_mutex;
};

#endif

