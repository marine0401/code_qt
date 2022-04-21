#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket_send_Thread = new QThread();
    send_work = new Send();
    send_work->moveToThread(socket_send_Thread);
    socket_send_Thread->start();
    connect(send_work,&Send::destroyed,[=](){
        socket_send_Thread->quit();
        qDebug() << "socket_send_Thread quit" << endl;
    });


    camera_read_Thread = new QThread();
    m_read_work = new ReadFromCamera();
    m_read_work->moveToThread(camera_read_Thread);

    connect(m_read_work,&ReadFromCamera::destroyed,[=](){
        camera_read_Thread->quit();
        qDebug() << "camera_read_Thread quit" << endl;
    });


    connect(camera_read_Thread,&QThread::started,m_read_work,&ReadFromCamera::init);

    connect(this,&MainWindow::startRead,m_read_work,&ReadFromCamera::read);

    connect(m_read_work,&ReadFromCamera::getdata,send_work,&Send::work);

    connect(this,&MainWindow::send_init,send_work,&Send::init);



}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_read_work;
    delete send_work;
}


void MainWindow::on_connect_clicked()
{

    emit send_init(ui->ip->text(),ui->port->text().toInt());

}

void MainWindow::on_send_clicked()
{

}

void MainWindow::on_readFromCamera_clicked()
{
    emit startRead();
}

void MainWindow::on_init_clicked()
{
    camera_read_Thread->start();
}
