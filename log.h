#ifndef LOG_H
#define LOG_H
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QTextCodec>

class Log
{
public:
    Log() : path(""){};
    Log(QString path) : path(path){
        QString displayString;
        QFile file(path);
        //目标文件路径
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            delete this;
            return;
        }
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QByteArray arr = file.readAll();
        data = codec->toUnicode(arr);
        file.close();
    };

    void insert(QString recoder,QString reverse);
    void save();

    QString getAll(){
        return data;
    }
private:
    QString data="";
    QString path;
};

#endif // LOG_H
