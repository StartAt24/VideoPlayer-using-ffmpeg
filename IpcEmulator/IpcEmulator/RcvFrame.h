#pragma once
#include <QThread>
#include "Cffmpeg.h"
#include "DJJQue.h"

class CRcvFrame:
	public QThread
{
	Q_OBJECT
public:
	explicit CRcvFrame(Cffmpeg* pFFmpeg, DJJQue* pDJJQue);
	~CRcvFrame();

protected:
	void run();
	
private:
	Cffmpeg		*m_pFFmpeg;
	DJJQue		*m_pDJJQue;
};

