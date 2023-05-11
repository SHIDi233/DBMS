#ifndef NEWSERVER_H
#define NEWSERVER_H


#include<QTcpServer>//监听套接字
#include<QTcpSocket>//通信套接字
#include<QUdpSocket>//音视频通话用udp
#include<QProcess>
#include<QThread>
#include<QMutex>
#include<iostream>
using namespace std;


class NewServer : public QThread
{
    Q_OBJECT
public:
    NewServer(qintptr);
    void run() override;//伪线程启动
public slots:
    void set(qintptr handle);
signals:
    void st(qintptr handle);
private:
    QMutex m_mutex;//锁

    qintptr handle;

    //账户所需所有socket
    /***************/
    QTcpSocket *socket_ser;//长连接TCP
    bool isOnline;//是否在线
    bool isSet = false;

    int number;//账号


    //信息发送区(单体)
    /***************/
    void send(QByteArray);//单体发送信息
    void send(int*);//单体发送信息
    void sendImg(QByteArray);//发送图片信息
    void f();//推送流

    void mode(int);
    bool isTrue(QString,QString);//检测登录是否成功

    void send_word();//发送文本
    void send_img();//发送图片

    void startVideo();
    void startAudio();
};

#endif // NEWSERVER_H
