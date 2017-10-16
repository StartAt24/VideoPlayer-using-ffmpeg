#pragma once
#include <QThread>
#include "SDLPlay.h"
#include "Cffmpeg.h"
#include "DJJQue.h"
#include <QMutex>
#include <QMutexLocker>

class CMgr;

class CGiveFrame:
	public QThread
{
public:
	explicit CGiveFrame(CSDLPlay *pSDLPlay, Cffmpeg *pFFmpeg, DJJQue *pDJJQue, CSDLPlay *pTheFlash, CMgr *pMgr);
	~CGiveFrame();
	void stop();

	void SetPause(bool);
	bool GetPause();

	void SetMakeSure(bool);
	bool GetMakeSure();
protected:
	void run();

private:
	DJJQue		*m_pDJJQue;
	CSDLPlay	*m_pSDLPlay;
	Cffmpeg		*m_pFFmpeg;
	QMutex		*m_pLock;
	CSDLPlay    *m_pTheFlash;
	CMgr		*m_pMgr;
	bool		m_bStopped;
	QMutex      m_StopMutex;
	bool		m_bPause;
	QMutex		m_PauseMutex;

	bool		m_bMakeSurePause;
	QMutex		m_MakeSureMutex;
};

