
// playerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "player.h"
#include "playerDlg.h"
#include "afxdialogex.h"
#include "stdio.h"
#include "Enconn.h"
#include "pkgqueue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static  Uint8  *audio_chunk;   
static  Uint32  audio_len;   
static  Uint8  *audio_pos;   
uint8_t *out_buffer;
static AVPacket flush_pkt;
VideoState *is=NULL;
FILE *fptmp;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CplayerDlg 对话框




CplayerDlg::CplayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CplayerDlg::IDD, pParent)
	, m_audiochan(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//  m_RichEdit = _T("");
}

void CplayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STMSG, m_msg);
	//  DDX_Text(pDX, IDC_RICHEDIT21, m_RichEdit);
	DDX_Control(pDX, IDC_RICHEDIT21, m_RichEdit);
	DDX_Radio(pDX, IDC_RADIO1, m_audiochan);
	DDV_MinMaxInt(pDX, m_audiochan, 0, 15);
}

BEGIN_MESSAGE_MAP(CplayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CplayerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CplayerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CplayerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CplayerDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON2, &CplayerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CplayerDlg::OnBnClickedButton3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CplayerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON4, &CplayerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO1, &CplayerDlg::OnBnClickedRadio1)	
	ON_BN_CLICKED(IDC_RADIO6, &CplayerDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO2, &CplayerDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO7, &CplayerDlg::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO13, &CplayerDlg::OnBnClickedRadio13)
	ON_BN_CLICKED(IDC_RADIO14, &CplayerDlg::OnBnClickedRadio14)
	ON_BN_CLICKED(IDC_RADIO17, &CplayerDlg::OnBnClickedRadio17)
	ON_BN_CLICKED(IDC_RADIO12, &CplayerDlg::OnBnClickedRadio12)
END_MESSAGE_MAP()


// CplayerDlg 消息处理程序

BOOL CplayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	GetDlgItem(IDC_IPADDRESS1)->SetWindowText(_T("192.168.7.118"));
	//ShowWindow(SW_MINIMIZE);
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)<0)
	{
		exit(-1);
	}
	av_register_all();
	avformat_network_init();
	// TODO: 在此添加额外的初始化代码
	
	conn = new Enconn();	
	m_Dec = new Decoderav(GetDlgItem(IDC_STATICVIDEO)->GetSafeHwnd());
	conn->SetMessageHwnd(m_hWnd);
	conn->SetRichHandle(&m_RichEdit);
	packet_queue_init(&qv);
	packet_queue_init(&qa);
	conn->m_AudioOut=&qa;
	conn->m_VideoOut=&qv;
	m_Dec->SetVideoQue(&qa,&qv);	
	m_audiochan=0;	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CplayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CplayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CplayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CplayerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	
}


void CplayerDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CplayerDlg::OnBnClickedButton1()
{
	//mp4转码成aac 和 h264
	int retw;
	int i;
	AVFormatContext *ic=NULL;
	AVPacket packet = {0,0};
	char filepath[2048];
	int ret=0;
	int dts=0;
	int s;
	FILE *fp,*fpv,*fplog;
	fp=fopen("test.aac","wb+");
	fpv=fopen("test.264","wb+");
	CFile file;
	unsigned char adts_header[7] = {0};
	unsigned char audioObjectType,samplingFrequencyIndex,channelConfiguration;
	TCHAR szFilter[] = _T("视频(*.mp4)|*.mp4|"); 
	CFileDialog fileDlg(TRUE, _T("MP4"), NULL, 0, szFilter, 0); 
	CString strFilePath;

	if (IDOK==fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();		
		CStringA strTempA(strFilePath);
		const char *chartemp = (LPCSTR)strTempA;
		memcpy(filepath,chartemp,strTempA.GetLength()+1);

	}
	else
	{

		return;
	}
	//if (!file.Open("log.txt",
		//CFile::modeRead | 
		//CFile::shareDenyWrite))
	//{
		//TRACE(_T("Load (file): Error opening file %s\n"),"log.txt");
		//return FALSE;
	//};
	//objtype 0 null ||   1--AAC MAIN ||2 AAC LC ||3 AAC SSR||4 AAC LTP||5 SBR||6 AAC Scalable ||7 TwinVQ...
	//0--9600Hz||1--88200Hz||2--64000Hz||3--4800Hz||4--44100Hz||5--32000Hz||6--24000Hz||7--22050Hz||8--16000Hz||9--12000Hz||10--11025Hz||11--8000Hz||
	//avcodec_register_all();
	//avdevice_register_all();

   // avfilter_register_all();
	
	avformat_open_input(&ic, filepath, NULL,NULL);
	ret=avformat_find_stream_info(ic,NULL);
	for(i=0;i<ic->nb_streams;i++)
	{

		TRACE("packet.stream_index=%d\n",ic->streams[i]->codec->codec_id);
	}
	AVBitStreamFilterContext* h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
	while(1)
		{
	av_init_packet(&packet);	
	ret=av_read_frame(ic,&packet);
	//TRACE("packet.stream_index============%d\n",packet.stream_index);
	//usleep(400000);
	
	if(packet.stream_index<8 && packet.size>0){
		//av_bitstream_filter_filter(h264bsfc, ic->streams[0]->codec, NULL, &packet.data, &packet.size, packet.data, packet.size, 0);
		audioObjectType=(ic->streams[1]->codec->extradata[0]& 0xF8) >> 3;
		samplingFrequencyIndex=((ic->streams[1]->codec->extradata[0]& 0x7) << 1) | (ic->streams[1]->codec->extradata[1]>> 7);
		channelConfiguration=(ic->streams[1]->codec->extradata[1] >> 3) & 0x0F;	
		//printf("asc=%d,%d,%d\n",audioObjectType,samplingFrequencyIndex,channelConfiguration);
		adts_header[0] = (unsigned char)0xFF;
		adts_header[1] = (unsigned char)0xF1;
	//FX，X应该要根据profile来生成//////
	/* packet[2] = (unsigned char)(((profile)<<6) + (freqIdx<<2) +(chanCfg>>2));
	packet[3] = (unsigned char)(((chanCfg&3)<<6) + (packetLen>>11));
	0: 96000 Hz||1: 88200 Hz||2: 64000 Hz||3: 48000 Hz||4: 44100 Hz||5: 32000 Hz||6: 24000 Hz||7: 22050 Hz||8: 16000 Hz||9: 12000 Hz
	10: 11025 Hz||11: 8000 Hz||12: 7350 Hz||13: Reserved||14: Reserved||15: frequency is written explictly
	0:main profile 1:LC 2:SSR 3 reserved
	*/
		adts_header[2] = (unsigned char)(((audioObjectType-1)<<6) + (samplingFrequencyIndex<<2) +(channelConfiguration>>2));
		adts_header[3] = (unsigned char)(((channelConfiguration&3)<<6) + (packet.size+7>>11));
		adts_header[4] = (unsigned char)((packet.size+7&0x7FF) >> 3);
		adts_header[5] = (unsigned char)(((packet.size+7&7)<<5) + 0x1F);
		adts_header[6] = (unsigned char)0xFC;
		//fwrite(adts_header,1,7,fp);
		retw=fwrite(packet.data,1,packet.size,fp);

		TRACE("Audio pts============%f\n",packet.pts*av_q2d(ic->streams[packet.stream_index]->time_base));
	
		//TRACE("Aret============%d\n",packet.pts);
		
	}
	else if(packet.stream_index==8 && packet.size>0)
		{
			av_bitstream_filter_filter(h264bsfc, ic->streams[1]->codec, NULL, &packet.data, &packet.size, packet.data, packet.size, 0);
			s=packet.size;
			fwrite(&s,1,4,fpv);
			fwrite(packet.data,1,packet.size,fpv);
			TRACE("Video pts ret=%f\n",packet.pts*av_q2d(ic->streams[packet.stream_index]->time_base));
			//TRACE("Aret%d\n",packet.pts);
			
		}
	//dts++;
	if(ret<0)break;
	//if(dts>20){return 0;}
	av_free_packet(&packet);
	}

	av_dump_format(ic,1,0,0);
	avformat_close_input(&ic);
	av_bitstream_filter_close(h264bsfc); 
	fclose(fp);
	fclose(fpv);
}


void CplayerDlg::OnBnClickedButtonPlay()
{
	int i,videoIndex=-1,audioIndex=-1,gotyuv,ret,gotpcm;
	AVFormatContext *pInfmctx=NULL;
	AVCodecContext *pVCodecCtx=NULL;	
	AVCodec *pVCodec=NULL;	
	AVPacket avpkg={0,0};
	AVFrame *pFrame,*pFrameYUV; 
	struct SwsContext *img_convert_ctx;
	unsigned char *outbuf=NULL;
	FILE *fp;
	fp=fopen("output.yuv","wb+");
	av_register_all();
	avformat_network_init();
	avformat_open_input(&pInfmctx,"F:\\视频\\爱情歌.泳儿音乐会.Vincy.Live.2015.BD720P.X264.AAC.Cantonese.CHS.Mp4Ba.mp4",NULL,NULL);

	avformat_find_stream_info(pInfmctx,NULL);

	for (i=0;i<pInfmctx->nb_streams;i++)
	{
		if (pInfmctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoIndex=i;
		}else if (pInfmctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
		{
			audioIndex=i;

		}
	}

	pVCodecCtx=pInfmctx->streams[videoIndex]->codec;
	if (pVCodecCtx == NULL)
	{
		TRACE("can not find avdocecctx!!\n");
		return;
	}
	pVCodec=avcodec_find_decoder(pVCodecCtx->codec_id);

	if (pVCodec == NULL)
	{
		TRACE("can not find avdocec!!\n");
		return;
	}
	
	if(avcodec_open2(pVCodecCtx,pVCodec,NULL)<0)
	{
		TRACE("can not avcodec_open2!!\n");
		return;
	}
	
	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	outbuf=(unsigned char*)av_malloc(avpicture_get_size(PIX_FMT_YUV420P,pVCodecCtx->width,pVCodecCtx->height));

	avpicture_fill((AVPicture *)pFrameYUV, outbuf, PIX_FMT_YUV420P, pVCodecCtx->width, pVCodecCtx->height); 
	av_init_packet(&avpkg);
	img_convert_ctx = sws_getContext(pVCodecCtx->width, pVCodecCtx->height, pVCodecCtx->pix_fmt,   
	pVCodecCtx->width, pVCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	int j=0;
	while(av_read_frame(pInfmctx, &avpkg)>=0){ 
		if (avpkg.stream_index == videoIndex)
		{

			ret=avcodec_decode_video2(pVCodecCtx,pFrame,&gotyuv,&avpkg);
			if (ret<0)
			{
				TRACE("decode frame error!\n");
			}
			if (gotyuv)
			{
				Sleep(1);//33毫秒
				j++;
				if(j==151){
						TRACE("gotyuv!=%d==%d\n",pVCodecCtx->width*pVCodecCtx->height*2);
				
						//fwrite(pFrame->data[0],1,pVCodecCtx->width*pVCodecCtx->height,fp);
						//fwrite(pFrame->data[1],1,pVCodecCtx->width*pVCodecCtx->height/4,fp);//yu12
						//fwrite(pFrame->data[2],1,pVCodecCtx->width*pVCodecCtx->height/4,fp);
				
						int a=0,i=0;
						for (i=0;i<pVCodecCtx->height;i++)
						{
							fwrite(pFrame->data[0]+i*pFrame->linesize[0],1,pVCodecCtx->width,fp);
						}
						for (i=0;i<pVCodecCtx->height/2;i++)
						{
							fwrite(pFrame->data[1]+i*pFrame->linesize[1],1,pVCodecCtx->width/2,fp);//yu12
						}
						for (i=0;i<pVCodecCtx->height/2;i++)
						{
				
							fwrite(pFrame->data[2]+i*pFrame->linesize[2],1,pVCodecCtx->width/2,fp);
				
						}
						fclose(fp);return;
				}
			}
		}else if (avpkg.stream_index == audioIndex)
		{
// 			ret=avcodec_decode_audio4(pACodecCtx,pFrame,&gotyuv,&avpkg);
// 			if (ret<0)
// 			{
// 				TRACE("decode audio error!\n");
// 			}
// 			if (gotpcm)
// 			{
// 			}
		}

		av_free_packet(&avpkg); 
	}
	
	fclose(fp);
}


DWORD WINAPI MyThreadProc1(LPVOID lpParameter)
{

	#define  WIDTH (1280)
	#define  HEIGHT	(720)
	void *szData = NULL;
	int screen_w=0,screen_h=0;  
	
	SDL_Surface *screen;   
	SDL_Window *pWindow;
	//SDL_Overlay *bmp; 

	SDL_Thread *video_tid;  
	SDL_Event event;
	SDL_Rect sdlRT;
	sdlRT.h = HEIGHT;
	sdlRT.w = WIDTH;
	sdlRT.x = 0;
	sdlRT.y = 0;

	SDL_Rect dstRT;
	dstRT.h = HEIGHT;
	dstRT.w = WIDTH;
	dstRT.x = 0;
	dstRT.y = 0;
	FILE * pFile = fopen( "output.yuv", "rb" );
	if ( pFile == NULL )
	{
		int u = 0;
	}
	szData=av_malloc(WIDTH*HEIGHT*2);
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)<0)
	{
		exit(-1);
	}

	//pWindow=SDL_CreateWindow("Hello World!", 10, 20, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	//Sleep(1000);
	pWindow=SDL_CreateWindowFrom((void *)lpParameter);
	int iPitch = WIDTH*SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_IYUV);    

	int iWidth = 0;
	int iHeight = 0;
	SDL_GetWindowSize(pWindow, &iWidth, &iHeight );
	dstRT.h = iHeight;
	dstRT.w = iWidth;
	int iii = SDL_GetNumRenderDrivers();
	TRACE("ifo=%d,%d %d\n",iWidth,iHeight,iPitch);
	SDL_Renderer * pRender = SDL_CreateRenderer( pWindow, 0, SDL_RENDERER_ACCELERATED );
	SDL_RendererInfo info;
	SDL_GetRendererInfo(pRender, &info);

	SDL_GetRenderDriverInfo(0, &info);    //d3d
	TRACE("iii=%s\n",info.name);
	SDL_GetRenderDriverInfo(1, &info);    //opgl
	TRACE("iii=%s\n",info.name);
	SDL_GetRenderDriverInfo(2, &info);    //opgle2
	TRACE("iii=%s\n",info.name);
	SDL_GetRenderDriverInfo(3, &info);    //software
	TRACE("iii=%s\n",info.name);

	SDL_Texture * pTexture = SDL_CreateTexture( pRender,SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT );
	
	//读yuv文件，该文件中存放的数据尺寸为 1280*720的yv12数据
	
	while ( fread( szData, 1, (WIDTH*HEIGHT*3)/2, pFile ) != NULL )
	{
		int i = SDL_UpdateTexture( pTexture,&sdlRT,szData,iPitch);
		SDL_RenderClear( pRender );
		SDL_RenderCopy( pRender, pTexture, &sdlRT, &dstRT );
		SDL_RenderPresent( pRender );
		Sleep(40);
	}
	fclose(pFile);
	if ( pTexture != NULL )
	{
		SDL_DestroyTexture( pTexture );
		pTexture = NULL    ;
	}

	if ( pRender != NULL )
	{
		SDL_DestroyRenderer( pRender );
		pRender = NULL;
	}


	if ( NULL != pWindow )
	{
		SDL_DestroyWindow( pWindow );
		pWindow = NULL;
	}
	SDL_Quit();
	return 0;
}
void CplayerDlg::OnBnClickedButton2()
{	
	HANDLE handle1;
	handle1 = CreateThread(NULL,0,MyThreadProc1,( GetDlgItem(IDC_STATICVIDEO)->GetSafeHwnd()),0,NULL);
	


}

 

int av_usleep(int64_t usec)
{

	Sleep(usec / 1000);
	return 0;
}

int64_t av_gettime_relative(void)
{
	FILETIME ft;
	int64_t t;
	GetSystemTimeAsFileTime(&ft);
	t = (int64_t)ft.dwHighDateTime << 32 | ft.dwLowDateTime;
	return t / 10 - 11644473600000000; /* Jan 1, 1601 */
}
static void set_clock_at(Clock *c, double pts, int serial, double time)
{
	c->pts = pts;
	c->last_updated = time;
	c->pts_drift = c->pts - time;
	c->serial = serial;
}
static void set_clock(Clock *c, double pts, int serial)
{
	double time = av_gettime_relative() / 1000000.0;
	set_clock_at(c, pts, serial, time);
}
static void init_clock(Clock *c, int *queue_serial)
{
	c->speed = 1.0;
	c->paused = 0;
	c->queue_serial = queue_serial;
	set_clock(c, NAN, -1);
}


/* The audio function callback takes the following parameters:  
 * stream: A pointer to the audio buffer to be filled  
 * len: The length (in bytes) of the audio buffer  
 * 回调函数 
*/   
void  fill_audio(void *udata,Uint8 *stream,int len){   
	AVPacket pkt = { 0 };
	int serial,len1,got_frame,ret;
	AVRational tb;
	while(len >0)
	{
		if (is->audio_buf_index >= is->audio_buf_size)
		{
		
		if (!is->frame) {
			if (!(is->frame = av_frame_alloc()))
				TRACE("alloc frame error %d\n",AVERROR(ENOMEM));
				return;
		} else {
			av_frame_unref(is->frame);
		}
		if (packet_queue_get(&is->audioq, &pkt, 1, &serial) < 0)
		{
			return;
		}				
		AVCodecContext *dec = is->audio_st->codec;
		ret = avcodec_decode_audio4(dec, is->frame, &got_frame, &pkt);
		if (ret < 0) {
			/* if error, we skip the frame */
			TRACE("avcodec_decode_audio4 fail\n");
			return;
		}
		//tb.den=1000;
		//tb.num=1;
		//is->frame->pts = av_rescale_q(is->frame->pkt_pts, is->audio_st->time_base, tb);		
		//is->audio_clock = is->frame->pts * av_q2d(tb) + (double) is->frame->nb_samples / is->frame->sample_rate;
		is->audio_clock =is->frame->pkt_pts*av_q2d(is->audio_st->time_base);
		TRACE("Audio clock=%0.3f\n",is->audio_clock);	
		if (got_frame)
		{
			ret=swr_convert(is->swr_ctx,&is->audio_buf, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)is->frame->data ,is->frame->nb_samples); 			
			if(ret>0)
			{
				
				is->audio_buf_size=av_samples_get_buffer_size(NULL,av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO),ret,AV_SAMPLE_FMT_S16,1);
				is->audio_buf_index=0;	
				//fwrite(out_buffer,1,is->audio_buf_size,fptmp);
				//TRACE("decode audio=%d! %d\n",is->frame->linesize[0],is->audio_buf_size);
				
			}
			
		}		  			
		
		}
		//SDL_memset(stream, 0, len); 
		len1=is->audio_buf_size - is->audio_buf_index;
		if(len1>len) len1=len;  			  	
		//SDL_MixAudio(stream,audio_pos,len,SDL_MIX_MAXVOLUME);  	
		memcpy(stream,is->audio_buf+is->audio_buf_index,len1);			
		is->audio_buf_index+=len1;
		len-=len1;
		stream+=len1;
	}
	
}   
static void refresh_loop_wait_event(VideoState *is, SDL_Event *event) {
	double remaining_time = 0.0;
	SDL_PumpEvents();
	while (!SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_FIRSTEVENT,SDL_LASTEVENT)) {		
		if (remaining_time > 0.0)av_usleep((int64_t)(remaining_time * 1000000.0));
		remaining_time = REFRESH_RATE;		
		//video_refresh(is, &remaining_time);
		SDL_PumpEvents();
	}
}
/* handle an event sent by the GUI */
static void event_loop(VideoState *cur_stream)
{
	SDL_Event event;
	double incr, pos, frac;
	for (;;) {
		double x;
		refresh_loop_wait_event(cur_stream, &event);
		switch (event.type) {		
		case SDL_QUIT:		
			break;
		default:
			break;
		}
	}
}
int MyThreadRead2(LPVOID lpParameter)
{	
	int i,videoIndex=-1,audioIndex=-1,gotyuv,ret;
	int vHeight,vWidth;
	AVFormatContext *pInfmctx=NULL;
	AVCodecContext *pVCodecCtx=NULL;
	AVCodecContext *pACodecCtx=NULL;
	AVCodec *pACodec=NULL;
	AVCodec *pVCodec=NULL;
	AVPacket avpkg={0,0};
	AVFrame *pFrame,*pFrameYUV; 
	struct SwsContext *img_convert_ctx;
	unsigned char *outbuf=NULL;
	unsigned char *szData = NULL;
	SDL_Window *pWindow;
	SDL_Rect sdlRT;SDL_Rect dstRT;
	TCHAR szFilter[] = _T("视频(*.mp4)|*.mp4|"); 
	SDL_AudioSpec sdlaudio;	
	CFileDialog fileDlg(TRUE, _T("MP4"), NULL, 0, szFilter, 0);//这个无用，但是不能取消，取消后SDL_AudioOpen失败	
	CString strFilePath;
	char filepath[2024];
	TRACE("filepath=%s\n",is->filename);
	avformat_open_input(&pInfmctx,is->filename,NULL,NULL);
	avformat_find_stream_info(pInfmctx,NULL);
	for (i=0;i<pInfmctx->nb_streams;i++)
	{
		if (pInfmctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoIndex=i;
		}else if (pInfmctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
		{
			audioIndex=i;

		}
	}

	pVCodecCtx=pInfmctx->streams[videoIndex]->codec;
	if (pVCodecCtx == NULL)
	{
		TRACE("can not find avdocecctx!!\n");
		return 0;
	}
	vWidth=pVCodecCtx->width;
	vHeight=pVCodecCtx->height;
	sdlRT.h = vHeight;
	sdlRT.w = vWidth;
	sdlRT.x = 0;
	sdlRT.y = 0;		
	dstRT.x = 0;
	dstRT.y = 0;
	pWindow=SDL_CreateWindowFrom((void *)is->dispwnd);
	SDL_Renderer * pRender = SDL_CreateRenderer( pWindow, 0, SDL_RENDERER_ACCELERATED );
	SDL_Texture * pTexture = SDL_CreateTexture( pRender,SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, vWidth, vHeight );
	int iPitch = vWidth*SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_IYUV);    
	int iWidth = 0;
	int iHeight = 0;
	SDL_GetWindowSize(pWindow, &iWidth, &iHeight );
	dstRT.h = iHeight;
	dstRT.w = iWidth;
	/////////////////


	pVCodec=avcodec_find_decoder(pVCodecCtx->codec_id);

	if (pVCodec == NULL)
	{
		TRACE("can not find avdocec!!\n");
		return 0;
	}

	pACodecCtx=pInfmctx->streams[audioIndex]->codec;
	if (pVCodecCtx == NULL)
	{
		TRACE("can not find avdocecctx!!\n");
		return 0;
	}
	pACodec=avcodec_find_decoder(pACodecCtx->codec_id);

	if (pACodec == NULL)
	{
		TRACE("can not find pACodec!!\n");
		return 0;
	}
	if(avcodec_open2(pVCodecCtx,pVCodec,NULL)<0)
	{
		TRACE("can not avcodec_open2!!\n");
		return 0;
	}
	if(avcodec_open2(pACodecCtx,pACodec,NULL)<0)
	{
		TRACE("can not avcodec_open2!!\n");
		return 0;
	}	
	szData=(unsigned char*)av_malloc(vWidth*vHeight*2);
	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	outbuf=(unsigned char*)av_malloc(avpicture_get_size(PIX_FMT_YUV420P,pVCodecCtx->width,pVCodecCtx->height));

	avpicture_fill((AVPicture *)pFrameYUV, outbuf, PIX_FMT_YUV420P, pVCodecCtx->width, pVCodecCtx->height); 
	av_init_packet(&avpkg);
	img_convert_ctx = sws_getContext(pVCodecCtx->width, pVCodecCtx->height, pVCodecCtx->pix_fmt,   
		pVCodecCtx->width, pVCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	sdlaudio.freq = pACodecCtx->sample_rate;   
	sdlaudio.format = AUDIO_S16SYS;   
	sdlaudio.channels = pACodecCtx->channels;   
	sdlaudio.silence = 0;   
	sdlaudio.samples = 1024; //播放AAC，M4a，缓冲区的大小  
	//wanted_spec.samples = 1152; //播放MP3，WMA时候用  
	sdlaudio.callback = fill_audio;   
	sdlaudio.userdata = pACodecCtx; 
	int64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
	AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;
	int out_nb_samples=1024;
	int out_sample_rate=44100;  
	 int out_channels=av_get_channel_layout_nb_channels(out_channel_layout); 
	int64_t in_channel_layout=av_get_default_channel_layout(pACodecCtx->channels);	 
	 //Out Buffer Size  
	 int out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);  
	 uint8_t *out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);	
	 struct SwrContext *au_convert_ctx;
	au_convert_ctx = swr_alloc();  
	au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,  
	in_channel_layout,pACodecCtx->sample_fmt , pACodecCtx->sample_rate,0, NULL);  
	swr_init(au_convert_ctx);
	if (SDL_OpenAudio(&sdlaudio, NULL)<0)//步骤（2）打开音频设备   
	{   
		TRACE("can't open audio.\n");   
		return 0;   
	}   
	//for (;;)
	{
	
	while(av_read_frame(pInfmctx, &avpkg)>=0){ 
		if(avpkg.stream_index == videoIndex)TRACE("VVVVVVVVVVVVVVVV\n");
		if (avpkg.stream_index == videoIndex)
		{
			//video
			ret=avcodec_decode_video2(pVCodecCtx,pFrame,&gotyuv,&avpkg);						
			if (ret<0)
			{
				TRACE("decode frame error!\n");
			}
			if (gotyuv)
			{
				//Sleep(10);//33毫秒
				TRACE("gotyuv!=%d==%d\n",pVCodecCtx->width*pVCodecCtx->height*2);
				//fwrite(pFrame->data[0],1,pVCodecCtx->width*pVCodecCtx->height,fp);
				//fwrite(pFrame->data[1],1,pVCodecCtx->width*pVCodecCtx->height/4,fp);//yu12
				//fwrite(pFrame->data[2],1,pVCodecCtx->width*pVCodecCtx->height/4,fp);

				memcpy(szData,pFrame->data[0],vWidth*vHeight);
				memcpy(szData+vWidth*vHeight,pFrame->data[1],vWidth*vHeight/4);
				memcpy(szData+vWidth*vHeight+vWidth*vHeight/4,pFrame->data[2],vWidth*vHeight/4);
				SDL_UpdateTexture( pTexture,&sdlRT,szData,iPitch);
				SDL_RenderClear( pRender );
				SDL_RenderCopy( pRender, pTexture, &sdlRT, &dstRT );
				SDL_RenderPresent( pRender );
			}

		}else if (avpkg.stream_index == audioIndex)
		{
		//sound
			ret=avcodec_decode_audio4(pACodecCtx,pFrame,&gotyuv,&avpkg);
			if (ret<0)
			{
				TRACE("decode audio error!\n");
			}
			if (gotyuv)
			{						
				ret=swr_convert(au_convert_ctx,&out_buffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)pFrame->data ,pFrame->nb_samples); 

				if(ret>0)
					{
						out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,ret,out_sample_fmt, 1);
						//fwrite(out_buffer,1,out_buffer_size,fp);
						TRACE("decode audio=%d! %d\n",pFrame->linesize[0],out_buffer_size);
						//Set audio buffer (PCM data)  
						audio_chunk = (Uint8 *) out_buffer;   
						//Audio buffer length  
						audio_len =out_buffer_size;  
						audio_pos = audio_chunk;  
						//回放音频数据   
						SDL_PauseAudio(0);  						  
						while(audio_len>0)//等待直到音频数据播放完毕!   
							SDL_Delay(1);   
						//--------------------------------------- 
						}
			}
		}
		av_free_packet(&avpkg); 
	}

	av_seek_frame(pInfmctx,videoIndex,0,AVSEEK_FLAG_ANY);
	}

	if ( pTexture != NULL )
	{
		SDL_DestroyTexture( pTexture );
		pTexture = NULL    ;
	}

	if ( pRender != NULL )
	{
		SDL_DestroyRenderer( pRender );
		pRender = NULL;
	}


	if ( NULL != pWindow )
	{
		SDL_DestroyWindow( pWindow );
		pWindow = NULL;
	}
	
	SDL_CloseAudio();
	SDL_Quit();

	return 0;
}
static int video_thread(LPVOID lpParameter)
{		
	AVPacket pkt = { 0 };
	int serial,gotyuv,ret;
	AVFrame *frame = av_frame_alloc();
	AVCodecContext *pVCodecCtx = is->video_st->codec;
	int iPitch = pVCodecCtx->width*SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_IYUV);   
	unsigned char *szData=(unsigned char*)av_malloc(pVCodecCtx->width*pVCodecCtx->height*2);
	AVRational tb;
	tb.den=1000;
	tb.num=1;
	
	double delay=1000;
	while (1)
	{
		
		if (packet_queue_get(&is->videoq, &pkt, 1, &serial) < 0)
		{
				return -1;
		}
		
		delay=1000;
		ret=avcodec_decode_video2(pVCodecCtx,frame,&gotyuv,&pkt);						
		if (ret<0)
		{
			TRACE("decode frame error!\n");
		}
		if (gotyuv)
		{
		
			//frame->pts = av_rescale_q(frame->pkt_pts,is->video_st->time_base,tb);
			is->frame_timer =frame->pkt_pts*av_q2d(is->video_st->time_base);			
			is->audio_diff_cum=is->audio_clock - is->frame_timer;			
			delay=(is->frame_timer - is->frame_last_returned_time)*1000000.0;
			if (is->audio_diff_cum >0)//需要加速
			{
				delay-=fabs(is->audio_diff_cum)*1000000.0;
			}else
			{
				delay+=fabs(is->audio_diff_cum)*1000000.0;
			}
			//av_usleep((int64_t)(delay));
			//delay=33366;
			if(delay>50*1000){
				delay=50*1000;
				//TRACE("Audio-Video diff=%0.3f %0.3f sec\n",is->audio_diff_cum,delay);
			}else if (delay<1000)
			{
				delay=1000;
			}
			av_usleep(delay);
			//TRACE("Audio-Video diff=%0.3f %0.3f sec\n",is->audio_diff_cum,delay);
			is->frame_last_returned_time=is->frame_timer;
			//is->vidclk = is->frame->pts * av_q2d(tb) + (double) is->frame->nb_samples / is->frame->sample_rate;
			//TRACE("gotyuv!=%d==%d\n",pVCodecCtx->width*pVCodecCtx->height*2);
			//fwrite(pFrame->data[0],1,pVCodecCtx->width*pVCodecCtx->height,fp);
			//fwrite(pFrame->data[1],1,pVCodecCtx->width*pVCodecCtx->height/4,fp);//yu12
			//fwrite(pFrame->data[2],1,pVCodecCtx->width*pVCodecCtx->height/4,fp);
			//memcpy(szData,frame->data[0],pVCodecCtx->width*pVCodecCtx->height);
			//memcpy(szData+pVCodecCtx->width*pVCodecCtx->height,frame->data[1],pVCodecCtx->width*pVCodecCtx->height/4);
			//memcpy(szData+pVCodecCtx->width*pVCodecCtx->height+pVCodecCtx->width*pVCodecCtx->height/4,frame->data[2],pVCodecCtx->width*pVCodecCtx->height/4);
			int a=0,i=0;
			for (i=0;i<pVCodecCtx->height;i++)
			{
				memcpy(szData+a,frame->data[0]+i*frame->linesize[0],pVCodecCtx->width);
				a+=pVCodecCtx->width;
			}
			for (i=0;i<pVCodecCtx->height/2;i++)
			{
				memcpy(szData+a,frame->data[1]+i*frame->linesize[1],pVCodecCtx->width/2);//yu12
				a+=pVCodecCtx->width/2;
			}
			for (i=0;i<pVCodecCtx->height/2;i++)
			{

				memcpy(szData+a,frame->data[2]+i*frame->linesize[2],pVCodecCtx->width/2);
				a+=pVCodecCtx->width/2;

			}

			SDL_UpdateTexture( is->pTexture,&is->sdlRT,szData,iPitch);
			SDL_RenderClear( is->pRender );
			SDL_RenderCopy( is->pRender, is->pTexture, &is->sdlRT, &is->dstRT );
			SDL_RenderPresent( is->pRender );
		}
		av_free_packet(&pkt);
		
	}
	
	return 0;
}
int MyThreadRead(LPVOID lpParameter)
{
	VideoState *is=(VideoState *)lpParameter;
	AVPacket avpkg={0,0};
	AVPacket *pkt=&avpkg;
	AVCodecContext *pVCodecCtx=NULL;
	AVCodecContext *pACodecCtx=NULL;
	AVCodec *pACodec=NULL;
	AVCodec *pVCodec=NULL;
	AVFrame *pFrame,*pFrameYUV;
	int i,videoIndex=-1,audioIndex=-1,gotyuv,ret,j;

	fptmp=fopen("test.pcm","wb");
	CoInitialize(NULL);//没有这句，SDL_Audioopen会失败
	TRACE("filepath=%s\n",is->filename);
	ret=avformat_open_input(&is->ic,is->filename,NULL,NULL);
	if (ret < 0) {
		TRACE("avformat_open_input error =%d\n", ret);		
		goto fail;
	}
	avformat_find_stream_info(is->ic,NULL);
	j=0;
	for (i=0;i<is->ic->nb_streams;i++)
	{
		if (is->ic->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoIndex=is->iVideoStreamIndex=i;
		}else if (is->ic->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
		{
			audioIndex=is->iAudioStreamIndex[is->iAudioStreamIndexNum]=i;
			is->iAudioStreamIndexNum++;

		}
	}	
	is->audio_st = is->ic->streams[audioIndex];
	is->duration=is->ic->streams[audioIndex]->duration*av_q2d(is->audio_st->time_base);
	is->video_st = is->ic->streams[videoIndex];
	pVCodecCtx=is->ic->streams[videoIndex]->codec;
	pVCodec=avcodec_find_decoder(pVCodecCtx->codec_id);
	if (pVCodec == NULL)
	{
		TRACE("can not find avdocec!!\n");
		return 0;
	}
	if(avcodec_open2(pVCodecCtx,pVCodec,NULL)<0)
	{
		TRACE("can not avcodec_open2!!\n");
		return 0;
	}
	
	pVCodecCtx=is->ic->streams[videoIndex]->codec;
	if (pVCodecCtx == NULL)
	{
		TRACE("can not find avdocecctx!!\n");
		goto fail;
	}
	is->width=pVCodecCtx->width;
	is->height=pVCodecCtx->height;	
	int iWidth = 0;
	int iHeight = 0;
	iWidth=pVCodecCtx->width;
	iHeight=pVCodecCtx->height;
	is->sdlRT.h = iHeight;
	is->sdlRT.w = iWidth;
	is->sdlRT.x = 0;
	is->sdlRT.y = 0;		
	is->dstRT.x = 0;
	is->dstRT.y = 0;
	is->pwind=SDL_CreateWindowFrom((void *)is->dispwnd);
	is->pRender = SDL_CreateRenderer( is->pwind, 0, SDL_RENDERER_ACCELERATED );
	is->pTexture = SDL_CreateTexture( is->pRender,SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pVCodecCtx->width, pVCodecCtx->height );	   	
	SDL_GetWindowSize(is->pwind, &iWidth, &iHeight );
	is->dstRT.h = iHeight;
	is->dstRT.w = iWidth;
	pACodecCtx=is->ic->streams[audioIndex]->codec;	
	pACodec=avcodec_find_decoder(pACodecCtx->codec_id);
	if (pACodec == NULL)
	{
		TRACE("can not find pACodec!!\n");
		goto fail;
	}
	if(avcodec_open2(pACodecCtx,pACodec,NULL)<0)
	{
		TRACE("can not avcodec_open2!!\n");
		return 0;
	}		
	is->audio_buf_size  = 0;
	is->audio_buf_index = 0;
	is->audio_buf=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);	
	SDL_AudioSpec want,have;
	want.freq = pACodecCtx->sample_rate;   
	want.format = AUDIO_S16SYS;   
	want.channels = pACodecCtx->channels;   
	want.silence = 0;   
	want.samples = 1024; //播放AAC，M4a，缓冲区的大小  
	//wanted_spec.samples = 1152; //播放MP3，WMA时候用  
	want.callback = fill_audio;   
	want.userdata = pACodecCtx;
	int64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
	AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;
	int out_nb_samples=1024;
	int out_sample_rate=pACodecCtx->sample_rate;  
	int out_channels=av_get_channel_layout_nb_channels(out_channel_layout); 
	int64_t in_channel_layout=av_get_default_channel_layout(pACodecCtx->channels);	 
	int out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);  
	out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);	 
	//Out Buffer Size  		
	struct SwrContext *au_convert_ctx;
	is->swr_ctx = swr_alloc();  
	is->swr_ctx=swr_alloc_set_opts(is->swr_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,  
	in_channel_layout,pACodecCtx->sample_fmt , pACodecCtx->sample_rate,0, NULL);  
	swr_init(is->swr_ctx);
	packet_queue_start(&is->audioq);//创建解码线程前调用
	packet_queue_start(&is->videoq);
	is->video_tid = SDL_CreateThread(video_thread,0,is);
	if (SDL_OpenAudio(&want, &have)<0)//步骤（2）打开音频设备   
	{   
		TRACE("can't open audio err=%s\n",SDL_GetError());   
		return 0;   
	}
	SDL_PauseAudio(0);
	
	while(av_read_frame(is->ic, pkt)>=0){
		av_usleep(3000);
		if (pkt->stream_index == is->iAudioStreamIndex[0]) {
			packet_queue_put(&is->audioq, pkt);
		}
		else if (pkt->stream_index == is->iVideoStreamIndex)
		{
			packet_queue_put(&is->videoq, pkt);audio_len=out_buffer_size;audio_pos=out_buffer;
		}else
		{

			av_free_packet(pkt);
		}
		if (is->audioq.size>1024*1024*5 || is->videoq.size>1024*1024*5)
		{
			av_usleep(100000);
		}
		
	}


fail:
	return 0;
}
int MyThreadDiaplsyVideo2(LPVOID lpParameter)
{	
	VideoState *is=(VideoState *)lpParameter;
	is->ytop    = 0;
	is->xleft   = 0;

	/* start video display */
	is->pictq_mutex = SDL_CreateMutex();
	is->pictq_cond  = SDL_CreateCond();

	is->subpq_mutex = SDL_CreateMutex();
	is->subpq_cond  = SDL_CreateCond();

	packet_queue_init(&is->videoq);
	packet_queue_init(&is->audioq);
	packet_queue_init(&is->subtitleq);

	is->continue_read_thread = SDL_CreateCond();

	init_clock(&is->vidclk, &is->videoq.serial);
	init_clock(&is->audclk, &is->audioq.serial);
	init_clock(&is->extclk, &is->extclk.serial);
	is->audio_clock_serial = -1;
	is->audio_last_serial = -1;
	is->av_sync_type = 1;
	is->read_tid     = SDL_CreateThread(MyThreadRead,0,is);
	if (!is->read_tid) {
		av_free(is);
		return NULL;
	}
	event_loop(is);
}
void CplayerDlg::OnBnClickedButton3()
{
	HANDLE handle1;	
	is = (VideoState*)av_mallocz(sizeof(VideoState));
	if (!is)return;
	memset(is,0,sizeof(VideoState));
	TCHAR szFilter[] = _T("视频(*.mp4)|*.mp4|"); 
	CFileDialog fileDlg(TRUE, _T("MP4"), NULL, 0, szFilter, 0); 
	CString strFilePath;	
	if (IDOK==fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();		
		CStringA strTempA(strFilePath);
		const char *chartemp = (LPCSTR)strTempA;
		memcpy(is->filename,chartemp,strTempA.GetLength()+1);
		is->dispwnd=GetDlgItem(IDC_STATICVIDEO)->GetSafeHwnd();
		TRACE("filename=%s\n",is->filename);
		//handle1 = CreateThread(NULL,0,MyThreadRead2,is,0,NULL);MyThreadDiaplsyVideo2
		SDL_CreateThread(MyThreadDiaplsyVideo2,0,is);
		SetWindowText(strFilePath); 
		SetTimer(1, 1000, NULL);
		//handle1 = CreateThread(NULL,0,MyThreadDiaplsyVideo,( GetDlgItem(IDC_STATICVIDEO)->GetSafeHwnd()),0,NULL);
	}
	else
	{
		return;
	}
	
	//handle1 = CreateThread(NULL,0,MyThreadPlayVideo,0,0,NULL);
}


void CplayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString tmp;
	int clock=is->audio_clock;
	int duration=is->duration;
	switch (nIDEvent)   
	{   
		case 1:	
			{
			tmp.Format(_T("%2d:%2d:%2d / %2d:%2d:%2d"),clock/3600,clock%3600/60,clock%60,duration/3600,duration%3600/60,duration%60);
			m_msg.SetWindowText(tmp);
			}
			break;
		default:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}



void CplayerDlg::OnBnClickedButton4()
{
	char ip[36];
	CString str;
	CString strFilePath("191");
	size_t sz=0;	
	
	GetDlgItem(IDC_IPADDRESS1)->GetWindowTextW(str);
	//strcpy(ip,str.GetBuffer(0));
	wcstombs_s(&sz,ip,str.GetBuffer(0),str.GetLength());
	char passwd[20];
	strcpy(passwd,"123");
	conn->Enctype=1;
	conn->H264File.Open(strFilePath+".264",CFile::modeCreate | CFile::modeWrite);
	conn->AacFile.Open(strFilePath+".aac",CFile::modeCreate | CFile::modeWrite);
	conn->saveStream=0;
	conn->iAudioIdx=0;
	conn->iVideoIdx=1;
	conn->Connect(0,ip,3100,passwd,0);	
	m_Dec->vWidth=640;m_Dec->vHeight=360;
	m_Dec->dispwnd=GetDlgItem(IDC_STATICVIDEO)->GetSafeHwnd();
	m_Dec->StartDecode();
}

BOOL CplayerDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message) {
	case RM_ST_FAIL:	//连接失败		
		GetDlgItem(IDC_STATIC1)->SetWindowTextW(_T("未连接"));
		::AfxMessageBox(_T("连接失败"));
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("连接"));
		(GetDlgItem(IDC_BUTTON1))->EnableWindow(true);
		break;
	case RM_ST_SUCCESS://连接成功
		//::AfxMessageBox(_T(""));
		GetDlgItem(IDC_STATIC1)->SetWindowTextW(_T("已连接"));
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("断开"));
		(GetDlgItem(IDC_BUTTON1))->EnableWindow(true);
		break;
	case RM_ST_CONNING://连接中		
		GetDlgItem(IDC_STATIC1)->SetWindowTextW(_T("连接中..."));
		break;	
	case RM_DISCONN://断开
		GetDlgItem(IDC_STATIC1)->SetWindowTextW(_T("已断开"));
		(GetDlgItem(IDC_BUTTON1))->EnableWindow(true);
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("连接"));
		::AfxMessageBox(_T("编码器断开"));
		break;
	case RM_PASSWD_ERR://密码错误
		GetDlgItem(IDC_STATIC1)->SetWindowTextW(_T("已断开"));
		(GetDlgItem(IDC_BUTTON1))->EnableWindow(true);
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("连接"));
		::AfxMessageBox(_T("连接密码错误"));
		break;	
	case RM_UPDATE_STATUS://升级状态				
		//m_Progress.SetPos(pMsg->lParam);	

		break;
	default:
		break;
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CplayerDlg::OnBnClickedRadio1()//1
{
	UpdateData(TRUE);
	conn->iAudioIdx=m_audiochan;
}

void CplayerDlg::OnBnClickedRadio2()//2
{
	UpdateData(TRUE);
	conn->iAudioIdx=m_audiochan;
}


void CplayerDlg::OnBnClickedRadio6()//3
{
	UpdateData(TRUE);
	conn->iAudioIdx=m_audiochan;
}

void CplayerDlg::OnBnClickedRadio7()
{
	UpdateData(TRUE);
	conn->iAudioIdx=m_audiochan;
}

void CplayerDlg::OnBnClickedRadio13()
{
	UpdateData(TRUE);
	conn->iAudioIdx=m_audiochan;
}

void CplayerDlg::OnBnClickedRadio14()
{
	UpdateData(TRUE);
	conn->iAudioIdx=m_audiochan;
}


void CplayerDlg::OnBnClickedRadio17()
{
	UpdateData(TRUE);
	conn->iAudioIdx=m_audiochan;
}

void CplayerDlg::OnBnClickedRadio12()
{
	UpdateData(TRUE);
	conn->iAudioIdx=m_audiochan;
}
