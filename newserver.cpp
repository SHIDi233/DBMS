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

    socket_ser->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    isSet=true;
    isOnline=true;

    QString acc;
    int len_acc;
    QString pass;
    int len_pass;

    if(socket_ser->waitForReadyRead()){//接收账户密码
        socket_ser->read((char*)&len_acc,sizeof(int));
        char rec[len_acc+1];
        socket_ser->read(rec,len_acc);//读取字符串
        rec[len_acc]='\0';
        acc = QString(rec);

        socket_ser->read((char*)&len_pass,sizeof(int));
        char p[len_pass+1];
        socket_ser->read(p,len_pass);//读取字符串
        p[len_pass]='\0';
        pass = QString(p);
    }
    else{

    }

    if(User::getUser()->log(acc,pass)){
        int back = 1;
        socket_ser->write((char*)&back,sizeof(int));
        socket_ser->flush();
        qDebug()<<"有用户成功登录";
    }
    else{
        int back = 0;
        socket_ser->write((char*)&back,sizeof(int));
        socket_ser->flush();
        qDebug()<<"有用户尝试登录，失败";
    }

    socket_ser->flush();
    socket_ser->readAll();
    User::getUser()->loadDB();

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




            QString s(rec);

            if(s.contains("use")){
                s.replace(";","");
                QStringList list = s.split(" ");
                db_name=list[1];
            }

            db = User::getUser()->getDB(db_name.toUpper());

            SqlAnalysis sa(db);
            QVector<QVector<QString>> out = sa.parse_sql(s);
            socket_ser->flush();

            int vn = out.length();
            socket_ser->write((char*)&vn,sizeof(int));
            socket_ser->flush();
            //传回
            for(QVector<QString> v : out){
                //if(!socket_ser->waitForBytesWritten());
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

