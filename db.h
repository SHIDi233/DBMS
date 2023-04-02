/**
 *
 * DB控制器, 创建/删除库, 加载表, 控制表
 */
#ifndef DB_H
#define DB_H

#include "QString"
#include "table.h"
#include "QVector"
#include "boolstat.h"

class DB
{
private:
    char _name[128];//名字
    bool _type;//类型: true:系统数据库 false:用户数据库
    char _file_path[265];//文件路径
    QString _crtime;//创建时间
public:
    DB();

    const char* getName();
    bool getType();
    const char* getFilePath();

    QString createTable(QString tableName);//创建表
    QString dropTable(QString tableName);//删除表
    Table* select(bool isAll, //如果查询的是*则isAll为true, 此时column_name直接传空数组即可
                   const QVector<QString>& column_names, //所有查询的列名
                   QString table_names, //表名
                   const QVector<BoolStat>& boolStats);//查询表
};

#endif // DB_H
