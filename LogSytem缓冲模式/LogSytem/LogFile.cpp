#define _CRT_SECURE_NO_WARNINGS
#include "LogFile.h"
#include "Mutex.h"
#include <time.h>
#include <stdio.h>
#include <direct.h>

void CLogFile::append(const char* logcontent, int len)
{
	Mutex lock;
	append_unlocked(logcontent, len);
}

void CLogFile::flush()
{
	Mutex lock;
	m_AppendFile->flush();
}

void CLogFile::append_unlocked(const char* logcontent, int len)
{
	if (m_AppendFile->writtenBytes() + m_nFileLen + len> m_nRollSize)
		rollFile(); // 超过一定大小，生成新的日志文件
	m_AppendFile->append(logcontent, len);
	m_AppendFile->flush();
}

bool CLogFile::rollFile()
{
	char szFile[128] = { 0x00 };
	sprintf_s(szFile, "%s\\log_data%d.log", m_strFolderName.c_str(), m_nLogNo);
	
	m_AppendFile.reset(new CAppendFile(szFile)); // 打开新的日志文件进行读写
	m_nFileLen = 0;
	m_nLogNo++;
	return true;
}

void CLogFile::checkLogFile()
{
	time_t now = 0;
	std::string folderPath = getLogFolderName(m_strbasename, &now); // 目录名
	m_strFolderName = folderPath;
	_mkdir(folderPath.c_str());

	FILE* checkFile;
	char szLogFile[128] = { 0x00 };
	do 
	{
		sprintf_s(szLogFile, "%s/log_data%d.log", folderPath.c_str(), m_nLogNo);
		fopen_s(&checkFile, szLogFile, "ab+");
		if (checkFile != NULL)
		{
			fseek(checkFile, 0L, SEEK_END);
			m_nFileLen = ftell(checkFile);
			if (m_nFileLen >= m_nRollSize)
			{
				fclose(checkFile);
				checkFile = NULL;
				m_nLogNo++;
			}
		}
	} while (checkFile == NULL);
	fclose(checkFile);
}

std::string CLogFile::getLogFolderName(const std::string& basename, time_t* now)
{
	std::string filename;
	filename.reserve(basename.size() + 64);
	filename = basename;

	char timebuf[32];
	struct tm* tm;
	*now = time(NULL);
	tm = localtime(now);
	strftime(timebuf, sizeof(timebuf), "%Y%m%d", tm);
	filename += timebuf;

	return filename;
}

CLogFile::~CLogFile()
{
}
