#pragma once

extern "C"
{
#include <sdl/SDL.h>
#include <libavformat/avformat.h>
}
#include <windows.h>

class CMgr;

class CSDLPlay
{
public:
	CSDLPlay(CMgr* mgr);
	~CSDLPlay();
	int SDLInit();
	int SDLCreateWindow(int screen_width, int screen_height, HWND hWindow);
	int SDLShow(uint8_t* buf, int linsize);
	int SDLClose();

	//改变窗口大小时的一些步骤
	int SDLCreateRenderer(int screen_width, int screen_height);
	int SDLDestery();
	int SDLFlashShow(uint8_t* buf, int linsize);

	int				m_iScreenW, m_iScreenH;	//创建的窗口宽高
	bool			m_bResize;				//是否改变大小
	bool			m_bRefresh;				//是否刷新

	CMgr			*m_pMgr;				//管理类

private:
	
	
	SDL_Renderer	*m_pSdlRenderer;		//渲染器
	SDL_Texture		*m_pSdlTexture;	
	SDL_Rect		 m_tSdlRect;
	SDL_Thread		*m_pVideo_tid;
	SDL_Event		 m_uEvent;
	HWND			 m_hWnd;				//播放视频的窗口句柄
public:
	SDL_Window		*m_pScreen;				//窗口
};

