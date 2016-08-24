#ifndef __CASYNCLOG__H
#define __CASYNCLOG__H

#include "BufferL.h"
#include <stdint.h>
#include <vector>
#include "Thread.h"
#include "Mutex.h"
#include <memory>
#include "Condition.h"

#define  LOG_LEVEL_NORMAL   1
#define  LOG_LEVEL_WARING   2
#define  LOG_LEVEL_ERROR    3

const unsigned int BUFF_SIZE = 4000 * 1000;

class CAsyncLog
{
public:
	CAsyncLog(const std::string& basename, size_t rollSize, int flushInterval = 3); // basenameӦ�������ڵ��ļ��е�

	~CAsyncLog()
	{
		if (m_bRunning)
			stop();
	}

	void append(const char* logContent, int len); // ����buffer�У���buffer����д�뵽��־�ļ�

public:
	bool start()
	{
		if (!m_bRunning)
		{
			m_bRunning = true;
			m_thread.start(); // ��ʼ�߳�
		}
		return true;
	}

	void stop() // ֹͣ�߳�
	{
		m_bRunning = false;
		m_cond.notify();
		m_thread.join(); // �ȴ��߳̽���
	}

	void setLogLevel(int logLevel) // ��־����
	{
		m_LogLevel = logLevel;
	}

	int getLogLevel()
	{
		return m_LogLevel;
	}

private:
	typedef BufferL<BUFF_SIZE> Buffer;
	typedef std::vector<std::unique_ptr<Buffer>> BufferPtrVector;
	typedef std::unique_ptr<Buffer> BufferPtr;

	void threadFunc();

	const int m_nFlushInterval; // ˢ��ʱ��
	int       m_nRollSize; // ��־��С
	int       m_LogLevel;
	bool      m_bRunning;
	Thread    m_thread;       // ִ��д����־�߳�
	std::string m_BaseName;   // ��־��
	BufferPtr m_CurrentBuffer;
	BufferPtr m_NextBuffer;

	Mutex     m_lock;

	Condition m_cond;
	
	BufferPtrVector m_vectBuffers; // ���д��buffer��vector
};

#endif

