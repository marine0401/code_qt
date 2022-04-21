#include <QCoreApplication>
#include<QFile>
#include<QTcpServer>
#include<QTcpSocket>
#include<QObject>
#include<QByteArray>
#include<QString>
#include<QThread>
#include"mywork.h"
#include"public_varation.h"
#include <QMetaType>
Q_DECLARE_METATYPE(QByteArray);
int main(int argc, char *argv[])
{     
    QCoreApplication a(argc, argv);
    qRegisterMetaType<QByteArray>("QByteArray&");
    m_server = new QTcpServer();
    m_server->listen(QHostAddress::Any,8080);

    qDebug() << "main thread id:"<<QThread::currentThreadId() << endl;

    QObject::connect(m_server,&QTcpServer::newConnection,[=](){
     MyWork *m_work= new MyWork();
     QThread *temp = new QThread();
    m_work->moveToThread(temp);
    QObject::connect(temp,&QThread::started,m_work,&MyWork::work);

/*abort*/
//    handledata *Handle = new handledata ();
//    QThread *Handle_Thread = new QThread();
//    Handle->moveToThread(Handle_Thread);
//    Handle_Thread->start();
//    QObject::connect(m_work,&MyWork::getdata,Handle,&handledata::work);


    QObject::connect(m_work,&MyWork::m_destroyed,[=](){
        temp->quit();
        temp->wait();
        qDebug() << "QThread Id " <<QThread::currentThreadId() <<"destroyed " << endl;
    });


    temp->start();
    qDebug() << m_work << endl;

    });



    return a.exec();
}



