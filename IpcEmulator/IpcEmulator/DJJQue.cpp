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