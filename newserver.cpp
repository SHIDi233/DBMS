#include "newserver.h"

NewServer::NewServer(qintptr handle)
{
    this->handle = handle;
}

void NewServer::send(QByteArray sendMsg){
    socket_ser->write(sendMsg);//发送二进制消息
}

void NewServer::send(int* sendMsg){
    socket_ser->write((char*)sendMsg,sizeof(int));//发送二进制消息
}



void NewServer::run(){
    socket_ser = new QTcpSocket();
    socket_ser->setSocketDescriptor(handle);
    QString ip=socket_ser->peerAddress().toString();
    qint16 port  =socket_ser->peerPort();
    QString temp = QString("端口是%1，ip地址是%2").arg(port).arg(ip);
    isSet=true;
    isOnline=true;
    while(isOnline){
        if(socket_ser->waitForReadyRead()){
            qDebug()<<"收到信息";
        int len = -1;
        socket_ser->read((char*)&len,sizeof(int));//读取长度
        qDebug()<<len;
        char rec[len+1];
        socket_ser->read(rec,len);//读取字符串
        rec[len]='\0';
        qDebug()<<rec;

        }
    }
}


