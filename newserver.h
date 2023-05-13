#ifndef NEWSERVER_H
#define NEWSERVER_H


#include<QTcpServer>//监听套接字
#include<QTcpSocket>
#include<QThread>
#include<QMutex>
#include<iostream>
#include"db.h"
using namespace std;


class NewServer : public QThread
{
    Q_OBJECT
public:
    NewServer(qintptr);
    void run() override;//伪线程启动
    QString db_name = "";

public slots:
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

    DB* db;
    //信息发送区(单体)
    /***************/
    void send(QByteArray);//单体发送信息
    void send(int*);//单体发送信息

};

#endif // NEWSERVER_H
