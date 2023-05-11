#ifndef WKTCPSERVER_H
#define WKTCPSERVER_H

#include <QTcpServer>

class WKTcpServer : public QTcpServer
{
public:
    explicit WKTcpServer(QObject *parent = nullptr);
    void incomingConnection(qintptr handle);
};

#endif // WKTCPSERVER_H
