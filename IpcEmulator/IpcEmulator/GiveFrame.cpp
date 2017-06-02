#include "GiveFrame.h"

CGiveFrame::CGiveFrame(CSDLPlay* pSDLPlay, Cffmpeg *pFFmpeg, DJJQue *pDJJQue)
{
	m_pDJJQue = pDJJQue;
	m_pSDLPlay = pSDLPlay;
	m_pFFmpeg = pFFmpeg;
}


CGiveFrame::~CGiveFrame()
{

}

void CGiveFrame::run()
{
	while (1)
	{
		uint8_t* test;
		int lineSize;

		AVPacket* pkt;
		m_pDJJQue->PopPacket(&pkt);

		
		if (m_pFFmpeg->ProcessPkt(pkt, &test, &lineSize))
		{
			m_pSDLPlay->SDLShow(test, lineSize);
			free(test);
		}
		//av_packet_free 才能释放pkt本身和其拥有的数据所占的内存
		av_packet_free(&pkt);
	}
}