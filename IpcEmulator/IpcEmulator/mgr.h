#pragma once
#include "common_def.h"
#include  <QString>
#include "Cffmpeg.h"
#include "SDLPlay.h"
#include "DJJQue.h"
#include "GiveFrame.h"
#include "RcvFrame.h"


enum VideoType
{
	LOCAL_FILE = 0,	//本地视频
	REMOTE_CAMERA,		//相机
};

class CMgr
{
public:
	CMgr();
	~CMgr();

public:
	QString		m_FilePath;				//打开的文件路径 || 播放的RTSP视频流路径
	QString		m_FileSavePath;			//文件保存路径
	VideoType	m_videoType;			//播放的类型，本地视频 or RTSP
	QString		m_ip;					//RTSP ip
	QString		m_password;				//RTSP password
	bool		m_bIsRecord;			//是否在录制视频
	clock_t		m_iCodecCost;			//解码所用的时间，用来让视频能平衡播放;

	//类成员
	Cffmpeg*	m_pFFmpeg;
	CSDLPlay*	m_pSDLPlay;
	DJJQue*		m_pQue;
	CGiveFrame*	m_pGive;
	CRcvFrame*	m_pRcv;

	//修复闪烁现象
	CSDLPlay    *m_pTheFlash;			//只是在正常播放的渲染器被destory的时候 零时渲染一下。
private:
	void init();
	void close();
};

