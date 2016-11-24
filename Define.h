#ifndef __DEFINE_H__
#define __DEFINE_H__


#define RM_CONNECT_BEGIN	WM_USER + 4020
#define UM_FIXFINSH			WM_USER + 1020
#define UM_FIXPROGESS		UM_FIXFINSH + 1
#define RM_CONNECT_ERROR	RM_CONNECT_BEGIN + 1
#define RM_CONNECT_SUCCESS	RM_CONNECT_BEGIN + 2

#define RM_ST_FAIL		RM_CONNECT_BEGIN + 3		//����ʧ��
#define RM_ST_SUCCESS	RM_CONNECT_BEGIN + 4		//���ӳɹ�
#define RM_ST_CONNING	RM_CONNECT_BEGIN + 5		//��������

#define RM_ERR_PARAM		RM_CONNECT_BEGIN + 6		//��������
#define RM_DISCONN			RM_CONNECT_BEGIN + 7		//�Ͽ�
#define RM_ERR_UPDATE		RM_CONNECT_BEGIN + 8		//��������
#define RM_PASSWD_ERR		RM_CONNECT_BEGIN + 9		//�������
#define RM_UPDATE_STATUS	RM_CONNECT_BEGIN + 10		//

#define RM_REC_DATA			RM_CONNECT_BEGIN + 11		//DATA����
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
/* ��ñ�����ͼ�����*/
/*ϵͳ������*/
typedef struct __SYSPARAMS {
	unsigned char szMacAddr[8];				//MAC ��ַ
	unsigned int dwAddr;					//IP ��ַ
	unsigned int dwGateWay;					//����
	unsigned int dwNetMark;					//��������
	char strName[16];						//��Ʒ���к�
	char strVer[10];						//�������汾��
	WORD unChannel;							//ͨ����
	char bType; 							//���ֱ���������  0 -------VGABOX     3-------200 4-------110 5-------120 6--------1200  8---ENC-1100
	char bTemp[3]; 							//����
	int nTemp[7];							//����
} SYSPARAMS;
/*��Ƶ����*/
typedef struct __VIDEOPARAM
{
    DWORD nDataCodec;   					//���뷽ʽ
											//��Ƶ��mmioFOURCC('H','2','6','4');
	DWORD nFrameRate;   					//��Ƶ������֡��
	DWORD nWidth;       					//��Ƶ�����
	DWORD nHight;       					//��Ƶ���߶�
	DWORD nColors;      						//��Ƶ����ɫ��
	DWORD nQuality;							//��Ƶ������ϵ��
	WORD sCbr;								//��Ƶ��������/������
	WORD sBitrate;							//��Ƶ������
}VideoParam;

/*��Ƶ����*/
typedef struct __AUDIOPARAM
{
	DWORD Codec;  							//���뷽ʽ
    DWORD SampleRate; 						//������
	DWORD BitRate;  						//����
	DWORD Channel;  						//������
	DWORD SampleBit;  						//����λ��
	BYTE  LVolume;							//����������  0 --------31
	BYTE  RVolume;							//����������  0---------31
	WORD  InputMode;                        // 1------ �������  0------��������	
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
	BYTE bVmode;		//��Ƶģʽ    0 ------- D1  1 ------CIF   2------4CIF 
	BYTE bType;            // 0 -------VGABOX     0f-------AVBOX
	char sTemp[26];	
//	int nTemp[7];
	////// ver 1.0
}SysParamsV2;
/*����Ļ�ṹ����*/
typedef struct __RecAVTitle
{
    int  x;                 				//x pos
	int  y;        				//y pos
	int len;   //Textʵ�ʳ���
	char Text[65];          				//text
}RecAVTitle;

/*��ӱ���*/
typedef struct  __RTitle__ {	
	unsigned int uflag;	
	RecAVTitle title;
}RTitle;

/*ϵͳ���������*/
typedef struct __SAVEPARAM {
#if 0
	SYSPARAMS sysPara;                 		//��׼ϵͳ����
#else
	SysParamsV2 sysPara;
#endif

	VideoParam videoPara[DSP_NUM];				 	//��Ƶ����
	AudioParam audioPara[DSP_NUM];					//��Ƶ����
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
/*��Ϣͷ��ÿ����Ϣ����ʱ�������˽ṹ*/
typedef struct __HDB_MSGHEAD							
{
	/*   
	## ���ȣ�ͨ��htonsת��
	## ��Ϣ���ȣ������ṹͷ
	## �ͺ������������
	*/
	WORD	nLen;		
	WORD	nVer;							//�汾
	BYTE	nMsg;							//��Ϣ��
	BYTE	szTemp[3];						//����
}MSGHEAD;

typedef struct _DataHeader
{
    DWORD   id;
	long    biWidth;							//VGA��Width		Audio��SampleRate
	long    biHeight;							//VGA��Height	Audio��BitRate
	WORD    biBitCount;							//VGA��BitCount	Audio��Channel
	DWORD   fccCompressedHandler;				//VGA��mmioFOURCC (��H��,��2��,��6��,��4��)
											    //Audio��mmioFOURCC(��A��,��D��,��T��,��S��)
	DWORD   fccHandler;	   						//Audio��SampleBit
    DWORD   dwCompressedDataLength;
    DWORD   dwFlags;
    WORD    dwSegment;
	long    dwPacketNumber;
}DataHeader;
/*��Ƶ����Ƶ���緢������ͷ*/
typedef struct __HDB_FRAME_HEAD {
	DWORD ID;								//=mmioFOURCC('4','D','S','P');
	DWORD nTimeTick;    					//ʱ��� 
	DWORD nFrameLength; 					//֡����
	DWORD nDataCodec;   					//��������
	//���뷽ʽ
	//��Ƶ :mmioFOURCC('H','2','6','4');
	//��Ƶ :mmioFOURCC('A','D','T','S');
	DWORD nFrameRate;   					//��Ƶ  :֡��
	//��Ƶ :������ (default:44100)
	DWORD nWidth;       					//��Ƶ  :��
	//��Ƶ :ͨ���� (default:2)
	DWORD nHight;       					//��Ƶ  :��
	//��Ƶ :����λ (default:16)
	DWORD nColors;      					//��Ƶ  :��ɫ��  (default: 24)
	//��Ƶ :��Ƶ���� (default:64000)
	DWORD dwSegment;						//�ְ���־λ
	DWORD dwFlags;							//��Ƶ:  I ֡��־
	//��Ƶ:  ����
	DWORD dwPacketNumber; 					//�����
	DWORD nOthers;      					//����
}FRAMEHEAD;
typedef struct __HDB_SIZETYPE
{
	int nflag; // =0 ԭʼ��С =1 ��С��ָ���ߴ�
	int width;
	int height;
}SizeType;
typedef struct low_bitrate
{
	int nWidth;		//��
	int nHeight;		//��
	int nFrame;		//֡��
	int nBitrate;		//����
	int nTemp;		//����
}low_bitrate;
/*�����ʲ���*/

/*�߿����*/
typedef struct __pic_revise__ {
	short hporch;     // �󣺸�ֵ���ң���ֵ
	short vporch;     // �ϣ���ֵ���£���ֵ
	int temp[4];
} pic_revise ;
typedef struct LogoInfo
{
	int x;	//��ʾλ������,���Ͻ�Ϊ0,0
	int y;	
	int width;	//logoͼƬ���
	int height;
	int enable;	//�Ƿ���ʾlogo	,��ʾ:1,����ʾ:0
	char filename[16];//ͼƬ����15���ַ�,Ĭ��Ϊlogo.png
	int alpha;//͸����[0,100]
	int isThrough;//�Ƿ�͸��ĳһ����ֵ,ʵ���οյ�Ч��,͸��1,��͸2
	int filter;//���isThroughΪ1����������һ��Ҫ͸��ֵ��һ��Ϊ0x00��0xFF
	int errcode;//������Ϊ0��ʾû�д���1��ʾ��Ч��logo�ļ�
	int nTemp;	//����	
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
#define  MSG_ADDCLIENT      	1      //һ���ͻ������ӳɹ�
#define  MSG_DELETECLIENT   	2      //һ���ͻ��˶Ͽ�
#define  MSG_CONNECTSUCC    	3      //���ӳɹ�
#define  MSG_PASSWORD_ERR   	4      //���������
#define  MSG_MAXCLIENT_ERR  	5       //�������ͻ���
#define  MSG_AUDIODATA			6		//��Ƶ����
#define  MSG_SCREENDATA     	7      //��Ļ����(����������)
#define  MSG_HEARTBEAT      	8      //������Ϣ
#define  MSG_PASSWORD       	9      //��������
#define  MSG_DATAINFO       	10     //������Ϣ(���RecDataInfo)�ṹ
#define  MSG_REQ_I          	11     //����I֡
#define  MSG_SET_FRAMERATE  	12     //����֡��
#define  MSG_PPT_INDEX  		15

#define MSG_SYSPARAMS			16		//��ȡϵͳ����
#define MSG_SETPARAMS			17		//���ò���
#define MSG_RESTARTSYS			18		//����

#define MSG_UpdataFile			19		//�ϴ������ļ�
#define MSG_SAVEPARAMS			20		//�������
#define MSG_UpdataFile_FAILS		21		//����ʧ��
#define MSG_UpdataFile_SUCC			22		//�����ɹ�

#define MSG_DECODE_STATUS			23		//����������״̬(���1BYTE) 0:û������ 1:���ӳɹ� 2:����������
#define MSG_DECODE_DISCONNECT		24		//�Ͽ��������������������
#define MSG_DECODE_CONNECT			25		//�Ͽ��������������������

#define MSG_UPDATEFILE_DECODE 		26
#define MSG_UPDATEFILE_ROOT 		27
#define MSG_UPDATEFILE_WEB 			28

#define MSG_MODE_CODE				29		//�л��ɱ���й���
#define MSG_MODE_DECODE				30		//�л��ɽ���й���

#define MSG_ADD_TEXT				33		//������Ļ

#define MSG_MOUT          			40  	//���û��ȡ Ϊ�ɼ����1·���·��
#define MSG_SENDFLAG    			41  	//���ö�·��ʱ���ͱ�־��Audio or Video or VGA
#define MSG_FARCTRL      			42  	//Զң
#define MSG_SETVGAADJUST            43 //�߿����,��Ϣͷ+pic_revise
#define MSG_QUALITYVALUE			46 //���ø������������ģʽ(0:�� 1:�� 2:��) 
#define MSG_TRACKAUTO				48 //�����Զ�����ģ����Զ��ֶ�ģʽ
#define MSG_UPLOADIMG 			    49//�ϴ�logoͼƬ
#define MSG_GET_LOGOINFO 			50//��ȡlogoͼƬ��Ϣ
#define MSG_SET_LOGOINFO 			51//����logoͼƬ��Ϣ
#define MSG_GET_VIDEOPARAM			0x70 	//��ȡ��Ƶ����
#define MSG_SET_VIDEOPARAM			0x71 	//������Ƶ����
#define MSG_GET_AUDIOPARAM			0x72 	//��ȡ��Ƶ����
#define MSG_SET_AUDIOPARAM			0x73 	//������Ƶ����
#define MSG_SENDAUDIO				0x74 	//�������� ��Ϣͷ���һ��BYTE 1�������ͣ�0�����󲻷���
#define MSG_CHG_UDPTCP				0x75		// UDP �� TCP �л�
#define MSG_SET_SYSTIME				0x77	//����ϵͳʱ��
#define MSG_SET_DEFPARAM			0x79	//�����������

#define MSG_SET_PICPARAM			0x90	//����ͼ���ɫ�ȡ����ȡ����Ͷ�
#define MSG_GET_PICPARAM			0x91	//���ͼ���ɫ�ȡ����ȡ����Ͷ�

#define MSG_PIC_DATA				0x94
#define MSG_LOW_SCREENDATA   	    0x95//����������
#define MSG_LOW_BITRATE 			0x96 //������
#define MSG_MUTE        			0x97
#define MSG_LOCK_SCREEN   			0x99//������Ļ
#define MSG_GSCREEN_CHECK 			0x9a//����У��
#define MSG_TREC_EXCHANGE		0x9f//ppt��������
#define AVIIF_KEYFRAME				0x00000010
#define MSG_FIX_RESOLUTION 			0x9e//���������ֱ���
#define MSG_PIC_DATAEX				0xA0//ppt��������

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