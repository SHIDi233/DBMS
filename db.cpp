#include "db.h"
#include "cstring"
#include "QDir"
#include "table.h"
#include "QDateTime"

DB::DB(QString name, bool type, QString filePath, QString crtime)
{
    //赋值
    strcpy(_name, name.toLatin1().data());
    _type = type;
    strcpy(_filePath, filePath.toLatin1().data());
    strcpy(_crtime, crtime.toLatin1().data());
}

QString DB::createTable(QString tableName){

    QDir path(_filePath);

    //创建表定义文件
    QFile tdfFile(path.absoluteFilePath(tableName + ".tdf"));
    if(!tdfFile.open(QIODevice::WriteOnly)) { return "表定义文件创建失败"; };
    tdfFile.close();

    //创建记录文件
    QFile trdFile(path.absoluteFilePath(tableName + ".trd"));
    if(!trdFile.open(QIODevice::WriteOnly)) { return "记录文件创建失败"; };
    trdFile.close();

    //创建完整性描述文件
    QFile ticFile(path.absoluteFilePath(tableName + ".tic"));
    if(!ticFile.open(QIODevice::WriteOnly)) { return "完整性文件创建失败"; };
    ticFile.close();

    //创建索引描述文件
    QFile tidFile(path.absoluteFilePath(tableName + ".tid"));
    if(!tidFile.open(QIODevice::WriteOnly)) { return "索引描述文件创建失败"; };
    tidFile.close();

    //获取当前时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    //创建表类
    Table table(_name, path.absoluteFilePath(tableName + ".tdf"),
                path.absoluteFilePath(tableName + ".trd"),
                path.absoluteFilePath(tableName + ".tic"),
                path.absoluteFilePath(tableName + ".tid"), current_date);

    return "表格创建成功";
}
