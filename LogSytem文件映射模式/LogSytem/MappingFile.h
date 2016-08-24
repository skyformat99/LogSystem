#ifndef __MAPPINGFILE__H
#define __MAPPINGFILE__h

#include "MappingFileGurd.h"
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <time.h>
#elif defined __linux__ || defined __linux
#include <unistd.h>
#endif

class MappingFile
{
public:
	MappingFile(char* strFileName, size_t nRollSize, long nFileTell = 0) : m_strFileName(strFileName), m_nRollSize(nRollSize)
		, m_ptr(nullptr), m_cur(nullptr), m_mappingGurd(new MappingFileGurd(getSysDate().c_str(), strFileName, nFileTell))
	{
		std::string strMutex = getSysDate() + "MappingMutex";
		m_mapping = new Mutex(strMutex.c_str());
#ifdef _WIN32
		m_strFileName = getSysDate() + "Mapping";
		m_hFile = CreateFile(strFileName, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile || GetLastError() == ERROR_ALREADY_EXISTS)
		{
			m_hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, m_strFileName.c_str());
			if (!m_hFileMapping)
			{
				m_mapping->lock();
				m_hFileMapping = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, nRollSize, m_strFileName.c_str());
				if (m_hFileMapping)
				{
					m_ptr = MapViewOfFile(m_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
					if (m_ptr)
						m_cur = (char*)m_ptr + m_mappingGurd->getWrittenBytes();
				}
				else if (GetLastError() == ERROR_ALREADY_EXISTS)
				{
					m_hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, m_strFileName.c_str());
					m_ptr = MapViewOfFile(m_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
					if (m_ptr)
						m_cur = (char*)m_ptr + m_mappingGurd->getWrittenBytes();
				}
				m_mapping->unlock();
			}
			else
			{
				m_ptr = MapViewOfFile(m_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (m_ptr)
				{
					m_mapping->lock();
					m_cur = (char*)m_ptr + m_mappingGurd->getWrittenBytes();
					m_mapping->unlock();
				}
			}
		}
#elif defined __linux__ || defined __linux
#endif
	}
	~MappingFile()
	{
#ifdef _WIN32
		FlushViewOfFile(m_ptr, writtenBytes());
		UnmapViewOfFile(m_ptr);
		CloseHandle(m_hFileMapping);
		SetFilePointer(m_hFile, m_mappingGurd->getWrittenBytes(), NULL, FILE_BEGIN);
		SetEndOfFile(m_hFile);  // ╫ь╤онд╪Ч
		CloseHandle(m_hFile);
		delete m_mappingGurd;
#elif defined __linux__ || defined __linux
#endif
	}

	void append(const char* fileContent, int nLen)
	{
		m_mapping->lock();
		memcpy(m_cur, fileContent, nLen);
		m_cur += nLen;
		m_mappingGurd->setWrittenByts(nLen);
		m_mapping->unlock();
	}

	int writtenBytes()
	{
		return m_mappingGurd->getWrittenBytes();
	}

	std::string getSysDate()
	{
		char timebuf[32];
		struct tm tm;
		time_t now;
		now = time(NULL);
		localtime_s(&tm, &now);
		strftime(timebuf, sizeof(timebuf), "%Y%m%d", &tm);
		return timebuf;
	}

private:
	int m_nRollSize;
	std::string m_strFileName;
	MappingFileGurd* m_mappingGurd;
#ifdef _WIN32
	HANDLE m_hFile;
	HANDLE m_hFileMapping;
#elif defined __linux__ || defined __linux
#endif
	void*  m_ptr;
	char*  m_cur;
	Mutex*  m_mapping;
};

#endif