#pragma once
#include "afxdialogex.h"
#include "stdio.h"
#include "Enconn.h"
#include "pkgqueue.h"
#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
class Decoderav
{
public:
	PacketQueue* m_pInputQuea;	
	PacketQueue* m_pInputQuev;
	HWND dispwnd;
	SDL_Window *pWindow;	
	SDL_AudioSpec sdlaudio;
	int i,videoIndex,audioIndex,gotyuv,ret;
	int vHeight,vWidth;
	AVFormatContext *pInfmctx;
	AVCodecContext *pVCodecCtx;
	AVCodecContext *pACodecCtx;
	AVCodec *pACodec;
	AVCodec *pVCodec;
	AVPacket avpkg;
	HANDLE m_hDecodeVideo;
	HANDLE m_hDecodeAudio;
public:
	Decoderav(HWND dispw);
	~Decoderav(void);
	int Decoderav::SetVideoQue(PacketQueue* qa,PacketQueue* qv);
	int Decoderav::StartDecode(void);	

};

