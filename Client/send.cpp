#include "send.h"

Send::Send()
{

}
Send::~Send()
{

}
void Send::init(QString ip,qint16 port)
{
    m_socket = new QTcpSocket();
    m_socket->connectToHost(ip,port);
}

void Send::work(unsigned char * data,qint64 size)
{
    qDebug() << "send "<<*data << endl;
    m_socket->write(reinterpret_cast<const char*>(data),size);
    m_socket->waitForBytesWritten();

}


