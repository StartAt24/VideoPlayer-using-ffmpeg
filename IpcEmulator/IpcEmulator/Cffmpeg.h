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
	AVFormatContext		*m_pFormatCtx;					//保存需要读入的文件的格式信息,如流的个数和数量
	AVCodec				*m_pCodec;						//真正的编解码器，其中有编解码需要用到的函数
	AVFrame				*m_pSrcFrame, *m_pDstFrame;
	QString				m_QSfilepath;
	char				m_errBuf[1024];
	int					m_iVideoindex;
	struct SwsContext	*m_pImg_convert_ctx;
	AVPacket			*m_pPacket;

public:
	AVCodecContext		*m_pCodecCtx;					//保存相应流的详细编码信息,如视频的宽、高、编码类型
};

