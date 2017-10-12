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

				//֪ͨ�����Ѿ�������Ⱦ���ˣ����Ըı䴰�ڴ�С�ˡ�
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
		//av_packet_free �����ͷ�pkt�������ӵ�е�������ռ���ڴ�,av_free_packet�����ͷ� packet���õ�data���ڴ�;
		av_packet_free(&pkt);
	}
}

void CGiveFrame::stop()
{
	printf("Stop GiveFrame thread!\n");
	QMutexLocker locker(&m_mutex);
	m_bStopped = true;
}