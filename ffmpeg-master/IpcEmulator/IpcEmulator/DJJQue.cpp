#include "DJJQue.h"

#define  MAX_CACHE_PKT   50			//��󻺴��package��

DJJQue::DJJQue()
{	
	//�����ź���;
	//��һ��������ʾ��ȫ���ƣ�һ��ֱ�Ӵ���NULL��
	//�ڶ���������ʾ��ʼ��Դ������
	//������������ʾ��󲢷�������
	//���ĸ�������ʾ�ź��������ƣ�����NULL��ʾ�����ź�����
	m_hFrameSem = CreateSemaphore(NULL, 0, 10, NULL);
	m_hPktSem = CreateSemaphore(NULL, 0, MAX_CACHE_PKT, NULL);

	//��ʼ���¼�,�ֶ���λ, ��ʼ����״̬�������¼�;
	m_hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

	//que������������������ �ڶ���������FALSE�����߳����У�TRUE����ʾΪ�����߳�����
	//��Ϊ�������̲߳���������Ӧ���Ƿ��߳�����
	m_hLock = CreateMutex(NULL, FALSE, NULL);

	//����д���ź�,��ʼ��д
	m_hWrite = CreateSemaphore(NULL, 1, 1, NULL);
	//���Զ����ź�,��ʼ���Զ�
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
	//�Ƿ���Խ���pushpacket
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

	//��ȡ�ź�����ǰ��ֵ;
	long count = 0;
	//count��������ǰ���ź�����ֵ��
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

//�������¼�ȥ���ƴ�Ź��̣���������Ϊ���¼����������¼�
void DJJQue::PushPacket(AVPacket* pktIn)
{
	//�������Ԫ�أ�����������
	WaitForSingleObject(m_hLock, INFINITE);
	while (1)
	{
		//������г���==��󻺴���,�ȴ����п�д�źŴ�����ͬʱ�ͷ���;
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
	//�����ȡ��Ԫ��
	WaitForSingleObject(m_hLock, INFINITE);
	while (1)
	{
		//������г���==0���ȴ����пɶ��źŴ�����ͬʱ�ͷ���;
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