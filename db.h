﻿/**
 *
 * DB控制器, 创建/删除库, 加载表, 控制表
 */
#ifndef DB_H
#define DB_H

#include "QString"
#include "QVector"
#include "boolstat.h"
#include "integrity.h"
#include "table.h"
#include "QList"

const int DBBYTE = 128 + 1 + 256 + 32;

class DB
{
private:
    char _name[128];//名字
    bool _type;//类型: true:系统数据库 false:用户数据库
    char _filePath[256];//文件路径
    char _crtime[32];//创建时间

    QList<Table*> tables;
public:

    DB(QString name, bool type, QString filePath, QString crtime);//创建数据库时调用

    DB(QString name);//读取数据库时调用

    QString getName();
    bool getType();
    QString getFilePath();


    //字段管理
    QString addColumn(QString tableName, QString columnName, TYPE type, int typeLen = 0, Integrity *integrity = nullptr);//增加列

    QString modifyColumn(QString tableName, QString columnName, TYPE type, Integrity *integrity = nullptr);//修改列类型

    QString dropColumn(QString tableName, QString columnName);//删除列


    //表管理
    QString createTable(QString tableName);//创建表

    QString dropTable(QString tableName);//删除表

    // TODO: 修改表函数的创建


    //记录管理
    QString insertRecord(QString tableName, const QVector<QString>& columnNameList, const QVector<QString>& valueList);//插入记录

    QString updateRecord(QString tableName, QString columnName, QString value, const QVector<BoolStat>& boolStats);//更新记录

    QVector<QVector<QString>> select(bool isAll, //如果查询的是*则isAll为true, 此时column_name直接传空数组即可
                   const QVector<QString>& column_names, //所有查询的列名
                   QString tableName, //表名
                   const QVector<BoolStat>& boolStats);//查询记录

    QString deleteRecord(QString tableName, const QVector<BoolStat>& boolStats);

    bool writeTable(Table* t, QString filePath);//将表描述信息写入文件
    bool readTables(QString filePath);//从文件中读取表描述信息
    bool writeTables(QString filePath);//将表描述信息覆盖原信息

    //序列化与反序列化
    int serialize(char buf[]);
    int deSerialize(char buf[]);
};

#endif // DB_H
