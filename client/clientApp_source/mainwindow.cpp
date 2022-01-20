#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>
#include<QHostAddress>
#include <QKeyEvent>
#include<QEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ui->port->setText("7777");
//    ui->port->setReadOnly(true);
//    ui->ip->setText("192.168.3.1");
//    ui->ip->setReadOnly(true);
    setWindowTitle("无内鬼对讲机");
    isconnect=0;
    is_set_name=0;
    ui->disconnect->setDisabled(true);


    //通信套接字
    m_tcp = new QTcpSocket;


    //从服务端读取数据
    connect(m_tcp,&QTcpSocket::readyRead,this,[=](){
        QByteArray data = m_tcp->readAll();
        ui->record->append(data);

    });

    //tcp连接断开时
    connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
        isconnect=0;
        m_tcp->close();
        m_tcp->deleteLater();
        ui->record->append("服务器已经和客户端断开了连接");
        ui->connect->setDisabled(false);
        ui->disconnect->setEnabled(false);
        m_status->setText("未连接");
    });


    //tcp连接连接时
    connect(m_tcp,&QTcpSocket::connected,this,[=](){
        isconnect=1;
       m_status->setText("已连接");
       ui->record->append("已经连接到了服务器");
       ui->connect->setDisabled(true);
       ui->disconnect->setDisabled(false);
       m_tcp->write(name.toUtf8());   //返回客户端名字
    });


    //状态栏
    m_status = new QLabel;
    ui->statusbar->addWidget(new QLabel("连接状态:"));
    ui->statusbar->addWidget(m_status);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sendMsg_clicked()
{
     ui->msg->setFocus();

   if(setname()){
    QString msg = ui->msg->toPlainText();//以纯文本的方式读取
    m_tcp->write(msg.toUtf8());
    ui->msg->clear();
   }

   if(!isconnect){
       QMessageBox::warning(this,"警告","请连接服务器");
   }

}

void MainWindow::on_connect_clicked()
{
    if(is_set_name!=0){
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
//    m_tcp->connectToHost(QHostAddress(ip),port);    //ip方式
//     m_tcp->connectToHost("xxxxxx.com",port); //域名方式
     m_tcp->connectToHost(ui->ip->text().toUtf8(),port);
    }else{

        QMessageBox::warning(this,"警告","请先设置昵称");
    }
}

void MainWindow::on_disconnect_clicked()
{
    m_tcp->close();
    m_tcp->deleteLater();
    ui->connect->setDisabled(false);
    ui->disconnect->setEnabled(false);


}

int MainWindow::setname()
{
    if(name==NULL){
        QMessageBox::warning(this,"警告","请设置昵称");
        return 0;

    }
    else{

        name = ui->name->text();
        qDebug()<<name;
        return 1;
    }
}

void MainWindow::on_nameok_clicked()
{
    name=ui->name->text();
    if(name!=NULL){
    ui->record->append("设置"+name+"昵称成功");
    is_set_name=1;
    }else {
    QMessageBox::warning(this,"警告","不能设置空昵称");
}
}


