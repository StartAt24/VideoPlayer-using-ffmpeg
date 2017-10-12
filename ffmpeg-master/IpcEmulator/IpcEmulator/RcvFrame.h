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

protected:
	void run();
	
private:
	Cffmpeg		*m_pFFmpeg;
	DJJQue		*m_pDJJQue;
	QMutex		m_mutex;
	bool		m_bStopped;
};

