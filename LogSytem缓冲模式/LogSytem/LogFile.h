#ifndef _CLOGFILE__H
#define _CLOGFILE__H

#include "noncopyble.h"
#include <iostream>
#include <assert.h>
#include "AppendFile.h"
#include <memory>

class CLogFile : noncopyble
{
public:
	CLogFile(const std::string& basename, size_t rollSize) :
		m_strbasename(basename), m_nRollSize(rollSize),
		m_nLogNo(0), m_nFileLen(0)
	{
		checkLogFile();
		rollFile();
	}
	~CLogFile();

	void append(const char* logcontent, int len);
	void flush();
	bool rollFile(); // �жϵ�ǰĿ¼���Ƿ�����־�ļ�

private:
	void append_unlocked(const char* logcontent, int len);
	static std::string getLogFolderName(const std::string& basename, time_t* now);
	void checkLogFile();

	const std::string m_strbasename;
	const size_t m_nRollSize;

	std::string m_strFolderName;
	int m_nCount;

	int m_nLogNo;

	long m_nFileLen;

	std::unique_ptr<CAppendFile> m_AppendFile;
};

#endif
