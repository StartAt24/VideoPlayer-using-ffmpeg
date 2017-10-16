#include "RcvFrame.h"

CRcvFrame::CRcvFrame(Cffmpeg* pFFmpeg, DJJQue* pDJJQue)
{
	m_pFFmpeg = pFFmpeg;
	m_pDJJQue = pDJJQue;
}


CRcvFrame::~CRcvFrame()
{
}

void CRcvFrame::run()
{
	int frameSaved = 1000;
	bool saving = true;
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
	}
}