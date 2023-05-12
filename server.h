#ifndef SERVER_H
#define SERVER_H
#include<QThread>
#include<QTcpServer>
#include<QMutex>

class Server : public QThread
{
     Q_OBJECT
signals:
    void sh(QString);
public:
    static Server* getInstance();
    static QMutex m_mutex;
    static QMutex m_mutexP;
    static QMutex m_mutexV;
    static QMutex m_mutexA;
    Server();
    void run();
//    Ser* getItem(int);
//    void addItem(Ser*);
    static Server* m_instance;


public slots:
    void newsec();
private:



    //QTcpSocket *socket_ser;
    QTcpServer* server;

    QString temp;

    bool isRunning;

    //Ser* sers[50];
    //QVector<Ser*>*  sers;

    int num=0;//总用户数

};


#endif // SERVER_H
