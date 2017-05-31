#include "Cffmpeg.h"

#include "common_def.h"
#include <stdio.h>
#define __STDC_CONSTANT_MACROS

#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "swscale.lib")
//�ڴ������ܶ�CPUЧ����Ӱ��; �������CPU�Ĵ�ȡ����;


Cffmpeg::Cffmpeg()
{
	ffmpegInit();
}


Cffmpeg::~Cffmpeg()
{
	ffmpegClose();
	av_free_packet(m_pPacket);
}

int Cffmpeg::ffmpegInit()
{
	m_pFormatCtx	= NULL;
	m_pCodecCtx		= NULL;
	m_pCodec = NULL;
	m_pSrcFrame = NULL;
	m_pDstFrame = NULL;
	m_pImg_convert_ctx = NULL;
	m_pPacket = NULL;
	m_QSfilepath.clear();
	memset(m_errBuf, 0, sizeof(m_errBuf));
	m_iVideoindex = -1;

	av_register_all();
	avcodec_register_all();
	avformat_network_init();
	m_pFormatCtx = avformat_alloc_context();
	m_pSrcFrame = av_frame_alloc();				//����һ��AVFrame ����һЩ��ʼ������
	m_pDstFrame = av_frame_alloc();				//����һ��AVFrame ����һЩ��ʼ������
	m_pPacket = (AVPacket *)av_malloc(sizeof(AVPacket));
	return DJJ_SUCCESS;
}

void Cffmpeg::ffmpegClose()
{
	if (m_pFormatCtx)
		avformat_free_context(m_pFormatCtx);
	if (m_pSrcFrame)
		av_frame_free(&m_pSrcFrame);
	if (m_pDstFrame)
		av_frame_free(&m_pDstFrame);
	if (m_pImg_convert_ctx)
		sws_freeContext(m_pImg_convert_ctx);

	/*
	if (m_pPacket)
		av_free_packet(m_pPacket);
	*/
}

int Cffmpeg::OpenVideoFile(QString filepath)
{
	int ret, i;
	QByteArray qbFilepath = filepath.toLatin1();
	char	*szFilepath = qbFilepath.data();

	//ffmpegClose();

	AVDictionary* options = NULL;
	av_dict_set(&options, "rtsp_transport", "udp", 0);

	ret = avformat_open_input(&m_pFormatCtx, szFilepath, NULL, &options);
	if(0 != ret)
	{
		av_strerror(ret, m_errBuf, sizeof(m_errBuf));
		printf("Could not open input stream, filename: %s. Error:%d(%s)\n", szFilepath, ret, m_errBuf);
		avformat_close_input(&m_pFormatCtx);
		return DJJ_FAILURE;
	}

	ret = avformat_find_stream_info(m_pFormatCtx, NULL);
	if (ret < 0)
	{
		av_strerror(ret, m_errBuf, sizeof(m_errBuf));
		printf("Could not find stream information Error: %d(%s)\n", ret, m_errBuf);
		return DJJ_FAILURE;
	}

	//Output Info----------------------------
	printf("------------------------File Information-------------------------\n");
	av_dump_format(m_pFormatCtx, 0, szFilepath, 0);			   //��ȡ�ļ�����Ϣ�� AVFormatContext
	printf("------------------------------END--------------------------------\n");

	return DJJ_SUCCESS;
}

int Cffmpeg::SwsVideo(int dstWidth/* =0 */, int dstHeight/* =0 */, AVPixelFormat pix_fmt /* = AV_PIX_FMT_YUV420P */)
{
	int		i, videoindex, ret;
	uint8_t		*out_buffer;

	m_iVideoindex = -1;
	for (i = 0; i < m_pFormatCtx->nb_streams; i++)
	{
		if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			m_iVideoindex = i;
			break;
		}
	}
	if (-1 == m_iVideoindex)
	{
		printf("Can not find a video stream.\n");
		return DJJ_FAILURE;
	}
	m_pCodecCtx = m_pFormatCtx->streams[m_iVideoindex]->codec;
	m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);  //ͨ��code ID����һ���Ѿ�ע�������Ƶ������;
	if (m_pCodec == NULL)
	{
		printf("Codec not found!\n");
		return DJJ_FAILURE;
	}

	ret = avcodec_open2(m_pCodecCtx, m_pCodec, NULL);	//ʹ�ø�����AvCodec��ʼ��AVCodecContext
	if (ret < 0)		  
	{
		av_strerror(ret, m_errBuf, sizeof(m_errBuf));
		printf("Could not open codec. Error:%d(%s)\n", ret, m_errBuf);
		avcodec_close(m_pCodecCtx);
		return DJJ_FAILURE;
	}

	//avpicture_get_size ����֮��һ֡ͼ��Ĵ�С;
	//av_malloc��װ��malloc������һЩ��ȫ�Բ���;
	out_buffer = (uint8_t *)av_malloc(avpicture_get_size(pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height));

	//avpicture_fill ��pFrameYUV ���� AV_PIX_FMT_YUV420P�ĸ�ʽ���������� out_buffer, ����ת����֮�� Ҳ�Զ����浽��out_buffer;
	avpicture_fill((AVPicture *)m_pDstFrame, out_buffer, pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);

	//��ò���ʼ��һ��SwsContext�ṹ�壬����һ�����ż���ʽת���ĺ��� ��������ű���Ϊ1��1
	m_pImg_convert_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
		(dstWidth == 0 ? m_pCodecCtx->width:dstWidth), (dstHeight == 0? m_pCodecCtx->height:dstHeight), 
		pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);

	return DJJ_SUCCESS;
}


int Cffmpeg::Decode(AVFrame* pOutFrame)
{
	int ret, got_picture=0;
	
	if (av_read_frame(m_pFormatCtx, m_pPacket) >= 0)			//��Դ�ļ������ж�ȡһ��AVPacket���ݰ�;  ����Ƶ���ܰ�����֡��
	{
		if (m_pPacket->stream_index == m_iVideoindex)				//�������Ƶ֡�Ļ������н���
		{
			/**avcodec_decode_video2
			**������Ƶ��AVPacket
			**ʹ��av_read_frame��ȡý��������Ҫ�����жϣ����Ϊ��Ƶ������øú�������
			**����got_picture>0ʱ����ʾ����õ�AVFrame *pFrame, �����Զ�picture���д���;
			**/
			ret = avcodec_decode_video2(m_pCodecCtx, m_pSrcFrame, &got_picture, m_pPacket);

			if (ret < 0)
			{
				av_strerror(ret, m_errBuf, sizeof(m_errBuf));
				printf("Decode Error:%d(%s)\n", ret, m_errBuf);
				return DJJ_FAILURE;
			}

			if (got_picture)
			{
				//ͼƬ����ת�������캯���� 
				sws_scale(m_pImg_convert_ctx, (const uint8_t* const*)m_pSrcFrame->data, m_pSrcFrame->linesize, 0,
					m_pCodecCtx->height, m_pDstFrame->data, m_pDstFrame->linesize);
				memcpy(pOutFrame, m_pDstFrame, sizeof(AVFrame));
			}
		}
		av_free_packet(m_pPacket); //ÿ�ζ�ȡpacket֮ǰ��Ҫ��free;
	}

	if (got_picture)
		return DJJ_SUCCESS;
	else
		return DJJ_FAILURE;
}


int Cffmpeg::DecodeVideo(const char* filepath)
{
	AVFormatContext *pFormatCtx = NULL; //������Ҫ������ļ��ĸ�ʽ��Ϣ,�����ĸ���������
	int				i, videoindex;
	AVCodecContext  *pCodecCtx;			//������Ӧ������ϸ������Ϣ,����Ƶ�Ŀ��ߡ���������
	AVCodec			*pCodec;			//�����ı�������������б������Ҫ�õ��ĺ���

	//���ڱ�������֡�����ݽṹ���������֡�ֱ��Ǳ�����ɫת��ǰ�����֡ͼ��
	AVFrame *pFrame, *pFrameYUV;		//���������ݼ�ת��֮�������;
	uint8_t *out_buffer;
	AVPacket *packet;					//�����ļ�ʱ�Ὣ��/��Ƶ֡���뵽packet��
	int  y_size;
	int  ret, got_picture;
	struct SwsContext *img_convert_ctx;

	FILE *fp_yuv = fopen("output.yuv", "wb+");

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context(); //����һ��AVFormatContext�ṹ�岢��ʼ��;

	/*avformat_open_input ���ܣ� ��һ���ļ�����ȡ�ļ�ͷ
	**��������ṹ��AVIOContext�ĳ�ʼ��;��������Э���ʶ��RTMP ���� file��;
	**ʹ�û����߷ֵ��ļ�Э���Ӧ��URLProtocol,ͨ������ָ��ķ�ʽ����FFMPEG����(��רҵ�ô�)��
	**���ø�URLProtocol�ĺ�������open,read�Ȳ�����
	**/
	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0)
	{
		printf("Could not open input stream.\n");
		return DJJ_FAILURE;
	}

	/*avformat_find_stream_info
	  ��ȡһ������Ƶ���ݲ����һЩ��ص���Ϣ��
	*/
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("Could not find stream information.\n");
		return DJJ_FAILURE;
	}

	videoindex = -1;

	for (i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
		}
	}

	if (videoindex == -1)
	{
		printf("Can not find a video stream.\n");
		return DJJ_FAILURE;
	}

	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);  //ͨ��code ID����һ���Ѿ�ע�������Ƶ������;
	if (pCodec == NULL)
	{
		printf("Codec not found.\n");
		return DJJ_FAILURE;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)		  //ʹ�ø�����AvCodec��ʼ��AVCodecContext
	{
		printf("Could not open codec.\n");
		return DJJ_FAILURE;
	}

	pFrame = av_frame_alloc();							  //����һ��AVFrame ����һЩ��ʼ������
	pFrameYUV = av_frame_alloc();


	
	//avpicture_get_size ����֮��һ֡ͼ��Ĵ�С;
	//av_malloc��װ��malloc������һЩ��ȫ�Բ���;
	out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));

	//avpicture_fill ��pFrameYUV ���� AV_PIX_FMT_YUV420P�ĸ�ʽ���������� out_buffer, ����ת����֮�� Ҳ�Զ����浽��out_buffer;
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));

	//Output Info----------------------------
	printf("------------------------File Information-------------------------\n");
	av_dump_format(pFormatCtx, 0, filepath, 0);			   //��ȡ�ļ�����Ϣ�� AVFormatContext
	printf("------------------------------END--------------------------------\n");

	//��ò���ʼ��һ��SwsContext�ṹ�壬����һ�����ż���ʽת���ĺ��� ��������ű���Ϊ1��1
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);


	uint8_t* p420UV;
	p420UV = (uint8_t*)malloc(pCodecCtx->width * pCodecCtx->height * 3 / 2);



	while (av_read_frame(pFormatCtx, packet) >= 0)			//��Դ�ļ������ж�ȡһ��AVPacket���ݰ�;  ����Ƶ���ܰ�����֡��
	{
		if (packet->stream_index == videoindex)				//�������Ƶ֡�Ļ������н���
		{
			/**avcodec_decode_video2
			**������Ƶ��AVPacket
			**ʹ��av_read_frame��ȡý��������Ҫ�����жϣ����Ϊ��Ƶ������øú�������
			**����got_picture>0ʱ����ʾ����õ�AVFrame *pFrame, �����Զ�picture���д���;
			**/
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet); 

			if (ret < 0)
			{
				printf("Decode Error.\n");
				return DJJ_FAILURE;
			}

			if (got_picture)
			{
				//ͼƬ����ת�������캯���� 
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0,
					pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
			}
		}
		av_free_packet(packet); //ÿ�ζ�ȡpacket֮ǰ��Ҫ��free;
	}

	//flush decoder
	//FIX: FLUSH Frames remained in Codec;
	while (1)
	{
		ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		if (ret < 0)
			break;
		if (!got_picture)
			break;
		sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
			pFrameYUV->data, pFrameYUV->linesize);
		
		printf("Flush Decoder: Succeed to decode 1 frame!\n");
	}

	sws_freeContext(img_convert_ctx);

	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return DJJ_SUCCESS;
}

int Cffmpeg::ReadPkt(AVPacket** pOutPkt)
{
	int ret;
	ret = av_read_frame(m_pFormatCtx, *pOutPkt);
	
 	if (ret >= 0)			//��Դ�ļ������ж�ȡһ��AVPacket���ݰ�;  ����Ƶ���ܰ�����֡��,���ʱ��ΪAVpacket�����ݷ����ڴ�
	{
		if ((*pOutPkt)->stream_index == m_iVideoindex)				//�������Ƶ֡�Ļ������н���
		{ 
		//	printf("i :%d ; ret : %d\n", i++, ret);
			return DJJ_SUCCESS;
		}
		//av_packet_unref(pkt); //ÿ�ζ�ȡpacket֮ǰ��Ҫ��free;
	}
	return DJJ_FAILURE;
}

int Cffmpeg::ProcessPkt(AVPacket* pPkt, uint8_t** out, int* linesize)
{
	int ret, got_picture = 0;
	AVFrame* testFrame;
	testFrame = av_frame_alloc();

	ret = avcodec_decode_video2(m_pCodecCtx, testFrame, &got_picture, pPkt);
	
	if (ret < 0)
	{
		av_strerror(ret, m_errBuf, sizeof(m_errBuf));
		printf("Decode Error:%d(%s)\n", ret, m_errBuf);
		return DJJ_FAILURE;
	}

	if (got_picture)
	{
		//ͼƬ����ת�������캯���� 
		
		sws_scale(m_pImg_convert_ctx, (const uint8_t* const*)testFrame->data, testFrame->linesize, 0,
			m_pCodecCtx->height, m_pDstFrame->data, m_pDstFrame->linesize);
		//memcpy(pOutFrame, m_pDstFrame, sizeof(AVFrame));
		
		(*out) = (uint8_t*)malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, m_pCodecCtx->width, m_pCodecCtx->height));

		memcpy((*out), m_pDstFrame->data[0], avpicture_get_size(AV_PIX_FMT_YUV420P, m_pCodecCtx->width, m_pCodecCtx->height));
		*linesize = m_pDstFrame->linesize[0];
	}

	av_frame_unref(testFrame);
	av_frame_free(&testFrame);

	if (got_picture)
		return DJJ_SUCCESS;
	else
		return DJJ_FAILURE;
}
