#ifndef HANDLEDATA_H
#define HANDLEDATA_H
#include<QDebug>
#include<QObject>
#include<QFile>
#include<QMutex>
class handledata:public QObject
{
    Q_OBJECT
public:
    handledata();
    ~handledata();

public slots:
    void work(QByteArray data);
    bool getCanSend();

private:
    QByteArray m_buffer;
    QFile* outputfile;
    unsigned char * m_data;
    bool m_canSend;
    mutable QMutex m_mutex;

};

#endif // HANDLEDATA_H
