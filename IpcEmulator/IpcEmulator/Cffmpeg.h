#pragma once
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

class Cffmpeg
{
public:
	Cffmpeg();
	~Cffmpeg();
	int ffmpegInit();
	int OpenVideoFile(QString filepath);
	int SwsVideo(int dstWidth=0, int dstHeight=0, AVPixelFormat pix_fmt = AV_PIX_FMT_YUV420P);
	int Decode(AVFrame* pOutFrame);
	void ffmpegClose();
	int DecodeVideo(const char* filepath);
	int ReadPkt(AVPacket** pOutPkt);
	int ProcessPkt(AVPacket* pPkt, uint8_t** out, int* linesize);

private:
	AVFormatContext		*m_pFormatCtx;					//������Ҫ������ļ��ĸ�ʽ��Ϣ,�����ĸ���������
	AVCodec				*m_pCodec;						//�����ı�������������б������Ҫ�õ��ĺ���
	AVFrame				*m_pSrcFrame, *m_pDstFrame;
	QString				m_QSfilepath;
	char				m_errBuf[1024];
	int					m_iVideoindex;
	struct SwsContext	*m_pImg_convert_ctx;
	AVPacket			*m_pPacket;

public:
	AVCodecContext		*m_pCodecCtx;					//������Ӧ������ϸ������Ϣ,����Ƶ�Ŀ��ߡ���������
};

