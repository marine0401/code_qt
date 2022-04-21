#ifndef SEND_H
#define SEND_H
#include<QObject>
#include<QTcpSocket>
class Send  : public QObject
{
    Q_OBJECT
public:
    Send();
    ~Send();
public slots:
    void work(unsigned char * data,qint64 size);
    void init(QString ip,qint16 port);
private:
    QTcpSocket *m_socket;

};



#endif // SEND_H
