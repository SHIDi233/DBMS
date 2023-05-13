#include "newserver.h"
#include "control.h"
#include "sqlanalysis.h"

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
    isOnline=true;


    user.loadDB();

    //db = user.getDB("testDB");



    while(isOnline){
        if(socket_ser->waitForReadyRead()){
            qDebug()<<"收到信息";
            int len = -1;
            socket_ser->read((char*)&len,sizeof(int));//读取长度
            qDebug()<<len;
            char rec[len+1];
            socket_ser->read(rec,len);//读取字符串
            rec[len]='\0';
            qDebug()<<rec;

            db = user.getDB(db_name);

            QString s(rec);
            SqlAnalysis sa(db,this);
            QVector<QVector<QString>> out = sa.parse_sql(s);
            socket_ser->flush();

            int vn = out.length();
            socket_ser->write((char*)&vn,sizeof(int));
            socket_ser->flush();
            //传回
            for(QVector<QString> v : out){
                int sn = v.length();
                socket_ser->write((char*)&sn,sizeof(int));
                socket_ser->flush();
                for(QString s : v){
                    int len = s.length();
                    socket_ser->write((char*)&len,sizeof(int));
                    socket_ser->flush();
                    socket_ser->write(s.toUtf8().data(),len);
                    socket_ser->flush();
                    socket_ser->waitForBytesWritten();
                }
//            if(socket_ser->waitForBytesWritten())
//                socket_ser->flush();

            }
            QString ss = "senhjsbfidsnfjiasenfjosdnfiodsnfedsfesfnuisnfdsjikfndsjkfnedsjifesuiofhesdiofdsuiofdsiovdsuiohdsiojfaskhduiashdashduiashduiashduiasdhsauihdasuidhsauid";
            socket_ser->write(ss.toUtf8().data(),ss.length());
            socket_ser->flush();

        }
    }
}

//void NewServer::send_int(){
//    while(1){
//        if(socket_ser->waitForReadyRead()){

//        }
//    }
//}

