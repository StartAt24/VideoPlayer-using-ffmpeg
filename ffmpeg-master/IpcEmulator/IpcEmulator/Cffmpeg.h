#pragma once
#include <QObject>
#include <QString>
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
#include <libavutil/pixfmt.h>
#ifdef __cplusplus
}
#endif

class CMgr;

/*��Qt���źŲ�ȥ����Cffmpeg��ipcemulator������*/
class Cffmpeg : public QObject
{
	Q_OBJECT
public:
	Cffmpeg(CMgr* mgr);
	~Cffmpeg();
	int ffmpegInit();
	void ffmpegClose();

	int OpenVideoFile(QString filepath);
	int SwsVideo(int dstWidth=0, int dstHeight=0, AVPixelFormat pix_fmt = AV_PIX_FMT_YUV420P);

	int ReadPkt(AVPacket** pOutPkt);
	int ProcessPkt(AVPacket* pPkt, uint8_t** out, int* linesize);

	int SavePre(QString saveFilepath);
	int Saving(AVPacket *pkt);
	void SaveEnd();

	//�϶�����
	int ControlBroadCast(int targetTime);
	void Seek(int targetTime);

	//�����źŸ�B;
	void sendSignalToIpcwindow()
	{
		emit toIpcwindow(m_currentSec);
	}

signals:
	void toIpcwindow(int);

public slots:
void refresh(bool flag)
{
	m_bRefresh = flag;
}

private:
	AVFormatContext		*m_pFormatCtx;					//������Ҫ������ļ��ĸ�ʽ��Ϣ,�����ĸ���������
	AVCodec				*m_pCodec;						//�����ı�������������б������Ҫ�õ��ĺ���
	AVFrame				*m_pSrcFrame, *m_pDstFrame;
	QString				m_QSfilepath;
	char				m_errBuf[1024];
	int					m_iVideoindex;
	struct SwsContext	*m_pImg_convert_ctx;
	AVPacket			*m_pPacket;

	AVFormatContext		*m_pO_fmt_ctx;
	AVCodecContext		*m_pC;

	AVStream			*m_OutStream;					//�����ļ������
	AVStream			*m_InStream;					//�����ļ�������

	CMgr				*m_pMgr;
	
public:
	//�������������Ϣ
	AVCodecContext		*m_pCodecCtx;					//������Ӧ������ϸ������Ϣ,����Ƶ�Ŀ��ߡ���������
	int64_t				m_duration;
	int64_t				m_currentSec;
	bool				m_bRefresh;
};

