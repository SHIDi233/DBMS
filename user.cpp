#include "user.h"
#include "Control.h"
#include "db.h"
#include "QDateTime"
#include "QDataStream"

User::User()
{

}


QString User::createDb(QString name)
{
    // TODO: 判断权限

    if(name.size() > 127) { return "数据库名过长"; }//判断名字长度是否符合要求

    // TODO: 判断数据库是否已经存在

    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    //创建数据库路径
    QDir path(rootPath);
    if(!path.cd("data")) { path.mkdir("data"); }
    if(!path.cd(name)) { path.mkdir(name); }
    DB database(name, false, path.absolutePath(), current_date);

    // 创建表描述文件
    QFile tbFile(path.absoluteFilePath(name + ".tb"));
    if(!tbFile.open(QIODevice::WriteOnly)) { return "表描述文件创建失败"; };
    tbFile.close();

    // 创建日志文件
    QFile logFile(path.absoluteFilePath(name + ".log"));
    if(!logFile.open(QIODevice::WriteOnly)) { return "日志文件创建失败"; };
    logFile.close();

    //写入文件
    QFile dbFile(rootPath.absoluteFilePath("ruanko.db"));
    dbFile.open(QIODevice::Append);
    QDataStream dbOut(&dbFile);
    char buf[DBBYTE];
    int len = database.serialize(buf);
    dbOut.writeRawData(buf, len);

    return "数据库创建成功";
}
