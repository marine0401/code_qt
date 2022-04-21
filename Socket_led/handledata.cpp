#include "handledata.h"
#include "protocol.h"
#include<QThread>
#include"public_varation.h"

handledata::handledata()
{
    outputfile = new QFile("/kinston/output.yuv");
    outputfile->open(QIODevice::ReadWrite|QIODevice::Append);
    m_data = (unsigned char *)malloc(15);
    m_canSend = true;
}
handledata::~handledata()
{
    free(m_data);
}

void handledata::work(QByteArray data)
{

    m_canSend = false;
    qDebug() << "child" << " thread id :" << QThread::currentThreadId() << endl;
    m_buffer.append(data);

    /*
    char * dataPtr = m_buffer.data();
    Packet *packPtr = (Packet *)dataPtr;

   qDebug() << "buffer size :" << m_buffer.size() <<endl;
   qDebug() << "buffer length :" << m_buffer.length() << endl;
   qDebug() << "packet sync "<<packPtr->sync << endl;
   qDebug() << "packet length"<<packPtr->length << endl;
   qDebug() << "size of packet data :" << sizeof(packPtr->data) << endl;
*/
   outputfile->write(data);
   m_buffer.clear();
   m_canSend = true;
/*****************
    while(m_buffer.length() > 2)
    {

            if(packPtr->sync == SYNC)
            {
    //以下可以打印看看
    //            qDebug() << packPtr->sync;
    //            qDebug() << packPtr->length;
    //            qDebug() << sizeof(struct packet);
    //            qDebug() << QString("%1").arg(packPtr->length, 0, 16);
    //              qDebug() << sizeof(packPtr->data);

                if(m_buffer.length() >= packPtr->length )
                {
                    qint32 dataLen = packPtr->length - 2; //数据长度
                    qDebug() << dataLen;

                    memset(m_data, 0 , 15);
                    memcpy(m_data, dataPtr + 2, dataLen); //取出的数据
                    //这里就是视频数据，可以加更多的判断条件去组视频帧

                    qDebug() << "data";
                    //outputfile->write(data);


//                     for(int pos = 0; pos < dataLen; pos++)
//                     {
//                         //可以发出信号,将组好的视频帧去显示
//     //                   qDebug() << data[pos];
//                        //qDebug() << QString("%1").arg(data[pos], 0, 16);

//                     }
                    //需要删除已取出的数据的包
                    m_buffer.remove(0, packPtr->length);
                    continue;
                    qDebug() << "after remove buffer size:" << m_buffer.size() <<endl;
                }



            }


   }
   ***********************************/


}
bool handledata::getCanSend()
{
    QMutexLocker locker(&m_mutex);
    return m_canSend;
}

