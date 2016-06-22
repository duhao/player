#include "StdAfx.h"
#include "Decoderav.h"
#include "pkgqueue.h"
DWORD WINAPI ThreadVideo(LPVOID lpParameter){
	Decoderav* dav=(Decoderav*)lpParameter;
	AVPacket pkt = { 0 };
	AVFrame *frm;
	SDL_Rect sdlRT;SDL_Rect dstRT;
	INT serial,gotyuv,ret,vWidth,vHeight;
	vWidth=dav->vWidth;
	vHeight=dav->vHeight;
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
	sdlRT.w = vWidth;
	sdlRT.h = vHeight;	
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
		
			memcpy(szData,frm->data[0],frm->width*frm->height);
			memcpy(szData+frm->width*frm->height,frm->data[1],frm->width*frm->height/4);
			memcpy(szData+frm->width*frm->height+frm->width*frm->height/4,frm->data[2],frm->width*frm->height/4);
			SDL_UpdateTexture( pTexture,&sdlRT,szData,iPitch);
			SDL_RenderClear( pRender );
			SDL_RenderCopy( pRender, pTexture, &sdlRT, &dstRT );
			SDL_RenderPresent( pRender );
		}


		av_free_packet(&pkt);
	}
	return 0;
}
/* The audio function callback takes the following parameters:  
 * stream: A pointer to the audio buffer to be filled  
 * len: The length (in bytes) of the audio buffer  
 * 回调函数 
*/   
//Buffer:  

unsigned char  *audio_chunk;   
int  audio_len;   
unsigned char   *audio_pos;   
unsigned char   *pcm_buffer;
/* Audio Callback 
	   57. * The audio function callback takes the following parameters:  
	   58. * stream: A pointer to the audio buffer to be filled  
	   59. * len: The length (in bytes) of the audio buffer  
	   60. *  
	   61.*/   
static void fill_audio(void *udata,unsigned char *stream,int len){   
		   //SDL 2.0  
			 SDL_memset(stream, 0, len);  
		   if(audio_len==0)return;   
		   len=(len>audio_len?audio_len:len);  
		  
			//SDL_MixAudio(stream,audio_pos,len,SDL_MIX_MAXVOLUME);  
		   memcpy(stream,audio_pos,len);
		   audio_pos += len;   
		  audio_len -= len;   
		   
}   
DWORD WINAPI ThreadAudio(LPVOID lpParameter){
	Decoderav* dav=(Decoderav*)lpParameter;
	AVFrame *frm;
	AVPacket pkt = { 0 };
	SDL_AudioSpec sdlaudio;	
	struct SwrContext *swr_ctx;
	TCHAR szFilter[] = _T("视频(*.mp4)|*.mp4|"); 
	CFileDialog fileDlg(TRUE, _T("MP4"), NULL, 0, szFilter, 0);//这个无用，但是不能取消，取消后SDL_AudioOpen失败	
	INT serial;
	int ret,gotyuv;
	dav->pACodec=avcodec_find_decoder(AV_CODEC_ID_AAC);
	if(!dav->pACodec){
		TRACE("AV_CODEC_ID_AAC not found\n");
	}
	dav->pACodecCtx=avcodec_alloc_context3(dav->pACodec);
	if(!dav->pACodecCtx){
		TRACE("aac pACodecCtxnot founc\n");
	}
	frm=av_frame_alloc();
	if(!frm){
		TRACE("AAc Count not alloc frame!\n");
	}
	if(avcodec_open2(dav->pACodecCtx,dav->pACodec,NULL)<0)
	{
		TRACE("aac avcodec_open2 fail!\n");
	}
	dav->pACodecCtx->sample_rate=16000;
	dav->pACodecCtx->channels=2;
	//dav->pACodecCtx->sample_fmt=AV_SAMPLE_FMT_S16;
	sdlaudio.freq = dav->pACodecCtx->sample_rate;   
	sdlaudio.format = AUDIO_S16SYS;   
	sdlaudio.channels = dav->pACodecCtx->channels;   
	sdlaudio.silence = 0;   
	sdlaudio.samples = 1024; //播放AAC，M4a，缓冲区的大小  
	//wanted_spec.samples = 1152; //播放MP3，WMA时候用  
	sdlaudio.callback = fill_audio;   
	sdlaudio.userdata = dav->pACodecCtx;
	pcm_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);// 1 second of 48khz 32bit audio X2
	swr_ctx = swr_alloc();  
	swr_ctx=swr_alloc_set_opts(swr_ctx,AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, dav->pACodecCtx->sample_rate,av_get_default_channel_layout(dav->pACodecCtx->channels),AV_SAMPLE_FMT_FLTP , dav->pACodecCtx->sample_rate,0, NULL);  
	swr_init(swr_ctx);
	if (SDL_OpenAudio(&sdlaudio, NULL)<0)//步骤（2）打开音频设备   
	{   
		TRACE("can't open audio.\n");   
		return 0;   
	}   
	SDL_PauseAudio(0);
	while(1)
	{
		if (packet_queue_get(dav->m_pInputQuea, &pkt, 1, &serial) < 0)
		{
			return -1;
		}
		ret=avcodec_decode_audio4(dav->pACodecCtx,frm,&gotyuv,&pkt);
		if (ret<0)
		{
			TRACE("decode audio error!\n");
		}
		if (gotyuv)
		{	
			//TRACE("pkt.%d %d %d %d %d %d %d %d\n",pkt.data[0],pkt.data[1],pkt.data[2],pkt.data[3],pkt.data[4],pkt.data[5],pkt.data[6],pkt.data[7]);

			ret=swr_convert(swr_ctx,&pcm_buffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)frm->data ,frm->nb_samples); 			
			if(ret>0)
			{
				audio_pos=pcm_buffer;
				audio_len=av_samples_get_buffer_size(NULL,av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO),ret,AV_SAMPLE_FMT_S16,1);
				//SDL_PauseAudio(0);
				while(audio_len>0)//Wait until finish  
				{SDL_Delay(1);}
				
				//fwrite(out_buffer,1,is->audio_buf_size,fptmp);
				//TRACE("decode audio=%d! %d\n",is->frame->linesize[0],is->audio_buf_size);

			}
			



		}
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


