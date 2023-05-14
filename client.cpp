#include "client.h"
#include<QTimer>
#include<QProcess>
#include<QDir>
#include"mainwindow.h"
#include<stdlib.h>
#include <QMetaType>
#include"ccnwindow.h"

CCNWindow* mw;

Client::Client(QString ip){
    qRegisterMetaType<QVector<QVector<QString>>>("QVector<QVector<QString>>");//注册diskInformation类型

    mw = new CCNWindow(this);
    mw->show();
    connect(this,SIGNAL(back(QVector<QVector<QString>>)),mw,SLOT(showTableAll(QVector<QVector<QString>>)));

    this->ip = ip;
}

void Client::run(){
    socket_client=new QTcpSocket();

        //有数据可读时，进行处理

    socket_client->connectToHost("192.168.10.187",9559);
    qDebug()<<"连接服务器中";

        while(1){
            if(socket_client->waitForConnected()){
                qDebug()<<"成功和服务器建立连接";
                //登录代码
//                int target = 0;//目标
//                socket_client->write((char*)&target,sizeof(int));//0
//               //socket_client->flush();
//                target = number;//目标
//                socket_client->write((char*)&target,sizeof(int));//发送自己账号
//                socket_client->write(pass.toUtf8().data());
//                socket_client->flush();
//                cout<<target;
                isOnline = true;
                //send("test");
//                //t->start();
                break;
            }





        }

        socket_client->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

        while(isOnline){
            if(socket_client->waitForReadyRead()){
            QVector<QVector<QString>> data;
            int vn = receive_int();
            for(int i=0;i<vn;i++){
                int sn = receive_int();
                QVector<QString> qss;

                for(int k=0;k<sn;k++){
                    int slen = receive_int();
                    qss.push_back(receive_QS(slen));
                }
                data.push_back(qss);
            }
            socket_client->readAll();
            emit back(data);
        }

    }

}



void Client::send(QString data){
    int len = data.length();
    socket_client->write((char*)&len,sizeof(int));
    socket_client->write(data.toUtf8().data());
    socket_client->flush();
    qDebug()<<data+"指令发出";
}





void Client::send_heart(){
    int temp =1;
    qDebug()<<"发送心跳";
    socket_client->write((char*)&temp,sizeof(int));//发送心跳
    socket_client->flush();
}

int Client::receive_int(){
    int temp;
    while(1){
        if(socket_client->waitForReadyRead()){
            socket_client->read((char*)&temp,sizeof(int));
            break;
        }
    }
    return temp;
}

QString Client::receive_QS(int len){
    if(len<=0 || len>=200)
        return QString("");
    char temp[len+1];
    while(1){
        if(socket_client->waitForReadyRead()){
            socket_client->read((char*)&temp,len);
            break;
        }
    }
    temp[len]='\0';
    QString s(temp);
    return s;
}
