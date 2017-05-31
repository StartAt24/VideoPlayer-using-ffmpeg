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
		int t;

		AVPacket* pkt;
		m_pDJJQue->PopPacket(&pkt);

		m_pFFmpeg->ProcessPkt(pkt, &test, &t);
		m_pSDLPlay->SDLShow(test, t);

		free(test);
		//av_packet_free �����ͷ�pkt�������ӵ�е�������ռ���ڴ�
		av_packet_free(&pkt);
	}
}