#pragma once


extern "C"
{
#include <sdl/SDL.h>
#include <libavformat/avformat.h>
}
#include <windows.h>

class CSDLPlay
{
public:
	CSDLPlay();
	~CSDLPlay();
	int SDLInit();
	int SDLCreateWindow(int screen_width, int screen_height, HWND hWindow);
	int SDLShow(uint8_t* buf, int linsize);
	int SDLClose();
private:
	int		m_iScreenW, m_iScreenH;
	SDL_Window		*m_pScreen;				//´°¿Ú
	SDL_Renderer	*m_pSdlRenderer;		//äÖÈ¾Æ÷
	SDL_Texture		*m_pSdlTexture;	
	SDL_Rect		 m_tSdlRect;
	SDL_Thread		*m_pVideo_tid;
	SDL_Event		 m_uEvent;
};

