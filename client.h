#ifndef CILENT_H
#define CILENT_H
#include<QThread>
#include<QTcpSocket>
#include<iostream>
using namespace std;
#include<QMutex>
#include<QImage>
#include<QBuffer>
#include<QPixmap>

class Client : public QThread
{
    Q_OBJECT
public:
    Client();
    void send(int target,QString data);
    void run() override;
    void clientConnect();
    void sendImg(QImage*);

    void send(QString);

    void mode(int);

    void startPic(int,QString);//发送图片
    void startVid(int,QString);//发送视频
    void startVideo(int);//开始视频通话

    bool getIsOnline(){return isOnline;};
    bool getIsTrue(){return isTrue;};
signals:
    void sh(int hisNumber,QString result);
    void sp(QPixmap);

    void stV(int ,int );

    void re(int,int);
    void se(int,QString);
    void reV(int,int);
    void seV(int,QString);
public slots:
    void send_heart();
private:
    QTcpSocket *socket_client;

    int number;
    QString pass;

    QMutex m_mutex;

    void recive_word();
    void recive_img();
    void recive_vid();

    void video();

    bool isOnline = true;
    bool isTrue = false;


    //音视频变量区
    int OtherNumber;
    int AudioPort;
    int VideoPort;

    QString FilePath;

    int port;
};

#endif // CILENT_H
