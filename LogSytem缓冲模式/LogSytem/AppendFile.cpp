#include "AppendFile.h"
#include <time.h>


void CAppendFile::append(const char* logcontent, int len)
{
	fwrite(logcontent, len, 1, m_fp);
	m_nWrittenBytes += len;
}

void CAppendFile::flush()
{
	fflush(m_fp);
}