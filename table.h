/**
 *
 * 表类，表的增删改查
 */
#ifndef TABLE_H
#define TABLE_H
#include<QString>
#include"Basic_Data.h"
#include"waittodo.h"
#include<QList>
#include"row.h"
#include "integrity.h"
#include "column.h"

const int TABLEBYTE = 128 + 4 + 4 + 256 + 256 + 256 + 256 + 32 + 32;

class Table
{
private:
    char _name[128];//表格名称
    int _recordNum;//记录数
    int _fieldNum;//字段数
    char _tdf[256];//表格定义文件路径
    char _tic[256];//表格完整性文件路径
    char _trd[256];//表格记录文件路径
    char _tid[256];//表格索引文件路径

    char _crtime[32];//创建时间
    char _mtime[32];//最后修改时间


    //缓存技术-待办列表
    QList<WaitToDo_Table> doList;

public:
    Table(QString name, QString tdf, QString tic, QString trd, QString tid, QString crtime);//创建表时调用这个函数
    Table(QString name);//修改表时调用这个函数

    //对表数据进行操作
    int insert();//插入
    int insert(int,Basic_Data);//键值对方式插入(废弃)
    int insert(bool[],Basic_Data[]);//整行方式插入
    int insert(Row input);//行类插入

    int del();

    //查找
    Row* selectAll();
    Row select();

    int commit();//表的缓存提交


    //字段管理
    QString addColumn(QString columnName, TYPE type, int typeLen, int integrity);//增加列

    //文件写入
    bool writeToFile();

    //序列化与反序列化
    int serialize(char buf[]);
    int deSerialize(char buf[]);

    //获取私有参数
    QString getName();
};

#endif // TABLE_H

