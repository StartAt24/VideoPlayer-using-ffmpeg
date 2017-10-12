#pragma once
#include <queue>
#include <QMutex>
#include <libavcodec/avcodec.h>
#include <windows.h>

class DJJQue
{
public:
	DJJQue();
	~DJJQue();
	void PushFrame(AVFrame* frameIn);
	void PopFrame(AVFrame** frameOut);
	void PushPacket(AVPacket* pktIn);
	void PopPacket(AVPacket** pktOut);


	void PktQueClear();
private:
	std::queue<AVFrame*>  m_qFrmaeQue;
	std::queue<AVPacket*> m_qPktQue;

	QMutex				  m_qFrameLock;
	QMutex				  m_qPktLock;

	HANDLE				  m_hFrameSem;			//frame���ź���
	HANDLE				  m_hPktSem;			//packet���ź���
	HANDLE				  m_hEvent;				//�¼��ľ��,�¼�Ϊ: ��push���㹻���֮֡����������.
	
	HANDLE				  m_hLock;				//que��������
	HANDLE				  m_hWrite;				//����д���ź�
	HANDLE				  m_hRead;				//���Զ����ź�

};

