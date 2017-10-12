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
	m_bRefresh = true;
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
	m_duration = 0;

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
	

	//获取视频时长;
	if (m_pFormatCtx->duration != AV_NOPTS_VALUE)
	{
		int hours, mins, secs, us;
		int64_t duration = m_pFormatCtx->duration + 5000;
		m_duration = duration;
		secs = duration / AV_TIME_BASE;
		us = duration % AV_TIME_BASE;
		mins = secs / 60;
		secs %= 60;
		hours = mins / 60;
		mins %= 60;
		printf("视频总时长:%02d:%02d:%02d.%03d\n", hours, mins, secs, (100 * us) / AV_TIME_BASE);
	}
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

	/*记录当前帧的时间戳
	  注意:目前IPC录制的视频（.asf）时间戳(pts)都是AV_NOPTS_VALUE ，所以手动将dts的值赋值给了 pts. 正确的计算pts值的方法还没有找到。
	*/
	if (pPkt->pts == AV_NOPTS_VALUE)
	{	
		m_currentSec = testFrame->pkt_dts *(double)av_q2d(m_pFormatCtx->streams[m_iVideoindex]->time_base);
	}
	else
	{
		m_currentSec = pPkt->pts*(double)av_q2d(m_pFormatCtx->streams[m_iVideoindex]->time_base);
	}
	
	if (m_currentSec <  (m_pFormatCtx->duration + 5000)/AV_TIME_BASE)
	{
		printf("CurrentSec is %d \n", m_currentSec);
		if (m_bRefresh)
		{
			sendSignalToIpcwindow();
		}
	}
	
	av_frame_unref(testFrame);
	av_frame_free(&testFrame);

	if (got_picture)
	{
		//计算解码耗时
		endTime = clock();
		m_pMgr->m_iCodecCost = endTime - startTime;
		return DJJ_SUCCESS;
	}
	else
		return DJJ_FAILURE;
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
	m_OutStream = avformat_new_stream(m_pO_fmt_ctx, NULL);
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

#if 0
/*本地视频拖动播放模块，采用while做，不采用队列*/
int Cffmpeg::ControlBroadCast(int targetTime)
{
	int ret, got_picture = 0;
	AVPacket * packet;
	packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	AVFrame* testFrame;
	testFrame = av_frame_alloc();

	SwsVideo();

	while (av_read_frame(m_pFormatCtx, packet) >= 0)
	{
		ret = avcodec_decode_video2(m_pCodecCtx, testFrame, &got_picture, packet); //这句话触发了空白！！
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
	}
}
#endif

void Cffmpeg::Seek(int targetTime)
{
	//零时针对 timebase 分子为0的情况进行的计算.
	av_seek_frame(m_pFormatCtx, m_iVideoindex, targetTime * m_pFormatCtx->streams[m_iVideoindex]->time_base.den, AVSEEK_FLAG_ANY);
}