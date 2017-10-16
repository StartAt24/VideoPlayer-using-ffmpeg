#pragma once
#include <QThread>
#include <QMutexLocker>
#include "Cffmpeg.h"
#include "DJJQue.h"

class CRcvFrame:
	public QThread
{
	Q_OBJECT
public:
	explicit CRcvFrame(Cffmpeg* pFFmpeg, DJJQue* pDJJQue);
	~CRcvFrame();
	void stop();
	void SetPause(bool);
	bool GetPause();

	void SetMakeSure(bool);
	bool GetMakeSure();

protected:
	void run();
	
private:
	Cffmpeg		*m_pFFmpeg;
	DJJQue		*m_pDJJQue;
	QMutex		m_StopMutex;
	bool		m_bStopped;
	QMutex      m_PauseMutex;
	bool		m_bPause;
	
	bool		m_bMakeSurePause;
	QMutex		m_MakeSureMutex;
};

