#include "VideoThread.h"
#include "common_def.h"

CVideoThread::CVideoThread(CMgr* mgr)
{

	m_pMgr = mgr;
	ThreadInit();
}


CVideoThread::~CVideoThread()
{
}

void CVideoThread::ThreadInit()
{
	m_sVideoFile.clear();
	m_bRunning = true;
	m_bPause = false;
	m_bDecoding = true;
	m_bExitDecode = false;
	m_bIsReady = false;
}

void CVideoThread::run()
{
	if (m_bIsReady == false)
	{
		m_pMgr->m_pFFmpeg->SwsVideo();
		//���������Ŵ���
		m_pMgr->m_pSDLPlay->SDLCreateWindow(m_pMgr->m_pFFmpeg->m_pCodecCtx->width, m_pMgr->m_pFFmpeg->m_pCodecCtx->height, m_hWnd);
		//��������˸���Ŵ���
		//m_pMgr->m_pTheFlash->SDLCreateWindow(m_pMgr->m_pFFmpeg->m_pCodecCtx->width, m_pMgr->m_pFFmpeg->m_pCodecCtx->height, m_hWnd);
		m_bIsReady = true;
	}
	
	DecodeAndShow(m_pMgr->m_FilePath);  //����뼰��ʾ
}

int CVideoThread::DecodeAndShow(QString videofile)
{

	m_pMgr->m_pGive->start();
	m_pMgr->m_pRcv->start();

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