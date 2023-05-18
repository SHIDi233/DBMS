#include"wktcpserver.h"
#include "server.h"
#include<iostream>
using namespace std;

Server* Server::m_instance = NULL;
QMutex Server::m_mutex;

void Server::run(){
    isRunning = true;//线程启动

    emit sh(temp);
    cout<<"start listening..."<<endl;

    server=new WKTcpServer();
    int por = 9559;
    bool isok=server->listen(QHostAddress::Any,9559);//在QT中绑定和监听都使用的是listen函数

    if(isok)
        {
            QString str1="监听成功。。。。。";
            cout<<"listening "<<por<< " successfully"<<endl;

        }
        else
        {
            cout<<"listening false"<<endl;
        }
    while(isRunning){
        if(server->waitForNewConnection()){
            cout<<"the number "<< num <<" connecting"<<endl;
        }
    }
}

Server* Server::getInstance(){
    if(m_instance == NULL)
     {
          m_mutex.lock();
          if (m_instance == NULL)
          {
             m_instance = new Server();
          }
          m_mutex.unlock();
      }
    return m_instance;

}

Server::Server()
{
    //sers = new QVector<Ser*>;
    for(int i=0;i<50;i++){
        //sers[i] = new Ser(NULL);
    }
}

void Server::newsec(){
    emit sh("connect one");
    //配置上通信套接字

}

//Ser* Server::getItem(int i){
//    return sers[i];
//    //return NULL;
//}

//void Server::addItem(Ser* s){
//    sers[num] = s;
//    num++;
//}


