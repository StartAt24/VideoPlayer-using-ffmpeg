#include "DJJQue.h"

#define  MAX_CACHE_PKT   50			//最大缓存的package数

DJJQue::DJJQue()
{	
	//创建信号量;
	//第一个参数表示安全控制，一般直接传入NULL。
	//第二个参数表示初始资源数量。
	//第三个参数表示最大并发数量。
	//第四个参数表示信号量的名称，传入NULL表示匿名信号量。
	m_hFrameSem = CreateSemaphore(NULL, 0, 10, NULL);
	m_hPktSem = CreateSemaphore(NULL, 0, MAX_CACHE_PKT, NULL);

	//初始化事件,手动置位, 初始触发状态的匿名事件;
	m_hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

	//que操作的锁（互斥量） 第二个参数：FALSE：非线程所有，TRUE：表示为创建线程所有
	//因为是两个线程操作，所以应该是非线程所有
	m_hLock = CreateMutex(NULL, FALSE, NULL);

	//可以写的信号,初始可写
	m_hWrite = CreateSemaphore(NULL, 1, 1, NULL);
	//可以读的信号,初始可以读
	m_hRead = CreateSemaphore(NULL, 0, 1, NULL);
}


DJJQue::~DJJQue()
{
	CloseHandle(m_hFrameSem);
	CloseHandle(m_hPktSem);
	CloseHandle(m_hEvent);

	CloseHandle(m_hLock);
	CloseHandle(m_hWrite);
	CloseHandle(m_hRead);
}

void DJJQue::PushFrame(AVFrame* frameIn)
{
	m_qFrameLock.lock();
	m_qFrmaeQue.push(frameIn);
	ReleaseSemaphore(m_hFrameSem, 1, NULL);
	m_qFrameLock.unlock();
}

void DJJQue::PopFrame(AVFrame** frameOut)
{
	WaitForSingleObject(m_hFrameSem, INFINITE);
	m_qFrameLock.lock();
	*frameOut = m_qFrmaeQue.front();
	m_qFrmaeQue.pop();
	m_qFrameLock.unlock();
}

/*
int iIn = 0, iOut = 0;
void DJJQue::PushPacket(AVPacket* pktIn)
{
	//是否可以进行pushpacket
	WaitForSingleObject(m_hEvent, INFINITE);
	m_qPktLock.lock();
	m_qPktQue.push(pktIn);

	printf("Push in %d\n", iIn++);
	long count = 0;
	ReleaseSemaphore(m_hPktSem, 1, &count);
	if (count >= MAX_CACHE_PKT-1)
	{
		ResetEvent(m_hEvent);
		printf("ResetEvent ~ Semaphore count: %ld \n", count);
	}
	
	m_qPktLock.unlock();
}

void DJJQue::PopPacket(AVPacket** pktOut)
{
	WaitForSingleObject(m_hPktSem, INFINITE);
	m_qPktLock.lock();
	*pktOut = m_qPktQue.front();
	m_qPktQue.pop();
	printf("Pop out %d\n", iOut++);

	//获取信号量当前的值;
	long count = 0;
	//count返回增加前的信号量的值；
	ReleaseSemaphore(m_hPktSem, 1, &count);
	WaitForSingleObject(m_hPktSem, INFINITE);

	printf("pop out ~ Semaphore count: %ld.\n", count);
	if (count < MAX_CACHE_PKT - 1)
	{
		SetEvent(m_hEvent);
	}

	m_qPktLock.unlock();
}
*/

int iIn=0, iOut=0;

//用两个事件去控制存放过程，即：队列为空事件，队列满事件
void DJJQue::PushPacket(AVPacket* pktIn)
{
	//互斥放入元素，用锁来操作
	WaitForSingleObject(m_hLock, INFINITE);
	while (1)
	{
		//如果队列长度==最大缓存量,等待队列可写信号触发，同时释放锁;
		if (m_qPktQue.size() <= MAX_CACHE_PKT - 1)
		{
			m_qPktQue.push(pktIn);
			printf("Que push in %d.\n", iIn++);
			ReleaseSemaphore(m_hRead, 1, NULL);
			break;
		}
		else
		{
			SignalObjectAndWait(m_hLock, m_hWrite, INFINITE, FALSE);
			WaitForSingleObject(m_hLock, INFINITE);
		}
	}
	ReleaseMutex(m_hLock);
}

void DJJQue::PopPacket(AVPacket** pktOut)
{
	//互斥的取出元素
	WaitForSingleObject(m_hLock, INFINITE);
	while (1)
	{
		//如果队列长度==0，等待队列可读信号触发，同时释放锁;
		if (!m_qPktQue.empty())
		{
			*pktOut = m_qPktQue.front();
			m_qPktQue.pop();
			printf("Que pop out : %d.\n", iOut++);
			ReleaseSemaphore(m_hWrite, 1, NULL);
			break;
		}
		else
		{
			SignalObjectAndWait(m_hLock, m_hRead, INFINITE, FALSE);
			WaitForSingleObject(m_hLock, INFINITE);
		}
	}
	
	ReleaseMutex(m_hLock);
}
 
void DJJQue::PktQueClear()
{
	std::queue<AVPacket*> empty;
	m_qPktQue.swap(empty);
}