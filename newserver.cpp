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

//    while(isOnline){
//        while(isSet){
//            m_mutex.lock();
//            cout<<1<<endl;
//        if(socket_ser->waitForReadyRead()){
//            int rec = -1;
//            socket_ser->read((char*)&rec,sizeof(int));//读取第一个数
//            //socket_ser->flush();
//            qDebug()<<rec;
//            mode(rec);//跳转相应功能
//            m_mutex.unlock();
//        }
//        }
//    }
}


