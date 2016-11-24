#ifndef __DEFINE_H__
#define __DEFINE_H__


#define RM_CONNECT_BEGIN	WM_USER + 4020
#define UM_FIXFINSH			WM_USER + 1020
#define UM_FIXPROGESS		UM_FIXFINSH + 1
#define RM_CONNECT_ERROR	RM_CONNECT_BEGIN + 1
#define RM_CONNECT_SUCCESS	RM_CONNECT_BEGIN + 2

#define RM_ST_FAIL		RM_CONNECT_BEGIN + 3		//连接失败
#define RM_ST_SUCCESS	RM_CONNECT_BEGIN + 4		//连接成功
#define RM_ST_CONNING	RM_CONNECT_BEGIN + 5		//正在连接

#define RM_ERR_PARAM		RM_CONNECT_BEGIN + 6		//参数错误
#define RM_DISCONN			RM_CONNECT_BEGIN + 7		//断开
#define RM_ERR_UPDATE		RM_CONNECT_BEGIN + 8		//升级错误
#define RM_PASSWD_ERR		RM_CONNECT_BEGIN + 9		//密码错误
#define RM_UPDATE_STATUS	RM_CONNECT_BEGIN + 10		//

#define RM_REC_DATA			RM_CONNECT_BEGIN + 11		//DATA数据
#define HEAD_LEN			sizeof(MSGHEAD)
struct MulAddr
{
	char chIP[16];
 	int nPort;
};
struct HDB_MSGHEAD
{
	WORD nLen;
	WORD nVer;
	BYTE nMsg;
	BYTE szTemp[3];
};


struct frame_bitrate
{
	int nFrame;
	int nBitrate;
};

#define PORT  3100
#define PORT1 3110
#define PORT2 3120

#define MAXNUM 4
#define CMDHEAD 2
#define ADTS 					0x53544441
#define AAC						0x43414130
#define PCM						0x4d435030

typedef unsigned short WORD ;
typedef unsigned char  BYTE;

#define DSP_NUM        	4
typedef struct
{
	unsigned int x0;
	unsigned int x1;
	unsigned int x2;
	unsigned int AccLen;
	unsigned int flag;
	unsigned int x5;
	unsigned int x6;
}AUDIO_Params;
/* 获得编码后的图像参数*/
/*系统参数表*/
typedef struct __SYSPARAMS {
	unsigned char szMacAddr[8];				//MAC 地址
	unsigned int dwAddr;					//IP 地址
	unsigned int dwGateWay;					//网关
	unsigned int dwNetMark;					//子网掩码
	char strName[16];						//产品序列号
	char strVer[10];						//编码器版本号
	WORD unChannel;							//通道数
	char bType; 							//区分编码器类型  0 -------VGABOX     3-------200 4-------110 5-------120 6--------1200  8---ENC-1100
	char bTemp[3]; 							//保留
	int nTemp[7];							//保留
} SYSPARAMS;
/*视频参数*/
typedef struct __VIDEOPARAM
{
    DWORD nDataCodec;   					//编码方式
											//视频：mmioFOURCC('H','2','6','4');
	DWORD nFrameRate;   					//视频：数据帧率
	DWORD nWidth;       					//视频：宽度
	DWORD nHight;       					//视频：高度
	DWORD nColors;      						//视频：颜色数
	DWORD nQuality;							//视频：质量系数
	WORD sCbr;								//视频：定码率/定质量
	WORD sBitrate;							//视频：带宽
}VideoParam;

/*音频参数*/
typedef struct __AUDIOPARAM
{
	DWORD Codec;  							//编码方式
    DWORD SampleRate; 						//采样率
	DWORD BitRate;  						//码率
	DWORD Channel;  						//声道数
	DWORD SampleBit;  						//量化位数
	BYTE  LVolume;							//左声道音量  0 --------31
	BYTE  RVolume;							//右声道音量  0---------31
	WORD  InputMode;                        // 1------ 麦克输入  0------线性输入	
}AudioParam;
typedef struct _SysParamsV2
{
	unsigned char szMacAddr[8];
	unsigned int dwAddr;
	unsigned int dwGateWay;
	unsigned int dwNetMark;
	char strName[16];
	int nFrame;
	int nColors;
	int nQuality;
	char strVer[10];
	char strAdminPassword[16];
	char strUserPassword[16];
	short sLeftRight;
	short sUpDown;
	////// ver 2.0//////////////////
	short sCbr;  //
	short sBitrate; //
	short sCodeType;//  0---RCH0     0f -----H264
	char  strCodeVer[8];    //
	BYTE bVmode;		//视频模式    0 ------- D1  1 ------CIF   2------4CIF 
	BYTE bType;            // 0 -------VGABOX     0f-------AVBOX
	char sTemp[26];	
//	int nTemp[7];
	////// ver 1.0
}SysParamsV2;
/*加字幕结构标题*/
typedef struct __RecAVTitle
{
    int  x;                 				//x pos
	int  y;        				//y pos
	int len;   //Text实际长度
	char Text[65];          				//text
}RecAVTitle;

/*添加标题*/
typedef struct  __RTitle__ {	
	unsigned int uflag;	
	RecAVTitle title;
}RTitle;

/*系统保存参数表*/
typedef struct __SAVEPARAM {
#if 0
	SYSPARAMS sysPara;                 		//标准系统参数
#else
	SysParamsV2 sysPara;
#endif

	VideoParam videoPara[DSP_NUM];				 	//视频参数
	AudioParam audioPara[DSP_NUM];					//音频参数
}SavePTable;



typedef struct Dm642Arg  {
	unsigned long width;
	unsigned long height;
	int gmode;
	unsigned long dviflag;
	unsigned long framesize;
	unsigned long flag;
	unsigned long checksum;
	unsigned long IFrame;
}Pic_params;
/*消息头，每个消息发送时均包含此结构*/
typedef struct __HDB_MSGHEAD							
{
	/*   
	## 长度，通过htons转换
	## 消息长度，包含结构头
	## 和后面的所有数据
	*/
	WORD	nLen;		
	WORD	nVer;							//版本
	BYTE	nMsg;							//消息字
	BYTE	szTemp[3];						//保留
}MSGHEAD;

typedef struct _DataHeader
{
    DWORD   id;
	long    biWidth;							//VGA：Width		Audio：SampleRate
	long    biHeight;							//VGA：Height	Audio：BitRate
	WORD    biBitCount;							//VGA：BitCount	Audio：Channel
	DWORD   fccCompressedHandler;				//VGA：mmioFOURCC (‘H’,’2’,’6’,’4’)
											    //Audio：mmioFOURCC(‘A’,’D’,’T’,’S’)
	DWORD   fccHandler;	   						//Audio：SampleBit
    DWORD   dwCompressedDataLength;
    DWORD   dwFlags;
    WORD    dwSegment;
	long    dwPacketNumber;
}DataHeader;
/*音频和视频网络发送数据头*/
typedef struct __HDB_FRAME_HEAD {
	DWORD ID;								//=mmioFOURCC('4','D','S','P');
	DWORD nTimeTick;    					//时间戳 
	DWORD nFrameLength; 					//帧长度
	DWORD nDataCodec;   					//编码类型
	//编码方式
	//视频 :mmioFOURCC('H','2','6','4');
	//音频 :mmioFOURCC('A','D','T','S');
	DWORD nFrameRate;   					//视频  :帧率
	//音频 :采样率 (default:44100)
	DWORD nWidth;       					//视频  :宽
	//音频 :通道数 (default:2)
	DWORD nHight;       					//视频  :高
	//音频 :采样位 (default:16)
	DWORD nColors;      					//视频  :颜色数  (default: 24)
	//音频 :音频码率 (default:64000)
	DWORD dwSegment;						//分包标志位
	DWORD dwFlags;							//视频:  I 帧标志
	//音频:  保留
	DWORD dwPacketNumber; 					//包序号
	DWORD nOthers;      					//保留
}FRAMEHEAD;
typedef struct __HDB_SIZETYPE
{
	int nflag; // =0 原始大小 =1 缩小到指定尺寸
	int width;
	int height;
}SizeType;
typedef struct low_bitrate
{
	int nWidth;		//宽
	int nHeight;		//高
	int nFrame;		//帧率
	int nBitrate;		//带宽
	int nTemp;		//保留
}low_bitrate;
/*低码率参数*/

/*边框调节*/
typedef struct __pic_revise__ {
	short hporch;     // 左：负值，右：正值
	short vporch;     // 上：负值，下：正值
	int temp[4];
} pic_revise ;
typedef struct LogoInfo
{
	int x;	//显示位置坐标,左上角为0,0
	int y;	
	int width;	//logo图片宽高
	int height;
	int enable;	//是否显示logo	,显示:1,不显示:0
	char filename[16];//图片名称15个字符,默认为logo.png
	int alpha;//透明度[0,100]
	int isThrough;//是否透过某一像素值,实现镂空的效果,透过1,不透2
	int filter;//如果isThrough为1，可以设置一个要透的值，一般为0x00或0xFF
	int errcode;//错误码为0表示没有错误，1表示无效的logo文件
	int nTemp;	//保留	
}LogoInfo;
enum APP_OUTPUT_FORMAT
{
	APP_OUTPUT_640X480 =1, //01
	APP_OUTPUT_720X576, //02
	APP_OUTPUT_800X600,  //03
	APP_OUTPUT_1024X576, //04
	APP_OUTPUT_1024X768, //05
	APP_OUTPUT_1280X720, //06
	APP_OUTPUT_1280X1024, //07
	APP_OUTPUT_MAX
};
#define  MSG_ADDCLIENT      	1      //一个客户端连接成功
#define  MSG_DELETECLIENT   	2      //一个客户端断开
#define  MSG_CONNECTSUCC    	3      //连接成功
#define  MSG_PASSWORD_ERR   	4      //连接密码错
#define  MSG_MAXCLIENT_ERR  	5       //超过最大客户端
#define  MSG_AUDIODATA			6		//音频数据
#define  MSG_SCREENDATA     	7      //屏幕数据(编码后的数据)
#define  MSG_HEARTBEAT      	8      //心跳消息
#define  MSG_PASSWORD       	9      //发送密码
#define  MSG_DATAINFO       	10     //数据信息(后根RecDataInfo)结构
#define  MSG_REQ_I          	11     //请求I帧
#define  MSG_SET_FRAMERATE  	12     //设置帧率
#define  MSG_PPT_INDEX  		15

#define MSG_SYSPARAMS			16		//获取系统参数
#define MSG_SETPARAMS			17		//设置参数
#define MSG_RESTARTSYS			18		//重启

#define MSG_UpdataFile			19		//上传升级文件
#define MSG_SAVEPARAMS			20		//保存参数
#define MSG_UpdataFile_FAILS		21		//升级失败
#define MSG_UpdataFile_SUCC			22		//升级成功

#define MSG_DECODE_STATUS			23		//解码器连接状态(后跟1BYTE) 0:没有连接 1:连接成功 2:正在连接中
#define MSG_DECODE_DISCONNECT		24		//断开解码器与编码器的连接
#define MSG_DECODE_CONNECT			25		//断开解码器与编码器的连接

#define MSG_UPDATEFILE_DECODE 		26
#define MSG_UPDATEFILE_ROOT 		27
#define MSG_UPDATEFILE_WEB 			28

#define MSG_MODE_CODE				29		//切换成编码盒工作
#define MSG_MODE_DECODE				30		//切换成解码盒工作

#define MSG_ADD_TEXT				33		//加入字幕

#define MSG_MOUT          			40  	//设置或获取 为采集输出1路或多路流
#define MSG_SENDFLAG    			41  	//设置多路流时发送标志：Audio or Video or VGA
#define MSG_FARCTRL      			42  	//远遥
#define MSG_SETVGAADJUST            43 //边框调整,消息头+pic_revise
#define MSG_QUALITYVALUE			46 //设置高清编码器码流模式(0:低 1:中 2:高) 
#define MSG_TRACKAUTO				48 //设置自动跟踪模块的自动手动模式
#define MSG_UPLOADIMG 			    49//上传logo图片
#define MSG_GET_LOGOINFO 			50//获取logo图片信息
#define MSG_SET_LOGOINFO 			51//设置logo图片信息
#define MSG_GET_VIDEOPARAM			0x70 	//获取视频参数
#define MSG_SET_VIDEOPARAM			0x71 	//设置视频参数
#define MSG_GET_AUDIOPARAM			0x72 	//获取音频参数
#define MSG_SET_AUDIOPARAM			0x73 	//设置音频参数
#define MSG_SENDAUDIO				0x74 	//请求声音 消息头后跟一个BYTE 1：请求发送，0：请求不发送
#define MSG_CHG_UDPTCP				0x75		// UDP 和 TCP 切换
#define MSG_SET_SYSTIME				0x77	//设置系统时间
#define MSG_SET_DEFPARAM			0x79	//保存出厂设置

#define MSG_SET_PICPARAM			0x90	//设置图像的色度、亮度、饱和度
#define MSG_GET_PICPARAM			0x91	//获得图像的色度、亮度、饱和度

#define MSG_PIC_DATA				0x94
#define MSG_LOW_SCREENDATA   	    0x95//低码率数据
#define MSG_LOW_BITRATE 			0x96 //低码率
#define MSG_MUTE        			0x97
#define MSG_LOCK_SCREEN   			0x99//锁定屏幕
#define MSG_GSCREEN_CHECK 			0x9a//绿屏校正
#define MSG_TREC_EXCHANGE		0x9f//ppt文字索引
#define AVIIF_KEYFRAME				0x00000010
#define MSG_FIX_RESOLUTION 			0x9e//导播锁定分辨率
#define MSG_PIC_DATAEX				0xA0//ppt文字索引

#define H264                    96

typedef struct 
{
    /**//* byte 0 */
    unsigned char csrc_len:4;        /**//* expect 0 */
    unsigned char extension:1;        /**//* expect 1, see RTP_OP below */
    unsigned char padding:1;        /**//* expect 0 */
    unsigned char version:2;        /**//* expect 2 */
    /**//* byte 1 */
    unsigned char payload:7;        /**//* RTP_PAYLOAD_RTSP */
    unsigned char marker:1;        /**//* expect 1 */
    /**//* bytes 2, 3 */
    unsigned short seq_no;            
    /**//* bytes 4-7 */
    unsigned  long timestamp;        
    /**//* bytes 8-11 */
    unsigned long ssrc;            /**//* stream number is used here. */
} RTP_FIXED_HEADER;


typedef struct {
    //byte 0
	unsigned char TYPE:5;
    unsigned char NRI:2;
	unsigned char F:1;    
         
} NALU_HEADER; /**//* 1 BYTES */

typedef struct {
    //byte 0
    unsigned char TYPE:5;
	unsigned char NRI:2; 
	unsigned char F:1;    
            
             
} FU_INDICATOR; /**//* 1 BYTES */

typedef struct {
    //byte 0
    unsigned char TYPE:5;
	unsigned char R:1;
	unsigned char E:1;
	unsigned char S:1;    
} FU_HEADER; /**//* 1 BYTES */


typedef struct Time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
}cTime;

#endif