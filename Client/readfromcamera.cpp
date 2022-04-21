#include "readfromcamera.h"

ReadFromCamera::ReadFromCamera()
{

}
ReadFromCamera::~ReadFromCamera(){


}
int ReadFromCamera::init()
{



    av_register_all();
    avcodec_register_all();
    avdevice_register_all();
    pFormatCtx = avformat_alloc_context();


    AVInputFormat * ifmt = av_find_input_format("dshow");

    avformat_open_input(&pFormatCtx,"video=Integrated Camera",ifmt,NULL);

    if(avformat_find_stream_info(pFormatCtx,NULL)<0)
    {

        printf("Couldn't find stream information. ");
        return -1;
    }


    videoindex=-1;

      for(i=0; i<pFormatCtx->nb_streams; i++)
      {
            if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
            {

                videoindex=i;
            }
      }

       if(videoindex==-1)
       {
            printf("Couldn't find a video stream. ");
            return -1;
       }

        pCodec = NULL;
        pCodecCtx = NULL;
        pCodecCtx=pFormatCtx->streams[videoindex]->codec;
        pCodec=avcodec_find_decoder(pCodecCtx->codec_id);

        if(pCodec==NULL)
        {
            printf("Codec not found.");
            return -1;
        }

        if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
        {
            printf("Could not open codec.");
            return -1;
        }

        packet = (AVPacket*)malloc(sizeof (AVPacket));

        ret = -1;
        got_picture = 0;




        pFrame=av_frame_alloc();

        pFrameYUV=av_frame_alloc();
        uint8_t *out_buffer=(uint8_t *)av_malloc(avpicture_get_size(pCodecCtx->pix_fmt,
                                                                        pCodecCtx->width,
                                                                        pCodecCtx->height));
        avpicture_fill((AVPicture *)pFrameYUV, out_buffer,
                           pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);


       img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                             pCodecCtx->pix_fmt, pCodecCtx->width,
                                             pCodecCtx->height, AV_PIX_FMT_YUV420P,
                                             SWS_BICUBIC, NULL, NULL, NULL);




}

void ReadFromCamera::read()
{
   unsigned char *head = (unsigned char *)malloc(5);
   head[0] = 0x57;

    FILE *fp_yuv=fopen("output.yuv","rwb+");
    for(int i=0;i<1000;i++)
    {

        if(av_read_frame(pFormatCtx, packet) < 0)
        {
            qDebug() << "quit" << endl;
            break;
        }

        if(packet->stream_index==videoindex)
        {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            qDebug() << i <<endl;

            if(got_picture)
            {

                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data,
                          pFrameYUV->linesize);

                int y_size=pCodecCtx->width*pCodecCtx->height;

               qDebug() <<  y_size << endl;

              // qDebug() << "buf->data :" << pFrameYUV->buf[0]->data;
                if(i ==0){
                   head[4] = ( (3*y_size/2+5) >> 24 ) &0xff;
                   head[3] = ((3*y_size/2+5) >> 16) & 0xff;
                   head[2] = ((3*y_size/2+5) >> 8) &0xff;
                   head[1] = ((3*y_size/2+5) & 0xff);
                }
                emit getdata(head,5);
                emit getdata(pFrameYUV->data[0],y_size);
                emit getdata(pFrameYUV->data[1],y_size/4);
                emit getdata(pFrameYUV->data[2],y_size/4);
               //fwrite(pFrameYUV->data[0],1,y_size,fp_yuv); //Y
               //fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
               //fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
/******************
                        for(int x = 0; x < 3; x++)
                        {
                            if(x == 0)
                            {
                                for(int j = 0; j < 92160;j++)
                                {
                               // qDebug() << pFrameYUV->data[0][j];
                                 memcpy(data+2,pFrameYUV->data[x]+j*10,sizeof(unsigned char)*10);
                                data[13] = '\0';
                                 emit getdata(data);



                                }
                            }else
                            {
                                for(int j = 0; j < 23040;j++)
                                {
                               // qDebug() << pFrameYUV->data[0][j];

                                    memcpy(data+2,pFrameYUV->data[x]+j*10,sizeof(unsigned char)*10);
                                     data[13] = '\0';
                                     emit getdata(data);
                                }
                            }


                         }
                         *******************/


                }
            }

    }

    //av_frame_free(&pFrameYUV);
    //av_frame_free(&pFrame);
    //avcodec_close(pCodecCtx);
    //avformat_close_input(&pFormatCtx);

//


}




//    char buf[1024];
//    int ret;
//    while(1)
//    {
//        if(ret = fread(buf,1,1024,fp_yuv))
//        {
//            m_socket->write(buf);
//            m_socket->waitForBytesWritten(20);
//        }
//        if(ret == 0)
//        {
//            break;
//        }
//    }




void ReadFromCamera::genprotocol(QByteArray &out,unsigned char *data)
{
   // qDebug() << " first "<< 0x57 << out.size() <<endl;
    out.clear();

    out.append((unsigned char)0x57);

    out.append((unsigned char)0x23 );

    out.append( (unsigned char) 0x03);

    qDebug() << "middle "<< out.size() <<endl;
    out.append((char*)data);
    qDebug() << "last "<< out.size() <<endl;

}
