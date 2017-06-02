#include "VideoThread.h"
#include "common_def.h"

CVideoThread::CVideoThread()
{
	m_pFFmpeg = new Cffmpeg();
	m_pSDLPlay = new CSDLPlay();
	m_pQue = new DJJQue();
	m_pGive = new CGiveFrame(m_pSDLPlay,m_pFFmpeg, m_pQue);
	m_pRcv = new CRcvFrame(m_pFFmpeg, m_pQue);
	ThreadInit();
}


CVideoThread::~CVideoThread()
{
	delete m_pFFmpeg;
	delete m_pSDLPlay;
}

void CVideoThread::ThreadInit()
{
	m_sVideoFile.clear();
	m_bRunning = true;
	m_bPause = false;
	m_bDecoding = true;
	m_bExitDecode = false;
}

void CVideoThread::run()
{
	//QString testPath("C:/Users/DJJ/Desktop/1080.asf");
	QString testPath("rtsp://admin:admin123@10.255.251.238");
	//while (m_bRunning)
	//{
		DecodeAndShow(testPath);
	//}
}

int CVideoThread::DecodeAndShow(QString videofile)
{
//	AVFrame *yuvFrame = av_frame_alloc();

	m_pFFmpeg->OpenVideoFile(videofile);
	m_pFFmpeg->SwsVideo();
	m_pSDLPlay->SDLCreateWindow(m_pFFmpeg->m_pCodecCtx->width, m_pFFmpeg->m_pCodecCtx->height, m_hWnd);
	
	/*
	while (!m_bExitDecode)
	{
		if(m_pFFmpeg->Decode(yuvFrame))
			m_pSDLPlay->SDLShow(yuvFrame);
	}

	if (yuvFrame)
		av_frame_free(&yuvFrame);
	*/
	m_pRcv->start();
	m_pGive->start();

	return DJJ_SUCCESS;
}

int CVideoThread::GiveFrame(unsigned char* yuvBuf)
{
	return DJJ_SUCCESS;
}

void CVideoThread::SetWindow(HWND hWnd)
{
	m_hWnd = hWnd;
}