#ifndef READFROMCAMERA_H
#define READFROMCAMERA_H
#include<QDebug>
#include<QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include<QByteArray>
#include<QThread>
extern "C"
{
#include "libavcodec/avcodec.h"  //编码器
#include "libavformat/avformat.h"
#include"libavutil/avutil.h"
#include"libswresample/swresample.h"

#include"libavutil/pixfmt.h"

#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include"libavutil/samplefmt.h"

}

#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avcodec.lib")
#pragma commemt(lib,"avutil.lib")
#pragma comment(lib,"swresample.lib")

class ReadFromCamera : public QObject
{
    Q_OBJECT
public:
    ReadFromCamera();
    ~ReadFromCamera();
    int init();
    void read();
signals:
    void getdata(unsigned char * data ,qint64 size);
private:
    AVFormatContext	*pFormatCtx;
    int i, videoindex;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
    AVFrame	*pFrame,*pFrameYUV;
    unsigned char *out_buffer;
    AVPacket *packet;
    int ret, got_picture;
    struct SwsContext *img_convert_ctx;

    unsigned char* data;

    void genprotocol(QByteArray &out,unsigned char *data);
};

#endif // READFROMCAMERA_H
