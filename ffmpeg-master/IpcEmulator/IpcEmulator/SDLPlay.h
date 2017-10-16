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

	//�ı䴰�ڴ�Сʱ��һЩ����
	int SDLCreateRenderer(int screen_width, int screen_height);
	int SDLDestery();
	int SDLFlashShow(uint8_t* buf, int linsize);

	int				m_iScreenW, m_iScreenH;	//�����Ĵ��ڿ��
	bool			m_bResize;				//�Ƿ�ı��С
	bool			m_bRefresh;				//�Ƿ�ˢ��

	CMgr			*m_pMgr;				//������

private:
	
	
	SDL_Renderer	*m_pSdlRenderer;		//��Ⱦ��
	SDL_Texture		*m_pSdlTexture;	
	SDL_Rect		 m_tSdlRect;
	SDL_Thread		*m_pVideo_tid;
	SDL_Event		 m_uEvent;
	HWND			 m_hWnd;				//������Ƶ�Ĵ��ھ��
public:
	SDL_Window		*m_pScreen;				//����
};

