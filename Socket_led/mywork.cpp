#include "mywork.h"
#include"protocol.h"
#include<QMutex>
#include<QThread>
MyWork::MyWork()
{
    m_file= new QFile("/sys/class/leds/firefly:blue:power/brightness");
    m_file->open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Unbuffered);
    temp.clear();




}
MyWork::~MyWork()
{
    delete m_socket;
    Handle_Thread->quit();
    Handle_Thread->wait();
    delete Handle;
    qDebug() << "MyWork destroyed " << "with Handle" << endl;
    emit m_destroyed();
}
void MyWork::work()
{
    i++;
    number = i;
    qDebug() << "child"<< i << " thread id :" << QThread::currentThreadId() << endl;
    m_socket = m_server->nextPendingConnection();
    QObject::connect(m_socket,&QTcpSocket::readyRead,
                     this,&MyWork::read,Qt::DirectConnection);
    connect(m_socket,&QTcpSocket::disconnected,this,&QObject::deleteLater);
    count = 0;
    Handle = new handledata ();
    Handle_Thread = new QThread();
    Handle->moveToThread(Handle_Thread);
    Handle_Thread->start();
    QObject::connect(this,&MyWork::getdata,Handle,&handledata::work);
}

void MyWork::read()
{


   m_buffer.append( m_socket->readAll());

//   while(temp.size()>10000){
//      emit getdata(temp.left(10000));
//       temp.remove(0,10000);
//   }

   char * dataPtr = m_buffer.data();
   Packet *packPtr = (Packet *)dataPtr;

   qDebug() << "temp len" << m_buffer.length();
   qDebug() << packPtr->sync;
   qDebug() << " packet len "<< packPtr->length;

   if(m_buffer.length() > 5)
   {

           if(packPtr->sync == SYNC)
           {
   //以下可以打印看看
   //
   //            qDebug() << sizeof(struct packet);
   //            qDebug() << QString("%1").arg(packPtr->length, 0, 16);
   //              qDebug() << sizeof(packPtr->data);

               if(m_buffer.length() >= packPtr->length )
               {
                   qint64 dataLen = packPtr->length - 5; //数据长度
                   qDebug() << dataLen;
                   char * data = (char *)malloc(dataLen);
                   memset(data, 0 , dataLen);

                   memcpy(data, dataPtr + 5, dataLen); //取出的数据
                   //这里就是视频数据，可以加更多的判断条件去组视频帧
                   my_num++;

                   QFile file("/kinston/"+QString("child thread ")+QString::number(i)+"-"+QString::number(my_num)+".yuv");
                   file.open(QIODevice::ReadWrite);
                   file.write(data);

                    free(data);
                    data = nullptr;
                   //outputfile->write(data);


//                     for(int pos = 0; pos < dataLen; pos++)
//                     {
//                         //可以发出信号,将组好的视频帧去显示
//     //                   qDebug() << data[pos];
//                        //qDebug() << QString("%1").arg(data[pos], 0, 16);

//                     }
                   //需要删除已取出的数据的包
                   m_buffer.remove(0, packPtr->length);

                   qDebug() << "after remove buffer size:" << m_buffer.size() <<endl;
               }



           }


  }












  /***************temp
   if(Handle->getCanSend()){

        emit getdata(temp);
        temp.clear();
   }

**********************/

   // m_socket->read(data,1024);
    //m_socket->write(received_data);


//    if(QString(received_data) == "close")
//    {
//        m_file->write("0");
//    }
//    else if(QString(received_data) == "open")
//    {
//        m_file->write("1");
//    }

}
