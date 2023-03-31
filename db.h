/**
 *
 * DB控制器, 创建/删除库, 加载表, 控制表
 */
#ifndef DB_H
#define DB_H

#include "QString"
#include "table.h"
#include "QVector"

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

    QString createTable();//创建表
    QString dropTable();//删除表
    QString alterTable();//更新表
    Table* select(Table* table, bool isAll = false,
                   const QVector<QString*>* column_names = nullptr,
                   QString* table_names = nullptr);//查询表

};

#endif // DB_H
