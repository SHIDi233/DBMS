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
    Client(QString& ip,QString& acc,QString& pass);
    void run() override;

signals:
    void back(QVector<QVector<QString>>);
public slots:
    void send_heart();
    void send(QString);
private:
    QTcpSocket *socket_client;
    bool isOnline = false;

    int receive_int();
    QString receive_QS(int);

    QString ip;
    QString acc;
    QString pass;

};

#endif // CILENT_H
