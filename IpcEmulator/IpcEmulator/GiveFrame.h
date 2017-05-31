#pragma once
#include <QThread>
#include "SDLPlay.h"
#include "Cffmpeg.h"
#include "DJJQue.h"

class CGiveFrame:
	public QThread
{
public:
	explicit CGiveFrame(CSDLPlay *pSDLPlay, Cffmpeg *pFFmpeg, DJJQue *pDJJQue);
	~CGiveFrame();

protected:
	void run();

private:
	DJJQue		*m_pDJJQue;
	CSDLPlay	*m_pSDLPlay;
	Cffmpeg		*m_pFFmpeg;
};

