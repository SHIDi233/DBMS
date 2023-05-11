#include "newserver.h"
#include"server.h"

NewServer::NewServer(qintptr handle)
{
    this->handle = handle;
    //this->socket_ser = socket_ser;

}
void NewServer::set(qintptr handle)
{
    //QTcpSocket *tcpSocket = new QTcpSocket(this);

}


void NewServer::send(QByteArray sendMsg){
    socket_ser->write(sendMsg);//发送二进制消息
}

void NewServer::send(int* sendMsg){
    socket_ser->write((char*)sendMsg,sizeof(int));//发送二进制消息
}

//void Ser::send(QByteArray){
//    socket_ser->write((char*)sendMsg,sizeof(int));//发送二进制消息
//}

void NewServer::f(){
    socket_ser->flush();
}


void NewServer::run(){
    socket_ser = new QTcpSocket();
    socket_ser->setSocketDescriptor(handle);
    QString ip=socket_ser->peerAddress().toString();
    qint16 port  =socket_ser->peerPort();
    QString temp = QString("端口是%1，ip地址是%2").arg(port).arg(ip);
    isSet=true;

    while(isOnline){
        while(isSet){
            m_mutex.lock();
            cout<<1<<endl;
        if(socket_ser->waitForReadyRead()){
            int rec = -1;
            socket_ser->read((char*)&rec,sizeof(int));//读取第一个数
            //socket_ser->flush();
            qDebug()<<rec;
            mode(rec);//跳转相应功能
            m_mutex.unlock();
        }
        }
    }
}

//模式选择
void NewServer::mode(int input){
    switch(input){
    case 0://登录
        //成功
        int a;//目标
        socket_ser->read((char*)&a,sizeof(int));//读取要发送到的账号
        cout<<"the number "<<a<<" loaded"<<endl;
        number = a;
        socket_ser->flush();
        //失败
//        isOnline=false;
        break;
    case 1://心跳
        cout<<"beat"<<endl;
        int temp;
        temp = 1;
        socket_ser->write((char*)&temp,sizeof(int));
        socket_ser->flush();
        break;
    case 2://退出登录

        break;
    case 3://发送文字(本服务器)
        send_word();
        break;
    case 4://发送图片(拉起新进程)
        send_img();
        break;
    case 5://拉起视频通话(拉起新进程)
        startVideo();
        break;
    case 6://拉起语音通话(拉起新进程)
        break;

    default:
        break;
    }
}

void NewServer::send_word(){
    int target = -1;//目标
    socket_ser->read((char*)&target,sizeof(int));//读取要发送到的账号
    qDebug()<<target;

    QByteArray sendMsg ;//内容
    sendMsg = socket_ser->readAll();//读取要发送到的信息
    qDebug()<<sendMsg;
    socket_ser->flush();

    for(int i=0;i<50;i++){
        if(Server::getInstance()->getItem(i)->number==target){
            int sender = 3;
            Server::getInstance()->getItem(i)->send(&sender);
            Server::getInstance()->getItem(i)->send(&number);//发送发送者id
            Server::getInstance()->getItem(i)->send(sendMsg);//发送信息
            Server::getInstance()->getItem(i)->f();//推送
        }
    }

}

void NewServer::send_img(){
    int target = -1;//目标
    socket_ser->read((char*)&target,sizeof(int));//读取要发送到的账号
    qDebug()<<target;
    cout<<"image"<<endl;
    QByteArray array;
        //while (socket_ser->waitForReadyRead()) {
            qDebug() << "bytesAvailable";
            array.append((QByteArray)socket_ser->readAll());
        //}

        for(int i=0;i<50;i++){
            if(Server::getInstance()->getItem(i)->number==target){
                int sender = 4;
                Server::getInstance()->getItem(i)->send(&sender);
                Server::getInstance()->getItem(i)->send(&number);//发送发送者id
                Server::getInstance()->getItem(i)->send(array);//发送信息
                Server::getInstance()->getItem(i)->f();//推送
            }
        }

}

void NewServer::startVideo(){
   int target = -1;//目标
   socket_ser->read((char*)&target,sizeof(int));//读取要发送到的账号

   //端口分配
   int PV,PA;
   int PV2,PA2;//目标用户所拥有的端口
   Server::getInstance()->getPVideo(PV);
   Server::getInstance()->getPAudio(PA);
   Server::getInstance()->getPVideo(PV2);
   Server::getInstance()->getPAudio(PA2);

   //进程拉起
   QProcess *pt = new QProcess;
   QString cmd = "videoserver.exe";//调用exe
   QStringList args;

   //加入参数
   args<<QString(QString::number(PV));
   args<<QString(QString::number(PA));
   args<<QString(QString::number(PV2));
   args<<QString(QString::number(PA2));
   pt->startDetached(cmd, args);//调用exe的同时进行传参
   pt->waitForFinished();


   //回发端口
   int s = 5;
   send(&s);
   s=0;
   send(&s);//标识拨打方
   send(&PV);
   send(&PA);
   f();//推送

   //发送通话请求
   for(int i=0;i<50;i++){
       if(Server::getInstance()->getItem(i)->number==target){
           int sender = 5;
           Server::getInstance()->getItem(i)->send(&sender);
           sender =1;
           Server::getInstance()->getItem(i)->send(&sender);//标识符
           Server::getInstance()->getItem(i)->send(&number);//发送发送者id
           Server::getInstance()->getItem(i)->send(&PV2);
           Server::getInstance()->getItem(i)->send(&PA2);
           Server::getInstance()->getItem(i)->f();//推送
       }
   }
}

void NewServer::startAudio(){
    int PA;
    Server::getInstance()->getPAudio(PA);
}
