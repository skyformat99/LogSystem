#include "AsyncLog.h"
#include <assert.h>
#include "LogFile.h"

CAsyncLog::CAsyncLog(const std::string& basename, size_t rollSize, int flushInterval) :
	m_nFlushInterval(flushInterval), m_bRunning(false), m_nRollSize(rollSize), 
	m_thread(std::bind(&CAsyncLog::threadFunc, this), "Logging"), 
	m_CurrentBuffer(new Buffer), 
	m_NextBuffer(new Buffer),
	m_LogLevel(LOG_LEVEL_NORMAL),
	m_BaseName(basename)
{
	m_CurrentBuffer->bzero();
	m_NextBuffer->bzero();
	m_vectBuffers.reserve(16); 
}

void CAsyncLog::append(const char* logContent, int len)
{
	m_lock.lock();
	if (m_CurrentBuffer->unuse() > len) // 先填满buffer
	{
		m_CurrentBuffer->append(logContent, len);
		m_lock.unlock();
		return;
	}
	else
		m_vectBuffers.push_back(BufferPtr(m_CurrentBuffer.release()));

	if (m_NextBuffer.get())
		m_CurrentBuffer.reset(m_NextBuffer.release());
	else
		m_CurrentBuffer.reset(new Buffer);

	m_CurrentBuffer->append(logContent, len);
	m_lock.unlock();
	m_cond.notify();
}

void CAsyncLog::threadFunc()
{
	CLogFile output(m_BaseName, m_nRollSize);

	BufferPtr newBuffer1(new Buffer); 
	BufferPtr newBuffer2(new Buffer); 
	newBuffer1->bzero();
	newBuffer2->bzero();
	BufferPtrVector buffersToWrite;   // 和前台
	buffersToWrite.reserve(16);
	while (m_bRunning)
	{

#ifdef _WIN32
		m_cond.wait(m_nFlushInterval * 1000);
#elif defined __linux__ || defined __linux
		m_cond.wait(m_nFlushInterval);
#endif

		m_lock.lock();
		if (m_vectBuffers.empty() || m_CurrentBuffer->length() == 0)
		{
			m_lock.unlock();
			continue;
		}

		m_vectBuffers.push_back(BufferPtr(m_CurrentBuffer.release()));

		m_CurrentBuffer.reset(newBuffer1.release());
		m_CurrentBuffer->reset();
		buffersToWrite.swap(m_vectBuffers);
		
		if (!m_NextBuffer.get())
		{
			m_NextBuffer.reset(newBuffer2.release());
			m_NextBuffer->reset();
		}

		m_lock.unlock();
		assert(!buffersToWrite.empty());
		for (auto& iter : buffersToWrite)
			output.append(iter->data(), iter->length()); // 写入日志文件

		if (buffersToWrite.size() > 2)
			buffersToWrite.resize(2);
		if (!newBuffer1.get())
		{
			assert(!buffersToWrite.empty());
			newBuffer1.reset(buffersToWrite[0].release());
			newBuffer1->reset();
		}
		if (!newBuffer2.get())
		{
			assert(!buffersToWrite.empty());
			newBuffer2.reset(buffersToWrite[1].release());
			newBuffer2->reset();
		}

		buffersToWrite.clear();
		output.flush();
	}

	m_lock.lock();
	auto p = m_CurrentBuffer.release();
	if (p == NULL || p->length() == 0)
	{
		m_lock.unlock();
		if (p)
			delete p;
		return;
	}
	else
	{
		output.append(p->data(), p->length());
		output.flush();
		m_lock.unlock();
		delete p;
	}
}