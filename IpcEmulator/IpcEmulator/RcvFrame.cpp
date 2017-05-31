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

		/*
		if (m_pFFmpeg->ReadPkt(&pkt))
		{
			m_pDJJQue->PushPacket(pkt);
		}
		else
		{
			//av_free_packet(pkt);
			//av_packet_unref(pkt);
			//av_free_packet(pkt);
			av_packet_free(&pkt);
		}
		*/
	}
}