#if _MSC_VER >=1600    // MSVC2015>1899,对于MSVC2010以上版本都可以使用
#pragma execution_character_set("utf-8")
#endif
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTcpSocket>
#include<QLabel>
#include<QMessageBox>

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

    void on_sendMsg_clicked();

    void on_connect_clicked();

    void on_disconnect_clicked();

    int setname(); //设置成功返回1

    void on_nameok_clicked();


private:
    Ui::MainWindow *ui;

    QTcpSocket * m_tcp;

    QLabel *m_status;  //状态栏

    QString name;

    int is_set_name; //昵称是否设置成功

    int isconnect;//是否连接到服务器标志位

};
#endif // MAINWINDOW_H
