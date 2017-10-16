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
	LOCAL_FILE = 0,	//������Ƶ
	REMOTE_CAMERA,		//���
};

class CMgr
{
public:
	CMgr();
	~CMgr();

public:
	QString		m_FilePath;				//�򿪵��ļ�·�� || ���ŵ�RTSP��Ƶ��·��
	QString		m_FileSavePath;			//�ļ�����·��
	VideoType	m_videoType;			//���ŵ����ͣ�������Ƶ or RTSP
	QString		m_ip;					//RTSP ip
	QString		m_password;				//RTSP password
	bool		m_bIsRecord;			//�Ƿ���¼����Ƶ
	clock_t		m_iCodecCost;			//�������õ�ʱ�䣬��������Ƶ��ƽ�ⲥ��;

	//���Ա
	Cffmpeg*	m_pFFmpeg;
	CSDLPlay*	m_pSDLPlay;
	DJJQue*		m_pQue;
	CGiveFrame*	m_pGive;
	CRcvFrame*	m_pRcv;

	//�޸���˸����
	CSDLPlay    *m_pTheFlash;			//ֻ�����������ŵ���Ⱦ����destory��ʱ�� ��ʱ��Ⱦһ�¡�
private:
	void init();
	void close();
};

