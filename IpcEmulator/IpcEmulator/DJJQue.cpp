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
}


DJJQue::~DJJQue()
{
	CloseHandle(m_hFrameSem);
	CloseHandle(m_hPktSem);
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