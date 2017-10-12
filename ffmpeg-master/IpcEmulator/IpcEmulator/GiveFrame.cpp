#include "GiveFrame.h"
#include "mgr.h"

extern HANDLE g_hRefreshEvent;

CGiveFrame::CGiveFrame(CSDLPlay* pSDLPlay, Cffmpeg *pFFmpeg, DJJQue *pDJJQue, CSDLPlay *pTheFlash, CMgr *pMgr)
{
	m_pDJJQue = pDJJQue;
	m_pSDLPlay = pSDLPlay;
	m_pFFmpeg = pFFmpeg;
	m_pTheFlash = pTheFlash;
	m_pMgr = pMgr;

	m_pLock = new QMutex();
	m_bStopped = false;
}


CGiveFrame::~CGiveFrame()
{
	stop();
	quit();
	wait();
	delete m_pLock;
}

void CGiveFrame::run()
{
	m_bStopped = false;
	while (1)
	{
		uint8_t* yuv;
		int lineSize;
		AVPacket* pkt;
		m_pDJJQue->PopPacket(&pkt);

		if (m_pFFmpeg->ProcessPkt(pkt, &yuv, &lineSize))
		{
			if (m_pSDLPlay->m_bResize)
			{
				m_pLock->lock();
					
				//m_pTheFlash->SDLDestery();
				//m_pTheFlash->SDLCreateRenderer(m_pFFmpeg->m_pCodecCtx->width, m_pFFmpeg->m_pCodecCtx->height);
				//m_pTheFlash->SDLFlashShow(test, lineSize);	

				m_pSDLPlay->SDLDestery();
				m_pSDLPlay->SDLCreateRenderer(m_pFFmpeg->m_pCodecCtx->width, m_pFFmpeg->m_pCodecCtx->height);
				m_pSDLPlay->SDLShow(yuv, lineSize);
				m_pSDLPlay->m_bResize = false;
				m_pSDLPlay->m_bRefresh = true;

				//通知窗口已经重新渲染好了，可以改变窗口大小了。
				SetEvent(g_hRefreshEvent);
				m_pLock->unlock();
			}
			else
			{
				m_pSDLPlay->SDLShow(yuv, lineSize);
			}	
			free(yuv);
		}
		
		if (m_pMgr->m_bIsRecord)
		{
			
			m_pFFmpeg->Saving(pkt);
		}

		{
			QMutexLocker locker(&m_mutex);
			if (m_bStopped)
			{
				av_packet_free(&pkt);
				break;
			}
		}
		//av_packet_free 才能释放pkt本身和其拥有的数据所占的内存,av_free_packet不能释放 packet引用的data的内存;
		av_packet_free(&pkt);
	}
}

void CGiveFrame::stop()
{
	printf("Stop GiveFrame thread!\n");
	QMutexLocker locker(&m_mutex);
	m_bStopped = true;
}