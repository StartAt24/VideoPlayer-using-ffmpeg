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

private:
	std::queue<AVFrame*>  m_qFrmaeQue;
	std::queue<AVPacket*> m_qPktQue;

	QMutex				  m_qFrameLock;
	QMutex				  m_qPktLock;

	HANDLE				  m_hFrameSem;
	HANDLE				  m_hPktSem;
};

