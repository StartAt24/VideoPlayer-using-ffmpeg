#include "DJJQue.h"


DJJQue::DJJQue()
{
	//��һ��������ʾ��ȫ���ƣ�һ��ֱ�Ӵ���NULL��
	//�ڶ���������ʾ��ʼ��Դ������
	//������������ʾ��󲢷�������
	//���ĸ�������ʾ�ź��������ƣ�����NULL��ʾ�����ź�����
	m_hFrameSem = CreateSemaphore(NULL, 0, 10, NULL);
	m_hPktSem = CreateSemaphore(NULL, 0, 10, NULL);
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
	m_qFrameLock.unlock();
	ReleaseSemaphore(m_hFrameSem, 1, NULL);
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
	m_qPktLock.lock();
	m_qPktQue.push(pktIn);
	printf("Push in %d\n", iIn++);
	m_qPktLock.unlock();
	ReleaseSemaphore(m_hPktSem, 1, NULL);
}

void DJJQue::PopPacket(AVPacket** pktOut)
{
	WaitForSingleObject(m_hPktSem, INFINITE);
	m_qPktLock.lock();
	*pktOut = m_qPktQue.front();
	m_qPktQue.pop();
	printf("Pop out %d\n", iOut++);
	m_qPktLock.unlock();
}