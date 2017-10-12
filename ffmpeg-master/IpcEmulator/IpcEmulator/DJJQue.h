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

	HANDLE				  m_hFrameSem;			//frame的信号量
	HANDLE				  m_hPktSem;			//packet的信号量
	HANDLE				  m_hEvent;				//事件的句柄,事件为: 当push了足够多的帧之后不做解码了.
	
	HANDLE				  m_hLock;				//que操作的锁
	HANDLE				  m_hWrite;				//可以写的信号
	HANDLE				  m_hRead;				//可以读的信号

};

