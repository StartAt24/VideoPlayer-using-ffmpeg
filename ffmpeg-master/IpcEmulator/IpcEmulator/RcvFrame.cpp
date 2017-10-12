#include "RcvFrame.h"

CRcvFrame::CRcvFrame(Cffmpeg* pFFmpeg, DJJQue* pDJJQue)
{
	m_pFFmpeg = pFFmpeg;
	m_pDJJQue = pDJJQue;
	m_bStopped = false;
}


CRcvFrame::~CRcvFrame()
{
	stop();
	quit();
	wait();
}

void CRcvFrame::run()
{
	m_bStopped = false;
	while (1)
	{	
		AVPacket* pkt = av_packet_alloc();
		for (;;)
		{
			
			if (m_pFFmpeg->ReadPkt(&pkt))
			{
				m_pDJJQue->PushPacket(pkt);
				break;
			}
		}

		{
			QMutexLocker locker(&m_mutex);
			if (m_bStopped)
				break;
		}
	}
}

void CRcvFrame::stop()
{
	printf("stop RcvFrame thread!\n");
	QMutexLocker locker(&m_mutex);
	m_bStopped = true;
}