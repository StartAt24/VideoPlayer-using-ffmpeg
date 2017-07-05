#include "mgr.h"


CMgr::CMgr()
{
	init();
}


CMgr::~CMgr()
{
}

void CMgr::init()
{
	m_FilePath.clear();
	m_FileSavePath.clear();
	m_videoType = LOCAL_FILE;
	m_ip.clear();
	m_password.clear();
	m_bIsRecord = false;

	m_pFFmpeg = new Cffmpeg(this);
	m_pSDLPlay = new CSDLPlay(this);
	m_pQue = new DJJQue();
	m_pRcv = new CRcvFrame(m_pFFmpeg, m_pQue);

	m_pTheFlash = new CSDLPlay(this);

	m_pGive = new CGiveFrame(m_pSDLPlay, m_pFFmpeg, m_pQue, m_pTheFlash, this);
	
}

void CMgr::close()
{
	delete m_pFFmpeg;
	delete m_pRcv; 
	delete m_pFFmpeg;
	delete m_pSDLPlay;
	delete m_pQue;
}