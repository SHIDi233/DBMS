#include "client.h"
#include<QTimer>
#include<QProcess>
#include<QDir>
#include"mainwindow.h"
#include<stdlib.h>

Client::Client(){

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
                send("test");
//                //t->start();
                break;
            }

        MainWindow* mw = new MainWindow;
        mw->show();



        }
        while(isOnline){
//            m_mutex.lock();
//            cout<<1<<endl;
            if(socket_client->waitForReadyRead()){
            int len = -1;
            socket_client->read((char*)&len,sizeof(int));//读取长度
            QString s;
            socket_client->read((char*)&s,len);//读取字符串
            qDebug()<<s;
            //mode(rec);//跳转相应功能
//            m_mutex.unlock();
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

void Client::send(int target,QString data){
    int t = 3;//目标
    socket_client->write((char*)&t,sizeof(int));//标识符
    //target = 233;//目标
    socket_client->write((char*)&target,sizeof(int));
    //socket_client->flush();
    //QString mystring= "test from cilent";
    socket_client->write(data.toUtf8().data());
    socket_client->flush();
    qDebug()<<"发出";
}



void Client::send_heart(){
    int temp =1;
    qDebug()<<"发送心跳";
    socket_client->write((char*)&temp,sizeof(int));//发送心跳
    socket_client->flush();
}
