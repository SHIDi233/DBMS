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
#include "boolstat.h"

const int TABLEBYTE = 128 + 4 + 4 + 256 + 256 + 256 + 256 + 32 + 32;

class DB;

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

    QVector<Column*> columns;//字段容器
    QList<Row*> rows;//记录列表
    QList<Integrity*> integrities;//约束容器
public:
    friend class DB;

    Table(QString name, QString tdf, QString tic, QString trd, QString tid, QString crtime);//创建表时调用这个函数
    Table(QString name);//修改表时调用这个函数
//    Table(char buf[]);//读取表时调用这个函数
    Table();
    ~Table();

    int del();

    //查找
    Row* selectAll();
    Row select();

    int commit();//表的缓存提交


    //字段管理
    QString addColumn(QString columnName, TYPE type, int typeLen, int integrity);//增加列

    QString dropColumn(QString columnName);//删除列

    QString modifyColumn(QString columnName, TYPE newType, int newTypeLen, int integrity);//修改列

    //数据管理
    QString insertRecord(const QVector<QString>& columnNameList, const QVector<QString>& valueList);//插入记录

    QString updateRecord(const QVector<QString>& columnNameList, const QVector<QString> valueList, QVector<BoolStat*>& boolStats);//更新记录

    QVector<QVector<QString>> select(bool isAll, //如果查询的是*则isAll为true, 此时column_name直接传空数组即可
                   const QVector<QString>& column_names, //所有查询的列名
                   QVector<BoolStat*>& boolStats);//查询记录

    QString deleteRecord(QVector<BoolStat*> &boolStats);

    //约束管理
    QString addIntegrity(QString integName, QString filed, ITGTYPE type, QString param);

    QString dropIntegrity(QString integName);

    //文件写入
    bool writeToFile();

    //序列化与反序列化
    int serialize(char buf[]);
    int deSerialize(char buf[]);

    //字段文件读写
    bool writeColumn(Column* c);
    bool writeColumns();
    bool readColumns();

    //记录文件读写
    bool readRecord();
    int getRowByte();

    //约束文件读写
    bool readInteg();

    //获取私有参数
    QString getName();
    QVector<Column*>& getColumns();
    QList<Row*>& getRows();

    //多表连接
    Table *operator+(Table *const t) {
        Table *res = new Table();
        res->columns.append(this->columns);
        res->columns.append(t->columns);
        if(this->rows.isEmpty()) {
            res->rows.append(t->rows);
        } else {
            for(auto &r1 : this->rows) {
                for(auto &r2 : t->rows) {
                    res->rows.append(r1->operator+(r2));
                }
            }
        }
        return res;
    }
};

#endif // TABLE_H

