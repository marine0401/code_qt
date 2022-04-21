#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include<QTcpSocket>
#include<QString>
#include<QThread>
#include"readfromcamera.h"
#include"send.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_clicked();

    void on_send_clicked();

    void on_readFromCamera_clicked();
    void on_init_clicked();

signals:
    void startRead();
    void send_init(QString ip,qint16 port);


private:
    Ui::MainWindow *ui;
    QTcpSocket *m_socket;
    quint16 port;
    QString ip;
    QThread *socket_send_Thread;
    QThread *camera_read_Thread;
    Send *send_work;
    ReadFromCamera *m_read_work;

};
#endif // MAINWINDOW_H
