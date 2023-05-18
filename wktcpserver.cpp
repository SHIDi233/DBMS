#include "wktcpserver.h"
#include"newserver.h"

WKTcpServer::WKTcpServer(QObject *parent)
    : QTcpServer{parent}
{

}

void WKTcpServer::incomingConnection(qintptr handle)
{
    NewServer *s = new NewServer(handle);
    //Server::getInstance()->addItem(s);//添加元素
    s->start();
}
