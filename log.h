#ifndef LOG_H
#define LOG_H
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QTextCodec>
#include"Control.h"

class Log
{
public:
    Log() : path(""){};
    Log(QString path1) {
        //Path.cd(User::getUser()->getName());
        QDir path2(Path);
        path2.cd("data");
        path2.cd(current_db);
        path = path2.absoluteFilePath(current_db+".log");
        //Path.cd("..");
        //QString displayString;
        QFile file(path);
        //目标文件路径
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            //delete this;
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
