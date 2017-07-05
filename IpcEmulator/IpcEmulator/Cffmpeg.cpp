#include "Cffmpeg.h"

#include "common_def.h"
#include <stdio.h>
#define __STDC_CONSTANT_MACROS

#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "swscale.lib")
//内存对齐可能对CPU效率有影响; 具体根据CPU的存取粒度;
#include "mgr.h"

Cffmpeg::Cffmpeg(CMgr* mgr)
{
	m_pMgr = mgr;
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

	m_pO_fmt_ctx = NULL;
	m_pC = NULL;
	m_OutStream = NULL;
	m_InStream = NULL;

	av_register_all();
	avcodec_register_all();
	avformat_network_init();
	m_pFormatCtx = avformat_alloc_context();
	m_pSrcFrame = av_frame_alloc();				//申请一个AVFrame 并做一些初始化工作
	m_pDstFrame = av_frame_alloc();				//申请一个AVFrame 并做一些初始化工作
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

	//-----写文件的释放流程
	/*
	avcodec_close(m_pO_fmt_ctx->streams[0]->codec);
	av_freep(&m_pO_fmt_ctx->streams[0]->codec);
	av_freep(&m_pO_fmt_ctx->streams[0]);

	avio_close(m_pO_fmt_ctx->pb);
	av_free(m_pO_fmt_ctx);
	*/
	//avformat_free_context(m_pO_fmt_ctx);
}

int Cffmpeg::OpenVideoFile(QString filepath)
{
	int ret, i;
 	QByteArray qbFilepath = filepath.toLatin1();
	char	*szFilepath = qbFilepath.data();

	AVDictionary* options = NULL;
	av_dict_set(&options, "rtsp_transport", "udp", 0);
	av_dict_set(&options, "max_delay", "3000000", 0);
	av_dict_set(&options, "stimeout", "2000000", 0);

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
	av_dump_format(m_pFormatCtx, 0, szFilepath, 0);			   //获取文件的信息到 AVFormatContext
	printf("------------------------------END--------------------------------\n");

	m_iVideoindex = -1;
	//nb_streams代表有几路视频,一般是2路：即音频和视频，顺序不一定。
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

	return DJJ_SUCCESS;
}

int Cffmpeg::SwsVideo(int dstWidth/* =0 */, int dstHeight/* =0 */, AVPixelFormat pix_fmt /* = AV_PIX_FMT_YUV420P */)
{
	int		i, ret;
	uint8_t		*out_buffer;

	m_pCodecCtx = m_pFormatCtx->streams[m_iVideoindex]->codec;
	m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);  //通过code ID查找一个已经注册的音视频编码器;
	//m_pCodec = avcodec_find_decoder(AV_CODEC_ID_HEVC);
	if (m_pCodec == NULL)
	{
		printf("Codec not found!\n");
		return DJJ_FAILURE;
	}

	ret = avcodec_open2(m_pCodecCtx, m_pCodec, NULL);	//使用给定的AvCodec初始化AVCodecContext
	if (ret < 0)		  
	{
		av_strerror(ret, m_errBuf, sizeof(m_errBuf));
		printf("Could not open codec. Error:%d(%s)\n", ret, m_errBuf);
		avcodec_close(m_pCodecCtx);
		return DJJ_FAILURE;
	}

	//avpicture_get_size 解码之后一帧图像的大小;
	//av_malloc封装了malloc并做了一些安全性操作;
	out_buffer = (uint8_t *)av_malloc(avpicture_get_size(pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height));

	//avpicture_fill 将pFrameYUV 按照 AV_PIX_FMT_YUV420P的格式“关联”到 out_buffer, 数据转换完之后 也自动保存到了out_buffer;
	avpicture_fill((AVPicture *)m_pDstFrame, out_buffer, pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);

	//获得并初始化一个SwsContext结构体，这是一个缩放及格式转换的函数 这里的缩放比例为1：1
	
	m_pImg_convert_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
		(dstWidth == 0 ? m_pCodecCtx->width:dstWidth), (dstHeight == 0? m_pCodecCtx->height:dstHeight), 
		pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
	/*
	//测试H265的播放转码;
	m_pImg_convert_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_FMT_YUV420P,
		(dstWidth == 0 ? m_pCodecCtx->width : dstWidth), (dstHeight == 0 ? m_pCodecCtx->height : dstHeight),
		pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
	*/
	return DJJ_SUCCESS;
}

int Cffmpeg::Decode(AVFrame* pOutFrame)
{
	int ret, got_picture=0;
	
	if (av_read_frame(m_pFormatCtx, m_pPacket) >= 0)			//从源文件容器中读取一个AVPacket数据包;  （音频可能包含多帧）
	{
		if (m_pPacket->stream_index == m_iVideoindex)				//如果是视频帧的话，进行解码
		{
			/**avcodec_decode_video2
			**解码视频流AVPacket
			**使用av_read_frame读取媒体流后需要进行判断，如果为视频流则调用该函数解码
			**返回got_picture>0时，表示解码得到AVFrame *pFrame, 其后可以对picture进行处理;
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
				//图片像素转换、拉伸函数； 
				sws_scale(m_pImg_convert_ctx, (const uint8_t* const*)m_pSrcFrame->data, m_pSrcFrame->linesize, 0,
					m_pCodecCtx->height, m_pDstFrame->data, m_pDstFrame->linesize);
				memcpy(pOutFrame, m_pDstFrame, sizeof(AVFrame));
			}
		}
		av_free_packet(m_pPacket); //每次读取packet之前需要先free;
	}

	if (got_picture)
		return DJJ_SUCCESS;
	else
		return DJJ_FAILURE;
}

int Cffmpeg::DecodeVideo(const char* filepath)
{
	AVFormatContext *pFormatCtx = NULL; //保存需要读入的文件的格式信息,如流的个数和数量
	int				i, videoindex;
	AVCodecContext  *pCodecCtx;			//保存相应流的详细编码信息,如视频的宽、高、编码类型
	AVCodec			*pCodec;			//真正的编解码器，其中有编解码需要用到的函数

	//用于保存数据帧的数据结构这里的两个帧分别是保存颜色转换前后的两帧图像
	AVFrame *pFrame, *pFrameYUV;		//解码后的数据及转换之后的数据;
	uint8_t *out_buffer;
	AVPacket *packet;					//解析文件时会将音/视频帧读入到packet中
	int  y_size;
	int  ret, got_picture;
	struct SwsContext *img_convert_ctx;

	FILE *fp_yuv = fopen("output.yuv", "wb+");

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context(); //申请一个AVFormatContext结构体并初始化;

	/*avformat_open_input 功能： 打开一个文件，读取文件头
	**输入输出结构体AVIOContext的初始化;输入数据协议的识别（RTMP 或者 file）;
	**使用获得最高分的文件协议对应的URLProtocol,通过函数指针的方式，与FFMPEG链接(非专业用词)；
	**调用该URLProtocol的函数进行open,read等操作；
	**/
	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0)
	{
		printf("Could not open input stream.\n");
		return DJJ_FAILURE;
	}

	/*avformat_find_stream_info
	  读取一部分音频数据并获得一些相关的信息；
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
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);  //通过code ID查找一个已经注册的音视频编码器;
	if (pCodec == NULL)
	{
		printf("Codec not found.\n");
		return DJJ_FAILURE;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)		  //使用给定的AvCodec初始化AVCodecContext
	{
		printf("Could not open codec.\n");
		return DJJ_FAILURE;
	}

	pFrame = av_frame_alloc();							  //申请一个AVFrame 并做一些初始化工作
	pFrameYUV = av_frame_alloc();


	
	//avpicture_get_size 解码之后一帧图像的大小;
	//av_malloc封装了malloc并做了一些安全性操作;
	out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));

	//avpicture_fill 将pFrameYUV 按照 AV_PIX_FMT_YUV420P的格式“关联”到 out_buffer, 数据转换完之后 也自动保存到了out_buffer;
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));

	//Output Info----------------------------
	printf("------------------------File Information-------------------------\n");
	av_dump_format(pFormatCtx, 0, filepath, 0);			   //获取文件的信息到 AVFormatContext
	printf("------------------------------END--------------------------------\n");

	//获得并初始化一个SwsContext结构体，这是一个缩放及格式转换的函数 这里的缩放比例为1：1
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);


	uint8_t* p420UV;
	p420UV = (uint8_t*)malloc(pCodecCtx->width * pCodecCtx->height * 3 / 2);



	while (av_read_frame(pFormatCtx, packet) >= 0)			//从源文件容器中读取一个AVPacket数据包;  （音频可能包含多帧）
	{
		if (packet->stream_index == videoindex)				//如果是视频帧的话，进行解码
		{
			/**avcodec_decode_video2
			**解码视频流AVPacket
			**使用av_read_frame读取媒体流后需要进行判断，如果为视频流则调用该函数解码
			**返回got_picture>0时，表示解码得到AVFrame *pFrame, 其后可以对picture进行处理;
			**/
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet); 

			if (ret < 0)
			{
				printf("Decode Error.\n");
				return DJJ_FAILURE;
			}

			if (got_picture)
			{
				//图片像素转换、拉伸函数； 
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0,
					pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
			}
		}
		av_free_packet(packet); //每次读取packet之前需要先free;
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
	
 	if (ret >= 0)			//从源文件容器中读取一个AVPacket数据包;  （音频可能包含多帧）,这个时候为AVpacket的数据分配内存
	{
		if ((*pOutPkt)->stream_index == m_iVideoindex)				//如果是视频帧的话，进行解码-->processPkt
		{ 
			return DJJ_SUCCESS;
		}
		//av_packet_unref(pkt); //每次读取packet之前需要先free;
	}

	return DJJ_FAILURE;
}

int Cffmpeg::ProcessPkt(AVPacket* pPkt, uint8_t** out, int* linesize)
{
	int ret, got_picture = 0;
	AVFrame* testFrame;
	testFrame = av_frame_alloc();
	clock_t  startTime, endTime;
	startTime = clock();

	ret = avcodec_decode_video2(m_pCodecCtx, testFrame, &got_picture, pPkt); //这句话触发了空白！！
	if (ret < 0)
	{
		av_strerror(ret, m_errBuf, sizeof(m_errBuf));
		printf("Decode Error:%d(%s)\n", ret, m_errBuf);
		return DJJ_FAILURE;
	}

	if (got_picture)
	{
		//图片像素转换、拉伸函数； 	
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
	{
		endTime = clock();
		m_pMgr->m_iCodecCost = endTime - startTime;
		return DJJ_SUCCESS;
	}
	else
		return DJJ_FAILURE;
}

int Cffmpeg::SaveVideo()
{
	AVFormatContext *i_fmt_ctx;
	AVStream *i_video_stream;

	AVFormatContext *o_fmt_ctx;
	AVStream *o_video_stream;

	avcodec_register_all();
	av_register_all();
	avformat_network_init();

	/* should set to NULL so that avformat_open_input() allocate a new one */
	i_fmt_ctx = NULL;
	char rtspUrl[] = "rtsp://admin:admin123@10.255.251.238";
	const char *filename = "123.mp4";

	AVDictionary* options = NULL;
	av_dict_set(&options, "rtsp_transport", "udp", 0);
	//av_dict_set(&options, "max_delay", "5000000", 0);

	if (avformat_open_input(&i_fmt_ctx, rtspUrl, NULL, NULL) != 0)
	{
		fprintf(stderr, "could not open input file\n");
		return -1;
	}

	if (avformat_find_stream_info(i_fmt_ctx, NULL)<0)
	{
		fprintf(stderr, "could not find stream info\n");
		return -1;
	}

	//av_dump_format(i_fmt_ctx, 0, argv[1], 0);

	/* find first video stream */
	for (unsigned i = 0; i<i_fmt_ctx->nb_streams; i++)
	{
		if (i_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			i_video_stream = i_fmt_ctx->streams[i];
			break;
		}
	}
	if (i_video_stream == NULL)
	{
		fprintf(stderr, "didn't find any video stream\n");
		return -1;
	}

	//初始化输出码流的AVFormatContext;
	avformat_alloc_output_context2(&o_fmt_ctx, NULL, NULL, filename);

	/*
	* since all input files are supposed to be identical (framerate, dimension, color format, ...)
	* we can safely set output codec values from first input file
	*/

	//创建输出码流的AVStream
	o_video_stream = avformat_new_stream(o_fmt_ctx, NULL);
	if (o_video_stream == NULL)
	{
		printf("avformat_new_stream failed.\n");
		return DJJ_FAILURE;
	}


	int ret;
	ret = avcodec_copy_context(o_video_stream->codec, i_video_stream->codec);
	if (ret < 0)
	{
		printf("Failed to copy context from input to output stream codec context.\n");
		//Close input
		//if (m_pO_fmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
		//	avio_close(m_pO_fmt_ctx->pb);
		//avformat_free_context(m_pO_fmt_ctx);
		//if (ret < 0 && ret != AVERROR_EOF)
		{
			printf("Error occured.\n");
			return DJJ_FAILURE;
		}
	}
	/*
	{
		AVCodecContext *c;
		c = o_video_stream->codec;
		c->bit_rate = 400000;
		c->codec_id = i_video_stream->codec->codec_id;
		//c->codec_id = AV_CODEC_ID_HEVC;

		c->codec_type = i_video_stream->codec->codec_type;
		c->time_base.num = i_video_stream->time_base.num;
		c->time_base.den = i_video_stream->time_base.den;
		fprintf(stderr, "time_base.num = %d time_base.den = %d\n", c->time_base.num, c->time_base.den);
		c->width = i_video_stream->codec->width;
		c->height = i_video_stream->codec->height;
		c->pix_fmt = i_video_stream->codec->pix_fmt;
		printf("%d %d %d", c->width, c->height, c->pix_fmt);
		c->flags = i_video_stream->codec->flags;
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;
		c->me_range = i_video_stream->codec->me_range;
		c->max_qdiff = i_video_stream->codec->max_qdiff;

		c->qmin = i_video_stream->codec->qmin;	//最小量化器
		c->qmax = i_video_stream->codec->qmax;  //最大量化器

		c->qcompress = i_video_stream->codec->qcompress;
	}
	*/

	//打开输出文件
		if (avio_open(&o_fmt_ctx->pb, filename, AVIO_FLAG_READ_WRITE)<0)
		{
			printf("Failed to open output file! \n");
			return DJJ_FAILURE;
		}

	//写头文件（对于某些没有头文件的封装格式 不需要此函数）
	ret = avformat_write_header(o_fmt_ctx, NULL);
	if (ret < 0)
	{
		printf("write header failed.\n");
	}

	int last_pts = 0;
	int last_dts = 0;

	int64_t pts, dts;
	int frameCount = 1000;
	while (1)
	{
		AVPacket i_pkt;
		av_init_packet(&i_pkt);
		i_pkt.size = 0;
		i_pkt.data = NULL;
		if (av_read_frame(i_fmt_ctx, &i_pkt) <0)
			break;
		/*
		* pts and dts should increase monotonically
		* pts should be >= dts
		*/
		i_pkt.pts = av_rescale_q_rnd(i_pkt.pts, i_video_stream->time_base, o_video_stream->time_base, (enum AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		i_pkt.dts = av_rescale_q_rnd(i_pkt.dts, i_video_stream->time_base, o_video_stream->time_base, (enum AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
		//printf("pts %d dts %d base %d\n",pkt.pts,pkt.dts, in_stream->time_base);
		i_pkt.duration = av_rescale_q(i_pkt.duration, i_video_stream->time_base, o_video_stream->time_base);

		i_pkt.pos = -1;;

		//printf("%lld %lld\n", i_pkt.pts, i_pkt.dts); 
		static int num = 1;
		printf("frame %d\n", num++);

		//将编码后的视频码流写入文件;
		av_interleaved_write_frame(o_fmt_ctx, &i_pkt);

		//av_packet_unref(&i_pkt);
		//av_free_packet(&i_pkt); 
		//av_init_packet(&i_pkt); 
		frameCount--;
		if(frameCount <= 0)
			break;
	}
	last_dts += dts;
	last_pts += pts;

	avformat_close_input(&i_fmt_ctx);

	//写文件尾(对于没有文件头的封装格式，不需要这一步)
	av_write_trailer(o_fmt_ctx);

	avcodec_close(o_fmt_ctx->streams[0]->codec);
	av_freep(&o_fmt_ctx->streams[0]->codec);
	av_freep(&o_fmt_ctx->streams[0]);

	avio_close(o_fmt_ctx->pb);
	av_free(o_fmt_ctx);

	return DJJ_SUCCESS;
}

int Cffmpeg::SavePre(QString saveFilepath)
{
	int ret, i, iError;
	AVOutputFormat *ofmt = NULL;

	QByteArray qout_filename = saveFilepath.toLatin1();
	const char* out_filename = qout_filename.data();

	avformat_alloc_output_context2(&m_pO_fmt_ctx, NULL, NULL, out_filename);
	
	if (!m_pO_fmt_ctx)
	{
		printf("Could not create output context!\n");
		//Close input
		if (m_pO_fmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
			avio_close(m_pO_fmt_ctx->pb);
		avformat_free_context(m_pO_fmt_ctx);
		if (ret < 0 && ret != AVERROR_EOF)
		{
			printf("Error occured.\n");
			return DJJ_FAILURE;
		}
	}

	ofmt = m_pO_fmt_ctx->oformat;
	
	/*
	for (i = 0; i < m_pFormatCtx->nb_streams; i++)
	{
		AVStream *in_stream = m_pFormatCtx->streams[i];
		AVStream *out_stream = avformat_new_stream(m_pO_fmt_ctx, in_stream->codec->codec);
		if (!out_stream)
		{
			printf("Failed allocating output stream.\n");
			//Close input
			if (m_pO_fmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
				avio_close(m_pO_fmt_ctx->pb);
			avformat_free_context(m_pO_fmt_ctx);
			if (ret < 0 && ret != AVERROR_EOF)
			{
				printf("Error occured.\n");
				return DJJ_FAILURE;
			}
		}

		ret = avcodec_copy_context(out_stream->codec, in_stream->codec);
		if (ret < 0)
		{
			printf("Failed to copy context from input to output stream codec context.\n");
			//Close input
			if (m_pO_fmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
				avio_close(m_pO_fmt_ctx->pb);
			avformat_free_context(m_pO_fmt_ctx);
			if (ret < 0 && ret != AVERROR_EOF)
			{
				printf("Error occured.\n");
				return DJJ_FAILURE;
			}
		}

		out_stream->codec->codec_tag = 0;

		if (m_pO_fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
			out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}
	*/

	m_InStream = m_pFormatCtx->streams[m_iVideoindex];
	m_OutStream = avformat_new_stream(m_pO_fmt_ctx, NULL);;
	if (!m_OutStream)
	{
		printf("Failed allocating output stream.\n");
		//Close input
		if (m_pO_fmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
			avio_close(m_pO_fmt_ctx->pb);
		avformat_free_context(m_pO_fmt_ctx);
		if (ret < 0 && ret != AVERROR_EOF)
		{
			printf("Error occured.\n");
			return DJJ_FAILURE;
		}
	}
	ret = avcodec_copy_context(m_OutStream->codec, m_InStream->codec);
	if (ret < 0)
	{
		printf("Failed to copy context from input to output stream codec context.\n");
		//Close input
		if (m_pO_fmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
			avio_close(m_pO_fmt_ctx->pb);
		avformat_free_context(m_pO_fmt_ctx);
		if (ret < 0 && ret != AVERROR_EOF)
		{
			printf("Error occured.\n");
			return DJJ_FAILURE;
		}
	}
	
	m_OutStream->codec->codec_tag = 0;

	if (m_pO_fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
		m_OutStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	
	//av_dump_format(m_pO_fmt_ctx, 0, out_filename, 1);
	//打开输出文件
	if (!(ofmt->flags & AVFMT_NOFILE))
	{
		ret = avio_open(&m_pO_fmt_ctx->pb, out_filename, AVIO_FLAG_READ_WRITE);
		if (ret < 0)
		{
			printf("Could not open output URL '%s'", out_filename);
			//Close input
			if (m_pO_fmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
				avio_close(m_pO_fmt_ctx->pb);
			avformat_free_context(m_pO_fmt_ctx);
			if (ret < 0 && ret != AVERROR_EOF)
			{
				printf("Error occured.\n");
				return DJJ_FAILURE;
			}
		}
	}
	
	//写文件头到输出文件
	//m_pO_fmt_ctx->video_codec_id = AV_CODEC_ID_HEVC;

	ret = avformat_write_header(m_pO_fmt_ctx, NULL);
	if (ret < 0)
	{
		av_strerror(ret, m_errBuf, sizeof(m_errBuf));
		printf("avformat_write_header Error:%d(%s)\n", ret, m_errBuf);

		printf("Error occured when opening output URL.\n");
		//Close input
		if (m_pO_fmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
			avio_close(m_pO_fmt_ctx->pb);
		avformat_free_context(m_pO_fmt_ctx);
		if (ret < 0 && ret != AVERROR_EOF)
		{
			printf("Error occured.\n");
			return DJJ_FAILURE;
		}
	}
	return DJJ_SUCCESS;
}

int Cffmpeg::Saving(AVPacket *pkt) //传入的这个pkt已经是读取过的pkt
{
	int ret;
	static int frame_index = 0;

	pkt->pts = av_rescale_q_rnd(pkt->pts, m_InStream->time_base, m_OutStream->time_base, (enum AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
	pkt->dts = av_rescale_q_rnd(pkt->dts, m_InStream->time_base, m_OutStream->time_base, (enum AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
	pkt->duration = av_rescale_q(pkt->duration, m_InStream->time_base, m_OutStream->time_base);

	//防止获取不到duration的情况
	if (pkt->duration == 0)
	{
		if (m_pCodecCtx->codec_id == AV_CODEC_ID_HEVC)
			pkt->duration = 512;
		if (m_pCodecCtx->codec_id == AV_CODEC_ID_H264)
			pkt->duration = 40;
	}
		
	pkt->pos = -1;
	
	printf("pts: %lld, dts: %lld, duration: %lld \n",pkt->pts, pkt->dts, pkt->duration);

	if (pkt->stream_index == m_iVideoindex)
	{
		printf("Receive %8d video frames from input URL\n", frame_index);
		frame_index++;
	}
	
	ret = av_interleaved_write_frame(m_pO_fmt_ctx, pkt);
	if (ret < 0)
	{ 
		if (ret == -22)
			return DJJ_SUCCESS;
		else
		{
			printf("Error muxing packet. error code %d\n", ret);
			return DJJ_FAILURE;
		}
	}
	return DJJ_SUCCESS;
}

void Cffmpeg::SaveEnd()
{
	av_write_trailer(m_pO_fmt_ctx);
	//-----写文件的释放流程
	avcodec_close(m_pO_fmt_ctx->streams[0]->codec);
	av_freep(&m_pO_fmt_ctx->streams[0]->codec);
	av_freep(&m_pO_fmt_ctx->streams[0]);

	avio_close(m_pO_fmt_ctx->pb);
	av_free(m_pO_fmt_ctx);
	//avformat_free_context(m_pO_fmt_ctx);
}
