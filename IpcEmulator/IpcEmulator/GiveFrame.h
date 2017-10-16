#pragma once
#include <QThread>
#include "SDLPlay.h"
#include "Cffmpeg.h"
#include "DJJQue.h"
#include <QMutex>

class CMgr;

class CGiveFrame:
	public QThread
{
public:
	explicit CGiveFrame(CSDLPlay *pSDLPlay, Cffmpeg *pFFmpeg, DJJQue *pDJJQue, CSDLPlay *pTheFlash, CMgr *pMgr);
	~CGiveFrame();

protected:
	void run();

private:
	DJJQue		*m_pDJJQue;
	CSDLPlay	*m_pSDLPlay;
	Cffmpeg		*m_pFFmpeg;
	QMutex		*m_pLock;
	CSDLPlay    *m_pTheFlash;
	CMgr		*m_pMgr;
};

