#include "StdAfx.h"
#include "Decoderav.h"
#include "pkgqueue.h"
DWORD WINAPI ThreadVideo(LPVOID lpParameter){
	Decoderav* dav=(Decoderav*)lpParameter;
	AVPacket pkt = { 0 };
	AVFrame *frm;
	SDL_Rect sdlRT;SDL_Rect dstRT;
	INT serial,gotyuv,ret,vWidth,vHeight;
	vWidth=1920;
	vHeight=1080;
	unsigned char* szData;
	int iPitch = vWidth*SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_IYUV); 
	dav->pVCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
	if(!dav->pACodec){
		TRACE("h264decode not founc\n");
	}
	dav->pVCodecCtx=avcodec_alloc_context3(dav->pVCodec);
	if(!dav->pACodecCtx){
		TRACE("h264 pACodecCtxnot founc\n");
	}
	frm=av_frame_alloc();
	if(!frm){
		TRACE("Count not alloc frame!\n");
	}
	if(avcodec_open2(dav->pVCodecCtx,dav->pVCodec,NULL)<0)
	{
		TRACE("avcodec_open2 fail!\n");
	}
	dav->pWindow=SDL_CreateWindowFrom((void *)dav->dispwnd);
	SDL_Renderer * pRender = SDL_CreateRenderer( dav->pWindow, 0, SDL_RENDERER_ACCELERATED );
	SDL_Texture * pTexture = SDL_CreateTexture( pRender,SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, vWidth, vHeight);
	szData=(unsigned char*)av_malloc(vWidth*vHeight*2);
	SDL_GetWindowSize(dav->pWindow, &dstRT.w, &dstRT.h);
	sdlRT.w = 1920;
	sdlRT.h = 1080;	
	dstRT.x=sdlRT.x = 0;
	dstRT.y=sdlRT.y = 0;

	while(1)
	{
		
		if (packet_queue_get(dav->m_pInputQuev, &pkt, 1, &serial) < 0)
		{
			return -1;
		}
		//if(pkt.data)TRACE("pkt.%d %d %d %d %d %d %d %d\n",pkt.data[0],pkt.data[1],pkt.data[2],pkt.data[3],pkt.data[4],pkt.data[5],pkt.data[6],pkt.data[7]);

		ret=avcodec_decode_video2(dav->pVCodecCtx,frm,&gotyuv,&pkt);
		if (ret<0)
		{
			//TRACE("decode frame error!\n");
		}
		if(gotyuv){
			//TRACE("gotyuv!\n");
			memcpy(szData,frm->data[0],vWidth*vHeight);
			memcpy(szData+vWidth*vHeight,frm->data[1],vWidth*vHeight/4);
			memcpy(szData+vWidth*vHeight+vWidth*vHeight/4,frm->data[2],vWidth*vHeight/4);
			SDL_UpdateTexture( pTexture,&sdlRT,szData,iPitch);
			SDL_RenderClear( pRender );
			SDL_RenderCopy( pRender, pTexture, &sdlRT, &dstRT );
			SDL_RenderPresent( pRender );
		}


		av_free_packet(&pkt);
	}
	return 0;
}
DWORD WINAPI ThreadAudio(LPVOID lpParameter){
	Decoderav* dav=(Decoderav*)lpParameter;
	AVPacket pkt = { 0 };
	INT serial;
	while(1)
	{
		if (packet_queue_get(dav->m_pInputQuea, &pkt, 1, &serial) < 0)
		{
			return -1;
		}
		//if(pkt.data)TRACE("pkt.%d %d %d %d %d %d %d %d\n",pkt.data[0],pkt.data[1],pkt.data[2],pkt.data[3],pkt.data[4],pkt.data[5],pkt.data[6],pkt.data[7]);







		av_free_packet(&pkt);
	}
	
	return 0;
}
Decoderav::Decoderav(HWND dispw)
{
	m_pInputQuea=NULL;
	m_pInputQuev=NULL;
	pWindow=SDL_CreateWindowFrom((void *)dispw);

}
Decoderav::~Decoderav(void)
{
}

int Decoderav::SetVideoQue(PacketQueue* qa,PacketQueue* qv)
{
	m_pInputQuea=qa;
	m_pInputQuev=qv;
	packet_queue_start(m_pInputQuea);//创建解码线程前调用
	packet_queue_start(m_pInputQuev);

	return 0;
}
int Decoderav::StartDecode(void){
	m_hDecodeVideo=CreateThread(NULL,0,ThreadVideo,(LPVOID)this,0,NULL);
	m_hDecodeAudio=CreateThread(NULL,0,ThreadAudio,(LPVOID)this,0,NULL);


	return 0;
}


