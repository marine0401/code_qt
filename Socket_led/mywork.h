#ifndef MYWORK_H
#define MYWORK_H
#include<QObject>
#include<QThread>
#include<QTcpServer>
#include<QTcpSocket>
#include<QString>
#include"handledata.h"
#include"public_varation.h"
#include<QFile>
class MyWork :public QObject
{
    Q_OBJECT
public:
    MyWork();
    ~MyWork();


signals:
    void m_destroyed();

public slots:
    void work();
    void read();
private:
    QTcpSocket *m_socket;
    int number;
    QFile *m_file ;
    QThread *Handle_Thread;
    handledata *Handle;
    QByteArray m_buffer;
    QByteArray temp;
    int count;

    qint16 my_num=0;
signals:
    void getdata(QByteArray data);
};

#endif // MYWORK_H
