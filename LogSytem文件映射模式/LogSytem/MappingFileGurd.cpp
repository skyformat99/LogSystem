#include "MappingFileGurd.h"

MappingFileGurd::MappingFileGurd(const char* strFilePath, const char* strFileName, int nfTellSize) : m_strFilePath(strFilePath), m_handleShared(NULL), m_ptr(NULL)
	, m_nWrittenBytes(0), m_nfTellSize(nfTellSize)
{
#ifdef _WIN32
	std::string m_strMutex = m_strFilePath + "MappingGurdMutex";
	m_mutex = new Mutex(m_strMutex.c_str());

	std::string m_strFile = m_strFilePath + "MappingGurd";
	m_handleShared = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, m_strFile.c_str());
	if (m_handleShared) // ����
	{
		m_ptr = MapViewOfFile(m_handleShared, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		m_mutex->lock();
		// �ȱȽ��ļ���
		if (strcmp(strFileName, (char*)m_ptr) == 0)
			m_nWrittenBytes = *(int*)((char*)m_ptr + 128); // ǰ128�ֽ�����־·����,��4�ֽ����ļ����д�С
		else
			m_nWrittenBytes = 0;
		m_mutex->unlock();
	}
	else  // �������򴴽�
	{
		m_mutex->lock();
		m_handleShared = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, GurdSize, m_strFile.c_str());
		if (m_handleShared)
		{
			m_ptr = MapViewOfFile(m_handleShared, FILE_MAP_ALL_ACCESS, 0, 0, 0); // ӳ���ƫ�Ƽ�ӳ����ֽ���
			memcpy_s(m_ptr, 128, strFileName, 128);
			*(int*)((char*)m_ptr + 128) = m_nfTellSize;
			m_nWrittenBytes = m_nfTellSize;
		}
		else if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			m_handleShared = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, m_strFile.c_str());
			m_ptr = MapViewOfFile(m_handleShared, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			m_nWrittenBytes = *(int*)((char*)m_ptr + 128);
		}
		m_mutex->unlock();
	}
#else
#endif
}

MappingFileGurd::~MappingFileGurd()
{
	UnmapViewOfFile(m_ptr);
	CloseHandle(m_handleShared);
	delete m_mutex;
}
