#include "RcvFrame.h"
#include "common_def.h"

CRcvFrame::CRcvFrame(Cffmpeg* pFFmpeg, DJJQue* pDJJQue)
{
	m_pFFmpeg = pFFmpeg;
	m_pDJJQue = pDJJQue;
	m_bStopped = false;
	m_bPause = false;
	m_bMakeSurePause = false;
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
			
			if (m_pFFmpeg->ReadPkt(&pkt) == DJJ_SUCCESS)
			{
				m_pDJJQue->PushPacket(pkt);
				break;
			}
			//如果到文件尾，则退出循环
			else if (m_pFFmpeg->ReadPkt(&pkt) == AVERROR_EOF)
			{
				m_bStopped = true;
				break;
			}
		}

		{
			QMutexLocker locker(&m_StopMutex);
			if (m_bStopped)
			{
				break;
			}		
		}
		while (GetPause())
		{
			SetMakeSure(true);
			Sleep(200);
		}
	}
}

void CRcvFrame::stop()
{
	printf("stop RcvFrame thread!\n");
	QMutexLocker locker(&m_StopMutex);
	m_bStopped = true;
}

void CRcvFrame::SetPause(bool pause)
{
	printf("Set CRcvFrame thread pause : %d!\n", pause);
	QMutexLocker locker(&m_PauseMutex);
	m_bPause = pause;
}

bool CRcvFrame::GetPause()
{
	QMutexLocker locker(&m_PauseMutex);
	return m_bPause;
}

void CRcvFrame::SetMakeSure(bool bMakesure)
{
	printf("MakeSure CRcvFrame thread pause : %d\n", bMakesure);
	QMutexLocker locker(&m_MakeSureMutex);
	m_bMakeSurePause = bMakesure;
}

bool CRcvFrame::GetMakeSure()
{
	QMutexLocker locker(&m_MakeSureMutex);
	return m_bMakeSurePause;
}