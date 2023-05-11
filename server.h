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


    //网络端口资源配置相关
    bool SPics[1000]={false};//图片等文件发送相关
    int maxSPices = 0;
    bool PVideo[1000]={false};//通话视频通道
    int maxPVideo = 0;
    bool PAudio[1000]={false};//通话音频通道
    int maxPAudio = 0;
};


#endif // SERVER_H
