#pragma once
#include "SDL.h"
extern "C"
{
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
	//#include "libavcodec/audioconvert.h"
#include "libavcodec/avcodec.h"
	//#include "libavutil/audioconvert.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
	//#include "libavutil/colorspace.h"
#include "libavutil/fifo.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/dict.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/avstring.h"
#include "libavutil/rational.h"
#include "libswresample/swresample.h" 	
}
typedef struct MyAVPacketList {
	AVPacket pkt;
	struct MyAVPacketList *next;
	int serial;
} MyAVPacketList;

typedef struct PacketQueue {
	MyAVPacketList *first_pkt, *last_pkt;
	int nb_packets;
	int size;
	int abort_request;
	int serial;
	SDL_mutex *mutex;
	SDL_cond *cond;
} PacketQueue;
extern PacketQueue qa;
extern PacketQueue qv;
extern int packet_queue_put_private(PacketQueue *q, AVPacket *pkt);
extern int packet_queue_put(PacketQueue *q, AVPacket *pkt);
extern int packet_queue_put_nullpacket(PacketQueue *q, int stream_index);
extern void packet_queue_init(PacketQueue *q);
extern void packet_queue_flush(PacketQueue *q);
extern void packet_queue_destroy(PacketQueue *q);
extern void packet_queue_abort(PacketQueue *q);
extern void packet_queue_start(PacketQueue *q);
extern int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, int *serial);












