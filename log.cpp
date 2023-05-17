#include "log.h"

void Log::insert(QString recoder,QString reverse){
    data+=recoder;
}

void Log::save(){
    if(path!=""){
        QFile file(path);
        //以文本方式打开
        if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
    //        QTextCodec *codec = QTextCodec::codecForName("utf-8");
            QTextStream out(&file); //IO设备对象的地址对其进行初始化
            out.setCodec("UTF-8");

            QByteArray arr = data.toUtf8();

            out << arr; //输出
            file.close();
         }
    }
}

